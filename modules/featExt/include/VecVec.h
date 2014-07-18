// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_VecVec
#define YARP_THRIFT_GENERATOR_STRUCT_VecVec

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>
#include <Feature.h>

class VecVec;


class VecVec : public yarp::os::idl::WirePortable {
public:
  // if you want to serialize this class without nesting, use this helper
  typedef yarp::os::idl::Unwrapped<VecVec > unwrapped;
  
  std::vector<Feature>  content;
  VecVec() {
  }
  VecVec(const std::vector<Feature> & content) : content(content) {
  }
  bool read(yarp::os::idl::WireReader& reader);
  bool read(yarp::os::ConnectionReader& connection);
  bool write(yarp::os::idl::WireWriter& writer);
  bool write(yarp::os::ConnectionWriter& connection);
};

#endif

