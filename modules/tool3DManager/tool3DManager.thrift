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
     * Performs a slide action from orientation theta and distance radius to the detected center of the object. \n
     * The trial consist on locating the object and executing the slide action
     * @return true/false on success/failure to do Action
     */
    bool slide(1:double thetha = 0.0, 2:double radius = 0.0);

    /**
     * Computes the effect of the action in terms of distance displaced, angle of displacement and rotation exerted on the object. \n
     * @return true/false on success/failure to compute Effect
     */
    bool compEff();


}



