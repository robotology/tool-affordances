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

#ifndef __AFFMANAGER_H__
#define __AFFMANAGER_H__

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

//for the thrift interface
#include <affManager_IDLServer.h>

YARP_DECLARE_DEVICES(icubmod)

/**********************************************************/
class AffManager : public yarp::os::RFModule, public affManager_IDLServer
{
protected:

	/* module parameters */
	std::string                 name;               //name of the module
	std::string                 hand;               //hand
	std::string                 camera;             //camera
	std::string                 robot;				//robot
    double                      tableHeight;        // height of the table with respect to the robot frame

    yarp::os::RpcServer         rpcCmd;				//human rpc port (receive commands via rpc)

	yarp::os::RpcClient         rpcMotorAre;        //rpc motor port ARE
	// yarp::os::RpcClient         rpcMotorIol;        //rpc motor port IOL
    yarp::os::RpcClient         rpcKarmaMotor;      //rpc motor port KARMA
    yarp::os::RpcClient         rpcKarmaFinder;     //rpc finder port KARMA

    yarp::os::RpcClient         rpcObjFinder;       //rpc connecting to object finder
    yarp::os::RpcClient         rpcToolBlobber;     //rpc connecting to tool Blobber

	//yarp::os::RpcClient         rpcBlobPicker;	//rpc motor port BlobPicker
	//yarp::os::RpcClient         rpcBlobSpot;	    //rpc motor port BlobSpotter

    yarp::os::RpcClient         rpcAffLearn;	    //rpc motor port affLearn
	yarp::os::RpcClient         rpcFeatExt;	        //rpc motor port featExtractor

    yarp::os::BufferedPort<yarp::os::Bottle >  userDataPort;  //port to receive user data.
    yarp::os::Port                             outDataPort;     // port to send out data

    /* class variables */

    //ParticleFilter              particleFilter;     //class to receive positions from the templateTracker module
    //SegmentationPoint           segmentPoint;       //class to request segmentation from activeSegmentation module
    //PointedLocation             pointedLoc;         //port class to receive pointed locations

	yarp::dev::PolyDriver					clientCart;
	yarp::dev::ICartesianControl			*icart;
    int                                     cartCntxt;

    yarp::dev::PolyDriver                   clientGaze;
    yarp::dev::IGazeControl                 *igaze;
    int                                     gazeCntxt;

    yarp::os::BufferedPort<yarp::os::Bottle>        blobSpotter;
	// yarp::os::BufferedPort<yarp::os::Bottle>     particleTracks;
	
	// Flags
	bool						running;
    bool                        actionDone;
    bool						objFound;
    bool						toolFound;
	bool						lookingAtTool;
	bool						lookingAtObject;
	bool						toolInHand;

	//yarp::os::Bottle			blobsInReachList;

	yarp::sig::Vector			target3DcoordsIni;		// Keeps the target position in 3D
    yarp::sig::Vector			target3DcoordsAfter;	// Keeps the target position in 3D
    yarp::sig::Vector			target3Dorient;		    // Keeps the target orientation

    yarp::sig::Vector			toolDim;    		    // Keeps the dimensions of the detected tool
    std::string                 toolPoseName;

    double                      effectAlpha;            // Angle in which the object has been moved by the action
    double                      effectDist;             // Distance that the object has been moved by the action
	
	
	/* motor functions */
	
	//void						reachToolExe();
	//void						lookAtRackExe();
	//void						lookAtPointExe();
    void                        goHomeExe();
    void                        goHomeNoHandsExe();
	void						askForToolExe();	
	bool						graspToolExe();
    void						lookAtToolExe();
        void                        handToCenter();
        void                        lookOverHand();	
        void                        simTool();

    void                        slideActionExe();
    
	/* perceptual functions*/
    void                        attachToolExe();
    void						observeToolExe();
	bool						locateObjExe();
    void						trackObjExe();

	//std::vector<int>			findToolsExe();
	//bool						selectToolExe();
   
    void                        computeEffectExe();
    void                        finishRound();

	
public:

	/* RPC commands handling */
	bool						attach(yarp::os::RpcServer &source);
	bool						start();
	bool						quit();
	bool                        goHome();       //
	bool                        goHomeNoHands();       //
    bool                        getTool();       //
    bool                        setTool(const std::string &tpName);
    bool						askForTool();   //
    bool						graspTool();    //
	bool						lookAtTool();

    bool                        doAction();
    bool                        trainDraw();
    bool                        slideAction();
    bool                        observeAndDo();

	bool						attachTool();
	bool						observeTool();  //
	bool						locateObj();   //
    bool						trackObj();   //
    bool                        computeEffect();
    

	//int							findTools();
	//bool						selectTool();
	

	// RF modules overrides
    bool						configure(yarp::os::ResourceFinder &rf);
    bool						interruptModule();
    bool						close();
    bool						updateModule();
    double						getPeriod();
};

#endif

