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
     * Performs several routines until it finds a tool
     * @return true/false on success/failure of finding a tool in any of its possible locations
     */
    bool lookAtTool();
    
    /**
     * Look at predefined rack position
     * @return true/false on success/failure of looking at that position
     */
    bool lookAtRack();
    
    /**
     * Look at user given coordinates
     * @return true/false on success/failure of looking at that position
     */
    bool lookAtPoint();
    
    /**
     * Look at the target object
     * @return true/false on success/failure of finding/looking at object
     */
    bool lookAtObject();

    
    /**
     * Asks for tool and move the arm to receiving position
     * @return true/false on success/failure on going to receive position
     */
    bool askForTool();    
    
    /**
     * Robot reaches for the tool without grasping
     * @return true/false on success/failure on computing and reaching the desired point
     */
    bool reachTool();
    
    /**
     * Closes hand on tool previously reached or received.
     * @return true/false on success/failure on holding the tool.
     */
    bool graspTool();
    
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
     */
    i32 findTools();   
    
    /**
     * Selects one tool among many
     * @return true/false on success/failure 
     * to select 
     */
    bool selectTool();
    
    /**
     * Quit the module
     * @return true/false on success/failure
     */
    bool quit();


}
