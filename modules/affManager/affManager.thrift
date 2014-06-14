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
     * Look at predefined rack position
     * @return true/false on success/failure of looking at that position    
    * bool lookAtRack();
     */

    /**
     * Look at user given coordinates
     * @return true/false on success/failure of looking at that position     
    * bool lookAtPoint();
     */

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
     * Look at the target object
     * @return true/false on success/failure of finding/looking at object
     */
    bool lookAtObject();

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
     */
    bool observeObj();
    
    /**
     * Attempts to recognise a tool on the actual visual field
     * @returns the number of potential tools found.    
    * i32 findTools();   
     */

    /**
     * Selects one tool among many
     * @return true/false on success/failure to select      
    * bool selectTool();
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
     * Executes a sliding action (push or draw) using the end-effector (tool or hand)
     * @return true/false on success/failure 
     * to select 
     */
    bool slideAction();

    
    /**
     * Quit the module
     * @return true/false on success/failure
     */
    bool quit();
    }
