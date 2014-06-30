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
    if (receivedCmd == "origin"){
        if (command.size() == 4){
            vector<double> origCoords(3);
            for (int i=1; i<=3; i++){
                origCoords[i-1] = command.get(i).asDouble();            
            }
            bool ok = blobber->setOrigin(origCoords);
            if (ok)
                responseCode = Vocab::encode("ack");
            else {
                fprintf(stdout,"Coordinates format not accepted. Try [origin X Y Z]. \n");
                responseCode = Vocab::encode("nack");
            }
        }else
            responseCode = Vocab::encode("nack");
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
        bool ok = blobber->setThresh(command.get(1).asInt());
        if (ok)
            responseCode = Vocab::encode("ack");
        else {
            fprintf(stdout,"Threshold for disparity considered set. \n");
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
        reply.addString("origin (int) (int) (int)- set the coordinates from where the 3D distance is computed.");        
        reply.addString("range (double) - modifies the distance within which blobs are considered reachable.");
        reply.addString("thresh (int) - to sets the lower limit of disparity in terms of luminosity (0-255) that is considered. In other words, objects with luminosity under T, i.e. further away, wont be considered.");
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
    return 0.3;
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

    Bottle originFrame = moduleRF->findGroup("origin");    
    if (originFrame.size()>0){
        for (int i=0; i<originFrame.size(); i++)
            origin[i] = originFrame.get(i+1).asDouble();
    }else{
        origin = Scalar(0,0,0);
    }
    fprintf(stdout,"Frame used : [%.2f,%.2f,%.2f]\n", origin[0], origin[1], origin[2] );
    verbose = moduleRF->check("verbose", Value(false)).asBool();
    range = moduleRF->check("range", Value(0.5)).asDouble();
    backgroundThresh = moduleRF->check("backgroundThresh", Value(50)).asInt();		// threshold of intensity if the disparity image, under which info is ignored.
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
    
    dispInPortName = "/" + moduleName + "/disp:i";
    BufferedPort<ImageOf<PixelBgr>  >::open( dispInPortName.c_str() );    

    imLeftInPortName = "/" + moduleName + "/imLeft:i";
    imagePortInLeft.open(imLeftInPortName);
    
    imRightInPortName = "/" + moduleName + "/imRight:i";
    imagePortInRight.open(imRightInPortName);

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
    
    BufferedPort<ImageOf<PixelBgr>  >::close();
    imagePortInLeft.close();
    imagePortInRight.close();

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
   
    BufferedPort<ImageOf<PixelBgr>  >::interrupt();
    imagePortInLeft.close();
    imagePortInRight.close();

    imageOutPort.interrupt();
    imgBinOutPort.interrupt();
    targetOutPort.interrupt();
    rpcGBS.interrupt();
    
    fprintf(stdout,"finished interrupt ports\n");
}

bool ToolBlobber::setOrigin(vector<double> o)
{   
    if (o.size()!=3){
        fprintf(stdout,"Coordinates format not valid, origin not modified. \n");
        return false;
    }

    for (int i=0; i<o.size(); i++)
        this->origin[i] = o[i];
    fprintf(stdout,"New origin Coords are : [%.2f,%.2f,%.2f]\n", origin[0], origin[1], origin[2] );
    return true;
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

bool ToolBlobber::setThresh(int t)
{
    if ((t<0) ||(t>255)) {
        fprintf(stdout,"Please select a valid luminance value (0-255). \n");
        return false;
    }
    fprintf(stdout,"New Threshold is : %d\n", t);
    this->backgroundThresh = t;
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
void ToolBlobber::onRead(ImageOf<PixelBgr> &disparity)
{
    yarp::os::Stamp ts;
    
    mutex.wait();    
    if(verbose){   cout << endl << "================ LOOP =================== "<< endl;}
    Scalar red = Scalar(255,0,0);
    Scalar green = Scalar(0,255,0);
    Scalar blue = Scalar(0,0,255);
    Scalar white = Scalar(255,255,255);

	/* Format disparty data to Mat grayscale */
    Mat disp((IplImage*) disparity.getIplImage());			
    cvtColor(disp, disp, CV_BGR2GRAY);						// Brg to grayscale

    /* Read camera Images */
    ImageOf<PixelRgb> *imInLeft = imagePortInLeft.read();  // read an image
    Mat leftIm((IplImage*) imInLeft->getIplImage());	
    ImageOf<PixelRgb> *imInRight = imagePortInRight.read();  // read an image
    Mat rightIm((IplImage*) imInRight->getIplImage());

    /* Prepare output image for visualization */
    ImageOf<PixelRgb> &imageOut  = imageOutPort.prepare();
    imageOut.resize(imInLeft->width(), imInLeft->height());		// Initialize features image
    imageOut.zero();
    //imageOut = disparity;	
    Mat imOut((IplImage*)imageOut.getIplImage(),false);

    /* Prepare binary image to ouput closest blob */
	ImageOf<PixelMono> &imgBinOut = imgBinOutPort.prepare();		// prepare an output image
	imgBinOut.resize(disparity.width(), disparity.height());		// Initialize features image
    imgBinOut.zero();
	Mat imgBin((IplImage*)imgBinOut.getIplImage(),false);
    
    /* Prepare output target port */
    Bottle &target = targetOutPort.prepare();
    target.clear();

    /* Filter disparity image to reduce noise */
    GaussianBlur(disp, disp, Size(gaussSize,gaussSize), 1.5, 1.5);
    Mat threshIm;
    threshold(disp, threshIm, backgroundThresh, 1, CV_THRESH_BINARY);			// First clean up background
    multiply(disp, threshIm, disp);	
    //cvtColor(disp, imOut, CV_GRAY2BGR);						                    // Grayscale to BGR    
    
    /* Find closest valid blob */
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

    floodFill(disp, fillMask, maxLoc, 255, 0, Scalar(maxVal/dispThreshRatioLow), Scalar(maxVal/dispThreshRatioHigh), FLOODFILL_MASK_ONLY + fillFlags);	// Paint closest valid blob white
    
    // XXX Smooth the image so that it connect components, and so that it wont cut the GBS blob
    GaussianBlur(fillMask, fillMask, Size(3,3), 1.5, 1.5);

    /* Find Contours */
    Mat edges;	
    vector<vector<Point > > contours;
    vector<Vec4i> hierarchy;
    //Canny( disp, edges, cannyThresh, cannyThresh*3, 3 );			// Detect edges using canny	
    findContours( fillMask(Range(1,disp.rows),Range(1,disp.cols)), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
    /* If any blob is found */
    if (contours.size()>0){
        /* Double check that only the bigger blob is selected as the valid one*/
        int blobI = 0;
        for( int c = 0; c < contours.size(); c++ ){
			double a = contourArea(contours[c]);	    					// Find the area of contour
			if(a > minBlobSize){											// Keep only the bigger
                blobI = c;
            }
        }
        /* Mark closest blob for visualization*/        
        //drawContours(imOut, contours, blobI, white, 2, 8); 
        //drawContours(imgBin, contours, blobI, white, -1, 8); 

        Mat cntMask(disp.size(), CV_8UC1, Scalar(0));                   // do a mask by using drawContours (-1) on another black image
        drawContours(cntMask, contours, blobI, Scalar(1), CV_FILLED, 8);  // Mask ignoring points outside the contours
        leftIm.copyTo(imOut,cntMask);

        /* check and plt the ROI */
        Rect blobBox = boundingRect(contours[blobI]);
        if(verbose)
            {cout << " blob Box is  ["<< blobBox.tl().x << "," << blobBox.tl().y << "]:["<<blobBox.br().x << ","<<blobBox.br().y << "]"<< endl;}
        rectangle(imgBin, blobBox, blue, 2 );

        Point center2DCoords;
        Moments mu = moments( contours[blobI], false );		
        center2DCoords = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
        // circle( imOut, center2DCoords, 4, red, -1, 8, 0 );

        /* Send Target coordinates*/
        if(verbose)
        {cout << "coords of the center of closest blob are : " << center2DCoords.x << ", "<< center2DCoords.y << endl;}
        
        target.addInt(center2DCoords.x);
        target.addInt(center2DCoords.y);

        cout << "Getting pixels from GBS" << endl;
        // Get graphBased segmenenation within the close blob
        Bottle cmdGBS,replyGBS;
        cmdGBS.clear();    replyGBS.clear();
        cmdGBS.addString("get_component_around");
        cmdGBS.addInt(center2DCoords.x);
        cmdGBS.addInt(center2DCoords.y);
        rpcGBS.write(cmdGBS, replyGBS);
        cout << "Received " << replyGBS.get(0).asString() << endl; 
        Bottle *segPixels = replyGBS.get(0).asList();

        //cout << "Received " << segPixels->size() << " pixel locations. Creating mask from received pixels" << endl;
        // XXX Go through all the returned pixels and make a mask out of them. 
        Mat segMask(disp.size(), CV_8UC1, Scalar(0));
        for( int sp = 0; sp < segPixels->size(); sp++ ){
            Bottle *pointB = segPixels->get(sp).asList();
            Point segPoint(pointB->get(0).asInt(), pointB->get(1).asInt());
            circle(segMask, segPoint, 0, Scalar(255));
            //circle(imgBin, segPoint, 0, Scalar(255));
        }
        
        
        // Multiply this mask with the one obtained from disparity
        multiply(cntMask, segMask, imgBin);
        // Put the result as the desired blob

    }
        
    mutex.post();

    /* write info on output ports */
    imageOutPort.setEnvelope(ts);
    imageOutPort.write();
    imgBinOutPort.write();
    targetOutPort.write();

}
//empty line to make gcc happy

