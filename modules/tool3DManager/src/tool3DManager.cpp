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
    hand = rf.check("hand",Value("right")).asString().c_str();
    robot = rf.check("robot",Value("icub")).asString().c_str();

    tableHeight = rf.check("tableHeight", Value(-0.10)).asDouble();      // Height of the table in the robots coord frame

    // Attach server port to read RPC commands via thrift
    attach(rpcCmd);
    running = true;

    // XXX Attach client port to send RPC commands via thrift to toolFeatExt
    // tool3DFeat_IDLServer toolFeatExtclient;
    // toolFeatExtclient.yarp().attachAsClient(rpcFeatExt);

    // XXX old variables which may be useful
    /*

    actionDone = false;
    objCoords3DLoc = false;
    trackingObj = false;
    tipOnView = false;    
    toolDim.resize(3, 0.0);
    toolTipPix.resize(2,0.0);
    toolPoseName = "undef";
    */

    // Initialize effect measuring vectors
    target3DcoordsIni.resize(3, 0.0);
    target3DcoordsAfter.resize(3, 0.0);
    target3DrotIni.resize(3, 0.0);
    target3DrotAfter.resize(3, 0.0);
    effectVec.resize(3, 0.0);

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
    retRPC = retRPC && rpcSimToolLoader.open(("/"+name+"/simToolLoader:rpc").c_str()); //rpc client to interact with simtooloader module
    retRPC = retRPC && rpcSimulator.open(("/"+name+"/simulator:rpc").c_str());         //rpc client to interact with simulator
    retRPC = retRPC && rpcMotorAre.open(("/"+name+"/are:rpc").c_str());                //rpc server to query ARE
    retRPC = retRPC && rpcKarmaMotor.open(("/"+name+"/karmaMotor:rpc").c_str());       //rpc server to query Karma Motor    
    retRPC = retRPC && rpcKarmaFinder.open(("/"+name+"/karmaFinder:rpc").c_str());     //rpc server to query Karma Finder    

    //retRPC = retRPC && rpcToolShow.open(("/"+name+"/tool3Dshow:rpc").c_str());		   //rpc server to query toolExplorer Module
    retRPC = retRPC && rpcFeatExt.open(("/"+name+"/featExt:rpc").c_str());             //rpc server to query tool Feat Extraction module
    retRPC = retRPC && rpcObjFinder.open(("/"+name+"/objFind:rpc").c_str());           //rpc server to query objectFinder
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
    outDataPort.interrupt();
    userDataPort.interrupt();
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
    outDataPort.close();
    userDataPort.close();
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


bool Tool3DManager::goHome(bool hands){
    goHomeExe(hands);
	return true;
}

bool Tool3DManager::getTool(int toolI, int graspOr, double graspDisp){
    bool ok;
    if (strcmp(robot.c_str(),"icubSim")==0){
        ok = loadToolSim(toolI,graspOr,graspDisp);
    }else{
        ok = graspTool();
    }
    return ok;
}

bool Tool3DManager::slide(double theta, double radius){
    return slideExe(theta,radius);
}

bool Tool3DManager::pull(double theta, double radius){
    return pullExe(theta,radius);
}

bool Tool3DManager::compEff(){
    return computeEffect();
}



/**********************************************************
					PRIVATE METHODS
/**********************************************************/

