/*
 * AFFORNDACE MANAGER
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
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

#include "tool3DManager.h"

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::math;
using namespace yarp::dev;

// XXX XXX Remove disp from all the functions using it.
// XXX Remove checks for "invalid" tools

/**********************************************************
					PUBLIC METHODS
/**********************************************************/

/******************* RF overwrites ************************/
bool Tool3DManager::configure(ResourceFinder &rf)
{
    printf("Configuring module...\n");

    name = rf.check("name",Value("tool3DManager")).asString().c_str();
    camera = rf.check("camera",Value("left")).asString().c_str();
    hand = rf.check("hand",Value("right")).asString().c_str();
    robot = rf.check("robot",Value("icub")).asString().c_str();

    cout << "Configuring module name " << name << ", camera " << camera << ", hand "<< hand << ", on robot " << robot << endl;

    if (robot=="icubSim"){
        cout << "Configuring for robot icubSim" << endl;
        tableHeight = rf.check("tableHeight", Value(-0.13)).asDouble();      // Height of the table in the robots coord frame
    }else{
        cout << "Configuring for robot real iCub" << endl;
        tableHeight = rf.check("tableHeight", Value(-0.12)).asDouble();      // Height of the table in the robots coord frame
    }
    

    // Attach server port to read RPC commands via thrift
    attach(rpcCmd);

    // Read and load the tool's models
    Bottle temp;
    string modelName = "obj";

    cout << "Loading models to buffer" << endl;
    bool noMoreModels = false;
    int n =0;
    while (!noMoreModels){      // read until there are no more objects.
        stringstream s;
        s.str("");
        s << modelName << n;
        string objNameNum = s.str();
        temp = rf.findGroup("objects").findGroup(objNameNum);

        if(!temp.isNull()) {
            cout << "model " << n << ", id: " << objNameNum;            
            string modelx = temp.get(2).asString();         // retrieve model name with format
            string::size_type idx;                          // remove format (.x) to save only tool name
            idx = modelx.rfind('.');                        // ..
            string model = modelx.substr(0,idx);            // format removed.
            cout << ", model: " << model << endl;

            models.push_back(model);
            temp.clear();
            n++;
        }else {
            noMoreModels = true;
        }
    }
    int numberObjs = n;
    cout << "Loaded " << numberObjs << " objects. " << endl;


    // Module flow parameters
    running = true;
    toolLoadedIdx = -1;
    trackingObj = false;
    toolname = "";
    seg2D = true;

    // Initialize effect measuring vectors
    target3DcoordsIni.resize(3, 0.0);
    target3DcoordsAfter.resize(3, 0.0);
    target3DrotIni.resize(3, 0.0);
    target3DrotAfter.resize(3, 0.0);
    effectVec.resize(3, 0.0);

	//ports
	bool ret = true;  
    ret = ret && matlabPort.open(("/"+name+"/matlab:i").c_str());                     // port to receive data from MATLAB processing
    ret = ret && trackPort.open(("/"+name+"/track:i").c_str());                     // port to receive data from MATLAB processing
    ret = ret && affDataPort.open(("/"+name+"/affData:o").c_str());                   // port to send data of computed affordances out for recording

    if (!ret){
		printf("Problems opening ports\n");
		return false;
	}

    //rpc
    bool retRPC = true; 
    retRPC = rpcCmd.open(("/"+name+"/rpc:i").c_str());					   			   // rpc in client to receive commands
    retRPC = retRPC && rpcMotorAre.open(("/"+name+"/are:rpc").c_str());                // rpc server to query ARE
    retRPC = retRPC && rpcSimToolLoader.open(("/"+name+"/simToolLoader:rpc").c_str()); // rpc server to query the simtooloader module
    retRPC = retRPC && rpcSimulator.open(("/"+name+"/simulator:rpc").c_str());         // rpc server to query the simulator
    retRPC = retRPC && rpcAffMotor.open(("/"+name+"/affMotor:rpc").c_str());           // rpc server to query Affordance Motor Module
    retRPC = retRPC && rpc3Dexp.open(("/"+name+"/obj3Dexp:rpc").c_str());              // rpc server to query objects3DExplorer module
    retRPC = retRPC && rpcObjFinder.open(("/"+name+"/objFind:rpc").c_str());           // rpc server to query objectFinder

	if (!retRPC){
		printf("Problems opening rpc ports\n");
		return false;
	}

    printf("Manager configured correctly \n");

	return true;
}


/**********************************************************/
bool Tool3DManager::updateModule()
{
	if (!running)
        return false;
	return true;
}

/**********************************************************/
bool Tool3DManager::interruptModule()
{    
    affDataPort.interrupt();    
    matlabPort.interrupt();
    trackPort.interrupt();

    rpcCmd.interrupt();
    rpcSimToolLoader.interrupt();
    rpcSimulator.interrupt();
    rpcMotorAre.interrupt();
    rpcAffMotor.interrupt();
    rpcObjFinder.interrupt();

    return true;
}

/**********************************************************/
bool Tool3DManager::close()
{
    affDataPort.close();
    matlabPort.close();
    trackPort.close();

    rpcCmd.close();
    rpcSimToolLoader.close();
    rpcSimulator.close();
    rpcMotorAre.close();
    rpcAffMotor.close();
    rpcObjFinder.close();

	running = false;

    return true;
}

/**********************************************************/
double Tool3DManager::getPeriod()
{
    return 0.1;
}

/***********************************************************/
/****************** RPC HANDLING METHODS *******************/

/* Atomic commands */
/***********************************************************************************/
// ======================== Module control commands
bool Tool3DManager::attach(yarp::os::RpcServer &source)
{
    return this->yarp().attachAsServer(source);
}

bool Tool3DManager::start(){
    cout << "Starting!" << endl;
	running = true;
	return true;
}

bool Tool3DManager::quit(){
    cout << "Quitting!" << endl;
	running = false;
	return true;
}

/***********************************************************************************/
// ======================== Tool Load and Info Extraction

bool Tool3DManager::setToolName(const string &tool){
    toolname = tool;
    bool ok = load3Dmodel(tool);
    return ok;
}

bool Tool3DManager::getToolByPose(const string &tool, double deg, double disp, double tilt, double shift){
    bool ok;
    if (robot=="icubSim"){
        toolname = tool;
        cout << "Loading tool " << tool << " in simulation" << endl;
        ok = loadToolSim(tool, deg, disp, tilt);
    }else{
        ok = loadToolPose(tool, deg, disp, tilt, shift);
    }
    return ok;
}

bool Tool3DManager::getTool(const string &tool){
    bool ok;
    if (robot=="icubSim"){
         cout << "Grasp params need to be given to grasp on simulator. Try getToolByPose." << endl;
    }else{
        toolname = tool;
        ok = getToolExe(tool);
    }
    return ok;
}

bool Tool3DManager::graspTool(){
    bool ok;
    if (robot=="icubSim"){
        cout << "Tool in sim is loaded, not grasped." << endl;
        ok = false;
    }else{
        ok = graspToolExe();
    }
    return ok;
}

bool Tool3DManager::explore(){
    bool ok;
    ok = exploreTool(tooltip);

    cout <<  "Tool loaded and pose and tooltip found at (" <<tooltip.x <<", " << tooltip.y << "," <<tooltip.z <<  ") !" << endl;

    ok = addToolTip(tooltip);
    if (!ok){
        cout << "Tool tip could not be attached." << endl;
        return false;
    }
    cout <<  "Tooltip attached, ready to perform action!" << endl;
    return ok;
}

bool Tool3DManager::lookTool(){
    bool ok;
    ok = lookToolExe();
    return ok;
}

bool Tool3DManager::regrasp(double deg, double disp, double tilt, double Z){
    bool ok;
    if (toolLoadedIdx<0){
        cout << "A tool has to be loaded before regrasping is possible" << endl;
        return false;
    }

    ok = regraspExe(tooltip, deg, disp, tilt, Z);
    return ok;
}

