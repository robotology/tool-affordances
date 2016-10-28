// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_tool3DManager_IDLServer
#define YARP_THRIFT_GENERATOR_tool3DManager_IDLServer

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>
#include <yarp/sig/Vector.h>

class tool3DManager_IDLServer;


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
   * Sets segmentation to 2D (true) or 3D (false)
   * @return true/false on success/failure to toggle segmentation method.
   */
  virtual bool setSeg(const bool seg = 0);
  /**
   * Loads the tool on objects3DExplorer and changes name
   * @return true/false on success/failure on setting name
   */
  virtual bool loadModel(const std::string& tool);
  /**
   * Performs the sequence to get the tool: \n
   * - On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i>, tilted at <i>tilt</i> and with a displacement on the -Y hand axis <i>disp</i>. Uses magnet function to hold it to hand.
   * - Moreover, the tool end effector is located and attached to the kinematic chain with affMotor and shown on the viewer
   * - On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i>, <i>deg</i>, <i>disp</i>  and <i>tilt</i> should correspond to the way in which the tool is given
   * @return true/false on success/failure of loading the tool with correct pose
   */
  virtual bool getToolParam(const std::string& tool, const double deg = 0, const double disp = 0, const double tilt = 45, const double shift = 0);
  /**
   * Performs the sequence to get the tool: \n
   * - Grasp (through ARE)
   * - Load tool in objects3Dexplorer (by setToolName)
   * - Find pose and tooltip with align method (by findPose)
   * @return true/false on success/failure of grasping and loading the named tool
   */
  virtual bool getToolAlign(const std::string& tool);
  /**
   * Communicates with ARE and KM to grasp a tool and move it to the center.
   * @return true/false on success/failure
   */
  virtual bool graspTool(const std::string& tool);
  /**
   * Communicates with O3DE to explore the tool and get the tooltip without a model
   * @return true/false on success/failure
   */
  virtual bool explore();
  /**
   * Communicates with KM  move the tool to the center.
   * @return true/false on success/failure
   */
  virtual bool lookTool();
  /**
   * Move tool in hand (sim) and change kinematic extension (sim and real).
   * @return true/false on success/failure of regrasping the tool
   */
  virtual bool regrasp(const double deg = 0, const double disp = 0, const double tilt = 45, const double shift = 0);
  /**
   * Start the methods to find the tool pose by aligning the partial reconstruction with a tool model: \n
   * @return true/false on success/failure of finding the tool pose
   */
  virtual bool findPose();
  /**
   * Queries toolFeatExt module to extract the feaures of the loaded module \n
   * @return true/false on success/failure to extract features
   */
  virtual bool getToolFeats();
  /**
   * Finds out the location of the object and returns its coordinates \n
   * @return Vector bottle with object coordinates in 3D X Y Z
   */
  virtual yarp::sig::Vector objLoc();
  /**
   * Finds out the rotation of the object and returns its rotation values \n
   * @return Vector bottle with object rotation
   */
  virtual yarp::sig::Vector objRot();
  /**
   * Command to train tools by their label
   * @return true/false on success/failure to train classifiers.
   */
  virtual bool learn(const std::string& label);
  /**
   * Checks whether the hand is full or empty
   * @return true/false  corresponding to full or empty hand
   */
  virtual bool check();
  /**
   * Adopt home position (optionally opens hands too)
   * @return true/false on success/failure
   */
  virtual bool goHome(const bool hands = 0);
  /**
   * Calibrates the table height through ARE: \n
   * @param calib . True performs calibration, false gets stored data.
   * @returns true
   */
  virtual bool findTable(const bool calib = 1);
  /**
   * Performs a slide action along the diameter of the circle of given radius and center on the object, from theta to -theta. \n
   * @return true/false on success/failure to do Action
   */
  virtual bool slide(const double thetha = 0, const double radius = 0);
  /**
   * Performs a drag action from the object in the direction indicated by theta and radius. \n
   * @return true/false on success/failure to do Action
   */
  virtual bool drag(const double thetha = 0, const double radius = 0, const double tilt = -15);
  /**
   * Performs a drag action from the given 3D coords in the direction indicated by theta and radius. \n
   * The trial consist on locating the object and executing the slide action
   * @return true/false on success/failure to do Action
   */
  virtual bool drag3D(const double x, const double y, const double z, const double thetha = 0, const double radius = 0, const double tilt = -15, const bool useTool = 1);
  /**
   * (Re)Initializes object template for tracking. The user has to click on the upper left and lower right corners of the object to be tracked (in that order).\n
   * @return true/false on success/failure to set the template and (re)start tracking
   */
  virtual bool trackObj();
  /**
   * Computes the effect of the action in terms of distance displaced, angle of displacement and rotation exerted on the object. \n
   * @return true/false on success/failure to compute Effect
   */
  virtual bool compEff();
  /**
   * Push the cube back in teh orginal position (in SIM)
   * @return true/false on success/failure to compute Effect
   */
  virtual bool resetObj();
  /**
   * Runs numAct actions for numPoses times, each time with a random tool in a random pose
   * @return true/false on success/failure to perform all actions
   */
  virtual bool runRandPoses(const int32_t numPoses, const int32_t numAct = 8);
  /**
   * Runs numAct actions with the given tool on the given pose and computes the effect. \n
   * @return true/false on success/failure to perform all actions
   */
  virtual bool runToolPose(const int32_t numRep = 1, const int32_t numAct = 8);
  /**
   * For the given tool, performs numAct actions for each toolpose. Tries all grasp orientation {-90, 0, 90}
   * @return true/false on success/failure to perfomr all actions on all toolPoses
   */
  virtual bool runToolTrial(const int32_t numRep = 1, const std::string& tool = "", const int32_t numAct = 8);
  /**
   * Runs full trials for all tool with indices between toolini and toolEnd. \n
   * @return true/false on success/failure to perform all actions
   */
  virtual bool runExp(const std::string& tool, const int32_t toolEnd = 54);
  /**
   * Extracts OMS-EGI features from grasped tool and calls MATLAB to get the predicted effects of possible action
   * Then, chooses the best action for the given goal (1: maxDist, 2: Pull) \n
   * @return true/false on success/failure to query matlab and perform action
   */
  virtual bool selectAction(const int32_t goal = 1);
  /**
   * Runs selectAction trials for all the test tools for given goal (1: maxDist, 2: Pull)
   * @return true/false on success/failure to perform actions selections
   */
  virtual bool predExp(const int32_t goal = 1);
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif
