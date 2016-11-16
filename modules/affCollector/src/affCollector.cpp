/*
 * TOOL 3D FEATURE EXTRACTOR
 * Copyright (C) 2015 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Tanis Mar
 * email: tanis.mar@iit.it
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

#include <affCollector.h>

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::math;

// This module saves and updates learned affordances (as action success rates) for a set of user defined labels


/************************* RF overwrites ********************************/
/************************************************************************/
bool  AffCollector::configure(ResourceFinder &rf)
{
    // add and initialize the port to send out the features via thrift.
    string name = rf.check("name",Value("affCollector")).asString().c_str();
    filename = rf.check("filename",Value("affFile.txt")).asString().c_str();
    rf.setDefaultContext("AffordancesProject");

    string contextPath = "/share/ICUBcontrib/contexts/AffordancesProject/";
    string icubContribEnvPath = yarp::os::getenv("ICUBcontrib_DIR");
    filepath  = icubContribEnvPath + contextPath;

    verb = rf.check("verbose",Value(true)).asBool();
    numAct = rf.check("numAct",Value(6)).asInt();
    binAff = rf.check("binary",Value(false)).asBool();

    act_labels.resize(numAct,"act");              //initialize with #numact empty strings

    //open ports
    bool ret = true;
    ret = ret && affInPort.open(("/"+name+"/aff:i").c_str());     // Receives pairs of action-effect information to update affordance information
    ret =  ret && labelsInPort.open("/"+name+"/labels:o");		  // Receives labels for affordances
    ret = ret && affOutPort.open(("/"+name+"/aff:o").c_str());    // Streams the success rate (affordances) of the active label
    if (!ret){
        printf("Problems opening ports\n");
        return false;
    }

    // open rpc ports
    bool retRPC = true;
    retRPC =  retRPC && rpcInPort.open("/"+name+"/rpc:i");
    if (!retRPC){
        printf("Problems opening ports\n");
        return false;
    }
    attach(rpcInPort);


    /* Module rpc parameters */
    closing = false;
    forgetFlag = true;

    /*Init variables*/
    activeLabel = -1;
    cout << endl << "Configuring done."<<endl;


    // If a file has been found, load its contents
    string filefound =rf.findFile(filename.c_str());
    if (filefound !=""){
        cout << "File found at " << filefound << endl;
        readfile(filename);
    }else{
        cout << "File not found at " << filefound << endl;
        cout << "No previous memory file found. Initializing values. " << endl;
    }


    cout << "Memory file location on PATH: " << filepath << endl;

    return true;
}

bool  AffCollector::updateModule()
{
    // read if there is any new affordance data. If so, update aff for active label
    Bottle *affBottle = affInPort.read(false);
    if (affBottle!=NULL){
        int act = affBottle->get(0).asInt();
        double eff = affBottle->get(1).asDouble();
        updateAff(act, eff);
    }

    // read if there is any new label.
    Bottle *labelBottle = labelsInPort.read(false);
    if (labelBottle!=NULL){
        string label = labelBottle->get(0).asString();
        setlabel(label);
    }

    // Stream affordances of active label
    if (activeLabel >=0) {
        Bottle &affBotOut = affOutPort.prepare();
        affBotOut.clear();
        affBotOut = getAffs();
        affOutPort.write();
    }

    return !closing;
}


double  AffCollector::getPeriod()
{
    return 0.2; //module periodicity (seconds)
}


bool  AffCollector::interruptModule()
{
    closing = true;
    affInPort.interrupt();
    affOutPort.interrupt();
    labelsInPort.interrupt();
    cout<<"Interrupting your module, for port cleanup"<<endl;
    return true;
}


bool  AffCollector::close()
{
    cout<<"Calling close function\n";
    affInPort.close();
    affOutPort.close();
    labelsInPort.close();
    return true;
}