bool Tool3DManager::findPose(){
    bool ok;
    if (robot=="icubSim"){
        cout << "Grasp in sim corresponds surely with given one." << endl;
        return false;
    }
    ok = findPoseExe(toolname, tooltip);
    if (!ok){
        cout << "Tool Pose could not be estimated properly" << endl;
        return false;
    }
    cout <<  "Tool loaded and pose and tooltip found at (" <<tooltip.x <<", " << tooltip.y << "," <<tooltip.z <<  ") !" << endl;

    ok = addToolTip(tooltip);
    if (!ok){
        cout << "Tool tip could not be attached." << endl;
        return false;
    }
    cout <<  "Tooltip attached, ready to perform action!" << endl;


    return true;
}

bool Tool3DManager::getToolFeats(){
    extractFeats();
    return true;
}

/***********************************************************************************/
// ======================== Functions to call actions - compute effects

bool Tool3DManager::goHome(bool hands){
    goHomeExe(hands);
    return true;
}

bool Tool3DManager::findTable(bool calib){

    double th = findTableHeight(calib);
    tableHeight = th;
    cout << "Setting table height to " << th << endl;
    return true;
}

bool Tool3DManager::slide(double theta, double radius){
    return slideExe(theta,radius);
}

bool Tool3DManager::drag3D(double x, double y, double z, double theta, double radius, double tilt, bool useTool){
    return drag3DExe(x, y, z, theta, radius, tilt, useTool);
}

bool Tool3DManager::drag(double theta, double radius, double tilt){
    return dragExe(theta,radius, tilt);
}

bool Tool3DManager::trackObj(){
    return trackObjExe();
}

bool Tool3DManager::compEff(){
    return computeEffect();
}

bool Tool3DManager::resetObj(){
    return resetCube();
}

/***********************************************************************************/
//  ======================== Functions to get object info:
Vector Tool3DManager::objLoc(){
    Vector loc;
    loc.clear();         // Clear to make space for new rotation values
    loc.resize(3);
    getObjLoc(loc);

    return loc;
}

Vector Tool3DManager::objRot(){
    Vector rot;
    rot.clear();         // Clear to make space for new rotation values
    rot.resize(3);
    getObjRot(rot);

    return rot;
}


/***********************************************************************************/
//  ======================== Exploration commands  =======================
bool Tool3DManager::runRandPoses(int numPoses,int numAct){
    double thetaDiv = 360.0/numAct;
    double theta = 0.0;
    Rand randG; // YARP random generator

    int tilt = 0;
    if (!(robot=="icubSim")){
        tilt = -15; // Action on the real robot need some tilt to not crash teh hand on the table
    }


    int toolI_prev = round(randG.scalar(1,52));
    for (int p=1 ; p <= numPoses ; p++){
        int toolI;

        double  graspOr = randG.scalar(-90,90);
        double  graspDisp = randG.scalar(-3,3);
        double  graspTilt = randG.scalar(0,45);

        cout << "Starting trial with orientation "<< graspOr <<", displacement "<<  graspDisp << " and tilt " << graspTilt << "." << endl;

        string tool = models[toolI];
        getToolByPose(tool, graspOr, graspDisp, graspTilt);  // re-grasp the tool with the given grasp parameters
        extractFeats();

        for (int i=1 ; i<=numAct ; i++){
            dragExe(theta,0.15,tilt);
            computeEffect();
            theta += thetaDiv;
        }
        toolI_prev = toolI;
    }
    return true;
}



bool Tool3DManager::runToolPose(int numRep,  int numAct){
    double thetaDiv = 360.0/numAct;
    double theta = 0.0;

    int tilt = 0;
    if (robot!="icubSim"){
        tilt = -15; // Action on the real robot need some tilt to not crash the hand on the table
    }

    // We assume the tool is grasped previously with getTool() or getToolByPose()
    for (int r=1 ; r<=numRep ; r++){
        for (int i=1 ; i<=numAct ; i++){
            dragExe(theta, 0.15, tilt);
            computeEffect();
            cout << "Going to next action" <<endl;
            theta += thetaDiv;
        }
    }
    return true;
}

bool Tool3DManager::runToolTrial(int numRep, const string &tool,  int numAct){


    // For each tool, run all combinations of
    // x 3 Grasps: left front right
    // x 8 thetas (every 45 degrees).
    for ( int ori = -90; ori < 100; ori = ori + 90){            // This is a loop for {-90, 0, 90}

        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<< endl;
        // Get tool
        if (robot == "icubSim"){
            getToolByPose(tool,ori);
        }else{
            cout << "Hand me the tool: " << tool << " at orientation "<< ori << "."<< endl;
            getToolExe(tool);
        }

        // Perform exploration actions
        runToolPose(numRep, numAct);
    }
    return true;
}

bool Tool3DManager::runExp(int toolIni, int toolEnd)
{
    for (int toolI=toolIni ; toolI<= toolEnd ; toolI++)
    {        
        string tool = models[toolI];
        runToolTrial(1, tool);
    }
    return true;
}

//  ======================== Exploitation commands  =======================
bool Tool3DManager::selectAction(int goal)
{   // Goals are 1 to achieve maximum displacement, 2 to perform pull
    int tilt = 0;
    if (!(robot=="icubSim")){
        tilt = -15; // Action on the real robot need some tilt to not crash teh hand on the table
        goHomeExe(false);
    }

    // A tool has to have been handled in the first place -> so the model is already loaded    
    // Extract features to send them to MATLAB.
    if (!extractFeats()){
        cout << "ToolFeatExt coudln't extract the features." << endl;
        return false;
    }
    cout << " Oriented 3D features extracted and sent to Matlab" << endl;

    Bottle *matReply = matlabPort.read(true);
    cout << " Read Bottle from Matlab of length " << matReply->size() << endl;
    cout << " Read prediction form Matlab " << matReply->toString().c_str() << endl;

    // Find the maximum for affPred and perform doAction on the max index.
    //Bottle *effPred = matReply->get(0).asList();
    //cout << ", Predicted Effect Vector " << effPred->toString().c_str() << endl;

    if (goal == 1) // Goal is to achieve maximum displacement
    {
        int numPoint = matReply->size();
        double bestEff = 0;
        int bestAngleI = 0;
        for ( int angleI = 0; angleI < numPoint; angleI++ ) // Find the approach that will generate predicted maximum effect
        {
           double predEff = matReply->get(angleI).asDouble();
           if (predEff > bestEff){
               bestEff = 	predEff;
               bestAngleI = angleI;
           }
        }
        double theta = 45 * bestAngleI;     // Get the drag angle corresponding to the best predicted effect
        printf("Best predicted drag from angle %g, predicted effect of %f m  \n", theta, bestEff);

        dragExe(theta,0.15, tilt);

        return computeEffect();
    }
    if (goal == 2) // Goal is to pull the object, if possible
    {
        // Pull action is dragging towards the robot -> angle 270 -> actionI = 6
        int pullI = 6;
        double predEff = matReply->get(pullI).asDouble();
        if (predEff < 0.05) // If the expected pull is smaller than 5 cm means teh tool can't afford pulling{
        {
            cout << "Please give me another tool, I can't pull with this tool" << endl;
            return false;
        }
        cout << "I can pull with this tool, expected drag is "<< predEff << endl;
        dragExe(270, 0.15, tilt);
        return computeEffect();
    }else{
        cout << "Please select a goal (1:Max disp, 2: pull)" <<endl;
        return false;
    }
}

bool Tool3DManager::predExp(int goal)
{   // Remember that MATLAB has to be connected to YARP, and the right affordance models loaded.

    int numTools_test = 52;
    // List the tools to use for testing:
    if (robot == "icubSim"){
        //int testTools[numTools_test] = {   0,  3,  7,       // hoe0, hoe3, hoe7
        //                       13, 16,      // hook3, hook6
        //                       24, 27, 29,  // rake4, rake7, rake9
        //                       32, 35, 38,  // stick2, stick5, stick8
        //                       48, 49,      // shovel8, shovel9
        //                       51 };        // star

        for (int testToolI = 1; testToolI < numTools_test; testToolI++ ){
            for ( int ori = -90; ori < 100; ori = ori + 90){            // This is a loop for {-90, 0, 90}            
                string& tool = models[testToolI];       // Because tools are 1 indexed on the file, but 0 on sim
                cout << "Attempting to load " << tool << endl;
                if (getToolByPose(tool, ori)){
                    cout << "Tool loaded, selecting best action for goal " << goal << endl;
                    for (int i = 1; i<3;i++){                                // this is just repetitions.
                        selectAction(goal);
                    }
                }else{
                    cout << "Couldn't load the desired test tool" << endl;
                    return false;
                }

            }
        }
        return true;

    }else{


    }

}


/***********************************************************************************/
// ========================  CONFIGURATION FUNTIONS


bool Tool3DManager::setSeg(bool seg){
    Bottle cmd3DE,reply3DE;                 // bottles for objects3DExplorer
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("setSeg");
    if (seg){
        cmd3DE.addString("ON");
    }else{
        cmd3DE.addString("OFF");
    }
    cout << "Sending RPC command to objects3DExplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;
    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "objects3DExplorer couldnt set segmentatio to ON/OFF" << endl;
        return false;
    }

    return true;
}



/**********************************************************/
                    /* PRIVATE METHODS */
/**********************************************************/


// ===================================================================================================================================/
/**************************************************** TOOL LOAD AND INFORMATION ******************************************************/
// ===================================================================================================================================/


/**********************************************************/
bool Tool3DManager::graspToolExe()
{
    Bottle cmdAMM, replyAMM;               // bottles for Affordance Motor Module
    Bottle cmdARE, replyARE;               // bottles for actionsRenderingEngine

    // Remove any end effector extension that might be.
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("tool");
    cmdAMM.addString("remove");
    rpcAffMotor.write(cmdAMM, replyAMM);

    // Send commands to ARE to get the tool, close the hand and go to central position
    fprintf(stdout,"Reach me a tool please.\n");
    cmdARE.clear();
    replyARE.clear();
    cmdARE.addString("tato");
    cmdARE.addString(hand);
    rpcMotorAre.write(cmdARE,replyARE);
    //Time::delay(2);

    // Close hand on tool grasp
    cmdARE.clear();
    replyARE.clear();
    cmdARE.addString("hand");
    cmdARE.addString("close_hand_tool");
    cmdARE.addString(hand);
    rpcMotorAre.write(cmdARE, replyARE);

    // Check if grasp was successful
    cmdARE.clear();
    replyARE.clear();
    cmdARE.addString("get");
    cmdARE.addString("holding");
    rpcMotorAre.write(cmdARE, replyARE);
    if(!replyARE.get(0).asBool())
        return false;

    //lookToolExe();
    goHomeExe();

    return true;
}


/**********************************************************/
bool Tool3DManager::lookToolExe()
{
    Bottle cmdAMM,replyAMM;               // bottles for the Affordance Motor Module
    Bottle cmdARE, replyARE;              // bottles for actionsRenderingEngine

    // Remove any end effector extension that might be.
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("tool");
    cmdAMM.addString("remove");
    rpcAffMotor.write(cmdAMM, replyAMM);

    // Close hand on tool grasp
    cmdARE.clear();
    replyARE.clear();
    cmdARE.addString("look");
    cmdARE.addString("hand");
    cmdARE.addString(hand);
    cmdARE.addString("fixate");
    cmdARE.addString("block_eyes");
    cmdARE.addDouble(5.0);
    rpcMotorAre.write(cmdARE, replyARE);
    //Time::delay(0.5);

    // Move hand to central position to check tool extension and perform regrasp easily.
    cout << "Moving arm to a central position" << endl;
    double dispY = (hand=="right")?0.15:-0.15;
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("push");            // Set a position in the center in front of the robot
    cmdAMM.addDouble(-0.2);
    cmdAMM.addDouble(dispY);
    cmdAMM.addDouble(0.0);
    cmdAMM.addDouble(0.0);       // No angle
    cmdAMM.addDouble(0.0);       // No radius
    rpcAffMotor.write(cmdAMM, replyAMM);

    // Stop head moving for further visual processing
    cmdARE.clear();
    replyARE.clear();
    cmdARE.addString("idle");
    rpcMotorAre.write(cmdARE, replyARE);

    return true;
}


/**********************************************************/
bool Tool3DManager::load3Dmodel(const string &cloudName)
{    
    // Communicates with objects3DExplorer to load the corresponding Model, and find its pose and tooltip
    Bottle cmd3DE,reply3DE;                 // bottles for toolFeatExt
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("loadCloud");
    cmd3DE.addString(cloudName);
    cout << "Sending RPC command to objects3DExplorer: " << cmd3DE.toString() << "."<< endl;    
    rpc3Dexp.write(cmd3DE, reply3DE);

    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "objects3DExplorer coudln't load the tool." << endl;
        return false;
    }

    cout << "Cloud " << cloudName << " loaded" << endl;
    return true;
}


/**********************************************************/
bool Tool3DManager::loadToolSim(const string &tool, const double graspOr,const double graspDisp, const double graspTilt)
{    
    toolname = tool;
    int toolI = findToolInd(tool);
    if (tool < 0){
        cout << "Tool label not found" << endl;
        return false;}

    double tiltValid = graspTilt;
    if (graspTilt > 90.0) {   tiltValid  = 90.0; }
    if (graspTilt < 0.0)  {   tiltValid  = 0.0; }
    cout << endl<<"Getting tool " << toolI <<" with orientation: "<< graspOr << ", displacement: " << graspDisp << "and tilt: " << graspTilt << endl;
    cout << "Tool present: " << toolLoadedIdx <<"."<< endl;

    Bottle cmdAMM,replyAMM;               // bottles for the Affordance Motor Module
    Bottle cmdSim,replySim;               // bottles for Simulator
    Bottle cmd3DE,reply3DE;               // bottles for toolFeatExt

    // Remove any end effector extension that might be.
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("tool");
    cmdAMM.addString("remove");
    rpcAffMotor.write(cmdAMM, replyAMM);

    // Move hand to center to receive tool on correct position - implemented by faking a push action to the center to avoid iCart dependencies.
    cout << "Moving "<< hand << " arm to a central position" << endl;
    double dispY = (hand=="right")?0.15:-0.15;
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("push");            // Set a position in the center in front of the robot
    cmdAMM.addDouble(-0.25);
    cmdAMM.addDouble(dispY);
    cmdAMM.addDouble(0.05);
    cmdAMM.addDouble(0.0);       // No angle
    cmdAMM.addDouble(0.0);       // No radius
    rpcAffMotor.write(cmdAMM, replyAMM);

    if (toolI != toolLoadedIdx)
    {
        cout << endl<<"Loading tool " << toolI <<", or: "<< graspOr << ", displacement: " << graspDisp <<endl;
        toolLoadedIdx = toolI;

        // Call simtoolloader to clean the world
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("del");        
        rpcSimToolLoader.write(cmdSim, replySim);

        // Call simtoolloader to create the tool
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("tool");
        cmdSim.addInt(toolI);               // tool
        cmdSim.addInt(0);                   // object -> Cube
        cmdSim.addInt(graspOr);             // orientation
        cmdSim.addInt(graspDisp);           // displacement
        //cmdSim.addInt(tiltValid);           // tilt
        cmdSim.addInt(45.0);                  // tilt
        rpcSimToolLoader.write(cmdSim, replySim);
        cout << "Sent RPC command to simtoolloader: " << cmdSim.toString() << "." <<endl;

        // check succesful tool creation
        if (replySim.size() <1){
            cout << "simtoolloader couldn't load tool." << endl;
            return false;
        }

        string cloudName = "sim/"+ tool;
        cout << "cloud model: " << cloudName << endl;

        // Query toolFeatExt to load model to load 3D Pointcloud.
        if (!load3Dmodel(cloudName)){
            cout << "Coudln't load the tool." << endl;
            return false;
        }        

    } else {        // Need just to reGrasp tool

        // Query simtoolloader to rotate the virtual tool
        cout << endl<< "Regrasping tool " << toolI <<" with orientation: "<< graspOr << " and displacement: " << graspDisp <<endl;

        // Call simtoolloader to create the tool
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("rot");
        cmdSim.addDouble(graspOr);             // orientation
        cmdSim.addDouble(graspDisp);           // displacement
        cmdSim.addDouble(tiltValid);           // displacement
        rpcSimToolLoader.write(cmdSim, replySim);
        cout << "Sent RPC command to simtoolloader: " << cmdSim.toString() << "." <<endl;

        // check succesful tool reGrasping
        if (replySim.size() <1){
            cout << "simtoolloader cloudln't load tool." << endl;
            return false;
        }
    }

    // Get the tooltip canonical coordinates wrt the hand coordinate frame from its bounding box.
    cout << "Getting tooltip coordinates." << endl;
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("findTooltipParam");
    cmd3DE.addDouble(graspOr);
    cmd3DE.addDouble(graspDisp);
    cmd3DE.addDouble(tiltValid);
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "Sent RPC command to object3DExplorer: " << cmd3DE.toString() << "."<< endl;
    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "Object3DExplorer coudln't compute the tooltip." << endl;
        return false;
    }
    cout << " Received reply: " << reply3DE.toString() << endl;

    Bottle tooltipBot = reply3DE.tail();
    tooltip.x = tooltipBot.get(0).asDouble();
    tooltip.y = tooltipBot.get(1).asDouble();
    tooltip.z = tooltipBot.get(2).asDouble();

    cout << "Tooltip of tool in positon: x= " << tooltip.x << ", y = " << tooltip.y << ", z = " << tooltip.z <<endl;

    cout << endl << "Attaching tooltip." << endl;
    addToolTip(tooltip);
    cout << "Tool loaded and tooltip attached" << endl;

    // Put action parameters on a port so they can be read
    graspVec.clear();		// Clear to make space for new coordinates
    graspVec.resize(4);     // Resize to save orientation - displacement coordinates coordinates
    graspVec[0] = toolI;
    graspVec[1] = graspOr;
    graspVec[2] = graspDisp;
    graspVec[3] = tiltValid;

    return true;
}


/**********************************************************/
bool Tool3DManager::loadToolPose(const string &tool, const double graspOr, const double graspDisp, const double graspTilt, const double graspShift)
{
    // Look For model label among all models.
    int toolI = findToolInd(tool);
    if (tool < 0)
        return false;

    cout << endl<<"Getting tool " << tool << " with orientation: "<< graspOr << ", displacement: " << graspDisp << ", tilt: " << graspTilt << "and shift: " << graspTilt << endl;
    toolLoadedIdx = toolI;
    toolname = tool;

    bool ok;

    ok = graspToolExe();
    if (!ok){
        cout << "iCub couldn't grasp the tool properly" << endl;
        return false;
    }

    string cloudName = "real/"+ tool;
    cout << "cloud model: " << cloudName << endl;

    ok = load3Dmodel(cloudName);
    if (!ok){
        cout << "O3DE could not load the tool" << endl;
        return false;
    }

    // Query objects3DExplorer to load model to load 3D Pointcloud.
    cout << "Querying objectd3DExplorer to load pointcloud, and set tooltip based on parameters." << endl;
    ok = findTipFromParam(tooltip, graspOr, graspDisp, graspTilt, graspShift);
    if (!ok){
        cout << "Tool Tip could not be computed from params." << endl;
        return false;
    }
    cout << "Tooltip of tool in positon: x= "<< tooltip.x << ", y = " << tooltip.y << ", z = " << tooltip.z <<endl;

    cout << endl << "Attaching tooltip." << endl;
    ok = addToolTip(tooltip);
    if (!ok){
        cout << "Tool tip could not be attached." << endl;
        return false;
    }
    cout <<  "Tooltip attached, ready to perform action!" << endl;


    // Put action parameters on a port so they can be read
    graspVec.clear();		// Clear to make space for new coordinates
    graspVec.resize(4);     // Resize to save orientation - displacement coordinates coordinates
    graspVec[0] = toolI;
    graspVec[1] = graspOr;
    graspVec[2] = graspDisp;
    graspVec[3] = graspTilt;

    return true;
}

/**********************************************************/
bool Tool3DManager::getToolExe(const string& tool)
{
    int toolI = findToolInd(tool);
    toolname = tool;

    bool ok;
    cout << endl << "Getting " << tool << endl;
    ok = graspToolExe();
    if (!ok){
        cout << "iCub couldn't grasp the tool properly" << endl;
        return false;
    }
    cout <<  "Tool grasped!" << endl;

    ok = findPoseExe(tool, tooltip);
    if (!ok){
        cout << "Tool Pose could not be estimated properly" << endl;
        return false;
    }
    double ori = getToolOri();
    cout <<  "Tool  grasped with orientation: " << ori << endl;
    cout <<  "Tooltip found at (" <<tooltip.x <<", " << tooltip.y << "," <<tooltip.z <<  ") !" << endl;

    ok =addToolTip(tooltip);
    if (!ok){
        cout << "Tool tip could not be attached." << endl;
        return false;
    }
    cout <<  "Tooltip attached, ready to perform action!" << endl;

    // Put action parameters on a port so they can be read
    graspVec.clear();		// Clear to make space for new coordinates
    graspVec.resize(4);     // Resize to save orientation - displacement coordinates coordinates
    graspVec[0] = toolI;
    graspVec[1] = ori;
    return true;
}


/**********************************************************/
int Tool3DManager::findToolInd(const string& tool)
{
    int toolI = -1;
    vector<string>::iterator it = std::find(models.begin(), models.end(), tool);
    if ( it == models.end() ) { //Not found
        return -1;
    }else{                          // Found
        toolI = std::distance(models.begin(), it);                  // return position of found element
    }
    return toolI;
}

double Tool3DManager::getToolOri()
{
    Bottle cmd3DE,reply3DE;                 // bottles for objects3DExplorer
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("getOri");
    rpc3Dexp.write(cmd3DE, reply3DE);
    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "objects3DExplorer coudln't return tool's orientation." << endl;
        return false;
    }
    double ori = reply3DE.get(0).asDouble();

    return ori;
}

/**********************************************************/
bool Tool3DManager::findPoseExe(const string& tool, Point3D &ttip)
{
    if (tool.size()<4){
        cout << " Tool not loaded, please load tool before attempting to estimate pose" << endl;
        return false;
    }

    // Communicates with objects3DExplorer to load the corresponding Model, and find its pose and tooltip
    Bottle cmd3DE,reply3DE;                 // bottles for objects3DExplorer

    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("loadCloud");
    cmd3DE.addString(tool);
    cout << "Sending RPC command to objects3DExplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;

    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "objects3DExplorer coudln't load the tool." << endl;
        return false;
    }

    // Query object3DExplorer to find the tooltip
    cout << "Finding out Pose and tooltip by aligning 3D partial view with model." << endl;
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("findTooltipAlign");
    cmd3DE.addInt(5);
    cout << "Sending RPC command to objects3Dexplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;
    if (reply3DE.get(0).asString() == "[nack]" ){
        cout << "Objects3Dexplorer couldn't align properly, trying from another pose." << endl;
        lookToolExe();
        cmd3DE.clear();   reply3DE.clear();
        cmd3DE.addString("findTooltipAlign");
        cmd3DE.addInt(5);
        rpc3Dexp.write(cmd3DE, reply3DE);
        if (reply3DE.get(0).asString() == "[nack]" ){
            cout << "Objects3Dexplorer couldn't find out the pose." << endl;
            return false;
        }
    }

    Bottle tooltipBot = reply3DE.tail();
    ttip.x = tooltipBot.get(0).asDouble();
    ttip.y = tooltipBot.get(1).asDouble();
    ttip.z = tooltipBot.get(2).asDouble();

    return true;
}



/**********************************************************/
bool Tool3DManager::regraspExe(Point3D &newTip, const double graspOr, const double graspDisp, const double graspTilt, const double graspShift)
    {
    double tiltValid = graspTilt;
    if (graspTilt > 90.0) {   tiltValid  = 90.0; }
    if (graspTilt < 0.0)  {   tiltValid  = 0.0; }

    // Query simtoolloader to rotate the virtual tool
    cout << endl<< "Regrasping tool " << toolLoadedIdx <<" with orientation: " << graspOr << ", disp " << graspDisp << ", Tilt " << tiltValid << " and shift " << graspShift << endl;

    // Call simtoolloader to create the tool
    if (robot == "icubSim"){
        Bottle cmdSim,replySim;       // bottles for Simulator
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("rot");
        cmdSim.addDouble(graspOr);             // orientation
        cmdSim.addDouble(graspDisp);           // displacement
        cmdSim.addDouble(tiltValid);           // displacement
        rpcSimToolLoader.write(cmdSim, replySim);
        cout << "Sent RPC command to simtoolloader: " << cmdSim.toString() << "." <<endl;

        // check succesful tool reGrasping
        if (replySim.size() <1){
            cout << "simtoolloader clouldn't regrasp tool." << endl;
            return false;
        }
        //cout << "Received reply: " << replySim.toString() << endl;
    }

    // Tasnform the tooltip from the canonical to the current grasp
    bool ok;
    ok = findTipFromParam(newTip, graspOr, graspDisp, graspTilt, graspShift);
    if (!ok){
        cout << "Tool tip could not be computed from params." << endl;
        return false;
    }
    cout << "Tooltip of tool in positon: x= "<< newTip.x << ", y = " << newTip.y << ", z = " << newTip.z <<endl;

    cout << endl << "Attaching tooltip." << endl;
    ok = addToolTip(newTip);
    if (!ok){
        cout << "Tool tip could not be attached." << endl;
        return false;
    }
    fprintf(stdout,"Tool loaded and tooltip attached \n");

    graspVec.clear();		// Clear to make space for new coordinates
    graspVec.resize(4);     // Resize to save orientation - displacement coordinates coordinates

    // Put action parameters on a port so they can be read
    graspVec[0] = toolLoadedIdx;
    graspVec[1] = graspOr;
    graspVec[2] = graspDisp;
    graspVec[3] = tiltValid;

    return true;
}


/**********************************************************/
bool Tool3DManager::findTipFromParam( Point3D &ttip, const double graspOr, const double graspDisp, const double graspTilt, const double graspShift)
{
    // Query object3DExplorer to find the tooltip
    cout << "Finding out tooltoltip from model and grasp parameters." << endl;
    Bottle cmd3DE, reply3DE;

    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("findTooltipParam");
    cmd3DE.addDouble(graspOr);
    cmd3DE.addDouble(graspDisp);
    cmd3DE.addDouble(graspTilt);
    cmd3DE.addDouble(graspShift);
    cout << "Sending RPC command to objects3Dexplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;
    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "Objects3Dexplorer couldn't estimate the tip." << endl;
        return false;
    }
    Bottle ttBot = reply3DE.tail();

    ttip.x = ttBot.get(0).asDouble();
    ttip.y = ttBot.get(1).asDouble();
    ttip.z = ttBot.get(2).asDouble();

    return true;
}


/**********************************************************/
bool Tool3DManager::exploreTool(Point3D &ttip)
{
    // Query object3DExplorer to find the tooltip
    cout << "Finding out tooltoltip by exploration and symmetry" << endl;
    Bottle cmd3DE, reply3DE;

    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("exploreTool");
    cout << "Sending RPC command to objects3Dexplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;
    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "There was some error exploring the tool." << endl;
        return false;
    }

    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("findPlanes");
    cout << "Sending RPC command to objects3Dexplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;
    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "Main planes could not be computed." << endl;
        return false;
    }

    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("findTooltipSym");
    cout << "Sending RPC command to objects3Dexplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;
    if (reply3DE.get(0).asString() != "[ack]" ){
        cout << "There was some estimating the tooltip." << endl;
        return false;
    }

    Bottle ttBot = reply3DE.tail();

    ttip.x = ttBot.get(0).asDouble();
    ttip.y = ttBot.get(1).asDouble();
    ttip.z = ttBot.get(2).asDouble();

    return true;
}

/**********************************************************/
bool Tool3DManager::addToolTip(const Point3D ttip)
{
    Bottle cmdAMM, replyAMM;

    // Attach the new tooltip to the "body schema"
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("tool");
    cmdAMM.addString("attach");
    cmdAMM.addString(hand.c_str());
    cmdAMM.addDouble(ttip.x);
    cmdAMM.addDouble(ttip.y);
    cmdAMM.addDouble(ttip.z);
    cout << "RPC command to affMotor: " << cmdAMM.toString() << endl;
    rpcAffMotor.write(cmdAMM, replyAMM);
    cout << "RPC reply from affMotor: " << replyAMM.toString() << endl;
    if (!(replyAMM.toString() == "[ack]")){
        cout <<  "Affordance Motor Module could not add the tooltip " << endl;
        return false;
    }

    return true;
}

/**********************************************************/
bool Tool3DManager::extractFeats()
{    // Query toolFeatExt to extract features
    cout << "Extacting features of handled tool." << endl;
    Bottle cmd3DE,reply3DE;                 // bottles for objects3DExplorer

    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("extractFeats");
    cout << "Sending RPC command to objects3DExplorer: " << cmd3DE.toString() << "."<< endl;
    rpc3Dexp.write(cmd3DE, reply3DE);
    cout << "RPC reply from objects3Dexplorer: " << reply3DE.toString() << "."<< endl;

    return true;
}


/**********************************************************/
/*void Tool3DManager::transformToolTip(const Point3D ttCanon, Point3D &tooltipTrans, const double graspOr, const double graspDisp, const double graspTilt)
{
    // Transform canonical coordinates to correspond with tilt, rotation and displacemnt of the tool.
    Point3D ttRot = {0.0, 0.0, 0.0};
    //ttRot.x = 0.0, ttRot.y = 0.0, ttRot.z = 0.0;    // Receive coordinates of tooltip of tool in canonical position
    // Rotate first around Y axis to match tooltip to end-effector orientation
    double sinOr = sin(graspOr*M_PI/180.0);
    double cosOr = cos(graspOr*M_PI/180.0);
    ttRot.x = ttCanon.x*cosOr - ttCanon.z*sinOr;
    ttRot.y = ttCanon.y;
    ttRot.z = ttCanon.x*sinOr + ttCanon.z*cosOr;
    cout << "Tooltip of tool rotated " << graspOr << " degrees: x= "<< ttRot.x << ", y = " << ttRot.y << ", z = " << ttRot.z << endl;

    // Now tilt 45 degrees arund Z to match the way in which the tool is held
    Point3D ttTilt = {0.0, 0.0, 0.0};
    // ttTilt.x =0.0, ttTilt.y=0.0, ttTilt.z=0.0;    // Receive coordinates of tooltip of tool in canonical position
    ttTilt.x = ttRot.x*cos(graspTilt*M_PI/180.0) - ttRot.y*sin(graspTilt*M_PI/180.0);
    ttTilt.y = ttRot.x*sin(graspTilt*M_PI/180.0) + ttRot.y*cos(graspTilt*M_PI/180.0);
    ttTilt.z = ttRot.z;
    cout << "Tooltip of tool rotated " << graspOr << " degrees and tilted 45 degrees: x= "<< ttTilt.x << ", y = " << ttTilt.y << ", z = " << ttTilt.z << endl;

    // Finally add translation along -Y axis to match handle displacement
    tooltipTrans.x = ttTilt.x;
    tooltipTrans.y = ttTilt.y - graspDisp/100.0;;
    tooltipTrans.z = ttTilt.z + 0.03;   // The center of the tool is always displaced 3cm from the tool ref frame to avoid collisions.
    cout << "Tooltip of tool rotated, tilted and displaced "<< graspDisp << "cm: x= "<< tooltipTrans.x << ", y = " << tooltipTrans.y << ", z = " << tooltipTrans.z << endl;

    return;
}
*/

