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

/** 
\defgroup karmaLearn Learning module of the KARMA Experiment
 
@ingroup icub_karma  
 
Machine Learning Module that allows the robot to learn online 
and predict a one-to-one map.

\section intro_sec Description 
This module belongs to the suite of KARMA software and is 
responsible for learning a generic map from \f$ R \f$ to \f$ R \f$. 
 
\section lib_sec Libraries 
- YARP libraries. 
- learningMachine library.

\section parameters_sec Parameters 
--context \e context
- To specify the module's context.
 
--from \e file
- To specify the module's configuration file.
 
\section portsc_sec Ports Created 
- \e /karmaLearn/rpc remote procedure call. \n 
    Recognized remote commands:
    - [train] "item" <in> <out>: issue the module to train
      internal machines with the input-output pair for the given
      item. The reply is [nack]/[ack].
    - [predict] "item" <in>: retrieve the predicted output for
      the specified input. The reply is [nack]/[ack] <out>
      <variance>, where the variance is meaningful only if not
      negative. In case the input command is of the form
      [predict] "item" (<in0> <in1> ...), the prediction is
      returned for the values contained in the list and the
      reply will look like [ack] (<out0> <out1> ...)
      (<variance0> <variance1> ...).
    - [span] "item" <step>: retrieve the predicted output for a
      given range of input specified by the parameter step. The
      reply is [nack]/[ack] (<out0> <out1> ...) (<variance0>
      <variance1> ...). The second parameter can be omitted and
      an internal default step size is used.
    - [optimize] "item" [<step>]|[(<val0> <val1> ...)]: for the
      specified item a search with the given step, or
      alternatively over the given list of input values, is
      performed in order to find out the input that elicits the
      maximum output. The reply is [nack]/[ack] <in> <out>. The
      second parameter can be omitted and an internal default
      step size is used.
    - [items]: retrieve the name of the items currently handled
      by the module.
    - [machine] "item": retrieve the content of the machine
      contained within the database that serves to represent the
      input-output relation of the given item.
    - [clear] "item": remove the item from the internal
      machines database. If no argument is given, clean up the
      whole database content. The reply is [nack]/[ack].
    - [save]: save the content of the internal machines database
      within the configuration file. The reply is [ack].
    - [plot] "item" <step>: stream out a yarp image displaying
      the map learned for the specified item as computed over
      the domain explored with the given step.
 
 - \e /karmaLearn/plot:o streams out images containing the
   learned maps.
 
\section conf_file_sec Configuration Files
The configuration file passed through the option \e --from
looks like as follows:
 
\code 
[general]
name      karmaLearn    // the module's stem-name
num_items 0             // the number of items 
in_lb     0.0           // scaler lower bound in
in_ub     360.0         // scaler upper bound in 
out_lb    0.0           // scaler lower bound out
out_ub    2.0           // scaler upper bound out
\endcode 

Moreover, once some learning has been carried out, the 
configuration file will be filled with the sections 
corresponding to the items. 
 
\section tested_os_sec Tested OS
Windows, Linux

\author Tanis Mar
*/ 

#include <affLearn.h>


using namespace cv;
using namespace std;
using namespace yarp::os;
using namespace yarp::sig;


/************************************************************************/

    /************************************************************************/
    //XXXX create learner


/******************************  Machine learning Routines  **************************************/
bool AffLearn::dataCollector(const string &matName, const yarp::os::Bottle *data)
{
    //Check the number of samples in the bottle
    int sampleN = data->size();
    cout << sampleN << " samples have been received\n\n" << endl;

    for (int sampleI = 0; sampleI < sampleN; sampleI++ ){
        Bottle* sample = data->get(sampleI).asList();
        string label = sample->get(0).asString();  // save the label of the object
        cout << "Sample " << sampleI <<" label is " << label << endl;
        Bottle *sampleDataB = sample->get(1).asList();
        printf("Sample has %i features \n", sampleDataB->size());
        Mat newRow(1,sample->get(1).asList()->size(), CV_32FC1);    //initialize empty  row matrix
        list2mat(sampleDataB, newRow);

        if(this->dataset.find(matName) == this->dataset.end()) // Check if the desired matrix exists already
        {
            cout << "Creating new data mat named: " << matName << endl;        
            dataset[matName]= newRow;
            vector<string> labels;
            labels.push_back(label);
            labelSet[matName] = labels;
            
        } else           //Just add the row to the existing matrix
        {
            cout << "Adding sample to existing matrix: " << matName << endl;
            dataset[matName].push_back(newRow);
            labelSet[matName].push_back(label);
        }
    featsPort.write(*sample);
    }
    //cout << "Mat = "<< endl << " "  << dataset[matName] << endl << endl;
    
    return true;
}


    /************************************************************************/
void AffLearn::train(const std::string &inputMat, const std::string &outputMat)
{
        
}

    /************************************************************************/
void AffLearn::predict(const string &item, const Bottle &input, Bottle &output,
                Bottle &variance)
{
}

/************************************************************************/
    

/************************************************************************/
void AffLearn::save()
{/*
    ofstream fout(configFileName.c_str());

    fout<<"[general]"<<endl;
    fout<<"name      "<<name<<endl;
    fout<<"num_items "<<machines.size()<<endl;
    fout<<"in_lb     "<<scalerIn.getLowerBoundIn()<<endl;
    fout<<"in_ub     "<<scalerIn.getUpperBoundIn()<<endl;
    fout<<"out_lb    "<<scalerOut.getLowerBoundIn()<<endl;
    fout<<"out_ub    "<<scalerOut.getUpperBoundIn()<<endl;
    fout<<endl;

    int i=0;
    for (map<string,IMachineLearner*>::const_iterator itr=machines.begin(); itr!=machines.end(); itr++, i++)
    {
        fout<<"[item_"<<i<<"]"<<endl;
        fout<<"name    "<<itr->first<<endl;
        fout<<"learner "<<("("+string(itr->second->toString().c_str())+")").c_str()<<endl;
        fout<<endl;
    }

    fout.close();
    */
}

    /************************************************************************/
void AffLearn::clear()
{
    /*
    for (map<string,IMachineLearner*>::const_iterator itr=machines.begin(); itr!=machines.end(); itr++)
        delete itr->second;

    machines.clear();
    plotItem="";
    */
}


    /************************************************************************/
bool AffLearn::respond(const Bottle &command, Bottle &reply)
{
    mutex.wait();
    if (command.size()>=1)
    {
        int header=command.get(0).asVocab();
        Bottle payload=command.tail();
        if (header==Vocab::encode("addData")) // command receives "addData" 'matName' <features>
        {
            if (payload.size()>=2)
            {
                string matName = payload.get(0).asString().c_str();
                cout << "Saving data to matrix " << matName << endl << endl;
                Bottle* data = payload.get(1).asList();                
                //cout << "Data: " << data->toString() << endl;
                dataCollector(matName, data);
                reply.addVocab(Vocab::encode("ack"));
            }
        }
        else{
            reply.addVocab(Vocab::encode("nack"));
        }
    }
    mutex.post();
    return true;
}

/**********************************  Utils  *******************************************/

void AffLearn::list2vector(const Bottle* bot, std::vector<double>& vec)
{
    vec.clear();
    for (int i = 0; i< bot->size(); i++ ){
        vec.push_back(bot->get(i).asDouble());        
    }
}

void AffLearn::list2mat(const Bottle* bot, cv::Mat& mat)
{
    
    mat.release();
    for (int i = 0; i< bot->size(); i++ ){
        mat.push_back(bot->get(i).asDouble());        
    }
    mat = mat.t();
    cout << "Creating row from list: " << mat << endl;
}


/*****************************  Module functions  *******************************************/
bool AffLearn::configure(ResourceFinder &rf)
{
    name = "affLearn";
    plotPort.open(("/"+name+"/plot:o").c_str());
    featsPort.open(("/"+name+"/feats:o").c_str());
    rpcPort.open(("/"+name+"/rpc:i").c_str());
    attach(rpcPort);
    return true;
}

/************************************************************************/
bool  AffLearn::interruptModule()
{
    plotPort.interrupt();
    rpcPort.interrupt();
    featsPort.interrupt();
    return true;
}

/************************************************************************/
bool  AffLearn::close()
{
    save();
    clear();
    featsPort.close();
    plotPort.close();
    rpcPort.close();
    return true;
}

/************************************************************************/
double  AffLearn::getPeriod()
{
    return 0.25;
}

/************************************************************************/
bool  AffLearn::updateModule()
{
    return true;
}


/************************************************************************/
int main(int argc, char *argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
    {
        printf("YARP server not available!\n");
        return -1;
    }

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("affLearn");
    rf.setDefaultConfigFile("affLearn.ini");
    rf.configure(argc,argv);

    AffLearn affLearn;
    return affLearn.runModule(rf);
}



