// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_toolRecognizer_IDLServer
#define YARP_THRIFT_GENERATOR_toolRecognizer_IDLServer

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class toolRecognizer_IDLServer;


class toolRecognizer_IDLServer : public yarp::os::Wire {
public:
  toolRecognizer_IDLServer();
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
   * Command to train tools by their label
   * @return true/false on success/failure to train classifiers.
   */
  virtual bool train(const std::string& label, const double tlx = 0, const double tly = 0, const double brx = 0, const double bry = 0);
  /**
   * Checks whether the hand is full or empty
   * @return true/false  corresponding to full or empty hand
   */
  virtual std::string recognize(const double tlx = 0, const double tly = 0, const double brx = 0, const double bry = 0);
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif
