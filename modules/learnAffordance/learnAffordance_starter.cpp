// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <stdio.h>
#include <yarp/os/Network.h>

#include "learnAffordance.h"
#include <yarp/os/RFModule.h>
//#include <yarp/dev/all.h>

//YARP_DECLARE_DEVICES(icubmod)

using namespace yarp::os;
//using namespace yarp::dev;


class learnAffordanceModule: public RFModule
{
   LearnAffordance learnAffordance;

public:

    bool configure(ResourceFinder &rf)
    {
		return learnAffordance.open();		
    }

    double getPeriod()
    {
        return 0.0;
    }
    
    bool updateModule()
    { 
        learnAffordance.loop();
        return true; 
    }

    bool interruptModule()
    {
        fprintf(stderr, "Interrupting\n");
        learnAffordance.interrupt();
        return true;
    }

    bool close()
    {
        fprintf(stderr, "Calling close\n");
        learnAffordance.close();
        return true;
    }

    //void respond();

};

int main(int argc, char *argv[]) 
{
    // we need to initialize the drivers list 
    //YARP_REGISTER_DEVICES(icubmod)
	Network yarp;

	learnAffordanceModule module;
    ResourceFinder rf;

    if (!module.configure(rf))
    {
        fprintf(stderr, "Error configuring module returning\n");
        return -1;
    }

    module.runModule();

    printf("Module shutting down\n");

    return 0;
}