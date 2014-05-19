/* 
blobPicker selects a specific blob among the available according to the criteria expressed in the parameter or through the rpc commands.
For that end, it receives a binary image of the blobs, and a list with the blob bounding boxes coordinates. 
It outputs the propagated image and returns the index of the selected blob in the bounding box list, and the binary image contianing only the selected blob.
The accepted criteria are:
	- 'long'
	- XXXX

*/
#include "blobPicker.h"
#include <cv.h>

using namespace yarp::sig;
using namespace yarp::os;

void BlobPicker::loop()
{
	Bottle cmd, reply;
	rpcPort.read(cmd, true);

	/* Check if any command has been received */
	if (cmd.size()== 0){
		reply.addString("nack");
		reply.addString("invalid command");
        rpcPort.reply(reply);
		return;
	}

	ImageOf<PixelRgb> &imageOut = imOutPort.prepare();	// prepare the port for output image
	
	ImageOf<PixelRgb> *imageIn = imInPort.read();		// read an image
	if(imageIn == NULL){
		reply.addString("nack");
		reply.addString("invalid image");
        rpcPort.reply(reply);
		return;
	}
	imageOut = *imageIn;								// input image to owerwrite with new elements

	Bottle *blobBoxes = blobsInPort.read();				// read the blobs bounding boxes
	// check we actually got blob information
	if(blobBoxes->size() <= 0){
		reply.addString("nack");
		reply.addString("no blobs received");
        rpcPort.reply(reply);
		return;
	}


	/********* Check for the longest blob  *********/
	// XXXXX all this has to be moved to the respond command, or make a thrift file.
	int selBlobI;										// index of the selected blob
	if (cmd.get(0).asString().c_str() == "long"){
		//printf("Looking for longer blob");
		double bLen;
		double maxLen=0;
		for (int j=0; j<blobBoxes->size(); j++)
		{
			CvPoint tl,br;
			Bottle *item = blobBoxes->get(j).asList();
			tl.x=(int)item->get(0).asInt();
			tl.y=(int)item->get(1).asInt();
			br.x=(int)item->get(2).asInt();
			br.y=(int)item->get(3).asInt();

			bLen=abs(tl.y-br.y);
			if (bLen > maxLen){
				maxLen = bLen;
				selBlobI = j;
				//printf("Longest blob	is blob %d \n", j);
			}
			//cvRectangle(imageOut.getIplImage(),tl,br,cvScalar(0,0,255),2);
		} 
	}
	//printf("Longest blob found, blob %i \n", maxLenI);

	// XXXXX Prepare image with only seleected blob.

	//****** Saving info of the selecteed blob *********
	Bottle *selBlobBox = blobBoxes->get(selBlobI).asList();
	CvPoint tl,br;
	tl.x=(int)selBlobBox->get(0).asInt();
	tl.y=(int)selBlobBox->get(1).asInt();
	br.x=(int)selBlobBox->get(2).asInt();
	br.y=(int)selBlobBox->get(3).asInt();

	//printf("Drawing rectangle\n");
	cvRectangle(imageOut.getIplImage(),tl,br,cvScalar(255,0,0),2);
	
	//Write info on ports
	reply.clear();
	reply.addInt(selBlobI);
	rpcPort.reply(reply);

	imOutPort.write();
}

bool BlobPicker::open()
{
    bool ret=true;
	ret = rpcPort.open("/blobPicker/cmd:rpc");					// RPC port 
    ret = ret && imInPort.open("/blobPicker/image:i");			// port receiving input image
    ret = ret && blobsInPort.open("/blobPicker/blobs:i");		// port to receive info about the blobs in the image.
	ret = ret && imOutPort.open("/blobPicker/image:o");			// port to send out the binary image of the selected blob
	ret = ret && blobOutPort.open("/blobPicker/blob:o");		// port to send out the coordinates of the selected blob
   
    return ret;
}

bool BlobPicker::close()
{
	
	printf("Closing");
    
	imOutPort.setStrict();
	imOutPort.write();

	blobsInPort.setStrict();
	blobsInPort.write();

	blobOutPort.setStrict();
	blobOutPort.write();


    imInPort.close();
	imOutPort.close();
	blobsInPort.close();    
    blobOutPort.close();
	printf("Closed");
    return true;
}

bool BlobPicker::interrupt()
{
	imInPort.interrupt();
	blobsInPort.interrupt();
	blobOutPort.interrupt();
    return true;
}

