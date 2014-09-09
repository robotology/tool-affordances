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

#include <yarp/os/Time.h>
#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Image.h>
#include <yarp/os/Vocab.h>

#include <cv.h>

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
    BufferedPort<ImageOf<PixelRgb> >    imOutPort;
    BufferedPort<ImageOf<PixelRgb> >    tempOutPort;
    BufferedPort<Bottle>                coordsInPort;
    BufferedPort<Bottle>                trackInPort;
    Port                                coordsOutPort;
    Port                                rpcPort;

    std::string                         name;               //name of the module
    std::string                         robot;
    std::string                         camera;
    double                              tableHeight;

    bool                                tracking;
    Vector                              coords2D;
    Vector                              coords3D;
    cv::Rect                            BBox;

    yarp::dev::PolyDriver                   clientGaze;
    yarp::dev::IGazeControl                 *igaze;
   
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
        tableHeight = rf.check("tableHeight", Value(-0.10)).asDouble();      // Height of the table in the robots coord frame

        // Open ports
        printf("Opening ports after prefix  \n" );
        bool ret= true;    
        ret = rpcPort.open(("/"+name+"/rpc:i").c_str()); 					    //rpc client to interact with the objectFinder
        ret = ret && imInPort.open(("/"+name+"/img:i").c_str());                // port to receive images
        ret = ret && coordsInPort.open(("/"+name+"/coords:i").c_str());         // port to receive yarpview coordinates
        ret = ret && trackInPort.open(("/"+name+"/track:i").c_str());           // port to receive tracker coordinates
        ret = ret && coordsOutPort.open(("/"+name+"/coords:o").c_str());        // port to send object coordinates
        ret = ret && imOutPort.open(("/"+name+"/imgOut:o").c_str());              // port to send croped image for template
        ret = ret && tempOutPort.open(("/"+name+"/crop:o").c_str());              // port to send croped image for template
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
        if (receivedCmd == "getPointClick")
        {
            if (! getPointClick())
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
                bCoords.addInt(coords2D(0));
                bCoords.addInt(coords2D(1));
                //bCoords.addDouble(coords3D(2));
            }
            return true;

        } else if (receivedCmd == "getPointTrack")
        {   
            if (!getPointTrack(tableHeight))
            {
                //Encode response
                responseCode = Vocab::encode("nack");
                reply.addVocab(responseCode);
            }
            else
            {
                //Encode response              
                printf("Sending out 3D point!!\n");
                responseCode = Vocab::encode("ack");
                reply.addVocab(responseCode);
                Bottle& bCoords = reply.addList();
                bCoords.addDouble(coords3D(0));
                bCoords.addDouble(coords3D(1));
                bCoords.addDouble(coords3D(2));
                printf("Coords Bottle %s \n", bCoords.toString().c_str());
                printf("sent reply %s \n", reply.toString().c_str());
            }
            return true;

        } else if (receivedCmd == "getBox")
        {   
            if (!getBox())
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
                Bottle& bBox = reply.addList();
                bBox.addInt(BBox.tl().x);
                bBox.addInt(BBox.tl().y);
                bBox.addInt(BBox.br().x);
                bBox.addInt(BBox.br().y);                
            }
        }
        else if (receivedCmd == "help")
        {
            reply.addVocab(Vocab::encode("many"));
            reply.addString("Available commands are:");
            reply.addString("help");
            reply.addString("quit");           
            reply.addString("getPointClick - reads a click and returns the 2D coordinates");
            reply.addString("getPointTrack - Retrieves 2D coords and returns the 3D coordinates of the object tracked based on the table Height");
            reply.addString("getBox - Crops the image based on user input and creates a template for the tracker with it");
            return true;
        }
        else if (receivedCmd == "quit")
        {
            reply.addString("Quitting.");
            return false; 
        } else{        
            reply.addString("Invalid command, type [help] for a list of accepted commands.");
        }
        return true;
    }

    /*************************************************************************/
    bool getPointClick()
    {
        printf("Click on the object in the image!!\n");
        //Bottle &out  = coordsOutPort.prepare();
        Bottle *obj2Dcoord = coordsInPort.read(true);
        printf("Point read!!\n");
        coords2D.resize(2,0.0);        
        coords2D[0] =  obj2Dcoord->get(0).asInt();
        coords2D[1] =  obj2Dcoord->get(1).asInt();
        printf("Point in 2D read: %.2f, %.2f!!\n", coords2D(0), coords2D(1));
       
        return true;    // get the projection
    }

    bool getPointTrack(double tableHeight)
    {
        // Put the input image at the moment of computing out
        printf("Propagating image!!\n");
        ImageOf<PixelRgb> *imgIn = imInPort.read();  // read an image
		if(imgIn == NULL)		{
            printf("No object tracked \n");
			return false;
		}	
        ImageOf<PixelRgb> &imOut  = imOutPort.prepare();
        imOut = *imgIn;
        imOutPort.write();
       
        // Retrieves and relays the 2D coordinates of the object tracked by the tracker
        //Bottle &out  = coordsOutPort.prepare();
        printf("Getting 3D coords of tracked object!!\n");
        Bottle *trackCoords = trackInPort.read(true);
        coords2D.resize(2,0);        
        coords2D[0] =  trackCoords->get(0).asInt();
        coords2D[1] =  trackCoords->get(1).asInt();
        printf("Point in 2D read: %.2f, %.2f!!\n", coords2D(0), coords2D(1));

        Vector table(4);  // specify the plane in the root reference frame as ax+by+cz+d=0; z=-tableHeight in this case
        table[0] = 0.0; table[1] = 0.0; table[2] = 1.0;  
        table[3] = -tableHeight;    // d -> so the equation of the table plane is z=-h 

        int camSel;
        if (camera != "left") {
               camSel = 1;}
        else { camSel = 0;}    

        if(igaze->get3DPointOnPlane(camSel,coords2D, table, coords3D)){
            printf("Point in 3D computed: %.2f, %.2f, %.2f!!\n", coords3D(0), coords3D(1), coords3D(2));
            return true;
	    } else  {
            printf("3D point could not be computed\n");
            return false;
        }
    }

    bool getBox()
    {
        // Crops the image based on user input and creates a template for the tracker with it.
        printf("Reading image!!\n");
        ImageOf<PixelRgb> *imgIn = imInPort.read();  // read an image
        cv::Mat img((IplImage*) imgIn->getIplImage());	   
     
        printf("Click first top left and then bottom right from the object !!\n");
        bool boxOK = false;
        //Bottle &out  = coordsOutPort.prepare();
        cv::Point tl, br;

        while (!boxOK){
            printf("Click on top left!\n");
            Bottle *point1 = coordsInPort.read(true);
            tl.x =  point1->get(0).asDouble();
            tl.y =  point1->get(1).asDouble();
            printf("Point read at %d, %d!!\n", tl.x, tl.y);

            printf("Click on bottom right!\n");
            Bottle *point2 = coordsInPort.read(true);            
            br.x =  point2->get(0).asDouble();
            br.y =  point2->get(1).asDouble();
            printf("Point read at %d, %d!!\n", br.x, br.y);

            BBox = cv::Rect(tl,br);            
            if (BBox.area() > 20) {
                printf("valid coordinates, cropping image!\n");
                boxOK = true;}
            else {printf("Coordinates not valid, click again!\n");}
        }

        printf("Prep out mat !!\n");
        ImageOf<PixelRgb> &templateOut  = tempOutPort.prepare();
        templateOut.resize(BBox.width, BBox.height);
        cv::Mat tempOut((IplImage*)templateOut.getIplImage(),false);
        img(BBox).copyTo(tempOut);
        //cv::GaussianBlur(img(BBox), imOut, cv::Size(1,1), 1, 1);

        double t0 = Time::now();
        while(Time::now()-t0 < 1) {  //send the template for one second
            printf("Writing Template!\n");
            tempOutPort.write();
            Time::delay(0.1);
        }

        tracking = true;
        return true;
    }

    /************************************************************************/
    bool close()
    {        
        // Close ports
        rpcPort.close();

        imInPort.close();
        imOutPort.close();
        tempOutPort.close();
        coordsInPort.close();
        trackInPort.close();
        coordsOutPort.close();

        return true;
    }

    /************************************************************************/
    bool interruptModule()
    {
        rpcPort.interrupt();
        // Interrupt any blocking reads on the input port
        imInPort.interrupt();
        imOutPort.interrupt();
        tempOutPort.close();
        coordsInPort.interrupt();
        trackInPort.interrupt();
        coordsOutPort.interrupt();

        return true;
    }

    /************************************************************************/
    double getPeriod()
    {
        return 0.1;
    }

    
    /************************************************************************/
    bool updateModule()
    {
	    if (!running)
            return false;

        if (tracking)
        {
           Bottle *coords = coordsInPort.read(true);
           Bottle coordsSend;
           coordsSend.addInt(coords->get(0).asInt());
           coordsSend.addInt(coords->get(1).asInt());
           coordsOutPort.write(coordsSend);
        }

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
