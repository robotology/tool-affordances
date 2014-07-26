
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/Semaphore.h>

#include <cv.h>
#include <fstream>
#include <iostream>

#include "VecVec.h"
#include "Feature.h"

class FeatExt
{
	yarp::os::Port														rpcCmd;         //human rpc port (receive commands via rpc)
    yarp::os::BufferedPort<yarp::os::Bottle>                            coordsInPort;// Reads coords from yarpview out 
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	inImPort;       // port for reading rgb images
	yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	binImPort;      // port for reading binary images
	yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	imPropOutPort;  // port for sending output images
	yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	imFeatOutPort;  // port for sending output images
    yarp::os::Port                                                      featPort;

    yarp::os::Semaphore         mutex;
    bool processing;
    cv::Rect ROI;
    bool ROIinit;
    cv::Point coords;
    bool coordsInit;

    int imWidth;
    int imHeight;
    int refAngle;
    bool verbose;
    std::string toolPoseLabel;               //label for the Tool-pose

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

	void featExtractor(const yarp::sig::ImageOf<yarp::sig::PixelRgb>& img, VecVec& feats);	

    
};

   
   


