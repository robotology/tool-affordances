
#ifndef __BLOB3DINFO_H__
#define __BLOB3DINFO_H__


#include <yarp/os/RFModule.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>

#include <blob3Dinfo_IDLServer.h>

class Blob3DInfo : public yarp::os::RFModule, public blob3Dinfo_IDLServer
{
	yarp::os::Port              rpcPort;				// rpc port (receive commands via rpc), returns bounding box of selected blob
	yarp::os::BufferedPort<yarp::os::Bottle> blobsInPort;								// input Port with blobs bounding boxes
	yarp::os::BufferedPort<yarp::os::Bottle> infoOutPort;								// input Port with blobs bounding boxes
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > imBinOutPort;		// make a port for reading images
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > im3DOutPort;		// output image Port with info drawn over  


public:

    Blob3DInfo()
    {
        // constructor
    }

    bool open();
	bool close();
	void loop(); 
	bool interrupt();

	bool start();
	bool stop();
	std::vector<int> blobsInRange( double range);
	info3D get3Dinfo();


};

#endif

   