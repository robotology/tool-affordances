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

    robot = moduleRF->check("robot", Value("icub")).asString();
    hand = moduleRF->check("hand", Value("right")).asString();
    camera = moduleRF->check("camera", Value("left")).asString();

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
    BufferedPort<Bottle >::open("/toolBlobber/seed:i" );    
    dispInPort.open("/toolBlobber/disp:i");
    imInLeftPort.open("/toolBlobber/imLeft:i");

    /* Output ports */
    rpcGBS.open("/toolBlobber/gbs:rpc");
    imageOutPort.open("/toolBlobber/img:o");
    imgBinOutPort.open("/toolBlobber/imgBin:o");    
    targetOutPort.open("/toolBlobber/target:o");

  	// Cartesian Controller Interface
	Property optCart;
	optCart.put("device","cartesiancontrollerclient");
    optCart.put("remote","/"+robot+"/cartesianController/"+hand+"_arm");
    optCart.put("local","/toolBlobber/cartesian_client/"+hand+"_arm");
	if (!clientCart.open(optCart))
		return false;    
    clientCart.view(icart);	// open the view	

    // Gaze Controller Interface
    Property optGaze("(device gazecontrollerclient)");
    optGaze.put("remote","/iKinGazeCtrl");
    optGaze.put("local","/toolBlobber/gaze_client");

    if (!clientGaze.open(optGaze))
        return false;
    clientGaze.view(igaze);

    igaze->setNeckTrajTime(0.8);
    igaze->setEyesTrajTime(0.4);
    igaze->setTrackingMode(true);

    isDisp = false;

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
    if(verbose){   cout << endl << " seed received "<< endl;}
    Scalar red = Scalar(255,0,0);
    Scalar green = Scalar(0,255,0);
    Scalar blue = Scalar(0,0,255);
    Scalar white = Scalar(255,255,255);

    // ================ READ FROM AND PREPARE PORTS =================
    printf("preparing ports\n");
	/* Read disparty data and format it to Mat grayscale */
    ImageOf<PixelBgr> *disparity = dispInPort.read(false);  // read an image
    Mat disp;
    if(disparity == NULL)		{
        printf("No disparity image \n");
		isDisp = false;
    }else{ 
        disp=(IplImage*) disparity->getIplImage();	
        cvtColor(disp, disp, CV_BGR2GRAY);						// Brg to grayscale
        isDisp = true;
    }
    

    /* Read camera Images */
    ImageOf<PixelRgb> *imInLeft = imInLeftPort.read();  // read an image
    if(imInLeft == NULL)		{
        printf("No input camera image \n");
		return;
	}
    Mat leftIm((IplImage*) imInLeft->getIplImage());	
    
    /* Read seed coordinates */
    Point seed;
    seed.x = seedIn.get(0).asInt();
    seed.y = seedIn.get(1).asInt();


    /* Prepare output image for visualization */
    ImageOf<PixelRgb> &imageOut  = imageOutPort.prepare();
    imageOut.resize(imInLeft->width(), imInLeft->height());		// Initialize features image
    imageOut.zero();    
    Mat imOut((IplImage*)imageOut.getIplImage(),false);
    
    /* Prepare binary image to ouput closest blob */
	ImageOf<PixelMono> &imgBinOut = imgBinOutPort.prepare();		// prepare an output image
	imgBinOut.resize(imInLeft->width(), imInLeft->height());		// Initialize features image
    imgBinOut.zero();
	Mat imgBin((IplImage*)imgBinOut.getIplImage(),false);
    
    /* Prepare output target port */
    Bottle &target = targetOutPort.prepare();
    target.clear();

    //=================== BEGIN ALGORITHM ======================
    printf("Begin algorithm\n");
    /* Get a mask of the tool from the 3Dinformation from disparity */
    Mat imAux;
    Mat cntMask;
    if (isDisp){            
        if ((seed.x < imInLeft->width()) && (seed.x>0) && (seed.y < imInLeft->height()) && (seed.y>0))
        {       
            printf("Calling getDispBlob\n");
            bool dispOK = getDispBlob(cntMask, disp, seed);

            if(!dispOK)
            {  cntMask.setTo(Scalar(255)); }

            }else{
                if (verbose) {cout << "seed outside image boundaries" << endl;}
                return;
        }
        printf("Disparity blob found\n");
    }

    /* Get Target blob coordinates for seeding GBS*/
    Point targetBlob; 
    targetBlob.x = seed.x;  
    targetBlob.y = seed.y;// + 15; //Search for the tool blob slighly under the detected tooltip

    target.addInt(targetBlob.x);
    target.addInt(targetBlob.y);       

    printf("Getting GBS points\n");
    // Get the seeded blob from graphBased segmenenation
    Bottle cmdGBS,replyGBS;
    cmdGBS.clear();    replyGBS.clear();
    cmdGBS.addString("get_component_around");
    cmdGBS.addInt(targetBlob.x);
    cmdGBS.addInt(targetBlob.y);
    rpcGBS.write(cmdGBS, replyGBS);
    if (replyGBS.size() > 0)
    {
        if (verbose) {cout << "Received " << replyGBS.size() <<  " bottle(s) from GBS " << endl;} 
        Bottle *segPixels = replyGBS.get(0).asList();
        // Go through all the returned pixels and make a mask out of them. 
        Mat segMask(leftIm.size(), CV_8UC1, Scalar(0));
        for( int sp = 0; sp < segPixels->size(); sp++ ){
            Bottle *pointB = segPixels->get(sp).asList();
            Point segPoint(pointB->get(0).asInt(), pointB->get(1).asInt());
            circle(segMask, segPoint, 0, Scalar(255));
        }             
        if (isDisp){
            multiply(cntMask, segMask, imAux);}         // Multiply this mask with the one obtained from disparity
        else{
            segMask.copyTo(imAux);
        }         // Multiply this mask with the one obtained from disparity
    }else{
        if (verbose) {cout << "Nothing received from GBS" << endl;}
        return;
    }

    cvtColor(imAux, imOut, CV_GRAY2RGB);    						//XXX grayscale to RGB

    // Rotate the tool blob according to the handling angle
    printf("Rotating Image\n");
    /* Get the info about the hand from the interface */
    yarp::sig::Vector handPose(3), handOr(4), handPix(2);
    icart->getPose(handPose, handOr);
    igaze->get2DPixel(0, handPose, handPix); 


    Point hand = Point(  (int)handPix[0] , (int)handPix[1] );
    
    /* Read tooltip coordinates */
    yarp::sig::Vector toolTipPix(2);
    toolTipPix[0] = seed.x;
    toolTipPix[1] = seed.y;
    if (verbose) {printf("Tooltip at pixel %g,%g \n", toolTipPix[0], toolTipPix[1]);}

    // Set the tool orientation as reference angle for feature extraction
    yarp::sig::Vector tipDif(2);
    tipDif[0]= handPix[0] - toolTipPix[0];
    tipDif[1]= handPix[1] - toolTipPix[1];
    double handAngle = atan2(double(tipDif[1]), double(tipDif[0])) * 180.0 / CV_PI;  
    if (verbose) {printf("Handling Angle is of %g degrees\n", handAngle);}

    /// Compute a rotation matrix with respect to the center of the hand
    //Compute the angle and the tip distance to normalize the tool image size
    double angle = -90+handAngle;
    double tipDist = sqrt(tipDif[0]*tipDif[0] + tipDif[1]*tipDif[1]);
    //double scale = 150/tipDist; 

    /// Get the rotation matrix with the specifications above
    if (verbose) {printf("Before rotation, the hand is on point %i,%i and the tooltip at %i,%i \n", hand.x,hand.y,seed.x,seed.y);}
    Mat rot_mat( 2, 3, CV_32FC1 );
    rot_mat = getRotationMatrix2D( hand, angle, 1.0 );
    if (verbose) {printf("Rotation Matrix:"); cout << "R = "<< endl << " "  << rot_mat << endl << endl;}
    
    // Rotate the image
    warpAffine(imAux, imAux, rot_mat, imgBin.size() );

    // Rotate the tooltip to get the correct ROI
    Point toolTipRot;
    double alphaRad = -angle * CV_PI/ 180.0;
    double s = sin(alphaRad); double c = cos(alphaRad);
    toolTipRot.x = (c*(seed.x-hand.x) - s*(seed.y-hand.y)) + hand.x;
    toolTipRot.y = (s*(seed.x-hand.x) + c*(seed.y-hand.y)) + hand.y;
    if (verbose) {printf("After rotation, the tooltip at %i,%i \n", toolTipRot.x,toolTipRot.y);}

    // Set the ROI to bound the tool and crop the arm away    
    yarp::sig::Vector ROIV(4);          // define ROI as tl.x, tl.y, br.x, br.y
    ROIV[0] = toolTipRot.x - 40;     // ROI left side some pixels to the left of the tooltip
    if (ROIV[0]<0) {ROIV[0] =0;}
    if (ROIV[0]>leftIm.cols) {ROIV[0]=leftIm.cols-1;}
	ROIV[1] = toolTipRot.y - 40; // ROI top side a little bit over the height of the tooltip
    if (ROIV[1]<0) {ROIV[1] =0;}
    if (ROIV[1]>leftIm.rows) {ROIV[1]=leftIm.rows-1;}
    ROIV[2] = toolTipRot.x + 40; // ROI right side some pixels to the right of the tooltip
    if (ROIV[2]<0) {ROIV[2] =0;}
    if (ROIV[2]>leftIm.cols) {ROIV[2]=leftIm.cols-1;}
    ROIV[3] = hand.y - 60;  // ROI bottom side at the same height of the hands center
    if (ROIV[3]<0) {ROIV[3] =0;}
    if (ROIV[3]>leftIm.rows) {ROIV[3]=leftIm.rows-1;}
    Rect ROI(Point(ROIV[0],ROIV[1]),Point(ROIV[2],ROIV[3]));
    

    //draw stuff in imOut for visualization
    //cvtColor(imAux, imOut, CV_GRAY2RGB);    						// grayscale to RGB
    //cvtColor(imOut(ROI), imgBin(ROI), CV_RGB2GRAY);    						// Brg to grayscale
    imAux(ROI).copyTo(imgBin(ROI));
    circle( imOut, targetBlob, 4, red, -1, 8, 0 );
    line(imOut,hand,seed,red);
    circle( imOut, toolTipRot, 4, green, -1, 8, 0 );
    line(imOut,hand,toolTipRot,green);
    rectangle(imOut, Point(ROIV[0],ROIV[1]),Point(ROIV[2],ROIV[3]), blue, 1);

    mutex.post();

    /* write info on output ports */
    //imageOutPort.setEnvelope(ts);
    imageOutPort.write();
    imgBinOutPort.write();
    targetOutPort.write();

}


bool ToolBlobber::getDispBlob(const Mat& disp, Mat& cntMask, Point seed)
{
    /* Filter disparity image to reduce noise an produce smoother blob */
    printf("- Smoothing image\n");
    Mat threshIm;       
    threshold(disp, threshIm, backgroundThresh, 1, CV_THRESH_BINARY);			// First clean up background
    multiply(disp,threshIm,disp);
    threshold(disp, threshIm, frontThresh, 1, CV_THRESH_BINARY_INV);			// and first noise 
    multiply(disp,threshIm,disp);
    dilate(disp, disp,Mat(), Point(-1,-1), 7);
    GaussianBlur(disp, disp, Size(21 ,21), 15, 15); 
    erode(disp, disp, Mat(), Point(-1,-1), 4);    
    
    printf("- Filling seeded blob \n");
    // Plot  the blob selected from seed in the outImage.
    int fillFlags = 8;                                          // pixel connectivity
    if (fixedRange){                                            // Set the flags for floodfill
        fillFlags += FLOODFILL_FIXED_RANGE;}
    Mat fillMask = Mat::zeros(disp.rows + 2, disp.cols + 2, CV_8U);
    printf("fails here");
    floodFill(disp, fillMask, seed, Scalar(255), 0, Scalar(50) , Scalar(50), FLOODFILL_MASK_ONLY + FLOODFILL_FIXED_RANGE + 4);	
    printf("Doesnt it");
    

    /* Find Contours */
    printf("- Finding contours \n");
    Mat edges;	
    vector<vector<Point > > contours;
    vector<Vec4i> hierarchy;
    findContours( fillMask(Range(1,disp.rows),Range(1,disp.cols)), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );    
	printf("Found %i contours from the mask. \n", contours.size());
    /* If any blob is found */
    if (contours.size()>0){        
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
        Mat cntMask(disp.size(), CV_8UC1, Scalar(0));                   // do a mask by using drawContours (-1) on another black image
        drawContours(cntMask, contours, blobI, Scalar(255), CV_FILLED, 8);  // Mask ignoring points outside the contours
     
    }else{
        if (verbose) {cout << "No contours detected" << endl;}
        return false;
    }   
}



//empty line to make gcc happy
