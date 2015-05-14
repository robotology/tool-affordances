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
    
    -# <b>doAction</b>: <i>approach</i> (default approach = 0). Performs an pull trial on <i>approach</i> cm wrt the object. \n
    The trial consist on locating the object, executing the pull, locating the potentially displaced object and computing the effect.\n
    return true/false on success/failure 
    
    -# <b>trainDraw</b>:  Performs several pull trials with approaches from -5 to 5 cm to learn the mapping: \n
    return true/false on success/failure 
    
    -# <b>trainObserve</b>: <i>tool deg</i> (default tool = 5 deg = 0). Performs  feature extraction on the given tool 5 times from slighlty different prespectives \n
     return true/false on success/failure 

    -# <b>observeAndDo</b>: <i>tool deg trials</i> (default tool = 5, deg = 0, trials = 1). Performs the whole routine a given number of trials with the given tool in the given orientation:  looking at the tool, feature extraction, perform a pull action, and compute the effect. \n
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
    name = rf.check("hand",Value("right")).asString().c_str();
    robot = rf.check("robot",Value("icub")).asString().c_str();

    tableHeight = rf.check("tableHeight", Value(-0.10)).asDouble();      // Height of the table in the robots coord frame

    attach(rpcCmd);

    // XXX old variables which may be useful
	running = true;
    actionDone = false;
    objCoords3DLoc = false;
    trackingObj = false;
    tipOnView = false;

    toolDim.resize(3, 0.0);
    toolTipPix.resize(2,0.0);
    target3DcoordsIni.resize(3, 0.0);		// Keeps the target position in 3D before the action
    target3DcoordsAfter.resize(3, 0.0);	    // Keeps the target position in 3D after the action
    toolPoseName = "undef";

	//ports
	bool ret = true;  
    ret = ret && outDataPort.open(("/"+name+"/data:o").c_str());                   // port to send data out for recording
    ret = ret && matlabPort.open(("/"+name+"/matlab:i").c_str());                  // port to receive data from MATLAB processing
    if (!ret){
		printf("Problems opening ports\n");
		return false;
	}
    //rpc
    bool retRPC = true; 
    retRPC = rpcCmd.open(("/"+name+"/rpc:i").c_str());					   			   //rpc client to interact with the affManager
    retRPC = retRPC && rpcSimTL.open(("/"+name+"/simTL:rpc").c_str());				   //rpc client to interact with the simulator
    retRPC = retRPC && rpcMotorAre.open(("/"+name+"/are:rpc").c_str());                //rpc server to query ARE
    retRPC = retRPC && rpcKarmaMotor.open(("/"+name+"/karmaMotor:rpc").c_str());       //rpc server to query Karma Motor    
    retRPC = retRPC && rpcKarmaFinder.open(("/"+name+"/karmaFinder:rpc").c_str());     //rpc server to query Karma Finder    

    retRPC = retRPC && rpcToolShow.open(("/"+name+"/tool3Dshow:rpc").c_str());		   //rpc server to query toolExplorer Module
    retRPC = retRPC && rpcFeatExt.open(("/"+name+"/featExt:rpc").c_str());             //rpc server to query tool Feat Extraction module
    retRPC = retRPC && rpcObjFinder.open(("/"+name+"/objFind:rpc").c_str());           //rpc server to query objectFinder
	if (!retRPC){
		printf("Problems opening rpc ports\n");
		return false;
	}

    /*
	// Cartesian Controller Interface for arm
	Property optCart;
	optCart.put("device","cartesiancontrollerclient");
    optCart.put("remote","/"+robot+"/cartesianController/"+hand+"_arm");
    optCart.put("local","/"+name+"/cartesian_client/"+hand+"_arm");
	if (!clientCart.open(optCart))
		return false;    
    clientCart.view(icart);	// open the view
    clientCart.view(iTorso);	// open the view
    icart->storeContext(&cartCntxt);

    // Cartesian Controller Interface for torso
	Property optPosTorso;
	optPosTorso.put("device","remote_controlboard");
	optPosTorso.put("remote","/"+robot+"/torso");
	optPosTorso.put("local","/"+name+"/torsoControl/torso");
	if (!clientTorso.open(optPosTorso))
		return false;
	clientTorso.view(iTorso);	// open the view

    // Gaze Controller Interface
    Property optGaze("(device gazecontrollerclient)");
    optGaze.put("remote","/iKinGazeCtrl");
    optGaze.put("local","/"+name+"/gaze_client");

    if (!clientGaze.open(optGaze))
        return false;
    clientGaze.view(igaze);

    igaze->storeContext(&gazeCntxt);        // store the original the context

    igaze->setNeckTrajTime(0.8);
    igaze->setEyesTrajTime(0.4);
    igaze->setTrackingMode(false);
    igaze->blockEyes(0.0);

    */
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
    outDataPort.interrupt();
    userDataPort.interrupt();
    matlabPort.interrupt();

    rpcCmd.interrupt();
    rpcSimTL.interrupt();
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
    outDataPort.close();
    userDataPort.close();
    matlabPort.close();

    rpcCmd.close();
    rpcSimTL.close();
    rpcMotorAre.close();
    rpcKarmaMotor.close();
    rpcKarmaFinder.close();

    rpcFeatExt.close();
    rpcObjFinder.close();

    clientGaze.close();
    clientCart.close();
    clientTorso.close();
    igaze->restoreContext(gazeCntxt);       // ... and then retrieve the stored context_0
    icart->restoreContext(cartCntxt);

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


bool Tool3DManager::goHome(bool hands){
    goHomeExe(hands);
	return true;
}

bool Tool3DManager::getTool(int toolI, int graspOr, double graspDisp){
    if (strcmp(robot.c_str(),"icubSim")==0){
        loadToolSim(toolI,graspOr,graspDisp);
    }else{
        graspTool();
    }

    return true;
}


/**********************************************************
					PRIVATE METHODS
/**********************************************************/

/**********************************************************/
void Tool3DManager::goHomeExe(bool hands)
{
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
    fprintf(stdout,"RPC to ARE %s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"Reply ARE %s:\n",replyAre.toString().c_str());

    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("idle");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"Reply from ARE: %s:\n",replyAre.toString().c_str());

    return;
}

/**********************************************************/
void Tool3DManager::loadToolSim(int toolI, int graspOr, double graspDisp)
{
    // Query simtoolloader to create the virtual tool and object
    Bottle cmdSim,replySim;       // bottles for Simulator
    cmdSim.clear();   replySim.clear();
    cmdSim.addString("del");
    fprintf(stdout,"RPC to Simulator %s\n",cmdSim.toString().c_str());
    rpcSimTL.write(cmdSim, replySim); // Call simtoolloader to clean the world

    cmdSim.clear();   replySim.clear();
    cmdSim.addString("tool");
    cmdSim.addInt(toolI);               // tool
    cmdSim.addInt(1);                   // object -> Cube
    cmdSim.addInt(graspOr);             // orientation
    cmdSim.addInt(graspDisp);           // displacement

    fprintf(stdout,"RPC to simulator %s\n",cmdSim.toString().c_str());
    rpcSimTL.write(cmdSim, replySim); // Call simtoolloader to create the tool

    // Get tool name from simtooloader response.
    // simtoolloader response is (sent command) (toolI loaded) (toolName) (object loaded)
    string meshName = replySim.get(3).asString();
    cout << "Tool model " << meshName << " has been loaded in the simulator." << endl;
    string::size_type idx;
    idx = meshName.rfind('.');
    string cloudName = meshName.substr(0,idx);
    cloudName = "sim/"+ cloudName;
    cout << "cloud model: " << cloudName << endl;


    // Query toolFeatExt to load model
    Bottle cmdTFE,replyTFE;                 // bottles for toolFeatExt
    cmdTFE.clear();   replyTFE.clear();
    cmdTFE.addString("loadModel");
    cmdTFE.addString(cloudName);
    fprintf(stdout,"RPC to toolFeatExt %s\n",cmdTFE.toString().c_str());
    rpcFeatExt.write(cmdSim, replySim);

    // XXX Send tool name to tool3Dshow to get the pointcloud of the tool to check.

    // Get the X and Y dimensions of the tool from its bounding box.
    // XXX XXX the canoncial tool is oriente to -Y, while the load on the simulator it at 45 degree. So either tilt the canonicals 45 to get the bounding box
    // , or do the actions with the tool to -Y

    // Send the XYZ dimensions of the tool to karmaMotor to attach end-effector
    // Send the XYZ dimensions of the tool to karmaToolFinder to display end-effector

}


//+++++++++++++++++++ MAIN ++++++++++++++++++++++++++++++++
/**********************************************************/
int main(int argc, char *argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
        return -1;

	YARP_REGISTER_DEVICES(icubmod)

    ResourceFinder rf;
    rf.setVerbose(true);
    // rf.setDefault("name","tool3DManager");
    //rf.setDefault("camera","left");
    //rf.setDefault("robot","icub");
    //rf.setDefault("hand","right");
    rf.setDefaultContext("tool3DManager");
    //rf.setDefaultConfigFile("affManager.ini");
    //rf.setDefault("tracking_period","30");
    rf.configure(argc,argv);

    Tool3DManager tool3DManager;
    return tool3DManager.runModule(rf);
}
