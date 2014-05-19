// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>
#include <yarp/os/Network.h>
#include <yarp/os/RpcClient.h>

using namespace yarp::os;

int main(int argc, char *argv[]) 
{
    Network yarp;
	yarp::os::RpcClient rpcOutPort;

	bool ret=true;
	const char* portName="/startAffEnv/rpc:o";
    ret = rpcOutPort.open(portName);  // give the port a name
	if (!ret){
		printf("Problems opening startAffEnv ports\n");
		return -1;
	}
	Network::connect(portName, "/icubSim/world");

	Bottle rpcCommands("world mk cyl 0.01 0.2 0.1 0.7 0.4 0 0 1");
	Bottle reply;
	rpcOutPort.write(rpcCommands, reply);
	Network::disconnect(portName, "/icubSim/world");

	Network::connect(portName, "/icubSim/head/rpc:i");
	rpcCommands.fromString("set pos 0 -45");
	rpcOutPort.write(rpcCommands, reply);
	rpcOutPort.close();

    return 0;
}