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

#include <yarp/os/Network.h>
#include <yarp/os/Module.h>
#include <yarp/dev/all.h>

#include "simtoolloader.h"

int main(int argc, char *argv[]) {

    Network yarp;
    if(! yarp.checkNetwork() ) {
        fprintf(stdout,"Error: yarp server does not seem available\n");
        return -1;
    }

    SimToolLoaderModule module;

    ResourceFinder rf;
    rf.setVerbose(true);
    rf.setDefaultContext("AffordancesProject");
    rf.setDefaultConfigFile("50tools.ini");
    rf.configure(argc, argv);
    module.runModule(rf);

    return 0;

}
