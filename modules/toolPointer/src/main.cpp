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
\defgroup toolPointer
 
@ingroup icub_module  
 
Uses 3D and image info to find the closest object and segment it out
 
Author: Tanis

CopyPolicy: Released under the terms of the GNU GPL v2.0. 

\section intro_sec Description 
Module which uses 3D coordinates and disparity information from to find the closest object, and computes euclidean distance to it.

\section lib_sec Libraries 
- YARP libraries. 
- OPENCV library. 

\section parameters_sec Parameters
 
\section portsa_sec Ports Accessed
The module is assumed to work on the output of a disparity computation module, which generally requires iKinGazeCtrl
 
\section portsc_sec Ports Created 

Input ports
- \e /<modName>/coords:i receives the coordinates of the tool blob from a click on yarpview
- \e /<modName>/rpc:i can be used to issue commands to the robot.
    Recognized remote commands:
 - 'help' produces this help.
        - 'quit' closes the module.

Output ports
- \e /<modName>/target:o streams out the center X Y Z coordinates of the closest object.
- \e /<modName>/img:o streams out a disparity image where the closest object is highlighted in green if reachable (considering a threshold distance), and in red otherwise.

\section in_files_sec Input Data Files
None.

\section out_data_sec Output Data Files 
None. 

\section conf_file_sec Configuration Files 
--from \e file 
- specify the configuration file (use \e --context option to 
  select the current context).

  The configuration file passed through the option \e --from
should look like as follows:
 
\code 
name                toolBlobber
robot               icub
origin              0 0 0.2
minBlobSize         400
\endcode 

\section tested_os_sec Tested OS
Linux, Windows

\author Tanis Mar

*/


#include "toolPointer.h"

#include <yarp/dev/all.h>
//YARP_DECLARE_DEVICES(icubmod)

using namespace yarp::os;

int main(int argc, char * argv[])
{
    /* initialize yarp network */
    Network::init();

    /* create the module */
    ToolPointerModule module;

    /* prepare and configure the resource finder */
    ResourceFinder rf;
    rf.setVerbose( true );
    rf.setDefaultContext( "AffordancesProject" );
    rf.setDefaultConfigFile( "toolPointer.ini" );
    rf.setDefault("name","toolPointer");
    rf.configure( argc, argv );

    /* run the module: runModule() calls configure first and, if successful, it then runs */
    module.runModule(rf);
    Network::fini();

    return 0;
}
//empty line to make gcc happy
