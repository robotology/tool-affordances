// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_affCollector_IDLServer
#define YARP_THRIFT_GENERATOR_affCollector_IDLServer

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>
#include <yarp/os/Bottle.h>

class affCollector_IDLServer;


class affCollector_IDLServer : public yarp::os::Wire {
public:
  affCollector_IDLServer();
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
   * Allows to modify online the number of possible actions
   * @return true/false on success/failure
   */
  virtual bool setnumact(const int32_t numAct);
  /**
   * Sets the labels for all the possible actions
   * @return true/false on success/failure
   */
  virtual bool setactlabels(const yarp::os::Bottle& labels);
  /**
   * Returns the labels of all actions in the repertoire
   */
  virtual yarp::os::Bottle getactlabels();
  /**
   * Returns the labels of all the known tools-poses
   */
  virtual yarp::os::Bottle gettoollabels();
  /**
   * Activates a category (or creates if note previously exisiting), for which affordance data (action success rate) can be updated.
   * @return the index of the active (new or not) label
   */
  virtual int32_t setlabel(const std::string& label);
  /**
   * Returns the active label (object/tool/cateagory/etc, user defined).
   */
  virtual std::string getlabel();
  /**
   * For the active label, update its affordance success rate for a given action, based on its effect.
   * @returns the updated success rate for that action and label
   */
  virtual double updateAff(const int32_t act, const double eff, const int32_t labI = -1);
  /**
   * Returns the success rate for all actions in the repertoire for a given known label (the active one by default).
   * if label == 'all', returns a concatenated vector with all konwn affordances.
   */
  virtual yarp::os::Bottle getAffs(const std::string& label = "active");
  /**
   * Returns the history of effects for a given action and known label (the active one by default).
   */
  virtual yarp::os::Bottle getAffHist(const std::string& label = "active", const int32_t act = -1);
  /**
   *  Based on the previously learnt affordances, returns the best label for a given action/task.
   * @return true/false on success/failure
   */
  virtual std::string selectTool(const std::string& action);
  /**
   * Returns the label and action with less certainty of known ones, based on variance.
   * @return true/false on success/failure
   */
  virtual std::string activeExp(const std::string& label = "active");
  /**
   * Returns the label and action that has been tried the least.
   * @return true/false on success/failure
   */
  virtual std::string balanceExp(const std::string& label = "active");
  /**
   * Clears all the learnt affordances of the active label, and sets it to unknown.
   * @return true/false on success/failure
   */
  virtual bool reset(const std::string& label = "active");
  /**
   * Clears all the learnt affordances all labels, and sets them to unknown.
   * @return true/false on success/failure
   */
  virtual bool clearAll();
  /**
   * Removes all teh content from the memory file. Use with precaution (requires confirmation)
   * @return true/false on success/failure
   */
  virtual std::string forgetAll();
  /**
   * Saves the known affordances history in a given file.
   * @return true/false on success/failure
   */
  virtual bool savetofile(const std::string& file = "default");
  /**
   * Reads labels and aff histories from files 'fileLables.txt' and the affHist in another 'fileHist.txt'
   * @return true/false on success/failure
   */
  virtual bool readfile(const std::string& file = "default");
  /**
   * Activates/Deactivates more verbose execution of the module.
   * @return true/false on success/failure
   */
  virtual bool verbose(const bool verb);
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif
