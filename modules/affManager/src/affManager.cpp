/*
 * AFFORNDACE MOTOR
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

    //simMode=rf.check("simMode", Value("off"),"simMode (string)").asString();
	
	attach(rpcCmd);
	running = true;
    actionDone = false;
    objCoords3DLoc = false;
    trackingObj = false;
    tipOnView = false;

    toolDim.resize(3, 0.0);
    toolTipPix.resize(2,0.0);
    target3DcoordsIni.resize(3, 0.0);		// Keeps the target position in 3D
    target3DcoordsAfter.resize(3, 0.0);	    // Keeps the target position in 3D
    toolPoseName = "undef";

	//ports
	bool ret = true;  
    ret = userDataPort.open(("/"+name+"/user:i").c_str());	                       // input port to receive data from user
    ret = ret && outDataPort.open(("/"+name+"/data:o").c_str());                           // port to send data out for recording
    ret = ret && matlabPort.open(("/"+name+"/matlab:i").c_str());                           // port to send data out for recording
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
bool AffManager::askForTool(){
	askForToolExe();
	return true;
}
bool AffManager::graspTool(){	
	return graspToolExe();
}

bool AffManager::findToolDims(){
	findToolDimsExe();
	return true;
}

bool AffManager::attachTool(){
	attachToolExe();
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

bool AffManager::locateObj(){
	locateObjExe();
	return true;
}

bool AffManager::slideAction(const int approach){
	slideActionExe(approach);
	return true;
}

bool AffManager::computeEffect(){
    computeEffectExe();
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
        askForToolExe();
        std::cout << "Give the tool to the iCub oriented " <<  deg << "!" << std::endl;
        if (!graspToolExe()){
            std::cout << "Tool could not be grasped properly, stopping!" << std::endl;
            return false;
        }
        // Find tool dimensions using KARMA
        //findToolDimsExe();    // XXX Possibly substitute this by fixed dimension values
        toolDim[0] = 0.16;
        toolDim[1] = -0.16;
        toolDim[2] = -0.04;
    }
    
    // Show and attach tooltip.
    attachToolExe();

    // Go home position with tool
    // goHomeNoHandsExe();
	return true;
}

bool AffManager::doAction(const int approach){
	goHomeNoHandsExe();
    if (!trackingObj)
        trackObjExe();
    Time::delay(2);
	if (locateObjExe())
	{
		slideActionExe(approach);
		goHomeNoHandsExe();
		Time::delay(2);
		locateObjExe();
		computeEffectExe();
	}
    finishRound();
	return true;
}

bool AffManager::trainDraw(int toolI, int pose){
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
		trainDraw(toolI,pose);
		finishRound();
		Time::delay(2);
    }
    return true;
}

bool AffManager::runExp(){
    printf("Starting Routine\n");
    //for ( int tool = 5; tool <= 7; tool++ ){
		int tool=3;
		for ( int pose = -90; pose < 100; pose=pose+90 ){
			observeAndDo(tool, pose, 10);
			Time::delay(2);
		}
    //}
	return true;
}

bool AffManager::predictDo(const int toolI, const int pose){
	getTool(toolI, pose);
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


bool AffManager::testPredict(int trials)
{
	for ( int trial = 0; trial < trials; trial++ ){
			int toolI = floor(Rand::scalar(3, 9+0.9999));
			int poseI = floor(Rand::scalar(0,2+0.9999));
			int pose = -90 + 90*poseI; // transform the index from 0 1 2 to -90 0 90;
	        printf("==================================================================\n");
	        printf("Performing Prediction test Trial %i with tool %i on pose %i \n", trial, toolI, pose);
	        predictDo(toolI, pose);
	    }
	return true;
}



/**********************************************************
					PRIVATE METHODS
/**********************************************************/

// Setting up methods
/**********************************************************/
void AffManager::goHomeExe()
{
	fprintf(stdout,"Start 'home' 'all' procedure:\n");
    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("home");
    cmdAre.addString("all");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"gone home %s:\n",replyAre.toString().c_str()); 
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
        rpcMotorAre.write(cmdAre,replyAre);     
        fprintf(stdout,"gone home with ARE: %s:\n",replyAre.toString().c_str());

        cmdAre.clear();
        replyAre.clear();
        cmdAre.addString("idle");
        rpcMotorAre.write(cmdAre,replyAre);     

    }
    return;
}


/**********************************************************/
void AffManager::askForToolExe()
{
	fprintf(stdout,"Reach me a tool please.\n");
	fprintf(stdout,"Start 'take_tool' procedure:\n");
    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("tato");
    cmdAre.addString("right");
    cmdAre.addString("no_gaze");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"Waiting for tool %s:\n",replyAre.toString().c_str());
    return;
}

/**********************************************************/
bool AffManager::graspToolExe()
{
    Time::delay(1);
	// Close hand toolwise
    fprintf(stdout,"Start 'clto' 'right' (close tool right) procedure:\n");
    Bottle cmdAre,replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("clto");
    cmdAre.addString("right");
    cmdAre.addString("no_gaze");
    fprintf(stdout,"%s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    fprintf(stdout,"'clto' 'right' action is %s:\n",replyAre.toString().c_str());
	Time::delay(0.5);

	// Check if grasp was successful
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("get");
	cmdAre.addString("holding");
    fprintf(stdout,"%s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    return replyAre.get(0).asBool();
}

/**********************************************************/
void AffManager::simTool(int toolI,int orDeg )
{
    // Put the hand in a s0afe position
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
    fprintf(stdout,"RPC to simulator %s\n",cmdSim.toString().c_str());
    rpcSim.write(cmdSim, replySim); // Call simtoolloader to create the tool

    cmdSim.clear();   replySim.clear();
    cmdSim.addString("tool");
    cmdSim.addInt(toolI);	// tool
    cmdSim.addInt(2);	// object
    cmdSim.addInt(orDeg);	// pose

    fprintf(stdout,"RPC to simulator %s\n",cmdSim.toString().c_str());
    rpcSim.write(cmdSim, replySim); // Call simtoolloader to create the tool

    //Vector handPos,handOr;
    //icart->getPose(handPos,handOr);
    //igaze->lookAtFixationPoint(handPos);    
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
    cmdFE.clear();
    replyFE.clear();
    cmdFE.addString("label");
    cmdFE.addString(label);
    fprintf(stdout,"%s\n",cmdFE.toString().c_str());
    rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.
    // At this point, other moduel running on parallel should be providing the featExt module a clear blob segmentation of the tool

    return true;
}



/**********************************************************/
void AffManager::findToolDimsExe()
{
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();   replyKM.clear();
    
    // If it is not working, bridge the "find" command by measuring the tool by hand and adding those values to toolDim.
    // Tooldim cooresponds to the difference in x, y, z between the hand end-effector and the tool end-effector, so does not really measure the tool dimensions
    // Then, to check, set the arm in idle and move it in front of the camera, to check if the dot still is in place.
    cmdKM.addString("find");
    cmdKM.addString("right");	// arm
    cmdKM.addString("left");	// eye
    fprintf(stdout,"%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM); // Call karma Motor to find the tool
    fprintf(stdout,"Tool frame found\n");

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

void AffManager::attachToolExe()
{
    // Send the coordinates to Karmafinder to display it and get the tip pixel
    Bottle cmdKF,replyKF;       // bottles for Karma Motor
    cmdKF.clear();replyKF.clear();
    cmdKF.addString("show");
    cmdKF.addDouble(toolDim[0]);
    cmdKF.addDouble(toolDim[1]);
    cmdKF.addDouble(toolDim[2]);
    fprintf(stdout,"%s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);

    // Attach the new tooltip to the "body schema"  
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("attach");
    cmdKM.addString("right");
    cmdKM.addDouble(toolDim[0]);
    cmdKM.addDouble(toolDim[1]);
    cmdKM.addDouble(toolDim[2]);
    fprintf(stdout,"%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);
    fprintf(stdout,"reply is %s:\n",replyKM.toString().c_str());
    fprintf(stdout,"Tool attached \n ");
    return;
}


// Look and featExt methods
/**********************************************************/
bool AffManager::lookAtToolExe()
{
    // Put tool on a comfortable lookable position
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
    //Vector fp;
    //igaze->getFixationPoint(fp); 							// retrieve the current fixation point
	//fprintf(stdout,"Tool looked at %.2f, %.2f, %.2f \n", fp[0], fp[1], fp[2] );

    return;
}

bool AffManager::gazeAtTool()
{

    Bottle toolTipB;
    Bottle cmdKF, replyKF;    // bottles for Karma Finder
    // Get ToolTip coordinates on image
    cmdKF.clear();   replyKF.clear();
    cmdKF.addString("tip");
    fprintf(stdout,"%s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);                   // Call karmaFinder module to get the tooltip
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

    /* So, 
     * read tooltip from KF - done on gezeAtTool
     * send it to toolBlobber (this will make toolBobber output an image which will be fed to featExt)
     * call fExt snapshot to get the image
    */
    if (!gazeAtTool()){
    	cout << "Tool not on sight, cant observe it, trying to look at it" << endl;
    	lookAtToolExe();
    	observeToolExe();
    }
    else{

		if (robot != "icubSim") {
			 /* Read tooltip coordinates  XXX this is a hack, it should be reading from KTF via rpc, as below*/
			printf("Getting tip coordinates \n");
			Bottle *toolTipIn = userDataPort.read(true);
			toolTipPix[0] = toolTipIn->get(0).asInt();
			toolTipPix[1] = toolTipIn->get(1).asInt();
			cout <<" Tooltip within view, at pixel: " << toolTipPix.toString().c_str() << endl;
			// XXX The waiting read has to be done because featExt doesnt work asyncronously, or rather, only does that with RPC query. gonan change that.
		}


		printf("Seeding ToolBlobber \n");
		Bottle cmdTB, replyTB;                                  // bottles for Tool Blobber
		cmdTB.clear();   replyTB.clear();
		cmdTB.addString("seed");
		cmdTB.addInt(toolTipPix[0]);
		cmdTB.addInt(toolTipPix[1]);
		fprintf(stdout,"%s\n",cmdTB.toString().c_str());
		rpcToolBlobber.write(cmdTB, replyTB);                   // Send tool blobber the seed point to retrieve from GBS


		// Get the features
		printf("Computing Features \n");
		Bottle cmdFE,replyFE;
		cmdFE.clear();
		replyFE.clear();
		cmdFE.addString("snapshot");
		fprintf(stdout,"%s\n",cmdFE.toString().c_str());
		rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.
		// At this point, other moduel running on parallel should be providing the featExt module a clear blob segmentation of the tool

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
    }
    
    return;
}

//------------ Action and effect Routines -------------
/**********************************************************/
void AffManager::slideActionExe(int approach)
{
    if (objCoords3DLoc){
        Bottle cmdKM,replyKM;       // bottles for Karma Motor
        cmdKM.clear();   replyKM.clear();

        double radius = 0.05;   // Radius in CM!!!
        double goPoint = approach/100.0;// floor(Rand::scalar(-radius, radius+0.01)*100.0)/100.0; //Floor to get only integer cm, and transform it to meters.
        int angle = 90;         // approach on the robot line
        double dist = 0.2;      // drag 20 cm
        
        //double minusTool = 0.15;
        cmdKM.addString("draw");
        cmdKM.addDouble(target3DcoordsIni[0] + radius - 0.03); // approach circle means it would go radius cm behind the robot, so we substract it and add 3 cm to go 3 cm behind the object center
        cmdKM.addDouble(target3DcoordsIni[1] + goPoint);     // Vary the approach coordinates on the Y axis between + and -radius.
        cmdKM.addDouble(target3DcoordsIni[2] + 0.05 - fabs(goPoint)/2);      // Z (height) is constant on the table plane + 0.04 not to hit the table itself
        cmdKM.addInt(angle);                        // Set the approach always on the center of the object
        cmdKM.addDouble(radius);                    // At 90deg, this means the tooltip will get always the radius dist behind the object.
        cmdKM.addDouble(dist);
        fprintf(stdout,"%s\n",cmdKM.toString().c_str());
        
        fprintf(stdout,"Performing pull on approach %d on object on coords %s\n",angle, target3DcoordsIni.toString().c_str());
        rpcKarmaMotor.write(cmdKM, replyKM); // Call karmaMotor to execute the action

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
        rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning

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
	// XXX So far track at the object. Improve to detect it automatically.
	rpcObjFinder.write(cmdFinder, replyFinder);
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
bool AffManager::locateObjExe()
{

    // Get the 2D coordinates of the object from objectFinder
    Vector coords3D(2);
    
    Bottle cmdFinder,replyFinder;
    //fprintf(stdout,"Get 3D coords of tracked object:\n");
    cmdFinder.clear();
    replyFinder.clear();
    cmdFinder.addString("getPointTrack");
    cmdFinder.addDouble(tableHeight);
	rpcObjFinder.write(cmdFinder, replyFinder);
    //printf("Received from rpc: %s \n", replyFinder.toString().c_str());

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
void AffManager::computeEffectExe()
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
    rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning

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

/*  This should be done only once per tool, for redoing an action with the same tool, it can be kept
    // Remove the tool from the "body schema"
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();   replyKM.clear();
    cmdKM.addString("tool ");
    cmdKM.addString("remove");	    
    fprintf(stdout,"%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM); // Call and featExt module to get tool features.
    fprintf(stdout,"Tool removed \n");

    toolDim.resize(3, 0.0);    
    
    Bottle cmdAre,replyAre;
    cmdAre.clear();    replyAre.clear();
    cmdAre.addString("drop");
    cmdAre.addString("away");
    fprintf(stdout,"%s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    fprintf(stdout,"Tool dropped \n");

    // Send command to templatePFTracker to reset tracker
    Port cmdPort;
    cmdPort.open("/templatePFTracker");
    Bottle cmd;
    cmd.addString("reset");
    cmdPort.write(cmd);      // Send the command
    cmdPort.close();
    */

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
    //rf.setDefaultContext("affManager");
    //rf.setDefaultConfigFile("affManager.ini");
    //rf.setDefault("tracking_period","30");
    rf.configure(argc,argv);

    AffManager affManager;
    return affManager.runModule(rf);
}
