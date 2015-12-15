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

/** 
\defgroup affManager Manager for the tool-pose dependent affordance Experiment
 
Manager which controls the rest of the modules of the tool-pose dependent affordance Experiment
and enables user control.

\section intro_sec Description 
This modules relies user commands to the rest of the modules in the tool-pose dependent affordance 
Experiment, in order to allow for the required feature extraction, action performance and effect observation. \n
It also executes basic motor actions to observe the tool. \n
It works with minor differences on the real robot and the simulator, save the necessary ones to grab the tool. 
 
\section lib_sec Libraries 
- YARP libraries. 
- icubmod library.

\section parameters_sec Parameters 
--robot \e robot
- Select the robot to connect to.

--name \e name
- Select the stem-name of the module used to open up ports. 
  By default \e name is <i>affManager</i>.

--camera \e camera
- Select the eye camera to use. 
  By default \e camera is <i>left</i>.

--hand \e hand
- Select the arm which will grasp the tool and do the actions. 
  By default \e name is <i>right</i>.
  
\section portsa_sec Ports Accessed
Assume that iCubInterface (with ICartesianControl interface
implemented) is running. 
 
\section portsc_sec Ports Created 
- \e /affManager/rpc:i receives the information to execute the different possible tasks as a Bottle. 
It manages the following commands through thrift interface:
    -# <b>start</b>:  Start the module \n
     return true/false on success/failure.

    -# <b>start</b>: Quit the module \n
     return true/false on success/failure
    
    -# <b>reset</b>: Sets the experiment flow flags to false (action done, object located, tip on view).\n
     return true/false on success/failure      

    -# <b>goHome</b>: Adopt home position, also opening hands \n 
    return true/false on success/failure
    
    -# <b>goHomeNoHands</b>: Adopt home position while keeping hand pose (tools remain grasped) \n
     return true/false on success/failure
    
    -# <b>findToolDims</b>: Uses active exploration and non-linear optimization to compute the tool dimensions (only on real robot) \n
     Makes use of KarmaMotor, KarmaToolProjection and KarmaToolFinder \n
     return true/false on success/failure 

    -# <b>lookAtTool</b>: Moves the tool in hand in front of iCub eyes to a position where it can be observed fully. \n
     return true/false on success/failure of bringing the tool in front
     
    -# <b>observeTool</b>: Finds tool in hand and does Feature Extraction. \n
     return true/false on success/failure finding and extracting feats from tool
     
    -# <b>trackObj</b>: Gets the bounding box of the object from the user, and uses the template to train the particle filter tracker. \n
    return true/false on success/failure of finding/looking at object
    
    -# <b>getTool</b>: <i>tool deg</i> (default tool = 5, deg = 0).Performs the sequence to get the tool: \n
    On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i> and uses magnet to hold it to hand.\n
    On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i> and <i> deg</i> should correspond to the way in which the tool is given \n    
    return true/false on success/failure holding a tool
    
    -# <b>doAction</b>: <i>approach</i> (default approach = 0). Performs an drag trial on <i>approach</i> cm wrt the object. \n
    The trial consist on locating the object, executing the drag, locating the potentially displaced object and computing the effect.\n
    return true/false on success/failure 
    
    -# <b>trainDraw</b>:  Performs several drag trials with approaches from -5 to 5 cm to learn the mapping: \n
    return true/false on success/failure 
    
    -# <b>trainObserve</b>: <i>tool deg</i> (default tool = 5 deg = 0). Performs  feature extraction on the given tool 5 times from slighlty different prespectives \n
     return true/false on success/failure 

    -# <b>observeAndDo</b>: <i>tool deg trials</i> (default tool = 5, deg = 0, trials = 1). Performs the whole routine a given number of trials with the given tool in the given orientation:  looking at the tool, feature extraction, perform a drag action, and compute the effect. \n
    return true/false on success/failure 
   
    -# <b>predictDo</b>: <i>tool deg</i> (default tool = 5, deg = 0). Gets a tool, observes it (feature extraction), reads the predicted affordance from MATLAB and perform the best predicted action.
   Needs matlab script running prediction based on the model. 
   return true/false on success/failure 
     
    -# <b>testPredict</b>: <i>tool</i> (default tool = 5). Performs the prediction and action (predictDo routine) 5 times on each orientation with the given tool.
    return true/false on success/failure 
    
- \e /affManager/matlab:i   Receives the affordance vector prediction from matlab to select te max effect and perfomr an action.
- \e /affManager/user:i     Port for user input. Can be used for user selection of the tooltip, tool dimensions or other data. 
 
- \e /affManager/are:rpc            Send commands to the Actions Rendering Engine module. \n
- \e /affManager/sim:rpc            Send commands to the Simulator. \n
- \e /affManager/karmaMotor:rpc     Send commands to the Karma motor for tool use related actions. \n
- \e /affManager/karmaFinder:rpc    Send commands to the finder part of the KARMA application in order to display and locate the tooltip. \n
- \e /affManager/featExt:rpc        Send commands to the Feature Extraction Module. \n
- \e /affManager/affLearn:rpc       Send commands to the affordance Learning Module. \n
- \e /affManager/objFind:rpc        Send commands to the objectFinder Module. \n
- \e /affManager/toolBlob:rpc       Send commands to the toolBlobber Module.\n

- \e /affManager/data:o             Outputs affordance data for recording (action parameters, effect, etc) .\n
 
\section tested_os_sec Tested OS
Windows, Linux

\author Tanis Mar
*/ 


#include "tool3DManager.h"

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::math;
using namespace yarp::dev;


/**********************************************************
					PUBLIC METHODS
/**********************************************************/

/******************* RF overwrites ************************/
bool Tool3DManager::configure(ResourceFinder &rf)
{
	printf("Configuring...\n");

    name = rf.check("name",Value("tool3DManager")).asString().c_str();
    camera = rf.check("camera",Value("left")).asString().c_str();
    hand = rf.check("hand",Value("right")).asString().c_str();
    robot = rf.check("robot",Value("icub")).asString().c_str();

    if (strcmp(robot.c_str(),"icubSim")==0){
        tableHeight = rf.check("tableHeight", Value(-0.13)).asDouble();      // Height of the table in the robots coord frame
    }else{
        tableHeight = rf.check("tableHeight", Value(-0.10)).asDouble();      // Height of the table in the robots coord frame
    }
    

    // Attach server port to read RPC commands via thrift
    attach(rpcCmd);
    running = true;
    toolLoadedIdx = 0;
    trackingObj = false;

    // XXX old variables which may be useful
    /*
    actionDone = false;
    objCoords3DLoc = false;
    tipOnView = false;
    toolDim.resize(3, 0.0);
    toolTipPix.resize(2,0.0);
    toolPoseName = "undef";
    */

    // Read the Models configurations
    Bottle temp;
    string modelName = "obj";

    cout << "Loading models to buffer" << endl;
    bool noMoreModels = false;
    int n =1;
    while (!noMoreModels){      // read until there are no more objects.
        stringstream s;
        s.str("");
        s << modelName << n;
        string objNameNum = s.str();
        temp = rf.findGroup("objects").findGroup(objNameNum);

        if(!temp.isNull()) {
            cout << "model " << n << ", id: " << objNameNum;
            cout << ", model: " << temp.get(2).asString() << endl;
            models.push_back(temp);
            temp.clear();
            n++;
        }else {
            noMoreModels = true;
        }
    }
    int numberObjs = n;
    cout << "Loaded " << numberObjs << " objects. " << endl;


    // Initialize effect measuring vectors
    target3DcoordsIni.resize(3, 0.0);
    target3DcoordsAfter.resize(3, 0.0);
    target3DrotIni.resize(3, 0.0);
    target3DrotAfter.resize(3, 0.0);
    effectVec.resize(3, 0.0);

	//ports
	bool ret = true;  
    ret = ret && effDataPort.open(("/"+name+"/effData:o").c_str());                   // port to send data of computed effect out for recording
    ret = ret && actDataPort.open(("/"+name+"/actData:o").c_str());                   // port to send data of action parameters out for recording
    ret = ret && graspDataPort.open(("/"+name+"/graspData:o").c_str());                   // port to send data of action parameters out for recording
    ret = ret && matlabPort.open(("/"+name+"/matlab:i").c_str());                  // port to receive data from MATLAB processing
    if (!ret){
		printf("Problems opening ports\n");
		return false;
	}
    //rpc
    bool retRPC = true; 
    retRPC = rpcCmd.open(("/"+name+"/rpc:i").c_str());					   			   //rpc client to interact with the affManager
    retRPC = retRPC && rpcSimToolLoader.open(("/"+name+"/simToolLoader:rpc").c_str()); //rpc client to interact with simtooloader module
    retRPC = retRPC && rpcSimulator.open(("/"+name+"/simulator:rpc").c_str());         //rpc client to interact with simulator
    retRPC = retRPC && rpcMotorAre.open(("/"+name+"/are:rpc").c_str());                //rpc server to query ARE
    retRPC = retRPC && rpcKarmaMotor.open(("/"+name+"/karmaMotor:rpc").c_str());       //rpc server to query Karma Motor    
    retRPC = retRPC && rpcKarmaFinder.open(("/"+name+"/karmaFinder:rpc").c_str());     //rpc server to query Karma Finder    
    retRPC = retRPC && rpcFeatExt.open(("/"+name+"/featExt:rpc").c_str());             //rpc server to query tool Feat Extraction module
    //retRPC = retRPC && rpcToolShow.open(("/"+name+"/tool3Dshow:rpc").c_str());	   //rpc server to query toolExplorer Module

    retRPC = retRPC && rpcObjFinder.open(("/"+name+"/objFind:rpc").c_str());         //rpc server to query objectFinder
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
    effDataPort.interrupt();
    actDataPort.interrupt();
    graspDataPort.interrupt();
    matlabPort.interrupt();

    rpcCmd.interrupt();
    rpcSimToolLoader.interrupt();
    rpcSimulator.interrupt();
    rpcMotorAre.interrupt();
    rpcKarmaMotor.interrupt();
    rpcKarmaFinder.interrupt();

    rpcFeatExt.interrupt();
    rpcObjFinder.interrupt();

    return true;
}

