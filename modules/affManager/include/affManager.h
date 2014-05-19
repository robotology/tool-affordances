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

#ifndef __AFFMOTOR_H__
#define __AFFMOTOR_H__

// Includes
#include <string>

#include <yarp/os/RFModule.h>
#include <yarp/os/RpcServer.h>
#include <yarp/os/RpcClient.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Port.h>
#include <yarp/sig/Image.h>
#include <yarp/sig/Vector.h>

//for iCart
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <yarp/sig/Vector.h>
#include <yarp/dev/CartesianControl.h>
#include <stdio.h>
#include <deque>

//for vision Stuff
#include <cv.h>

#include <affManager_IDLServer.h>

YARP_DECLARE_DEVICES(icubmod)

/**********************************************************/
// DATA Structs
struct affTuple
{
	// XXXX
};
/**********************************************************/
class AffManager : public yarp::os::RFModule, public affManager_IDLServer
{
protected:

	/* module parameters */
	std::string                 name;               //name of the module
	std::string                 hand;               //hand
	std::string                 camera;             //camera
	std::string                 robot;				//robot

    yarp::os::RpcServer         rpcCmd;				//human rpc port (receive commands via rpc)

	yarp::os::RpcClient         rpcMotorAre;        //rpc motor port ARE
	yarp::os::RpcClient         rpcMotorIol;        //rpc motor port IOL
    yarp::os::RpcClient         rpcMotorKarma;      //rpc motor port KARMA    

	yarp::os::RpcClient         rpcBlobPicker;	    //rpc motor port BlobPicker
	yarp::os::RpcClient         rpcBlobSpot;	    //rpc motor port BlobSpotter
	yarp::os::RpcClient         rpcBlobInfo;	    //rpc motor port Blob3DInfo

    //yarp::os::RpcClient         iolStateMachine;    //rpc to iol state machine

	/* class variables */

    //ParticleFilter              particleFilter;     //class to receive positions from the templateTracker module
    //SegmentationPoint           segmentPoint;       //class to request segmentation from activeSegmentation module
    //PointedLocation             pointedLoc;         //port class to receive pointed locations

	yarp::dev::PolyDriver					clientCart;
	yarp::dev::ICartesianControl			*icart;

    yarp::os::BufferedPort<yarp::os::Bottle>        blobSpotter;
	// yarp::os::BufferedPort<yarp::os::Bottle>        particleTracks;
	
	// Flags
	bool						running;
	bool						lookingAtTool;
	bool						lookingAtObject;
	bool						toolInHand;

	yarp::os::Bottle			blobsInReachList;

	cv::Point					target2Dcoords;
	yarp::sig::Vector			target3Dcoords;		// Keeps the target position in 3D
	yarp::sig::Vector			target3Dorient;		// Keeps the target orientation
	
	/* motor functions */
	void                        goHomeExe();
	void						lookAtToolExe();
	void						lookAtRackExe();
	void						lookAtPointExe();
	void						askForToolExe();
	void						reachToolExe();
	void						graspToolExe();
	void						observeToolExe();
	void						observeObjExe();
	

	/* perceptual functions*/
	std::vector<int>			findToolsExe();
	bool						selectToolExe();
	bool						get3Dposition();
	bool						get3Dorient();
	yarp::os::Bottle			getBlobs();

	
public:

	/* RPC commands handling */
	bool						attach(yarp::os::RpcServer &source);
	bool						start();
	bool						quit();
	bool                        goHome();       //
	bool						lookAtTool();
	bool						lookAtRack();
	bool						lookAtPoint();
	bool						askForTool();   //
	bool						reachTool();
	bool						graspTool();    //
	bool						observeTool();  //
	bool						observeObj();   //

	int							findTools();
	bool						selectTool();

	// RF modules overrides
    bool						configure(yarp::os::ResourceFinder &rf);
    bool						interruptModule();
    bool						close();
    bool						updateModule();
    double						getPeriod();
};
#endif

