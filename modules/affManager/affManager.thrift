#affManager.thrift

/**
* affManager_IDLServer
*
* Interface. 
*/

service affManager_IDLServer
{
    /**
     * Start the module
     * @return true/false on success/failure
     */
    bool start();
    
    /**
     * Adopt home position
     * @return true/false on success/failure
     */
    bool goHome();
    
    /**
     * Adopt home position while keeping hand pose
     * @return true/false on success/failure
     */
    bool goHomeNoHands();
    
    /**
     * performs the sequence to get the tool from user, look at it and extract its features.
     * @return true/false on success/failure of looking at that position    
     */
    bool getTool();

    /**
     * Allows the user to define a label for the tool configuration to deal with
     * @return true/false on success/failure of looking at that position    
     */
    bool setTool(1:string tpName="undef");
        
    /**
     * Asks for tool and move the arm to receiving position
     * @return true/false on success/failure on going to receive position
     */
    bool askForTool();  
    
    /**
     * Closes hand on tool previously reached or received.
     * @return true/false on success/failure on holding the tool.
     */
    bool graspTool();  

    /**
     * Moves the tool in hand to a comfortable lookable position, i.e., in front of iCubs eyes
     * @return true/false on success/failure of bringing the tool in front
     */
    bool lookAtTool();

    /**
     * Gets user object box and learns it for tracking
     * @return true/false on success/failure of finding/looking at object
     */
    bool  trackObj();

    /**
     * Look and locate in 3D the target object
     * @return true/false on success/failure of finding/looking at object
     */
    bool  locateObj();

    /**
     * Robot reaches for the tool without grasping
     * @return true/false on success/failure on computing and reaching the desired point     
    * bool reachTool();
    */

    /**
     * Finds tool in hand and observes it extracting features)
     * @return true/false on success/failure finding and extracting feats from tool
     */
    bool observeTool();
    
    /**
     * Observes (extracts some features) of target object
     * @return true/false on success/failure finding and extracting feats from object
     * bool observeObj();
    */
    
    /**
     * Uses active exploration and non-linear optimization to copmute the tool dimensions and attach its end-effector to the robot's arm.
     * Makes use of KarmaMotor, KarmaToolProjection and KarmaToolFinder
     * @return true/false on success/failure 
     * to select 
     */
    bool attachTool();

    /**
     * Executes the sequence to clear the visual field, look at the object, perform the action and observe the effect.
     * @return true/false on success/failure 
     * to select 
     */
    bool doAction();

     /**
     * Performs the drawing action a given number of times to learn the mapping
     * @return true/false on success/failure 
     * to select 
     */
    bool trainDraw();

     /**
     * Performs once the whole routine of looking at the tool getting its features ad then performing an action, getting also parameters and effect of the action
     * @return true/false on success/failure 
     * to select 
     */
    bool observeAndDo();


    /**
     * Executes a sliding action (push or draw) using the end-effector (tool or hand)
     * @return true/false on success/failure 
     * to select 
     */
    bool slideAction();

    /**
     * Computes the effect of the action as the difference in the position of the object before and after the slide action.
     * @return true/false on success/failure 
     * to select 
     */
    bool computeEffect();

    
    /**
     * Quit the module
     * @return true/false on success/failure
     */
    bool quit();
    }