/**************************** THRIFT CONTROL*********************************/
bool  AffCollector::attach(RpcServer &source)
{
    return this->yarp().attachAsServer(source);
}

/**********************************************************
                    PUBLIC METHODS
/**********************************************************/

// RPC Accesible via trhift.
/**********************************************************/
bool AffCollector::setnumact(const int num)
{
    if (num < 1 ) {
        return false;
    }

    numAct = num;
    return true;
}

bool AffCollector::setactlabels(const Bottle labels)
{
    numAct = labels.size();
    act_labels.clear();
    for (int i = 0; i < numAct ; i++){
        string act_label = labels.get(i).asString();
        act_labels.push_back(act_label);
    }
    return true;
}


int AffCollector::setlabel(const std::string& label)
{
    // Look for label in known labels vector.
    vector<string>::iterator it = std::find(knownLabels.begin(), knownLabels.end(), label);

    // not found -> add label structure for konwnLabels, knownAffs and affHist
    if ( it == knownLabels.end() ) {

        cout << "Label not found, initializing elements for " << label << endl;

        knownLabels.push_back(label);           // Add label to vector of known labels

        vector<double> affInit(numAct,-1.0);
        knownAffs.push_back(affInit);           // Add aff Vector corresponding to that label

        vector<vector<double> > affHistInit(numAct,vector<double>(0));
        affHist.push_back(affHistInit);         // Init eff Vector history for each action on that label

        activeLabel = knownLabels.size()-1;     // return position of newly add element

    }else {  // found  -> set label as active label
        activeLabel = std::distance(knownLabels.begin(), it); // return position of found element
        cout << "Label found, with index " << activeLabel << endl;
    }

    return activeLabel;
}

/**********************************************************/
string AffCollector::getlabel()
{
    if (activeLabel >= 0){
        return knownLabels[activeLabel];
    }

    return "empty";
}


/**********************************************************/
double AffCollector::updateAff(const int act, const double eff, const int labI)
{

    //if (labI > knownLabels.size()){
    //    cout << "label with index " << labI << " does not exist" << endl;
    //    return -1;
    // }
    int label;
    if (labI < 0){
        label = activeLabel;
    }else {
        label = labI;
    }

    if (act > numAct-1){
        cout << "action index out of limits. (Max = " << numAct -1 << ")." << endl;
        return -1;
    }

    // If the affordances are binary, round to 0 or 1
    if (binAff){
        double effBin;
        if (eff >= 0.5)
            effBin = 1.0;
        if (eff < 0.5)
            effBin = 0.0;

        // Add new effect to history of that action and label
        affHist[label][act].push_back(effBin);
    }else{
        // Add new effect to history of that action and label
        affHist[label][act].push_back(eff);
    }

    // Update corresponding success rate
    double succRate  = vecAvg(affHist[label][act]);      // XXX Eventually do sth better than avg (decay, forget,etc)
    knownAffs[label][act] = succRate;

    cout << "Updated Aff of label '" << knownLabels[label] << "', and action " << act << " to " << succRate << endl;

    return succRate;
}

/**********************************************************/
Bottle  AffCollector::getAffs(const std::string& label)
{
    Bottle affs;
    // Check the case where its called before any affordances are learnt
    if (knownAffs.size() <1){
        cout << "No affordance have been learnt yet" << endl;
        return affs;
    }

    //if param 'all'return all known affordances in a concatenated vector
    if (label == "all"){        
        for (int l =0; l < knownLabels.size(); l++){
            cout << "LABEL: " << knownLabels[l] << endl << "Affordances: ";
            affs.addString(knownLabels[l]);
            Bottle& affLab = affs.addList();
            for (int a =0; a < knownAffs[l].size(); a++){
                affLab.addDouble(knownAffs[l][a]);
                cout << knownAffs[l][a]<< " ";
            }
            cout << " " << endl;
        }
        return affs;
    }

    // Find the label (active or given)
    int labI;
    if (label == "active"){
        labI = activeLabel;
    }else {
        vector<string>::iterator it = std::find(knownLabels.begin(), knownLabels.end(), label);
        if ( it == knownLabels.end() ) { //Not found
            cout << "Given label not found" << endl;
            return affs;
        }else{                          // Found
            labI = std::distance(knownLabels.begin(), it);                  // return position of found element
        }
    }

    // Return affordances of given tool-pose label
    if (labI < 0)       // No tool has been selected yet
    {
        affs.addString("no_aff");
    }else{
        affs.addString(knownLabels[labI]);

        /*
        Bottle& affLab = affs.addList();
        for (int a =0; a < knownAffs[labI].size(); a++){
            affLab.addDouble(knownAffs[labI][a]);
        }
        */

        Property &affProps = affs.addDict();
        for (int a =0; a < knownAffs[labI].size(); a++){
            affProps.put(act_labels[a],knownAffs[labI][a]);
        }

    }
    return affs;
}



/**********************************************************/
Bottle  AffCollector::getAffHist(const std::string& label, const int act )
{
    Bottle history;
    if (act >= numAct){
        cout << "Action index over number of available actions. Try 0 - " << numAct-1 << endl;
        return history;
    }
    // Check the case where its called before any affordances are learnt
    if (activeLabel <0){
        cout << "No effect observed yet. " << endl;
        return history;
    }

    //if label 'all'return history of all toolposes used
    if (label == "all"){
        for (int l=0; l<knownLabels.size();l++){      // along labels
            history.addString(knownLabels[l]);
            cout << knownLabels[l] <<endl;
            Bottle& histLab = history.addList();
            histLab.clear();
            for (int a =0; a < affHist[l].size(); a++){
                Bottle& histAct = histLab.addList();
                histAct.clear();
                for (int e =0; e < affHist[l][a].size(); e++){
                    histAct.addDouble(affHist[l][a][e]);
                    cout << affHist[l][a][e] << " ";
                }
                cout << endl;
            }
        }
    }

    // Find the label
    int labI =-1;
    if (label == "active"){        
        labI = activeLabel;
    }else {
        vector<string>::iterator it = std::find(knownLabels.begin(), knownLabels.end(), label);
        if (it == knownLabels.end()){
            return history;
        }else{
            labI = std::distance(knownLabels.begin(), it);                  // return position of found element
        }
    }

    cout << knownLabels[labI] << endl;

    // Fill the bottle
    history.addString(knownLabels[labI]);
    if (act < 0){           // act < 0 means all actions in given label
        Bottle& histLab = history.addList();
        for (int a =0; a < affHist[labI].size(); a++){
            Bottle& histAct = histLab.addList();
            histAct.clear();
            for (int e =0; e < affHist[labI][a].size(); e++){
                histAct.addDouble(affHist[labI][a][e]);
                cout << affHist[labI][a][e] << " ";
            }
            cout << endl;
        }
    }else{
        cout << "Action " << act <<  endl;
        Bottle& histLab = history.addList();
        for (int e =0; e < affHist[labI][act].size(); e++){
            histLab.addDouble(affHist[labI][act][e]);
            cout << affHist[labI][act][e] << " ";
        }
        cout << endl;
    }


    return history;

}


/**********************************************************/
string AffCollector::selectTool(const int act)
{
    // Check the case where its called before any affordances are learnt
    if (knownAffs.size() <1){
        cout << "No afordance have been learnt yet" << endl;
        return "no_tool";
    }

    //Find the tool label with higher success rate for particular action
    int bestToolInd = -1;
    double bestRate = 0.0;
    for (int l =0; l < knownAffs.size(); l < l++)
    {
        double succRate = knownAffs[l][act];
        if (succRate > bestRate){
            bestToolInd = l;
            bestRate = succRate;
        }
    }

    return knownLabels[bestToolInd];
}

/**********************************************************/
string AffCollector::activeExp(const std::string& label)        // returns name of toolpose_N, N being maxVar Action
{
    // Check the case where its called before any affordances are learnt
    if (knownAffs.size() <1){
        cout << "No afordance have been learnt yet" << endl;
        return "noAff";
    }

    double maxVar = 0.0;
    double bestExpAction = -1;
    int bestLab = -1;

    if (label == "all"){
        for (int l =0; l < knownAffsVar.size(); l < l++)
        {
            for (int a =0; a < knownAffsVar[l].size(); a < a++)
            {
                double variance = knownAffsVar[l][a];
                if (variance > maxVar){
                    bestLab = l;
                    bestExpAction = a;
                    maxVar = variance;
                }
            }
        }

        if (bestLab < 0) {
            cout << "Label not found. " << endl;
            return "noAff";
        }

        cout << "ToolPose " << knownLabels[bestLab]<< endl;
        cout << "Action " << bestExpAction << " is the less certain, std =  " << sqrt(maxVar) << endl;

        stringstream s;
        s.str("");
        s << knownLabels[bestLab] << "_" << bestExpAction;
        string tool_act = s.str();

        return tool_act;
    }

    if (label == "active"){
        bestLab = activeLabel;
    }else {
        vector<string>::iterator it = std::find(knownLabels.begin(), knownLabels.end(), label);
        bestLab = std::distance(knownLabels.begin(), it);                  // return position of found element
    }
    if (bestLab < 0) {
        cout << "Label not found. Couldnt reset" << endl;
        return "noAff";
    }

    cout << "ToolPose " << knownLabels[bestLab]<< endl;
    cout << "Stds are " << endl;
    for (int a =0; a < knownAffsVar[bestLab].size(); a < a++)
    {
        double variance = knownAffsVar[bestLab][a];
        cout << sqrt(variance) << endl;

        if (variance > maxVar){
            bestExpAction = a;
            maxVar = variance;
        }
    }

    cout << "Action " << bestExpAction << " is the less certain, std =  " << sqrt(maxVar) << endl;

    stringstream s;
    s.str("");
    s << knownLabels[bestLab] << "_" << bestExpAction;
    string tool_act = s.str();

    return tool_act;

}

/**********************************************************/

string AffCollector::balanceExp(const std::string& label)        // returns name of toolpose_N, N being maxVar Action
{
    // Check the case where its called before any affordances are learnt
    if (knownAffs.size() <1){
        cout << "No afordance have been learnt yet" << endl;
        return "noAff";
    }

    int minTrials = 1000;
    double bestExpAction = -1;
    int bestLab = -1;

    if (label == "all"){
        for (int l =0; l < knownAffs.size(); l < l++)
        {
            cout << "ToolPose " << knownLabels[l]<< endl;
            for (int a =0; a < knownAffs[l].size(); a < a++)
            {
                int trialNum = affHist[l][a].size();
                cout << trialNum << endl;
                if (trialNum < minTrials){
                    bestLab = l;
                    bestExpAction = a;
                    minTrials = trialNum;
                }
            }
        }

        if (bestLab < 0) {
            cout << "Label not found. " << endl;
            return "noAff";
        }

        cout << "ToolPose " << knownLabels[bestLab]<< endl;
        cout << "Action " << bestExpAction << " has been the least tried " << minTrials << " times."<< endl;

        stringstream s;
        s.str("");
        s << knownLabels[bestLab] << "_" << bestExpAction;
        string tool_act = s.str();

        return tool_act;
    }

    if (label == "active"){
        bestLab = activeLabel;
    }else {
        vector<string>::iterator it = std::find(knownLabels.begin(), knownLabels.end(), label);
        bestLab = std::distance(knownLabels.begin(), it);                  // return position of found element
    }
    if (bestLab < 0) {
        cout << "Label not found. Couldnt reset" << endl;
        return "noAff";
    }

    cout << "ToolPose " << knownLabels[bestLab]<< endl;
    cout << "Trial nums have been " << endl;
    for (int a =0; a < knownAffs[bestLab].size(); a < a++)
    {
        int trialNum = affHist[bestLab][a].size();
        cout << trialNum << endl;
        if (trialNum < minTrials){
            bestExpAction = a;
            minTrials = trialNum;
        }
    }

    cout << "Action " << bestExpAction << " has been the least tried " << minTrials << " times."<< endl;

    stringstream s;
    s.str("");
    s << knownLabels[bestLab] << "_" << bestExpAction;
    string tool_act = s.str();

    return tool_act;

}



/**********************************************************/
bool AffCollector::reset(const std::string& label)
{
    // Check the case where its called before any affordances are learnt
    if (activeLabel <0){
        cout << "No afordance have been learnt yet" << endl;
        return false;
    }

    // Find the label
    int labI = -1;
    if (label == "active"){
        labI = activeLabel;
    }else {
        vector<string>::iterator it = std::find(knownLabels.begin(), knownLabels.end(), label);
        labI = std::distance(knownLabels.begin(), it);                  // return position of found element
    }
    if (labI < 0) {
        cout << "Label not found. Couldnt reset" << endl;
        return false;
    }


    for (int a = 0; a <numAct; a++){
        knownAffs[labI][a] = -1;
    }
    cout << "Affordance knowledge of label '" << knownLabels[labI] << "' forgotten (set to -1)" << endl;

    return true;
}

/**********************************************************/
bool AffCollector::clearAll()
{
    knownLabels.clear();
    knownAffs.clear();
    affHist.clear();
    activeLabel = -1;
    return true;
}


/**********************************************************/
string AffCollector::forgetAll()
{
    if (forgetFlag == false){
        knownLabels.clear();
        knownAffs.clear();
        affHist.clear();
        activeLabel = -1;
        savetofile(filename);       //overwrite all the information with empty
        return "All has been forgotten";
    } else {
        cout << "WARNING" << endl;
        cout << "This will destroy and delete all the previous learned information, also from the file." << endl;
        forgetFlag = false;
        return "WARNING! Irreversible action: Call command again command for confirmation";
    }


}


/**********************************************************/
bool AffCollector::savetofile(const string& fileN)
{
    // Put file in the proper context path
    /*
    ofstream fileLabels;
    fileLabels.open((filepath  + filename).c_str());
    ostream_iterator<string> fileLabels_it( fileLabels, "\n" );
    copy( knownLabels.begin( ), knownLabels.end( ), fileLabels_it );
    //fileLabels << knownLabels;
    fileLabels.close();
    */
    string file_name;
    if (fileN == "default"){
        cout << "No file given, using default: " << filename << endl;
        file_name = filename;
    }else{
        cout << "Using given name: " << fileN << endl;
        file_name = fileN;
    }

    ofstream fileHist;
    fileHist.open ((filepath  + file_name).c_str());
    for (int l=0; l<knownLabels.size();l++){      // along labels
        fileHist << knownLabels[l];
        fileHist << "\n";
        for (int a=0; a<numAct;a++){
            for (int e=0; e<affHist[l][a].size();e++){
                fileHist << affHist[l][a][e];
                fileHist << " ";
            }
            fileHist << "\n";
        }
    }
    fileHist.close();

    cout << "Labels saved to file " << (filepath  + file_name) << endl;
    return true;
}


