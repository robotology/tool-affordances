// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_STRUCT_BoundingBox
#define YARP_THRIFT_GENERATOR_STRUCT_BoundingBox

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class BoundingBox;


/**
 * graspChecker_IDLServer
 * Interface.
 */
class BoundingBox : public yarp::os::idl::WirePortable {
public:
  // Fields
  double tlx;
  double tly;
  double brx;
  double bry;

  // Default constructor
  BoundingBox() : tlx(0), tly(0), brx(0), bry(0) {
  }

  // Constructor with field values
  BoundingBox(const double tlx,const double tly,const double brx,const double bry) : tlx(tlx), tly(tly), brx(brx), bry(bry) {
  }

  // Copy constructor
  BoundingBox(const BoundingBox& __alt) : WirePortable(__alt)  {
    this->tlx = __alt.tlx;
    this->tly = __alt.tly;
    this->brx = __alt.brx;
    this->bry = __alt.bry;
  }

  // Assignment operator
  const BoundingBox& operator = (const BoundingBox& __alt) {
    this->tlx = __alt.tlx;
    this->tly = __alt.tly;
    this->brx = __alt.brx;
    this->bry = __alt.bry;
    return *this;
  }

  // read and write structure on a connection
  bool read(yarp::os::idl::WireReader& reader) override;
  bool read(yarp::os::ConnectionReader& connection) override;
  bool write(yarp::os::idl::WireWriter& writer) override;
  bool write(yarp::os::ConnectionWriter& connection) override;

private:
  bool write_tlx(yarp::os::idl::WireWriter& writer);
  bool nested_write_tlx(yarp::os::idl::WireWriter& writer);
  bool write_tly(yarp::os::idl::WireWriter& writer);
  bool nested_write_tly(yarp::os::idl::WireWriter& writer);
  bool write_brx(yarp::os::idl::WireWriter& writer);
  bool nested_write_brx(yarp::os::idl::WireWriter& writer);
  bool write_bry(yarp::os::idl::WireWriter& writer);
  bool nested_write_bry(yarp::os::idl::WireWriter& writer);
  bool read_tlx(yarp::os::idl::WireReader& reader);
  bool nested_read_tlx(yarp::os::idl::WireReader& reader);
  bool read_tly(yarp::os::idl::WireReader& reader);
  bool nested_read_tly(yarp::os::idl::WireReader& reader);
  bool read_brx(yarp::os::idl::WireReader& reader);
  bool nested_read_brx(yarp::os::idl::WireReader& reader);
  bool read_bry(yarp::os::idl::WireReader& reader);
  bool nested_read_bry(yarp::os::idl::WireReader& reader);

public:

  yarp::os::ConstString toString();

  // if you want to serialize this class without nesting, use this helper
  typedef yarp::os::idl::Unwrapped<BoundingBox > unwrapped;

  class Editor : public yarp::os::Wire, public yarp::os::PortWriter {
  public:

    Editor() {
      group = 0;
      obj_owned = true;
      obj = new BoundingBox;
      dirty_flags(false);
      yarp().setOwner(*this);
    }

    Editor(BoundingBox& obj) {
      group = 0;
      obj_owned = false;
      edit(obj,false);
      yarp().setOwner(*this);
    }

    bool edit(BoundingBox& obj, bool dirty = true) {
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

    BoundingBox& state() { return *obj; }

    void begin() { group++; }

    void end() {
      group--;
      if (group==0&&is_dirty) communicate();
    }
    void set_tlx(const double tlx) {
      will_set_tlx();
      obj->tlx = tlx;
      mark_dirty_tlx();
      communicate();
      did_set_tlx();
    }
    void set_tly(const double tly) {
      will_set_tly();
      obj->tly = tly;
      mark_dirty_tly();
      communicate();
      did_set_tly();
    }
    void set_brx(const double brx) {
      will_set_brx();
      obj->brx = brx;
      mark_dirty_brx();
      communicate();
      did_set_brx();
    }
    void set_bry(const double bry) {
      will_set_bry();
      obj->bry = bry;
      mark_dirty_bry();
      communicate();
      did_set_bry();
    }
    double get_tlx() {
      return obj->tlx;
    }
    double get_tly() {
      return obj->tly;
    }
    double get_brx() {
      return obj->brx;
    }
    double get_bry() {
      return obj->bry;
    }
    virtual bool will_set_tlx() { return true; }
    virtual bool will_set_tly() { return true; }
    virtual bool will_set_brx() { return true; }
    virtual bool will_set_bry() { return true; }
    virtual bool did_set_tlx() { return true; }
    virtual bool did_set_tly() { return true; }
    virtual bool did_set_brx() { return true; }
    virtual bool did_set_bry() { return true; }
    void clean() {
      dirty_flags(false);
    }
    bool read(yarp::os::ConnectionReader& connection) override;
    bool write(yarp::os::ConnectionWriter& connection) override;
  private:

    BoundingBox *obj;

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
    void mark_dirty_tlx() {
      if (is_dirty_tlx) return;
      dirty_count++;
      is_dirty_tlx = true;
      mark_dirty();
    }
    void mark_dirty_tly() {
      if (is_dirty_tly) return;
      dirty_count++;
      is_dirty_tly = true;
      mark_dirty();
    }
    void mark_dirty_brx() {
      if (is_dirty_brx) return;
      dirty_count++;
      is_dirty_brx = true;
      mark_dirty();
    }
    void mark_dirty_bry() {
      if (is_dirty_bry) return;
      dirty_count++;
      is_dirty_bry = true;
      mark_dirty();
    }
    void dirty_flags(bool flag) {
      is_dirty = flag;
      is_dirty_tlx = flag;
      is_dirty_tly = flag;
      is_dirty_brx = flag;
      is_dirty_bry = flag;
      dirty_count = flag ? 4 : 0;
    }
    bool is_dirty;
    int dirty_count;
    bool is_dirty_tlx;
    bool is_dirty_tly;
    bool is_dirty_brx;
    bool is_dirty_bry;
  };
};

#endif
