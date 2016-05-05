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
  Feature(const Feature& __alt) : WirePortable(__alt)  {
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

private:
  bool write_name(yarp::os::idl::WireWriter& writer);
  bool nested_write_name(yarp::os::idl::WireWriter& writer);
  bool write_content(yarp::os::idl::WireWriter& writer);
  bool nested_write_content(yarp::os::idl::WireWriter& writer);
  bool read_name(yarp::os::idl::WireReader& reader);
  bool nested_read_name(yarp::os::idl::WireReader& reader);
  bool read_content(yarp::os::idl::WireReader& reader);
  bool nested_read_content(yarp::os::idl::WireReader& reader);

public:

  yarp::os::ConstString toString();

  // if you want to serialize this class without nesting, use this helper
  typedef yarp::os::idl::Unwrapped<Feature > unwrapped;

  class Editor : public yarp::os::Wire, public yarp::os::PortWriter {
  public:

    Editor() {
      group = 0;
      obj_owned = true;
      obj = new Feature;
      dirty_flags(false);
      yarp().setOwner(*this);
    }

    Editor(Feature& obj) {
      group = 0;
      obj_owned = false;
      edit(obj,false);
      yarp().setOwner(*this);
    }

    bool edit(Feature& obj, bool dirty = true) {
      if (obj_owned) delete this->obj;
      this->obj = &obj;
      obj_owned = false;
      dirty_flags(dirty);
      return true;
    }

    virtual ~Editor() {
    if (obj_owned) delete obj;
    }

    bool isValid() const {
      return obj!=0/*NULL*/;
    }

    Feature& state() { return *obj; }

    void begin() { group++; }

    void end() {
      group--;
      if (group==0&&is_dirty) communicate();
    }
    void set_name(const std::string& name) {
      will_set_name();
      obj->name = name;
      mark_dirty_name();
      communicate();
      did_set_name();
    }
    void set_content(const std::vector<double> & content) {
      will_set_content();
      obj->content = content;
      mark_dirty_content();
      communicate();
      did_set_content();
    }
    void set_content(int index, const double elem) {
      will_set_content();
      obj->content[index] = elem;
      mark_dirty_content();
      communicate();
      did_set_content();
    }
    const std::string& get_name() {
      return obj->name;
    }
    const std::vector<double> & get_content() {
      return obj->content;
    }
    virtual bool will_set_name() { return true; }
    virtual bool will_set_content() { return true; }
    virtual bool did_set_name() { return true; }
    virtual bool did_set_content() { return true; }
    void clean() {
      dirty_flags(false);
    }
    bool read(yarp::os::ConnectionReader& connection);
    bool write(yarp::os::ConnectionWriter& connection);
  private:

    Feature *obj;

    bool obj_owned;
    int group;

    void communicate() {
      if (group!=0) return;
      if (yarp().canWrite()) {
        yarp().write(*this);
        clean();
      }
    }
    void mark_dirty() {
      is_dirty = true;
    }
    void mark_dirty_name() {
      if (is_dirty_name) return;
      dirty_count++;
      is_dirty_name = true;
      mark_dirty();
    }
    void mark_dirty_content() {
      if (is_dirty_content) return;
      dirty_count++;
      is_dirty_content = true;
      mark_dirty();
    }
    void dirty_flags(bool flag) {
      is_dirty = flag;
      is_dirty_name = flag;
      is_dirty_content = flag;
      dirty_count = flag ? 2 : 0;
    }
    bool is_dirty;
    int dirty_count;
    bool is_dirty_name;
    bool is_dirty_content;
  };
};

#endif