/**********************************************************/
bool AffCollector::readfile(const std::string& fileN)
{
    string file_name;
    if (fileN == "default"){
        cout << "No file given, using default: " << filename << endl;
        file_name = filename;
    }else{
        cout << "Using given name: " << fileN << endl;
        file_name = fileN;
    }

    cout << "Reading data from file: " << filepath +file_name << endl;

    // Read the affordance history
    ifstream fileHist((filepath + file_name).c_str());
    std::string line;

    int cnt = 0;
    affHist.clear();
    knownLabels.clear();
    while (getline(fileHist, line))          //read line by line (action-wise)
    {
        if(fileHist.eof()){
            std::cout << "EOF\n";
            break;
        }

        //cout << "Read line : " << line << endl;
        if (cnt == 0){                        // Read the name first
            knownLabels.push_back(line);
            affHist.push_back(vector<vector<double> >(0));      // Initialize the matrix (vec<vec>>) for first action data
            cnt++;
            continue;
        }

        vector<double> actVec;
        std::istringstream iss(line);
        double read_eff;
        while (iss >> read_eff){
            actVec.push_back(read_eff);
        }
        if (actVec.size() == 0){
            actVec.push_back(-1.0);
        }

        affHist.back().push_back(actVec);

        //cout << " - Saved #effects: " << actVec.size() <<endl;

        cnt++;
        if (cnt == numAct+1){ // Read hist of all action repertoire -> next label
            cnt = 0;            
        }
    }
    cout << "Read data from " << affHist.size() << " labels" <<endl;

    knownAffs.clear();
    knownAffsVar.clear();
    compRateFromHist(affHist,knownAffs);
    compVarFromHist(affHist,knownAffsVar);

    activeLabel = knownLabels.size()-1;
    return true;
}

/**********************************************************/
bool AffCollector::quit()
{
    closing = true;
    return true;
}


/**********************************************************
                    PRIVATE METHODS
/**********************************************************/

/***************** Helper Functions *************************************/
double AffCollector::vecAvg (const vector<double>& vec )
{
    double return_value = 0.0;
    int n = vec.size();
    int bad_i = 0;

    for ( int i=0; i < n; i++)
    {
        if (vec[i] < -0.01) {       // Do not include negative values (unknown or wrong samples)
            bad_i++;
        }else{
            return_value += vec[i];
        }
    }

    return ( return_value / (n-bad_i));
}

double AffCollector::vecVar (const vector<double>& vec)
{
    double mean = vecAvg(vec);
    double sum = 0.0;
    int bad_i = 0;
    int n = vec.size();

    for ( int j = 0; j < n; j++)
    {
        if (vec[j] < -0.01) {       // Do not include negative values (unknown or wrong samples)
            bad_i++;
        }else{
            sum += (vec[j] - mean)*(vec[j] - mean);
        }
    }

    return sum/(n - bad_i);


}

bool  AffCollector::compRateFromHist(const std::vector<std::vector<std::vector<double> > >& hist, std::vector<std::vector<double> >& affs)
{
    for(int l = 0; l<hist.size(); l++)
    {
        vector<double> affInit(numAct,-1.0);
        affs.push_back(affInit);           // Add aff Vector corresponding to that label

        for(int a= 0; a<numAct; a++){
            affs[l][a] = vecAvg(hist[l][a]);
        }
    }
}

bool  AffCollector::compVarFromHist(const std::vector<std::vector<std::vector<double> > >& hist, std::vector<std::vector<double> >& affsVar)
{
    for(int l = 0; l<hist.size(); l++)
    {
        vector<double> affInit(numAct,-1.0);
        affsVar.push_back(affInit);           // Add aff Vector corresponding to that label

        for(int a= 0; a<numAct; a++){
            affsVar[l][a] = vecVar(hist[l][a]);
        }
    }
}



/************************************************************************/
/************************************************************************/
int main(int argc, char * argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
    {
        printf("YARP server not available!\n");
        return -1;
    }

    AffCollector module;
    ResourceFinder rf;
    rf.setDefaultContext("AffordancesProject");
    rf.setDefaultConfigFile("affCollector.ini");
    rf.setVerbose(true);
    rf.configure(argc, argv);

    cout<<"Configure and Start module..."<<endl;
    return module.runModule(rf);

}

