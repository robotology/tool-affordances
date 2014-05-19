// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_info3D
#define YARP_THRIFT_GENERATOR_STRUCT_info3D

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class info3D;


/**
 * blob3Dinfo_IDLServer
 * Interface.
 */
class info3D : public yarp::os::idl::WirePortable {
public:
  // if you want to serialize this class without nesting, use this helper
  typedef yarp::os::idl::Unwrapped<info3D > unwrapped;
  
  bool ACK;
  std::string msg;
  std::vector<double>  content;
  info3D() : ACK(0), msg("") {
  }
  info3D(const bool ACK,const std::string& msg,const std::vector<double> & content) : ACK(ACK), msg(msg), content(content) {
  }
  bool read(yarp::os::idl::WireReader& reader);
  bool read(yarp::os::ConnectionReader& connection);
  bool write(yarp::os::idl::WireWriter& writer);
  bool write(yarp::os::ConnectionWriter& connection);
};

#endif

