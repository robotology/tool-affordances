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

#include "toolBlobber.h"

using namespace cv;
using namespace std;
using namespace yarp::os;
using namespace yarp::sig;


/**********************************************************/
bool ToolBlobberModule::configure(yarp::os::ResourceFinder &rf)
{
    moduleName = rf.check("name", Value("toolBlobber"), "module name (string)").asString();

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
    blobber = new ToolBlobber( moduleName, rf );

    /* now start the thread to do the work */
    blobber->open();
    return true ;
}

/**********************************************************/
bool ToolBlobberModule::interruptModule()
{
    closing = true;
    rpcInPort.interrupt();
    blobber->interrupt();
    return true;
}

/**********************************************************/
bool ToolBlobberModule::close()
{
    rpcInPort.close();
    fprintf(stdout, "starting the shutdown procedure\n");   
    blobber->close();
    fprintf(stdout, "deleting thread\n");
    delete blobber;
    fprintf(stdout, "done deleting thread\n");
    return true;
}

/**********************************************************/
bool ToolBlobberModule::updateModule()
{
    return !closing;
}

/**********************************************************/
bool ToolBlobberModule::respond(const Bottle &command, Bottle &reply)
{
    /* This method is called when a command string is sent via RPC */
    reply.clear();  // Clear reply bottle

    /* Get command string */
    string receivedCmd = command.get(0).asString().c_str();
    int responseCode;   //Will contain Vocab-encoded response

    if (receivedCmd == "seed"){
        Bottle coords;
        coords.addInt(command.get(1).asInt());
        coords.addInt(command.get(2).asInt());
        blobber->onRead(coords);
        responseCode = Vocab::encode("ack");
        reply.addVocab(responseCode);
        return true;

    }else if (receivedCmd == "range"){
        bool ok = blobber->setRange(command.get(1).asDouble());
        if (ok)
            responseCode = Vocab::encode("ack");
        else {
            fprintf(stdout,"Graspable Range not set. \n");
            responseCode = Vocab::encode("nack");
        }
        reply.addVocab(responseCode);
        return true;

    }else if (receivedCmd == "thresh"){
        bool ok = blobber->setThresh(command.get(1).asInt(), command.get(2).asInt());
        if (ok)
            responseCode = Vocab::encode("ack");
        else {
            fprintf(stdout,"Threshold for disparity considered set. \n");
            responseCode = Vocab::encode("nack");
        }
        reply.addVocab(responseCode);
        return true;


    }else if (receivedCmd == "fixedRange"){
        bool ok = blobber->setFixedRange(command.get(1).asBool());
        if (ok)
            responseCode = Vocab::encode("ack");
        else {
            fprintf(stdout,"Couldnt change the fixedness of the range. \n");
            responseCode = Vocab::encode("nack");
        }
        reply.addVocab(responseCode);
        return true;

    }else if (receivedCmd == "confidence"){
        bool ok = blobber->setConfMin(command.get(1).asDouble());
        if (ok)
            responseCode = Vocab::encode("ack");
        else {
            fprintf(stdout,"Confidence value given out of limits [0-1]. \n");
            responseCode = Vocab::encode("nack");
        }
        reply.addVocab(responseCode);
        return true;

    }else if (receivedCmd == "verbose"){
        bool ok = blobber->setVerbose(command.get(1).asString());
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
        reply.addString("seed  (int) (int) - calls the toolBlobber with the seed coordinates given.");
        reply.addString("range (double) - modifies the distance within which blobs are considered reachable.");
        reply.addString("thresh (int) (int) - to sets the lower limit and higher of disparity in terms of luminosity (0-255) that is considered. In other words, only objects with luminosity between low and high will be considered.");
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
double ToolBlobberModule::getPeriod()
{
    return 0.5;
}

/**********************************************************/
ToolBlobber::~ToolBlobber()
{
    
}

/**********************************************************/
ToolBlobber::ToolBlobber( const string &moduleName, ResourceFinder &rf)
{
    fprintf(stdout,"initialising Variables\n");
    this->moduleName = moduleName;
    this->moduleRF = &rf;
}

/**********************************************************/
bool ToolBlobber::open()
{
    this->useCallback();
    fprintf(stdout,"Parsing parameters\n");	

    verbose = moduleRF->check("verbose", Value(false)).asBool();
    range = moduleRF->check("range", Value(0.5)).asDouble();
    backgroundThresh = moduleRF->check("backgroundThresh", Value(60)).asInt();		// threshold of intensity if the disparity image, under which info is ignored.
    frontThresh = moduleRF->check("frontThresh", Value(190)).asInt();		// threshold of intensity if the disparity image, above which info is ignored.
    confidenceMin = moduleRF->check("confidenceMin", Value(0.8)).asDouble();		
    cannyThresh = moduleRF->check("cannyThresh", Value(20)).asDouble();
    minBlobSize = moduleRF->check("minBlobSize", Value(400)).asInt();
    gaussSize = moduleRF->check("gaussSize", Value(5)).asInt();
    dispThreshRatioLow = moduleRF->check("dispThreshRatioLow", Value(10)).asInt();    
    dispThreshRatioHigh = moduleRF->check("dispThreshRatioHigh", Value(20)).asInt();
    fixedRange = moduleRF->check("fixedRange", Value(true)).asBool();
        
    bool ret=true;
    //create all ports
    fprintf(stdout,"Opening ports\n");

    /* Inputs ports */

    seedInPortName = "/" + moduleName + "/seed:i";
    BufferedPort<Bottle >::open( seedInPortName.c_str() );    
    //seedInPortPort.open(seedInPortName);

    dispInPortName = "/" + moduleName + "/disp:i";
    dispInPort.open(dispInPortName);


    imInLeftPortName = "/" + moduleName + "/imLeft:i";
    imInLeftPort.open(imInLeftPortName);
    
    /* Output ports */

    rpcGBSPortName = "/" + moduleName + "/gbs:rpc";
    rpcGBS.open(rpcGBSPortName);

    imageOutPortName = "/" + moduleName + "/img:o";
    imageOutPort.open(imageOutPortName);

    imgBinOutPortName = "/" + moduleName + "/imgBin:o";
    imgBinOutPort.open(imgBinOutPortName);
    
    targetOutPortName = "/" + moduleName + "/target:o";
    targetOutPort.open(targetOutPortName);


    // Open disparity Thread
    string configFileDisparity = moduleRF->check("ConfigDisparity",Value("icubEyes.ini")).asString().c_str();
    string cameraContext = moduleRF->check("CameraContext",Value("cameraCalibration")).asString().c_str();
    string name = moduleRF->check("name",Value("blobCoordsExtractor")).asString().c_str();

    ResourceFinder cameraFinder;
    cameraFinder.setDefaultContext(cameraContext.c_str());
    cameraFinder.setDefaultConfigFile(configFileDisparity.c_str());
    cameraFinder.setVerbose();
    cameraFinder.configure(0,NULL);

    return ret;
}

/**********************************************************/
void ToolBlobber::close()
{
    fprintf(stdout,"now closing ports...\n");
    BufferedPort<Bottle >::close();

    dispInPort.close();
    //seedInPort.close();
    imInLeftPort.close();

    imageOutPort.close();
    imgBinOutPort.close();
    targetOutPort.close();
    rpcGBS.close();


    fprintf(stdout,"finished closing input and output ports...\n");
}

/**********************************************************/
void ToolBlobber::interrupt()
{	
    fprintf(stdout,"cleaning up...\n");
    fprintf(stdout,"attempting to interrupt ports\n");
    
    BufferedPort<Bottle >::interrupt();

    dispInPort.interrupt();
    //seedInPort.interrupt();
    imInLeftPort.interrupt();

    imageOutPort.interrupt();
    imgBinOutPort.interrupt();
    targetOutPort.interrupt();
    rpcGBS.interrupt();
    
    fprintf(stdout,"finished interrupt ports\n");
}


bool ToolBlobber::setRange(double r)
{
    if (r<0.1){
        fprintf(stdout,"Range is too small. \n");
        return false;
    }
    fprintf(stdout,"New Range is : %.2f\n", r);
    this->range = r;
    return true;
}

bool ToolBlobber::setThresh(int low, int high)
{
    if ((low<0) ||(low>255)||(high<0) ||(high>255)) {
        fprintf(stdout,"Please select valid luminance values (0-255). \n");
        return false;
    }
    fprintf(stdout,"New Threshold is : %i, %i\n", low, high);
    this->backgroundThresh = low;
    this->frontThresh = high;
    return true;
}

bool ToolBlobber::setVerbose(string verb)
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

bool ToolBlobber::setFixedRange(bool fixRangeFlag)
{
    if (fixRangeFlag == true){
        this->  fixedRange = true;
        fprintf(stdout,"Fixed range is ON \n");
        return true;
    } else if (fixRangeFlag ==false){
        this->fixedRange = false;
        fprintf(stdout,"Fixed range is OFF \n");
        return true;
    }    
    return false;
}

bool ToolBlobber::setConfMin(float confid)
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
void ToolBlobber::onRead(Bottle& seedIn)
{
    //yarp::os::Stamp ts;
    
    mutex.wait();    
    if(verbose){   cout << endl << "================ LOOP =================== "<< endl;}
    Scalar red = Scalar(255,0,0);
    Scalar green = Scalar(0,255,0);
    Scalar blue = Scalar(0,0,255);
    Scalar white = Scalar(255,255,255);

	/* Read disparty data and format it to Mat grayscale */
    ImageOf<PixelBgr> *disparity = dispInPort.read();  // read an image
    if(disparity == NULL)		{
        printf("No disparity image \n");
		return;
	}    
    Mat disp((IplImage*) disparity->getIplImage());	
    cvtColor(disp, disp, CV_BGR2GRAY);						// Brg to grayscale
        
    /* Read camera Images */
    ImageOf<PixelRgb> *imInLeft = imInLeftPort.read();  // read an image
    if(imInLeft == NULL)		{
        printf("No input camera image \n");
		return;
	}
    Mat leftIm((IplImage*) imInLeft->getIplImage());	
    
    /* Read seed coordinates */
    /*Bottle *seedIn = seedInPort.read(false);
    if (seedIn == NULL)  {  
        printf("no seed data received, no image produced");
        return;
    }*/
    Point seed;
    seed.x = seedIn.get(0).asInt();
    seed.y = seedIn.get(1).asInt();

    /* Prepare output image for visualization */
    ImageOf<PixelRgb> &imageOut  = imageOutPort.prepare();
    imageOut.resize(imInLeft->width(), imInLeft->height());		// Initialize features image
    imageOut.zero();    
    Mat imOut((IplImage*)imageOut.getIplImage(),false);
    
    //disp.copyTo(imOut);

    /* Prepare binary image to ouput closest blob */
	ImageOf<PixelMono> &imgBinOut = imgBinOutPort.prepare();		// prepare an output image
	imgBinOut.resize(disparity->width(), disparity->height());		// Initialize features image
    imgBinOut.zero();
	Mat imgBin((IplImage*)imgBinOut.getIplImage(),false);
    
    /* Prepare output target port */
    Bottle &target = targetOutPort.prepare();
    target.clear();

    /* Filter disparity image to reduce noise an produce smoother blob */
    //  Smooth the image so that it connect components, and so that it wont cut the GBS blob
    Mat threshIm;   
    
    threshold(disp, threshIm, backgroundThresh, 1, CV_THRESH_BINARY);			// First clean up background
    multiply(disp,threshIm,disp);
    threshold(disp, threshIm, frontThresh, 1, CV_THRESH_BINARY_INV);			// and first noise 
    multiply(disp,threshIm,disp);
    dilate(disp, disp,Mat(), Point(-1,-1), 7);
    GaussianBlur(disp, disp, Size(21 ,21), 15, 15); 
    erode(disp, disp, Mat(), Point(-1,-1), 4);
    
       
    /* Find closest valid blob */
    /*
    double minVal, maxVal; 
    Point minLoc, maxLoc;	
    int fillFlags = 8;                                          // pixel connectivity
    if (fixedRange){                                            // Set the flags for floodfill
        fillFlags += FLOODFILL_FIXED_RANGE;}

    int fillSize = 0;	
    Mat aux = disp.clone();
    Mat fillMask = Mat::zeros(disp.rows + 2, disp.cols + 2, CV_8U);
    while (fillSize < minBlobSize){			
        minMaxLoc( aux, &minVal, &maxVal, &minLoc, &maxLoc );		// Look for brighter (closest) point
        fillSize = floodFill(aux, maxLoc, 0, 0, Scalar(maxVal/dispThreshRatioLow), Scalar(maxVal/dispThreshRatioHigh), fillFlags);	// If its too small, paint it black and search again
    }
    */
    int fillFlags = 8;                                          // pixel connectivity
    if (fixedRange){                                            // Set the flags for floodfill
        fillFlags += FLOODFILL_FIXED_RANGE;}
    Mat fillMask = Mat::zeros(disp.rows + 2, disp.cols + 2, CV_8U);
    floodFill(disp, fillMask, seed, 255, 0, 50 , 50, FLOODFILL_MASK_ONLY + fillFlags);	// Paint closest valid blob white
    

    /* Find Contours */
    Mat edges;	
    vector<vector<Point > > contours;
    vector<Vec4i> hierarchy;
    findContours( fillMask(Range(1,disp.rows),Range(1,disp.cols)), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );
	printf("Found %i contours from the mask. \n", contours.size());
    /* If any blob is found */
    if (contours.size()>0){
        if ((seed.x < imInLeft->width()) && (seed.x>0) && (seed.y < imInLeft->height()) && (seed.y>0))
        {
            /* Double check that only the bigger blob is selected as the valid one*/
            int blobI = 0;
            for( int c = 0; c < contours.size(); c++ ){
                double a = contourArea(contours[c]);	    					// Find the area of contour
                printf("area of blob found = %g ", a);
                if(a > minBlobSize){											// Keep only the bigger
                    blobI = c;
                    printf(" - valid blob\n"); 
                }
            }
            /* Mark closest blob for visualization*/        
            drawContours(imOut, contours, blobI, red, 2, 8); 
            //drawContours(imgBin, contours, blobI, white, -1, 8); 

            Mat cntMask(disp.size(), CV_8UC1, Scalar(0));                   // do a mask by using drawContours (-1) on another black image
            drawContours(cntMask, contours, blobI, Scalar(255), CV_FILLED, 8);  // Mask ignoring points outside the contours
            leftIm.copyTo(imOut,cntMask);
            //cntMask.copyTo(imOut);

            /* Get Target blob coordinates for seeding GBS*/
            Point targetBlob; 
            targetBlob.x = seed.x;  
            targetBlob.y = seed.y;// + 15; //Search for the tool blob slighly under the detected tooltip
            circle( imOut, targetBlob, 4, red, -1, 8, 0 );

            target.addInt(targetBlob.x);
            target.addInt(targetBlob.y);       

            // Get graphBased segmenenation within the close blob
            Bottle cmdGBS,replyGBS;
            cmdGBS.clear();    replyGBS.clear();
            cmdGBS.addString("get_component_around");
            cmdGBS.addInt(targetBlob.x);
            cmdGBS.addInt(targetBlob.y);
            rpcGBS.write(cmdGBS, replyGBS);
            if (replyGBS.size() > 0)
            {
                if (verbose) {cout << "Received " << replyGBS.size() <<  "bottles from GBS " << endl;} 
                Bottle *segPixels = replyGBS.get(0).asList();
                // Go through all the returned pixels and make a mask out of them. 
                Mat segMask(disp.size(), CV_8UC1, Scalar(0));
                for( int sp = 0; sp < segPixels->size(); sp++ ){
                    Bottle *pointB = segPixels->get(sp).asList();
                    Point segPoint(pointB->get(0).asInt(), pointB->get(1).asInt());
                    circle(segMask, segPoint, 0, Scalar(255));
                }             
                multiply(cntMask, segMask, imgBin);         // Multiply this mask with the one obtained from disparity
            }else{
                if (verbose) {cout << "Nothing received from GBS" << endl;}
            }
        }else{
            if (verbose) {cout << "seed outside image boundaries" << endl;}
        }        
    }else{
            if (verbose) {cout << "No contours detected" << endl;}
    }
        
    mutex.post();

    /* write info on output ports */
    //imageOutPort.setEnvelope(ts);
    imageOutPort.write();
    imgBinOutPort.write();
    targetOutPort.write();

}
//empty line to make gcc happy

