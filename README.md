## Overview
This repository contains modules concerning the exploration and learning of tool affordances. 
The general learning schema consists of 3 phases: 
1. Exploration: After being provided a tool, iCub extracts a descriptor vector to represent it, and executes a series of actions, recording the achieved effects. 
2. Learning: After enough tool affordance data (that is, the effects achieved by each action with each tool) has been recorded, it is sent to an affordance learning model, which learns the relationship between the tool, the action, and the achieved effects. 
3. Prediction: Given any tool similar to the ones used diring training, the iCub extracts its descriptive features and feeds them to the trained affordance model, which in turn is able to predict the expected effect for any given action. This predictions can be used to select the best action for any task acheived through the action repertoire, or even to select another tool. 

A diagram of the described learning schema can be observed below:
![learnSchema](/toolAffCycle_noToolSel.png)

Moreover, it provides the motor and managing modules to run the tool use demo (click image to see video on Youtube):
[![tool use demo video](/tool_use_vidprof.png)](https://www.youtube.com/watch?v=2Jmm4zel134)

## Use
Most of the exploration actions can be executed either individually or in a cycle using the manager modules' commands, see affManager and tool3DManager help. For running full experimental exploration as well as for the behavior-based tool use demo, see the applications [here](https://github.com/robotology/tool-affordances/tree/master/app/scripts).

## Theoretical background
The methods executed by the modules in this repository have been described in detail in the following papers:

T. Mar, V. Tikhanoff, L. Natale "What can I do with this tool? Self-supervised learning of tool affordances from their 3D geometry.", _IEEE Transactions on Cognitive and Developmental Systems 2017_. 
T. Mar, V. Tikhanoff, G. Metta, L. Natale "Self-supervised learning of tool affordances from 3D tool representation through parallel SOM mapping ", _ICRA 2017_, Singapore. 
T. Mar, V. Tikhanoff, G. Metta, L. Natale "Multi-model approach based on 3D functional features for tool affordance learning in robotics", _Humanoids 2015_, Singapore. 
T. Mar, V. Tikhanoff, G. Metta, L. Natale "Self-supervised learning of grasp dependent tool affordances on the iCub Humanoid robot ", _ICRA 2015_, Seattle. 

As well as in Tanis Mar's PhD Thesis: 
"Affordances for and from Manipulation: A Developmental Approach to Tool Use Learning on iCub."

## Dependencies
All modules require
- [YARP](https://github.com/robotology/yarp)
- [iCub](https://github.com/robotology/icub-main)
- [icub-contrib-common](https://github.com/robotology/icub-contrib-common)

The experiments using 2D features require:
- [OpenCV](http://opencv.org/)

The experiments using 3D features interact with:
- [tool-incorporation](https://github.com/robotology/tool-incorporation)

## How to compile
In `Linux systems` code can be compiled as follows:
```
git clone https://github.com/robotology/tool-affordances.git
cd tool-affordances
mkdir build; cd build
ccmake ..
make install
```

## Modules
The modules are located under the `modules` directory. Given the number of different behaviors achieved by the modules in this repository, they can be divided according to the scenario:

Pull action with 2D features, as in the ICRA 2015 experiment:
* `affManager`: Manager module to coordinate the experiment published in ICRA 2015.
* `feat2Dext`: performs 2D feature extraction of a tool's blob, as in ICRA 2015's experiment.
* `positionFilter`: Provides location and template for a tracker as well as 3D location from 2D tracked position. 
 
Radial drag action with 3D features, as in Humanoids 2015, ICRA 2017 and TCDS 2017:
 * `aff-explorer-TCDS`: lua module to automatically run affordance exploration using the TCDS experiment scenario. 
  * `tool3DManager`: Manager module to cooridnate the experiment published in Humanoids 2015, ICRA 2017 and TCDS 2017, which apply 3D features and radial drag actions.
 
Table cleaning demo, involving zone dependent actions, tool incorporation and tool selection:
 * `aff-explorer`: lua module to automatically run affordance exploration using the demo's scenario
 * `table_cleaning_SM`: lua rFSM module to automatically run affordance exploration using the demo's scenario as a proper State Machine.
 * `drop-away`: lua module to send the drop-away command continuously for the port-monitored demo version.
  The demo can also be coordinated by means of port-monitors, using the PMs in /app/scripts/monitors
 
Generic, used in all behaviors: 
 * `affMotor`: Extension of [karmaMotor](https://github.com/robotology/karma) which adds radial drag and diametral slide actions.
 * `simtoolloader`: Enables easy loading of tool models in the simulator on the robot's hand, allowing the user to decide the orientation, tilt and shift of the tool. Sample tool models are provided in `./modules/simtoolloader/app/models/`
 * `affCollector`: Accepts labels of objects/tool/categories, and for each, based on pairs action-effect, computes and updates the success rate of each label for each action.


## Links to related repositories

 * 3D tool representation as well as pose and tooltip estimation, used in the demo and in all experiments using 3D information, are provided by the modules on: https://github.com/robotology/tool-incorporation

## License
Material included here is Copyright of _iCub Facility - Istituto Italiano di Tecnologia_
and is released under the terms of the GPL v2.0 or later. See the file LICENSE for details.
