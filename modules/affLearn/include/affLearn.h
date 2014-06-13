#ifndef __AFFLEARN_H__
#define __AFFLEARN_H__



#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <cv.h>

#include <yarp/os/all.h>
#include <yarp/sig/all.h>

/************************************************************************/
class AffLearn : public yarp::os::RFModule
{
protected:

    yarp::os::Semaphore mutex;
    yarp::os::RpcServer rpcPort;
    yarp::os::Port      plotPort;

    std::string name;
    std::string configFileName;
    
    std::map <std::string, cv::Mat> dataset;
    std::map <std::string, std::vector<std::string> > labelSet;

    bool dataCollector(const std::string &matName, const yarp::os::Bottle *data);

    void train(const std::string &inputMat, const std::string &outputMat);

    void cluster();

    void predict(const std::string &item, const yarp::os::Bottle &input, yarp::os::Bottle &output, yarp::os::Bottle &variance);

    void save();

    void clear();

    void list2vector(const yarp::os::Bottle* bot, std::vector<float>& vec);
    void list2mat(const yarp::os::Bottle* bot, cv::Mat& mat);

public:

    bool configure(yarp::os::ResourceFinder &rf); // configure all the module parameters and return true if successful

    bool interruptModule();                       // interrupt, e.g., the ports

    bool close();                                 // close and shut down the module

    double getPeriod();

    bool updateModule();

    bool respond(const yarp::os::Bottle &command, yarp::os::Bottle &reply);

};

#endif

