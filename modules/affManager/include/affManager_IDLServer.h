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
  affManager_IDLServer();
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
   * Sets the experiment flow flags to false (action done, object located, tip on view).
   * @return true/false on success/failure
   * to select
   */
  virtual bool reset();
  /**
   * Adopt home position
   * @return true/false on success/failure
   */
  virtual bool goHome();
  /**
   * Adopt home position while keeping hand pose  (tools remain grasped)
   * @return true/false on success/failure
   */
  virtual bool goHomeNoHands();
  /**
   * Uses active exploration and non-linear optimization to compute the tool dimensions (only on real robot)
   * Makes use of KarmaMotor, KarmaToolProjection and KarmaToolFinder
   * @return true/false on success/failure
   */
  virtual bool findToolDims();
  /**
   * Moves the tool in hand in front of iCub eyes to a position where it can be observed fully.
   * @return true/false on success/failure of bringing the tool in front
   */
  virtual bool lookAtTool();
  /**
   * Finds tool in hand and does Feature Extraction.
   * @return true/false on success/failure finding and extracting feats from tool
   */
  virtual bool observeTool();
  /**
   * Gets position of the object from the user, and uses the template to train the particle filter tracker.
   * @return true/false on success/failure of finding/looking at object
   */
  virtual bool trackObj();
  /**
   * Performs the sequence to get the tool: \n
   * - On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i> and uses magnet to hold it to hand.
   * - On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i> and <i> deg</i> should correspond to the way in which the tool is given
   * @return true/false on success/failure of looking at that position
   */
  virtual bool getTool(const int32_t tool = 5, const int32_t deg = 0);
  /**
   * Performs an pull trial on <i>approach</i> cm wrt the object. \n
   * The trial consist on locating the object, executing the pull, locating the potentially displaced object and computing the effect.\n
   * @return true/false on success/failure to do Action
   */
  virtual bool doAction(const int32_t approach = 0);
  /**
   * Performs several pull trials with approaches from -5 to 5 cm to learn the mapping:
   * @return true/false on success/failure
   */
  virtual bool trainDraw();
  /**
   * Performs  feature extraction on the given tool 5 times from slighlty different prespectives
   * @return true/false on success/failure
   */
  virtual bool trainObserve(const int32_t tool = 5, const int32_t deg = 0);
  /**
   * Performs the whole routine a given number of trials with the given tool in the given orientation:  looking at the tool, feature extraction, perform a pull action, and compute the effect. \n
   * @return true/false on success/failure
   */
  virtual bool observeAndDo(const int32_t tool = 5, const int32_t deg = 0, const int32_t trials = 1);
  /**
   * Gets a tool, observes it (feature extraction), reads the predicted affordance from MATLAB and perform the best predicted action.
   * Needs matlab script running prediction based on the model.
   * @return true/false on success/failure
   */
  virtual bool predictDo(const int32_t tool = 5, const int32_t deg = 0);
  /**
   * Performs the prediction and action (predictDo routine) 5 times on each orientation with the given tool.
   * @return true/false on success/failure
   */
  virtual bool testPredict(const int32_t tool = 5);
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif
