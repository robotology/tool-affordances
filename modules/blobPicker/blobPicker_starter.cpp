#include <stdio.h>
#include <yarp/os/Network.h>
#include "blobPicker.h"
#include <yarp/os/RFModule.h>

using namespace yarp::os;

class BlobPickerModule: public RFModule
{
   BlobPicker bPicker;
public:

    bool configure(ResourceFinder &rf)
    {
        return bPicker.open();
	}

    double getPeriod()
    {
        return 0.0;
    }
    
    bool updateModule()
    { 
        bPicker.loop();
        return true; 
    }

    bool interruptModule()
    {
        fprintf(stderr, "Interrupting\n");
        bPicker.interrupt();
        return true;
    }

    bool close()
    {
        fprintf(stderr, "Calling close\n");
        bPicker.close();
        return true;
    }

    //void respond();

};

int main(int argc, char *argv[]) 
{
    Network yarp;

    BlobPickerModule module;
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