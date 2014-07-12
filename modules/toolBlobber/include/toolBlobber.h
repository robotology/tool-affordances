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

#ifndef __TOOLBLOBBER_H__
#define __TOOLBLOBBER_H__

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

//for iCart and iGaze
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/GazeControl.h>
#include <yarp/dev/CartesianControl.h>

//openCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
//#include <highgui.h>

YARP_DECLARE_DEVICES(icubmod)

class ToolBlobber : public yarp::os::BufferedPort<yarp::os::Bottle >
{
private:

    /* Internal functions */
    bool getDispBlob(const cv::Mat& contourMask, cv::Mat& disp, cv::Point seed);

    std::string moduleName;                     // string containing module name
    std::string dispInPortName;                 // string containing disparity image port name
    std::string seedInPortName;                 // string containing the seed point to get the blob port name
    std::string imInLeftPortName;               // string containing left input image port name
    std::string targetOutPortName;              // string containing output target port name
    std::string imageOutPortName;               // string containing output image port name
    std::string imgBinOutPortName;              // string containing output binary image port name
    std::string rpcGBSPortName;                 // string containing rpc port name for GBS communication
    
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelBgr> >	dispInPort;		// Receives disparity greyscale image --- Handled by the clas itself    
    //yarp::os::BufferedPort<yarp::os::Bottle>	                        seedInPort;
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >    imInLeftPort;
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	imageOutPort;	// output image Port with info drawn over
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelMono> >	imgBinOutPort;	// output binary image of closest blob
    yarp::os::BufferedPort<yarp::os::Bottle>							targetOutPort;	// Send coordinates of closest point.      

    yarp::os::RpcClient                                                 rpcGBS;	    //rpc motor port GraphBasedSegmentation
    

    /* interface variables */ 
    yarp::dev::PolyDriver					clientCart;
	yarp::dev::ICartesianControl			*icart;
    yarp::dev::PolyDriver                   clientGaze;
    yarp::dev::IGazeControl                 *igaze;

    /* Pointer to the Resource Finder */
    yarp::os::ResourceFinder *moduleRF;

   /* module parameters */
	std::string                 hand;               //hand
	std::string                 camera;             //camera
	std::string                 robot;				//robot
    bool                        isDisp;              // bool representing if disparity image is present or not

    /* Algorithm Variables */
    int backgroundThresh;
    int frontThresh;
    int cannyThresh;
    int minBlobSize;
    bool verbose;
    float confidenceMin;
    
    int dispThreshRatioLow;
    int dispThreshRatioHigh;
    
    int gaussSize;
    float range;
    bool fixedRange;

protected:

    //cv::Rect blobBox; //(int x=5, int y=5, int width=5, int height=5); //create and dumm initialize blob bounding box
	
public:
    /**
     * constructor
     * @param moduleName is passed to the thread in order to initialise all the ports correctly
     */
    ToolBlobber( const std::string &moduleName, yarp::os::ResourceFinder &module );
    ~ToolBlobber();

    bool setRange(double range);
    bool setThresh(int low, int high);
    bool setVerbose(std::string verb);
    bool setConfMin(float confid);
    bool setFixedRange(bool fixRangeFlag);

    bool        open();
    void        close();
    //void        loop();
    void        onRead( yarp::os::Bottle& seed);
    void        interrupt();
    
    yarp::os::Semaphore         mutex;          //semaphore for accessing/modifying within the callback
       
};

class ToolBlobberModule:public yarp::os::RFModule
{
    /* module parameters */
    std::string             moduleName;
    std::string             handlerPortName;    
    yarp::os::RpcServer     rpcInPort;

    /* pointer to a new thread */
    ToolBlobber             *blobber;
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