/**********************************************************/
void Tool3DManager::goHomeExe(bool hands)
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
    fprintf(stdout,"Reply ARE: %s:\n",replyAre.toString().c_str());

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
bool Tool3DManager::loadToolSim(int toolI, int graspOr, double graspDisp)
{
    cout << endl<<"Loading tool " << toolI <<", or: "<< graspOr << ", displacement: " << graspDisp <<endl;
    // Remove any previous tool attached to the robot
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("remove");
    fprintf(stdout,"RPC to KarmaMotor: %s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"Reply from KarmaMotor: %s:\n",replyKM.toString().c_str());

    // Moving hand to center to receive tool on correct position - implemented by faking a push action to the center
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
    //fprintf(stdout,"Reply from KarmaMotor: %s:\n",replyKM.toString().c_str());

    // Query simtoolloader to create the virtual tool and object
    cout << "Loading tool " << toolI << " in the simulator " << endl;
    Bottle cmdSim,replySim;       // bottles for Simulator
    cmdSim.clear();   replySim.clear();
    cmdSim.addString("del");
    fprintf(stdout,"RPC to simtoolloader: %s\n",cmdSim.toString().c_str());
    rpcSimToolLoader.write(cmdSim, replySim); // Call simtoolloader to clean the world

    cmdSim.clear();   replySim.clear();
    cmdSim.addString("tool");
    cmdSim.addInt(toolI);               // tool
    cmdSim.addInt(1);                   // object -> Cube
    cmdSim.addInt(graspOr);             // orientation
    cmdSim.addInt(graspDisp);           // displacement
    rpcSimToolLoader.write(cmdSim, replySim); // Call simtoolloader to create the tool
    cout << "Sent RPC command to simtoolloader: " << cmdSim.toString() << "." <<endl;
    if (replySim.size() <1){
        cout << "simtoolloader cloudlnt load tool." << endl;
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

    // Query toolFeatExt to extract features
    cout << "Loading pointcloud to extract tooltip." << endl;
    Bottle cmdTFE,replyTFE;                 // bottles for toolFeatExt
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

    double ttxCanon, ttyCanon, ttzCanon;    // Receive coordinates of tooltip of tool in canonical position
    ttxCanon = replyTFE.get(0).asDouble();
    ttyCanon = replyTFE.get(1).asDouble();
    ttzCanon = replyTFE.get(2).asDouble();
    cout << "Tooltip of canonical model: x= "<< ttxCanon << ", y = " << ttyCanon << ", z = " << ttzCanon <<endl;

    // Transform canonical coordinates to correspond with tilt, rotation and displacemnt of the tool.
    double ttxRot =0.0, ttyRot=0.0, ttzRot=0.0;    // Receive coordinates of tooltip of tool in canonical position
    // Rotate first around Y axis to match tooltip to end-effector orientation
    ttxRot = ttxCanon*cos(graspOr*M_PI/180.0) - ttzCanon*sin(graspOr*M_PI/180.0);
    ttyRot = ttyCanon;
    ttzRot = ttxCanon*sin(graspOr*M_PI/180.0) + ttzCanon*cos(graspOr*M_PI/180.0);
    cout << "Tooltip of tool rotated " << graspOr << " degrees: x= "<< ttxRot << ", y = " << ttyRot << ", z = " << ttzRot << endl;

    // Now tilt 45 degrees arund Z to match the way in which the tool is held
    double ttxTilt =0.0, ttyTilt=0.0, ttzTilt=0.0;    // Receive coordinates of tooltip of tool in canonical position
    ttxTilt = ttxRot*cos(45.0*M_PI/180.0) - ttyRot*sin(45.0*M_PI/180.0);
    ttyTilt = ttxRot*sin(45.0*M_PI/180.0) + ttyRot*cos(45.0*M_PI/180.0);
    ttzTilt = ttzRot;
    cout << "Tooltip of tool rotated " << graspOr << " degrees and tilted 45 degrees: x= "<< ttxTilt << ", y = " << ttyTilt << ", z = " << ttzTilt << endl;

    // Finally add translation along -Y axis to match handle displacement
    tooltipX = ttxTilt;
    tooltipY = ttyTilt - graspDisp/100.0;
    tooltipZ = ttzTilt;

    cout << "Tooltip of tool rotated, tilted and displaced "<< graspDisp << "cm: x= "<< tooltipX << ", y = " << tooltipY << ", z = " << tooltipZ << endl;

    // Send the coordinates to Karmafinder to display it and get the tip pixel
    cout << endl << "Attaching tooltip." << endl;
    Bottle cmdKF,replyKF;       // bottles for Karma ToolFinder
    cmdKF.clear();replyKF.clear();
    cmdKF.addString("show");
    cmdKF.addDouble(tooltipX);
    cmdKF.addDouble(tooltipY);
    cmdKF.addDouble(tooltipZ);
    fprintf(stdout,"RCP to KarmaFinder %s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);
    fprintf(stdout,"Reply from KarmaFinder %s\n", replyKF.toString().c_str());

    // Attach the new tooltip to the "body schema"
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("attach");
    cmdKM.addString(hand);
    cmdKM.addDouble(tooltipX);
    cmdKM.addDouble(tooltipY);
    cmdKM.addDouble(tooltipZ);
    fprintf(stdout,"RPC to KarmaMotor%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"Reply from KarmaMotor %s:\n",replyKM.toString().c_str());

    fprintf(stdout,"Tool loaded and tooltip attached \n ");

    return true;
}

bool Tool3DManager::graspTool()
{
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
    fprintf(stdout,"Reply from ARE: %s\n", replyAre.toString().c_str());

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
    fprintf(stdout,"Reply from ARE: %s\n", replyAre.toString().c_str());
    Time::delay(0.5);

    // Go home and observe scenario.
    goHomeExe(false);

    return true;
}

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
        printf("Reply from simulator: %s \n", replySim.toString().c_str());

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
        coords3D.resize(3);

        Bottle cmdFinder,replyFinder;
        //fprintf(stdout,"Get 3D coords of tracked object:\n");
        cmdFinder.clear();        replyFinder.clear();
        cmdFinder.addString("getPointTrack");
        cmdFinder.addDouble(tableHeight);
        printf("RPC to objFinder: %s \n", cmdFinder.toString().c_str());
        rpcObjFinder.write(cmdFinder, replyFinder);
        printf("Reply from objFinder: %s \n", replyFinder.toString().c_str());

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
        printf("Reply from simulator: %s \n", replySim.toString().c_str());

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


bool Tool3DManager::slideExe(double theta, double radius)
{
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

    cout << "Approaching to push object on coords: (" << target3DcoordsIni[0] << ", " << target3DcoordsIni[2] << ", "<< target3DcoordsIni[2] << "). " <<endl;
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
    fprintf(stdout,"Reply from KarmaMotor: %s\n",replyKM.toString().c_str());


    goHomeExe();
    goHomeExe();
    // XXX Put action parameters on a port so they can be read

    return true;
}

bool Tool3DManager::pullExe(double theta, double radius)
{
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

    cout << "Approaching to push object on coords: (" << target3DcoordsIni[0] << ", " << target3DcoordsIni[1] << ", "<< target3DcoordsIni[2] << "). " <<endl;
    Bottle cmdKM,replyKM;                    // bottles for Karma Motor
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("pull");                 // Set a position in the center in front of the robot
    cmdKM.addDouble(target3DcoordsIni[0] - 0.03);   // Approach the end effector slightly behind the object to grab it properly
    cmdKM.addDouble(target3DcoordsIni[1]);
    cmdKM.addDouble(target3DcoordsIni[2]);   // Approach the center of the object, not its lower part.
    cmdKM.addDouble(theta);
    cmdKM.addDouble(radius);
    fprintf(stdout,"RPC to KarmaMotor: %s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"Reply from KarmaMotor: %s\n",replyKM.toString().c_str());


    goHomeExe();
    goHomeExe();
    // XXX Put action parameters on a port so they can be read

    return true;
}


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
    if(!getObjRot(target3DrotAfter))        // Get the rotation of the object after the action
    {
        cout << " Object rotation not available, cant compute effect"<< endl;
        return false;
    }
    cout << "Rotation after action: (" << target3DrotAfter[0] << ", " << target3DrotAfter[2] << ", "<< target3DrotAfter[2] << "). " <<endl;

    //To compute the displacement, we assume that the object hasnt move in the z axis (that is, has remained on the table)
    Vector displ = target3DcoordsAfter - target3DcoordsIni;
    double dx = displ[0];
    double dy = displ[1];

    double displDist = sqrt(dx*dx+dy*dy);  //sum of the squares of the differences
    double displAngle = atan2 (dy,dx) * 180 / M_PI;

    //To compute the rotation, we assume that the object has only rotated around axis Y (that is, has not been tipped)
    Vector rot = target3DrotAfter - target3DrotIni;
    double effectRot = rot[1];  // Rotation difference around Y axis

    // Put values into the effect Vector
    effectVec[0] = displDist;
    effectVec[1] = displAngle;
    effectVec[2] = effectRot;

    cout << "Object displaced " << displDist << " meters on " << displAngle << " direction, and rotated " << effectRot << " degrees."<< endl;

    // XXX put values on a port so they can be read

    return true;
}

//++++++++++++++++++++++++++++++ MAIN ++++++++++++++++++++++++++++++++//
/**********************************************************************/
int main(int argc, char *argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
        return -1;

    //YARP_REGISTER_DEVICES(icubmod)

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
