// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Network.h>
#include <yarp/os/RpcClient.h>

//for iCart
#include <yarp/sig/Vector.h>
#include <yarp/dev/CartesianControl.h>
#include <stdio.h>
#include <deque>
//#include <yarp/dev/all.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::os;

YARP_DECLARE_DEVICES(icubmod)

int main(int argc, char *argv[]) 
{
    Network yarp;
	BufferedPort<yarp::os::Bottle> infoOutPort;
	yarp::os::RpcClient rpcOutPort;
	YARP_REGISTER_DEVICES(icubmod)

	//open ports
	bool ret=true;
	const char* infoPortName="/askForTool/info:o";
	const char* rpcPortName="/askForTool/rpc:o";
    ret = infoOutPort.open(infoPortName);  // give the port a name
	ret = ret & rpcOutPort.open(rpcPortName);  // give the port a name 
	if (!ret){
		printf("Problems opening askForTool ports\n");
		return -1;
	}
	printf("Ports opened\n");
	
	//open iCart	
	Property optCart;
	optCart.put("device","cartesiancontrollerclient");
    optCart.put("remote","/icubSim/cartesianController/left_arm");
    optCart.put("local","/cartesian_client/left_arm");

	PolyDriver clientCart(optCart);
	ICartesianControl *icart;
	
	if (clientCart.isValid()) {
	   clientCart.view(icart);
	}
	

	//Find orientation and position of the hand
	Vector effectorPose(3);	
	Vector effectorOr(4);	
	printf("Obtaining pose\n");
	icart->getPose(effectorPose, effectorOr);
	printf("Pose obtained\n");
	printf("Hand effector in position %g %g %g \n",effectorPose[0], effectorPose[1], effectorPose[2]);	
	printf("Hand effector in orientation %g %g %g %g \n",effectorOr[0], effectorOr[1], effectorOr[2],effectorOr[3]);

	char poseStr [50];
	sprintf (poseStr, "world mk smodel rakeSU.x a.bmp %g %g %g",effectorPose[0], effectorPose[1], effectorPose[2]);
		//bind tool via magnet command
	Network::connect(rpcPortName, "/icubSim/world");
	Bottle rpcCommands(poseStr);
	Bottle reply;
	rpcOutPort.write(rpcCommands, reply);
	
	Bottle &infoOut = infoOutPort.prepare();			   // prepare port to send blob coords
	infoOut.addString("tool grasped");
	infoOutPort.write();
	infoOutPort.close();

    return 0;
}