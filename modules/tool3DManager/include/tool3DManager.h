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

#ifndef __TOOL3DMANAGER_H__
#define __TOOL3DMANAGER_H__

// Includes
#include <string>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <deque>

// YARP libraries
#include <yarp/os/RFModule.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Port.h>
#include <yarp/os/Os.h>
#include <yarp/os/Time.h>

#include <yarp/sig/Image.h>
#include <yarp/sig/Vector.h>

#include <yarp/math/Rand.h>
#include <yarp/math/Math.h>

//for iCart and iGaze
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/GazeControl.h>
#include <yarp/dev/CartesianControl.h>

#include <iCub/ctrl/math.h>
#include <iCub/ctrl/pids.h>

//for the thrift server interface
#include <tool3DManager_IDLServer.h>

// for thrift connection with toolFeatExt XXX
// #include <tool3DFeat_IDLServer.h>
// #include <Point3D.h>

//YARP_DECLARE_DEVICES(icubmod)

/**********************************************************/
class Tool3DManager : public yarp::os::RFModule, public tool3DManager_IDLServer
{
protected:

	/* module parameters */
	std::string                 name;               //name of the module
	std::string                 hand;               //hand
	std::string                 camera;             //camera
	std::string                 robot;				//robot
    double                      tableHeight;        // height of the table with respect to the robot frame

    yarp::os::RpcServer         rpcCmd;				//human rpc port (receive commands via rpc)

    yarp::os::RpcClient         rpcSimTL;           //rpc for the simulator tool loader
	yarp::os::RpcClient         rpcMotorAre;        //rpc motor port ARE
    yarp::os::RpcClient         rpcKarmaMotor;      //rpc motor port KARMA
    yarp::os::RpcClient         rpcKarmaFinder;     //rpc finder port KARMA

    yarp::os::RpcClient         rpcObjFinder;       //rpc connecting to object finder
    yarp::os::RpcClient         rpcToolShow;        //rpc connecting to pointcloud 3D visualizer
    yarp::os::RpcClient         rpcFeatExt;	        //rpc connection to toolFeatExt to extract features from cloud

    yarp::os::BufferedPort<yarp::os::Bottle >  userDataPort;  // port to receive user data.
    yarp::os::Port                             outDataPort;   // port to send out data
    yarp::os::BufferedPort<yarp::os::Bottle >  matlabPort;    // port to receive predictions from MATLAB

    /* class variables */

    //ParticleFilter              particleFilter;     //class to receive positions from the templateTracker module
    //SegmentationPoint           segmentPoint;       //class to request segmentation from activeSegmentation module
    //PointedLocation             pointedLoc;         //port class to receive pointed locations

    yarp::dev::PolyDriver					clientCart;
    yarp::dev::ICartesianControl			*icart;
    int                                     cartCntxt;

    yarp::dev::PolyDriver					clientTorso;
    yarp::dev::IPositionControl				*iTorso;

    yarp::dev::PolyDriver                   clientGaze;
    yarp::dev::IGazeControl                 *igaze;
    int                                     gazeCntxt;
    	
	// Flags
	bool						running;
    bool                        actionDone;
    bool						objCoords3DLoc;
	bool						trackingObj;
	bool 						tipOnView;
    
	yarp::sig::Vector			toolTipPix;		// Keeps the tool position in 2D for looking at it
	yarp::sig::Vector			target3DcoordsIni;		// Keeps the target position in 3D
    yarp::sig::Vector			target3DcoordsAfter;	// Keeps the target position in 3D

    yarp::sig::Vector			toolDim;    		    // Keeps the dimensions of the detected tool
    std::string                 toolPoseName;

    double                      effectAlpha;            // Angle in which the object has been moved by the action
    double                      effectDist;             // Distance that the object has been moved by the action

    struct                      Point3Dcoords { float x, y, z;};
    Point3Dcoords               tooltipCoords;
	
	
    /* Protected Methods */
    void                        goHomeExe(bool hands = false);
    void                        loadToolSim(int toolI = 0, int graspOr = 0, double graspDisp = 0);
    void                        graspTool(); // XXX



public:

	/* RPC commands handling */
	//atomic commands
    bool						attach(yarp::os::RpcServer &source);
	bool						start();
	bool						quit();
    bool                        goHome(bool hands = false);
    bool                        getTool(int toolI = 0, int graspOr = 0, double graspDisp = 0);

    // RF modules overrides
    bool						configure(yarp::os::ResourceFinder &rf);
    bool						interruptModule();
    bool						close();
    bool						updateModule();
    double						getPeriod();
};

#endif

