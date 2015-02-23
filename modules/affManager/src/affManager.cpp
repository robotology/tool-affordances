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


#include "affManager.h"

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::math;
using namespace yarp::dev;


/**********************************************************
					PUBLIC METHODS
/**********************************************************/


bool AffManager::configure(ResourceFinder &rf)
{
	printf("Configuring...\n");

    name=rf.find("name").asString().c_str();
    camera=rf.find("camera").asString().c_str();
    if ((camera!="left") && (camera!="right"))
        camera="left";

    hand=rf.find("hand").asString().c_str();
    if ((hand!="left") && (hand!="right"))
        hand="right";

	robot = rf.find("robot").asString().c_str();
    if ((robot!="icubSim") && (robot!="icub"))
        robot="icub";

    tableHeight = rf.check("tableHeight", Value(-0.10)).asDouble();      // Height of the table in the robots coord frame

	attach(rpcCmd);
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
    ret = ret &&  userDataPort.open(("/"+name+"/user:i").c_str());	                       // input port to receive data from user
    ret = ret && outDataPort.open(("/"+name+"/data:o").c_str());                   // port to send data out for recording
    ret = ret && matlabPort.open(("/"+name+"/matlab:i").c_str());                  // port to send data out for recording
    if (!ret){
		printf("Problems opening ports\n");
		return false;
	}
    //rpc
    bool retRPC = true; 
    retRPC = rpcCmd.open(("/"+name+"/rpc:i").c_str());					   			   //rpc client to interact with the affManager
    retRPC = retRPC && rpcSim.open(("/"+name+"/sim:rpc").c_str());					   //rpc client to interact with the simulator
    retRPC = retRPC && rpcMotorAre.open(("/"+name+"/are:rpc").c_str());                //rpc server to query ARE
    retRPC = retRPC && rpcKarmaMotor.open(("/"+name+"/karmaMotor:rpc").c_str());       //rpc server to query Karma Motor    
    retRPC = retRPC && rpcKarmaFinder.open(("/"+name+"/karmaFinder:rpc").c_str());     //rpc server to query Karma Finder    

	retRPC = retRPC && rpcFeatExt.open(("/"+name+"/featExt:rpc").c_str());			   //rpc server to query featExtractin Module
    retRPC = retRPC && rpcAffLearn.open(("/"+name+"/affLearn:rpc").c_str());		   //rpc server to query affLearn
    retRPC = retRPC && rpcObjFinder.open(("/"+name+"/objFind:rpc").c_str());           //rpc server to query objectFinder
    retRPC = retRPC && rpcToolBlobber.open(("/"+name+"/toolBlob:rpc").c_str());           //rpc server to query objectFinder
	if (!retRPC){
		printf("Problems opening rpc ports\n");
		return false;
	}

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

    printf("Manager configured correctly \n");

	return true;
}


/**********************************************************/
bool AffManager::updateModule()
{
	if (!running)
        return false;
	return true;
}

/**********************************************************/
bool AffManager::interruptModule()
{
    outDataPort.interrupt();
    userDataPort.interrupt();
    matlabPort.interrupt();

    rpcCmd.interrupt();
    rpcSim.interrupt();
    rpcMotorAre.interrupt();
    rpcKarmaMotor.interrupt();
    rpcKarmaFinder.interrupt();

    rpcFeatExt.interrupt();
    rpcAffLearn.interrupt();
    rpcObjFinder.interrupt();
    rpcToolBlobber.interrupt();

    return true;
}

/**********************************************************/
bool AffManager::close()
{
    outDataPort.close();
    userDataPort.close();
    matlabPort.close();

    rpcCmd.close();
    rpcSim.close();
    rpcMotorAre.close();
    rpcKarmaMotor.close();
    rpcKarmaFinder.close();

    rpcFeatExt.close();
    rpcAffLearn.close();

    rpcObjFinder.close();
    rpcToolBlobber.close();

    clientGaze.close();
    clientCart.close();
    clientTorso.close();
    igaze->restoreContext(gazeCntxt);       // ... and then retrieve the stored context_0
    icart->restoreContext(cartCntxt);

	running = false;

    return true;
}

/**********************************************************/
double AffManager::getPeriod()
{
    return 0.1;
}

/******************RPC HANDLING METHODS*******************/
bool AffManager::attach(yarp::os::RpcServer &source)
{
    return this->yarp().attachAsServer(source);
}

/* Atomic commands */
// Setting up commands
bool AffManager::start(){
	std::cout << "Starting!" << std::endl;
	running = true;
	return true;
}
bool AffManager::quit(){
	std::cout << "Quitting!" << std::endl; 
	running = false;
	return true;
}
bool AffManager::goHome(){
	goHomeExe();
	return true;
}
bool AffManager::goHomeNoHands(){
	goHomeNoHandsExe();
	return true;
}

bool AffManager::findToolDims(){
	findToolDimsExe();
	return true;
}

// Observation and fExt commands
bool AffManager::lookAtTool(){
	return lookAtToolExe();
}
bool AffManager::observeTool(){
	observeToolExe();
	return true;
}

// Action and effect commands
bool AffManager::trackObj(){
	trackObjExe();
	return true;
}

bool AffManager::reset(){
    finishRound();
    return true;
}

/* Combined routines */
bool AffManager::getTool(const int toolI, const int deg){
    
    //set the label for posterior trials on this pose
    std::ostringstream pose; 
    pose << "tool_" << toolI << "_or_" << deg;
    setLabel(pose.str());

    // Get the tool in the hand
    if (robot== "icubSim")    {
        simTool(toolI, deg);
        // Tooltip position wrt hand
        toolDim[0] = 0.16;
        toolDim[1] = -0.16;
        toolDim[2] = 0.03;

    }else {
        askForTool();
        std::cout << "Give the tool to the iCub oriented " <<  deg << "!" << std::endl;
        if (!graspTool()){
            std::cout << "Tool could not be grasped properly, stopping!" << std::endl;
            return false;
        }
        // Find tool dimensions using KARMA
        //findToolDimsExe();    //  For similar tools it can be performed once and then subsittuted by fixed values like below.
        toolDim[0] = 0.16;
        toolDim[1] = -0.16;
        toolDim[2] = -0.04;
    }
    
    // Show and attach tooltip.
    attachTool();

    // Go home position with tool
    // goHomeNoHandsExe();
	return true;
}

bool AffManager::doAction(const int approach){
	goHomeNoHandsExe();
    if (!trackingObj)
        trackObjExe();
    Time::delay(2);
	if (locateObj())
	{
		slideAction(approach);
		goHomeNoHandsExe();
		Time::delay(2);
		locateObj();
		computeEffect();
	}
    finishRound();
	return true;
}

bool AffManager::trainDraw(){
    if (!trackingObj)
        trackObjExe();
	for ( int app = -5; app <= 5 ; app++ ){
        printf("\n Starting approach to %g\n", app/100.0);
        doAction(app);
        //printf("Action with approach %g Finished\n", app/100.0);
        if (robot== "icubSim")    {
        	printf("Action finished, recreating objects!! \n");
            simMoveObj();
        } else{
            printf("Put the object back in place!!! \n");
            Time::delay(5);
        }
        //goHomeNoHandsExe();
    }
	return true;
}

bool AffManager::trainObserve(int tool, int pose){
	for ( int d = 0; d < 5; d++ ){
        printf("Starting Action %i\n",d);
        lookAtToolExe();
        observeToolExe();
        printf("Action %i Finished\n",d);
        Time::delay(2);
    }
	return true;
}

bool AffManager::observeAndDo(int toolI, int pose, int trials){
	getTool(toolI, pose);
    for ( int trial = 0; trial < trials; trial++ ){
        printf("==================================================================\n");
        printf("Performing Trial %i with tool %i on pose %i \n", trial, toolI, pose);

		lookAtToolExe();
		observeToolExe();
		trainDraw();
		finishRound();
		Time::delay(2);
    }
    return true;
}

