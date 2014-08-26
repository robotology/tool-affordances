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
     * Quit the module
     * @return true/false on success/failure
     */
    bool quit();

    
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
     * Uses active exploration and non-linear optimization to compute the tool dimensions
     * Makes use of KarmaMotor, KarmaToolProjection and KarmaToolFinder
     * @return true/false on success/failure 
     */
    bool findToolDims();

    /**
     * Attaches its end-effector relative position (provided by findToolDims or else wise) to the robot's kinematic chain.
     * @return true/false on success/failure 
     */
    bool attachTool();


    /**
     * Moves the tool in hand to a position where it can be observed fully, i.e., in front of iCub eyes
     * @return true/false on success/failure of bringing the tool in front
     */
    bool lookAtTool();

    /**
     * Finds tool in hand and observes it extracting features
     * @return true/false on success/failure finding and extracting feats from tool
     */
    bool observeTool();


    /**
     * Gets position of the object from the user, and uses the template to train the particle filter tracker.
     * @return true/false on success/failure of finding/looking at object
     */
    bool trackObj();

    /**
     * Look and locate in 3D the target object
     * @return true/false on success/failure of finding/looking at object
     */
    bool locateObj();

    /**
     * Executes a sliding action using the end-effector (tool or hand)
     * @return true/false on success/failure 
     * to select 
     */
    bool slideAction(1:i32 approach = 0);

    /**
     * Computes the effect of the action as the difference in the position of the object before and after the slide action.
     * @return true/false on success/failure 
     * to select 
     */
    bool computeEffect();

        
    /**
     * performs the sequence to get the tool from user, look at it and extract its features.
     * @return true/false on success/failure of looking at that position    
     */
    bool getTool(1:i32 deg = 0);

    /**
     * Executes the sequence to clear the visual field, look at the object, perform the action and observe the effect.
     * @return true/false on success/failure 
     * to select 
     */
    bool doAction(1:i32 approach = 0);

     /**
     * Performs the drawing action a given number of times to learn the mapping
     * @return true/false on success/failure 
     * to select 
     */
    bool trainDraw(1:i32 pose = 0);

     /**
     * Performs the feature Extraction on the tool a given number of times from slighlty different prespectives
     * @return true/false on success/failure 
     * to select 
     */
    bool trainObserve();

     /**
     * Performs once the whole routine of looking at the tool getting its features ad then performing an action, getting also parameters and effect of the action
     * @return true/false on success/failure 
     * to select 
     */
    bool observeAndDo(1:i32 pose = 0);
}



