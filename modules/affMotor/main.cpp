/* 
 * Copyright (C) 2012 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini
 * email:  ugo.pattacini@iit.it
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
\defgroup affMotor Motor Part for tool affordances experiments
 
Motor Control Module that allows the robot to push/slide//draw the
object and explore a tool. 

\section intro_sec Description 
This module aims to control the robot hands in order to properly
execute a set of actions aiming at displacing an object on the table
 to then learn the corresponding affordances. \n
It also enables the tool exploration.
 
\section lib_sec Libraries 
- YARP libraries. 

\section parameters_sec Parameters 
--robot \e robot
- Select the robot to connect to.

--name \e name
- Select the stem-name of the module used to open up ports. 
  By default \e name is <i>affMotor</i>.
 
--elbow_set <i>(<height> <weight>)</i>
- To specify how to weigh the task to keep the elbow high.
 
\section portsa_sec Ports Accessed
Assume that iCubInterface (with ICartesianControl interface
implemented) is running. 
 
\section portsc_sec Ports Created 
- \e /affMotor/rpc receives the information to execute the
  motor action as a Bottle. It manages the following commands:

  -# <b>Push</b>: <i>[push] cx cy cz theta radius</i>. \n
  The coordinates <i>(cx,cy,cz)</i> represent in meters the
  position of the object's centroid to be pushed; <i>theta</i>,
  given in degrees, and <i>radius</i>, specified in meters,
  account for the point from which push the object, that is
  located onto the circle centered in <i>(cx,cy,cz)</i> and
  contained in the x-y plane. \n
  The reply <i>[ack]</i> is returned as soon as the push is
  accomplished.

  -# <b>Draw / Pull</b>: <i>[draw]/[pull] cx cy cz theta radius dist</i>. \n
  The coordinates <i>(cx,cy,cz)</i> represent in meters the
  position of the object's centroid to be drawn closer;
  <i>theta</i>, given in degrees, and <i>radius</i>, specified
  in meters, account for the point from which draw the object,
  that is located onto the circle centered in <i>(cx,cy,cz)</i>
  and contained in the x-y plane. The parameter <i>dist</i>
  specifies the length in meters of the draw action. \n
  The reply <i>[ack] val</i> is returned at the end of the
  simulation, where <i>val</i> accounts for the quality of the
  action: the lower it is the better the action is.

  -# <b>Virtual draw / pull</b>: <i>[vdra]/[vpul] cx cy cz theta radius
   dist</i>. \n Simulate the draw without performing any
   movement in order to test the quality of the action. \n
   The reply <i>[ack] val</i> is returned at the end of the
   simulation, where <i>val</i> accounts for the quality of the
   action: the lower it is the better the action is.

  -# <b>Slide</b>: <i>[slide] cx cy cz theta radius</i>. \n
  Perfoms a slide action accross the circle centered
  at target object coordinates <i>(cx,cy,cz)</i>, in meters, from
  the point defined by <i>theta</i>,in degrees, and <i>radius</i>,
  in meters, to the diametral oppossite accross the cirlce,
  contained in the x-y plane. The action consist on a straight
  and uniform slide from the point specified by the circle's
  radius and theta angle to the opposite point of the circle,
  accross the center point where the object is located. The reply
  <i>[ack] val</i> is returned at the end of the simulation,
  where <i>val</i> accounts for the quality of the action: the lower
  it is the better the action is.

  -# <b>Virtual slide</b>: <i>[vsli] cx cy cz theta radius</i>. \n
   Simulate the slide without performing any movement in order to
   test the quality of the action. \n
   The reply <i>[ack] val</i> is returned at the end of the
   simulation, where <i>val</i> accounts for the quality of the
   action: the lower it is the better the action is.

  -# <b>Drag</b>: <i>[drag] cx cy cz theta radius</i>. \n
  Perfoms a drag action from the object's position to the circle
  of radius <i>radius</i> in the direction of angle <i>theta</i>.
  Target object coordinates <i>(cx,cy,cz)</i> and <i>radius</i>
  are in meters,while <i>theta</i> in degrees.The reply
  <i>[ack] val</i> is returned at the end of the simulation,
  where <i>val</i> accounts for the quality of the action: the lower
  it is the better the action is.

  -# <b>Virtual drag</b>: <i>[vdrg] cx cy cz theta radius</i>. \n
   Simulate the drag without performing any movement in order to
   test the quality of the action. \n
   The reply <i>[ack] val</i> is returned at the end of the
   simulation, where <i>val</i> accounts for the quality of the
   action: the lower it is the better the action is.

  -# <b>Tool-attach</b>: <i>[tool] [attach] arm x y z</i>. \n
  Attach a tool to the given arm whose dimensions are specified
  in the frame attached to the hand. The subsequent action will
  make use of this tool.

  -# <b>Tool-get</b>: <i>[tool] [get]</i>. \n
  Retrieve tool information as <i>[ack] arm x y z</i>.

  -# <b>Tool-remove</b>: <i>[tool] [remove]</i>. \n
  Remove the attached tool.

  -# <b>Find</b>: <i>[find] arm eye</i>. \n
  An exploration is performed which aims at finding the tool
  dimension. It is possible to select the arm for executing the
  movement as well as the eye from which the motion is observed.
  The reply <i>[ack] x y z</i> returns the tool's dimensions
  with respect to reference frame attached to the robot hand.
 
- \e /affMotor/stop:i receives request for immediate stop of
  any ongoing processing.
 
- \e /affMotor/vision:i receives the information about the
  pixel corresponding to the tool tip during the tool
  exploration phase.
 
- \e /affMotor/finder:rpc communicates with the module in
  charge of solving for the tool's dimensions.
 
\section tested_os_sec Tested OS
Windows, Linux

\author Ugo Pattacini
*/ 

#include <stdio.h>
#include <string>
#include <algorithm>

#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>

#include <iCub/ctrl/math.h>

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::math;
using namespace iCub::ctrl;


/************************************************************************/
class AffMotor: public RFModule, public PortReader
{
protected:
    PolyDriver driverG;
    PolyDriver driverL;
    PolyDriver driverR;
    PolyDriver driverHL;
    PolyDriver driverHR;

    IGazeControl      *iGaze;
    ICartesianControl *iCartCtrlL;
    ICartesianControl *iCartCtrlR;
    ICartesianControl *iCartCtrl;

    string pushHand;
    Matrix toolFrame;

    string handUsed;
    bool interrupting;
    double flip_hand;
    int shake_joint;

    bool elbow_set;
    double elbow_height,elbow_weight;

    double collisionThresh;

    BufferedPort<Bottle> visionPort;
    RpcClient            finderPort;
    RpcServer            rpcPort;
    Port                 stopPort;

    /************************************************************************/
    double dist(const Matrix &M)
    {
        double ret=0.0;
        for (int r=0; r<M.rows(); r++)
            for (int c=0; c<M.cols(); c++)
                ret+=M(r,c)*M(r,c);

        return sqrt(ret);
    }

    /************************************************************************/
    bool read(ConnectionReader &connection)
    {
        Bottle cmd; cmd.read(connection);
        interruptModule();
        return true;
    }

    /************************************************************************/
    bool respond(const Bottle &command, Bottle &reply)
    {
        int ack=Vocab::encode("ack");
        int nack=Vocab::encode("nack");

        //bool ok;

        int cmd=command.get(0).asVocab();
        switch (cmd)
        {
            //-----------------
            case VOCAB4('p','u','s','h'):
            {
                Bottle payload=command.tail();
                if (payload.size()>=5)
                {
                    Vector c(3);
                    double theta;
                    double radius;

                    c[0]=payload.get(0).asDouble();
                    c[1]=payload.get(1).asDouble();
                    c[2]=payload.get(2).asDouble();
                    theta=payload.get(3).asDouble();
                    radius=payload.get(4).asDouble();

                    bool ok = push(c,theta,radius,pushHand,toolFrame);
                    if (ok)
                        reply.addVocab(ack);
                    else
                        reply.addVocab(nack);
                }else {

                    reply.addVocab(nack);
                    reply.addString("Not enough amount of parameters for push action");
                }

                break;
            }

            //-----------------
            case VOCAB4('v','d','r','g'):
            case VOCAB4('d','r','a','g'):
            {
                Bottle payload=command.tail();
                if (payload.size()>=5)
                {
                    Vector c(3);
                    double theta;
                    double radius;
                    double handTilt;

                    c[0]=payload.get(0).asDouble();
                    c[1]=payload.get(1).asDouble();
                    c[2]=payload.get(2).asDouble();
                    theta=payload.get(3).asDouble();
                    radius=payload.get(4).asDouble();
                    handTilt=payload.get(5).asDouble();

                    double collision = drag(cmd==VOCAB4('v','d','r','g'), c, theta, radius, handTilt, pushHand, toolFrame);
                    if (collision < collisionThresh){
                        reply.addVocab(ack);
                        reply.addDouble(radius);
                        reply.addDouble(collision);
                    }
                    else{
                        reply.addVocab(nack);
                        reply.addDouble(radius);
                        reply.addDouble(collision);
                    }


                    if (cmd==VOCAB4('v','d','r','g'))
                        reply.addDouble(collision);
                }else {

                    reply.addVocab(nack);
                    reply.addString("Not enough amount of parameters for drag action");
                }

                break;
            }

            //-----------------
            case VOCAB4('s','l','i','d'):
            case VOCAB4('v','s','l','i'):
            {
                Bottle payload=command.tail();
                if (payload.size()>=5)
                {
                    Vector c(3);
                    double theta;
                    double radius;

                    c[0]=payload.get(0).asDouble();
                    c[1]=payload.get(1).asDouble();
                    c[2]=payload.get(2).asDouble();
                    theta=payload.get(3).asDouble();
                    radius=payload.get(4).asDouble();

                    double collision = slide(cmd==VOCAB4('v','s','l','i'), c, theta, radius, pushHand, toolFrame);
                    if (collision < collisionThresh){
                        reply.addVocab(ack);
                        reply.addDouble(radius);
                        reply.addDouble(collision);
                    }
                    else{
                        reply.addVocab(nack);
                        reply.addDouble(radius);
                        reply.addDouble(collision);
                    }

                    if (cmd==VOCAB4('v','s','l','i'))
                        reply.addDouble(collision);
                }else {

                    reply.addVocab(nack);
                    reply.addString("Not enough amount of parameters for slide action");
                }

                break;
            }

            //-----------------
            case VOCAB4('p','u','l','l'):   //Call the action as 'pull'
            case VOCAB4('v','p','u','l'):
            case VOCAB4('d','r','a','w'):   // or 'draw'
            case VOCAB4('v','d','r','a'):
            {
                Bottle payload=command.tail();
                if (payload.size()>=6)
                {
                    Vector c(3);
                    double theta;
                    double radius;
                    double dist;

                    c[0]=payload.get(0).asDouble();
                    c[1]=payload.get(1).asDouble();
                    c[2]=payload.get(2).asDouble();
                    theta=payload.get(3).asDouble();
                    radius=payload.get(4).asDouble();
                    dist=payload.get(5).asDouble();

                    double collision = pull(cmd==VOCAB4('v','d','r','a'), c, theta, radius, dist, pushHand, toolFrame);

                    if (collision < collisionThresh){
                        reply.addVocab(ack);
                        reply.addDouble(dist);
                        reply.addDouble(collision);
                    }
                    else{
                        reply.addVocab(nack);
                        reply.addDouble(dist);
                        reply.addDouble(collision);
                    }

                    if (cmd==VOCAB4('v','d','r','a'))
                        reply.addDouble(collision);
                } else {

                    reply.addVocab(nack);
                    reply.addString("Not enough amount of parameters for draw/pull action");
                }

                break;
            }

            //-----------------
            case VOCAB4('f','i','n','d'):
            {
                Bottle payload=command.tail();
                if (payload.size()>=2)
                {
                    string arm=payload.get(0).asString().c_str();
                    string eye=payload.get(1).asString().c_str();
                    Bottle solution;

                    if (findToolTip(arm,eye,solution))
                    {
                        reply.addVocab(ack);
                        reply.append(solution.tail());
                    }
                    else
                        reply.addVocab(nack);
                }

                break;
            }

            //-----------------
            case VOCAB4('c','o','l','T'):
            {
                Bottle payload=command.tail();
                
                double cT =  payload.get(0).asDouble();
                if (cT<=3){

                    collisionThresh =cT;
                    reply.addVocab(ack);
                }else{
                    reply.addVocab(nack);
                }
                break;
            }

            //-----------------
            case VOCAB4('t','o','o','l'):
            {
                if (command.size()>1)
                {
                    Bottle subcommand=command.tail();
                    int tag=subcommand.get(0).asVocab();
                    if (tag==Vocab::encode("attach"))
                    {
                        Bottle payload=subcommand.tail();
                        if (payload.size()>=4)
                        {
                            pushHand=payload.get(0).asString().c_str();

                            // Get the tool end effector coordinates w.r.t hand reference frame (RF)
                            Vector point(4);
                            point[0]=payload.get(1).asDouble();
                            point[1]=payload.get(2).asDouble();
                            point[2]=payload.get(3).asDouble();
                            point[3]=1.0;


                            Vector r(4,0.0);
                            // Set tool RF orientation so it goes along the tool long axis (assuming tool is held along Z plane
                            r[2]=-1.0;                          // Rotate around axis -Z (plane parallel to hand) ...
                            r[3]=atan2(-point[1],point[0]);     // ... along the -Y,X line from hand to tooltip points.
                            toolFrame=axis2dcm(r);                            
                            // Set tool RF to tooltip coordinates by adding translation
                            toolFrame.setCol(3,point);

                            reply.addVocab(ack);
                        }
                    }
                    else if (tag==Vocab::encode("get"))
                    {
                        reply.addVocab(ack);
                        reply.addString(pushHand.c_str());
                        reply.addDouble(toolFrame(0,3));
                        reply.addDouble(toolFrame(1,3));
                        reply.addDouble(toolFrame(2,3));
                    }
                    else if (tag==Vocab::encode("remove"))
                    {
                        pushHand="selectable";
                        toolFrame=eye(4,4);
                        iCartCtrlL->removeTipFrame();
                        iCartCtrlR->removeTipFrame();

                        reply.addVocab(ack);
                    }
                }

                break;
            }

            case VOCAB4('h','e','l','p'):
            {
                reply.addVocab(Vocab::encode("many"));
                reply.addString("Available commands are:");
                reply.addString("push - [push] cx cy cz theta radius - The object is pushed from the point at angle theta (deg) on the circle of radius (in m), centered at objects coordinates (cx,cy,cz) and contained in the x-y plane.");
                reply.addString("draw/pull - [draw]/[pull] cx cy cz theta radius dist - Pulls the object dist (in cm) from the point at angle theta (deg) on the circle of radius (in m), centered at objects coordinates (cx,cy,cz).");
                reply.addString("virtual draw/pull - [vdra]/[vpul] cx cy cz theta radius dist - Simulates the draw without performing any movement in order to test the quality of the action.");
                reply.addString("slide - [slid] cx cy cz theta radius - Performs a slide movement diametrically accross the circle or radius (in cm) centered at objects coordinates (cx,cy,cz) from point at angle theta.");
                reply.addString("virtual slide - [vsli] cx cy cz theta radius - Simulates the slide without performing any movement in order to test the quality of the action.");
                reply.addString("drag - [drag] cx cy cz theta radius - Perfoms a drag action from the object's position (cx,cy,cz) to the circle of radius in the direction of angle theta.");
                reply.addString("virtual drag - [vdrg] cx cy cz theta radius - Simulates the slide without performing any movement in order to test the quality of the action.");
                reply.addString("find - [find] arm eye - An exploration is performed which aims at finding the tool dimension.");
                reply.addString("Tool-Attach - [tool] [attach] arm x y z ");
                reply.addString("collision Thresh- [colT] (double) cT ");
                reply.addString("help - produces this help.");
                reply.addVocab(ack);
                break;
            }

            //-----------------
            default:
                interrupting=false;
                return RFModule::respond(command,reply);
        }

        interrupting=false;
        return true;
    }

    /***************************************************************/
    void changeElbowHeight()
    {
        if (elbow_set)
        {
            Bottle tweakOptions; 
            Bottle &optTask2=tweakOptions.addList();
            optTask2.addString("task_2");
            Bottle &plTask2=optTask2.addList();
            plTask2.addInt(6);
            Bottle &posPart=plTask2.addList();
            posPart.addDouble(0.0);
            posPart.addDouble(0.0);
            posPart.addDouble(elbow_height);
            Bottle &weightsPart=plTask2.addList();
            weightsPart.addDouble(0.0);
            weightsPart.addDouble(0.0);
            weightsPart.addDouble(elbow_weight);
            iCartCtrl->tweakSet(tweakOptions);
        }
    }

    /************************************************************************/
    bool push(const Vector &c, const double theta, const double radius,
              const string &armType="selectable", const Matrix &tipFrame=eye(4,4))
    {

        // wrt root frame: frame centered at c with x-axis pointing rightward,
        // y-axis pointing forward and z-axis pointing upward
        Matrix H0(4,4); H0.zero();
        H0(1,0)=1.0;
        H0(0,1)=-1.0;
        H0(2,2)=1.0;
        H0(0,3)=c[0]; H0(1,3)=c[1]; H0(2,3)=c[2]; H0(3,3)=1.0;

        double theta_rad=CTRL_DEG2RAD*theta;
        double _c=cos(theta_rad);
        double _s=sin(theta_rad);
        double _theta=CTRL_RAD2DEG*atan2(_s,_c);    // to have theta in [-180.0,180.0]
        double epsilon=0.05;

        // wrt H0 frame: frame centered at R*[_c,_s] with z-axis pointing inward
        // and x-axis tangential
        Matrix H1(4,4); H1.zero();
        H1(0,0)=-_s;       H1(1,0)=_c;
        H1(2,1)=-1.0;
        H1(0,2)=-_c;       H1(1,2)=-_s;
        H1(0,3)=radius*_c; H1(1,3)=radius*_s; H1(3,3)=1.0;

        // wrt H0 frame: frame centered at R*[_c,_s] with z-axis pointing outward
        // and x-axis tangential
        Matrix H2(4,4); H2.zero();
        H2(0,0)=_s;        H2(1,0)=-_c;
        H2(2,1)=-1.0;
        H2(0,2)=_c;        H2(1,2)=_s;
        H2(0,3)=radius*_c; H2(1,3)=radius*_s; H2(3,3)=1.0;

        // matrices that serve to account for pushing with the back of the hand
        Matrix H1eps=H1; Matrix H2eps=H2;
        H1eps(0,3)+=epsilon*_c; H1eps(1,3)+=epsilon*_s;
        H2eps(0,3)+=epsilon*_c; H2eps(1,3)+=epsilon*_s;
        
        // go back into root frame
        H1=H0*H1;
        H2=H0*H2;
        H1eps=H0*H1eps;
        H2eps=H0*H2eps;
        
        Vector xd1=H1.getCol(3).subVector(0,2);
        Vector od1=dcm2axis(H1);

        Vector xd2=H2.getCol(3).subVector(0,2);
        Vector od2=dcm2axis(H2);

        Vector xd1eps=H1eps.getCol(3).subVector(0,2);
        Vector od1eps=dcm2axis(H1eps);

        Vector xd2eps=H2eps.getCol(3).subVector(0,2);
        Vector od2eps=dcm2axis(H2eps);

        printf("identified locations...\n");
        printf("xd1=(%s) od1=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());
        printf("xd2=(%s) od2=(%s)\n",xd2.toString(3,3).c_str(),od2.toString(3,3).c_str());

        // choose the arm
        if (armType=="selectable")
        {
            if (xd1[1]>=0.0)
                iCartCtrl=iCartCtrlR;
            else
                iCartCtrl=iCartCtrlL;
        }
        else if (armType=="left")
            iCartCtrl=iCartCtrlL;
        else
            iCartCtrl=iCartCtrlR;

        // deal with the arm context
        int context;
        iCartCtrl->storeContext(&context);


        // Transform the end-effector to the tip of the tool (if any).
        Vector tip_x = tipFrame.getCol(3).subVector(0,2);
        Vector tip_o = yarp::math::dcm2axis(tipFrame);
        iCartCtrl->attachTipFrame(tip_x,tip_o);                // establish the new controlled frame

        Bottle options;
        Bottle &straightOpt=options.addList();
        straightOpt.addString("straightness");
        straightOpt.addDouble(10.0);
        iCartCtrl->tweakSet(options);
        changeElbowHeight();

        Vector dof;
        iCartCtrl->getDOF(dof);
        
        dof=1.0; dof[1]=0.0;
        iCartCtrl->setDOF(dof,dof);

        Vector xdhat1,odhat1,xdhat2,odhat2;
        Vector dummy;

        // try out different poses
        iCartCtrl->askForPose(xd1,od1,xdhat1,odhat1,dummy);
        iCartCtrl->askForPose(xd2,od2,xdhat2,odhat2,dummy);

        Matrix Hhat1=axis2dcm(odhat1); Hhat1(0,3)=xdhat1[0]; Hhat1(1,3)=xdhat1[1]; Hhat1(2,3)=xdhat1[2];
        Matrix Hhat2=axis2dcm(odhat2); Hhat2(0,3)=xdhat2[0]; Hhat2(1,3)=xdhat2[1]; Hhat2(2,3)=xdhat2[2];

        double d1=dist(H1-Hhat1);
        double d2=dist(H2-Hhat2);

        printf("solutions...\n");
        printf("#1: xdhat1=(%s) odhat1=(%s); e=%.3f\n",xdhat1.toString(3,3).c_str(),odhat1.toString(3,3).c_str(),d1);
        printf("#2: xdhat2=(%s) odhat2=(%s); e=%.3f\n",xdhat2.toString(3,3).c_str(),odhat2.toString(3,3).c_str(),d2);
        printf("selection: ");

        // compare solutions and choose the best
        Vector *xd,*od;
        if (fabs(_theta-90.0)<45.0)
        {
            printf("(detected singularity) ");
            if (iCartCtrl==iCartCtrlR)
            {
                xd=&xd1;
                od=&od1;
            }
            else
            {
                xd=&xd2;
                od=&od2;
            }
        }
        else if (fabs(_theta+90.0)<45.0)
        {
            printf("(detected singularity) ");
            if (iCartCtrl==iCartCtrlR)
            {
                xd=&xd2;
                od=&od2;
            }
            else
            {
                xd=&xd1;
                od=&od1;
            }
        }
        else if (d1<d2)
        {
            xd=&xd1;
            od=&od1;
        }
        else
        {
            xd=&xd2;
            od=&od2;
        }

        if (xd==&xd1)
            printf("#1 ");
        else
            printf("#2 ");

        if ((iCartCtrl==iCartCtrlR) && (_theta<0.0) && (xd==&xd2))
        {
            printf("(increased radius)");
            xd=&xd2eps;
            od=&od2eps;
        }
        else if ((iCartCtrl==iCartCtrlL) && (_theta<0.0) && (xd==&xd1))
        {
            printf("(increased radius)");
            xd=&xd1eps;
            od=&od1eps;
        }

        printf(": xd=(%s); od=(%s)\n",xd->toString(3,3).c_str(),od->toString(3,3).c_str());

        // execute the movement
        Vector offs(3,0.0); offs[2]=0.1;
        if (!interrupting)
        {
            Vector x=*xd+offs;

            printf("moving to: x=(%s); o=(%s)\n",x.toString(3,3).c_str(),od->toString(3,3).c_str());
            iCartCtrl->goToPoseSync(x,*od,1.0);
            iCartCtrl->waitMotionDone(0.1,4.0);
        }

        if (!interrupting)
        {
            printf("moving to: x=(%s); o=(%s)\n",xd->toString(3,3).c_str(),od->toString(3,3).c_str());
            iCartCtrl->goToPoseSync(*xd,*od,1.0);
            iCartCtrl->waitMotionDone(0.1,4.0);
        }

        double rmin,rmax,tmin,tmax;
        if (((fabs(theta)<10.0) || (fabs(theta-180.0)<10.0)))
        {
            rmin=0.04; rmax=0.18;
            tmin=0.40; tmax=0.60;
        }
        else
        {
            rmin=0.04; rmax=0.18;
            tmin=0.50; tmax=0.80;
        }

        // safe guard for using the tool
        if (armType!="selectable")
        {
            tmin*=1.3;
            tmax*=1.3;
        }

        double trajTime=tmin+((tmax-tmin)/(rmax-rmin))*(radius-rmin);
        trajTime=std::max(std::min(tmax,trajTime),tmin);

        if (!interrupting)
        {
            Matrix H=axis2dcm(*od);
            Vector center=c; center.push_back(1.0);
            H.setCol(3,center);
            //Vector x=-1.0*frame.getCol(3); x[3]=1.0; // Revert tool translation.
            //x=H*x; x.pop_back();

            //printf("moving to: x=(%s); o=(%s)\n",x.toString(3,3).c_str(),od->toString(3,3).c_str());
            //iCartCtrl->goToPoseSync(x,*od,trajTime);
            //iCartCtrl->waitMotionDone(0.1,3.0);
        }

        if (!interrupting)
        {
            printf("moving to: x=(%s); o=(%s)\n",xd->toString(3,3).c_str(),od->toString(3,3).c_str());
            iCartCtrl->goToPoseSync(*xd,*od,1.0);
            iCartCtrl->waitMotionDone(0.1,2.0);
            iCartCtrl->removeTipFrame();
        }

        iCartCtrl->removeTipFrame();            // to restore the center of the palm as actual end-effector for other uses (such as go Home)


        
        iCartCtrl->restoreContext(context);
        iCartCtrl->deleteContext(context);
        return true;
    }

    /************************************************************************/
    double pull(bool simulation, const Vector &c, const double theta,const double radius,  double &distance,
                const string &armType="selectable", const Matrix &tipFrame=eye(4,4))
    {
        // c0 is the projection of c on the sagittal plane
        Vector c_sag=c;
        c_sag[1]=0.0;

        // wrt root frame: frame centered at c_sag with x-axis pointing rightward,
        // y-axis pointing forward and z-axis pointing upward
        Matrix H0(4,4);

        Vector xd1;
        Vector od1;
        Vector xd2;
        Vector od2;

        int context;

        double dist = distance + 0.02;
        double res = 100.0;
        while (res>collisionThresh)
        {
            // It is impossible to perform the action with invalid pull distance.
            if (dist <0){
                return res;
            }else{
                dist = dist - 0.02;
            }

            printf("\n Trying pull with pull distance =%g\n", dist);

            // H0 reference frame of the target object
            H0.zero();
            H0(1,0)=1.0;
            H0(0,1)=-1.0;
            H0(2,2)=1.0;
            H0(0,3)=c_sag[0]; H0(1,3)=c_sag[1]; H0(2,3)=c_sag[2]; H0(3,3)=1.0;

            double theta_rad=CTRL_DEG2RAD*theta;
            double _c=cos(theta_rad);
            double _s=sin(theta_rad);

            // start point at H1, point of circle centered of the object at radius R and agle theta.
            // wrt H0 frame: frame translated in R*[_c,_s], keeping orientation
            Matrix H1=eye(4,4);
            H1(0,3)=radius*_c; H1(1,3)=radius*_s;

            // end point at H2, -dist on Y axis from H1 frame.
            // wrt H1 frame: frame translated in [0,-dist]:
            Matrix H2=eye(4,4);
            H2(1,3)=-dist;

            // go back into root frame
            H2=H0*H1*H2;
            H1=H0*H1;

            // apply final axes
            Matrix R(3,3); R.zero();
            R(0,0)=-1.0;
            R(2,1)=-1.0;
            R(1,2)=-1.0;

            H1.setSubmatrix(R,0,0);
            H2.setSubmatrix(R,0,0);

            xd1=H1.getCol(3).subVector(0,2);
            od1=dcm2axis(H1);

            xd2=H2.getCol(3).subVector(0,2);
            od2=dcm2axis(H2);

            printf("identified locations on the sagittal plane...\n");
            printf("xd1=(%s) od1=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());
            printf("xd2=(%s) od2=(%s)\n",xd2.toString(3,3).c_str(),od2.toString(3,3).c_str());

            // choose the arm
            if (armType=="selectable")
            {
                if (xd1[1]>=0.0)
                    iCartCtrl=iCartCtrlR;
                else
                    iCartCtrl=iCartCtrlL;
            }
            else if (armType=="left")
                iCartCtrl=iCartCtrlL;
            else
                iCartCtrl=iCartCtrlR;

            // recover the original place: do translation and rotation
            if (c[1]!=0.0)
            {
                Vector r(4,0.0);
                r[2]=-1.0;
                r[3]=atan2(c[1],fabs(c[0]));
                Matrix H=axis2dcm(r);

                H(0,3)=H1(0,3);
                H(1,3)=H1(1,3)+c[1];
                H(2,3)=H1(2,3);
                H1(0,3)=H1(1,3)=H1(2,3)=0.0;
                H1=H*H1;

                H(0,3)=H2(0,3);
                H(1,3)=H2(1,3)+c[1];
                H(2,3)=H2(2,3);
                H2(0,3)=H2(1,3)=H2(2,3)=0.0;
                H2=H*H2;

                xd1=H1.getCol(3).subVector(0,2);
                od1=dcm2axis(H1);

                xd2=H2.getCol(3).subVector(0,2);
                od2=dcm2axis(H2);
            }

            printf("in-place locations...\n");
            printf("xd1=(%s) od1=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());
            printf("xd2=(%s) od2=(%s)\n",xd2.toString(3,3).c_str(),od2.toString(3,3).c_str());

            // deal with the arm context
            iCartCtrl->storeContext(&context);

            // Transform the end-effector to the tip of the tool (if any).
            Vector tip_x=tipFrame.getCol(3).subVector(0,2);
            Vector tip_o=yarp::math::dcm2axis(tipFrame);
            iCartCtrl->attachTipFrame(tip_x,tip_o);                // establish the new controlled frame

            Bottle options;
            Bottle &straightOpt=options.addList();
            straightOpt.addString("straightness");
            straightOpt.addDouble(30.0);
            iCartCtrl->tweakSet(options);
            changeElbowHeight();

            Vector dof;
            iCartCtrl->getDOF(dof);

            dof=1.0; dof[1]=0.0;
            iCartCtrl->setDOF(dof,dof);

            // simulate the movements
            Vector xdhat1,odhat1,xdhat2,odhat2,qdhat;
            iCartCtrl->askForPose(xd1,od1,xdhat1,odhat1,qdhat);
            iCartCtrl->askForPose(qdhat,xd2,od2,xdhat2,odhat2,qdhat);

            double e_x1=norm(xd1-xdhat1);
            double e_o1=norm(od1-odhat1);
            printf("testing x=(%s); o=(%s) => xhat=(%s); ohat=(%s) ... |e_x|=%g; |e_o|=%g\n",
                   xd1.toString(3,3).c_str(),od1.toString(3,3).c_str(),
                   xdhat1.toString(3,3).c_str(),odhat1.toString(3,3).c_str(),
                   e_x1,e_o1);

            double e_x2=norm(xd2-xdhat2);
            double e_o2=norm(od2-odhat2);
            printf("testing x=(%s); o=(%s) => xhat=(%s); ohat=(%s) ... |e_x|=%g; |e_o|=%g\n",
                   xd2.toString(3,3).c_str(),od2.toString(3,3).c_str(),
                   xdhat2.toString(3,3).c_str(),odhat2.toString(3,3).c_str(),
                   e_x2,e_o2);

            double nearness_penalty=((norm(xdhat1)<0.15)||(norm(xdhat2)<0.15)?10.0:0.0);
            printf("nearness penalty=%g\n",nearness_penalty);
            res=e_x1+e_o1+e_x2+e_o2+nearness_penalty;
            printf("final quality=%g\n",res);

            if ((res>collisionThresh)) {           // If scape condition not met, remove tipFrame to avoid accumulation
                iCartCtrl->removeTipFrame(); }     // to restore the center of the palm as actual end-effector for other uses (such as go Home)

            // Check escape conditions:
            // Only an action simulation desired
            if (simulation){
                return res;
            }
        }

        // If not an action simulation and a valid distance has been found:
        // execute the movements    
        Vector offs(3,0.0); offs[2]=0.06;   // Offset 6 cm on Y axis (on top of the xd1 location)
        distance = dist;

        if (!interrupting)
        {   // First action component: place the tool over the starting point
            Vector x=xd1+offs;

            printf("moving to: x=(%s); o=(%s)\n",x.toString(3,3).c_str(),od1.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(x,od1,2.0);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Second action component: lower the tool to meet the object
            printf("moving to: x=(%s); o=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xd1,od1,1.5);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Third action component: perform the pull from the original point to the end point
            printf("moving to: x=(%s); o=(%s)\n",xd2.toString(3,3).c_str(),od2.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xd2,od2,3.5);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }
        if (!interrupting)
        {   // Fourth and final action component: lift the tool to release the object
            Vector x=xd2+offs;

            printf("moving to: x=(%s); o=(%s)\n",x.toString(3,3).c_str(),od2.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(x,od2,2.0);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }


        iCartCtrl->removeTipFrame();            // to restore the center of the palm as actual end-effector for other uses (such as go Home)


        iCartCtrl->restoreContext(context);
        iCartCtrl->deleteContext(context);

        return res;
    }

    /************************************************************************/    
    double drag(bool simulation, const Vector &c, const double theta, double &radius, const double handtilt,
                const string &armType = "selectable", const Matrix &tipFrame=eye(4,4))
    {
        // c0 is the projection of c on the sagittal plane
        Vector c_sag=c;
        c_sag[1]=0.0;

        // wrt root frame: frame centered at c_sag with x-axis pointing rightward,
        // y-axis pointing forward and z-axis pointing upward
        Matrix H0(4,4);

        Vector xd0;
        Vector od0;
        Vector xd1;
        Vector od1;

        int context;

        double rad = radius + 0.02;
        double res = 100.0;
        while (res>collisionThresh)
        {
            // It is impossible to perform the action with invalid radius.
            if (rad <0){
                return res;
            }else{
                rad = rad - 0.02;
            }

            printf("\n Trying action with radius =%g\n",rad);

            // wrt root frame: frame centered at c_sag with x-axis pointing rightward,
            // y-axis pointing forward and z-axis pointing upward

            // start at point wrt H0
            H0.zero();
            H0(1,0)=1.0;
            H0(0,1)=-1.0;
            H0(2,2)=1.0;
            H0(0,3)=c_sag[0]; H0(1,3)=c_sag[1]; H0(2,3)=c_sag[2]; H0(3,3)=1.0;

            double theta_rad=CTRL_DEG2RAD*theta;
            double _c=cos(theta_rad);
            double _s=sin(theta_rad);

            // end point at H1, point of circle centered of the object at radius R and angle theta.
            // wrt H0 frame translated in R*[_c,_s], keeping orientation
            Matrix H1=eye(4,4);
            H1(0,3)=rad*_c; H1(1,3)=rad*_s;

            // go back into root frame
            H1=H0*H1;

            // apply final axes
            Matrix R(3,3); R.zero();
            R(0,0)=-1.0;
            R(2,1)=-1.0;
            R(1,2)=-1.0;

            H0.setSubmatrix(R,0,0);
            H1.setSubmatrix(R,0,0);

            xd0=H0.getCol(3).subVector(0,2);
            od0=dcm2axis(H0);

            xd1=H1.getCol(3).subVector(0,2);
            od1=dcm2axis(H1);

            printf("identified locations on the sagittal plane...\n");
            printf("xd0=(%s) od0=(%s)\n",xd0.toString(3,3).c_str(),od0.toString(3,3).c_str());
            printf("xd1=(%s) od1=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());

            // choose the arm
            if (armType=="selectable")
            {
                if (xd0[1]>=0.0)
                    iCartCtrl=iCartCtrlR;
                else
                    iCartCtrl=iCartCtrlL;
            }
            else if (armType=="left")
                iCartCtrl=iCartCtrlL;
            else
                iCartCtrl=iCartCtrlR;

            // recover the original place: do translation and rotation
            if (c[1]!=0.0)
            {
                Vector r(4,0.0);
                r[2]=-1.0;
                r[3]=atan2(c[1],fabs(c[0]));
                Matrix H=axis2dcm(r);

                // Add translation for tables height.
                H(0,3)=H0(0,3);
                H(1,3)=H0(1,3)+c[1];
                H(2,3)=H0(2,3);
                H0(0,3)=H0(1,3)=H0(2,3)=0.0;
                H0=H*H0;

                H(0,3)=H1(0,3);
                H(1,3)=H1(1,3)+c[1];
                H(2,3)=H1(2,3);
                H1(0,3)=H1(1,3)=H1(2,3)=0.0;
                H1=H*H1;

                xd0=H0.getCol(3).subVector(0,2);
                od0=dcm2axis(H0);
                xd1=H1.getCol(3).subVector(0,2);
                od1=dcm2axis(H1);

            }

            printf("in-place locations...\n");
            printf("xd0=(%s) od0=(%s)\n",xd0.toString(3,3).c_str(),od0.toString(3,3).c_str());
            printf("xd1=(%s) od1=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());

            // Add an extra hand rotation so that the tool is no on upward angle from the hand
            xd0=H0.getCol(3).subVector(0,2);
            xd1=H1.getCol(3).subVector(0,2);

            Vector handRot0(4);
            handRot0[0] = 0.0; handRot0[1] = 1.0; handRot0[2] = 0.0; handRot0[3] = handtilt*M_PI/180.0; //Tilt hand 'handtilt' extra degrees
            Matrix R0 = axis2dcm(handRot0);
            H0 = R0*H0;
            od0=dcm2axis(H0);

            Vector handRot1(4);
            handRot1[0] = 0.0; handRot1[1] = 1.0; handRot1[2] = 0.0; handRot1[3] = (handtilt+10)*M_PI/180.0; //Tilt hand 'handtilt' extra degrees
            Matrix R1 = axis2dcm(handRot1);
            H1 = R1*H1;
            od1=dcm2axis(H1);

            printf("apply tilt (if any)...\n");
            printf("xd0=(%s) od0=(%s)\n",xd0.toString(3,3).c_str(),od0.toString(3,3).c_str());
            printf("xd1=(%s) od1=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());

            // deal with the arm context            
            iCartCtrl->storeContext(&context);

            // Transform the end-effector to the tip of the tool (if any).            
            Vector tip_x = tipFrame.getCol(3).subVector(0,2);
            Vector tip_o = yarp::math::dcm2axis(tipFrame);
            iCartCtrl->attachTipFrame(tip_x,tip_o);                // establish the new controlled frame

            Bottle options;
            Bottle &straightOpt=options.addList();
            straightOpt.addString("straightness");
            straightOpt.addDouble(30.0);
            iCartCtrl->tweakSet(options);
            changeElbowHeight();

            Vector dof;
            iCartCtrl->getDOF(dof);

            dof=1.0; dof[1]=0.0;
            iCartCtrl->setDOF(dof,dof);


            // simulate the movements
            Vector xdhat0,odhat0,xdhat1,odhat1,qdhat;
            iCartCtrl->askForPose(xd0,od0,xdhat0,odhat0,qdhat);
            iCartCtrl->askForPose(qdhat,xd1,od1,xdhat1,odhat1,qdhat);

            double e_x0=norm(xd0-xdhat0);
            double e_o0=norm(od0-odhat0);
            printf("testing x=(%s); o=(%s) => xhat=(%s); ohat=(%s) ... |e_x|=%g; |e_o|=%g\n",
                   xd0.toString(3,3).c_str(),od0.toString(3,3).c_str(),
                   xdhat0.toString(3,3).c_str(),odhat0.toString(3,3).c_str(),
                   e_x0,e_o0);

            double e_x1=norm(xd1-xdhat1);
            double e_o1=norm(od1-odhat1);
            printf("testing x=(%s); o=(%s) => xhat=(%s); ohat=(%s) ... |e_x|=%g; |e_o|=%g\n",
                   xd1.toString(3,3).c_str(),od1.toString(3,3).c_str(),
                   xdhat1.toString(3,3).c_str(),odhat1.toString(3,3).c_str(),
                   e_x1,e_o1);

            double nearness_penalty=((norm(xdhat0)<0.15)||(norm(xdhat1)<0.15)?10.0:0.0);
            printf("nearness penalty=%g\n",nearness_penalty);
            res=e_x0+e_o0+e_x1+e_o1+nearness_penalty;
            printf("final quality=%g\n",res);

            if ((res>collisionThresh)) {           // If scape condition not met, remove tipFrame to avoid accumulation
                iCartCtrl->removeTipFrame(); }     // to restore the center of the palm as actual end-effector for other uses (such as go Home)

            // Check escape conditions:
            // Only an action simulation desired
            if (simulation){
                return res;
            }
        }

        // If not an action simulation and a valid radius has been found:
        // execute the movements
        Vector offs(3,0.0); offs[2]=0.08;
        radius = rad;

        if (!interrupting)
        {   // First action component: place the tool over the object
            Vector xA=xd0+offs;

            printf("approach obj to :  x=(%s); o=(%s)\n",xA.toString(3,3).c_str(),od0.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xA,od0,2.0);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Second action component: lower the tool to meet the object            
            printf("moving to object: x=(%s); o=(%s)\n",xd0.toString(3,3).c_str(),od0.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xd0,od0,1.5);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Third action component: waypoint 1 at mid to enforce straighness
            Vector xW1 = (xd0+xd1)/2;
            Vector oW1 = od0;
            printf("waypoint at     : x=(%s); o=(%s)\n",xW1.toString(3,3).c_str(),oW1.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xW1,oW1,1.0);
            iCartCtrl->waitMotionDone(0.1,3.0);
        }

        if (!interrupting)
        {   // Fourth action component: drag the tool to the desired position
            printf("dragging to     : x=(%s); o=(%s)\n",xd1.toString(3,3).c_str(),od1.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xd1,od1,1.5);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Fifth and final action component: lift the tool to release the object
            Vector xD=xd1+offs;
            printf("Releasing object: x=(%s); o=(%s)\n",xD.toString(3,3).c_str(),od1.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xD,od1,2.0);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        iCartCtrl->removeTipFrame();            // to restore the center of the palm as actual end-effector for other uses (such as go Home)

        iCartCtrl->restoreContext(context);
        iCartCtrl->deleteContext(context);

        return res;
    }

    /************************************************************************/
    double slide(bool simulation, const Vector &c, const double theta, double &radius,
                 const string &armType="selectable", const Matrix &tipFrame=eye(4,4))
    {
        // c0 is the projection of c on the sagittal plane
        Vector c_sag=c;
        c_sag[1]=0.0;

        Matrix H0(4,4);

        Vector xdstart;
        Vector odstart;
        Vector xdend;
        Vector odend;

        int context;

        double rad = radius + 0.02;
        double res = 100.0;
        while (res>collisionThresh)
        {
            // It is impossible to perform the action with invalid radius.
            if (rad <0){
                return res;
            }else{
                rad = rad - 0.02;
            }
            printf("\n Trying action with radius =%g\n",rad);

            // wrt root frame: frame centered at c_sag with x-axis pointing rightward,
            // y-axis pointing forward and z-axis pointing upward
            H0.zero();
            H0(1,0)=1.0;
            H0(0,1)=-1.0;
            H0(2,2)=1.0;
            H0(0,3)=c_sag[0]; H0(1,3)=c_sag[1]; H0(2,3)=c_sag[2]; H0(3,3)=1.0;

            double theta_rad=CTRL_DEG2RAD*theta;
            double _c=cos(theta_rad);
            double _s=sin(theta_rad);

            // start at point wrt H0 frame translated in R*[_c,_s], keeping orientation
            Matrix Hstart=eye(4,4);
            Hstart(0,3)=rad*_c; Hstart(1,3)=rad*_s;

            // finish at point wrt H0 frame translated in -R*[_c,_s], that is, at the opposite side of the circle, keeping orientation
            Matrix Hend=eye(4,4);
            Hend(0,3)=-rad*_c; Hend(1,3)=-rad*_s;

            // go back into root frame
            Hstart=H0*Hstart;
            Hend=H0*Hend;

            // apply final axes
            Matrix R(3,3); R.zero();
            R(0,0)=-1.0;
            R(2,1)=-1.0;
            R(1,2)=-1.0;

            Hstart.setSubmatrix(R,0,0);
            Hend.setSubmatrix(R,0,0);

            xdstart=Hstart.getCol(3).subVector(0,2);
            odstart=dcm2axis(Hstart);

            xdend=Hend.getCol(3).subVector(0,2);
            odend=dcm2axis(Hend);

            printf("identified locations on the sagittal plane...\n");
            printf("xdstart=(%s) odstart=(%s)\n",xdstart.toString(3,3).c_str(),odstart.toString(3,3).c_str());
            printf("xdend=(%s) odend=(%s)\n",xdend.toString(3,3).c_str(),odend.toString(3,3).c_str());

            // choose the arm
            if (armType=="selectable")
            {
                if (xdstart[1]>=0.0)
                    iCartCtrl=iCartCtrlR;
                else
                    iCartCtrl=iCartCtrlL;
            }
            else if (armType=="left")
                iCartCtrl=iCartCtrlL;
            else
                iCartCtrl=iCartCtrlR;

            // recover the original place: do translation and rotation
            if (c[1]!=0.0)
            {
                Vector r(4,0.0);
                r[2]=-1.0;
                r[3]=atan2(c[1],fabs(c[0]));
                Matrix H=axis2dcm(r);

                H(0,3)=Hstart(0,3);
                H(1,3)=Hstart(1,3)+c[1];
                H(2,3)=Hstart(2,3);
                Hstart(0,3)=Hstart(1,3)=Hstart(2,3)=0.0;
                Hstart=H*Hstart;

                H(0,3)=Hend(0,3);
                H(1,3)=Hend(1,3)+c[1];
                H(2,3)=Hend(2,3);
                Hend(0,3)=Hend(1,3)=Hend(2,3)=0.0;
                Hend=H*Hend;

                xdstart=Hstart.getCol(3).subVector(0,2);
                odstart=dcm2axis(Hstart);

                xdend=Hend.getCol(3).subVector(0,2);
                odend=dcm2axis(Hend);
            }

            printf("in-place locations...\n");
            printf("xdstart=(%s) odstart=(%s)\n",xdstart.toString(3,3).c_str(),odstart.toString(3,3).c_str());
            printf("xdend=(%s) odend=(%s)\n",xdend.toString(3,3).c_str(),odend.toString(3,3).c_str());

            // deal with the arm context
            iCartCtrl->storeContext(&context);


            // Transform the end-effector to the tip of the tool (if any).
            Vector tip_x=tipFrame.getCol(3).subVector(0,2);
            Vector tip_o=yarp::math::dcm2axis(tipFrame);

            iCartCtrl->attachTipFrame(tip_x,tip_o);                // establish the new controlled frame

            Bottle options;
            Bottle &straightOpt=options.addList();
            straightOpt.addString("straightness");
            straightOpt.addDouble(50.0);
            iCartCtrl->tweakSet(options);
            changeElbowHeight();

            Vector dof;
            iCartCtrl->getDOF(dof);

            dof=1.0; dof[1]=0.0;
            iCartCtrl->setDOF(dof,dof);

            // simulate the movements
            Vector xdhatStart,odhatStart,xdhatEnd,odhatEnd,qdhat;
            iCartCtrl->askForPose(xdstart,odstart,xdhatStart,odhatStart,qdhat);
            iCartCtrl->askForPose(qdhat,xdend,odend,xdhatEnd,odhatEnd,qdhat);

            double e_xStart=norm(xdstart-xdhatStart);
            double e_oStart=norm(odstart-odhatStart);
            printf("testing x=(%s); o=(%s) => xhat=(%s); ohat=(%s) ... |e_x|=%g; |e_o|=%g\n",
                   xdstart.toString(3,3).c_str(),odstart.toString(3,3).c_str(),
                   xdhatStart.toString(3,3).c_str(),odhatStart.toString(3,3).c_str(),
                   e_xStart,e_oStart);

            double e_xEnd=norm(xdend-xdhatEnd);
            double e_oEnd=norm(odend-odhatEnd);
            printf("testing x=(%s); o=(%s) => xhat=(%s); ohat=(%s) ... |e_x|=%g; |e_o|=%g\n",
                   xdend.toString(3,3).c_str(),odend.toString(3,3).c_str(),
                   xdhatEnd.toString(3,3).c_str(),odhatEnd.toString(3,3).c_str(),
                   e_xEnd,e_oEnd);

            double nearness_penalty=((norm(xdhatStart)<0.15)||(norm(xdhatEnd)<0.15)?10.0:0.0);
            printf("nearness penalty=%g\n",nearness_penalty);
            res = e_xStart + e_oStart + e_xEnd + e_oEnd + nearness_penalty;
            printf("final quality=%g\n",res);            

            if ((res>collisionThresh)) {           // If scape condition not met, remove tipFrame to avoid accumulation
                iCartCtrl->removeTipFrame(); }     // to restore the center of the palm as actual end-effector for other uses (such as go Home)

            // Check escape conditions:
            // Only an action simulation desired
            if (simulation){
                return res;
            }
        }

        // execute the movements
        cout << "Starting slide Execution" << endl;
        Vector offs(3,0.0); offs[2]=0.06;
        radius = rad;

        if (!interrupting)
        {   // First action component: place the tool at angle theta on the circle centered on the object and radius R
            printf("Positioning at: x=(%s); o=(%s)\n",xdstart.toString(3,3).c_str(),odstart.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xdstart,odstart,2.0);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Waypoint at object location to enforce straighness, and contact with object
            Vector xW1 = (xdstart+ xdend)/2.0;
            printf("Waypoint at: x=(%s); o=(%s)\n",xW1.toString(3,3).c_str(),odstart.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xW1,odstart,1.5);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Second action component: slide the tool to the oppoiste side of the circle through the center, were the object is
            printf("moving to: x=(%s); o=(%s)\n",xdend.toString(3,3).c_str(),odend.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xdend,odend,1.5);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }

        if (!interrupting)
        {   // Third and final action component: lift the tool to release the object
            Vector xC=xdend+offs;
            printf("moving to: x=(%s); o=(%s)\n",xC.toString(3,3).c_str(),odend.toString(3,3).c_str());
            iCartCtrl->goToPoseSync(xC,odend,2.0);
            iCartCtrl->waitMotionDone(0.1,5.0);
        }


        iCartCtrl->removeTipFrame();            // to restore the center of the palm as actual end-effector for other uses (such as go Home)

        iCartCtrl->restoreContext(context);
        iCartCtrl->deleteContext(context);

        return res;
    }

    /************************************************************************/
    void shakeHand()
    {
        IEncoders        *ienc;
        IVelocityControl *ivel;

        if (handUsed=="left")
        {
            driverHL.view(ienc);
            driverHL.view(ivel);
        }
        else
        {
            driverHR.view(ienc);
            driverHR.view(ivel);
        }

        double pos;
        ienc->getEncoder(shake_joint,&pos);

        double e=flip_hand-pos;
        if ((flip_hand>0.0) && (e<0.0) ||
            (flip_hand<0.0) && (e>0.0))
        {
            flip_hand=-flip_hand;
            e=flip_hand-pos;
        }

        ivel->velocityMove(shake_joint,120.0*sign(e));
    }

    /************************************************************************/
    void stopHand(const string &hand)
    {
        IVelocityControl *ivel;
        if (hand=="left")
            driverHL.view(ivel);
        else
            driverHR.view(ivel);

        ivel->stop(4);
    }

    /************************************************************************/
    void moveTool(const string &arm, const string &eye, const Vector &xd, const Vector &od,
                  const Vector &xOffset, const int maxItems)
    {
        iGaze->restoreContext(0);
        
        if (!interrupting)
        {
            iGaze->setTrackingMode(true);
            iGaze->lookAtFixationPoint(xd+xOffset);
            iCartCtrl->goToPoseSync(xd,od,1.0);
            iCartCtrl->waitMotionDone(0.1);
        }

        iGaze->setSaccadesMode(false);
        iGaze->setNeckTrajTime(2.5);
        iGaze->setEyesTrajTime(1.5);

        // put the shaking joint in velocity mode
        IControlMode2 *imode;
        if (arm=="left")
            driverHL.view(imode);
        else
            driverHR.view(imode);
        imode->setControlMode(shake_joint,VOCAB_CM_VELOCITY);
        handUsed=arm;   // this triggers the hand shaking

        // gaze robustly at the tool tip
        Vector pxCum(2,0.0);
        int cnt=0; bool done=false;
        double t0=Time::now();
        while (!interrupting && !done)
        {
            double t1=Time::now();
            if (Bottle *target=visionPort.read(false))
            {
                if (target->size()>=2)
                {
                    Vector px(2);
                    px[0]=target->get(0).asDouble();
                    px[1]=target->get(1).asDouble()+50.0;
                    iGaze->lookAtMonoPixel(eye=="left"?0:1,px);

                    pxCum+=px;
                    cnt++;
                }
            }

            if (t1-t0>=3.0)
            {
                if (cnt>20)
                    done=fabs(pxCum[1]/cnt-120)<30.0;

                pxCum=0.0;
                cnt=0;
                t0=t1;
            }

            Time::delay(0.02);
        }

        // gather sufficient information
        Bottle command,reply;
        command.addVocab(Vocab::encode("enable"));
        finderPort.write(command,reply);

        command.clear();
        command.addVocab(Vocab::encode("num"));
        finderPort.write(command,reply);
        int curItems=reply.get(1).asInt();

        int nItems=0;
        while (!interrupting && (nItems<curItems+maxItems))
        {
            finderPort.write(command,reply);
            nItems=reply.get(1).asInt();

            if (Bottle *target=visionPort.read(false))
            {
                if (target->size()>=2)
                {
                    Vector px(2);
                    px[0]=target->get(0).asDouble();
                    px[1]=target->get(1).asDouble()+50.0;
                    iGaze->lookAtMonoPixel(eye=="left"?0:1,px);
                }
            }

            Time::delay(0.1);
        }

        command.clear();
        command.addVocab(Vocab::encode("disable"));
        finderPort.write(command,reply);

        handUsed="null";
        stopHand(arm);
    }

    /************************************************************************/
    bool findToolTip(const string &arm, const string &eye, Bottle &reply)
    {
        if (arm=="left")
            iCartCtrl=iCartCtrlL;
        else if (arm=="right")
            iCartCtrl=iCartCtrlR;
        else
            return false;

        int context_arm,context_gaze;
        iCartCtrl->storeContext(&context_arm);
        iGaze->storeContext(&context_gaze);

        Vector dof;
        iCartCtrl->getDOF(dof);
        dof=1.0; dof[0]=dof[1]=0.0;
        iCartCtrl->setDOF(dof,dof);

        Bottle command;
        command.addVocab(Vocab::encode("clear"));
        finderPort.write(command,reply);

        // solving
        command.clear();
        command.addVocab(Vocab::encode("select"));
        command.addString(arm.c_str());
        command.addString(eye.c_str());
        finderPort.write(command,reply);

        Matrix R(4,4);
        R(0,0)=-1.0;
        R(2,1)=-1.0;
        R(1,2)=-1.0;
        R(3,3)=+1.0;
        Vector r(4,0.0); r[0]=-1.0;
        Vector xd(3,0.0),od;
        Vector offset(3,0.0); offset[2]=0.1;

        // point 1
        r[3]=0.0;
        od=dcm2axis(axis2dcm(r)*R);
        xd[0]=-0.35;
        shake_joint=4;
        moveTool(arm,eye,xd,od,offset,25);

        // point 2
        r[3]=CTRL_DEG2RAD*(arm=="left"?30.0:-30.0);
        od=dcm2axis(axis2dcm(r)*R);
        xd[1]=(arm=="left")?-0.15:0.15;
        offset[1]=(arm=="left")?0.1:-0.1;
        moveTool(arm,eye,xd,od,offset,25);

        // point 3
        r[3]=CTRL_DEG2RAD*(arm=="left"?20.0:-20.0);
        od=dcm2axis(axis2dcm(r)*R);
        xd[2]=0.15;
        offset[1]=(arm=="left")?0.2:-0.2;
        offset[2]=0.1;
        moveTool(arm,eye,xd,od,offset,25);

        // point 4
        r[3]=CTRL_DEG2RAD*(arm=="left"?10.0:-10.0);
        od=dcm2axis(axis2dcm(r)*R);
        xd[0]=-0.3;
        xd[1]=(arm=="left")?-0.05:0.05;
        xd[2]=-0.05;
        moveTool(arm,eye,xd,od,offset,25);

        // point 5
        r[3]=CTRL_DEG2RAD*(arm=="left"?45.0:-45.0);
        od=dcm2axis(axis2dcm(r)*R);
        xd[0]=-0.35;
        xd[1]=(arm=="left")?-0.05:0.05;
        xd[2]=0.1;
        offset[1]=(arm=="left")?0.1:-0.1;
        moveTool(arm,eye,xd,od,offset,25);

        // point 6
        xd[0]=-0.35;
        xd[1]=(arm=="left")?-0.1:0.1;
        xd[2]=0.0;
        Vector r1(4,0.0); r1[2]=(arm=="left")?-1.0:1.0; r1[3]=CTRL_DEG2RAD*45.0;
        Vector r2(4,0.0); r2[0]=(arm=="left")?1.0:-1.0; r2[3]=CTRL_DEG2RAD*45.0;
        od=dcm2axis(axis2dcm(r2)*axis2dcm(r1)*R);
        offset[0]=0;
        offset[1]=(arm=="left")?-0.05:0.05;
        offset[2]=0.1;
        shake_joint=6;
        moveTool(arm,eye,xd,od,offset,50);

        // solving
        command.clear();
        command.addVocab(Vocab::encode("find"));
        finderPort.write(command,reply);

        iCartCtrl->restoreContext(context_arm);
        iCartCtrl->deleteContext(context_arm);

        iGaze->restoreContext(context_gaze);
        iGaze->deleteContext(context_gaze);

        return true;
    }

public:
    /************************************************************************/
    bool configure(ResourceFinder &rf)
    {
        string name=rf.check("name",Value("affMotor")).asString().c_str();
        string robot=rf.check("robot",Value("icub")).asString().c_str();
        collisionThresh = rf.check("colThr",Value(2.5)).asDouble();
        elbow_set=rf.check("elbow_set");

        if (elbow_set)
        {
            if (Bottle *pB=rf.find("elbow_set").asList())
            {
                elbow_height=pB->get(0).asDouble();
                elbow_weight=pB->get(1).asDouble();
            }
            else
            {
                elbow_height=0.4;
                elbow_weight=30.0;
            }
        }

        Property optionG("(device gazecontrollerclient)");
        optionG.put("remote","/iKinGazeCtrl");
        optionG.put("local",("/"+name+"/gaze_ctrl").c_str());

        Property optionL("(device cartesiancontrollerclient)");
        optionL.put("remote",("/"+robot+"/cartesianController/left_arm").c_str());
        optionL.put("local",("/"+name+"/cart_ctrl/left_arm").c_str());

        Property optionR("(device cartesiancontrollerclient)");
        optionR.put("remote",("/"+robot+"/cartesianController/right_arm").c_str());
        optionR.put("local",("/"+name+"/cart_ctrl/right_arm").c_str());

        Property optionHL("(device remote_controlboard)");
        optionHL.put("remote",("/"+robot+"/left_arm").c_str());
        optionHL.put("local",("/"+name+"/hand_ctrl/left_arm").c_str());

        Property optionHR("(device remote_controlboard)");
        optionHR.put("remote",("/"+robot+"/right_arm").c_str());
        optionHR.put("local",("/"+name+"/hand_ctrl/right_arm").c_str());

        if (!driverG.open(optionG))
            return false;

        if (!driverL.open(optionL))
        {
            driverG.close();
            return false;
        }

        if (!driverR.open(optionR))
        {
            driverG.close();
            driverL.close();
            return false;
        }

        if (!driverHL.open(optionHL))
        {
            driverG.close();
            driverL.close();
            driverR.close();
            return false;
        }

        if (!driverHR.open(optionHR))
        {
            driverG.close();
            driverL.close();
            driverR.close();
            driverHL.close();
            return false;
        }

        driverG.view(iGaze);
        driverL.view(iCartCtrlL);
        driverR.view(iCartCtrlR);

        visionPort.open(("/"+name+"/vision:i").c_str());
        finderPort.open(("/"+name+"/finder:rpc").c_str());
        rpcPort.open(("/"+name+"/rpc").c_str());
        stopPort.open(("/"+name+"/stop:i").c_str());
        attach(rpcPort);
        stopPort.setReader(*this);

        interrupting=false;
        handUsed="null";
        flip_hand=6.0;

        pushHand="selectable";
        toolFrame=eye(4,4);

        return true;
    }

    /************************************************************************/
    bool interruptModule()
    {
        interrupting=true;

        iGaze->stopControl();
        iCartCtrlL->stopControl();
        iCartCtrlR->stopControl();

        if (handUsed!="null")
        {
            stopHand("left");
            stopHand("right");
        }

        return true;
    }

    /************************************************************************/
    bool close()
    {
        visionPort.close();
        finderPort.close();
        rpcPort.close();
        stopPort.close();   // close prior to shutting down motor-interfaces

        driverG.close();
        driverL.close();
        driverR.close();
        driverHL.close();
        driverHR.close();
        return true;
    }

    /************************************************************************/
    double getPeriod()
    {
        return 0.02;
    }

    /************************************************************************/
    bool updateModule()
    {
        if (!interrupting && (handUsed!="null"))
            shakeHand();

        return true;
    }
};


/************************************************************************/
int main(int argc, char *argv[])
{
    Network yarp;
    if (!yarp.checkNetwork())
    {
        printf("YARP server not available!\n");
        return 1;
    }

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.configure(argc,argv);

    AffMotor affMotor;
    return affMotor.runModule(rf);
}




