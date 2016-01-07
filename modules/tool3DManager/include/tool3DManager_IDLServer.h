// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_tool3DManager_IDLServer
#define YARP_THRIFT_GENERATOR_tool3DManager_IDLServer

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class tool3DManager_IDLServer;


/**
 * tool3DManager_IDLServer
 * Interface.
 */
class tool3DManager_IDLServer : public yarp::os::Wire {
public:
  tool3DManager_IDLServer();
  /**
   * Start the module
   * @return true/false on success/failure
   */
  virtual bool start();
  /**
   * Quit the module
   * @return true/false on success/failure
   */
  virtual bool quit();
  /**
   * Sets the table height: \n
   * @returns true
   */
  virtual bool settableheight(const double th = -0.1);
  /**
   * Adopt home position
   * @return true/false on success/failure
   */
  virtual bool goHome(const bool hands = 0);
  /**
   * Move arm with tool to center to have tool in visual field and check  grasping.
   * @return true/false on success/failure
   */
  virtual bool centerTool();
  /**
   * Performs the sequence to get the tool: \n
   * - On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i>, tilted at <i>tilt</i> and with a displacement on the -Y hand axis <i>disp</i>. Uses magnet function to hold it to hand.
   * - Moreover, the tool end effector is located and attached to the kinematic chain with karmaMotor and shown with karmaToolFinder.
   * - On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i>, <i>deg</i>, <i>disp</i>  and <i>tilt</i> should correspond to the way in which the tool is given
   * @return true/false on success/failure of looking at that position
   */
  virtual bool getTool(const int32_t tool = 0, const double deg = 0, const double disp = 0, const double tilt = 45);
  /**
   * Performs the sequence to get the tool: \n
   * - On the simulator calls simtoolloader to rotate the handled tool  <i>tool</i> at the orientation <i>deg</i>, tilted at <i>tilt</i> and with a displacement on the -Y hand axis <i>disp</i>.
   * - The new tool end effector position is located and attached to the kinematic chain with karmaMotor and shown with karmaToolFinder.
   * - On the real robot the robot does not regrasp, but updates is end-effector position to match the real tool pose.
   * @return true/false on success/failure of looking at that position
   */
  virtual bool regrasp(const double deg = 0, const double disp = 0, const double tilt = 45, const double Z = 0);
  /**
   * Queries toolFeatExt module to extract the feaures of the loaded module \n
   * @return true/false on success/failure to extract features
   */
  virtual bool getToolFeats();
  /**
   * Performs a slide action along the diameter of the circle of radius and center on the object, from theta to -theta. \n
   * The trial consist on locating the object and executing the slide action
   * @return true/false on success/failure to do Action
   */
  virtual bool slide(const double thetha = 0, const double radius = 0);
  /**
   * Performs a drag action from the object to the direction indicated by theta and radius. \n
   * The trial consist on locating the object and executing the slide action
   * @return true/false on success/failure to do Action
   */
  virtual bool drag(const double thetha = 0, const double radius = 0, const double tilt = 0);
  /**
   * (Re)Initializes object tracking. The user has to click on the upper left and lower right corners of the object to be tracked (in that order).\n
   * @return true/false on success/failure to set the template and (re)start tracking
   */
  virtual bool trackObj();
  /**
   * Computes the effect of the action in terms of distance displaced, angle of displacement and rotation exerted on the object. \n
   * @return true/false on success/failure to compute Effect
   */
  virtual bool compEff();
  /**
   * Runs numAct actions for numPoses times, each time with a random tool in a random pose
   * @return true/false on success/failure to perform all actions
   */
  virtual bool runRandPoses(const int32_t numPoses, const int32_t numAct = 8);
  /**
   * Runs numAct actions with the given tool on the given pose and computes the effect. \n
   * @return true/false on success/failure to perform all actions
   */
  virtual bool runToolPose(const int32_t toolI, const double deg = 0, const double disp = 0, const double tilt = 45, const int32_t numAct = 8);
  /**
   * Runs numAct actions with the given tool on the given orientation, for the displacements {-2, 0, 2} and computes the effect. \n
   * @return true/false on success/failure to perform all actions
   */
  virtual bool runToolOr(const int32_t toolI, const double graspOr = 0, const int32_t numAct = 8);
  /**
   * For the given tool, performs N actions for each toolpose. Tries all toolposes as combinations
   * of grasp orientation {-90, 0, 90} and displacements { -2, 0, 2} cm. \n
   * @return true/false on success/failure to perfomr all actions on all toolPoses
   */
  virtual bool runToolTrial(const int32_t toolI, const int32_t numAct = 8);
  /**
   * Runs full trials for all tool with indices between toolini and toolEnd. \n
   * @return true/false on success/failure to perform all actions
   */
  virtual bool runExp(const int32_t toolIni = 1, const int32_t toolEnd = 54);
  /**
   * Extracts OMS-EGI features from grasped tool and calls MATLAB to get the predicted effects of possible action
   * Then, chooses the best action for the given goal (1: maxDist, 2: Pull) \n
   * @return true/false on success/failure to perform all actions
   */
  virtual bool selectAction(const int32_t goal = 1);
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif

