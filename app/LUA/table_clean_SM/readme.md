# Demo overview
## Application
This state machine coordinates the modules described on the [tool use demo application](https://github.com/robotology/tool-affordances/blob/master/app/scripts/tool_use_demo.xml.template), which is organized according to the following architecture:
![architecture](https://github.com/robotology/tool-affordances/blob/master/app/LUA/table_clean_SM/TableCleaning_Architecture_3.png)

## Behavior
Thereby, it allows the iCub to achieve the task of cleaning the table in front of it of objects, using either its own hand, or tools if required, to bring objects closer and place them in a box to its left side.
An example execution of such behavior can be observed on the following video (click image to see video on Youtube):
[![tool use demo video](/tool_use_vidprof.png)](https://www.youtube.com/watch?v=2Jmm4zel134)

## Execution
In order to run this demo, two xml applications need to be launched:
- TOOL_USE_DEMO: loads all the required modules except for the tool recognition ones.
- TOOL_USE_OTFR: loads the tool recognition modules. In fact, this application is an instance of the OnTheFlyRecognition (OTFR), pre-trained with the tools commonly use in the demo, namely, rake, shovel and stick, shown below:

![demo_tools](https://github.com/robotology/tool-affordances/blob/master/app/LUA/table_clean_SM/Demo_tools.png)

When all modules are started, the rFSM GUI will open the table-clean state machine. In order to start the demo, just click on the play button. The robot will go to home position to observe the table, and react to objects placed on the table, as can be observed in the video above.

For understanding and debugging purposes, a functional walkthrough of the demo is provided on the following document, where the relevant function calls and all the rpc calls are listed and briefly explained: 

[Walkthough](https://github.com/robotology/tool-affordances/blob/master/app/LUA/table_clean_SM/demoFunctionCalls.md)

# Further info
## Affordance knowledge
In the current state of the demo, the knowledge of the affordances that each of the considered tool-poses offer is provided by the [affCollector](https://github.com/robotology/tool-affordances/tree/master/modules/affCollector) module, which in turns reads it from the file `affs_DEMO.txt` where they have been pre-trained. The affordances of new tools can be added, by adding the corresponding entries in the file (ideally learned by exploration by the robot). Note, however, that in that case this new tools would also have to trained in the classifier, as explained below. 

## (Re)Training
The demo requires two trained classifiers for successful execution; to recognize the tool given by the user to the robot, and to discriminate between full and empty hand after grasp attempts. On its current state, both are implemented making use of the `on the fly recognition (OTFR)` pipeline, although in slighly different manners. 

The tool recognition classifier can be trained using the classical OTFR pipeline in `human` mode, where objects are being shown by the user, or using the OTFR `robot tool` mode, where the robot holds the tool and observes them autonomously. In either case, the use of the ` onTheflyRecognition_with_tools` app is recommended, although not mandatory (check the [OTFR documentation](https://github.com/robotology/onthefly-recognition)). In order to avoid the need to train the classifer after cloning, the set of tools commonly used for the demo (rake, stick, shovel), are readily available on the `tool-incorporation` repo. Of course, this classifier can be retrained to adapt for new tools. In that case, however, the corresponding 3D models of the new tools will have to be provded making the label and model name of each tool match exactly. See the [tool-incorporation documentation](https://github.com/robotology/tool-incorporation) for more information about how and where to do so.

The classifier in charge of evaluating grasp success uses a different pipeline, given that the images should not be cropped by disparity but around the projection of the frame of the hand holding the object. In order to do so, this classifier relies on the `graspChecker` module to manage the custom OTFR pipeline. As in the case of tools, a trained dataset is readily available [here](https://github.com/robotology/tool-affordances/tree/master/app/conf), which needs to be copied on the local context path of the `himrep` module. In case this classifier needs to be retrained, it can be easily done using the command `train` offered by the tool3DManager module giving either `empty` or `full` as labels. (For ease, objects can be placed in the robot's hand using `expect` command offered by ARE. 

## Dependencies
Because of the complexity and multi-modality of this demo, it depends in a number of repositories:
- [tool-affordances](https://github.com/robotology/tool-affordances).
- [tool-incorporation](https://github.com/robotology/tool-incorporation).
- [segmentation](https://github.com/robotology/segmentation) (for `lbpExtract` and `dispBlobber` and/or `graph-based-segmentation` modules).
- [stereo-vision](https://github.com/robotology/stereo-vision) (for `SFM` module)
- [segmentation-to-pointcloud](https://github.com/robotology/segmentation-to-pointcloud) (for `seg2cloud` module).
- [onTheFlyRecognition](https://github.com/robotology/onthefly-recognition) and [himrep](https://github.com/robotology/himrep) for the object classificaton modules.
