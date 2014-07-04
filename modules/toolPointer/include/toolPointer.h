/*
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Tanis Mar
 * email:  Tanis Mar
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#ifndef __TOOLPOINTER_H__
#define __TOOLPOINTER_H__

#include <yarp/os/BufferedPort.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/Thread.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
#include <yarp/os/Semaphore.h>
#include <yarp/os/Stamp.h>
#include <yarp/os/Port.h>
#include <yarp/os/RpcClient.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Image.h>

#include <time.h>
#include <string>
#include <iostream>
#include <iomanip>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cv.h>
#include <highgui.h>


class ToolPointer : public yarp::os::BufferedPort<yarp::os::Bottle>
{
private:

    std::string moduleName;                                 // string containing module name

    std::string coordsInPortName;                           // string containing incoming coords port name
    std::string segImgPortName;                             // string containing segmentation image port name
    std::string targetOutPortName;                          // string containing output target port name
    std::string imageOutPortName;                           // string containing output image port name
    std::string imgBinOutPortName;                          // string containing output binary image port name
    std::string rpcGBSPortName;                             // string containing rpc port name for GBS communication
    
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >    segImgPort;     // input port to receive segmenteatoin images

    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	imageOutPort;	// output image Port with info drawn over
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelMono> >	imgBinOutPort;	// output binary image of closest blob
    yarp::os::BufferedPort<yarp::os::Bottle>							targetOutPort;	// Send coordinates of closest point.      

    yarp::os::RpcClient                                                 rpcGBS;	        //rpc motor port GraphBasedSegmentation
    
    /* Pointer to the Resource Finder */
    yarp::os::ResourceFinder *moduleRF;
    /* Pointer to the Disparity Thread */
    //DisparityThread* dispThr;

    /* Algorithm Variables */
 
    int minBlobSize;
    bool verbose;
    float confidenceMin;
    
protected:

public:
    /**
     * constructor
     * @param moduleName is passed to the thread in order to initialise all the ports correctly
     */
    ToolPointer( const std::string &moduleName, yarp::os::ResourceFinder &module );
    ~ToolPointer();

    bool setVerbose(std::string verb);
    bool setConfMin(float confid);
    
    bool        open();
    void        close();
    void        onRead( yarp::os::Bottle &coords );
    void        interrupt();
    
    yarp::os::Semaphore         mutex;          //semaphore for accessing/modifying within the callback
       
};

class ToolPointerModule:public yarp::os::RFModule
{
    /* module parameters */
    std::string             moduleName;
    std::string             handlerPortName;    
    yarp::os::RpcServer     rpcInPort;

    /* pointer to a new thread */
    ToolPointer             *pointer;
    bool                    closing;

public:

    bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful
    bool interruptModule();                       // interrupt, e.g., the ports
    bool close();                                 // close and shut down the module

    double getPeriod();
    bool updateModule();
    bool respond(const yarp::os::Bottle &command, yarp::os::Bottle &reply);
};


#endif
//empty line to make gcc happy