// ===================================================================================================================================/
/***************************************************   OBJECT LOCALIZATION   *********************************************************/
// ===================================================================================================================================/

/**********************************************************/
double Tool3DManager::findTableHeight(bool calib){

    Bottle cmdARE, replyARE;
    if (calib){
        cout << "Asking ARE to calibrate on table: " << hand <<endl;

        cmdARE.clear();    replyARE.clear();
        cmdARE.addString("calib");
        cmdARE.addString("table");
        cmdARE.addString("left");
        rpcMotorAre.write(cmdARE,replyARE);
    }

    cout << "Retrieving table height data from ARE " << hand <<endl;
    cmdARE.clear();     replyARE.clear();
    cmdARE.addString("get");
    cmdARE.addString("table");
    rpcMotorAre.write(cmdARE,replyARE);

    double th = replyARE.get(0).asDouble();

    cout << "Table height is " << th <<endl;

    return th;
}


/**********************************************************/
bool Tool3DManager::trackObjExe()
{
    goHomeExe(false);
    // Select the target object to be tracked
    printf("\n \n Click first TOP LEFT and then BOTTOM RIGHT from the object to set the tracking template. Please.\n");
    Bottle cmdFinder,replyFinder;
    cmdFinder.clear();
    replyFinder.clear();
    cmdFinder.addString("getBox");
    //fprintf(stdout,"RPC to objFinder %s:\n", cmdFinder.toString().c_str());
    rpcObjFinder.write(cmdFinder, replyFinder);
    //fprintf(stdout,"  Reply from objFinder %s:\n",replyFinder.toString().c_str());

    printf("Object template has been set properly\n");
    trackingObj = true;

    // Set ARE to constantly track the object
    /*Bottle cmdAre, replyAre;
    cmdAre.addString("track");
    cmdAre.addString("track");
    cmdAre.addString("no_sacc");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"tracking started %s:\n",replyAre.toString().c_str());
    */
    return true;
}

/**********************************************************/
bool Tool3DManager::getObjLoc(Vector &coords3D)
{
    if (robot=="icubSim"){
        Bottle cmdSim,replySim;
        cout << endl <<"Getting 3D coords of object." <<endl;
        cmdSim.clear();        replySim.clear();
        cmdSim.addString("world");
        cmdSim.addString("get");
        cmdSim.addString("box");
        cmdSim.addInt(1);
        //printf("RPC to simulator: %s \n", cmdSim.toString().c_str());
        rpcSimulator.write(cmdSim, replySim);
        //printf("  Reply from simulator: %s \n", replySim.toString().c_str());

        if (replySim.size() >1){
            Vector coords3Dworld(3,0.0);
            // REad coordinates returned by simualtor
            coords3Dworld(0) = replySim.get(0).asDouble();
            coords3Dworld(1) = replySim.get(1).asDouble();
            coords3Dworld(2) = replySim.get(2).asDouble();
            printf("Point in 3D in world coordinates retrieved: %g, %g %g\n", coords3Dworld(0), coords3Dworld(1), coords3Dworld(2));

            // Transform object coordinates from World to robot!!!
            coords3D(0) = -(coords3Dworld(2) + 0.026);        // Xr = -Zw + 0.026 m
            coords3D(1) = -coords3Dworld(0);                // Yr = -Xw
            coords3D(2) = coords3Dworld(1) - 0.5976;        // Zr = Yw - 0.5976 m
            printf("Point in 3D in robot coordinates: %g, %g %g\n", coords3D(0), coords3D(1), coords3D(2));

            return true;
        }
        cout << "No 3D point received" << endl;
        return false;

    }else{

        // Get the 2D coordinates of the object from objectFinder
        Bottle *trackCoords = trackPort.read(true);
        if (trackCoords->size() <1){
            cout << "No 2D point received" << endl;
            return false;
        }

        cout << " Read from tracker: " << trackCoords->toString() << endl;
        double tlx = trackCoords->get(0).asList()->get(0).asDouble();
        double tly = trackCoords->get(0).asList()->get(1).asDouble();
        double brx = trackCoords->get(0).asList()->get(2).asDouble();
        double bry = trackCoords->get(0).asList()->get(3).asDouble();

        Vector coords2D(2,0.0);
        coords2D(0) = (tlx + brx)/2;
        coords2D(1) = (tly + bry)/2;

        cout << "Object tracked at 2D coordinates (" << coords2D[0] << ", " << coords2D[1] << ")" << endl;

        coords3D.resize(3);
        Bottle cmdAre, replyAre;
        cmdAre.addString("get");
        cmdAre.addString("s2c");
        Bottle& boords_bot = cmdAre.addList();
        boords_bot.addDouble(coords2D[0]);
        boords_bot.addDouble(coords2D[1]);
        rpcMotorAre.write(cmdAre,replyAre);


        if (replyAre.size() >1){
            coords3D(0) = replyAre.get(0).asDouble();
            coords3D(1) = replyAre.get(1).asDouble();
            coords3D(2) = replyAre.get(2).asDouble();
            printf("Point in 3D retrieved: %g, %g %g\n", coords3D(0), coords3D(1), coords3D(2));

            return true;
        }
        cout << "No 3D point received" << endl;
        return false;
    }
}

/**********************************************************/
bool Tool3DManager::getObjRot(Vector &rot3D)
{
    if (robot=="icubSim"){
        Bottle cmdSim,replySim;
        //fprintf(stdout,"Get 3D coords of tracked object:\n");
        cmdSim.clear();        replySim.clear();
        cmdSim.addString("world");
        cmdSim.addString("rot");
        cmdSim.addString("box");
        cmdSim.addInt(1);
        //printf("RPC to simulator: %s \n", cmdSim.toString().c_str());
        rpcSimulator.write(cmdSim, replySim);
        //printf("  Reply from simulator: %s \n", replySim.toString().c_str());

        if (replySim.size() >1){
            rot3D(0) = replySim.get(0).asDouble();
            rot3D(1) = replySim.get(1).asDouble();
            rot3D(2) = replySim.get(2).asDouble();
            printf("Rotation in 3D retrieved: %g, %g %g\n", rot3D(0), rot3D(1), rot3D(2));
            return true;
        }
        cout << "No 3D point received" << endl;
        return false;

    }else{

        return false;
    }
}


