
#include "toolRecognizer.h"

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;

/**********************************************************
                    PRIVATE METHODS
/**********************************************************/



/*******************************************************************************/
bool ToolRecognizer::trainObserve(const string &label)
{
    ImageOf<PixelRgb> img= *portImgIn.read(true);
    portImgOut.write(img);

    Bottle bot = *portBBcoordsIn.read(true);
    yarp::os::Bottle *items=bot.get(0).asList();

    double tlx = items->get(0).asDouble();
    double tly = items->get(1).asDouble();
    double brx = items->get(2).asDouble();
    double bry = items->get(3).asDouble();
    yInfo("[trainObserve] got bounding Box is %lf %lf %lf %lf", tlx, tly, brx, bry);

    Bottle cmd,reply;
    cmd.addVocab(Vocab::encode("train"));
    Bottle &options=cmd.addList().addList();
    options.addString(label.c_str());

    options.add(bot.get(0));

    yInfo("[trainObserve] Sending training request: %s\n",cmd.toString().c_str());
    portHimRep.write(cmd,reply);
    yInfo("[trainObserve] Received reply: %s\n",reply.toString().c_str());

    return true;
}



/**********************************************************/
bool ToolRecognizer::classifyObserve(string &label)
{
    ImageOf<PixelRgb> img= *portImgIn.read(true);
    portImgOut.write(img);

    Bottle cmd,reply;
    cmd.addVocab(Vocab::encode("classify"));
    Bottle &options=cmd.addList();

    Bottle bot = *portBBcoordsIn.read(true);

    for (int i=0; i<bot.size(); i++)
    {
        ostringstream tag;
        tag<<"blob_"<<i;
        Bottle &item=options.addList();
        item.addString(tag.str().c_str());
        item.addList()=*bot.get(i).asList();
    }

    yInfo("[classifyObserve] Sending classification request: %s\n",cmd.toString().c_str());
    portHimRep.write(cmd,reply);
    yInfo("[classifyObserve] Received reply: %s\n",reply.toString().c_str());

    label = processScores(reply);

    yInfo("[classifyObserve] the recognized label is %s", label.c_str());

    return true;
}
/**********************************************************/
string ToolRecognizer::processScores(const Bottle &scores)
{

    double maxScoreObj=0.0;
    string label  ="";

    for (int i=0; i<scores.size(); i++)
    {
        ostringstream tag;
        tag<<"blob_"<<i;

        Bottle *blobScores=scores.find(tag.str().c_str()).asList();
        if (blobScores==NULL)
            continue;

        for (int j=0; j<blobScores->size(); j++)
        {
            Bottle *item=blobScores->get(j).asList();
            if (item==NULL)
                continue;

            string name=item->get(0).asString().c_str();
            double score=item->get(1).asDouble();

            yInfo("name is %s with score %f", name.c_str(), score);

            if (score>maxScoreObj)
            {
                maxScoreObj = score;
                label.clear();
                label = name;
            }

        }
    }
    return label;
}

/*******************************************************************************/
bool ToolRecognizer::configure(ResourceFinder &rf)
{
    name = rf.check("name", Value("graspChecker"), "Getting module name").asString();
    running = true;

    printf("Opening ports\n" );
    bool ret= true;

    ret = ret && portBBcoordsIn.open("/"+name+"/bb:i");
    ret = ret && portImgIn.open("/"+name+"/img:i");
    ret = ret && portImgIn.open("/"+name+"/img:o");

    ret = ret && portHimRep.open("/"+name+"/himrep:rpc");
    ret = ret && portRpc.open("/"+name+"/rpc:i");

    if (!ret){
        printf("Problems opening ports\n");
        return false;
    }
    printf("Ports opened\n");

    attach(portRpc);

    return true;
}

/*******************************************************************************/
bool ToolRecognizer::interruptModule()
{
    portBBcoordsIn.interrupt();
    portImgIn.interrupt();
    portImgOut.interrupt();

    portHimRep.interrupt();
    portRpc.interrupt();
    return true;
}

/*******************************************************************************/
bool ToolRecognizer::close()
{
    portBBcoordsIn.close();
    portImgIn.close();
    portImgOut.close();

    portHimRep.close();
    portRpc.close();
    return true;
}

/*******************************************************************************/
double ToolRecognizer::getPeriod()
{
    return 0.1;
}

/*******************************************************************************/
bool ToolRecognizer::updateModule()
{
    if (!running)
        return false;
    return true;
}


/* =========================================== RPC COMMANDS ========================================= */

/******************RPC HANDLING METHODS*******************/
bool ToolRecognizer::attach(yarp::os::RpcServer &source)
{
    return this->yarp().attachAsServer(source);
}

/* Atomic commands */
// Setting up commands
bool ToolRecognizer::start(){
    std::cout << "Starting!" << std::endl;
    running = true;
    return true;
}
bool ToolRecognizer::quit(){
    std::cout << "Quitting!" << std::endl;
    running = false;
    return true;
}

//Thrifted
bool ToolRecognizer::train(const string &label)
{
    return trainObserve(label);
}

string ToolRecognizer::recognize()
{
    string label;
    classifyObserve(label);

    return label;
}


/*******************************************************************************/
int main(int argc,char *argv[])
{   
    Network yarp;
    if (!yarp.checkNetwork())
    {
        yError("unable to find YARP server!");
        return 1;
    }

    ToolRecognizer toolRecognizer;
    ResourceFinder rf;
    rf.setDefaultContext("toolRecognizer");
    rf.configure(argc,argv);
    return toolRecognizer.runModule(rf);
}

