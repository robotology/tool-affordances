## Application
This state machine coordinates the modules described on the [tool use demo application](https://github.com/robotology/tool-affordances/blob/master/app/scripts/tool_use_demo.xml.template), which is organized according to the following architecture:
![appArchit](/TableCleaning_Architecture_3.png)

## Behavior
Thereby, it allows the iCub to achieve the task of cleaning the table in front of it of objects, using either its own hand, or tools if required, to bring objects closer and place them in a box to its left side.
An example execution of such behavior can be observed on the following video (click image to see video on Youtube):
[![tool use demo video](/tool_use_vidprof.png)](https://www.youtube.com/watch?v=2Jmm4zel134)

## Execution
In order to run this demo, two xml applications need to be launched:
- TOOL_USE_DEMO: loads all the required modules except for the tool recognition ones.
- TOOL_USE_OTFR: loads the tool recognition modules. In fact, this application is an instance of the OnTheFlyRecognition (OTFR), pre-trained with the tools commonly use in the demo, namely, rake, hook, hoe, shovel and stick, shown below:
![demoTools](/demoTools.png)
Of course, this classifier can be retrained to adapt for new tools, in order to see how to do that, check the [OTFR documentation](https://github.com/robotology/onthefly-recognition).
When all modules are started, the rFSM GUI will open the table-clean state machine. In order to start the demo, just click on the play button. The robot will go to home position to observe the table, and react to objects placed on the table, as can be observed in the video above.
