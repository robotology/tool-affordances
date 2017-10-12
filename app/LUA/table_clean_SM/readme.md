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

## Retraining
Of course, this classifier can be retrained to adapt for new tools. In order to see how to do that, the ` onTheflyRecognition_with_tools` app is recommended, although not mandatory (check the [OTFR documentation](https://github.com/robotology/onthefly-recognition)). In that case, however, the corresponding 3D models of the new tools will have to be provded making the label and model name of each tool match exactly. See the [tool-incorporation documentation](https://github.com/robotology/tool-incorporation) for more information about how and where to do so.

Also, the demo relies on a second classifier to make sure whether attempted grasps on objects have been successful or not. This classifier relies on the `graspChecker` module to manage the custom OTFR pipeline. In case it is necessary to retrain it, it can be easily done using the command `train empty/full` offered by the tool3DManager module. (For ease, objects can be placed in the robot's hand using `expect` command offered by ARE. 
