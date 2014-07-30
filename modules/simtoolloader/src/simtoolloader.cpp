 /*
  * Copyright (C) 2013 RobotCub Consortium, European Commission FP6 Project IST-004370
  * Author:  Afonso Gonçalves
  * email:   agoncalves@isr.ist.utl.pt
  * website: www.robotcub.org
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

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "simtoolloader.h"

using namespace std;
using namespace yarp::os;
//using namespace yarp::sig;

double SimToolLoaderModule::getPeriod() {

    return 1;
}

bool   SimToolLoaderModule::updateModule() {

    return true;
}

bool   SimToolLoaderModule::configure(yarp::os::ResourceFinder &rf) {

    Bottle table;
    Bottle temp;
    char objectName[] = {'o', 'b', 'j', '0', '\0'};

    /* module name */
    moduleName = rf.check("name", Value("simobjloader"),
                          "Module name (string)").asString();

    setName(moduleName.c_str());

    /* port names */
    simToolLoaderSimOutputPortName  = "/";
    simToolLoaderSimOutputPortName += getName( rf.check("simToolLoaderSimOutputPort",
                                     Value("/sim:o"),
                                     "Loader output port(string)")
                                     .asString() );

    simToolLoaderCmdInputPortName  = "/";
    simToolLoaderCmdInputPortName += getName( rf.check("simToolLoaderCmdInputPort",
                                     Value("/cmd:i"),
                                     "Loader input port(string)")
                                     .asString() );

    /* open ports */
    if (!simToolLoaderSimOutputPort.open(
            simToolLoaderSimOutputPortName.c_str())) {

        cout << getName() << ": unable to open port"
        << simToolLoaderSimOutputPortName << endl;
        return false;
    }

    if (!simToolLoaderCmdInputPort.open(
            simToolLoaderCmdInputPortName.c_str())) {

        cout << getName() << ": unable to open port"
        << simToolLoaderCmdInputPortName << endl;
        return false;
    }

    /* Rate thread period */
    threadPeriod = rf.check("threadPeriod", Value(0.5),
        "Control rate thread period key value(double) in seconds ").asDouble();

    /* Read Table Configuration */
    table = rf.findGroup("table");


    /* Read the Objects configurations */
    numberObjs  = rf.findGroup("objects").find("number").asInt();
    vector<Bottle> object;
    for ( int n=0 ; n<numberObjs ; n++ ) {
        objectName[3]++;
        //cout << objectName;
        temp = rf.findGroup("objects").findGroup(objectName);
        object.push_back(temp);
        temp.clear();
    }

    /* Create the control rate thread */
    ctrlThread = new CtrlThread(&simToolLoaderSimOutputPort,
                                &simToolLoaderCmdInputPort,
                                threadPeriod,
                                table, object);
    /* Starts the thread */
    if (!ctrlThread->start()) {
        delete ctrlThread;
        return false;
    }

    return true;
}

bool   SimToolLoaderModule::interruptModule() {

    cout << "Interrupting your module, for port cleanup" << endl;

    simToolLoaderSimOutputPort.interrupt();
    simToolLoaderCmdInputPort.interrupt();

    return true;
}

bool   SimToolLoaderModule::close() {

    /* optional, close port explicitly */
    cout << "Calling close function\n";

    ctrlThread->stop();
    delete ctrlThread;
    simToolLoaderSimOutputPort.close();
    simToolLoaderCmdInputPort.close();

    return true;
}

CtrlThread::CtrlThread(RpcClient *simToolLoaderSimOutput,
                       Port *simToolLoaderCmdInput,
                       const double period, const Bottle table,
                       vector<Bottle> object)
                       :RateThread(int(period*1000.0)) {

    simToolLoaderSimOutputPort = simToolLoaderSimOutput;
    simToolLoaderCmdInputPort  = simToolLoaderCmdInput;
    threadTable  = table;
    threadObject = object;
}

bool CtrlThread::threadInit() {

    cout << endl << "thread initialization" << endl;

    // Make a connection to the iCub Simulator world port:
    (*simToolLoaderSimOutputPort).addOutput("/icubSim/world");

    simWorld = SimWorld(threadTable, threadObject);

    srand (time(NULL));

    Bottle simCmd;
    simCmd.addString("world");
    simCmd.addString("set");
    simCmd.addString("mdir");

    //const ConstString icubRootEnvPath = yarp::os::getenv("ICUB_ROOT");
    //const ConstString localModelsPath = "/contrib/src/poeticon/poeticonpp/simobjloader/models";

    const ConstString icubContribEnvPath = yarp::os::getenv("ICUBcontrib_DIR");
    const ConstString localModelsPath    = "/share/ICUBcontrib/contexts/simobjloader/models";
    const ConstString modelsPath         = icubContribEnvPath + localModelsPath;
    simCmd.addString(modelsPath);
    writeSim(simCmd);


    // Cartesian Controller Interface
	Property optCart;
	optCart.put("device","cartesiancontrollerclient");
    optCart.put("remote","/icubSim/cartesianController/right_arm");
    optCart.put("local","/simobjloader/cartesian_client/right_arm");
	if (!clientCart.open(optCart))
		return false;    
    clientCart.view(icart);	// open the view	

    cout << endl << "thread initialized" << endl;
    return true;
}

void CtrlThread::threadRelease() {

    cout << endl << "thread release" << endl;
}

void CtrlThread::run() {

    Bottle controlCmd;
    Bottle simCmd;
    int objIndex  = 0;
    int toolIndex = 0;
    NetInt32 code;


    if( (*simToolLoaderCmdInputPort).read(controlCmd, true) ) {
        code = Vocab::encode((controlCmd.get(0)).asString());

        cout << (controlCmd.get(0)).asString() << endl;

        switch(code) {
            case VOCAB3('d','e','l'):
                cout << "Clear world" <<endl;

                //Clear the World:
                simCmd = simWorld.deleteAll();
                writeSim(simCmd);

                //reply to the control manager
                controlCmd.clear();
                controlCmd.addVocab(code);
                replyCmd(controlCmd);

                break;

            case VOCAB4('t','o','o','l'):
                cout << "Create table and tool (on the table)." <<endl;

                //Clear the World:
                simCmd = simWorld.deleteAll();
                writeSim(simCmd);

                //Create the table:
                //simWorld.simTable->objSubIndex = 1;  //needs to be automatic, each type of object has his own subIndex
                simCmd = simWorld.simTable->makeObjectBottle(simWorld.objSubIndex, false);
                writeSim(simCmd);

                //Create the tool on the table:
                toolIndex = controlCmd.get(1).asInt();
                if ( toolIndex==0 ) {
                    toolIndex = rand() % simWorld.simObject.size() + 1;
                }
                //simWorld.simObject[toolIndex-1]->objSubIndex = 1;  //needs to be automatic, each type of object has his own subIndex
                simWorld.simObject[toolIndex-1]->setObjectPosition(
                                        threadTable.get(4).asDouble()+0.05,
                                        threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                        0.35);
                simCmd = simWorld.simObject[toolIndex-1]->makeObjectBottle(simWorld.objSubIndex);
                writeSim(simCmd);
                simWorld.simObject[toolIndex-1]->setObjectRotation(0, -50, 0);
                simCmd = simWorld.simObject[toolIndex-1]->rotateObjectBottle();
                writeSim(simCmd);

                //reply to the control manager the tool ID
                controlCmd.clear();
                controlCmd.addVocab(code);
                controlCmd.addInt(toolIndex);
                replyCmd(controlCmd);

                break;

            case VOCAB3('o','b','j'):

                cout << "Create the tool in the hand, the objects on the table." <<endl;

                //Clear the World:
                simCmd = simWorld.deleteAll();
                writeSim(simCmd);

                //Create the table:
                //simWorld.simTable->objSubIndex = 1;  //needs to be automatic, each type of object has his own subIndex
                simCmd = simWorld.simTable->makeObjectBottle(simWorld.objSubIndex, false);
                writeSim(simCmd);

                //Create one tool in the hand:
                toolIndex = controlCmd.get(1).asInt();
                if ( toolIndex==0 ) {
                    toolIndex = rand() % simWorld.simObject.size() + 1;
                }
                //simWorld.simObject[toolIndex-1]->objSubIndex = 1;  //needs to be automatic, each type of object has his own subIndex
                yarp::sig::Vector pos, orRad, orGrad;
                icart->getPose(pos, orRad);
                //orGrad[0] = orRad[0]*(180/3.1416);
                //orGrad[1] = orRad[1]*(180/3.1416);
                //orGrad[2] = orRad[2]*(180/3.1416);
                printf("position of the hand is %s \n", pos.toString().c_str());
                printf("orientation of the hand is %s \n",orRad.toString().c_str());
                pos[0]+=0.05;
                simWorld.simObject[toolIndex-1]->setObjectPosition(pos[0],pos[1],pos[2]);//(0.23, 0.70, 0.20);    //left arm end effector position
                simCmd = simWorld.simObject[toolIndex-1]->makeObjectBottle(simWorld.objSubIndex);
                writeSim(simCmd);
                //simWorld.simObject[toolIndex-1]->setObjectRotation(70, 120, 30);

                simWorld.simObject[toolIndex-1]->setObjectRotation(-65, -5, 110);
                simCmd = simWorld.simObject[toolIndex-1]->rotateObjectBottle();
                writeSim(simCmd);
                simCmd = simWorld.simObject[toolIndex-1]->grabObjectBottle(RIGHT);        //left arm by default
                writeSim(simCmd);

                //Create one object:
                objIndex = controlCmd.get(2).asInt();
                if ( objIndex==0 ) {
                    objIndex = rand() % simWorld.simObject.size() + 1;
                }
                //simWorld.simObject[objIndex-1]->objSubIndex = 1;  //needs to be automatic, each type of object has his own subIndex
                simWorld.simObject[objIndex-1]->setObjectPosition(
                                        threadTable.get(4).asDouble()+0.1,
                                        threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                        0.45);
                simCmd = simWorld.simObject[objIndex-1]->makeObjectBottle(simWorld.objSubIndex);
                writeSim(simCmd);

                //reply to the control manager the tool and object IDs
                controlCmd.clear();
                controlCmd.addVocab(code);
                controlCmd.addInt(toolIndex);
                controlCmd.addInt(objIndex);
                replyCmd(controlCmd);

                break;

        }
    }

}

void CtrlThread::writeSim(Bottle cmd) {

    Bottle response;
    (*simToolLoaderSimOutputPort).write(cmd,response);
}

void CtrlThread::replyCmd(Bottle cmd) {

    (*simToolLoaderCmdInputPort).reply(cmd);
}

void SimObject::setObjectPosition(double posx, double posy,  double posz) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;
}

void SimObject::setObjectRotation(double rotx, double roty,  double rotz) {

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;
}

void SimObject::setObjectColor(double red,  double green, double blue) {

    colorR = red;
    colorG = green;
    colorB = blue;
}

SimBox::SimBox(double posx, double posy,  double posz,
               double rotx, double roty,  double rotz,
               double red,  double green, double blue,
               double sizx, double sizy,  double sizz) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    colorR = red;
    colorG = green;
    colorB = blue;

    sizeX = sizx;
    sizeY = sizy;
    sizeZ = sizz;
}

Bottle SimBox::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("box");
    cmd.addDouble(sizeX);
    cmd.addDouble(sizeY);
    cmd.addDouble(sizeZ);
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    cmd.addDouble(colorR);
    cmd.addDouble(colorG);
    cmd.addDouble(colorB);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[BOX]++;
    objSubIndex=ind[BOX];

    return cmd;
}

Bottle SimBox::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("box");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimBox::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("box");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

Bottle SimBox::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

SimSBox::SimSBox(double posx, double posy,  double posz,
                 double rotx, double roty,  double rotz,
                 double red,  double green, double blue,
                 double sizx, double sizy,  double sizz) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    colorR = red;
    colorG = green;
    colorB = blue;

    sizeX = sizx;
    sizeY = sizy;
    sizeZ = sizz;
}

Bottle SimSBox::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("sbox");
    cmd.addDouble(sizeX);
    cmd.addDouble(sizeY);
    cmd.addDouble(sizeZ);
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    cmd.addDouble(colorR);
    cmd.addDouble(colorG);
    cmd.addDouble(colorB);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[SBOX]++;
    objSubIndex=ind[SBOX];

    return cmd;
}

Bottle SimSBox::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

Bottle SimSBox::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("sbox");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimSBox::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("sbox");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

SimSph::SimSph(double posx, double posy,  double posz,
               double rotx, double roty,  double rotz,
               double red,  double green, double blue,
               double rad) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    colorR = red;
    colorG = green;
    colorB = blue;

    radius = rad;
}

Bottle SimSph::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("sph");
    cmd.addDouble(radius);
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    cmd.addDouble(colorR);
    cmd.addDouble(colorG);
    cmd.addDouble(colorB);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[SPH]++;
    objSubIndex=ind[SPH];

    return cmd;
}

Bottle SimSph::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

Bottle SimSph::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("sph");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimSph::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("sph");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

SimSSph::SimSSph(double posx, double posy,  double posz,
                 double rotx, double roty,  double rotz,
                 double red,  double green, double blue,
                 double rad) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    colorR = red;
    colorG = green;
    colorB = blue;

    radius = rad;
}

Bottle SimSSph::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("ssph");
    cmd.addDouble(radius);
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    cmd.addDouble(colorR);
    cmd.addDouble(colorG);
    cmd.addDouble(colorB);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[SSPH]++;
    objSubIndex=ind[SSPH];

    return cmd;
}

Bottle SimSSph::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

Bottle SimSSph::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("ssph");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimSSph::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("sshp");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

SimCyl::SimCyl(double posx, double posy, double posz,
               double rotx, double roty, double rotz,
               double red, double green, double blue,
               double rad, double hei) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    colorR = red;
    colorG = green;
    colorB = blue;

    radius = rad;
    height = hei;
}

Bottle SimCyl::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("cyl");
    cmd.addDouble(radius);
    cmd.addDouble(height);
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    cmd.addDouble(colorR);
    cmd.addDouble(colorG);
    cmd.addDouble(colorB);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[CYL]++;
    objSubIndex=ind[CYL];

    return cmd;
}

Bottle SimCyl::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

Bottle SimCyl::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("cyl");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimCyl::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("cyl");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

SimSCyl::SimSCyl(double posx, double posy,  double posz,
                 double rotx, double roty,  double rotz,
                 double red,  double green, double blue,
                 double rad,  double hei) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    colorR = red;
    colorG = green;
    colorB = blue;

    radius = rad;
    height = hei;
}

Bottle SimSCyl::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("scyl");
    cmd.addDouble(radius);
    cmd.addDouble(height);
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    cmd.addDouble(colorR);
    cmd.addDouble(colorG);
    cmd.addDouble(colorB);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[SCYL]++;
    objSubIndex=ind[SCYL];

    return cmd;
}

Bottle SimSCyl::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

Bottle SimSCyl::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("scyl");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimSCyl::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("scyl");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

SimModel::SimModel(double posx, double posy, double posz,
                   double rotx, double roty, double rotz,
                   ConstString mes, ConstString tex) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    mesh    = mes;
    texture = tex;
}

Bottle SimModel::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("model");
    cmd.addString(mesh.c_str());
    cmd.addString(texture.c_str());
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[MODEL]++;
    objSubIndex=ind[MODEL];

    return cmd;
}

Bottle SimModel::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

Bottle SimModel::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("model");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimModel::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("model");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

SimSModel::SimSModel(double posx, double posy, double posz,
                     double rotx, double roty, double rotz,
                     ConstString mes, ConstString tex) {

    positionX = posx;
    positionY = posy;
    positionZ = posz;

    rotationX = rotx;
    rotationY = roty;
    rotationZ = rotz;

    mesh    = mes;
    texture = tex;
}

Bottle SimSModel::makeObjectBottle(vector<int>& ind, bool collision) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("mk");
    cmd.addString("smodel");
    cmd.addString(mesh.c_str());
    cmd.addString(texture.c_str());
    cmd.addDouble(positionX);
    cmd.addDouble(positionY);
    cmd.addDouble(positionZ);
    if (collision == false) {
        cmd.addString("false");
    }

    ind[SMODEL]++;
    objSubIndex=ind[SMODEL];

    return cmd;
}

Bottle SimSModel::deleteObject() {
    //not done yet, needs to delete every object and then recreate every one except the one to be deleted
    Bottle cmd;

    return cmd;
}

Bottle SimSModel::rotateObjectBottle() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("rot");
    cmd.addString("smodel");
    cmd.addInt   (objSubIndex);
    cmd.addDouble(rotationX);
    cmd.addDouble(rotationY);
    cmd.addDouble(rotationZ);
    return cmd;
}

Bottle SimSModel::grabObjectBottle(iCubArm arm) {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("grab");
    cmd.addString("smodel");
    cmd.addInt   (objSubIndex);
    switch(arm) {
        case RIGHT:
            cmd.addString("right");
            break;
        case LEFT:
            cmd.addString("left");
            break;
        default:
            cmd.addString("right");
    }
    cmd.addInt(1);
    return cmd;
}

SimWorld::SimWorld() {

}

SimWorld::SimWorld(const Bottle& threadTable,
                   vector<Bottle>& threadObject) {

    simObject.resize(threadObject.size());
    objSubIndex.resize(8);
    for ( int n=0 ; n<8 ; n++ ) {
        objSubIndex[n]=0;
    }

    simTable = new SimSBox(threadTable.get(4).asDouble(),
                           threadTable.get(5).asDouble(),
                           threadTable.get(6).asDouble(),
                           0, 0, 0,
                           threadTable.get(7).asDouble(),
                           threadTable.get(8).asDouble(),
                           threadTable.get(9).asDouble(),
                           threadTable.get(1).asDouble(),
                           threadTable.get(2).asDouble(),
                           threadTable.get(3).asDouble());
    simTable->objSubIndex=0;

    for ( int n=0 ; n<threadObject.size() ; n++ ) {
        if      (threadObject[n].get(1).asString() == "Box") {
            simObject[n] = new SimBox(threadTable.get(4).asDouble()-0.05,
                                      threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                      threadTable.get(6).asDouble()-0.01,
                                      0, 0, 0,
                                      threadObject[n].get(5).asDouble(),
                                      threadObject[n].get(6).asDouble(),
                                      threadObject[n].get(7).asDouble(),
                                      threadObject[n].get(2).asDouble(),
                                      threadObject[n].get(3).asDouble(),
                                      threadObject[n].get(4).asDouble());
        }
        else if (threadObject[n].get(1).asString() == "Sph") {
            simObject[n] = new SimSph(threadTable.get(4).asDouble()-0.05,
                                      threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                      threadTable.get(6).asDouble()-0.01,
                                      0, 0, 0,
                                      threadObject[n].get(3).asDouble(),
                                      threadObject[n].get(4).asDouble(),
                                      threadObject[n].get(5).asDouble(),
                                      threadObject[n].get(2).asDouble());
        }
        else if (threadObject[n].get(1).asString() == "Cyl") {
            simObject[n] = new SimCyl(threadTable.get(4).asDouble()-0.05,
                                      threadTable.get(5).asDouble()+0.2,
                                      threadTable.get(6).asDouble()-0.01,
                                      0, 0, 0,
                                      threadObject[n].get(4).asDouble(),
                                      threadObject[n].get(5).asDouble(),
                                      threadObject[n].get(6).asDouble(),
                                      threadObject[n].get(2).asDouble(),
                                      threadObject[n].get(3).asDouble());
        }
        else if (threadObject[n].get(1).asString() == "Model") {
            simObject[n] = new SimModel(threadTable.get(4).asDouble()-0.05,
                                        threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                        threadTable.get(6).asDouble()-0.01,
                                        0, 0, 0,
                                        threadObject[n].get(2).asString(),
                                        threadObject[n].get(3).asString());
        }
/*        else if (threadObject[n].get(1).asString() == "SBox") {
            simObject[n] = new SimSBox(threadTable.get(4).asDouble()-0.05,
                                       threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                       threadTable.get(6).asDouble()-0.01,
                                       0, 0, 0,
                                       threadObject[n].get(5).asDouble(),
                                       threadObject[n].get(6).asDouble(),
                                       threadObject[n].get(7).asDouble(),
                                       threadObject[n].get(2).asDouble(),
                                       threadObject[n].get(3).asDouble(),
                                       threadObject[n].get(4).asDouble());
        }
        else if (threadObject[n].get(1).asString() == "SSph") {
            simObject[n] = new SimSSph(threadTable.get(4).asDouble()-0.05,
                                       threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                       threadTable.get(6).asDouble()-0.01,
                                       0, 0, 0,
                                       threadObject[n].get(3).asDouble(),
                                       threadObject[n].get(4).asDouble(),
                                       threadObject[n].get(5).asDouble(),
                                       threadObject[n].get(2).asDouble());
        }
        else if (threadObject[n].get(1).asString() == "SCyl") {
            simObject[n] = new SimSCyl(threadTable.get(4).asDouble()-0.05,
                                       threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                       threadTable.get(6).asDouble()-0.01,
                                       0, 0, 0,
                                       threadObject[n].get(4).asDouble(),
                                       threadObject[n].get(5).asDouble(),
                                       threadObject[n].get(6).asDouble(),
                                       threadObject[n].get(2).asDouble(),
                                       threadObject[n].get(3).asDouble());
        }
        else if (threadObject[n].get(1).asString() == "SModel") {
            simObject[n] = new SimSModel(threadTable.get(4).asDouble()-0.05,
                                         threadTable.get(5).asDouble()+((threadTable.get(2).asDouble())/2)+0.1,
                                         threadTable.get(6).asDouble()-0.01,
                                         0, 0, 0,
                                         threadObject[n].get(2).asString(),
                                       threadObject[n].get(3).asString());
        }*/
        simObject[n]->objSubIndex=0;
    }
}

Bottle SimWorld::deleteAll() {

    Bottle cmd;
    cmd.addString("world");
    cmd.addString("del");
    cmd.addString("all");
    resetSimObjectIndex();
    return cmd;
}

void SimWorld::resetSimObjectIndex() {
    simTable->objSubIndex=0;
    for ( int n=0 ; n<simObject.size() ; n++ ) {
        simObject[n]->objSubIndex=0;
    }
    for ( int n=0 ; n<8 ; n++ ) {
        objSubIndex[n]=0;
    }
}
