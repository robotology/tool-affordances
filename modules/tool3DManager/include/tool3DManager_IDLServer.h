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
   * Adopt home position
   * @return true/false on success/failure
   */
  virtual bool goHome(const bool hands = 0);
  /**
   * Performs the sequence to get the tool: \n
   * - On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i> and displacement on the -Y hand axis <i>disp</i>. Uses magnet to hold it to hand.
   * - Moreover, the tool end effector is located and attached to the kinematic chain with karmaMotor and shown with karmaToolFinder.
   * - On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i> and <i> deg</i> should correspond to the way in which the tool is given
   * @return true/false on success/failure of looking at that position
   */
  virtual bool getTool(const int32_t tool = 0, const int32_t deg = 0, const double disp = 0);
  /**
   * Performs a slide action along the diameter of the circle of radius and center on the object, from theta to -theta. \n
   * The trial consist on locating the object and executing the slide action
   * @return true/false on success/failure to do Action
   */
  virtual bool slide(const double thetha = 0, const double radius = 0);
  /**
   * Performs a pull action from the object to the direction indicated by theta and radius. \n
   * The trial consist on locating the object and executing the slide action
   * @return true/false on success/failure to do Action
   */
  virtual bool pull(const double thetha = 0, const double radius = 0);
  /**
   * Computes the effect of the action in terms of distance displaced, angle of displacement and rotation exerted on the object. \n
   * @return true/false on success/failure to compute Effect
   */
  virtual bool compEff();
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif

