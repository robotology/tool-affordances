// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <tool3DManager_IDLServer.h>
#include <yarp/os/idl/WireTypes.h>



class tool3DManager_IDLServer_start : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_quit : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_setSeg : public yarp::os::Portable {
public:
  bool seg;
  bool _return;
  void init(const bool seg);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_loadModel : public yarp::os::Portable {
public:
  std::string tool;
  bool _return;
  void init(const std::string& tool);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_getToolParam : public yarp::os::Portable {
public:
  std::string tool;
  double deg;
  double disp;
  double tilt;
  double shift;
  bool _return;
  void init(const std::string& tool, const double deg, const double disp, const double tilt, const double shift);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_getToolAlign : public yarp::os::Portable {
public:
  std::string tool;
  bool _return;
  void init(const std::string& tool);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_graspTool : public yarp::os::Portable {
public:
  std::string tool;
  bool _return;
  void init(const std::string& tool);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_explore : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_lookTool : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_regrasp : public yarp::os::Portable {
public:
  double deg;
  double disp;
  double tilt;
  double shift;
  bool _return;
  void init(const double deg, const double disp, const double tilt, const double shift);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_findPose : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_getToolFeats : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_objLoc : public yarp::os::Portable {
public:
  yarp::sig::Vector _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_objRot : public yarp::os::Portable {
public:
  yarp::sig::Vector _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_goHome : public yarp::os::Portable {
public:
  bool hands;
  bool _return;
  void init(const bool hands);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_findTable : public yarp::os::Portable {
public:
  bool calib;
  bool _return;
  void init(const bool calib);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_slide : public yarp::os::Portable {
public:
  double thetha;
  double radius;
  bool _return;
  void init(const double thetha, const double radius);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_drag : public yarp::os::Portable {
public:
  double thetha;
  double radius;
  double tilt;
  bool _return;
  void init(const double thetha, const double radius, const double tilt);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_drag3D : public yarp::os::Portable {
public:
  double x;
  double y;
  double z;
  double thetha;
  double radius;
  double tilt;
  bool useTool;
  bool _return;
  void init(const double x, const double y, const double z, const double thetha, const double radius, const double tilt, const bool useTool);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_trackObj : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_compEff : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_resetObj : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_runRandPoses : public yarp::os::Portable {
public:
  int32_t numPoses;
  int32_t numAct;
  bool _return;
  void init(const int32_t numPoses, const int32_t numAct);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_runToolPose : public yarp::os::Portable {
public:
  int32_t numRep;
  int32_t numAct;
  bool _return;
  void init(const int32_t numRep, const int32_t numAct);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_runToolTrial : public yarp::os::Portable {
public:
  int32_t numRep;
  std::string tool;
  int32_t numAct;
  bool _return;
  void init(const int32_t numRep, const std::string& tool, const int32_t numAct);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_runExp : public yarp::os::Portable {
public:
  std::string tool;
  int32_t toolEnd;
  bool _return;
  void init(const std::string& tool, const int32_t toolEnd);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_selectAction : public yarp::os::Portable {
public:
  int32_t goal;
  bool _return;
  void init(const int32_t goal);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_predExp : public yarp::os::Portable {
public:
  int32_t goal;
  bool _return;
  void init(const int32_t goal);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

bool tool3DManager_IDLServer_start::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("start",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_start::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_start::init() {
  _return = false;
}

bool tool3DManager_IDLServer_quit::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("quit",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_quit::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_quit::init() {
  _return = false;
}

bool tool3DManager_IDLServer_setSeg::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("setSeg",1,1)) return false;
  if (!writer.writeBool(seg)) return false;
  return true;
}

bool tool3DManager_IDLServer_setSeg::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_setSeg::init(const bool seg) {
  _return = false;
  this->seg = seg;
}

bool tool3DManager_IDLServer_loadModel::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("loadModel",1,1)) return false;
  if (!writer.writeString(tool)) return false;
  return true;
}

bool tool3DManager_IDLServer_loadModel::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_loadModel::init(const std::string& tool) {
  _return = false;
  this->tool = tool;
}

bool tool3DManager_IDLServer_getToolParam::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(6)) return false;
  if (!writer.writeTag("getToolParam",1,1)) return false;
  if (!writer.writeString(tool)) return false;
  if (!writer.writeDouble(deg)) return false;
  if (!writer.writeDouble(disp)) return false;
  if (!writer.writeDouble(tilt)) return false;
  if (!writer.writeDouble(shift)) return false;
  return true;
}

bool tool3DManager_IDLServer_getToolParam::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_getToolParam::init(const std::string& tool, const double deg, const double disp, const double tilt, const double shift) {
  _return = false;
  this->tool = tool;
  this->deg = deg;
  this->disp = disp;
  this->tilt = tilt;
  this->shift = shift;
}

bool tool3DManager_IDLServer_getToolAlign::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("getToolAlign",1,1)) return false;
  if (!writer.writeString(tool)) return false;
  return true;
}

bool tool3DManager_IDLServer_getToolAlign::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_getToolAlign::init(const std::string& tool) {
  _return = false;
  this->tool = tool;
}

bool tool3DManager_IDLServer_graspTool::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("graspTool",1,1)) return false;
  if (!writer.writeString(tool)) return false;
  return true;
}

bool tool3DManager_IDLServer_graspTool::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_graspTool::init(const std::string& tool) {
  _return = false;
  this->tool = tool;
}

bool tool3DManager_IDLServer_explore::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("explore",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_explore::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_explore::init() {
  _return = false;
}

bool tool3DManager_IDLServer_lookTool::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("lookTool",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_lookTool::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_lookTool::init() {
  _return = false;
}

bool tool3DManager_IDLServer_regrasp::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(5)) return false;
  if (!writer.writeTag("regrasp",1,1)) return false;
  if (!writer.writeDouble(deg)) return false;
  if (!writer.writeDouble(disp)) return false;
  if (!writer.writeDouble(tilt)) return false;
  if (!writer.writeDouble(shift)) return false;
  return true;
}

bool tool3DManager_IDLServer_regrasp::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_regrasp::init(const double deg, const double disp, const double tilt, const double shift) {
  _return = false;
  this->deg = deg;
  this->disp = disp;
  this->tilt = tilt;
  this->shift = shift;
}

bool tool3DManager_IDLServer_findPose::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("findPose",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_findPose::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_findPose::init() {
  _return = false;
}

bool tool3DManager_IDLServer_getToolFeats::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("getToolFeats",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_getToolFeats::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_getToolFeats::init() {
  _return = false;
}

bool tool3DManager_IDLServer_objLoc::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("objLoc",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_objLoc::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.read(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_objLoc::init() {
}

bool tool3DManager_IDLServer_objRot::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("objRot",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_objRot::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.read(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_objRot::init() {
}

bool tool3DManager_IDLServer_goHome::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("goHome",1,1)) return false;
  if (!writer.writeBool(hands)) return false;
  return true;
}

bool tool3DManager_IDLServer_goHome::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_goHome::init(const bool hands) {
  _return = false;
  this->hands = hands;
}

bool tool3DManager_IDLServer_findTable::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("findTable",1,1)) return false;
  if (!writer.writeBool(calib)) return false;
  return true;
}

bool tool3DManager_IDLServer_findTable::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_findTable::init(const bool calib) {
  _return = false;
  this->calib = calib;
}

bool tool3DManager_IDLServer_slide::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("slide",1,1)) return false;
  if (!writer.writeDouble(thetha)) return false;
  if (!writer.writeDouble(radius)) return false;
  return true;
}

bool tool3DManager_IDLServer_slide::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_slide::init(const double thetha, const double radius) {
  _return = false;
  this->thetha = thetha;
  this->radius = radius;
}

bool tool3DManager_IDLServer_drag::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(4)) return false;
  if (!writer.writeTag("drag",1,1)) return false;
  if (!writer.writeDouble(thetha)) return false;
  if (!writer.writeDouble(radius)) return false;
  if (!writer.writeDouble(tilt)) return false;
  return true;
}

bool tool3DManager_IDLServer_drag::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_drag::init(const double thetha, const double radius, const double tilt) {
  _return = false;
  this->thetha = thetha;
  this->radius = radius;
  this->tilt = tilt;
}

bool tool3DManager_IDLServer_drag3D::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(8)) return false;
  if (!writer.writeTag("drag3D",1,1)) return false;
  if (!writer.writeDouble(x)) return false;
  if (!writer.writeDouble(y)) return false;
  if (!writer.writeDouble(z)) return false;
  if (!writer.writeDouble(thetha)) return false;
  if (!writer.writeDouble(radius)) return false;
  if (!writer.writeDouble(tilt)) return false;
  if (!writer.writeBool(useTool)) return false;
  return true;
}

bool tool3DManager_IDLServer_drag3D::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_drag3D::init(const double x, const double y, const double z, const double thetha, const double radius, const double tilt, const bool useTool) {
  _return = false;
  this->x = x;
  this->y = y;
  this->z = z;
  this->thetha = thetha;
  this->radius = radius;
  this->tilt = tilt;
  this->useTool = useTool;
}

bool tool3DManager_IDLServer_trackObj::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("trackObj",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_trackObj::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_trackObj::init() {
  _return = false;
}

bool tool3DManager_IDLServer_compEff::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("compEff",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_compEff::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_compEff::init() {
  _return = false;
}

bool tool3DManager_IDLServer_resetObj::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("resetObj",1,1)) return false;
  return true;
}

bool tool3DManager_IDLServer_resetObj::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_resetObj::init() {
  _return = false;
}

bool tool3DManager_IDLServer_runRandPoses::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("runRandPoses",1,1)) return false;
  if (!writer.writeI32(numPoses)) return false;
  if (!writer.writeI32(numAct)) return false;
  return true;
}

bool tool3DManager_IDLServer_runRandPoses::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_runRandPoses::init(const int32_t numPoses, const int32_t numAct) {
  _return = false;
  this->numPoses = numPoses;
  this->numAct = numAct;
}

bool tool3DManager_IDLServer_runToolPose::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("runToolPose",1,1)) return false;
  if (!writer.writeI32(numRep)) return false;
  if (!writer.writeI32(numAct)) return false;
  return true;
}

bool tool3DManager_IDLServer_runToolPose::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_runToolPose::init(const int32_t numRep, const int32_t numAct) {
  _return = false;
  this->numRep = numRep;
  this->numAct = numAct;
}

bool tool3DManager_IDLServer_runToolTrial::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(4)) return false;
  if (!writer.writeTag("runToolTrial",1,1)) return false;
  if (!writer.writeI32(numRep)) return false;
  if (!writer.writeString(tool)) return false;
  if (!writer.writeI32(numAct)) return false;
  return true;
}

bool tool3DManager_IDLServer_runToolTrial::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_runToolTrial::init(const int32_t numRep, const std::string& tool, const int32_t numAct) {
  _return = false;
  this->numRep = numRep;
  this->tool = tool;
  this->numAct = numAct;
}

bool tool3DManager_IDLServer_runExp::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("runExp",1,1)) return false;
  if (!writer.writeString(tool)) return false;
  if (!writer.writeI32(toolEnd)) return false;
  return true;
}

bool tool3DManager_IDLServer_runExp::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_runExp::init(const std::string& tool, const int32_t toolEnd) {
  _return = false;
  this->tool = tool;
  this->toolEnd = toolEnd;
}

bool tool3DManager_IDLServer_selectAction::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("selectAction",1,1)) return false;
  if (!writer.writeI32(goal)) return false;
  return true;
}

bool tool3DManager_IDLServer_selectAction::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_selectAction::init(const int32_t goal) {
  _return = false;
  this->goal = goal;
}

bool tool3DManager_IDLServer_predExp::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("predExp",1,1)) return false;
  if (!writer.writeI32(goal)) return false;
  return true;
}

bool tool3DManager_IDLServer_predExp::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_predExp::init(const int32_t goal) {
  _return = false;
  this->goal = goal;
}

tool3DManager_IDLServer::tool3DManager_IDLServer() {
  yarp().setOwner(*this);
}
bool tool3DManager_IDLServer::start() {
  bool _return = false;
  tool3DManager_IDLServer_start helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::start()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::quit() {
  bool _return = false;
  tool3DManager_IDLServer_quit helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::quit()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::setSeg(const bool seg) {
  bool _return = false;
  tool3DManager_IDLServer_setSeg helper;
  helper.init(seg);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::setSeg(const bool seg)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::loadModel(const std::string& tool) {
  bool _return = false;
  tool3DManager_IDLServer_loadModel helper;
  helper.init(tool);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::loadModel(const std::string& tool)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::getToolParam(const std::string& tool, const double deg, const double disp, const double tilt, const double shift) {
  bool _return = false;
  tool3DManager_IDLServer_getToolParam helper;
  helper.init(tool,deg,disp,tilt,shift);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::getToolParam(const std::string& tool, const double deg, const double disp, const double tilt, const double shift)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::getToolAlign(const std::string& tool) {
  bool _return = false;
  tool3DManager_IDLServer_getToolAlign helper;
  helper.init(tool);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::getToolAlign(const std::string& tool)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::graspTool(const std::string& tool) {
  bool _return = false;
  tool3DManager_IDLServer_graspTool helper;
  helper.init(tool);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::graspTool(const std::string& tool)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::explore() {
  bool _return = false;
  tool3DManager_IDLServer_explore helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::explore()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::lookTool() {
  bool _return = false;
  tool3DManager_IDLServer_lookTool helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::lookTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::regrasp(const double deg, const double disp, const double tilt, const double shift) {
  bool _return = false;
  tool3DManager_IDLServer_regrasp helper;
  helper.init(deg,disp,tilt,shift);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::regrasp(const double deg, const double disp, const double tilt, const double shift)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::findPose() {
  bool _return = false;
  tool3DManager_IDLServer_findPose helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::findPose()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::getToolFeats() {
  bool _return = false;
  tool3DManager_IDLServer_getToolFeats helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::getToolFeats()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
yarp::sig::Vector tool3DManager_IDLServer::objLoc() {
  yarp::sig::Vector _return;
  tool3DManager_IDLServer_objLoc helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","yarp::sig::Vector tool3DManager_IDLServer::objLoc()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
yarp::sig::Vector tool3DManager_IDLServer::objRot() {
  yarp::sig::Vector _return;
  tool3DManager_IDLServer_objRot helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","yarp::sig::Vector tool3DManager_IDLServer::objRot()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::goHome(const bool hands) {
  bool _return = false;
  tool3DManager_IDLServer_goHome helper;
  helper.init(hands);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::goHome(const bool hands)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::findTable(const bool calib) {
  bool _return = false;
  tool3DManager_IDLServer_findTable helper;
  helper.init(calib);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::findTable(const bool calib)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::slide(const double thetha, const double radius) {
  bool _return = false;
  tool3DManager_IDLServer_slide helper;
  helper.init(thetha,radius);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::slide(const double thetha, const double radius)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::drag(const double thetha, const double radius, const double tilt) {
  bool _return = false;
  tool3DManager_IDLServer_drag helper;
  helper.init(thetha,radius,tilt);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::drag(const double thetha, const double radius, const double tilt)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::drag3D(const double x, const double y, const double z, const double thetha, const double radius, const double tilt, const bool useTool) {
  bool _return = false;
  tool3DManager_IDLServer_drag3D helper;
  helper.init(x,y,z,thetha,radius,tilt,useTool);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::drag3D(const double x, const double y, const double z, const double thetha, const double radius, const double tilt, const bool useTool)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::trackObj() {
  bool _return = false;
  tool3DManager_IDLServer_trackObj helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::trackObj()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::compEff() {
  bool _return = false;
  tool3DManager_IDLServer_compEff helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::compEff()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::resetObj() {
  bool _return = false;
  tool3DManager_IDLServer_resetObj helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::resetObj()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::runRandPoses(const int32_t numPoses, const int32_t numAct) {
  bool _return = false;
  tool3DManager_IDLServer_runRandPoses helper;
  helper.init(numPoses,numAct);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::runRandPoses(const int32_t numPoses, const int32_t numAct)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::runToolPose(const int32_t numRep, const int32_t numAct) {
  bool _return = false;
  tool3DManager_IDLServer_runToolPose helper;
  helper.init(numRep,numAct);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::runToolPose(const int32_t numRep, const int32_t numAct)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::runToolTrial(const int32_t numRep, const std::string& tool, const int32_t numAct) {
  bool _return = false;
  tool3DManager_IDLServer_runToolTrial helper;
  helper.init(numRep,tool,numAct);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::runToolTrial(const int32_t numRep, const std::string& tool, const int32_t numAct)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::runExp(const std::string& tool, const int32_t toolEnd) {
  bool _return = false;
  tool3DManager_IDLServer_runExp helper;
  helper.init(tool,toolEnd);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::runExp(const std::string& tool, const int32_t toolEnd)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::selectAction(const int32_t goal) {
  bool _return = false;
  tool3DManager_IDLServer_selectAction helper;
  helper.init(goal);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::selectAction(const int32_t goal)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::predExp(const int32_t goal) {
  bool _return = false;
  tool3DManager_IDLServer_predExp helper;
  helper.init(goal);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::predExp(const int32_t goal)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool tool3DManager_IDLServer::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  reader.expectAccept();
  if (!reader.readListHeader()) { reader.fail(); return false; }
  yarp::os::ConstString tag = reader.readTag();
  bool direct = (tag=="__direct__");
  if (direct) tag = reader.readTag();
  while (!reader.isError()) {
    // TODO: use quick lookup, this is just a test
    if (tag == "start") {
      bool _return;
      _return = start();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "quit") {
      bool _return;
      _return = quit();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "setSeg") {
      bool seg;
      if (!reader.readBool(seg)) {
        seg = 0;
      }
      bool _return;
      _return = setSeg(seg);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "loadModel") {
      std::string tool;
      if (!reader.readString(tool)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = loadModel(tool);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getToolParam") {
      std::string tool;
      double deg;
      double disp;
      double tilt;
      double shift;
      if (!reader.readString(tool)) {
        reader.fail();
        return false;
      }
      if (!reader.readDouble(deg)) {
        deg = 0;
      }
      if (!reader.readDouble(disp)) {
        disp = 0;
      }
      if (!reader.readDouble(tilt)) {
        tilt = 45;
      }
      if (!reader.readDouble(shift)) {
        shift = 0;
      }
      bool _return;
      _return = getToolParam(tool,deg,disp,tilt,shift);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getToolAlign") {
      std::string tool;
      if (!reader.readString(tool)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = getToolAlign(tool);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "graspTool") {
      std::string tool;
      if (!reader.readString(tool)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = graspTool(tool);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "explore") {
      bool _return;
      _return = explore();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "lookTool") {
      bool _return;
      _return = lookTool();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "regrasp") {
      double deg;
      double disp;
      double tilt;
      double shift;
      if (!reader.readDouble(deg)) {
        deg = 0;
      }
      if (!reader.readDouble(disp)) {
        disp = 0;
      }
      if (!reader.readDouble(tilt)) {
        tilt = 45;
      }
      if (!reader.readDouble(shift)) {
        shift = 0;
      }
      bool _return;
      _return = regrasp(deg,disp,tilt,shift);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "findPose") {
      bool _return;
      _return = findPose();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getToolFeats") {
      bool _return;
      _return = getToolFeats();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "objLoc") {
      yarp::sig::Vector _return;
      _return = objLoc();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.write(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "objRot") {
      yarp::sig::Vector _return;
      _return = objRot();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.write(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "goHome") {
      bool hands;
      if (!reader.readBool(hands)) {
        hands = 0;
      }
      bool _return;
      _return = goHome(hands);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "findTable") {
      bool calib;
      if (!reader.readBool(calib)) {
        calib = 1;
      }
      bool _return;
      _return = findTable(calib);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "slide") {
      double thetha;
      double radius;
      if (!reader.readDouble(thetha)) {
        thetha = 0;
      }
      if (!reader.readDouble(radius)) {
        radius = 0;
      }
      bool _return;
      _return = slide(thetha,radius);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "drag") {
      double thetha;
      double radius;
      double tilt;
      if (!reader.readDouble(thetha)) {
        thetha = 0;
      }
      if (!reader.readDouble(radius)) {
        radius = 0;
      }
      if (!reader.readDouble(tilt)) {
        tilt = -15;
      }
      bool _return;
      _return = drag(thetha,radius,tilt);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "drag3D") {
      double x;
      double y;
      double z;
      double thetha;
      double radius;
      double tilt;
      bool useTool;
      if (!reader.readDouble(x)) {
        reader.fail();
        return false;
      }
      if (!reader.readDouble(y)) {
        reader.fail();
        return false;
      }
      if (!reader.readDouble(z)) {
        reader.fail();
        return false;
      }
      if (!reader.readDouble(thetha)) {
        thetha = 0;
      }
      if (!reader.readDouble(radius)) {
        radius = 0;
      }
      if (!reader.readDouble(tilt)) {
        tilt = -15;
      }
      if (!reader.readBool(useTool)) {
        useTool = 1;
      }
      bool _return;
      _return = drag3D(x,y,z,thetha,radius,tilt,useTool);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "trackObj") {
      bool _return;
      _return = trackObj();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "compEff") {
      bool _return;
      _return = compEff();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "resetObj") {
      bool _return;
      _return = resetObj();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "runRandPoses") {
      int32_t numPoses;
      int32_t numAct;
      if (!reader.readI32(numPoses)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(numAct)) {
        numAct = 8;
      }
      bool _return;
      _return = runRandPoses(numPoses,numAct);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "runToolPose") {
      int32_t numRep;
      int32_t numAct;
      if (!reader.readI32(numRep)) {
        numRep = 1;
      }
      if (!reader.readI32(numAct)) {
        numAct = 8;
      }
      bool _return;
      _return = runToolPose(numRep,numAct);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "runToolTrial") {
      int32_t numRep;
      std::string tool;
      int32_t numAct;
      if (!reader.readI32(numRep)) {
        numRep = 1;
      }
      if (!reader.readString(tool)) {
        tool = "";
      }
      if (!reader.readI32(numAct)) {
        numAct = 8;
      }
      bool _return;
      _return = runToolTrial(numRep,tool,numAct);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "runExp") {
      std::string tool;
      int32_t toolEnd;
      if (!reader.readString(tool)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(toolEnd)) {
        toolEnd = 54;
      }
      bool _return;
      _return = runExp(tool,toolEnd);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "selectAction") {
      int32_t goal;
      if (!reader.readI32(goal)) {
        goal = 1;
      }
      bool _return;
      _return = selectAction(goal);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "predExp") {
      int32_t goal;
      if (!reader.readI32(goal)) {
        goal = 1;
      }
      bool _return;
      _return = predExp(goal);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "help") {
      std::string functionName;
      if (!reader.readString(functionName)) {
        functionName = "--all";
      }
      std::vector<std::string> _return=help(functionName);
      yarp::os::idl::WireWriter writer(reader);
        if (!writer.isNull()) {
          if (!writer.writeListHeader(2)) return false;
          if (!writer.writeTag("many",1, 0)) return false;
          if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<std::string> ::iterator _iterHelp;
          for (_iterHelp = _return.begin(); _iterHelp != _return.end(); ++_iterHelp)
          {
            if (!writer.writeString(*_iterHelp)) return false;
           }
          if (!writer.writeListEnd()) return false;
        }
      reader.accept();
      return true;
    }
    if (reader.noMore()) { reader.fail(); return false; }
    yarp::os::ConstString next_tag = reader.readTag();
    if (next_tag=="") break;
    tag = tag + "_" + next_tag;
  }
  return false;
}

std::vector<std::string> tool3DManager_IDLServer::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("start");
    helpString.push_back("quit");
    helpString.push_back("setSeg");
    helpString.push_back("loadModel");
    helpString.push_back("getToolParam");
    helpString.push_back("getToolAlign");
    helpString.push_back("graspTool");
    helpString.push_back("explore");
    helpString.push_back("lookTool");
    helpString.push_back("regrasp");
    helpString.push_back("findPose");
    helpString.push_back("getToolFeats");
    helpString.push_back("objLoc");
    helpString.push_back("objRot");
    helpString.push_back("goHome");
    helpString.push_back("findTable");
    helpString.push_back("slide");
    helpString.push_back("drag");
    helpString.push_back("drag3D");
    helpString.push_back("trackObj");
    helpString.push_back("compEff");
    helpString.push_back("resetObj");
    helpString.push_back("runRandPoses");
    helpString.push_back("runToolPose");
    helpString.push_back("runToolTrial");
    helpString.push_back("runExp");
    helpString.push_back("selectAction");
    helpString.push_back("predExp");
    helpString.push_back("help");
  }
  else {
    if (functionName=="start") {
      helpString.push_back("bool start() ");
      helpString.push_back("Start the module ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="quit") {
      helpString.push_back("bool quit() ");
      helpString.push_back("Quit the module ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="setSeg") {
      helpString.push_back("bool setSeg(const bool seg = 0) ");
      helpString.push_back("Sets segmentation to 2D (true) or 3D (false) ");
      helpString.push_back("@return true/false on success/failure to toggle segmentation method. ");
    }
    if (functionName=="loadModel") {
      helpString.push_back("bool loadModel(const std::string& tool) ");
      helpString.push_back("Loads the tool on objects3DExplorer and changes name ");
      helpString.push_back("@return true/false on success/failure on setting name ");
    }
    if (functionName=="getToolParam") {
      helpString.push_back("bool getToolParam(const std::string& tool, const double deg = 0, const double disp = 0, const double tilt = 45, const double shift = 0) ");
      helpString.push_back("Performs the sequence to get the tool: \n ");
      helpString.push_back("- On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i>, tilted at <i>tilt</i> and with a displacement on the -Y hand axis <i>disp</i>. Uses magnet function to hold it to hand. ");
      helpString.push_back("- Moreover, the tool end effector is located and attached to the kinematic chain with affMotor and shown on the viewer ");
      helpString.push_back("- On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i>, <i>deg</i>, <i>disp</i>  and <i>tilt</i> should correspond to the way in which the tool is given ");
      helpString.push_back("@return true/false on success/failure of loading the tool with correct pose ");
    }
    if (functionName=="getToolAlign") {
      helpString.push_back("bool getToolAlign(const std::string& tool) ");
      helpString.push_back("Performs the sequence to get the tool: \n ");
      helpString.push_back("- Grasp (through ARE) ");
      helpString.push_back("- Load tool in objects3Dexplorer (by setToolName) ");
      helpString.push_back("- Find pose and tooltip with align method (by findPose) ");
      helpString.push_back("@return true/false on success/failure of grasping and loading the named tool ");
    }
    if (functionName=="graspTool") {
      helpString.push_back("bool graspTool(const std::string& tool) ");
      helpString.push_back("Communicates with ARE and KM to grasp a tool and move it to the center. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="explore") {
      helpString.push_back("bool explore() ");
      helpString.push_back("Communicates with O3DE to explore the tool and get the tooltip without a model ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="lookTool") {
      helpString.push_back("bool lookTool() ");
      helpString.push_back("Communicates with KM  move the tool to the center. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="regrasp") {
      helpString.push_back("bool regrasp(const double deg = 0, const double disp = 0, const double tilt = 45, const double shift = 0) ");
      helpString.push_back("Move tool in hand (sim) and change kinematic extension (sim and real). ");
      helpString.push_back("@return true/false on success/failure of regrasping the tool ");
    }
    if (functionName=="findPose") {
      helpString.push_back("bool findPose() ");
      helpString.push_back("Start the methods to find the tool pose by aligning the partial reconstruction with a tool model: \n ");
      helpString.push_back("@return true/false on success/failure of finding the tool pose ");
    }
    if (functionName=="getToolFeats") {
      helpString.push_back("bool getToolFeats() ");
      helpString.push_back("Queries toolFeatExt module to extract the feaures of the loaded module \n ");
      helpString.push_back("@return true/false on success/failure to extract features ");
    }
    if (functionName=="objLoc") {
      helpString.push_back("yarp::sig::Vector objLoc() ");
      helpString.push_back("Finds out the location of the object and returns its coordinates \n ");
      helpString.push_back("@return Vector bottle with object coordinates in 3D X Y Z ");
    }
    if (functionName=="objRot") {
      helpString.push_back("yarp::sig::Vector objRot() ");
      helpString.push_back("Finds out the rotation of the object and returns its rotation values \n ");
      helpString.push_back("@return Vector bottle with object rotation ");
    }
    if (functionName=="goHome") {
      helpString.push_back("bool goHome(const bool hands = 0) ");
      helpString.push_back("Adopt home position (optionally opens hands too) ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="findTable") {
      helpString.push_back("bool findTable(const bool calib = 1) ");
      helpString.push_back("Calibrates the table height through ARE: \n ");
      helpString.push_back("@param calib . True performs calibration, false gets stored data. ");
      helpString.push_back("@returns true ");
    }
    if (functionName=="slide") {
      helpString.push_back("bool slide(const double thetha = 0, const double radius = 0) ");
      helpString.push_back("Performs a slide action along the diameter of the circle of given radius and center on the object, from theta to -theta. \n ");
      helpString.push_back("@return true/false on success/failure to do Action ");
    }
    if (functionName=="drag") {
      helpString.push_back("bool drag(const double thetha = 0, const double radius = 0, const double tilt = -15) ");
      helpString.push_back("Performs a drag action from the object in the direction indicated by theta and radius. \n ");
      helpString.push_back("@return true/false on success/failure to do Action ");
    }
    if (functionName=="drag3D") {
      helpString.push_back("bool drag3D(const double x, const double y, const double z, const double thetha = 0, const double radius = 0, const double tilt = -15, const bool useTool = 1) ");
      helpString.push_back("Performs a drag action from the given 3D coords in the direction indicated by theta and radius. \n ");
      helpString.push_back("The trial consist on locating the object and executing the slide action ");
      helpString.push_back("@return true/false on success/failure to do Action ");
    }
    if (functionName=="trackObj") {
      helpString.push_back("bool trackObj() ");
      helpString.push_back("(Re)Initializes object template for tracking. The user has to click on the upper left and lower right corners of the object to be tracked (in that order).\n ");
      helpString.push_back("@return true/false on success/failure to set the template and (re)start tracking ");
    }
    if (functionName=="compEff") {
      helpString.push_back("bool compEff() ");
      helpString.push_back("Computes the effect of the action in terms of distance displaced, angle of displacement and rotation exerted on the object. \n ");
      helpString.push_back("@return true/false on success/failure to compute Effect ");
    }
    if (functionName=="resetObj") {
      helpString.push_back("bool resetObj() ");
      helpString.push_back("Push the cube back in teh orginal position (in SIM) ");
      helpString.push_back("@return true/false on success/failure to compute Effect ");
    }
    if (functionName=="runRandPoses") {
      helpString.push_back("bool runRandPoses(const int32_t numPoses, const int32_t numAct = 8) ");
      helpString.push_back("Runs numAct actions for numPoses times, each time with a random tool in a random pose ");
      helpString.push_back("@return true/false on success/failure to perform all actions ");
    }
    if (functionName=="runToolPose") {
      helpString.push_back("bool runToolPose(const int32_t numRep = 1, const int32_t numAct = 8) ");
      helpString.push_back("Runs numAct actions with the given tool on the given pose and computes the effect. \n ");
      helpString.push_back("@return true/false on success/failure to perform all actions ");
    }
    if (functionName=="runToolTrial") {
      helpString.push_back("bool runToolTrial(const int32_t numRep = 1, const std::string& tool = \"\", const int32_t numAct = 8) ");
      helpString.push_back("For the given tool, performs numAct actions for each toolpose. Tries all grasp orientation {-90, 0, 90} ");
      helpString.push_back("@return true/false on success/failure to perfomr all actions on all toolPoses ");
    }
    if (functionName=="runExp") {
      helpString.push_back("bool runExp(const std::string& tool, const int32_t toolEnd = 54) ");
      helpString.push_back("Runs full trials for all tool with indices between toolini and toolEnd. \n ");
      helpString.push_back("@return true/false on success/failure to perform all actions ");
    }
    if (functionName=="selectAction") {
      helpString.push_back("bool selectAction(const int32_t goal = 1) ");
      helpString.push_back("Extracts OMS-EGI features from grasped tool and calls MATLAB to get the predicted effects of possible action ");
      helpString.push_back("Then, chooses the best action for the given goal (1: maxDist, 2: Pull) \n ");
      helpString.push_back("@return true/false on success/failure to query matlab and perform action ");
    }
    if (functionName=="predExp") {
      helpString.push_back("bool predExp(const int32_t goal = 1) ");
      helpString.push_back("Runs selectAction trials for all the test tools for given goal (1: maxDist, 2: Pull) ");
      helpString.push_back("@return true/false on success/failure to perform actions selections ");
    }
    if (functionName=="help") {
      helpString.push_back("std::vector<std::string> help(const std::string& functionName=\"--all\")");
      helpString.push_back("Return list of available commands, or help message for a specific function");
      helpString.push_back("@param functionName name of command for which to get a detailed description. If none or '--all' is provided, print list of available commands");
      helpString.push_back("@return list of strings (one string per line)");
    }
  }
  if ( helpString.empty()) helpString.push_back("Command not found");
  return helpString;
}


