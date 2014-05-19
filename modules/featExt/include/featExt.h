
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>
#include <yarp/sig/Vector.h>

#include "VecVec.h"
#include "Feature.h"

class FeatExt
{
	yarp::os::Port														rpcCmd;           //human rpc port (receive commands via rpc)
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	inImPort;  // port for reading rgb images
	yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	binImPort; // port for reading binary images
	yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	imPropOutPort; // port for sending output images
	yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	imFeatOutPort; // port for sending output images
    yarp::os::BufferedPort<VecVec> featPort;

public:

    FeatExt()
    {
        // constructor
    }

    bool open();

    bool close();

    void loop(); 

    bool interrupt();

	double getPeriod();

	int processRpcCmd(const yarp::os::Bottle &cmd, yarp::os::Bottle &b);

	void featExtractor(const yarp::sig::ImageOf<yarp::sig::PixelRgb>& img );	

};

   
   


