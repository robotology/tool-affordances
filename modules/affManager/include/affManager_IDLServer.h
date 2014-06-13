// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_affManager_IDLServer
#define YARP_THRIFT_GENERATOR_affManager_IDLServer

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class affManager_IDLServer;


/**
 * affManager_IDLServer
 * Interface.
 */
class affManager_IDLServer : public yarp::os::Wire {
public:
  affManager_IDLServer() { yarp().setOwner(*this); }
/**
 * Start the module
 * @return true/false on success/failure
 */
  virtual bool start();
/**
 * Adopt home position
 * @return true/false on success/failure
 */
  virtual bool goHome();
/**
 * performs the sequence to get the tool from user, look at it and extract its features.
 * @return true/false on success/failure of looking at that position
 */
  virtual bool getTool();
/**
 * Asks for tool and move the arm to receiving position
 * @return true/false on success/failure on going to receive position
 */
  virtual bool askForTool();
/**
 * Closes hand on tool previously reached or received.
 * @return true/false on success/failure on holding the tool.
 */
  virtual bool graspTool();
/**
 * Moves the tool in hand to a comfortable lookable position, i.e., in front of iCubs eyes
 * @return true/false on success/failure of bringing the tool in front
 */
  virtual bool lookAtTool();
/**
 * Look at the target object
 * @return true/false on success/failure of finding/looking at object
 */
  virtual bool lookAtObject();
/**
 * Finds tool in hand and observes it extracting features)
 * @return true/false on success/failure finding and extracting feats from tool
 */
  virtual bool observeTool();
/**
 * Observes (extracts some features) of target object
 * @return true/false on success/failure finding and extracting feats from object
 */
  virtual bool observeObj();
/**
 * Uses active exploration and non-linear optimization to copmute the tool dimensions and attach its end-effector to the robot's arm.
 * Makes use of KarmaMotor, KarmaToolProjection and KarmaToolFinder
 * @return true/false on success/failure
 * to select
 */
  virtual bool attachTool();
/**
 * Executes the sequence to clear the visual field, look at the object, perform the action and observe the effect.
 * @return true/false on success/failure
 * to select
 */
  virtual bool doAction();
/**
 * Executes a sliding action (push or draw) using the end-effector (tool or hand)
 * @return true/false on success/failure
 * to select
 */
  virtual bool slideAction();
/**
 * Quit the module
 * @return true/false on success/failure
 */
  virtual bool quit();
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif

