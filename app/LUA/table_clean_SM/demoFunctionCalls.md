## Demo function calls walkthrough

The intention of this document is to provide detailed description of the relevant functions called during the tool use demo in order to facilitate understanding and debugging. To that end, relevant function calls within each module, as well as *all* rpc calls to other modules (from where the call is made) are listed and briefly explained. 

Functions and rpc calls are listed in the order in which they will appear on a complete execution of the demo. For the sake of clarity hierarchical levels and different types of calls are indicated using the following format:

* Different indentations indicate functions called within parent function (previous indentation).

* **Bold** represents RPC call to another module. The first element indicates the module to which the rpc call is sent, while the second element, within " " quote marks, represents the command sent.

* *Italic* represents a functon call within the module.

## Functions and calls by state

### OBSERVE
State task: Look at table and if there are stable blobs, select one to act upon.

*Calls:*

* *update_object_list()* Read blob 2D position (from `lbpExtract`).
* *get_stable_object_count()* Check for not moving objects based on 2D coords.
* *get_3d_pos()* Compute the 3D position of each stable blob.
  * **ARE:get "s2c"** Use homography to get 3d coords from 2d coords and table height.
* *update_zone()*: Assign a zone to each detected stable object.
* *check_left_arm_busy()*:
  * *get_are_status()* 
    * **ARE:get "get status"** Get status of whether an action is being performed.
  * *get_are_holding()*
    * **ARE:get "get holding"** Check by encoders whether hand is holding anything.
* select_object(): Select a blob from the detected stable ones, based on proximity to grasp

### SELECT ACTION
State Task: Based on the position of the target object, select the best action to do, or decide a tool is needed if no action is possible

*Calls:*

* *select_action()*: Select action to do on target object, based on position.
  * If close enough, select grasp (left side) or push (right side).
  * If it is not reachable, check whether the tool in the hand allows action:
  * *get_tool_affordance()*
    * **affCollector "getAffs"** Returns affordances of active label tool.
    * If any action is doable, do the preferred possible one
    * If there is no tool, or the tool in hand does not afford any action, return `not_affordable`
		
* If `not_affordable` returned, then go to  `get_tool` state
* If any action is affordable on the target object, go to `do_action` state

### GET TOOL
State task: Ask for a tool, recognize it, estimate its pose and correspondingly extend end-effector.

*Calls:* 

* *ask_for_tool(<label>)* Get a tool, recognizes it, and estimates pose.
  * **T3DM "graspTool"** Open hand, grasp tool, observe it and recognize it.
    * **AffMotor "tool remove"** Remove previous tool's end-effector extension.
    * **ToolInc "cleartool"** Remove model and info from previous tool.
 	* **ARE:cmd.io "tato right"** Extend hand to wait for tool.
    * **ARE:cmd.io "hand close_hand_tool"** Close hand with in tool grasp.
    * **ARE:get "get holding"** Check by encoders whether hand is holding anything.
    * If label is `unknown`
	  * **ToolInc "recog"** Recognize tool using OTFR trained classifier.
  * **T3DM "findPose"** Find pose and end-effector of tool in hand.
    * **ToolInc "findTooltipAlign"** Find pose and tooltip of tool in hand by aligning partial 3D reconstruction with loaded model.
      * *getPointCloud()* Get the partial 3D reconstruction and filters it.
        * **seg2Cloud "flood3d"** Segment and reconstruct cloud based on depth distance among pixels.
        * Filters outliers and limits x,y and z around hand.
      * *alignWithScale()* Use ICP to align partial reconstruction with tool model, several reconstruction scales are tried.
* set_tool_label() Communicates the label of recognized tool to affCollector.
  * **affCollector "setlabel <tool_pose_name>"** Make the given tool-pose the active label.

### DO ACTION

State task: Perform the selected action on the selected object

*Calls:*

* *perform_action(action, object)* Execute the desired action on the desired object.
  * If object reachable left:
    * **ARE:cmd:io "take <target + offsets> left"** 
  * If object reachable right:
    * **ARE:cmd:io "drag3D <target + offsets> <no_tool>"** 
  * If object non-reachable 
    * **affMotor "drag_<direction> <target + offsets> <direction>"**

