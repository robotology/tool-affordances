#include "learnAffordance.h"

#include <yarp/sig/Vector.h>
#include <string>

using namespace yarp::sig;
using namespace yarp::os;

void LearnAffordance::loop()
{		
	Bottle *log = logPort.read();  // read a target
	if (log)
	{
		printf("Log info read: %s\n", log->get(0).asString().c_str());
		if(framing(*log) == true)
		{
			printf("send data out\n \n\n");
			featPort.write();					
		}	
	}
}

bool LearnAffordance::framing(yarp::os::Bottle& log)
{
	Bottle *position = targetPort.read();  // read a target
	Bottle *blobFeat = blobPort.read();  // read a target
	//printf("Blobdata %g, %g, %g \n", blobFeat->get(0).asList()->get(4).asDouble(), blobFeat->get(0).asList()->get(5).asDouble(), blobFeat->get(0).asList()->get(6).asDouble() );

	double x,y,plane;
	double axis1, axis2, orientation;

	if (position && blobFeat){
		x = position->get(0).asDouble();
		y = position->get(1).asDouble();
		plane = position->get(2).asDouble();
		printf("Target object is in position %g %g, %g \n",x ,y, plane );
			
		//double boundingbox[4];
		//for (int i=0;i<4;i++){
		//	boundingbox[i] = blobFeat->get(i).asDouble();
		//}
		
		if (blobFeat && 
			blobFeat->size() &&
			(blobFeat->get(0).asList() != NULL) && 
			(blobFeat->get(0).asList()->size() >= 7) )
		{		
			orientation = blobFeat->get(0).asList()->get(4).asDouble();
			axis1 =  blobFeat->get(0).asList()->get(5).asDouble();
			axis2 =  blobFeat->get(0).asList()->get(6).asDouble();
			printf("Blob orient and axes are %g, %g, %g \n", orientation, axis1, axis2 );
		}
	}
	
	// Read from log port. second value is a double with the timestamp, which maybe can be ignored. Third value says if it is an action begin (out) or an end (out)
	bool ACTION_BEGINS = false;
	if (!strcmp(log.get(0).asString().c_str(),"out"))
	{
		ACTION_BEGINS= true;

		//printf("Action Begins %s \n", log.get(1).asList()->get(0).asString().c_str());
		tuple.clear();
		string action = log.get(1).asList()->get(0).asString().c_str();
		printf("Action  %s begins", action.c_str());
		tuple.action = action;

		string objectLab = log.get(1).asList()->get(1).asString().c_str();
		printf(" on object %s\n", objectLab.c_str());
		tuple.objectLab= objectLab; 
		
		tuple.init.push_back(x);
		tuple.init.push_back(y);
		tuple.init.push_back(orientation);	
		tuple.init.push_back(axis1);	
		tuple.init.push_back(axis2);
	}

	bool ACTION_END = false;
	if (!strcmp(log.get(0).asString().c_str(),"in"))
	{
		ACTION_END= true;
		printf("Action Ends\n ");
		tuple.final.push_back(x);
		tuple.final.push_back(y);
		tuple.final.push_back(orientation);		
		tuple.final.push_back(axis1);	
		tuple.final.push_back(axis2);	
		// now lets put it all together to send 
		return fillTuple();	
		
	}
	
	return false;
}

bool LearnAffordance::fillTuple() 
{
	/*
	* TODO: implement clustering algorithm
	
	// update process
	cluster(0,0) = tuple.init[0];
	cluster(0,1) = tuple.init[1];
	*/
	// fill  the output feature port
	Bottle& btAffTuple = featPort.prepare();	
	btAffTuple.clear();

	Bottle& btInit = btAffTuple.addList();
	for(int i=0; i<tuple.init.size(); i++)
		btInit.addDouble(tuple.init[i]);

	Bottle& btFinal = btAffTuple.addList();
	for(int i=0; i<tuple.final.size(); i++)
		btFinal.addDouble(tuple.final[i]);

	btAffTuple.addString(tuple.action.c_str());
	btAffTuple.addString(tuple.objectLab.c_str());

	return true;
}

bool LearnAffordance::open()
{
	printf("Opening resources\n");
	bool ret=true;
	ret = targetPort.open("/learnAff/targetPos:i");		  //port to receive the object coordinates in the 3D space
	ret = ret && blobPort.open("/learnAff/blobs:i");	  //port to receive info about the blobs in the image.
	ret = ret && logPort.open("/learnAff/log:i");		  //port to read the log
	ret = ret && featPort.open("/learnAff/feat:o");		  //port to send the features

	
	if (!ret){
		printf("Problems opening featAffordance ports\n");
		return false;
	}

    return true;
}

bool LearnAffordance::close()
{
	blobPort.close();	
    logPort.close();
    featPort.close();
    targetPort.close();

	//clientGaze.close();

    return true;
}

bool LearnAffordance::interrupt()
{
	printf("%s : %d\n", __FILE__, __LINE__);
	blobPort.interrupt();
	featPort.interrupt();
	logPort.interrupt();
    targetPort.interrupt();
    return true;
}