/**********************************************************/
bool Tool3DManager::computeEffect()
{
    cout << endl << "Computing effect from action."  << endl;

    effectVec.clear();		// Clear to make space for new coordinates
    effectVec.resize(3);    // Resize to 3D coordinates

    // Get the actual coordinates
    target3DcoordsAfter.clear();		// Clear to make space for new coordinates
    target3DcoordsAfter.resize(3);      // Resize to 3D coordinates
    target3DrotAfter.clear();         // Clear to make space for new rotation values
    target3DrotAfter.resize(3);
    if(!getObjLoc(target3DcoordsAfter))     // Get the location of the object after the action
    {
        cout << " Object not located, cant observe effect"<< endl;
        return false;
    }
    cout << "Coords after action: (" << target3DcoordsAfter[0] << ", " << target3DcoordsAfter[2] << ", "<< target3DcoordsAfter[2] << "). " <<endl;

    //To compute the displacement, we assume that the object hasnt move in the z axis (that is, has remained on the table)
    Vector displ = target3DcoordsAfter - target3DcoordsIni;
    double dx = displ[0];
    double dy = displ[1];

    double displDist = sqrt(dx*dx+dy*dy);  //sum of the squares of the differences
    double displAngle = atan2 (dy,dx) * 180 / M_PI;

    // Put values into the effect Vector
    effectVec[0] = displDist;
    effectVec[1] = displAngle;

    if (robot=="icubSim"){
        if(!getObjRot(target3DrotAfter))        // Get the rotation of the object after the action
        {
            cout << " Object rotation not available, cant compute effect"<< endl;
            return false;
        }
        cout << "Rotation after action: (" << target3DrotAfter[0] << ", " << target3DrotAfter[2] << ", "<< target3DrotAfter[2] << "). " <<endl;

        //To compute the rotation, we assume that the object has only rotated around axis Y (that is, has not been tipped)
        Vector rot = target3DrotAfter - target3DrotIni;
        double effectRot = rot[1];  // Rotation difference around Y axis
        effectVec[2] = effectRot;
    } else {
        effectVec[2] = 0.0;
    }

    cout << "Object displaced " << effectVec[0] << " meters on " << effectVec[1] << " direction, and rotated " << effectVec[2] << " degrees."<< endl;

    // Prepare the affordance out bottle
    aff_out_data.clear();
    Bottle& aff_toolpose = aff_out_data.addList();
    aff_toolpose.addString(toolname);
    aff_toolpose.addInt(graspVec[0]);           // toolI
    aff_toolpose.addDouble(graspVec[1]);        // orientation
    Bottle& aff_action = aff_out_data.addList();
    aff_action.addDouble(actVec[0]);        // theta
    aff_action.addDouble(actVec[1]);        // radius
    Bottle& aff_effect = aff_out_data.addList();
    aff_effect.addDouble(effectVec[0]);        // distance
    aff_effect.addDouble(effectVec[1]);        // angle
    aff_effect.addDouble(effectVec[2]);        // rotation

    // Send all data so it can be read and saved.
    sendAffData();

    resetCube();

    return true;
}

bool Tool3DManager::resetCube()
{
    if (robot=="icubSim"){
        // Put the cube back in place to restart round:
        Bottle cmdSim,replySim;       // bottles for Simulator
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("move");
        cmdSim.addInt(0);                   // object -> Cube
        rpcSimToolLoader.write(cmdSim, replySim); // Call simtoolloader to create the tool
        //cout << "Sent RPC command to simtoolloader: " << cmdSim.toString() << "." <<endl;
        //cout << " Received reply: " << replySim.toString() << endl;
        Time::delay(0.3); // give time for the cube to reach its position before jumping to next step
    } else {
        cout << '\n' << endl;
        cout << "Effect computed, 5 seconds to put the object back in place" <<endl;
        cout << "5" <<endl;
        Time::delay(1);
        cout << "4" <<endl;
        Time::delay(1);
        cout << "3" <<endl;
        Time::delay(1);
        cout << "2" <<endl;
        Time::delay(1);
        cout << "1" <<endl;
        Time::delay(1);
    }
    return true;
}

bool Tool3DManager::sendAffData()
{
    // Send (action - grasp - effect) parameters together so that they are synced

    // put values on a port so they can be read
    affDataPort.write(aff_out_data);

    return true;
}


// ===================================================================================================================================/
/******************************************************   ACTION EXECUTION   *********************************************************/
// ===================================================================================================================================/

/**********************************************************/
void Tool3DManager::goHomeExe(const bool hands)
{
    cout << endl << "Going home, hands: " << hands <<endl;

    if (robot == "icubSim"){
        Bottle cmd3DE,reply3DE;                 // bottles for O3DE
        cmd3DE.clear();   reply3DE.clear();
        cmd3DE.addString("showTipProj");
        cmd3DE.addString("OFF");
        rpc3Dexp.write(cmd3DE, reply3DE);

        Bottle cmdAMM, replyAMM;
        // Move hand to center to receive tool on correct position - implemented by faking a push action to the center to avoid iCart dependencies.
        cout << "Moving "<< hand << " arm to a central position" << endl;
        double dispY = (hand=="right")?0.20:-0.20;
        cmdAMM.clear();replyAMM.clear();
        cmdAMM.addString("push");            // Set a position in the center in front of the robot
        cmdAMM.addDouble(-0.50);
        cmdAMM.addDouble(dispY);
        cmdAMM.addDouble(0.10);
        cmdAMM.addDouble(0.0);       // No angle
        cmdAMM.addDouble(0.0);       // No radius
        rpcAffMotor.write(cmdAMM, replyAMM);

        cmd3DE.clear();   reply3DE.clear();
        cmd3DE.addString("showTipProj");
        cmd3DE.addString("ON");
        rpc3Dexp.write(cmd3DE, reply3DE);


    }else{
        Bottle cmdAre, replyAre;
        cmdAre.clear();
        replyAre.clear();
        if (hands){
            cmdAre.addString("home");
            cmdAre.addString("all");
        }else{
            cmdAre.addString("home");
            cmdAre.addString("head");
            cmdAre.addString("arms");
        }
        rpcMotorAre.write(cmdAre,replyAre);
    }
    return;
}


/**********************************************************/
bool Tool3DManager::slideExe(const double theta, const double radius)
{
    actVec.clear();		// Clear to make space for new coordinates
    actVec.resize(2);   // Resize to save theta - radius coordinates coordinates

    cout << endl<< "Performing slide action from angle " << theta <<" and radius "<< radius  << endl;
    target3DcoordsIni.clear();		// Clear to make space for new coordinates
    target3DcoordsIni.resize(3);    // Resizze to 3D coordinates
    target3DrotIni.clear();         // Clear to make space for new rotation values
    target3DrotIni.resize(3);

    // Locate object and perform slide action with given theta and aradius parameters
    if (!getObjLoc(target3DcoordsIni))
    {
        cout << " Object not located, cant perform action"<< endl;
        return false;
    }
    getObjRot(target3DrotIni);               // Get the initial rotation of the object

    // Action during the Affordance Motor Module execution transforms the end-effector from the hand to the tip pf the tool,
    // so the representation has to be inverted so it still shows the right tooltip while performing the action
    // and restored afterwards so it shows it also during not execution.
    Point3D tooltip_tmp= tooltip;
    tooltip.x = 0.0;    tooltip.y = 0.0;  tooltip.z = 0.0;


    cout << "Approaching to coords: (" << target3DcoordsIni[0] << ", " << target3DcoordsIni[2] << ", "<< target3DcoordsIni[2] << "). " <<endl;
    Bottle cmdAMM,replyAMM;                    // bottles for the Affordance Motor Module
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("slide");                 // Set a position in the center in front of the robot
    cmdAMM.addDouble(target3DcoordsIni[0]);
    cmdAMM.addDouble(target3DcoordsIni[1]);
    cmdAMM.addDouble(target3DcoordsIni[2] + 0.03);   // Approach the center of the object, not its lower part.
    cmdAMM.addDouble(theta);
    cmdAMM.addDouble(radius);
    //fprintf(stdout,"RPC to affMotor: %s\n",cmdAMM.toString().c_str());
    rpcAffMotor.write(cmdAMM, replyAMM);
    //fprintf(stdout,"  Reply from affMotor: %s\n",replyAMM.toString().c_str());


    // Restore show tool coordinates
    tooltip = tooltip_tmp;

    // Put action parameters on a port so they can be read
    actVec[0] = theta;
    actVec[1] = radius;

    goHomeExe();

    return true;
}

