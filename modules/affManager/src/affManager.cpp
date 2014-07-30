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
	running=true;
	lookingAtTool = false;
    actionDone = false;
    objFound = false;

    toolDim.resize(3, 0.0);
    target3DcoordsIni.resize(3, 0.0);		// Keeps the target position in 3D
    target3DcoordsAfter.resize(3, 0.0);	    // Keeps the target position in 3D
    toolPoseName = "undef";

	//rpc
	bool retRPC = true;  
    retRPC = userDataPort.open(("/"+name+"/user:i").c_str());	
    retRPC = outDataPort.open(("/"+name+"/data:o").c_str());                            // port to send data out for recording
    retRPC = retRPC && rpcCmd.open(("/"+name+"/rpc:i").c_str());					   //rpc client to interact with the affManager
    retRPC = retRPC && rpcMotorAre.open(("/"+name+"/are:rpc").c_str());                //rpc server to query ARE
    retRPC = retRPC && rpcKarmaMotor.open(("/"+name+"/karmaMotor:rpc").c_str());       //rpc server to query Karma Motor    
    retRPC = retRPC && rpcKarmaFinder.open(("/"+name+"/karmaFinder:rpc").c_str());     //rpc server to query Karma Finder    
	// retRPC = retRPC && rpcBlobPicker.open(("/"+name+"/blobPick:rpc").c_str());      //rpc server to query blobPicker
	retRPC = retRPC && rpcFeatExt.open(("/"+name+"/featExt:rpc").c_str());			   //rpc server to query featExtractin Module
    retRPC = retRPC && rpcAffLearn.open(("/"+name+"/affLearn:rpc").c_str());		   //rpc server to query affLearn
    retRPC = retRPC && rpcObjFinder.open(("/"+name+"/objFind:rpc").c_str());           //rpc server to query objectFinder
    retRPC = retRPC && rpcToolBlobber.open(("/"+name+"/toolBlob:rpc").c_str());           //rpc server to query objectFinder
	if (!retRPC){
		printf("Problems opening rpc ports\n");
		return false;
	}

	// Cartesian Controller Interface
	Property optCart;
	optCart.put("device","cartesiancontrollerclient");
    optCart.put("remote","/"+robot+"/cartesianController/"+hand+"_arm");
    optCart.put("local","/"+name+"/cartesian_client/"+hand+"_arm");
	if (!clientCart.open(optCart))
		return false;    
    clientCart.view(icart);	// open the view	

    icart->storeContext(&cartCntxt);

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
    igaze->setTrackingMode(true);


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

    rpcCmd.interrupt();
    rpcMotorAre.interrupt();
	//rpcMotorIol.interrupt();
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

    rpcCmd.close();
    rpcMotorAre.close();
	//rpcMotorIol.close();
    rpcKarmaMotor.close();
    rpcKarmaFinder.close();

    rpcFeatExt.close();
    rpcAffLearn.close();

    rpcObjFinder.close();
    rpcToolBlobber.close();

    clientGaze.close();
    clientCart.close();
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

bool AffManager::getTool(){
	//askForToolExe();
    //if (!graspToolExe()){
    //   std::cout << "Tool couldnt be grasped properly, stopping!" << std::endl;
    //    return false;
    //}
    lookAtToolExe();

    attachToolExe();
    observeToolExe();
    goHomeNoHandsExe();
	return true;
}

bool AffManager::setTool(const string &tpName){
    toolPoseName = tpName;
	return true;
}

bool AffManager::askForTool(){
	askForToolExe();
	return true;
}
bool AffManager::graspTool(){
	graspToolExe();
	return toolInHand;
}
bool AffManager::lookAtTool(){
	lookAtToolExe();
	return true;
}
bool AffManager::observeTool(){
	observeToolExe();
	return true;
}

bool AffManager::trackObj(){
	trackObjExe();
	return true;
}

bool AffManager::locateObj(){
	locateObjExe();
	return true;
}

bool AffManager::doAction(){
	goHomeNoHandsExe();
    //trackObjExe();
	bool objLocated = locateObjExe();
	if (objLocated)
	{
		slideActionExe();
		goHomeNoHandsExe();
		locateObjExe();
		computeEffectExe();
	}
    finishRound();
	return true;
}

bool AffManager::observeAndDo(){
    goHomeNoHandsExe();
    lookAtToolExe();
    observeToolExe();
    //trackObjExe();
    bool objLocated = locateObjExe();
    if (objLocated)
    {
        slideActionExe();
        goHomeNoHandsExe();
        locateObjExe();
        computeEffectExe();
    }
    finishRound();
    return true;
}

bool AffManager::trainDraw(){
    goHomeNoHandsExe();
    trackObjExe();
	for ( int d = 0; d < 20; d++ ){
        printf("RestartingAction\n");
        doAction();
        printf("Action %i Finished\n",d);
        Time::delay(5);
    }
	return true;
}
bool AffManager::attachTool(){
	attachToolExe();
	return true;
}
bool AffManager::slideAction(){
	slideActionExe();
	return true;
}

bool AffManager::computeEffect(){
    computeEffectExe();
    return true;
}

/**********************************************************
					PRIVATE METHODS
/**********************************************************/

// Motor Commands
/**********************************************************/
void AffManager::goHomeExe()
{
	fprintf(stdout,"Start 'home' 'all' proceedure:\n");
    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("home");
    cmdAre.addString("all");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"gone home %s:\n",replyAre.toString().c_str());   
    lookingAtTool = false;
    lookingAtObject = false;
}

/**********************************************************/
void AffManager::goHomeNoHandsExe()
{
	fprintf(stdout,"Start 'home' 'arms' 'head' proceedure:\n");
    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("home");
    cmdAre.addString("arms");
    cmdAre.addString("head");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"gone home %s:\n",replyAre.toString().c_str());
    lookingAtTool = false;
    lookingAtObject = false;
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
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"Waiting for tool %s:\n",replyAre.toString().c_str());
    Time::delay(3);
    return;
}

/**********************************************************/
bool AffManager::graspToolExe()
{
    Time::delay(0.5);
	// Close hand toolwise
    fprintf(stdout,"Start 'clto' 'right' (close tool right) procedure:\n");
    Bottle cmdAre,replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("clto");
    cmdAre.addString("right");
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
	toolInHand = replyAre.get(0).asBool();
    return toolInHand;
}

/**********************************************************/
void AffManager::lookAtToolExe()
{
    // Put tool on a comfortable lookable position
    handToCenter();
    fprintf(stdout,"Moving hand to the center:\n");
    lookOverHand();
    return;
}

void AffManager::handToCenter()
{
    Vector xd(3), od(4);                            // Set a position in the center in front of the robot
    xd[0]=-0.35 + Rand::scalar(0,0.05); xd[1]=0.05 + Rand::scalar(0,0.04); xd[2]=0.05;
    
    Vector oy(4), ox(4);

    oy[0]=0.0; oy[1]=1.0; oy[2]=0.0; oy[3]=M_PI;
    ox[0]=1.0; ox[1]=0.0; ox[2]=0; ox[3]=M_PI/2.0;
        
    Matrix Ry = iCub::ctrl::axis2dcm(oy);   // from axis/angle to rotation matrix notation
    Matrix Rx = iCub::ctrl::axis2dcm(ox);

    Matrix R = Ry*Rx;                 // compose the two rotations keeping the order
    //fprintf(stdout,"M = \n[ %s ] \n\n", R.toString().c_str());
    od = iCub::ctrl::dcm2axis(R);     // from rotation matrix back to the axis/angle notation     

    fprintf(stdout,"Command send to move to %.2f, %.2f, %.2f on the robot frame\n", xd[0], xd[1], xd[2] );

    icart->goToPoseSync(xd,od);   // send request and wait for reply
    fprintf(stdout,"Hand moving!!");
    icart->waitMotionDone(0.04);
    fprintf(stdout,"Movement completed!!");
    return;
}

void AffManager::lookOverHand()
{
    Vector handPos,handOr;
    icart->getPose(handPos,handOr);
    if (robot== "icubSim")
    {
        handPos[2] += 0.15;// Tool center round 15 cm over the hand
    }else {
        handPos[2] += 0.25;
    }
              

    fprintf(stdout,"Looking to %.2f, %.2f, %.2f\n", handPos[0], handPos[1], handPos[2] );

    igaze->lookAtFixationPoint(handPos);     
    igaze->waitMotionDone(0.04);                                // wait until the operation is done
    
    Vector fp;
    igaze->getFixationPoint(fp); 							// retrieve the current fixation point
    lookingAtTool = true;        
	fprintf(stdout,"Tool looked at %.2f, %.2f, %.2f \n", fp[0], fp[1], fp[2] );
	
    return;
}

void AffManager::simTool()
{
    fprintf(stdout,"Creating virtual tool at -0.3 0 0.15 \n");
    yarp::os::RpcClient simPort;
    const char* portName="/affManager/rpcSim:o";
    simPort.open(portName);  // give the port a name
	Network::connect(portName, "/icubSim/world");
    Bottle simCmd, reply;
    simCmd.addString("world mk cyl 0.01 0.2 0 0.75 0.35 0 0 1");
    simPort.write(simCmd, reply);
    simCmd.clear();reply.clear();
    simCmd.addString("world rot cyl 1 90 0 0");
    simCmd.clear();reply.clear();
    simCmd.addString("world grab cyl 1 right 1");
    simPort.write(simCmd, reply);
    Network::disconnect(portName, "/icubSim/world");
    return;
}

/**********************************************************/
void AffManager::slideActionExe()
{

    if (objFound){
        Bottle cmdKM,replyKM;       // bottles for Karma Motor
        cmdKM.clear();   replyKM.clear();

        double radius = 0.05;   // Radius in CM!!!
        double goPoint = floor(Rand::scalar(-radius, radius+0.01)*100.0)/100.0; //Floor to get only integer cm, and transform it to meters.
        int angle = 90;         // approach on the robot line
        double dist = 0.2;      // drag 20 cm
        
        //double minusTool = 0.15;
        cmdKM.addString("draw");
        cmdKM.addDouble(target3DcoordsIni[0] + 0.04); // approach circle means it would go 7 cm behind the robot, so add 4 to go 3 cm behind th robot
        cmdKM.addDouble(target3DcoordsIni[1] + goPoint);     // Vary the approach coordinates on the Y axis between + and -radius.
        cmdKM.addDouble(target3DcoordsIni[2]);
        cmdKM.addInt(angle);                        // Set the approach always on che center of the object
        cmdKM.addDouble(radius);                    // At 90deg, this means the tooltip will get always the radius dist behind the object.
        cmdKM.addDouble(dist);
        fprintf(stdout,"%s\n",cmdKM.toString().c_str());
        
        fprintf(stdout,"Performing draw with angle %d on object on coords %s\n",angle, target3DcoordsIni.toString().c_str());
        rpcKarmaMotor.write(cmdKM, replyKM); // Call karmaMotor to execute the action

        icart->waitMotionDone(0.04);
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
        
        fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
        rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning

        outDataPort.write(bSample);

    } else  {
        printf("location hadnt been initialized, so action could not be done");
    }


    return;
}


// Perceptual Methods
/**********************************************************/
void AffManager::attachToolExe()
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

