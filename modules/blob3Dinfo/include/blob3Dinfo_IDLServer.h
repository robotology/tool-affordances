// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_blob3Dinfo_IDLServer
#define YARP_THRIFT_GENERATOR_blob3Dinfo_IDLServer

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>
#include <info3D.h>

class blob3Dinfo_IDLServer;


class blob3Dinfo_IDLServer : public yarp::os::Wire {
public:
  blob3Dinfo_IDLServer() { yarp().setOwner(*this); }
/**
 * Start the module
 * @return true/false on success/failure
 */
  virtual bool start();
/**
 * Stop the module.
 * @return true/false on success/failure
 */
  virtual bool stop();
/**
 * Compute once the 3D position
 * @return true/false on success/failure
 */
  virtual std::vector<int32_t>  blobsInRange(const double range);
/**
 * Compute once the 3D position
 * @return true/false on success/failure
 */
  virtual info3D get3Dinfo();
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif

