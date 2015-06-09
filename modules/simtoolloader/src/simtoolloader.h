 /*
  * Copyright (C) 2013 RobotCub Consortium, European Commission FP6 Project IST-004370 / iCub Facility
  * Author:  Afonso Gonçalves, Tanis Mar
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

#ifndef SIMTOOLLOADERMODULE_H
#define SIMTOOLLOADERMODULE_H



#include <string>
#include <vector>
#include <yarp/os/all.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/Math.h>
#include <yarp/math/SVD.h>

#include <iCub/ctrl/math.h>
#include <iCub/ctrl/pids.h>

#include <gsl/gsl_math.h>

//for iCart and iGaze
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/CartesianControl.h>

YARP_DECLARE_DEVICES(icubmod)

using namespace std;
using namespace yarp::os;

enum iCubArm {LEFT, RIGHT};
enum objType {BOX, SBOX, SPH, SSPH, CYL, SCYL, MODEL, SMODEL};

class SimObject {
protected:
    double positionX;
    double positionY;
    double positionZ;

    double rotationX;
    double rotationY;
    double rotationZ;

    double colorR;
    double colorG;
    double colorB;

public:
    int    objSubIndex;

    void setObjectPosition(double posx, double posy,  double posz);
    void setObjectRotation(double rotx, double roty,  double rotz);
    void setObjectColor   (double red,  double green, double blue);

    virtual Bottle   makeObjectBottle(vector<int>& ind, bool collision = true)  = 0;
    virtual Bottle   grabObjectBottle(iCubArm arm)                     = 0;
    virtual Bottle   releaseObjectBottle(iCubArm arm)                  = 0;
    virtual Bottle   rotateObjectBottle()                              = 0;
    virtual Bottle   moveObjectBottle()	                               = 0;
    virtual Bottle   deleteObject()                                    = 0; //XXX no implementation yet
    virtual string   getObjName()                                      = 0;
};

class SimBox: public SimObject {
private:
    double sizeX;
    double sizeY;
    double sizeZ;

public:
    SimBox(double posx, double posy, double posz,
           double rotx, double roty, double rotz,
           double red, double green, double blue,
           double sizx, double sizy, double sizz);
    virtual Bottle   makeObjectBottle(vector<int>& ind, bool collision = true);
    virtual Bottle   deleteObject();
    virtual Bottle   rotateObjectBottle();
    virtual Bottle   moveObjectBottle();
    virtual Bottle   grabObjectBottle(iCubArm arm);
    virtual Bottle   releaseObjectBottle(iCubArm arm);
    string           getObjName();

};

class SimSBox: public SimObject {
private:
    double sizeX;
    double sizeY;
    double sizeZ;

public:
    SimSBox(double posx, double posy, double posz,
            double rotx, double roty, double rotz,
            double red, double green, double blue,
            double sizx, double sizy, double sizz);
    virtual Bottle   makeObjectBottle(vector<int>& ind, bool collision = true);
    virtual Bottle   deleteObject();
    virtual Bottle   rotateObjectBottle();
    virtual Bottle   moveObjectBottle();
    virtual Bottle   grabObjectBottle(iCubArm arm);
    virtual Bottle   releaseObjectBottle(iCubArm arm);
    string      getObjName();
};

class SimSph: public SimObject {
private:
    double radius;

public:
    SimSph(double posx, double posy,  double posz,
           double rotx, double roty,  double rotz,
           double red,  double green, double blue,
           double rad);
    virtual Bottle   makeObjectBottle(vector<int>& ind, bool collision = true);
    virtual Bottle   deleteObject();
    virtual Bottle   rotateObjectBottle();
    virtual Bottle   moveObjectBottle();
    virtual Bottle   grabObjectBottle(iCubArm arm);
    virtual Bottle   releaseObjectBottle(iCubArm arm);
    string      getObjName();
};

class SimCyl: public SimObject {
private:
    double radius;
    double height;

public:
    SimCyl(double posx, double posy,  double posz,
           double rotx, double roty,  double rotz,
           double red,  double green, double blue,
           double rad,  double hei);
    virtual Bottle   makeObjectBottle(vector<int>& ind, bool collision = true);
    virtual Bottle   deleteObject();
    virtual Bottle   rotateObjectBottle();
    virtual Bottle   moveObjectBottle();
    virtual Bottle   grabObjectBottle(iCubArm arm);
    virtual Bottle   releaseObjectBottle(iCubArm arm);
    string           getObjName();
};

class SimModel: public SimObject {
private:
    ConstString mesh;
    ConstString texture;

public:
    SimModel(double posx, double posy, double posz,
             double rotx, double roty, double rotz,
             ConstString mes, ConstString tex);
    virtual Bottle   makeObjectBottle(vector<int>& ind, bool collision = false);
    virtual Bottle   deleteObject();
    virtual Bottle   rotateObjectBottle();
    virtual Bottle   moveObjectBottle();
    virtual Bottle   grabObjectBottle(iCubArm arm);
    virtual Bottle   releaseObjectBottle(iCubArm arm);
    string           getObjName();
};


class SimWorld {
protected:

public:
    SimWorld();
    SimWorld(const Bottle& threadTable, vector<Bottle>& threadObject);
    Bottle deleteAll();
    void resetSimObjectIndex();

    SimObject *simTable;
    vector<SimObject*> simObject;
    vector<int> objSubIndex;
};

class CtrlThread:public RateThread {
protected:

    Port      *simToolLoaderCmdInputPort;
    RpcClient *simToolLoaderSimOutputPort;

    Bottle threadTable;
    vector<Bottle> threadObject;
    string threadHand;
    SimWorld simWorld;
    int objIndex;
    int toolIndex;

    yarp::dev::PolyDriver					clientCart;
	yarp::dev::ICartesianControl			*icart;

public:
    CtrlThread(RpcClient *simToolLoaderSimOutput,
               Port *simToolLoaderCmdInput,
               const double period,
               const Bottle table,
               vector<Bottle> object,
               string hand);
    bool threadInit();
    void threadRelease();
    void run();
    void writeSim(Bottle cmd);
    void replyCmd(Bottle cmd);
};

class SimToolLoaderModule: public RFModule {
    string          moduleName;
    string          hand;

    string          simToolLoaderSimOutputPortName;
    RpcClient       simToolLoaderSimOutputPort;

    string          simToolLoaderCmdInputPortName;
    Port            simToolLoaderCmdInputPort;

    int             threadPeriod;
    int             numberObjs;

    CtrlThread      *ctrlThread;

public:
    double getPeriod();
    bool configure(yarp::os::ResourceFinder &rf);
    bool updateModule();
    bool interruptModule();
    bool close();
};

#endif /* SIMOBJLOADERMODULE_H */
