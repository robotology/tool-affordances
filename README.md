## Overview
This repository contains modules concerning the exploration and learning of tool affordances. 
The general learning schema consists of 3 phases: 
1. Exploration: After being provided a tool, iCub extracts a descriptor vector to represent it, and executes a series of actions, recording the achieved effects. 
2. Learning: After enough tool affordance data (that is, the effects achieved by each action with each tool) has been recorded, it is sent to an affordance learning model, which learns the relationship between the tool, the action, and the achieved effects. 
3. Prediction: Given any tool similar to the ones used diring training, the iCub extracts its descriptive features and feeds them to the trained affordance model, which in turn is able to predict the expected effect for any given action. This predictions can be used to select the best action for any task acheived through the action repertoire, or even to select another tool. 

A diagram of the described learning schema can be observed below:
![learnSchema](/toolAffCycle_noToolSel.png)

This general schema was used in several studies on tool affordances with iCub, listed [below](#related-publications).

Moreover, it provides the motor and coordination modules to run the [tool use demo](https://github.com/robotology/tool-affordances/tree/master/app/LUA/table_clean_SM) (click on image to see video on Youtube):
[![tool use demo video](/tool_use_vidprof.png)](https://www.youtube.com/watch?v=2Jmm4zel134)

## Modules
The modules are located under the `modules` directory. Given the number of different behaviors achieved by the modules in this repository, they can be divided according to their function:

#### Perception:
* `feat2Dext`: performs 2D feature extraction of a tool's blob, as in ICRA 2015's experiment.
* `positionFilter`: Provides location and template for a tracker as well as 3D location from 2D tracked position (used in ICRA 2015 experiment). 
 * `graspChecker`:  Module that coordinates the classification pipeline of OTFR, trained to discriminate whether the iCub's hand is holding any object or not.

#### Motor:
* `drop-away`: lua module to send the drop-away command continuously. It should be connected to ARE by the `pm_drop_clas` port monitor in order to generate the behavior of dropping away objects only when the hand is grasping something. 
 * `affMotor`: Extension of [karmaMotor](https://github.com/robotology/karma) which adds radial drag and diametral slide actions.
  
#### Memory
 * `simtoolloader`: Enables easy loading of tool models in the simulator on the robot's hand, allowing the user to decide the orientation, tilt and shift of the tool. Sample tool models are provided in `./modules/simtoolloader/app/models/`
 * `affCollector`: Accepts labels of objects/tool/categories, and for each, based on pairs action-effect, computes and updates the success rate of each label for each action.

#### Coordination
*Managers:*
(Relay and organize commands, but do not execute any behavior)
* `affManager`: Manager module to coordinate the experiment published in ICRA 2015.
* `tool3DManager`: Manager module to cooridnate the experiment published in Humanoids 2015, ICRA 2017 and TCDS 2017, which apply 3D features and radial drag actions.

*State Machines:*
(Upon execution generate a given behavior based on inputs and states)
* `table_cleaning_SM`: lua rFSM module to automatically run the [table cleaning demo](https://github.com/robotology/tool-affordances/tree/master/app/LUA/table_clean_SM), involving zone dependent actions, tool incorporation and tool selection:
* `aff-explorer`: lua module to automatically run affordance exploration using the demo's scenario.
* `aff-explorer-TCDS`: lua module to automatically run affordance exploration using the TCDS experiment scenario. 

#### Data analysis and learning
The `MATLAB` folder contains all the functions for data analysis and visualization applied in the studies listed below. These functions are conveniently organized in the following folders:
 * `readData`: Contains the functions required to read, load and format the data recorded through exploration (performed on the robot or simulation by means of the C++ modules listed above).
 * `learning`: Contains the functions in charge of data processing, analysis and learning used to train the affordance models, as described in the papers listed below. 
 * `plot`: Contains the functions used for visualization of data, processing steps and results.
 * `scritps`: Contains scritps which organize the calls to the functions above ina complete analysis pipeline. 


## Use
The modules in this repository are intended for two main uses:
* Execution of tool affordance exploration and learning experiments. To this end, the modules `affManager` and `tool3DManager` provide a set of rpc commands that enable exploratory actions to be performed either individually or in a cycle for a larger number of tools. More information about the available commands can be obtained through the `rpc help` command provided by either module. 
* Execution of the table cleaning demo. Due to its complexity and dependency on external modules, the description of the demo provided separaterly on the following link: https://github.com/robotology/tool-affordances/tree/master/app/LUA/table_clean_SM

## How to compile
In `Linux systems` code can be compiled as follows:
```
git clone https://github.com/robotology/tool-affordances.git
cd tool-affordances
mkdir build; cd build
ccmake ..
make install
```

## Dependencies
All modules require
- [YARP](https://github.com/robotology/yarp)
- [iCub](https://github.com/robotology/icub-main)
- [icub-contrib-common](https://github.com/robotology/icub-contrib-common)

The experiments using 2D features require:
- [OpenCV](http://opencv.org/)

3D tool representation as well as pose and tooltip estimation, used in the demo and in all experiments using 3D information, are provided by the modules on: 
- [tool-incorporation](https://github.com/robotology/tool-incorporation)

## Related Publications
The methods executed by the modules in this repository have been described in detail in the following papers:

- T. Mar, V. Tikhanoff, L. Natale "What can I do with this tool? Self-supervised learning of tool affordances from their 3D geometry.", _IEEE Transactions on Cognitive and Developmental Systems 2017_. 
- T. Mar, V. Tikhanoff, G. Metta, L. Natale "Self-supervised learning of tool affordances from 3D tool representation through parallel SOM mapping ", _ICRA 2017_, Singapore. 
- T. Mar, V. Tikhanoff, G. Metta, L. Natale "Multi-model approach based on 3D functional features for tool affordance learning in robotics", _Humanoids 2015_, Singapore. 
- T. Mar, V. Tikhanoff, G. Metta, L. Natale "Self-supervised learning of grasp dependent tool affordances on the iCub Humanoid robot ", _ICRA 2015_, Seattle. 

As well as in Tanis Mar's PhD Thesis: 

"Affordances for and from Manipulation: A Developmental Approach to Tool Use Learning on iCub."

## License
Material included here is Copyright of _iCub Facility - Istituto Italiano di Tecnologia_
and is released under the terms of the GPL v2.0 or later. See the file LICENSE for details.