/**********************************************************/
void AffManager::observeToolExe(){
    
    // Get Hand coordinates on image 
    Vector handPose, handOr, handPix;
    icart->getPose(handPose, handOr);
    igaze->get2DPixel(0, handPose, handPix);

    /* Read tooltip coordinates  XXX this is a hack, it should be reading from KTF via rpc, as below*/
    yarp::sig::Vector toolTipPix(2);
    Bottle *toolTipIn = userDataPort.read(true);
    toolTipPix[0] = toolTipIn->get(0).asInt();
    toolTipPix[1] = toolTipIn->get(1).asInt();
        
    /*    
    // Get ToolTip coordinates on image
    Bottle toolTipB;
    Bottle cmdKF, replyKF;                                  // bottles for Karma Motor
    cmdKF.clear();   replyKF.clear();
    cmdKF.addString("tip");
    fprintf(stdout,"%s\n",cmdKF.toString().c_str());
    rpcKarmaFinder.write(cmdKF, replyKF);                   // Call karmaFinder module to get the tooltip
    toolTipB = replyKF.tail();			                    
    cout <<"Tooltip at pixel: " << toolTipB.toString().c_str() << endl;
    
    Vector toolTipPix(2);
    toolTipPix[0] = toolTipB.get(0).asInt();
    toolTipPix[1] = toolTipB.get(1).asInt();
    
    */
    /* So, 
     * read tooltip from KF
     * send it to toolBlobber (this will make tB output an image)
     * call fExt snapshot to get the image
    */
    Bottle cmdTB, replyTB;                                  // bottles for Tool Blobber
    cmdTB.clear();   replyTB.clear();
    cmdTB.addString("seed");
    cmdTB.addInt(toolTipPix[0]);
    cmdTB.addInt(toolTipPix[1]);
    fprintf(stdout,"%s\n",cmdTB.toString().c_str());
    rpcToolBlobber.write(cmdTB, replyTB);                   // Send tool blobber the seed point to retrieve from GBS
    
    // Get the features
    Bottle cmdFE,replyFE;
    cmdFE.clear();
    replyFE.clear();
    cmdFE.addString("snapshot");
    cmdFE.addString(toolPoseName);
    fprintf(stdout,"%s\n",cmdFE.toString().c_str());
    rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.
    // At this point, other moduel running on parallel should be providing the featExt module a clear blob segmentation of the tool

    // Send them to Afflearn to be analyzed
    Bottle cmdLearn,replyLearn;
    cmdLearn.clear();
    replyLearn.clear();
    cmdLearn.addString("addData");
    cmdLearn.addString("toolFeats");
    cmdLearn.append(replyFE);
    fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
    rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning
    fprintf(stdout,"Data processed by learner \n");
    
    
    return;
}

void AffManager::trackObjExe()
{
    // Select the target object to be tracked
    printf("\n \n Click first TOP LEFT and then BOTTOM RIGHT from the object to set the tracking template. Please.\n");
    Bottle cmdFinder,replyFinder;
    cmdFinder.clear();
    replyFinder.clear();
    cmdFinder.addString("getBox");
	// XXX So far track at the object. Improve to detect it automatically.
	rpcObjFinder.write(cmdFinder, replyFinder);
    printf("Object template has been set properly\n");
    return;

    // Set ARE to constantly track the object
    Bottle cmdAre, replyAre;
    cmdAre.addString("track");
    cmdAre.addString("track");
    cmdAre.addString("no_sacc");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"tracking started%s:\n",replyAre.toString().c_str());

}

/**********************************************************/
bool AffManager::locateObjExe()
{

    // Get the 2D coordinates of the object from objectFinder
    Vector coords2D(2);
    
    Bottle cmdFinder,replyFinder;
    fprintf(stdout,"Get 2D coords of tracked object:\n");
    cmdFinder.clear();
    replyFinder.clear();
    cmdFinder.addString("getPointTrack");
	rpcObjFinder.write(cmdFinder, replyFinder);
    printf("Received from rpc: %s \n", replyFinder.toString().c_str());

    if (replyFinder.size() >1){
        coords2D(0) = replyFinder.get(1).asList()->get(0).asInt();
        coords2D(1) = replyFinder.get(1).asList()->get(1).asInt();
        printf("Point in 2D read: %g, %g\n", coords2D(0), coords2D(1));
    } else {
		cout << "No 2D point received" << endl;
        return false;
	}
       
    // Transform the 2D coordinates into 3D
    Vector coords3D(3);
    Vector table(4);  // specify the plane in the root reference frame as ax+by+cz+d=0; z=-tableHeight in this case
    table[0] = 0.0; table[1] = 0.0; table[2] = 1.0;  
    table[3] = -tableHeight;    // d -> so th eequation of the table plane is z=-h 

    int camSel;
    if (camera != "left") {
           camSel = 1;}
    else { camSel = 0;}    

    if(igaze->get3DPointOnPlane(camSel,coords2D, table, coords3D)){  
        //igaze->lookAtFixationPoint(coords3D);                 // move the gaze to the desired fixation point
        //igaze->waitMotionDone();                              // wait until the operation is done
        //printf(" Looking done\n");
    
        //lookingAtObject = true;

	    if (!actionDone){			
            printf("The point selected is %.2f %.2f %.2f\n", coords3D[0], coords3D[1], coords3D[2]);
		    target3DcoordsIni[0] = coords3D[0];
		    target3DcoordsIni[1] = coords3D[1];
		    target3DcoordsIni[2] = coords3D[2] + 0.04;        // Set the point 3 cm over the table plane
		    //fprintf(stdout,"Object is located at %s:\n", target3DcoordsIni.toString().c_str());
		    objFound = true;
	    }else{
		    printf("After action, the object is in %.2f %.2f %.2f\n", coords3D[0], coords3D[1], coords3D[2]);
		    target3DcoordsAfter[0] = coords3D[0];
		    target3DcoordsAfter[1] = coords3D[1];
		    target3DcoordsAfter[2] = coords3D[2] + 0.03;
		    //fprintf(stdout,"Object is located at %s:\n", target3DcoordsAfter.toString().c_str());
		    }
	} else  {
        printf("3D point couldnt be computed\n");
        objFound = false;
    }
    return objFound;
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
    fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
    rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning

     outDataPort.write(bSample);
    
    return;
}

void AffManager::finishRound()
{
    // reinitialize some flags for next round
    actionDone = false;
    //objFound = false;
    lookingAtTool = false;
    
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


    // Send command to featExt to reset ROI.
    Bottle cmdFE, replyFE;
    cmdFE.clear();
    replyFE.clear();
    cmdFE.addString("reset");
    fprintf(stdout,"%s\n",cmdFE.toString().c_str());
    rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.
    fprintf(stdout,"featExt reseted \n");
    */

}


/*
vector<int> AffManager::findToolsExe()
{
	// Call blob3Dinfo to get the indices of the blobs within a given range.
	// Filter the blob image keeeping only those blobs, and pass it to ToolSelector/blobpicker.
	// Which uses featExt to select the correct blob based on its features.
	
	Bottle blobList;
	vector<int> blobsInReachIndices;
	blobsInReachList.clear();

    blobList.clear();
    bool invalid = false;
    // grab the blobs
    blobList=getBlobs();
	if (blobList.size()==0)    {        
        fprintf (stdout,"No blobs received\n");   
        invalid = true;
    }
	
	// Thrift approach
	//AffManager affManager;
	//affManager.yarp().attachAsClient(rpcBlobInfo);
	//affManager.blobsInRange(0.5);
	

	Bottle cmdBlob, replyBlob;
    cmdBlob.clear();
    replyBlob.clear();
    cmdBlob.addString("blobsInRange");
	cmdBlob.addDouble(0.5);
	rpcBlobInfo.write(cmdBlob,replyBlob);

	if (replyBlob.get(0).isList()){
		fprintf (stdout,"Thrift returns vectors as values within a list\n");   
		for (int j=0; j<replyBlob.get(0).asList()->size(); j++){
			blobsInReachIndices[j] = replyBlob.get(0).asList()->get(j).asInt();
			blobsInReachList.append(*blobList.get(blobsInReachIndices[j]).asList());
		}
	}

	if (replyBlob.get(0).isInt()){
		fprintf (stdout,"Thrift returns vectors as values in the main bottle \n");   
		for (int j=0; j<replyBlob.size();j++){
			blobsInReachIndices[j] = replyBlob.get(j).asInt();	//get the indices of the inReach blobs
			blobsInReachList.append(*blobList.get(blobsInReachIndices[j]).asList());
			// Bottle &blob = blobsInReachList.addList();//(blobList.get(blobsInReachIndices[j]).asList());			
			// blob.addDouble(blobList.get(blobsInReachIndices[j]).asList()->get(0).asDouble()); XXX repeat
		}
	}
	return blobsInReachIndices;
}
*/

