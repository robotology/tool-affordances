// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_Feature
#define YARP_THRIFT_GENERATOR_STRUCT_Feature

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class Feature;


class Feature : public yarp::os::idl::WirePortable {
public:
  // Fields
  std::string name;
  std::vector<double>  content;
  
  // Default constructor
  Feature() : name("") {
  }
  
  // Constructor with field values
  Feature(const std::string& name,const std::vector<double> & content) : name(name), content(content) {
  }
  
  // Copy constructor
  Feature(const Feature& __alt) {
    this->name = __alt.name;
    this->content = __alt.content;
  }
  
  // Assignment operator
  const Feature& operator = (const Feature& __alt) {
    this->name = __alt.name;
    this->content = __alt.content;
    return *this;
  }
  
  // read and write structure on a connection
  bool read(yarp::os::idl::WireReader& reader);
  bool read(yarp::os::ConnectionReader& connection);
  bool write(yarp::os::idl::WireWriter& writer);
  bool write(yarp::os::ConnectionWriter& connection);
  
  yarp::os::ConstString toString();
  
  // if you want to serialize this class without nesting, use this helper
  typedef yarp::os::idl::Unwrapped<Feature > unwrapped;
};

#endif