/**********************************************************/
bool Tool3DManager::dragExe(const double theta, const double radius, const double tilt)
{
    actVec.clear();		// Clear to make space for new coordinates
    actVec.resize(2);   // Resize to save theta - radius coordinates coordinates

    cout << endl<< "Performing drag action from angle " << theta <<" and radius "<< radius  << endl;
    target3DcoordsIni.clear();		// Clear to make space for new coordinates
    target3DcoordsIni.resize(3);    // Resizze to 3D coordinates
    target3DrotIni.clear();         // Clear to make space for new rotation values
    target3DrotIni.resize(3);

    // Locate object and perform slide action with given theta and aradius parameters
    if (!getObjLoc(target3DcoordsIni))
    {
        cout << " Object not located, cant perform action"<< endl;
        return false;
    }
    getObjRot(target3DrotIni);               // Get the initial rotation of the object

    if (robot != "icubSim"){        
        bool coordOK = checkSaveDrag(target3DcoordsIni, theta, radius);       // check if the desired drag is save (heurisitcs).

        if (!coordOK){
            cout << "Object coordenated out of save reach" << endl;
            return false;
        }
    }

    // Action during the Affordance Motor Module execution transforms the end-effector from the hand to the tip pf the tool,
    // so the representation has to be inverted so it still shows the right tooltip while performing the action
    // and restored afterwards so it shows it also during not execution.
    cout << "Deactivating Tootip projection: " << endl; 
    Bottle cmd3DE,reply3DE;                 // bottles for O3DE
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("showTipProj");
    cmd3DE.addString("OFF");
    rpc3Dexp.write(cmd3DE, reply3DE);

    // Perform drag action
    cout << "Approaching to object on coords: (" << target3DcoordsIni[0] << ", " << target3DcoordsIni[1] << ", "<< target3DcoordsIni[2] << "). " <<endl;
    Bottle cmdAMM,replyAMM;                    // bottles for the Affordance Motor Module
    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("drag");                 // Set a position in the center in front of the robot
    cmdAMM.addDouble(target3DcoordsIni[0] - 0.04);   // Approach the end effector slightly behind the object to grab it properly
    cmdAMM.addDouble(target3DcoordsIni[1]);
    cmdAMM.addDouble(target3DcoordsIni[2]);   // Approach the center of the object, not its lower part.
    cmdAMM.addDouble(theta);
    cmdAMM.addDouble(radius);
    cmdAMM.addDouble(tilt);
    rpcAffMotor.write(cmdAMM, replyAMM);

    // Restore show tool coordinates
    cout << "Reactivating Tootip projection: " << endl;
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("showTipProj");
    cmd3DE.addString("ON");
    rpc3Dexp.write(cmd3DE, reply3DE);

    // Put action parameters on a vector so they can be sent
    actVec[0] = theta;
    actVec[1] = radius;

    goHomeExe();

    return true;
}



/**********************************************************/
bool Tool3DManager::drag3DExe(double x, double y, double z, double theta, double radius, double tilt, bool useTool)
{
    cout << endl<< "Performing drag3D action from angle " << theta <<" and radius "<< radius  <<", tilt:"<< tilt <<endl;
    Bottle cmdAMM,replyAMM;                    // bottles for the Affordance Motor Module
    Bottle cmdARE,replyARE;


    if (!useTool){
         yInfo()<<"Removing tool";
        // Remove any end effector so that action is carried wrt hand reference frame.
        cmdAMM.clear();replyAMM.clear();
        cmdAMM.addString("tool");
        cmdAMM.addString("remove");
        rpcAffMotor.write(cmdAMM, replyAMM);
    }

    // Action during the Affordance Motor Module execution transforms the end-effector from the hand to the tip pf the tool,
    // so the representation has to be inverted so it still shows the right tooltip while performing the action
    // and restored afterwards so it shows it also during not execution.
    //Point3D tooltip_tmp = tooltip;
    //tooltip.x = 0.0;    tooltip.y = 0.0;  tooltip.z = 0.0;

    cout << "Deactivating Tootip projection: " << endl; 
    Bottle cmd3DE,reply3DE;                 // bottles for O3DE
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("showTipProj");
    cmd3DE.addString("OFF");
    rpc3Dexp.write(cmd3DE, reply3DE);

    cout << "Approaching to object on coords: (" << x << ", " << y << ", "<< z << "). " <<endl;

    // Close hand on tool grasp
    if (robot != "icubSim"){
        cmdARE.clear();
        replyARE.clear();
        cmdARE.addString("look");
        cmdARE.addString("hand");
        cmdARE.addString(hand);
        cmdARE.addString("fixate");
        cmdARE.addString("block_eyes");
        cmdARE.addDouble(5.0);
        rpcMotorAre.write(cmdARE, replyARE);
    }

    cmdAMM.clear();replyAMM.clear();
    cmdAMM.addString("drag");                 // Set a position in the center in front of the robot
    cmdAMM.addDouble(x);   // Approach the end effector slightly behind the object to grab it properly
    cmdAMM.addDouble(y);
    cmdAMM.addDouble(z);   // Approach the center of the object, not its lower part.
    cmdAMM.addDouble(theta);
    cmdAMM.addDouble(radius);
    cmdAMM.addDouble(tilt);
    rpcAffMotor.write(cmdAMM, replyAMM);

    // Stop head moving for further visual processing
    if (robot != "icubSim"){
        cmdARE.clear();
        replyARE.clear();
        cmdARE.addString("idle");
        rpcMotorAre.write(cmdARE, replyARE);
    }

    // Restore show tool coordinates
    cout << "Reactivating Tootip projection: " << endl;
    cmd3DE.clear();   reply3DE.clear();
    cmd3DE.addString("showTipProj");
    cmd3DE.addString("ON");
    rpc3Dexp.write(cmd3DE, reply3DE);

    // Re-attach the tooltip for further actions
    if (!useTool){
        cmdAMM.clear();replyAMM.clear();
        cmdAMM.addString("tool");
        cmdAMM.addString("attach");
        cmdAMM.addString(hand.c_str());
        cmdAMM.addDouble(tooltip.x);
        cmdAMM.addDouble(tooltip.y);
        cmdAMM.addDouble(tooltip.z);
        rpcAffMotor.write(cmdAMM, replyAMM);
        if (!(replyAMM.toString() == "[ack]")){
            cout <<  "Aff Motor Module could not re-attach the tooltip " << endl;
            return false;
        }
    }

    goHomeExe();

    return true;
}

bool Tool3DManager::checkSaveDrag(Vector coords, double theta , double radius)
{
    // Define general workspace
    bool coordOK = true;
    if ((coords[0] < -0.6) || (coords[0] > -0.3)){
        coordOK = false;
    }
    if ((coords[1] < 0.15) || (coords[1] > 0.4)){
        coordOK = false;
    }
    if ((coords[2] < -0.17) || (coords[2] > 0.0)){
        coordOK = false;
    }

    // Check limit for specific actions
//    if (theta > 180) {
//        if (coords[0] > -0.)
//        {
//            coordOK = false;
//        }
//    }
    return coordOK;
}

//++++++++++++++++++++++++++++++ MAIN ++++++++++++++++++++++++++++++++//
/**********************************************************************/
int main(int argc, char *argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
        return -1;

    ResourceFinder rf;
    rf.setVerbose(true);
    // rf.setDefault("name","tool3DManager");
    //rf.setDefault("camera","left");
    rf.setDefault("robot","icub");
    //rf.setDefault("hand","right");
    rf.setDefaultContext("AffordancesProject");
    rf.setDefaultConfigFile("realTools.ini");
    //rf.setDefault("tracking_period","30");
    rf.configure(argc,argv);

    Tool3DManager tool3DManager;
    return tool3DManager.runModule(rf);
}
