#include <stdio.h>
#include <yarp/os/Network.h>
#include "blob3Dinfo.h"
#include <yarp/os/RFModule.h>

using namespace yarp::os;

class Blob3DInfoModule: public RFModule
{
   Blob3DInfo b3Dinfo;
public:

    bool configure(ResourceFinder &rf)
    {
        return b3Dinfo.open();
	}

    double getPeriod()
    {
        return 0.0;
    }
    
    bool updateModule()
    { 
        b3Dinfo.loop();
        return true; 
    }

    bool interruptModule()
    {
        fprintf(stderr, "Interrupting\n");
        b3Dinfo.interrupt();
        return true;
    }

    bool close()
    {
        fprintf(stderr, "Calling close\n");
        b3Dinfo.close();
        return true;
    }

    //void respond();

};

int main(int argc, char *argv[]) 
{
    Network yarp;

    Blob3DInfoModule module;
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