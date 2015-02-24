/*
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Tanis Mar
 * email:  tanis.mar@iit.it
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
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >	inImPort;       // port for reading images
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
    std::string objName;               //label for the nextr trials

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

   
   