bool AffManager::predictDo(const int toolI, const int pose){
	lookAtToolExe();
	observeToolExe();
	Bottle *matReply = matlabPort.read(true);
    
	 //cout << " Read reply form Matlab " << matReply->toString().c_str() << endl;

	 // Find the maximum for affPred and perform doAction on the max index.
	 int predClust = matReply->get(0).asInt();
	 Bottle *affPred = matReply->get(1).asList();
	 //cout << "Cluster " << predClust << ", Predicted Effect Vector " << affPred->toString().c_str() << endl;
	 int numPoint = affPred->size();
	 double bestEff = 0;
	 int bestApproachI = 0;
	 for ( int approachI = 0; approachI < numPoint; approachI++ ) // Find the approach that will generate predicted maximum effect
	 {
		double predEff = affPred->get(approachI).asDouble();
		if (predEff > bestEff){
			bestEff = 	predEff;
			bestApproachI = approachI;
		}
     }
	 int bestApproach = -5 + bestApproachI; // Transform the index to the distance between -5 ad 5 cm
	 printf("Best predicted approach at  %i, predicted effect of %f m  \n", bestApproach, bestEff);

	 Bottle bOut;bOut.clear();
	 bOut.addString("toolpose");
	 bOut.addInt(toolI);
	 bOut.addInt(pose);

	 cout << "writing out " << bOut.toString().c_str() << endl;
	 outDataPort.write(bOut);


	 bOut.clear();
	 bOut.addString("prediction");
	 bOut.addInt(predClust);
     Bottle& bPredEff = bOut.addList();
	 bPredEff.addDouble(bestEff);

	 cout << "writing out " << bOut.toString().c_str() << endl;
	 outDataPort.write(bOut);

	 return doAction(bestApproach);
 }


bool AffManager::testPredict(int tool)
{
    //for ( int tool = 3; tool <= 9; tool++ ){
		for ( int pose = -90; pose < 100; pose=pose+90 ){
			getTool(tool, pose);
			printf("==================================================================\n");
	        printf("Performing prediction test with tool %i on pose %i \n", tool, pose);
	        for ( int trial = 1; trial <= 5; trial++ ){
	        	printf("Trial %i \n", trial);
	        	predictDo(tool, pose);
	        	simMoveObj();
	        }
		}
    //}

	return true;
}



/**********************************************************
					PRIVATE METHODS
/**********************************************************/

