#ifndef __BLOBPICKER_H__
#define __BLOBPICKER_H__


#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>


class BlobPicker
{
	yarp::os::Port              rpcPort;				// rpc port (receive commands via rpc), returns bounding box of selected blob
	yarp::os::BufferedPort<yarp::os::Bottle> blobsInPort;							// input Port with blobs bounding boxes
	yarp::os::BufferedPort<yarp::os::Bottle> blobOutPort;							// output Port with selected blob bounding box
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > imInPort;		// make a port for reading images
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > imOutPort;		// output image Port with info drawn over  


public:

    BlobPicker()
    {
        // constructor
    }

    bool open();

    bool close();

    void loop(); 

    bool interrupt();

};

#endif


   
   



   