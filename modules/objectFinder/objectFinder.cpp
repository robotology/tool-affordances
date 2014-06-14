/* 
 * Copyright (C) 2014 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Tanis Mar
 * email: tanis.mar@iit.it
 * website: www.robotcub.org
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
/*
\author Tanis Mar
*/ 

//#include <iostream>
//#include <string>

#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Image.h>
#include <yarp/os/Vocab.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/GazeControl.h>
YARP_DECLARE_DEVICES(icubmod)

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

/************************************************************************/
class ObjectFinder: public RFModule
{
protected:
    BufferedPort<ImageOf<PixelRgb> >    imInPort;
    BufferedPort<Bottle>                coordsInPort;
    BufferedPort<Bottle>                coordsOutPort;
    Port                                rpcPort;
    std::string                         name;               //name of the module
    std::string                         robot;
    std::string                         camera;
    int                                 camSel;

    Vector                              coords2D;
    Vector                              coords3D;
    double                              tableHeight;

    PolyDriver                          clientGaze;
    IGazeControl                        *igaze;
    int                                 gazeCntxt;

    bool running;

public:
    /************************************************************************/
    bool configure(ResourceFinder &rf)
    {
      	printf("Configuring...\n");        
        name = rf.check("name", Value("objectFinder"), "Getting module name").asString();
        setName(name.c_str());
        camera = rf.check("camera", Value("left"), "Selecting the camera").asString().c_str();
        robot = rf.check("robot", Value("icub"), "Choosing a robot").asString().c_str();
        tableHeight = rf.check("tableHeight", Value(-0.1)).asDouble();      // Height of the table in the robots coord frame
        if (camera != "left") {
            camSel = 1;
        }else {
            camSel = 0;
        }


        // Open ports
        printf("Opening ports after prefix  \n" );
        bool ret= true;    
        ret = rpcPort.open(("/"+name+"/rpc:i").c_str()); 					    //rpc client to interact with the objectFinder
        ret = ret && imInPort.open(("/"+name+"/img:i").c_str());                // port to receive images
        ret = ret && coordsInPort.open(("/"+name+"/coords:i").c_str());         // port to receive coordinates
        ret = ret && coordsOutPort.open(("/"+name+"/coords3D:o").c_str());      // port to receive coordinates
	    if (!ret){
		    printf("Problems opening ports\n");
		    return false;
	    }
        printf("Ports opened\n");

        // Attach rpcPort to the respond() method
        attach(rpcPort);

        // Gaze Controller Interface
        Property optGaze("(device gazecontrollerclient)");
        optGaze.put("remote","/iKinGazeCtrl");
        optGaze.put("local","/"+name+"/gaze_client");

        if (!clientGaze.open(optGaze))
            return false;
        clientGaze.view(igaze);

        igaze->storeContext(&gazeCntxt);        // store the original the context

        igaze->setNeckTrajTime(0.8);
        igaze->setEyesTrajTime(0.4);
        igaze->setTrackingMode(true);
		
		running = true;

        return true;
    }

    bool respond(const Bottle &      command,
                 Bottle &      reply)
    {
        // This method is called when a command string is sent via RPC

        // Get command string
        string receivedCmd = command.get(0).asString().c_str();
       
        bool f;
        int responseCode;   //Will contain Vocab-encoded response

        reply.clear();  // Clear reply bottle        
        if (receivedCmd == "getPoint")
        {
            printf("Respond get\n");
            if (! getPoint())
            {
                //Encode response
                responseCode = Vocab::encode("nack");
                reply.addVocab(responseCode);
            }
            else
            {
                //Encode response                
                responseCode = Vocab::encode("ack");
                reply.addVocab(responseCode);
                Bottle& bCoords = reply.addList();
                bCoords.addDouble(coords3D(0));
                bCoords.addDouble(coords3D(1));
                bCoords.addDouble(coords3D(2));
            }
            return true;
        }        
        else if (receivedCmd == "help")
        {
            reply.addVocab(Vocab::encode("many"));
            reply.addString("Available commands are:");
            reply.addString("help");
            reply.addString("quit");
            reply.addString("getPoint");
            return true;
        }
        else if (receivedCmd == "quit")
        {
            reply.addString("Quitting.");
            return false; //note also this
        }
        
        reply.addString("Invalid command, type [help] for a list of accepted commands.");
        return true;
    }

    /*************************************************************************/
    bool getPoint()
    {
        printf("Click on the object in the image!!\n");
        //Bottle &out  = coordsOutPort.prepare();
        Bottle *obj2Dcoord = coordsInPort.read(true);
        printf("Point read!!\n");
        coords2D.resize(2,0.0);        
        coords2D[0] =  obj2Dcoord->get(0).asDouble();
        coords2D[1] =  obj2Dcoord->get(1).asDouble();
        printf("Point in 2D read: %.2f, %.2f!!\n", coords2D(0), coords2D(0));
       
        Vector table(4);  // specify the plane in the root reference frame as ax+by+cz+d=0; z=-tableHeight in this case
        table[0]=0.0;     // a
        table[1]=0.0;     // b
        table[2]=1.0;     // c
        table[3]=-tableHeight;    // d -> so th eequation of the table plane is z=-h
        
        if(igaze->get3DPointOnPlane(camSel,coords2D, table, coords3D)){
            printf("The point selected is %.2f %.2f %.2f\n", coords3D[0], coords3D[1],coords3D[2]);
            igaze->lookAtFixationPoint(coords3D);                 // move the gaze to the desired fixation point
            igaze->waitMotionDone();                              // wait until the operation is done
            return true;    // get the projection
        }else
            return false;
    }
    

    /************************************************************************/
    bool close()
    {        
        // Close ports
        imInPort.close();
        rpcPort.close();
        coordsInPort.close();
        coordsOutPort.close();

        clientGaze.close();

        return true;
    }

    /************************************************************************/
    double getPeriod()
    {
        return 0.1;
    }


    // we don't need a thread since the actions library already
    // incapsulates one inside dealing with all the tight time constraints
    /************************************************************************/
    bool updateModule()
    {
	    if (!running)
            return false;
	    return true;
    }

    /************************************************************************/
    bool interruptModule()
    {
        // Interrupt any blocking reads on the input port
        imInPort.interrupt();
        coordsInPort.interrupt();
        coordsOutPort.interrupt();

        return true;
    }
};


/************************************************************************/
int main(int argc, char *argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
        return -1;

    YARP_REGISTER_DEVICES(icubmod)

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefault("name","objectFinder");
	rf.setDefault("camera","left");
	rf.setDefault("robot","icub");
    rf.setDefaultContext("AffordancesProject");
    rf.setDefaultConfigFile("objectFinder.ini");

    rf.configure(argc,argv);

    ObjectFinder objectFinder;
    return objectFinder.runModule(rf);
}
