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
 * Performs several routines until it finds a tool
 * @return true/false on success/failure of finding a tool in any of its possible locations
 */
  virtual bool lookAtTool();
/**
 * Look at predefined rack position
 * @return true/false on success/failure of looking at that position
 */
  virtual bool lookAtRack();
/**
 * Look at user given coordinates
 * @return true/false on success/failure of looking at that position
 */
  virtual bool lookAtPoint();
/**
 * Look at the target object
 * @return true/false on success/failure of finding/looking at object
 */
  virtual bool lookAtObject();
/**
 * Asks for tool and move the arm to receiving position
 * @return true/false on success/failure on going to receive position
 */
  virtual bool askForTool();
/**
 * Robot reaches for the tool without grasping
 * @return true/false on success/failure on computing and reaching the desired point
 */
  virtual bool reachTool();
/**
 * Closes hand on tool previously reached or received.
 * @return true/false on success/failure on holding the tool.
 */
  virtual bool graspTool();
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
 * Attempts to recognise a tool on the actual visual field
 * @returns the number of potential tools found.
 */
  virtual int32_t findTools();
/**
 * Selects one tool among many
 * @return true/false on success/failure
 * to select
 */
  virtual bool selectTool();
/**
 * Quit the module
 * @return true/false on success/failure
 */
  virtual bool quit();
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif

