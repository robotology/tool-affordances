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
    std::string                     name;               //name of the module
    std::string                     hand;               //hand
    std::string                     camera;             //camera
    std::string                     robot;				//robot
    double                          tableHeight;        // height of the table with respect to the robot frame
    std::vector<yarp::os::Bottle>   models;             // Vector to contain all models considered in the experiment.

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
	bool 						tipOnView;
    */

    //yarp::sig::Vector			toolTipPix;             // Keeps the tool position in 2D for looking at it
    yarp::sig::Vector			target3DcoordsIni;		// Keeps the target position in 3D before the action
    yarp::sig::Vector			target3DcoordsAfter;	// Keeps the target position in 3D after the action
    yarp::sig::Vector			target3DrotIni;         // Keeps the target rotation in 3D before the action
    yarp::sig::Vector			target3DrotAfter;		// Keeps the target rotation in 3D after the action

    yarp::sig::Vector			effectVec;              // Measurements of the effect of the action (distance Moved - Angle of movement - Rotation)
    yarp::sig::Vector			graspVec;               // Measurements of the grasp parameters (toolI - grasp Orientation - grasp Displacement)
    yarp::sig::Vector			actVec;                 // Measurements of the action parameters (action angle, distance of drag)

    //yarp::sig::Vector			toolDim;    		    // Keeps the dimensions of the detected tool
    //std::string               toolPoseName;

    struct                      Point3D {double x;double y; double z;};

    Point3D                     toolTipCanon, tooltip;            // Coordinates of the tooltip in 3D.
    int                         toolLoadedIdx;      // Index of the tool loaded, in order to decide whether to reload or reGrasp
    bool						trackingObj;

	
    /* Protected Methods */    
    // Tool loading and feature extraction
    bool                        loadToolSim(const int toolI = 3, const double graspOr = 0.0, const double graspDisp = 0.0, const double graspTilt = 45.0);
    bool                        loadToolReal(const int toolI = 3, const double graspOr = 0.0, const double graspDisp = 0.0, const double graspTilt = 45.0);
    bool                        regraspExe(const double deg = 0.0, const double disp = 0.0, const double tilt = 45.0, const double Z = 0.0);
    bool                        extractFeats();
    void                        transformToolTip(const Point3D ttCanon, Point3D &tooltipTrans, const double graspOr = 0.0, const double graspDisp = 0.0, const double graspTilt = 45.0 );

    // Object Localization and effect computation
    bool                        trackObjExe();
    bool                        getObjLoc(yarp::sig::Vector &coords3D);
    bool                        getObjRot(yarp::sig::Vector &rot3D);
    bool                        computeEffect();
    bool                        sendAffData();

    // Action
    void                        goHomeExe(const bool hands = false);
    void                        toolToCenter();
    bool                        slideExe(const double theta = 0.0, const double radius = 0.1);
    bool                        dragExe(const double theta = 0.0, const double radius = 0.1, const double tilt = 0.0);


public:

	/* RPC commands handling */
	//atomic commands
    bool						attach(yarp::os::RpcServer &source);
	bool						start();
	bool						quit();
	bool						settableheight(double th = -0.1);
    bool                        goHome(bool hands = false);
    bool                        centerTool();
    bool                        getTool(int toolI = 0, double deg = 0.0, double disp = 0.0, double tilt = 45.0);
    bool                        regrasp(double deg = 0.0, double disp = 0.0, double tilt = 45.0, double Z = 0.0);
    bool                        getToolFeats();
    bool                        slide(double theta, double radius);
    bool                        drag(double theta, double radius, double tilt);
    bool                        compEff();
    bool                        trackObj();

    // Experiment functions
    bool                        runRandPoses(int numPoses = 50,int numAct = 8);
    bool                        runToolPose(int toolI, double graspOr = 0.0, double graspDisp = 0.0, double graspTilt = 45.0, int numAct = 8);
    bool                        runToolOr(int toolI, double graspOr = 0.0, int numAct = 8);
    bool                        runToolTrial(int toolI, int numAct = 8);
    bool                        runExp(int toolIni, int toolEnd);
    bool                        selectAction(int goal = 0);

    // RF modules overrides
    bool						configure(yarp::os::ResourceFinder &rf);
    bool						interruptModule();
    bool						close();
    bool						updateModule();
    double						getPeriod();
};

#endif

