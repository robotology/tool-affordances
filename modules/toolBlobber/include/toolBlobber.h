/*
 * IMAGE SEEDING AND NORMALIZATION 
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Tanis Mar
 * email: tanis.mar@iit.it
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

/** 
\defgroup toolBlobber Normalizes segmentated image to prepare it for Feature Extraction
Uses Graph-Based Segmentation and optionally StereoVision to segment the tool blob from the background on 2D.
 

\section intro_sec Description 
This module objective is to segment the tool from the background and output only the tool's blob on a binary image. 
It can do so based solely on the Graph-Based Segmentation algorithm seeded with the tooltip position (computed elsewhere), 
or using the iCubs stereoVision to detect the tool as the closest blob to the iCub. 

\section lib_sec Libraries 
    - YARP
    - icubmod
    - OpenCV
    - stereoVision

\section parameters_sec Parameters 
No inline parameters
  
\section portsa_sec Ports Accessed
Assumes that robotInterface (with ICartesianControl interface implemented) is running. 

\section portsc_sec Ports Created 
- \e /toolBlobber/rpc:i receives the information to execute the different possible tasks as a Bottle. 
It manages the following commands through respond interface:
   -# <b>seed</b> <i>(int) x (int) y </i> Calls the toolBlobber with the seed coordinates given.
   -# <b>label</b>  <i>(string) label</i> (default = 'undef'): sets the label that will be given to subsequently extracted feature vectors to string 'label'.             
   -# <b>setROI</b> <i>(int) tl.x (int) tl.y (int) br.x (int) br.y</i>: Sets a region of interest for feature extraction from the coordinates given as pixel positions. 
   -# <b>refAngle</b>  <i>(int) angle</i>: Sets the refernece angle for prespective nomalization of the blob to 'angle'.    
   -# <b>snapshot</b>: Performs feature extraction on a single frame. 
   -# <b>click</b>: "Asks user to click on viewer and performs feature extraction on the selected blob on one frame.
   -# <b>verbose</b> <i>(string) ON/OFF</i> : ON/OFF sets printfs of the extraction process on or off, respectively.
   -# <b>reset</b>: Resets ROI and all other set values to default. 
   -# <b>help </b>: Produces help on rpc commands. 
    
- \e /toolBlobber/seed:i            Receives seed coordinates to generate blob from GBS module
- \e /toolBlobber/imLeft:i          Left camera image to feed stereoVision Library
- \e /toolBlobber/disp:i            StereoDisparity disparity image
- \e /toolBlobber/img:o             Send out the selected blob and coordinates superposed on the disparity image
- \e /toolBlobber/target:o			Sends out coordinates of selected object
- \e /toolBlobber/imgBin:o			Send out the binary image of selected blob
- \e /toolBlobber/gbs:rpc			Send commands to the Graph Based Segmentation module

\section tested_os_sec Tested OS
Windows, Linux

\author Tanis Mar
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
