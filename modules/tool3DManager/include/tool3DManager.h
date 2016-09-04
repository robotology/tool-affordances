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
#include <algorithm>
#include <iterator>
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
#include <yarp/os/LogStream.h>

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
    std::vector<std::string>   models;             // Vector to contain all models considered in the experiment.

    yarp::os::RpcServer         rpcCmd;                 //human rpc port (receive commands via rpc)

    yarp::os::RpcClient         rpcSimToolLoader;       //rpc for the simulator tool loader
    yarp::os::RpcClient         rpcSimulator;           //rpc for the simulator tool loader
    yarp::os::RpcClient         rpcMotorAre;            //rpc motor port ARE
    yarp::os::RpcClient         rpcAffMotor;            //rpc port to affMotor Module


    yarp::os::RpcClient         rpcFeatExt;             //rpc connection to toolFeatExt to extract features from cloud
    yarp::os::RpcClient         rpc3Dexp;               //rpc connection to toolFeatExt to extract features from cloud

    yarp::os::RpcClient         rpcObjFinder;           //rpc connecting to object finder


    yarp::os::Port                             affDataPort;   // port to send out data of computed affordance data: tool, grasp, action, effect
    yarp::os::BufferedPort<yarp::os::Bottle >  matlabPort;    // port to receive predictions from MATLAB
    yarp::os::BufferedPort<yarp::os::Bottle >  trackPort;     // port to receive

    /* class variables */
	// Flags
	bool						running;



    yarp::sig::Vector			target3DcoordsIni;		// Keeps the target position in 3D before the action
    yarp::sig::Vector			target3DcoordsAfter;	// Keeps the target position in 3D after the action
    yarp::sig::Vector			target3DrotIni;         // Keeps the target rotation in 3D before the action
    yarp::sig::Vector			target3DrotAfter;		// Keeps the target rotation in 3D after the action

    yarp::os::Bottle			aff_out_data;           // Data concerning measured affordance data (toolpose - action - effect)

    yarp::sig::Vector			effectVec;              // Measurements of the effect of the action (distance Moved - Angle of movement - Rotation)
    yarp::sig::Vector			graspVec;               // Measurements of the grasp parameters (toolI - grasp Orientation - grasp Displacement)
    yarp::sig::Vector			actVec;                 // Measurements of the action parameters (action angle, distance of drag)

    struct                      Point3D {double x;double y; double z;};

    Point3D                     toolTipCanon, tooltip;            // Coordinates of the tooltip in 3D.
    int                         toolLoadedIdx;      // Index of the tool loaded, in order to decide whether to reload or reGrasp
    bool						trackingObj;
    std::string                 toolname;
    bool                        seg2D;              // if false, seg is from 3D.

	
    /****  Protected Methods ******/
    // Tool loading and feature extraction
    bool                        graspToolExe();
    bool                        lookToolExe();
    bool                        load3Dmodel(const std::string& tool);
    bool                        loadToolSim(const std::string& tool, const double graspOr = 0.0, const double graspDisp = 0.0, const double graspTilt = 45.0);
    bool                        loadToolPose(const std::string& tool, const double graspOr = 0.0, const double graspDisp = 0.0, const double graspTilt = 45.0,  const double graspShift = 0.0);
    bool                        getToolExe(const std::string& tool);

    bool                        findPoseExe(const std::string& tool, Point3D &ttip);
    bool                        regraspExe(Point3D &newTooltip, const double deg = 0.0, const double disp = 0.0, const double tilt = 45.0, const double Z = 0.0);

    bool                        exploreTool(Point3D &ttip);
    bool                        findTipFromParam( Point3D &ttip, const double graspOr = 0.0, const double graspDisp = 0.0, const double graspTilt = 45.0, const double graspShift = 0.0);
    bool                        addToolTip(const Point3D ttip);
    bool                        extractFeats();

    // Object Localization and effect computation
    double                      findTableHeight(bool calib = true);
    bool                        trackObjExe();
    bool                        getObjLoc(yarp::sig::Vector &coords3D);
    bool                        getObjRot(yarp::sig::Vector &rot3D);
    bool                        computeEffect();
    bool                        sendAffData();
    bool                        resetCube();

    // Action
    void                        goHomeExe(const bool hands = false);
    bool                        slideExe(const double theta = 0.0, const double radius = 0.1);
    bool                        dragExe(double theta = 0.0, double radius = 0.1, double tilt = 0.0);
    bool                        drag3DExe(double x, double y, double z,  double theta = 0.0, double radius = 0.1, double tilt = -15.0, bool useTool = true);


public:

	/* RPC commands handling */
    // module control commands
    bool						attach(yarp::os::RpcServer &source);
	bool						start();
	bool						quit();
    bool                        setSeg(bool);

    // tool load and information
    bool                        setToolName(const std::string &tool);
    bool                        getToolByPose(const std::string& tool, double deg = 0.0, double disp = 0.0, double tilt = 45.0, double shift = 0.0);
    bool                        getTool(const std::string &tool);
    bool                        graspTool();
    bool                        lookTool();
    bool                        explore();
    bool                        regrasp(double deg = 0.0, double disp = 0.0, double tilt = 45.0, double shift = 0.0);
    bool                        findPose();
    bool                        getToolFeats();

    // Get object information
    yarp::sig::Vector           objLoc();
    yarp::sig::Vector           objRot();

    // actions
    bool                        goHome(bool hands = false);
    bool						findTable(bool calib = true);
    bool                        slide(double theta, double radius);
    bool                        drag3D(double x, double y, double z, double theta = 0.0, double radius = 0.0, double tilt = -15.0, bool useTool = true);
    bool                        drag(double theta, double radius, double tilt);
    bool                        trackObj();
    bool                        compEff();
    bool                        resetObj();

    // Experiment functions
    bool                        runRandPoses(int numPoses = 50,int numAct = 8);
    bool                        runToolPose(const std::string& tool, double graspOr = 0.0, double graspDisp = 0.0, double graspTilt = 45.0, int numAct = 8);
    bool                        runToolOr(const std::string& tool, double graspOr = 0.0, int numAct = 8);
    bool                        runToolTrial(const std::string& tool, int numAct = 8);
    bool                        runExp(int toolIni, int toolEnd);
    bool                        selectAction(int goal = 1);
    bool                        predExp(int goal = 1);

    // RF modules overrides
    bool						configure(yarp::os::ResourceFinder &rf);
    bool						interruptModule();
    bool						close();
    bool						updateModule();
    double						getPeriod();
};

#endif

