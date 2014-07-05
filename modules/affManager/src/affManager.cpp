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

    tableHeight = rf.check("tableHeight", Value(-0.15)).asDouble();      // Height of the table in the robots coord frame

    //simMode=rf.check("simMode", Value("off"),"simMode (string)").asString();
	
	attach(rpcCmd);
	running=true;
	lookingAtTool = false;
    actionDone = false;
    objFound = false;

    toolDim.resize(3, 0.0);
    target3DcoordsIni.resize(3, 0.0);		// Keeps the target position in 3D
    target3DcoordsAfter.resize(3, 0.0);	    // Keeps the target position in 3D

	//rpc
	bool retRPC = true;    
    retRPC = rpcCmd.open(("/"+name+"/rpc:i").c_str());					//rpc client to interact with the italkManager
    retRPC = retRPC && rpcMotorAre.open(("/"+name+"/are:rpc").c_str());                //rpc server to query ARE
    retRPC = retRPC && rpcKarmaMotor.open(("/"+name+"/karmaMotor:rpc").c_str());            //rpc server to query Karma Motor    
	// retRPC = retRPC && rpcBlobPicker.open(("/"+name+"/blobPick:rpc").c_str());      //rpc server to query blobPicker
	retRPC = retRPC && rpcFeatExt.open(("/"+name+"/featExt:rpc").c_str());			   //rpc server to query featExtractin Module
    retRPC = retRPC && rpcAffLearn.open(("/"+name+"/affLearn:rpc").c_str());		   //rpc server to query affLearn
    retRPC = retRPC && rpcObjFinder.open(("/"+name+"/objFind:rpc").c_str());           //rpc server to query objectFinder
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
    rpcCmd.interrupt();
    rpcMotorAre.interrupt();
	//rpcMotorIol.interrupt();
    rpcKarmaMotor.interrupt();

    rpcFeatExt.interrupt();
    rpcAffLearn.interrupt();

    return true;
}

/**********************************************************/
bool AffManager::close()
{
    rpcCmd.close();
    rpcMotorAre.close();
	//rpcMotorIol.close();
    rpcKarmaMotor.close();

    rpcFeatExt.close();
    rpcAffLearn.close();

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
	askForToolExe();
    if (!graspToolExe()){
        std::cout << "Tool couldnt be grasped properly, stopping!" << std::endl;
        return false;
    }
    lookAtToolExe();

    //attachToolExe();
    observeToolExe();
    goHomeNoHandsExe();
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
    trackObjExe();
	bool objLocated = locateObjExe();
	if (objLocated)
	{
		slideActionExe();
		goHomeNoHandsExe();
		locateObjExe();
		computeEffect();
	}
    finishRound();
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
/*
void AffManager::lookAtPointExe()
{
	bool pointReached = false;
	int cnt =0;
	vector<int>  potTools;
	fprintf(stdout,"Point me where to look.\n");
	fprintf(stdout,"Start 'look' 'raw' procedure:\n");
	while (!lookingAtTool && cnt<10){  // XXX properly modifiy to flag based while
		Bottle cmdAre, replyAre;
		cmdAre.clear();
		replyAre.clear();
		cmdAre.addString("look");
		cmdAre.addString("raw");
		fprintf(stdout,"Looking to point\n");
		rpcMotorAre.write(cmdAre,replyAre);
		potTools = findToolsExe();
		if (potTools.size()>0){
			fprintf(stdout,"Tools Found!\n");
			lookingAtTool = true;
			return;		
		}
		else {
			fprintf(stdout,"Tried %d times \n", cnt);
			fprintf(stdout,"No Tool Found, point again \n");
			cnt++;
		}
	}
	fprintf(stdout,"I am tired of looking around, just give me the tool \n");
	return;
}
*/

/**********************************************************/
/*
void AffManager::lookAtRackExe()
{
	fprintf(stdout,"Start looking at Rack procedure:\n");
	fprintf(stdout,"Search on the right:\n");
	//Coordinates of points to the right/lef of the robot:
	double x = -0.1;
	double z = 0.5;
	double yR = 1;
	double yL = -1;
	vector<int>  potTools;

    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
	// first look right
    cmdAre.addString("look");
    cmdAre.addDouble(x);
	cmdAre.addDouble(yR);
	cmdAre.addDouble(z);
    rpcMotorAre.write(cmdAre,replyAre);
	potTools = findToolsExe();
	if (potTools.size()>0){
		lookingAtTool = true;
		return;
	}
	// if nothing found, then look left
	fprintf(stdout,"Nothing found, search on the left:\n");
	cmdAre.clear();
    replyAre.clear();
	// first look right
    cmdAre.addString("look");
    cmdAre.addDouble(x);
	cmdAre.addDouble(yR);
	cmdAre.addDouble(z);
    rpcMotorAre.write(cmdAre,replyAre);
	potTools = findToolsExe();
	if (potTools.size()>0){
		lookingAtTool = true;
		return;
	}
	// still nothing found
	lookingAtTool = false;
	return;
}
*/
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
/*
void AffManager::reachToolExe()
{
	get3Dposition();
	get3Dorient();	
	icart->goToPoseSync(target3Dcoords,target3Dorient);   // send request and wait for reply
	bool done=false;
	while (!done) {
	   icart->checkMotionDone(&done);
	   Time::delay(0.04);   // or any suitable delay
	}
	return;
}
*/

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
    if (robot== "icubSim")
    {
        simTool();
    }

    return;
}

void AffManager::handToCenter()
{
    Vector xd(3), od(4);                            // Set a position in the center in front of the robot
    xd[0]=-0.3; xd[1]=0.15; xd[2]=0.05;
    
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
    handPos[2] += 0.15;          // Tool center round 15 cm over the hand

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
    Time::delay(0.05);
    simCmd.addString("world rot cyl 1 90 0 0");
    simCmd.clear();reply.clear();
    Time::delay(0.05);
    simCmd.addString("world grab cyl 1 right 1");
    simPort.write(simCmd, reply);
    Network::disconnect(portName, "/icubSim/world");
    return;
}

/**********************************************************/
void AffManager::slideActionExe()
{
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();   replyKM.clear();

    int drawAngle = (int)Rand::scalar(0,180);
    double drawDist = 0.2;
    double drawRadius = 0.05;
    double minusTool = 0.15;
    cmdKM.addString("draw");
    cmdKM.addDouble(target3DcoordsIni[0]+minusTool); // Draw closer if tool has not been attached as endeffector
    cmdKM.addDouble(target3DcoordsIni[1]);
    cmdKM.addDouble(target3DcoordsIni[2]);
    cmdKM.addInt(drawAngle);
    cmdKM.addDouble(drawDist);
    cmdKM.addDouble(drawRadius);
    fprintf(stdout,"Performing draw with angle %d on object on coords %s\n",drawAngle, target3DcoordsIni.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM); // Call karmaMotor to execute the action
    actionDone = true;


    // Save the features of the action
    Bottle cmdLearn,replyLearn;
    cmdLearn.clear();
    replyLearn.clear();
    cmdLearn.addString("addData");
    cmdLearn.addString("actionFeats");
    // Add the action data in the format accepted by affLearn: ("Object" ( 4 6 3 2 ) "Obj2" (3 5 6 21 ))
    Bottle& bData = cmdLearn.addList();
    Bottle& bSample = bData.addList();
    bSample.addString("action 1");
    Bottle &toolData = bSample.addList();
    toolData.addInt(drawAngle);
    
    fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
    rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning


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

    // XXX if find not working, substitute here the values by hand measured ones.
    Bottle toolDimB = replyKM.tail();			// XXX Check that this TAIL actually works!!!
    toolDim[0] = toolDimB.get(0).asDouble();
    toolDim[1] = toolDimB.get(1).asDouble();
    toolDim[2] = toolDimB.get(2).asDouble();    
    cout << "Tool dims are" << toolDim.toString().c_str() << endl;

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
void AffManager::observeToolExe()
{
    
    // XXX Check that the ROI extraction works properly on the robot.
    // Get Hand coordinates on image 
    Vector handPose, handOr, handPix;
    icart->getPose(handPose, handOr);
    igaze->get2DPixel(0, handPose, handPix);

    // Get ToolTip coordinates on image
    Bottle cmdKM,replyKM;       // bottles for Karma Motor
    cmdKM.clear();   replyKM.clear();
    Bottle toolTipB;
    cmdKM.addString("tool");
    cmdKM.addString("get");	
    fprintf(stdout,"%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM); // Call and featExt module to get tool features.
    cout <<"Response: " << replyKM.toString().c_str() << endl;
    toolTipB = replyKM.tail();			// XXX Check that this TAIL actually works!!!
    cout <<"Tooltip coordinates: " << toolTipB.toString().c_str() << endl;

    Vector toolTipPose(3), toolTipPix(2);
    toolTipPose[0] = toolTipB.get(0).asDouble();
    toolTipPose[1] = toolTipB.get(1).asDouble();
    toolTipPose[2] = toolTipB.get(2).asDouble();    
    igaze->get2DPixel(0, toolTipPose, toolTipPix);

    // Set the ROI to bound the tool and crop the arm away
    Vector ROI(4);          // define ROI as tl.x, tl.y, br.x, br.y
	ROI[0] = toolTipPix[0] - 40;  // ROI left side some pixels to the left of the tooltip
    ROI[1] = toolTipPix[1]; // ROI top side at the same height of the tooltip
    ROI[2] = toolTipPix[0] + 40;  // ROI right side some pixels to the right of the tooltip
    ROI[3] = handPix[1]; // ROI bottom side at the same height of the hands center
    
    Bottle cmdFE,replyFE;
    cmdFE.clear();
    replyFE.clear();
    cmdFE.addString("setROI");
    cmdFE.addInt((int)ROI[0]);
    cmdFE.addInt((int)ROI[1]);
    cmdFE.addInt((int)ROI[2]);
    cmdFE.addInt((int)ROI[3]);
    fprintf(stdout,"%s\n",cmdFE.toString().c_str());
    rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.

    // Set the tool orientation as reference angle for feature extraction
    Vector tipDif(2);
    tipDif [0]= handPix[0] - toolTipPix[0];
    tipDif [1]= handPix[1] - toolTipPix[1];
    double handAngle = atan2(double(tipDif[1]), double(tipDif[0])) * 180.0 / M_PI;  
    
    cmdFE.clear();
    replyFE.clear();
    cmdFE.addString("refAngle");
    cmdFE.addInt((int)handAngle);
    rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.

    // Get the features
    cmdFE.clear();
    replyFE.clear();
    cmdFE.addString("snapshot");
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

    if(igaze->get3DPointOnPlane(camSel,coords2D, table, coords3D)){   // XXX check this on the robot, it crashes on simulator
        igaze->lookAtFixationPoint(coords3D);                 // move the gaze to the desired fixation point
        igaze->waitMotionDone();                              // wait until the operation is done
        printf(" Looking done\n");
    
        lookingAtObject = true;

	    if (!actionDone){			
            printf("The point selected is %.2f %.2f %.2f\n", coords3D[0], coords3D[1], coords3D[2]);
		    target3DcoordsIni[0] = coords3D[0];
		    target3DcoordsIni[1] = coords3D[1];
		    target3DcoordsIni[2] = coords3D[2];
		    fprintf(stdout,"Object is located at %s:\n", target3DcoordsIni.toString().c_str());
		    objFound = true;
	    }else{
		    cout << "3D coords after action are" << replyFinder.get(1).asList()->toString().c_str() << endl;
		    target3DcoordsAfter[0] = coords3D[0];
		    target3DcoordsAfter[1] = coords3D[1];
		    target3DcoordsAfter[2] = coords3D[2];
		    fprintf(stdout,"Object is located at %s:\n", target3DcoordsAfter.toString().c_str());
		    }
	} else  {
        printf("3D point couldnt be computed\n");
        objFound = false;
    }
    return objFound;
}

/**********************************************************/
void AffManager::computeEffect()
{
    //To compute the effect, we assume that the object hasnt move in the z axis ( that is, has remained on the table)
    Vector delta = target3DcoordsAfter - target3DcoordsIni;
    double dx = delta[0];
    double dy = delta[1];

    effectAlpha = atan2 (dy,dx) * 180 / M_PI;
    effectDist = sqrt(dx*dx+dy*dy);  //sum of the squares of the differences

    // save the effect features
    Bottle cmdLearn,replyLearn;
    cmdLearn.clear();
    replyLearn.clear();
    cmdLearn.addString("addData");
    cmdLearn.addString("effectFeats");
    // Add the effect data in the format accepted by affLearn: ("Obj1" ( 4 6 3 2 ) "Obj2" (3 5 6 21 ))
    Bottle& bData = cmdLearn.addList();
    Bottle& bSample = bData.addList();
    bSample.addString("effect");
    Bottle &effData = bSample.addList();
    effData.addDouble(effectAlpha);
    effData.addDouble(effectDist);
    fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
    rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning
    
    return;
}

void AffManager::finishRound()
{
    // reinitialize some flags for next round
    actionDone = false;
    objFound = false;
    lookingAtTool = false;
    
    target3DcoordsIni.clear();		// Keeps the target position in 3D before the action
    target3DcoordsAfter.clear();	// Keeps the target position in 3D after the action
    
    target3DcoordsIni.resize(3);    // Keeps the target position in 3D before the action
    target3DcoordsAfter.resize(3);	// Keeps the target position in 3D after the action

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
