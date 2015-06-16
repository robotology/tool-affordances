#tool3DManager.thrift

/**
* tool3DManager_IDLServer
*
* Interface. 
*/

service tool3DManager_IDLServer
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
    bool goHome(1:bool hands = false);
    
    /**
     * Performs the sequence to get the tool: \n
     * - On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i> and displacement on the -Y hand axis <i>disp</i>. Uses magnet to hold it to hand.
     * - Moreover, the tool end effector is located and attached to the kinematic chain with karmaMotor and shown with karmaToolFinder.
     * - On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i> and <i> deg</i> should correspond to the way in which the tool is given 
     * @return true/false on success/failure of looking at that position    
     */
    bool getTool(1:i32 tool = 0, 2:i32 deg = 0, 3:double disp = 0);

    /**
     * Queries toolFeatExt module to extract the feaures of the loaded module \n
     * @return true/false on success/failure to extract features
     */
    bool getToolFeats();

    /**
     * Performs a slide action along the diameter of the circle of radius and center on the object, from theta to -theta. \n
     * The trial consist on locating the object and executing the slide action
     * @return true/false on success/failure to do Action
     */
    bool slide(1:double thetha = 0.0, 2:double radius = 0.0);

    /**
     * Performs a pull action from the object to the direction indicated by theta and radius. \n
     * The trial consist on locating the object and executing the slide action
     * @return true/false on success/failure to do Action
     */
    bool pull(1:double thetha = 0.0, 2:double radius = 0.0);


    /**
     * Computes the effect of the action in terms of distance displaced, angle of displacement and rotation exerted on the object. \n
     * @return true/false on success/failure to compute Effect
     */
    bool compEff();

    /**
     * Runs numAct actions with the given tool on the given pose and computes the effect. \n
     * @return true/false on success/failure to perform all actions
     */
    bool runToolPose(1: i32 toolI, 2: i32 graspOr = 0, 3: double graspDisp = 0.0, 4: i32 numAct = 8);

    /**
     * Runs numAct actions with the given tool on the given orientation, for the displacements {-2, 0, 2} and computes the effect. \n
     * @return true/false on success/failure to perform all actions
     */
    bool runToolOr(1: i32 toolI, 2: i32 graspOr = 0, 3: i32 numAct = 8);

    /**
     * For the given tool, performs N actions for each toolpose. Tries all toolposes as combinations
     * of grasp orientation {-90, 0, 90} and displacements { -2, 0, 2} cm. \n
     * @return true/false on success/failure to perfomr all actions on all toolPoses
     */
    bool runToolTrial(1: i32 toolI, 2: i32 numAct = 8);

    /**
     * Runs full trials for all tool with indices between toolini and toolEnd. \n
     * @return true/false on success/failure to perform all actions
     */
    bool runExp(1: i32 toolIni = 1, 2: i32 toolEnd = 50);



}