/**********************************************************/
/*
bool AffManager::selectToolExe(){
	
	vector<int> blobsInReachIndices = findToolsExe();
	cv::Point tl,br;

	Bottle cmdBlob, replyBlob;
    cmdBlob.clear();
    replyBlob.clear();
    cmdBlob.addString("long");						// Choose the tool selectoin method
	Bottle indices = cmdBlob.addList();
	for (int i=0;i<blobsInReachIndices.size();i++){ // Send the indices of the potential tools to choose from
		indices.addInt(blobsInReachIndices[i]);
	}
	rpcBlobPicker.write(cmdBlob,replyBlob);			// Ask BlobPicker to select the right blob from the potentially good
													// Receive the index of the correct blob		
	if (!replyBlob.size()==1){						// Only one index should have been received
		fprintf (stdout,"Could not decide for the right tool \n");   
		return false;
	}
	if (replyBlob.get(0).isInt()){
		int blobI = replyBlob.get(0).asInt();					//get bounding box coordinates
		tl.x = blobsInReachList.get(blobI).asList()->get(0).asInt();
		tl.y = blobsInReachList.get(blobI).asList()->get(1).asInt();
		br.x = blobsInReachList.get(blobI).asList()->get(2).asInt();
		br.y = blobsInReachList.get(blobI).asList()->get(3).asInt();

		target2Dcoords.x = (tl.x+br.x)>>1;	// divide by two to get the centroid of the object's blob.
        target2Dcoords.y = (tl.y+br.y)>>1;	//
		return true;
	}
	else{
		return false;
	}
}
*/

/**********************************************************/
/*
Bottle AffManager::getBlobs()
{
   // grab resources
   // mutexResources.wait();
	Bottle blobList;
    if (Bottle *pBlobs=blobSpotter.read(false))
    {
        blobList=*pBlobs;
        printf("Received blobs list: %s\n",blobList.toString().c_str());

        if (blobList.size()==1)
        {
            if (blobList.get(0).asVocab()==Vocab::encode("empty"))
                blobList.clear();
        }
    }
    // release resources
    //mutexResources.post();

    return blobList;
}
*/

/**********************************************************/
/*
bool AffManager::get3Dposition()
{
    Bottle cmdMotor,replyMotor;
    cmdMotor.addString("get");
    cmdMotor.addString("s2c");
    Bottle &options=cmdMotor.addList();
    options.addString(camera.c_str());
    options.addInt(target2Dcoords.x);
    options.addInt(target2Dcoords.y);
    printf("Sending motor query: %s\n",cmdMotor.toString().c_str());
    rpcMotorAre.write(cmdMotor,replyMotor);
    printf("Received blob cartesian coordinates (for 2D coordinates %d %d): %s\n", target2Dcoords.x, target2Dcoords.y, replyMotor.toString().c_str());

    if (replyMotor.size()>=3)
    {
        target3Dcoords.resize(3);
        target3Dcoords[0]=replyMotor.get(0).asDouble();
        target3Dcoords[1]=replyMotor.get(1).asDouble();
        target3Dcoords[2]=replyMotor.get(2).asDouble();
		printf("3D cartesian coords found %d %d %d): %s\n", target3Dcoords[0], target3Dcoords[1], target3Dcoords[2]);
		return true;
    }
    else{
		printf("No proper coordinates retrieved \n");
        return false;
	}
}
*/
/**********************************************************/
/*
bool AffManager::get3Dorient()
{
	target3Dorient[0]=0.0; target3Dorient[1]=0.0; target3Dorient[2]=1.0; target3Dorient[3]=3.14;
	// XXX Use the PCL and minimumBoundingBox to get the orientation axis
	return true;
}
*/
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
