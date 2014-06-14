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

    //simMode=rf.check("simMode", Value("off"),"simMode (string)").asString();
	
	attach(rpcCmd);
	running=true;
	lookingAtTool = false;
    actionDone = false;

	//incoming
	/*bool ret = true;  
	ret = blobSpotter.open(("/"+name+"/blobSpot:i").c_str());
	if (!ret){
		printf("Problems opening incoming ports\n");
		return false;
	}
	*/

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
    optCart.put("remote","/"+robot+"/cartesianController/right_arm");
    optCart.put("local","/"+name+"/cartesian_client/right_arm");
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

    igaze->setNeckTraj-+Time(0.8);
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

bool AffManager::getTool(){
	askForToolExe();
    if (!graspToolExe()){
        std::cout << "Tool couldnt be grasped properly, stopping!" << std::endl;
        return false;
    }
    lookAtToolExe();

    attachToolExe();
    observeToolExe();

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
/*
bool AffManager::lookAtPoint(){
	lookAtPointExe();
	return lookingAtTool;
}
*/
/*
bool AffManager::lookAtRack(){
	lookAtRackExe();
	return lookingAtTool;
}
*/
/*
bool AffManager::reachTool(){
	reachToolExe();
	return true;
}
*/

bool AffManager::observeTool(){
	observeToolExe();
	return true;
}
bool AffManager::observeObj(){
	observeObjExe();
	return true;
}
/*
int  AffManager::findTools(){
	vector<int> blobsInReachIndices = findToolsExe();	
	return blobsInReachIndices.size();
}
*/
/*
bool AffManager::selectTool(){
	selectToolExe();
	return true;
}
*/

bool AffManager::doAction(){
	observeObjExe();
    slideActionExe();
    observeObjExe();
    computeEffect();
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

    int drawAngle = (int)Rand::scalar(0,359);
    double drawDist = 0.2;
    cmdKM.addString("draw");
    cmdKM.addDouble(target3DcoordsIni[1]);
    cmdKM.addDouble(target3DcoordsIni[2]);
    cmdKM.addInt(drawAngle);
    cmdKM.addDouble(drawDist);
    fprintf(stdout,"Performing draw with angle %d on object on coords %s\n",drawAngle, target3DcoordsIni.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM); // Call and featExt module to get tool features.


    // Save the features of the action
    Bottle cmdLearn,replyLearn;
    cmdLearn.clear();
    replyLearn.clear();
    cmdLearn.addString("addData");
    cmdLearn.addString("actionFeats");
    // Add the action data in the format accepted by affLearn: ("Object" ( 4 6 3 2 ) "Obj2" (3 5 6 21 ))
    Bottle& bData = cmdLearn.addList();
    bData.addString("action 1");
    Bottle &toolData = bData.addList();
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

    // find the dimensions and tooltip of the tool
    Bottle toolDim;
    cmdKM.addString("find");
    cmdKM.addString("right");	// arm
    cmdKM.addString("right");	// eye
    fprintf(stdout,"%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM); // Call and featExt module to get tool features.
    toolDim = replyKM.tail();
    fprintf(stdout,"TOOL AT %s:\n", toolDim.toString().c_str());

    cmdKM.clear();    replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("remove");
    rpcKarmaMotor.write(cmdKM, replyKM); // Call and featExt module to get tool features.

    cmdKM.clear();replyKM.clear();
    cmdKM.addString("tool");
    cmdKM.addString("attach");
    cmdKM.addString("right");
    cmdKM.addDouble(toolDim.get(0).asDouble());
    cmdKM.addDouble(toolDim.get(1).asDouble());
    cmdKM.addDouble(toolDim.get(2).asDouble());
    fprintf(stdout,"%s\n",cmdKM.toString().c_str());
    rpcKarmaMotor.write(cmdKM, replyKM);

    fprintf(stdout,"reply is %s:\n",replyKM.toString().c_str());
    return;
}

/**********************************************************/
void AffManager::observeToolExe()
{
    Bottle cmdFE,replyFE;
    cmdFE.clear();
    replyFE.clear();
    cmdFE.addString("snapshot");
    fprintf(stdout,"%s\n",cmdFE.toString().c_str());
    rpcFeatExt.write(cmdFE, replyFE); // Call and featExt module to get tool features.
    // At this point, NearThingdetector, which will be running on parallel, should separate the tool blob from the rest using disparity

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

/**********************************************************/
void AffManager::observeObjExe()
{
	//Time::delay(0.5);
    fprintf(stdout,"Start 'observe' procedure:\n");
    Bottle cmdFinder,replyFinder;
    cmdFinder.clear();
    replyFinder.clear();
    cmdFinder.addString("getPoint");
	// XXX So far point at the object. Improve to detect it automatically.
	rpcObjFinder.write(cmdFinder, replyFinder);
    lookingAtTool = false;
    lookingAtObject = true;
    fprintf(stdout,"3D point received!\n");
    Bottle coords3dB = replyFinder.tail();
    if (!actionDone){
        target3DcoordsIni[0] = coords3dB.get(0).asDouble();
        target3DcoordsIni[1] = coords3dB.get(1).asDouble();
        target3DcoordsIni[2] = coords3dB.get(2).asDouble();
        actionDone = true;
    }else{
        target3DcoordsAfter[0] = coords3dB.get(0).asDouble();
        target3DcoordsAfter[1] = coords3dB.get(1).asDouble();
        target3DcoordsAfter[2] = coords3dB.get(2).asDouble();
        }
    fprintf(stdout,"Object is located at %s:\n",replyFinder.toString().c_str());

    return;
}

/**********************************************************/
void AffManager::computeEffect()
{
    //To compute the effect, we assume that the object hasnt move in the z axis ( that is, has remained on the table)
    Vector delta = target3DcoordsAfter - target3DcoordsIni;
    double dx = delta[0];
    double dy = delta[1];

    effectAlpha = atan2 (dy,dx) * 180 / PI;
    effectDist = sqrt(dx*dx+dy*dy);  //sum of the squares of the differences

    // save the effect features
    Bottle cmdLearn,replyLearn;
    cmdLearn.clear();
    replyLearn.clear();
    cmdLearn.addString("addData");
    cmdLearn.addString("effectFeats");
    // Add the effect data in the format accepted by affLearn: ("Object" ( 4 6 3 2 ) "Obj2" (3 5 6 21 ))
    Bottle& bData = cmdLearn.addList();
    bData.addString("effect 1");
    Bottle &effData = bData.addList();
    effData.addDouble(effectAlpha);
    effData.addDouble(effectDist);
    fprintf(stdout,"%s\n",cmdLearn.toString().c_str());
    rpcAffLearn.write(cmdLearn, replyLearn);            // Send features to affLearn so they are saved and used for learning
    
    return;
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
