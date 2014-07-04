/*
 * Copyright (C) 2014 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
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

#include "toolPointer.h"

using namespace cv;
using namespace std;
using namespace yarp::os;
using namespace yarp::sig;


/**********************************************************/
bool ToolPointerModule::configure(yarp::os::ResourceFinder &rf)
{
    moduleName = rf.check("name", Value("toolPointer"), "module name (string)").asString();

    setName(moduleName.c_str());

    handlerPortName =  "/";
    handlerPortName += getName();
    handlerPortName +=  "/rpc:i";

    if (!rpcInPort.open(handlerPortName.c_str()))    {
        fprintf(stdout, "%s : Unable to open input RPC port %s\n", getName().c_str(), handlerPortName.c_str());
        return false;
    }

    attach(rpcInPort);
    closing = false;

    /* create the thread and pass pointers to the module parameters */
    pointer = new ToolPointer( moduleName, rf );

    /* now start the thread to do the work */
    pointer->open();
    return true ;
}

/**********************************************************/
bool ToolPointerModule::interruptModule()
{
    closing = true;
    rpcInPort.interrupt();
    pointer->interrupt();
    return true;
}

/**********************************************************/
bool ToolPointerModule::close()
{
    rpcInPort.close();
    fprintf(stdout, "starting the shutdown procedure\n");   
    pointer->close();
    fprintf(stdout, "deleting thread\n");
    delete pointer;
    fprintf(stdout, "done deleting thread\n");
    return true;
}

/**********************************************************/
bool ToolPointerModule::updateModule()
{
    return !closing;
}

/**********************************************************/
bool ToolPointerModule::respond(const Bottle &command, Bottle &reply)
{
    /* This method is called when a command string is sent via RPC */
    reply.clear();  // Clear reply bottle

    /* Get command string */
    string receivedCmd = command.get(0).asString().c_str();
    int responseCode;   //Will contain Vocab-encoded response
    if (receivedCmd == "confidence"){
        bool ok = pointer->setConfMin(command.get(1).asDouble());
        if (ok)
            responseCode = Vocab::encode("ack");
        else {
            fprintf(stdout,"Confidence value given out of limits [0-1]. \n");
            responseCode = Vocab::encode("nack");
        }
        reply.addVocab(responseCode);
        return true;

    }else if (receivedCmd == "verbose"){
        bool ok = pointer->setVerbose(command.get(1).asString());
        if (ok)
            responseCode = Vocab::encode("ack");
        else {
            fprintf(stdout,"Verbose can only be set to ON or OFF. \n");
            responseCode = Vocab::encode("nack");
        }
        reply.addVocab(responseCode);
        return true;

    }else if (receivedCmd == "help"){
        reply.addVocab(Vocab::encode("many"));
        responseCode = Vocab::encode("ack");
        reply.addString("Available commands are:");
        reply.addString("confidence (double) - Sets the confidence value [0-1] over which the obtained coordinates are sent.");
        reply.addString("verbose ON/OFF - Sets active the printouts of the program, for debugging or visualization.");
        reply.addString("help - produces this help.");
        reply.addString("quit - closes the module.");
        
        reply.addVocab(responseCode);
        return true;
    } else if (receivedCmd == "quit"){
        responseCode = Vocab::encode("ack");
        reply.addVocab(responseCode);
        closing = true;
        return true;
    }
    
    reply.addString("Invalid command, type [help] for a list of accepted commands.");
    
    return true;
}

/**********************************************************/
double ToolPointerModule::getPeriod()
{
    return 0.3;
}

/**********************************************************/
ToolPointer::~ToolPointer()
{
    
}

/**********************************************************/
ToolPointer::ToolPointer( const string &moduleName, ResourceFinder &rf)
{
    fprintf(stdout,"initialising Variables\n");
    this->moduleName = moduleName;
    this->moduleRF = &rf;
}

/**********************************************************/
bool ToolPointer::open()
{
    this->useCallback();
    fprintf(stdout,"Parsing parameters\n");	

    verbose = moduleRF->check("verbose", Value(false)).asBool();
    confidenceMin = moduleRF->check("confidenceMin", Value(0.8)).asDouble();		
    minBlobSize = moduleRF->check("minBlobSize", Value(400)).asInt();
        
    bool ret=true;
    //create all ports
    fprintf(stdout,"Opening ports\n");

    /* Inputs ports */
    
    coordsInPortName = "/" + moduleName + "/coords:i";
    BufferedPort<Bottle >::open( coordsInPortName.c_str() );    
        
    segImgPortName = "/" + moduleName + "/segIm:i";
    segImgPort.open(segImgPortName);


    /* Output ports */

    rpcGBSPortName = "/" + moduleName + "/gbs:rpc";
    rpcGBS.open(rpcGBSPortName);

    imageOutPortName = "/" + moduleName + "/img:o";
    imageOutPort.open(imageOutPortName);

    imgBinOutPortName = "/" + moduleName + "/imgBin:o";
    imgBinOutPort.open(imgBinOutPortName);
    
    targetOutPortName = "/" + moduleName + "/target:o";
    targetOutPort.open(targetOutPortName);



    return ret;
}

/**********************************************************/
void ToolPointer::close()
{
    fprintf(stdout,"now closing ports...\n");
    
    BufferedPort<Bottle  >::close();

    imageOutPort.close();
    imgBinOutPort.close();
    targetOutPort.close();
    rpcGBS.close();


    fprintf(stdout,"finished closing input and output ports...\n");
}

/**********************************************************/
void ToolPointer::interrupt()
{	
    fprintf(stdout,"cleaning up...\n");
    fprintf(stdout,"attempting to interrupt ports\n");
   
    BufferedPort<Bottle >::interrupt();

    imageOutPort.interrupt();
    imgBinOutPort.interrupt();
    targetOutPort.interrupt();
    rpcGBS.interrupt();
    
    fprintf(stdout,"finished interrupt ports\n");
}


bool ToolPointer::setVerbose(string verb)
{
    if (verb == "ON"){
        verbose = true;
        fprintf(stdout,"Verbose is : %s\n", verb.c_str());
        return true;
    } else if (verb == "OFF"){
        verbose = false;
        fprintf(stdout,"Verbose is : %s\n", verb.c_str());
        return true;
    }    
    return false;
}

bool ToolPointer::setConfMin(float confid)
{
    if ((confid<0) ||(confid>1)) {
        fprintf(stdout,"Please select a valid confidence value [0-1]. \n");
        return false;
    }
    fprintf(stdout,"New minimum confidence Value : %f\n", confid);
    this->confidenceMin = confid;
    return true;
}



/**********************************************************/
void ToolPointer::onRead(Bottle &coords)
{
    yarp::os::Stamp ts;
    
    mutex.wait();    
    if(verbose){   cout << endl << " == RECEIVED CLICK COORDS =="<< endl;}

    /* Read Image */
    ImageOf<PixelRgb> *segImIn = segImgPort.read();  // read an image
    Mat segIm((IplImage*) segImIn->getIplImage());	

    /* Prepare output image for visualization */
    ImageOf<PixelRgb> &imageOut  = imageOutPort.prepare();
    imageOut.resize(segImIn->width(), segImIn->height());		        // Initialize image to 0
    imageOut.zero();
    Mat imOut((IplImage*)imageOut.getIplImage(), false);
    cvtColor(imOut, imOut, CV_RGB2GRAY);								// Create greyscale image for further analysis

    /* Prepare binary image to ouput closest blob */
	ImageOf<PixelMono> &imgBinOut = imgBinOutPort.prepare();		// prepare an output image
	imgBinOut.resize(segImIn->width(), segImIn->height());		// Initialize binary image to zero
    imgBinOut.zero();
	Mat imgBin((IplImage*)imgBinOut.getIplImage(),false);
    
    /* Prepare output target port */
    Bottle &target = targetOutPort.prepare();
    target.clear();

    Point coordsPx;
    coordsPx.x = coords.get(0).asInt();
    coordsPx.y = coords.get(1).asInt();

        /* Send Target coordinates*/
        if(verbose)    {cout << "coords received are : " << coords.get(0).asInt() << ", "<< coords.get(1).asInt() << endl;}

        cout << "Getting pixels from GBS" << endl;
        // Get graphBased segmenenation within the close blob
        Bottle cmdGBS,replyGBS;
        cmdGBS.clear();    replyGBS.clear();
        cmdGBS.addString("get_component_around");
        cmdGBS.addInt(coordsPx.x);
        cmdGBS.addInt(coordsPx.y);
        rpcGBS.write(cmdGBS, replyGBS);
        cout << "Received " << replyGBS.size() << " bottles in the reply" << endl;         
        if (replyGBS.size() > 0 ){
            Bottle *segPixels = replyGBS.get(0).asList();
            cout << "Received " << segPixels->size() << " pixel locations. Creating mask from received pixels" << endl;
            // Go through all the returned pixels and make a mask out of them. 
            Mat segMask(segIm.size(), CV_8UC1, Scalar(0));
            for( int sp = 0; sp < segPixels->size(); sp++ ){
                Bottle *pointB = segPixels->get(sp).asList();
                Point segPoint(pointB->get(0).asInt(), pointB->get(1).asInt());
                circle(segMask, segPoint, 0, Scalar(1));
                circle(imgBin, segPoint, 0, Scalar(255));
            } 
        
            // Multiply this mask with the one obtained from disparity
            segIm.copyTo(imOut,segMask);
            //multiply(segIm, segMask, imOut);
        }
        // Put the result as the desired blob

    mutex.post();

    /* write info on output ports */
    imageOutPort.setEnvelope(ts);
    imageOutPort.write();
    imgBinOutPort.write();
    targetOutPort.write();

}
//empty line to make gcc happy

