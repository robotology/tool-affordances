// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_Feature
#define YARP_THRIFT_GENERATOR_STRUCT_Feature

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class Feature;


class Feature : public yarp::os::idl::WirePortable {
public:
  // if you want to serialize this class without nesting, use this helper
  typedef yarp::os::idl::Unwrapped<Feature > unwrapped;
  
  std::string name;
  std::vector<double>  content;
  Feature() : name("") {
  }
  Feature(const std::string& name,const std::vector<double> & content) : name(name), content(content) {
  }
  bool read(yarp::os::idl::WireReader& reader);
  bool read(yarp::os::ConnectionReader& connection);
  bool write(yarp::os::idl::WireWriter& writer);
  bool write(yarp::os::ConnectionWriter& connection);
};

#endif