/**********************************************************/
bool Tool3DManager::close()
{
    effDataPort.close();
    actDataPort.close();
    graspDataPort.close();
    matlabPort.close();

    rpcCmd.close();
    rpcSimToolLoader.close();
    rpcSimulator.close();
    rpcMotorAre.close();
    rpcKarmaMotor.close();
    rpcKarmaFinder.close();

    rpcFeatExt.close();
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
bool Tool3DManager::attach(yarp::os::RpcServer &source)
{
    return this->yarp().attachAsServer(source);
}

/* Atomic commands */
// Setting up commands
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

bool Tool3DManager::settableheight(double th){
    cout << "Setting table height to " << th << endl;
    tableHeight = th;
    return true;
}


bool Tool3DManager::goHome(bool hands){
    goHomeExe(hands);
	return true;
}

bool Tool3DManager::getToolFeats(){
    extractFeats();
    return true;
}

bool Tool3DManager::getTool(int toolI, double deg, double disp, double tilt){
    bool ok;
    if (strcmp(robot.c_str(),"icubSim")==0){
        ok = loadToolSim(toolI, deg, disp, tilt);
    }else{
        ok = loadToolReal(toolI, deg, disp, tilt);
    }
    return ok;
}

bool Tool3DManager::regrasp(double deg, double disp, double tilt){
    bool ok;
    if (toolLoadedIdx==0){
        cout << "A tool has to be loaded before regrasping is possible" << endl;
        return false;
    }

    ok = regraspExe(deg, disp, tilt);
    return ok;
}

bool Tool3DManager::slide(double theta, double radius){
    return slideExe(theta,radius);
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

// Functions to run experiment:
bool Tool3DManager::runToolPose(int toolI, double graspOr, double graspDisp, double graspTilt, int numAct){
    double thetaDiv = 360.0/numAct;
    double theta = 0.0;

    getTool(toolI, graspOr, graspDisp, graspTilt);  // (re-)grasp the tool with the given grasp parameters

    for (int i=1 ; i<=numAct ; i++){
        dragExe(theta,0.15);
        computeEffect();
        theta += thetaDiv;
    }
    return true;
}

// Functions to run experiment:
bool Tool3DManager::runRandPoses(int numPoses,int numAct){
    double thetaDiv = 360.0/numAct;
    double theta = 0.0;
    Rand randG; // YARP random generator

    int toolI_prev = round(randG.scalar(1,52));
    for (int p=1 ; p<=numPoses ; p++){
        bool toolOK = false;
        int toolI;
        while (!toolOK){                // Prevent known non-working tools to stop the run
            toolI = round(randG.scalar(1,52));
            if ((toolI == 8) || (toolI == 42) || (toolI == 43) || (toolI == 44) || (toolI == 45) ||(toolI == 50)||(toolI == toolI_prev)){
                cout << "unvalid tool" <<endl;}
            else{
                toolOK = true;
            }
        }

        double  graspOr = randG.scalar(-90,90);
        double  graspDisp = randG.scalar(-3,3);
        double  graspTilt = randG.scalar(0,45);

        cout << "Starting trial with orientation "<< graspOr <<", displacement "<<  graspDisp << " and tilt " << graspTilt << "." << endl;

        getTool(toolI, graspOr, graspDisp, graspTilt);  // re-grasp the tool with the given grasp parameters

        for (int i=1 ; i<=numAct ; i++){
            dragExe(theta,0.15);
            computeEffect();
            theta += thetaDiv;
        }
        toolI_prev = toolI;
    }
    return true;
}

bool Tool3DManager::runToolOr(int toolI, double graspOr, int numAct){

    // For each tool with an orientation, run all displacements (-2, 0, +2) cm
    for (int disp=-2 ; disp<3 ; disp += 2){                 // This is a loop for {-2,0,2}
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<< endl;
        cout << "Running trial with tool: " << toolI << " grasped at orientation "<< graspOr << ", and displacement "<< disp <<"."<< endl;
        runToolPose(toolI, graspOr, disp);
    }

    return true;
}

// Functions to run experiment:
bool Tool3DManager::runToolTrial(int toolI, int numAct){

    // For each tool, run all combinations of
    // x 3 Grasps: left front right
    // x 3 disps (-2, 0, +2) cm
    // x 8 thetas (every 45 degrees).

    for ( int ori = -90; ori < 100; ori = ori + 90){            // This is a loop for {-90, 0, 90}
        for (int disp=-2 ; disp<3 ; disp += 2){                 // This is a loop for {-2,0,2}
            cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<< endl;
            cout << "Running trial with tool: " << toolI << " grasped at orientation "<< ori << ", and displacement "<< disp <<"."<< endl;
            runToolPose(toolI, ori, disp);
        }
    }
    return true;
}


bool Tool3DManager::runExp(int toolIni, int toolEnd)
{
    for (int toolI=toolIni ; toolI<= toolEnd ; toolI++)
    {
        if (toolI != 8)             // XXX For some strange reason, loading tool 8 chrashes the simulator
            runToolTrial(toolI);
    }
    return true;
}



/**********************************************************
					PRIVATE METHODS
/**********************************************************/

/**********************************************************/
void Tool3DManager::goHomeExe(const bool hands)
{
    cout << endl << "Going home, hands: " << hands <<endl;
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
    fprintf(stdout,"RPC to ARE: %s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"  Reply ARE: %s:\n",replyAre.toString().c_str());

    /*
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("idle");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"Reply from ARE: %s:\n",replyAre.toString().c_str());
    */

    return;
}

/**********************************************************/
bool Tool3DManager::loadToolSim(const int toolI, const double graspOr,const double graspDisp, const double graspTilt)
{    
    double tiltValid = graspTilt;
    if (graspTilt > 90.0) {   tiltValid  = 90.0; }
    if (graspTilt < 0.0)  {   tiltValid  = 0.0; }
    cout << endl<<"Getting tool " << toolI <<" with orientation: "<< graspOr << ", displacement: " << graspDisp << "and tilt: " << graspTilt << endl;
    cout << "Tool present: " << toolLoadedIdx <<"."<< endl;

    Bottle cmdKM,replyKM;               // bottles for Karma Motor
    Bottle cmdSim,replySim;             // bottles for Simulator
    Bottle cmdTFE,replyTFE;             // bottles for toolFeatExt
    Bottle cmdKF,replyKF;       // bottles for Karma ToolFinder

    // Remove any end effector extension that might be.
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("remove");
    fprintf(stdout,"RPC to KarmaMotor: %s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"  Reply from KarmaMotor: %s\n",replyKM.toString().c_str());

    // Move hand to center to receive tool on correct position - implemented by faking a push action to the center to avoid iCart dependencies.
    cout << "Moving arm to a central position" << endl;
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("push");            // Set a position in the center in front of the robot
    cmdKM.addDouble(-0.25);
    cmdKM.addDouble(0.15);
    cmdKM.addDouble(0.05);
    cmdKM.addDouble(0.0);       // No angle
    cmdKM.addDouble(0.0);       // No radius
    //fprintf(stdout,"RPC to KarmaMotor: %s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    //fprintf(stdout,"  Reply from KarmaMotor: %s:\n",replyKM.toString().c_str());

    if (toolI != toolLoadedIdx)
    {
        cout << endl<<"Loading tool " << toolI <<", or: "<< graspOr << ", displacement: " << graspDisp <<endl;
        toolLoadedIdx = toolI;

        // Call simtoolloader to clean the world
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("del");
        fprintf(stdout,"RPC to simtoolloader: %s\n",cmdSim.toString().c_str());
        rpcSimToolLoader.write(cmdSim, replySim);

        // Call simtoolloader to create the tool
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("tool");
        cmdSim.addInt(toolI);               // tool
        cmdSim.addInt(1);                   // object -> Cube
        cmdSim.addInt(graspOr);             // orientation
        cmdSim.addInt(graspDisp);           // displacement
        cmdSim.addInt(tiltValid);           // tilt
        rpcSimToolLoader.write(cmdSim, replySim);
        cout << "Sent RPC command to simtoolloader: " << cmdSim.toString() << "." <<endl;

        // check succesful tool creation
        if (replySim.size() <1){
            cout << "simtoolloader couldn't load tool." << endl;
            return false;
        }
        cout << "Received reply: " << replySim.toString() << endl;

        // Get tool name from simtooloader response.
        // simtoolloader response is (sent command) (toolI loaded) (toolName) (object loaded)
        cout << "Retrieving tool name." << endl;
        string meshName = replySim.get(2).asString();
        string::size_type idx;
        idx = meshName.rfind('.');
        string cloudName = meshName.substr(0,idx);
        cloudName = "sim/"+ cloudName;
        cout << "cloud model: " << cloudName << endl;

        // Query toolFeatExt to load model to load 3D Pointcloud.
        cout << "Loading pointcloud to extract tooltip." << endl;

        cmdTFE.clear();   replyTFE.clear();
        cmdTFE.addString("loadModel");
        cmdTFE.addString(cloudName);
        rpcFeatExt.write(cmdTFE, replyTFE);
        cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
        if (replyTFE.size() <1){
            cout << "ToolFeatExt coudln't load the tool." << endl;
            return false;
        }
        cout << " Received reply: " << replyTFE.toString() << endl;

        // Extract Canonical tool features.
        if (!extractFeats()){
            cout << "ToolFeatExt coudln't extract the features." << endl;
            return false;
        }
        cout << " Canonical 3D features extracted" << endl;

        // Get the tooltip canonical coordinates wrt the hand coordinate frame from its bounding box.
        cout << "Getting canonical tooltip coordinates." << endl;
        cmdTFE.clear();   replyTFE.clear();
        cmdTFE.addString("getToolTip");
        rpcFeatExt.write(cmdTFE, replyTFE);
        cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
        if (replyTFE.size() <1){
            cout << "ToolFeatExt coudln't compute the tooltip." << endl;
            return false;
        }
        cout << " Received reply: " << replyTFE.toString() << endl;

        toolTipCanon.x = replyTFE.get(0).asDouble();
        toolTipCanon.y = replyTFE.get(1).asDouble();
        toolTipCanon.z = replyTFE.get(2).asDouble();
        cout << "Tooltip of canonical model: x= "<< toolTipCanon.x << ", y = " << toolTipCanon.y << ", z = " << toolTipCanon.z <<endl;

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
        cout << "Received reply: " << replySim.toString() << endl;
    }

    // Tasnform the tooltip from the canonical to the current grasp
    Point3D ttTrans;            // Point 3D srtuct for tooltips.
    transformToolTip(toolTipCanon, ttTrans, graspOr, graspDisp,  tiltValid);

    tooltip.x = ttTrans.x;
    tooltip.y = ttTrans.y;
    tooltip.z = ttTrans.z;
    cout << "Tooltip of tool in positon: x= "<< ttTrans.x << ", y = " << ttTrans.y << ", z = " << ttTrans.z <<endl;

    cout << endl << "Attaching tooltip." << endl;

    // Send the coordinates to Karmafinder to display it and get the tip pixel
    cmdKF.clear();replyKF.clear();
    cmdKF.addString("show");
    cmdKF.addDouble(tooltip.x);
    cmdKF.addDouble(tooltip.y);
    cmdKF.addDouble(tooltip.z);
    fprintf(stdout,"RCP to KarmaFinder %s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);
    fprintf(stdout,"  Reply from KarmaFinder %s\n", replyKF.toString().c_str());

    // Attach the new tooltip to the "body schema"
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("attach");
    cmdKM.addString(hand);
    cmdKM.addDouble(tooltip.x);
    cmdKM.addDouble(tooltip.y);
    cmdKM.addDouble(tooltip.z);
    fprintf(stdout,"RPC to KarmaMotor%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"  Reply from KarmaMotor: %s\n",replyKM.toString().c_str());

    fprintf(stdout,"Tool loaded and tooltip attached \n");

    // Send command to rotate the pointcloud so that features are extracted from rotated model
    cout << "Rotating PointCloud for coherent feature extraction." << endl;
    cmdTFE.clear();   replyTFE.clear();
    cmdTFE.addString("setCanonicalPose");
    cmdTFE.addDouble(graspOr);
    cmdTFE.addDouble(graspDisp);
    cmdTFE.addDouble(tiltValid);
    rpcFeatExt.write(cmdTFE, replyTFE);
    cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
    if (replyTFE.size() <1){
        cout << "ToolFeatExt coudln't rotate the pointcloud." << endl;
        return false;
    }
    cout << " Received reply: " << replyTFE.toString() << endl;

    if (!extractFeats()){
        cout << "ToolFeatExt coudln't extract the features." << endl;
        return false;
    }
    cout << " Oriented 3D features extracted" << endl;

    graspVec.clear();		// Clear to make space for new coordinates
    graspVec.resize(4);     // Resize to save orientation - displacement coordinates coordinates

    // Put action parameters on a port so they can be read
    graspVec[0] = toolI;
    graspVec[1] = graspOr;
    graspVec[2] = graspDisp;
    graspVec[3] = tiltValid;
    //graspDataPort.write(graspVec);

    return true;
}

/**********************************************************/
bool Tool3DManager::loadToolReal(const int toolI, const double graspOr, const double graspDisp, const double graspTilt)
{

    double tiltValid = graspTilt;
    if (graspTilt > 90.0) {   tiltValid  = 90.0; }
    if (graspTilt < 0.0)  {   tiltValid  = 0.0; }

    cout << endl<<"Getting tool " << toolI <<" with orientation: "<< graspOr << ", displacement: " << graspDisp << "and tilt: " << graspTilt << endl;
    toolLoadedIdx = toolI;

    Bottle cmdKM,replyKM;               // bottles for Karma Motor
    Bottle cmdTFE,replyTFE;             // bottles for toolFeatExt
    Bottle cmdKF,replyKF;       // bottles for Karma ToolFinder

    // Send commands to ARE to get the tool, close the hand and go to central position
    fprintf(stdout,"Reach me a tool please.\n");
    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("tato");
    cmdAre.addString(hand);
    cmdAre.addString("no_gaze");
    fprintf(stdout,"RPC to ARE: %s\n", cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"  Reply from ARE: %s\n", replyAre.toString().c_str());

    // Send commands to ARE to get the tool, close the hand and go to central position
    Time::delay(3);
    // Close hand on tool grasp
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("clto");
    cmdAre.addString(hand);
    cmdAre.addString("no_gaze");
    fprintf(stdout,"RPC to ARE: %s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    fprintf(stdout,"  Reply from ARE: %s\n", replyAre.toString().c_str());
    Time::delay(0.5);

    // Check if grasp was successful
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("get");
    cmdAre.addString("holding");
    rpcMotorAre.write(cmdAre, replyAre);
    if(!replyAre.get(0).asBool())
        return false;


    // Get tool name in order to load its 3Dmodel
    cout << "Retrieving tool name." << endl;
    string meshName = models[toolI].get(2).asString();
    string::size_type idx;
    idx = meshName.rfind('.');
    string cloudName = meshName.substr(0,idx);  //remove format
    cloudName = "real/"+ cloudName;
    cout << "cloud model: " << cloudName << endl;

    // Query toolFeatExt to load model to load 3D Pointcloud.
    cout << "Loading pointcloud to extract tooltip." << endl;

    cmdTFE.clear();   replyTFE.clear();
    cmdTFE.addString("loadModel");
    cmdTFE.addString(cloudName);
    rpcFeatExt.write(cmdTFE, replyTFE);
    cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
    if (replyTFE.size() <1){
        cout << "ToolFeatExt coudln't load the tool." << endl;
        return false;
    }
    cout << " Received reply: " << replyTFE.toString() << endl;

    // Extract Canonical tool features.
    if (!extractFeats()){
        cout << "ToolFeatExt coudln't extract the features." << endl;
        return false;
    }
    cout << " Canonical 3D features extracted" << endl;

    // Get the tooltip canonical coordinates wrt the hand coordinate frame from its bounding box.
    cout << "Getting canonical tooltip coordinates." << endl;
    cmdTFE.clear();   replyTFE.clear();
    cmdTFE.addString("getToolTip");
    rpcFeatExt.write(cmdTFE, replyTFE);
    cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
    if (replyTFE.size() <1){
        cout << "ToolFeatExt coudln't compute the tooltip." << endl;
        return false;
    }
    cout << " Received reply: " << replyTFE.toString() << endl;

    toolTipCanon.x = replyTFE.get(0).asDouble();
    toolTipCanon.y = replyTFE.get(1).asDouble();
    toolTipCanon.z = replyTFE.get(2).asDouble();
    cout << "Tooltip of canonical model: x= "<< toolTipCanon.x << ", y = " << toolTipCanon.y << ", z = " << toolTipCanon.z <<endl;

    // Tasnform the tooltip from the canonical to the current grasp
    Point3D ttTrans;            // Point 3D srtuct for tooltips.
    transformToolTip(toolTipCanon, ttTrans, graspOr, graspDisp, tiltValid);

    tooltip.x = ttTrans.x;
    tooltip.y = ttTrans.y;
    tooltip.z = ttTrans.z;
    cout << "Tooltip of tool in positon: x= "<< ttTrans.x << ", y = " << ttTrans.y << ", z = " << ttTrans.z <<endl;

    cout << endl << "Attaching tooltip." << endl;

    // Send the coordinates to Karmafinder to display it and get the tip pixel
    cmdKF.clear();replyKF.clear();
    cmdKF.addString("show");
    cmdKF.addDouble(tooltip.x);
    cmdKF.addDouble(tooltip.y);
    cmdKF.addDouble(tooltip.z);
    fprintf(stdout,"RCP to KarmaFinder %s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);
    fprintf(stdout,"  Reply from KarmaFinder %s\n", replyKF.toString().c_str());

    // Attach the new tooltip to the "body schema"
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("attach");
    cmdKM.addString(hand);
    cmdKM.addDouble(tooltip.x);
    cmdKM.addDouble(tooltip.y);
    cmdKM.addDouble(tooltip.z);
    fprintf(stdout,"RPC to KarmaMotor%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"  Reply from KarmaMotor: %s\n",replyKM.toString().c_str());

    fprintf(stdout,"Tool loaded and tooltip attached \n ");

    // Send command to rotate the pointcloud so that features are extracted from rotated model
    cout << "Rotating PointCloud for coherent feature extraction." << endl;
    cmdTFE.clear();   replyTFE.clear();
    cmdTFE.addString("setCanonicalPose");
    cmdTFE.addDouble(graspOr);
    cmdTFE.addDouble(graspDisp);
    cmdTFE.addDouble(tiltValid);
    rpcFeatExt.write(cmdTFE, replyTFE);
    cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
    if (replyTFE.size() <1){
        cout << "ToolFeatExt coudln't rotate the pointcloud." << endl;
        return false;
    }
    cout << " Received reply: " << replyTFE.toString() << endl;

    if (!extractFeats()){
        cout << "ToolFeatExt coudln't extract the features." << endl;
        return false;
    }
    cout << " Oriented 3D features extracted" << endl;

    graspVec.clear();		// Clear to make space for new coordinates
    graspVec.resize(4);     // Resize to save orientation - displacement coordinates coordinates

    // Put action parameters on a port so they can be read
    graspVec[0] = toolI;
    graspVec[1] = graspOr;
    graspVec[2] = graspDisp;
    graspVec[3] = tiltValid;

    //graspDataPort.write(graspVec);

    // Go home and observe scenario.
    goHomeExe(false);
    return true;
}

/**********************************************************/
bool Tool3DManager::regraspExe(const double graspOr, const double graspDisp, const double graspTilt)
    {
    double tiltValid = graspTilt;
    if (graspTilt > 90.0) {   tiltValid  = 90.0; }
    if (graspTilt < 0.0)  {   tiltValid  = 0.0; }

    // Query simtoolloader to rotate the virtual tool
    cout << endl<< "Regrasping tool " << toolLoadedIdx <<" with orientation: "<< graspOr << " and displacement: " << graspDisp <<endl;

    // Call simtoolloader to create the tool
    if (strcmp(robot.c_str(),"icubSim")==0){
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
            cout << "simtoolloader cloudln't regrasp tool." << endl;
            return false;
        }
        cout << "Received reply: " << replySim.toString() << endl;
    }

    // Tasnform the tooltip from the canonical to the current grasp
    Point3D ttTrans;            // Point 3D srtuct for tooltips.
    transformToolTip(toolTipCanon, ttTrans, graspOr, graspDisp,  tiltValid);

    tooltip.x = ttTrans.x;
    tooltip.y = ttTrans.y;
    tooltip.z = ttTrans.z;
    cout << "Tooltip of tool in positon: x= "<< ttTrans.x << ", y = " << ttTrans.y << ", z = " << ttTrans.z <<endl;

    cout << endl << "Attaching tooltip." << endl;

    // Send the coordinates to Karmafinder to display it and get the tip pixel
    Bottle cmdKF,replyKF;       // bottles for Karma Tool Finder
    cmdKF.clear();replyKF.clear();
    cmdKF.addString("show");
    cmdKF.addDouble(tooltip.x);
    cmdKF.addDouble(tooltip.y);
    cmdKF.addDouble(tooltip.z);
    fprintf(stdout,"RCP to KarmaFinder %s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);
    fprintf(stdout,"  Reply from KarmaFinder %s\n", replyKF.toString().c_str());

    // Attach the new tooltip to the "body schema"
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("attach");
    cmdKM.addString(hand);
    cmdKM.addDouble(tooltip.x);
    cmdKM.addDouble(tooltip.y);
    cmdKM.addDouble(tooltip.z);
    fprintf(stdout,"RPC to KarmaMotor%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"  Reply from KarmaMotor: %s\n",replyKM.toString().c_str());

    fprintf(stdout,"Tool loaded and tooltip attached \n");

    // Send command to rotate the pointcloud so that features are extracted from rotated model
    cout << "Rotating PointCloud for coherent feature extraction." << endl;
    Bottle cmdTFE,replyTFE;       // bottles for Tool Feature Extractor
    cmdTFE.clear();   replyTFE.clear();
    cmdTFE.addString("setCanonicalPose");
    cmdTFE.addDouble(graspOr);
    cmdTFE.addDouble(graspDisp);
    cmdTFE.addDouble(tiltValid);
    rpcFeatExt.write(cmdTFE, replyTFE);
    cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
    if (replyTFE.size() <1){
    cout << "ToolFeatExt coudln't rotate the pointcloud." << endl;
    return false;
    }
    cout << " Received reply: " << replyTFE.toString() << endl;

    if (!extractFeats()){
    cout << "ToolFeatExt coudln't extract the features." << endl;
    return false;
    }
    cout << " Oriented 3D features extracted" << endl;

    graspVec.clear();		// Clear to make space for new coordinates
    graspVec.resize(4);     // Resize to save orientation - displacement coordinates coordinates

    // Put action parameters on a port so they can be read
    graspVec[0] = toolLoadedIdx;
    graspVec[1] = graspOr;
    graspVec[2] = graspDisp;
    graspVec[3] = tiltValid;
    //graspDataPort.write(graspVec);

    return true;
}

/**********************************************************/
bool Tool3DManager::extractFeats()
{    // Query toolFeatExt to extract features
    cout << "Loading pointcloud to extract tooltip." << endl;
    Bottle cmdTFE,replyTFE;                 // bottles for toolFeatExt
    cmdTFE.clear();   replyTFE.clear();
    cmdTFE.addString("getFeats");
    rpcFeatExt.write(cmdTFE, replyTFE);
    cout << "Sent RPC command to toolFeatExt: " << cmdTFE.toString() << "."<< endl;
    if (replyTFE.size() <1){
        cout << "ToolFeatExt coudln't extract the features." << endl;
        return false;
    }
    return true;
}


/**********************************************************/
void Tool3DManager::transformToolTip(const Point3D ttCanon, Point3D &tooltipTrans, const double graspOr, const double graspDisp, const double graspTilt)
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
    fprintf(stdout,"RPC to objFinder %s:\n", cmdFinder.toString().c_str());
    rpcObjFinder.write(cmdFinder, replyFinder);
    fprintf(stdout,"  Reply from objFinder %s:\n",replyFinder.toString().c_str());

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
    if (strcmp(robot.c_str(),"icubSim")==0){
        Bottle cmdSim,replySim;
        cout << endl <<"Geting 3D coords of object." <<endl;
        cmdSim.clear();        replySim.clear();
        cmdSim.addString("world");
        cmdSim.addString("get");
        cmdSim.addString("box");
        cmdSim.addInt(1);
        printf("RPC to simulator: %s \n", cmdSim.toString().c_str());
        rpcSimulator.write(cmdSim, replySim);
        printf("  Reply from simulator: %s \n", replySim.toString().c_str());

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

    // If object is not being tracked, initialize tracking.
        if (!trackingObj)
            trackObjExe();

        // Get the 2D coordinates of the object from objectFinder
        coords3D.resize(3);

        Bottle cmdFinder,replyFinder;
        //fprintf(stdout,"Get 3D coords of tracked object:\n");
        cmdFinder.clear();        replyFinder.clear();
        cmdFinder.addString("getPointTrack");
        cmdFinder.addDouble(tableHeight);
        printf("RPC to objFinder: %s \n", cmdFinder.toString().c_str());
        rpcObjFinder.write(cmdFinder, replyFinder);
        printf("  Reply from objFinder: %s \n", replyFinder.toString().c_str());

        if (replyFinder.size() >1){
            coords3D(0) = replyFinder.get(1).asList()->get(0).asDouble();
            coords3D(1) = replyFinder.get(1).asList()->get(1).asDouble();
            coords3D(2) = replyFinder.get(1).asList()->get(2).asDouble();
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
    if (strcmp(robot.c_str(),"icubSim")==0){
        Bottle cmdSim,replySim;
        //fprintf(stdout,"Get 3D coords of tracked object:\n");
        cmdSim.clear();        replySim.clear();
        cmdSim.addString("world");
        cmdSim.addString("rot");
        cmdSim.addString("box");
        cmdSim.addInt(1);
        printf("RPC to simulator: %s \n", cmdSim.toString().c_str());
        rpcSimulator.write(cmdSim, replySim);
        printf("  Reply from simulator: %s \n", replySim.toString().c_str());

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
        cout << "XXX rotation detection not yet implemented for the real robot." << endl;
        return false;
    }
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

    cout << "Approaching to coords: (" << target3DcoordsIni[0] << ", " << target3DcoordsIni[2] << ", "<< target3DcoordsIni[2] << "). " <<endl;
    Bottle cmdKM,replyKM;                    // bottles for Karma Motor
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("slide");                 // Set a position in the center in front of the robot
    cmdKM.addDouble(target3DcoordsIni[0]);
    cmdKM.addDouble(target3DcoordsIni[1]);
    cmdKM.addDouble(target3DcoordsIni[2] + 0.03);   // Approach the center of the object, not its lower part.
    cmdKM.addDouble(theta);
    cmdKM.addDouble(radius);
    fprintf(stdout,"RPC to KarmaMotor: %s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"  Reply from KarmaMotor: %s\n",replyKM.toString().c_str());

    // Put action parameters on a port so they can be read
    actVec[0] = theta;
    actVec[1] = radius;
    //actDataPort.write(actVec);

    goHomeExe();
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

    cout << "Approaching to object on coords: (" << target3DcoordsIni[0] << ", " << target3DcoordsIni[1] << ", "<< target3DcoordsIni[2] << "). " <<endl;
    Bottle cmdKM,replyKM;                    // bottles for Karma Motor
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("drag");                 // Set a position in the center in front of the robot
    cmdKM.addDouble(target3DcoordsIni[0] - 0.03);   // Approach the end effector slightly behind the object to grab it properly
    cmdKM.addDouble(target3DcoordsIni[1]);
    cmdKM.addDouble(target3DcoordsIni[2]);   // Approach the center of the object, not its lower part.
    cmdKM.addDouble(theta);
    cmdKM.addDouble(radius);
    cmdKM.addDouble(tilt);
    fprintf(stdout,"RPC to KarmaMotor: %s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"  Reply from KarmaMotor: %s\n",replyKM.toString().c_str());

    // Put action parameters on the vector so that they can be sent
    actVec[0] = theta;
    actVec[1] = radius;
    //actDataPort.write(actVec);

    goHomeExe();
    goHomeExe();

    return true;
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

    if (strcmp(robot.c_str(),"icubSim")==0){
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

    // put values on a port so they can be read
    // effDataPort.write(effectVec);

    if (strcmp(robot.c_str(),"icubSim")==0){
        // Put the cube back in place to restart round:
        Bottle cmdSim,replySim;       // bottles for Simulator
        cmdSim.clear();   replySim.clear();
        cmdSim.addString("move");
        cmdSim.addInt(1);                   // object -> Cube
        rpcSimToolLoader.write(cmdSim, replySim); // Call simtoolloader to create the tool
        cout << "Sent RPC command to simtoolloader: " << cmdSim.toString() << "." <<endl;
        cout << " Received reply: " << replySim.toString() << endl;
        Time::delay(0.3); // give time for the cube to reach its position before jumping to next step
    }
    // Send all data so it can be read and saved.
    sendAffData();

    return true;
}

bool Tool3DManager::sendAffData()
{
    // Send (action - grasp - effect) parameters together so that they are synced

    // put values on a port so they can be read
    actDataPort.write(actVec);
    graspDataPort.write(graspVec);
    effDataPort.write(effectVec);

    return true;
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
