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

#include <sstream>
#include <stdio.h>
#include <yarp/math/Rand.h>
#include <yarp/math/Math.h>
#include <yarp/os/Os.h>
#include <yarp/os/Time.h>

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
        hand="left";

	hand=rf.find("robot").asString().c_str();
    if ((hand!="icubSim") && (hand!="icub"))
        hand="icubSim";

    //simMode=rf.check("simMode", Value("off"),"simMode (string)").asString();
	
	attach(rpcCmd);
	running=true;
	lookingAtTool = false;

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
    // retRPC = retRPC && rpcMotorKarma.open(("/"+name+"/karma:rpc").c_str());            //rpc server to query Karma
    // retRPC = retRPC && rpcBlobSpot.open(("/"+name+"/blobSpot:rpc").c_str());           //rpc server to query blobSpotter
	retRPC = retRPC && rpcBlobPicker.open(("/"+name+"/blobPick:rpc").c_str());         //rpc server to query blobPicker
	retRPC = retRPC && rpcBlobInfo.open(("/"+name+"/blobInfo:rpc").c_str());			//rpc server to query blob3Dinfo
	if (!retRPC){
		printf("Problems opening rpc ports\n");
		return false;
	}
	
	// Cartesian Controller Interface
	Property optCart;
	optCart.put("device","cartesiancontrollerclient");
    optCart.put("remote","/icub/cartesianController/left_arm");
    optCart.put("local","/cartesian_client/left_arm");
	if (!clientCart.open(optCart))
		return false;    
    clientCart.view(icart);	// open the view	

	return true;
}


/**********************************************************/
bool AffManager::updateModule()
{
	/* Depending on the human input, perform different actions. 
	These can range from carrying out single actions to the whole experiment.
	Single actions:
	- 'prepare' : Look to the scenario and get the robot ready to start the experiment
	- 'takeTool':	 
	*/

	if (!running)
        return false;
	return true;

}

/**********************************************************/
bool AffManager::interruptModule()
{
    rpcCmd.interrupt();
    rpcMotorAre.interrupt();
	rpcMotorIol.interrupt();
    rpcMotorKarma.interrupt();

    return true;
}

/**********************************************************/
bool AffManager::close()
{
    rpcCmd.close();
    rpcMotorAre.close();
	rpcMotorIol.close();
    rpcMotorKarma.close();

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
bool AffManager::lookAtTool(){
	lookAtToolExe();
	return true;
}
bool AffManager::lookAtPoint(){
	lookAtPointExe();
	return lookingAtTool;
}
bool AffManager::lookAtRack(){
	lookAtRackExe();
	return lookingAtTool;
}
bool AffManager::askForTool(){
	askForToolExe();
	return true;
}
bool AffManager::reachTool(){
	reachToolExe();
	return true;
}
bool AffManager::graspTool(){
	graspToolExe();
	return toolInHand;
}
bool AffManager::observeTool(){
	observeToolExe();
	return true;
}
bool AffManager::observeObj(){
	observeObjExe();
	return true;
}
int  AffManager::findTools(){
	vector<int> blobsInReachIndices = findToolsExe();	
	return blobsInReachIndices.size();
}
bool AffManager::selectTool(){
	selectToolExe();
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

void AffManager::lookAtToolExe()
{
	
	findToolsExe();
	lookAtRackExe();

}

/**********************************************************/
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

/**********************************************************/
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

/**********************************************************/
void AffManager::askForToolExe()
{
	fprintf(stdout,"Reach me a tool please.\n");
	fprintf(stdout,"Start 'take_tool' procedure:\n");
    Bottle cmdAre, replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("tato");
    cmdAre.addString("left");
    rpcMotorAre.write(cmdAre,replyAre);
    fprintf(stdout,"Waiting for tool %s:\n",replyAre.toString().c_str());
}

/**********************************************************/
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

/**********************************************************/
void AffManager::graspToolExe()
{
    Time::delay(0.5);
	// Close hand toolwise
    fprintf(stdout,"Start 'clto' 'left' (close tool left) procedure:\n");
    Bottle cmdAre,replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("clto");
    fprintf(stdout,"%s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    fprintf(stdout,"'clto' 'left' action is %s:\n",replyAre.toString().c_str());
	Time::delay(0.5);

	// Check if grasp was successful
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("get");
	cmdAre.addString("holding");
    fprintf(stdout,"%s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
	toolInHand = replyAre.get(0).asBool();
}

/**********************************************************/
void AffManager::observeToolExe()
{
	Time::delay(1.0);
    fprintf(stdout,"Start 'observe' procedure:\n");
    Bottle cmdAre,replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("observe");
    fprintf(stdout,"%s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    fprintf(stdout,"'observe' 'action is %s:\n",replyAre.toString().c_str());


    Bottle cmdBlob,replyBlob;
    cmdBlob.clear();
    replyBlob.clear();
    cmdBlob.addString("snapshot");
    fprintf(stdout,"%s\n",cmdBlob.toString().c_str());
    rpcBlobInfo.write(cmdAre, replyAre);
	// XXX separate the tool blob from the rest using disparity, and perform featEXT to get features.
}

/**********************************************************/
void AffManager::observeObjExe()
{
	Time::delay(1.0);
    fprintf(stdout,"Start 'observe' procedure:\n");
    Bottle cmdAre,replyAre;
    cmdAre.clear();
    replyAre.clear();
    cmdAre.addString("look");
	// XXX So far point at the object. Improve to detect it automatically.
	cmdAre.addString("raw");
    fprintf(stdout,"%s\n",cmdAre.toString().c_str());
    rpcMotorAre.write(cmdAre, replyAre);
    fprintf(stdout,"'observe' 'action is %s:\n",replyAre.toString().c_str());
	// XXX Perform featExt on the Objects blob
}


// Perceptual Methods
/**********************************************************/
vector<int> AffManager::findToolsExe()
{
	/*Call blob3Dinfo to get the indices of the blobs within a given range.
	Filter the blob image keeeping only those blobs, and pass it to ToolSelector/blobpicker.
	Which uses featExt to select the correct blob based on its features.
	*/
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
	
	/* Thrift approach
	AffManager affManager;
	affManager.yarp().attachAsClient(rpcBlobInfo);
	affManager.blobsInRange(0.5);
	*/

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
/**********************************************************/
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


/**********************************************************/
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

/**********************************************************/
bool AffManager::get3Dorient()
{
	target3Dorient[0]=0.0; target3Dorient[1]=0.0; target3Dorient[2]=1.0; target3Dorient[3]=3.14;
	// XXX Use the PCL and minimumBoundingBox to get the orientation axis
	return true;
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
	rf.setDefault("robot","icubSim");
    //rf.setDefaultContext("affManager");
    //rf.setDefaultConfigFile("affManager.ini");
    //rf.setDefault("tracking_period","30");
    rf.configure(argc,argv);

    AffManager affManager;
    return affManager.runModule(rf);
}
