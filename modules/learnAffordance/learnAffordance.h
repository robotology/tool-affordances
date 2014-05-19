#include <yarp/os/BufferedPort.h>
#include <yarp/sig/Image.h>

#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/RateThread.h>

#include <stdio.h>
#include <yarp/sig/Matrix.h>
#include <string>

using namespace std;
using namespace yarp::os;
//using namespace yarp::dev;
using namespace yarp::sig;

#define CLUSTER_MATRIX_ROWS	10
#define CLUSTER_MATRIX_COLS	10

class AffordanceInfo
{
public:
	AffordanceInfo() {}
	virtual ~AffordanceInfo() {}

	void clear(void) {
		init.clear();
		final.clear();
		action.clear();
		objectLab.clear();
	}

public:
	yarp::sig::Vector init;
	yarp::sig::Vector final;
	string action;
	string objectLab;
};


class LearnAffordance
{
   BufferedPort<yarp::os::Bottle> targetPort;
   BufferedPort<yarp::os::Bottle> blobPort;
   BufferedPort<yarp::os::Bottle> featPort;
   BufferedPort<yarp::os::Bottle> logPort;

public:

    LearnAffordance()
    {
        // constructor
		cluster.resize(CLUSTER_MATRIX_ROWS, CLUSTER_MATRIX_COLS);
    }

    bool open();

    bool close();

    void loop(); 

    bool interrupt();

private:
	bool framing(yarp::os::Bottle& log);
	bool fillTuple();

private:
	yarp::sig::Matrix cluster;
	AffordanceInfo tuple;

};