// Setting up methods
/**********************************************************/
void AffManager::goHomeExe()
{
	Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("home");
    cmdAre.addString("all");
    fprintf(stdout,"RPC to ARE %s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre,replyAre);    
    fprintf(stdout,"Reply ARE home %s:\n",replyAre.toString().c_str()); 
    return;
}

/**********************************************************/
void AffManager::goHomeNoHandsExe()
{
	//fprintf(stdout,"Command go home no hands");
    if (robot== "icubSim")    {
    	iTorso->positionMove(0,0.0);
    	iTorso->positionMove(1,0.0);
    	iTorso->positionMove(2,0.0);

        // Move the arm to home position
    	Vector posV,orV;
    	icart->getPose(posV,orV);
        posV[0]= -0.25; posV[1] = 0.15; posV[2] = 0.10;
    	icart->goToPoseSync(posV,orV);   // first take the tool out of the way
        icart->waitMotionDone(0.1,3.0);

        Vector posH(3),orH(4);
        posH[0]= -0.25; posH[1] = 0.30; posH[2] = 0.10;
        Matrix R2(4,4);
        // pose x-axis   y-axis         z-axis            translation
        R2(0,0)= 0.0;  R2(0,1)= 0.0;  R2(0,2)=-1.0;    R2(0,3)= 0.0;        // x-coordinate in root frame
        R2(1,0)= 1.0;  R2(1,1)= 0.0;  R2(1,2)= 0.0;    R2(1,3)= 0.0;     // y-coordinate    "
        R2(2,0)= 0.0;  R2(2,1)= 1.0;  R2(2,2)= 0.0;    R2(2,3)= 0.0;     // z-coordinate    "
        R2(3,0)= 0.0;  R2(3,1)= 0.0;  R2(3,2)= 0.0;    R2(3,3)= 1.0;        // Translation
        orH = iCub::ctrl::dcm2axis(R2);     // from rotation matrix back to the axis/angle notation

        icart->goToPoseSync(posH,orH);   // then go to the home position
        icart->waitMotionDone(0.1, 3.0);

        // Move the head
        Vector ang;
        igaze->getAngles(ang);          // 
        ang[0]= 0.0;                    // azimuth-component [deg]
        ang[1]=-60.0;                   // elevation-component [deg]
        igaze->lookAtAbsAngles(ang);    // move the gaze

    } else{

	    //fprintf(stdout,"Start 'home' 'arms' 'head' proceedure:\n");
        Bottle cmdAre, replyAre;
        cmdAre.clear();
        replyAre.clear();
        cmdAre.addString("home");
        cmdAre.addString("arms");
        cmdAre.addString("head");
        fprintf(stdout,"RPC to ARE %s\n",cmdAre.toString().c_str());
        rpcMotorAre.write(cmdAre,replyAre);     
        fprintf(stdout,"Reply from ARE: %s:\n",replyAre.toString().c_str());
        
        cmdAre.clear();
        replyAre.clear();
        cmdAre.addString("idle");
        rpcMotorAre.write(cmdAre,replyAre);
        fprintf(stdout,"Reply from ARE: %s:\n",replyAre.toString().c_str());

    }
    return;
}

/**********************************************************/
void AffManager::askForTool()
{
	fprintf(stdout,"Reach me a tool please.\n");
    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("tato");
    cmdAre.addString(hand);
    cmdAre.addString("no_gaze");
    fprintf(stdout,"RPC to ARE: %s:\n", cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"Reply from ARE %s:\n", replyAre.toString().c_str());
    return;
}

/**********************************************************/
bool AffManager::graspTool()
{
    Time::delay(1);
	// Close hand on tool grasp
    Bottle cmdAre,replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("clto");
    cmdAre.addString(hand);
    cmdAre.addString("no_gaze");
    fprintf(stdout,"RPC to ARE: %s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    fprintf(stdout,"Reply from ARE: %s:\n", replyAre.toString().c_str());
	Time::delay(0.5);

	// Check if grasp was successful
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("get");
	cmdAre.addString("holding");
    rpcMotorAre.write(cmdAre, replyAre);
    return replyAre.get(0).asBool();
}

/**********************************************************/
void AffManager::simTool(int toolI,int orDeg )
{
    // Put the hand in a safe position
    Vector xd(3), od(4);                            // Set a position in the center in front of the robot
    xd[0]=-0.25; xd[1]=0.15; xd[2]=0.05;
    Vector oy(4), ox(4);

    oy[0]=0.0; oy[1]=1.0; oy[2]=0.0; oy[3]=M_PI;
    ox[0]=1.0; ox[1]=0.0; ox[2]=0; ox[3]=M_PI/2.0;
        
    Matrix Ry = iCub::ctrl::axis2dcm(oy);   // from axis/angle to rotation matrix notation
    Matrix Rx = iCub::ctrl::axis2dcm(ox);

    Matrix R = Ry*Rx;                 // compose the two rotations keeping the order
    //fprintf(stdout,"M = \n[ %s ] \n\n", R.toString().c_str());
    od = iCub::ctrl::dcm2axis(R);     // from rotation matrix back to the axis/angle notation     

    icart->goToPoseSync(xd,od);   // send request and wait for reply
    icart->waitMotionDone(0.1, 3.0);
    
    // Query simtoolloader to create the virtual tool and object
    Bottle cmdSim,replySim;       // bottles for Simulator
    cmdSim.clear();   replySim.clear();
    cmdSim.addString("del");
    fprintf(stdout,"RPC to Simulator %s\n",cmdSim.toString().c_str());
    rpcSim.write(cmdSim, replySim); // Call simtoolloader to create the tool

    cmdSim.clear();   replySim.clear();
    cmdSim.addString("tool");
    cmdSim.addInt(toolI);	    // tool
    cmdSim.addInt(2);	        // object
    cmdSim.addInt(orDeg);	    // pose

    fprintf(stdout,"RPC to simulator %s\n",cmdSim.toString().c_str());
    rpcSim.write(cmdSim, replySim); // Call simtoolloader to create the tool

    return;
}

void AffManager::simMoveObj()
{	// Query simtoolloader move the object to its original position object
    Bottle cmdSim,replySim;       // bottles for Simulator
    cmdSim.clear();   replySim.clear();
	cmdSim.addString("move");
	cmdSim.addInt(2);	// object

	fprintf(stdout,"RPC to simulator %s\n",cmdSim.toString().c_str());
    rpcSim.write(cmdSim, replySim); // Call simtoolloader to create the tool
}

bool AffManager::setLabel(const string &label){

    printf("Setting trial label \n");
    Bottle cmdFE,replyFE;
    cmdFE.clear();   replyFE.clear();
    cmdFE.addString("label");
    cmdFE.addString(label);
    fprintf(stdout,"RPC to featExt: %s\n",cmdFE.toString().c_str());
    rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.
    

    return true;
}



/**********************************************************/
void AffManager::findToolDimsExe()
{
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();   replyKM.clear();
    
    // If it is not working, bridge the "find" command by measuring the tool by hand and adding those values to toolDim.
    // Tooldim cooresponds to the difference in x, y, z between the hand end-effector and the tool end-effector, so does not really measure the tool dimensions
    // Then, to check, set the arm in idle and move it in front of the camera, to check if the tooltip dot indicator is still in place.
    cmdKM.addString("find");
    cmdKM.addString(hand);	    // arm
    cmdKM.addString("left");	// eye
    fprintf(stdout,"RCP to KarmaMotor:  %s\n", cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM); // Call karma Motor to find the tool
    fprintf(stdout,"Reply from KarmaMotor: %s:\n", replyKM.toString().c_str());

    //  if find not working, substitute here the values by hand measured ones.
    Bottle toolDimB = replyKM.tail();
    toolDim[0] = toolDimB.get(0).asDouble();
    toolDim[1] = toolDimB.get(1).asDouble();
    toolDim[2] = toolDimB.get(2).asDouble();    
    cout << "Tool dims are" << toolDim.toString().c_str() << endl;
  /*  
    printf ("measure the tool coordinates from the robot frame and input them as X Y Z");
    Bottle *toolDimB = userDataPort.read(true);
    toolDim[0] = toolDimB->get(0).asDouble();
    toolDim[1] = toolDimB->get(1).asDouble();
    toolDim[2] = toolDimB->get(2).asDouble();   
    cout << "Tool dims are" << toolDim.toString().c_str() << endl;
*/
    return;
}

void AffManager::attachTool()
{
    // Send the coordinates to Karmafinder to display it and get the tip pixel
    Bottle cmdKF,replyKF;       // bottles for Karma Motor
    cmdKF.clear();replyKF.clear();
    cmdKF.addString("show");
    cmdKF.addDouble(toolDim[0]);
    cmdKF.addDouble(toolDim[1]);
    cmdKF.addDouble(toolDim[2]);
    fprintf(stdout,"RCP to KarmaFinder%s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);
    fprintf(stdout,"Reply from KarmaFinder%s\n", replyKF.toString().c_str());

    // Attach the new tooltip to the "body schema"  
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("attach");
    cmdKM.addString(hand);
    cmdKM.addDouble(toolDim[0]);
    cmdKM.addDouble(toolDim[1]);
    cmdKM.addDouble(toolDim[2]);
    fprintf(stdout,"RPC to KarmaMotor%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"Reply from KarmaMotor %s:\n",replyKM.toString().c_str());
    fprintf(stdout,"Tool attached \n ");
    return;
}


// Look and featExt methods
/**********************************************************/
bool AffManager::lookAtToolExe()
{    
	igaze->setTrackingMode(false);

    tipOnView = false;
    double disp = 0;
    while (!tipOnView){
        cout << "Tooltip not on view, looking for it" << endl;
        handToCenter();
        //fprintf(stdout,"Moving hand to the center:\n");

        lookOverHand(disp);
        Time::delay(1);

        tipOnView = gazeAtTool();        
        disp += 0.02;
    }
    cout << "Tooltip now on view" << endl;
	igaze->setTrackingMode(true);
    return true;
}

void AffManager::handToCenter()
{
    // Move arm to have the tool within the visual field
    Vector xd(3), od(4);                            // Set a position in the center in front of the robot
    if (robot== "icubSim")    {
        xd[0]=-0.35 + Rand::scalar(0,0.05); xd[1]=0.05 + Rand::scalar(0,0.04); xd[2]=0.05;}
    else {
        xd[0]=-0.35 + Rand::scalar(0,0.05); xd[1]=0.10 + Rand::scalar(0,0.04); xd[2]=0.10;}
    
    
    Vector oy(4), ox(4);

    oy[0]=0.0; oy[1]=1.0; oy[2]=0.0; oy[3]=M_PI;
    ox[0]=1.0; ox[1]=0.0; ox[2]=0; ox[3]=M_PI/2.0;
        
    Matrix Ry = iCub::ctrl::axis2dcm(oy);   // from axis/angle to rotation matrix notation
    Matrix Rx = iCub::ctrl::axis2dcm(ox);

    Matrix R = Ry*Rx;                 // compose the two rotations keeping the order
    //fprintf(stdout,"M = \n[ %s ] \n\n", R.toString().c_str());
    od = iCub::ctrl::dcm2axis(R);     // from rotation matrix back to the axis/angle notation     

    //fprintf(stdout,"Command send to move to %.2f, %.2f, %.2f on the robot frame\n", xd[0], xd[1], xd[2] );

    icart->goToPoseSync(xd,od);   // send request and wait for reply
    icart->waitMotionDone(0.1,3.0);
    return;
}

void AffManager::lookOverHand(double disp)
{
    Vector handPos,handOr;
    icart->getPose(handPos,handOr);
    if (robot== "icubSim")
    {        
        handPos[2] += 0.15 + disp;// Tool center round 15 cm over the hand
    }else {
        handPos[2] += 0.20 + disp;
    }
    //fprintf(stdout,"Looking over hand to %.2f, %.2f, %.2f\n", handPos[0], handPos[1], handPos[2] );
    igaze->lookAtFixationPoint(handPos);
    igaze->waitMotionDone(0.1, 3.0);

    return;
}

bool AffManager::gazeAtTool()
{

    Bottle toolTipB;
    Bottle cmdKF, replyKF;    // bottles for Karma Finder
    // Get ToolTip coordinates on image
    cmdKF.clear();   replyKF.clear();
    cmdKF.addString("tip");
    fprintf(stdout,"RPC to KarmaFinder: %s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);                   // Call karmaFinder module to get the tooltip
    fprintf(stdout,"Reply from KarmaFinder: %s\n",replyKF.toString().c_str());
    toolTipB = replyKF.tail();
    toolTipPix[0] = toolTipB.get(0).asInt();
    toolTipPix[1] = toolTipB.get(1).asInt();

    if ((toolTipPix[0]<0) || (toolTipPix[0]>320) ||(toolTipPix[1]<0) ||(toolTipPix[0]>240)){
        cout <<"Tooltip out of sight, at pixel: " << toolTipB.toString().c_str() << endl;
        return false;
    }
    cout <<" Tooltip within view, at pixel: " << toolTipB.toString().c_str() << endl;	
	return true;
}

/**********************************************************/
void AffManager::observeToolExe(){
    
    // Get Hand coordinates on image 
    Vector handPose, handOr, handPix;
    icart->getPose(handPose, handOr);
    igaze->get2DPixel(0, handPose, handPix);

    
    /* read tooltip - done on gezeAtTool
     * send it to toolBlobber (this will make toolBobber output an image which will be fed to featExt)
     * call fExt snapshot to get the image
    */
    if (!gazeAtTool()){
    	cout << "Tool not on sight, cant observe it, trying to look at it again" << endl;
    	lookAtToolExe();
    	observeToolExe();
    }
    else{

		if (robot != "icubSim") {
			/* Read tooltip coordinates  
            XXX this is a hack to cope with unstability in the segmentatoin. Ideally, it should be reading from KTF via rpc, as performed in gazeAtTool() */
			printf("Getting tip coordinates \n");
			Bottle *toolTipIn = userDataPort.read(true);
			toolTipPix[0] = toolTipIn->get(0).asInt();
			toolTipPix[1] = toolTipIn->get(1).asInt();
			cout <<" Tooltip within view, at pixel: " << toolTipPix.toString().c_str() << endl;
        }


		printf("Seeding ToolBlobber \n");
		Bottle cmdTB, replyTB;                                  // bottles for Tool Blobber
		cmdTB.clear();   replyTB.clear();
		cmdTB.addString("seed");
		cmdTB.addInt(toolTipPix[0]);
		cmdTB.addInt(toolTipPix[1]);
		fprintf(stdout,"RPC to toolBlobber: %s\n", cmdTB.toString().c_str());
		rpcToolBlobber.write(cmdTB, replyTB);                   // Send tool blobber the seed point to retrieve from GBS
        fprintf(stdout,"Reply from toolBlobber: %s\n", replyTB.toString().c_str());

		// Get the features
		printf("Computing Features \n");
		Bottle cmdFE,replyFE;
		cmdFE.clear();
		replyFE.clear();
		cmdFE.addString("snapshot");
		fprintf(stdout,"RPC to featExt: %s\n", cmdFE.toString().c_str());
		rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.
        fprintf(stdout,"Reply from featExt: %s\n", replyFE.toString().c_str());

        /*
		// Send them to Afflearn to be analyzed
		//printf("Sending data to affLearn\n");
		Bottle cmdLearn,replyLearn;
		cmdLearn.clear();
		replyLearn.clear();
		cmdLearn.addString("addData");
		cmdLearn.addString("toolFeats");
		cmdLearn.append(replyFE);
		fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
		rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning
		//fprintf(stdout,"Data processed by learner \n");
        */
    }
    
    return;
}

//------------ Action and effect Routines -------------
/**********************************************************/
void AffManager::slideAction(int approach)
{
    if (objCoords3DLoc){
        Bottle cmdKM,replyKM;       // bottles for Karma Motor
        cmdKM.clear();   replyKM.clear();

        double radius = 0.05;   // Radius in CM!!!
        double goPoint = approach/100.0;// floor(Rand::scalar(-radius, radius+0.01)*100.0)/100.0; //Floor to get only integer cm, and transform it to meters.
        int angle = 90;         // approach on the robot line
        double dist = 0.2;      // drag 20 cm
        
        //double minusTool = 0.15;
        fprintf(stdout,"Performing pull on approach %d on object on coords %s\n",angle, target3DcoordsIni.toString().c_str());
        cmdKM.addString("draw");
        cmdKM.addDouble(target3DcoordsIni[0] + radius - 0.03); // approach circle means it would go radius cm behind the robot, so we substract it and add 3 cm to go 3 cm behind the object center
        cmdKM.addDouble(target3DcoordsIni[1] + goPoint);     // Vary the approach coordinates on the Y axis between + and -radius.
        cmdKM.addDouble(target3DcoordsIni[2] + 0.05 - fabs(goPoint)/2);      // Z (height) is constant on the table plane + 0.04 not to hit the table itself
        cmdKM.addInt(angle);                        // Set the approach always on the center of the object
        cmdKM.addDouble(radius);                    // At 90deg, this means the tooltip will get always the radius dist behind the object.
        cmdKM.addDouble(dist);               
        
        fprintf(stdout,"RPC to KarmaMotor: %s\n",cmdKM.toString().c_str());
        rpcKarmaMotor.write(cmdKM, replyKM); // Call karmaMotor to execute the action
        fprintf(stdout,"Reply to KarmaMotor: %s\n", replyKM.toString().c_str());

        icart->waitMotionDone(0.1,20.0);
        actionDone = true;


        // Save the features of the action
        Bottle cmdLearn,replyLearn;
        cmdLearn.clear();
        replyLearn.clear();
        cmdLearn.addString("addData");
        cmdLearn.addString("actionFeats");
        // Add the action data in the format accepted by affLearn: ("Object" ( 4 6 3 2 ) "Obj2" (3 5 6 21 ))
        Bottle bData = cmdLearn.addList();
        Bottle bSample = bData.addList();
        bSample.addString("action");
        Bottle& toolData = bSample.addList();
        toolData.addDouble(goPoint);
        
        //fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
        //rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning

        outDataPort.write(bSample);

    } else  {
        printf("Object coords hadn't been located, so action could not be done");
    }
    return;
}

/**********************************************************/
void AffManager::trackObjExe()
{
	goHomeNoHands();
    // Select the target object to be tracked
    printf("\n \n Click first TOP LEFT and then BOTTOM RIGHT from the object to set the tracking template. Please.\n");
    Bottle cmdFinder,replyFinder;
    cmdFinder.clear();
    replyFinder.clear();
    cmdFinder.addString("getBox");
    fprintf(stdout,"RPC to objFinder %s:\n", cmdFinder.toString().c_str());
	rpcObjFinder.write(cmdFinder, replyFinder);
    fprintf(stdout,"Reply from objFinder %s:\n",replyFinder.toString().c_str());

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
    return;
}

/**********************************************************/
bool AffManager::locateObj()
{

    // Get the 2D coordinates of the object from objectFinder
    Vector coords3D(2);
    
    Bottle cmdFinder,replyFinder;
    //fprintf(stdout,"Get 3D coords of tracked object:\n");
    cmdFinder.clear();
    replyFinder.clear();
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
    } else {
		cout << "No 3D point received" << endl;
        return false;
	}


	if (!actionDone){			
        //printf("The point selected is %.2f %.2f %.2f\n", coords3D[0], coords3D[1], coords3D[2]);
		target3DcoordsIni[0] = coords3D[0];
		target3DcoordsIni[1] = coords3D[1];
		target3DcoordsIni[2] = coords3D[2];        // Set the point 4 cm over the table plane
		//fprintf(stdout,"Object is located at %s:\n", target3DcoordsIni.toString().c_str());
		objCoords3DLoc = true;
	}else{
		//printf("After action, the object is in %.2f %.2f %.2f\n", coords3D[0], coords3D[1], coords3D[2]);
		target3DcoordsAfter[0] = coords3D[0];
		target3DcoordsAfter[1] = coords3D[1];
		target3DcoordsAfter[2] = coords3D[2];
		//fprintf(stdout,"Object is located at %s:\n", target3DcoordsAfter.toString().c_str());
	}

    return objCoords3DLoc;
}

/**********************************************************/
void AffManager::computeEffect()
{
    //To compute the effect, we assume that the object hasnt move in the z axis ( that is, has remained on the table)
    Vector delta = target3DcoordsAfter - target3DcoordsIni;
    double dx = delta[0];
    double dy = delta[1];
    // we suppoose the object stays on a constant plane ( Z coords)

    effectAlpha = atan2 (dy,dx) * 180 / M_PI;
    effectDist = sqrt(dx*dx+dy*dy);  //sum of the squares of the differences

    printf("Object Displaced: %g m \n", effectDist);

    // save the effect features
    Bottle cmdLearn,replyLearn;
    cmdLearn.clear();
    replyLearn.clear();
    cmdLearn.addString("addData");
    cmdLearn.addString("effectFeats");
    // Add the effect data in the format accepted by affLearn: ("Obj1" ( 4 6 3 2 ) "Obj2" (3 5 6 21 ))
    Bottle bData = cmdLearn.addList();
    Bottle bSample = bData.addList();
    bSample.addString("effect");
    Bottle& effData = bSample.addList();
    effData.addDouble(effectAlpha);
    effData.addDouble(effectDist);
    //fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
    //rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning

     outDataPort.write(bSample);
    
    return;
}

/**********************************************************/
void AffManager::finishRound()
{
    // reinitialize some flags for next round
    actionDone = false;
    objCoords3DLoc = false;
    tipOnView = false;
    
    target3DcoordsIni.clear();		// Keeps the target position in 3D before the action
    target3DcoordsAfter.clear();	// Keeps the target position in 3D after the action
    
    target3DcoordsIni.resize(3);    // Keeps the target position in 3D before the action
    target3DcoordsAfter.resize(3);	// Keeps the target position in 3D after the action

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
    rf.setDefault("name","affManager");
	rf.setDefault("camera","left");
	rf.setDefault("robot","icub");
    rf.setDefault("hand","right");
    //rf.setDefaultContext("affManager");
    //rf.setDefaultConfigFile("affManager.ini");
    //rf.setDefault("tracking_period","30");
    rf.configure(argc,argv);

    AffManager affManager;
    return affManager.runModule(rf);
}
