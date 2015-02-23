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
  // Fields
  std::vector<Feature>  content;

  // Default constructor
  VecVec() {
  }

  // Constructor with field values
  VecVec(const std::vector<Feature> & content) : content(content) {
  }

  // Copy constructor
  VecVec(const VecVec& __alt) : WirePortable(__alt)  {
    this->content = __alt.content;
  }

  // Assignment operator
  const VecVec& operator = (const VecVec& __alt) {
    this->content = __alt.content;
    return *this;
  }

  // read and write structure on a connection
  bool read(yarp::os::idl::WireReader& reader);
  bool read(yarp::os::ConnectionReader& connection);
  bool write(yarp::os::idl::WireWriter& writer);
  bool write(yarp::os::ConnectionWriter& connection);

private:
  bool write_content(yarp::os::idl::WireWriter& writer);
  bool nested_write_content(yarp::os::idl::WireWriter& writer);
  bool read_content(yarp::os::idl::WireReader& reader);
  bool nested_read_content(yarp::os::idl::WireReader& reader);

public:

  yarp::os::ConstString toString();

  // if you want to serialize this class without nesting, use this helper
  typedef yarp::os::idl::Unwrapped<VecVec > unwrapped;

  class Editor : public yarp::os::Wire, public yarp::os::PortWriter {
  public:

    Editor() {
      group = 0;
      obj_owned = true;
      obj = new VecVec;
      dirty_flags(false);
      yarp().setOwner(*this);
    }

    Editor(VecVec& obj) {
      group = 0;
      obj_owned = false;
      edit(obj,false);
      yarp().setOwner(*this);
    }

    bool edit(VecVec& obj, bool dirty = true) {
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

    VecVec& state() { return *obj; }

    void begin() { group++; }

    void end() {
      group--;
      if (group==0&&is_dirty) communicate();
    }
    void set_content(const std::vector<Feature> & content) {
      will_set_content();
      obj->content = content;
      mark_dirty_content();
      communicate();
      did_set_content();
    }
    void set_content(int index, const Feature& elem) {
      will_set_content();
      obj->content[index] = elem;
      mark_dirty_content();
      communicate();
      did_set_content();
    }
    const std::vector<Feature> & get_content() {
      return obj->content;
    }
    virtual bool will_set_content() { return true; }
    virtual bool did_set_content() { return true; }
    void clean() {
      dirty_flags(false);
    }
    bool read(yarp::os::ConnectionReader& connection);
    bool write(yarp::os::ConnectionWriter& connection);
  private:

    VecVec *obj;

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
    void mark_dirty_content() {
      if (is_dirty_content) return;
      dirty_count++;
      is_dirty_content = true;
      mark_dirty();
    }
    void dirty_flags(bool flag) {
      is_dirty = flag;
      is_dirty_content = flag;
      dirty_count = flag ? 1 : 0;
    }
    bool is_dirty;
    int dirty_count;
    bool is_dirty_content;
  };
};

#endif

