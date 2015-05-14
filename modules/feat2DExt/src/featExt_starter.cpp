#include <stdio.h>
#include <yarp/os/Network.h>
#include "featExt.h"
#include <yarp/os/RFModule.h>

using namespace yarp::os;

class FeatExtModule: public RFModule
{
   FeatExt fExt;
public:

    bool configure(ResourceFinder &rf)
    {
        return fExt.open();
    }

    double getPeriod()
    {
        return 0.5;
    }
    
    bool updateModule()
    { 
        fExt.loop();
        return true; 
    }

    bool interruptModule()
    {
        fprintf(stderr, "Interrupting\n");
        fExt.interrupt();
        return true;
    }

    bool close()
    {
        fprintf(stderr, "Calling close\n");
        fExt.close();
        return true;
    }

    void respond();

};

int main(int argc, char *argv[]) 
{
    Network yarp;

    FeatExtModule module;
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