/* 
Computes some operations related to the 3D position of blobs via stereovision
- get orientation 
- get 3D position
- recognize the blobs within a range
- 
*/

#include <iCub/data3D/SurfaceMeshWithBoundingBox.h>
#include "blob3Dinfo.h"
#include <cv.h>

using namespace std;
using namespace yarp::sig;
using namespace yarp::os;

void Blob3DInfo::loop()
{

	im3DOutPort.write();
}

bool Blob3DInfo::open()
{
    bool ret=true;
    ret = rpcPort.open("/blob3Dinfo/cmd:rpc");						// RPC port
    ret = ret && blobsInPort.open("/blob3Dinfo/blobs:i");			// port to receive info about the blobs in the image.
	ret = ret && infoOutPort.open("/blob3Dinfo/info:o");			// port to send the requested info about the blobs
	ret = ret && imBinOutPort.open("/blob3Dinfo/image3D:o");		// port to send out the binary image with the blobs
	ret = ret && im3DOutPort.open("/blob3Dinfo/imageBin:o");		// port to send out the 3D image
   
    return ret;
}

bool Blob3DInfo::close()
{
	
	printf("Closing");
    
	infoOutPort.setStrict();
	infoOutPort.write();

	imBinOutPort.setStrict();
	imBinOutPort.write();

	im3DOutPort.setStrict();
	im3DOutPort.write();

    rpcPort.close();
	blobsInPort.close();
    infoOutPort.close();
	imBinOutPort.close();
    im3DOutPort.close();

	printf("Closed");
    return true;
}

bool Blob3DInfo::interrupt()
{
	blobsInPort.interrupt();
	infoOutPort.interrupt();
	imBinOutPort.interrupt();
	im3DOutPort.interrupt();

    return true;
}

vector<int> Blob3DInfo::blobsInRange( double range){
	// read blobsList from blobspotter
	// loop the blobs, and for each, do either
	// do the 3D projection of the ROI within the bounding box of each blob, and check for minimum distance there
	// or just check the center of the bounding box, and compute its distance in 3D.

	// returns indices of the blobs which have points within the ditance range.
	//vector<int> ind = 
	return ind;
}

info3D Blob3DInfo::get3Dinfo(){
}