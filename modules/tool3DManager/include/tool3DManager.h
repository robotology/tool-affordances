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

    yarp::os::RpcClient         rpcSimToolLoader;           //rpc for the simulator tool loader
    yarp::os::RpcClient         rpcSimulator;           //rpc for the simulator tool loader
	yarp::os::RpcClient         rpcMotorAre;        //rpc motor port ARE
    yarp::os::RpcClient         rpcKarmaMotor;      //rpc motor port KARMA
    yarp::os::RpcClient         rpcKarmaFinder;     //rpc finder port KARMA
    yarp::os::RpcClient         rpcFeatExt;	        //rpc connection to toolFeatExt to extract features from cloud

    yarp::os::RpcClient         rpcObjFinder;       //rpc connecting to object finder
    //yarp::os::RpcClient         rpcToolShow;        //rpc connecting to pointcloud 3D visualizer


    yarp::os::Port                             effDataPort;   // port to send out data of computed effect
    yarp::os::Port                             actDataPort;   // port to send out data of action parameters
    yarp::os::Port                             graspDataPort; // port to send out data of grasp parameters
    yarp::os::BufferedPort<yarp::os::Bottle >  matlabPort;    // port to receive predictions from MATLAB

    /* class variables */
	// Flags
	bool						running;
    /*
    bool                        actionDone;
    bool						objCoords3DLoc;
	bool						trackingObj;
	bool 						tipOnView;
    */

    //yarp::sig::Vector			toolTipPix;             // Keeps the tool position in 2D for looking at it
    yarp::sig::Vector			target3DcoordsIni;		// Keeps the target position in 3D before the action
    yarp::sig::Vector			target3DcoordsAfter;	// Keeps the target position in 3D after the action
    yarp::sig::Vector			target3DrotIni;         // Keeps the target rotation in 3D before the action
    yarp::sig::Vector			target3DrotAfter;		// Keeps the target rotation in 3D after the action
    yarp::sig::Vector			effectVec;              // Measurements of the effect of the action (distance Moved - Angle of movement - Rotation)
    yarp::sig::Vector			graspVec;               // Measurements of the effect of the action (distance Moved - Angle of movement - Rotation)
    yarp::sig::Vector			actVec;                 // Measurements of the effect of the action (distance Moved - Angle of movement - Rotation)

    //yarp::sig::Vector			toolDim;    		    // Keeps the dimensions of the detected tool
    //std::string               toolPoseName;

    struct                      Point3D {double x;double y; double z;};

    Point3D                     toolTipCanon, tooltip;            // Coordinates of the tooltip in 3D.
    int                         toolLoadedIdx;      // Index of the tool loaded, in order to decide whether to reload or reGrasp


	
    /* Protected Methods */
    void                        goHomeExe(const bool hands = false);
    bool                        loadToolSim(const int toolI = 0, const int graspOr = 0, const double graspDisp = 0.0);
    bool                        extractFeats();
    void                        transformToolTip(const Point3D ttCanon, const int graspOr, const double graspDisp, Point3D &tooltipTrans);
    bool                        graspTool();
    bool                        getObjLoc(yarp::sig::Vector &coords3D);
    bool                        getObjRot(yarp::sig::Vector &rot3D);
    bool                        slideExe(const double theta = 0.0, const double radius = 0.0);
    bool                        pullExe(const double theta = 0.0, const double radius = 0.0);
    bool                        computeEffect();
    bool                        sendAffData();

public:

	/* RPC commands handling */
	//atomic commands
    bool						attach(yarp::os::RpcServer &source);
	bool						start();
	bool						quit();
    bool                        goHome(bool hands = false);
    bool                        getTool(int toolI = 0, int graspOr = 0, double graspDisp = 0);
    bool                        getToolFeats();
    bool                        slide(double theta, double radius);
    bool                        pull(double theta, double radius);
    bool                        compEff();

    // Experiment functions
    bool                        runToolPose(int toolI, int graspOr = 0, double graspDisp = 0, int numAct = 8);
    bool                        runToolTrial(int toolI, int numAct = 8);
    bool                        runExp(int toolIni, int toolEnd);


    // RF modules overrides
    bool						configure(yarp::os::ResourceFinder &rf);
    bool						interruptModule();
    bool						close();
    bool						updateModule();
    double						getPeriod();
};

#endif

