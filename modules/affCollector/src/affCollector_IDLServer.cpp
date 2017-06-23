// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <affCollector_IDLServer.h>
#include <yarp/os/idl/WireTypes.h>



class affCollector_IDLServer_start : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_quit : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_setnumact : public yarp::os::Portable {
public:
  int32_t numAct;
  bool _return;
  void init(const int32_t numAct);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_setactlabels : public yarp::os::Portable {
public:
  yarp::os::Bottle labels;
  bool _return;
  void init(const yarp::os::Bottle& labels);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_getactlabels : public yarp::os::Portable {
public:
  yarp::os::Bottle _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_setlabel : public yarp::os::Portable {
public:
  std::string label;
  int32_t _return;
  void init(const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_getlabel : public yarp::os::Portable {
public:
  std::string _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_updateAff : public yarp::os::Portable {
public:
  int32_t act;
  double eff;
  int32_t labI;
  double _return;
  void init(const int32_t act, const double eff, const int32_t labI);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_getAffs : public yarp::os::Portable {
public:
  std::string label;
  yarp::os::Bottle _return;
  void init(const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_getAffHist : public yarp::os::Portable {
public:
  std::string label;
  int32_t act;
  yarp::os::Bottle _return;
  void init(const std::string& label, const int32_t act);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_selectTool : public yarp::os::Portable {
public:
  int32_t action;
  std::string _return;
  void init(const int32_t action);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_activeExp : public yarp::os::Portable {
public:
  std::string label;
  std::string _return;
  void init(const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_balanceExp : public yarp::os::Portable {
public:
  std::string label;
  std::string _return;
  void init(const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_reset : public yarp::os::Portable {
public:
  std::string label;
  bool _return;
  void init(const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_clearAll : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_forgetAll : public yarp::os::Portable {
public:
  std::string _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_savetofile : public yarp::os::Portable {
public:
  std::string file;
  bool _return;
  void init(const std::string& file);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_readfile : public yarp::os::Portable {
public:
  std::string file;
  bool _return;
  void init(const std::string& file);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

class affCollector_IDLServer_verbose : public yarp::os::Portable {
public:
  bool verb;
  bool _return;
  void init(const bool verb);
  virtual bool write(yarp::os::ConnectionWriter& connection) YARP_OVERRIDE;
  virtual bool read(yarp::os::ConnectionReader& connection) YARP_OVERRIDE;
};

bool affCollector_IDLServer_start::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("start",1,1)) return false;
  return true;
}

bool affCollector_IDLServer_start::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_start::init() {
  _return = false;
}

bool affCollector_IDLServer_quit::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("quit",1,1)) return false;
  return true;
}

bool affCollector_IDLServer_quit::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_quit::init() {
  _return = false;
}

bool affCollector_IDLServer_setnumact::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("setnumact",1,1)) return false;
  if (!writer.writeI32(numAct)) return false;
  return true;
}

bool affCollector_IDLServer_setnumact::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_setnumact::init(const int32_t numAct) {
  _return = false;
  this->numAct = numAct;
}

bool affCollector_IDLServer_setactlabels::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("setactlabels",1,1)) return false;
  if (!writer.write(labels)) return false;
  return true;
}

bool affCollector_IDLServer_setactlabels::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_setactlabels::init(const yarp::os::Bottle& labels) {
  _return = false;
  this->labels = labels;
}

bool affCollector_IDLServer_getactlabels::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("getactlabels",1,1)) return false;
  return true;
}

bool affCollector_IDLServer_getactlabels::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.read(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_getactlabels::init() {
}

bool affCollector_IDLServer_setlabel::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("setlabel",1,1)) return false;
  if (!writer.writeString(label)) return false;
  return true;
}

bool affCollector_IDLServer_setlabel::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readI32(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_setlabel::init(const std::string& label) {
  _return = 0;
  this->label = label;
}

bool affCollector_IDLServer_getlabel::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("getlabel",1,1)) return false;
  return true;
}

bool affCollector_IDLServer_getlabel::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readString(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_getlabel::init() {
  _return = "";
}

bool affCollector_IDLServer_updateAff::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(4)) return false;
  if (!writer.writeTag("updateAff",1,1)) return false;
  if (!writer.writeI32(act)) return false;
  if (!writer.writeDouble(eff)) return false;
  if (!writer.writeI32(labI)) return false;
  return true;
}

bool affCollector_IDLServer_updateAff::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readDouble(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_updateAff::init(const int32_t act, const double eff, const int32_t labI) {
  _return = (double)0;
  this->act = act;
  this->eff = eff;
  this->labI = labI;
}

bool affCollector_IDLServer_getAffs::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("getAffs",1,1)) return false;
  if (!writer.writeString(label)) return false;
  return true;
}

bool affCollector_IDLServer_getAffs::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.read(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_getAffs::init(const std::string& label) {
  this->label = label;
}

bool affCollector_IDLServer_getAffHist::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("getAffHist",1,1)) return false;
  if (!writer.writeString(label)) return false;
  if (!writer.writeI32(act)) return false;
  return true;
}

bool affCollector_IDLServer_getAffHist::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.read(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_getAffHist::init(const std::string& label, const int32_t act) {
  this->label = label;
  this->act = act;
}

bool affCollector_IDLServer_selectTool::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("selectTool",1,1)) return false;
  if (!writer.writeI32(action)) return false;
  return true;
}

bool affCollector_IDLServer_selectTool::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readString(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_selectTool::init(const int32_t action) {
  _return = "";
  this->action = action;
}

bool affCollector_IDLServer_activeExp::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("activeExp",1,1)) return false;
  if (!writer.writeString(label)) return false;
  return true;
}

bool affCollector_IDLServer_activeExp::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readString(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_activeExp::init(const std::string& label) {
  _return = "";
  this->label = label;
}

bool affCollector_IDLServer_balanceExp::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("balanceExp",1,1)) return false;
  if (!writer.writeString(label)) return false;
  return true;
}

bool affCollector_IDLServer_balanceExp::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readString(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_balanceExp::init(const std::string& label) {
  _return = "";
  this->label = label;
}

bool affCollector_IDLServer_reset::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("reset",1,1)) return false;
  if (!writer.writeString(label)) return false;
  return true;
}

bool affCollector_IDLServer_reset::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_reset::init(const std::string& label) {
  _return = false;
  this->label = label;
}

bool affCollector_IDLServer_clearAll::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("clearAll",1,1)) return false;
  return true;
}

bool affCollector_IDLServer_clearAll::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_clearAll::init() {
  _return = false;
}

bool affCollector_IDLServer_forgetAll::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("forgetAll",1,1)) return false;
  return true;
}

bool affCollector_IDLServer_forgetAll::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readString(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_forgetAll::init() {
  _return = "";
}

bool affCollector_IDLServer_savetofile::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("savetofile",1,1)) return false;
  if (!writer.writeString(file)) return false;
  return true;
}

bool affCollector_IDLServer_savetofile::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_savetofile::init(const std::string& file) {
  _return = false;
  this->file = file;
}

bool affCollector_IDLServer_readfile::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("readfile",1,1)) return false;
  if (!writer.writeString(file)) return false;
  return true;
}

bool affCollector_IDLServer_readfile::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_readfile::init(const std::string& file) {
  _return = false;
  this->file = file;
}

bool affCollector_IDLServer_verbose::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("verbose",1,1)) return false;
  if (!writer.writeBool(verb)) return false;
  return true;
}

bool affCollector_IDLServer_verbose::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_verbose::init(const bool verb) {
  _return = false;
  this->verb = verb;
}

affCollector_IDLServer::affCollector_IDLServer() {
  yarp().setOwner(*this);
}
bool affCollector_IDLServer::start() {
  bool _return = false;
  affCollector_IDLServer_start helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::start()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::quit() {
  bool _return = false;
  affCollector_IDLServer_quit helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::quit()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::setnumact(const int32_t numAct) {
  bool _return = false;
  affCollector_IDLServer_setnumact helper;
  helper.init(numAct);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::setnumact(const int32_t numAct)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::setactlabels(const yarp::os::Bottle& labels) {
  bool _return = false;
  affCollector_IDLServer_setactlabels helper;
  helper.init(labels);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::setactlabels(const yarp::os::Bottle& labels)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
yarp::os::Bottle affCollector_IDLServer::getactlabels() {
  yarp::os::Bottle _return;
  affCollector_IDLServer_getactlabels helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","yarp::os::Bottle affCollector_IDLServer::getactlabels()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t affCollector_IDLServer::setlabel(const std::string& label) {
  int32_t _return = 0;
  affCollector_IDLServer_setlabel helper;
  helper.init(label);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","int32_t affCollector_IDLServer::setlabel(const std::string& label)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::string affCollector_IDLServer::getlabel() {
  std::string _return = "";
  affCollector_IDLServer_getlabel helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::string affCollector_IDLServer::getlabel()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
double affCollector_IDLServer::updateAff(const int32_t act, const double eff, const int32_t labI) {
  double _return = (double)0;
  affCollector_IDLServer_updateAff helper;
  helper.init(act,eff,labI);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","double affCollector_IDLServer::updateAff(const int32_t act, const double eff, const int32_t labI)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
yarp::os::Bottle affCollector_IDLServer::getAffs(const std::string& label) {
  yarp::os::Bottle _return;
  affCollector_IDLServer_getAffs helper;
  helper.init(label);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","yarp::os::Bottle affCollector_IDLServer::getAffs(const std::string& label)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
yarp::os::Bottle affCollector_IDLServer::getAffHist(const std::string& label, const int32_t act) {
  yarp::os::Bottle _return;
  affCollector_IDLServer_getAffHist helper;
  helper.init(label,act);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","yarp::os::Bottle affCollector_IDLServer::getAffHist(const std::string& label, const int32_t act)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::string affCollector_IDLServer::selectTool(const int32_t action) {
  std::string _return = "";
  affCollector_IDLServer_selectTool helper;
  helper.init(action);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::string affCollector_IDLServer::selectTool(const int32_t action)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::string affCollector_IDLServer::activeExp(const std::string& label) {
  std::string _return = "";
  affCollector_IDLServer_activeExp helper;
  helper.init(label);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::string affCollector_IDLServer::activeExp(const std::string& label)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::string affCollector_IDLServer::balanceExp(const std::string& label) {
  std::string _return = "";
  affCollector_IDLServer_balanceExp helper;
  helper.init(label);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::string affCollector_IDLServer::balanceExp(const std::string& label)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::reset(const std::string& label) {
  bool _return = false;
  affCollector_IDLServer_reset helper;
  helper.init(label);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::reset(const std::string& label)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::clearAll() {
  bool _return = false;
  affCollector_IDLServer_clearAll helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::clearAll()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::string affCollector_IDLServer::forgetAll() {
  std::string _return = "";
  affCollector_IDLServer_forgetAll helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::string affCollector_IDLServer::forgetAll()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::savetofile(const std::string& file) {
  bool _return = false;
  affCollector_IDLServer_savetofile helper;
  helper.init(file);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::savetofile(const std::string& file)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::readfile(const std::string& file) {
  bool _return = false;
  affCollector_IDLServer_readfile helper;
  helper.init(file);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::readfile(const std::string& file)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affCollector_IDLServer::verbose(const bool verb) {
  bool _return = false;
  affCollector_IDLServer_verbose helper;
  helper.init(verb);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::verbose(const bool verb)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool affCollector_IDLServer::read(yarp::os::ConnectionReader& connection) {
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
    if (tag == "setnumact") {
      int32_t numAct;
      if (!reader.readI32(numAct)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = setnumact(numAct);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "setactlabels") {
      yarp::os::Bottle labels;
      if (!reader.read(labels)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = setactlabels(labels);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getactlabels") {
      yarp::os::Bottle _return;
      _return = getactlabels();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.write(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "setlabel") {
      std::string label;
      if (!reader.readString(label)) {
        reader.fail();
        return false;
      }
      int32_t _return;
      _return = setlabel(label);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getlabel") {
      std::string _return;
      _return = getlabel();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "updateAff") {
      int32_t act;
      double eff;
      int32_t labI;
      if (!reader.readI32(act)) {
        reader.fail();
        return false;
      }
      if (!reader.readDouble(eff)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(labI)) {
        labI = -1;
      }
      double _return;
      _return = updateAff(act,eff,labI);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeDouble(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getAffs") {
      std::string label;
      if (!reader.readString(label)) {
        label = "active";
      }
      yarp::os::Bottle _return;
      _return = getAffs(label);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.write(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getAffHist") {
      std::string label;
      int32_t act;
      if (!reader.readString(label)) {
        label = "active";
      }
      if (!reader.readI32(act)) {
        act = -1;
      }
      yarp::os::Bottle _return;
      _return = getAffHist(label,act);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.write(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "selectTool") {
      int32_t action;
      if (!reader.readI32(action)) {
        reader.fail();
        return false;
      }
      std::string _return;
      _return = selectTool(action);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "activeExp") {
      std::string label;
      if (!reader.readString(label)) {
        label = "active";
      }
      std::string _return;
      _return = activeExp(label);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "balanceExp") {
      std::string label;
      if (!reader.readString(label)) {
        label = "active";
      }
      std::string _return;
      _return = balanceExp(label);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "reset") {
      std::string label;
      if (!reader.readString(label)) {
        label = "active";
      }
      bool _return;
      _return = reset(label);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "clearAll") {
      bool _return;
      _return = clearAll();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "forgetAll") {
      std::string _return;
      _return = forgetAll();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "savetofile") {
      std::string file;
      if (!reader.readString(file)) {
        file = "default";
      }
      bool _return;
      _return = savetofile(file);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "readfile") {
      std::string file;
      if (!reader.readString(file)) {
        file = "default";
      }
      bool _return;
      _return = readfile(file);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "verbose") {
      bool verb;
      if (!reader.readBool(verb)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = verbose(verb);
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

std::vector<std::string> affCollector_IDLServer::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("start");
    helpString.push_back("quit");
    helpString.push_back("setnumact");
    helpString.push_back("setactlabels");
    helpString.push_back("getactlabels");
    helpString.push_back("setlabel");
    helpString.push_back("getlabel");
    helpString.push_back("updateAff");
    helpString.push_back("getAffs");
    helpString.push_back("getAffHist");
    helpString.push_back("selectTool");
    helpString.push_back("activeExp");
    helpString.push_back("balanceExp");
    helpString.push_back("reset");
    helpString.push_back("clearAll");
    helpString.push_back("forgetAll");
    helpString.push_back("savetofile");
    helpString.push_back("readfile");
    helpString.push_back("verbose");
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
    if (functionName=="setnumact") {
      helpString.push_back("bool setnumact(const int32_t numAct) ");
      helpString.push_back("Allows to modify online the number of possible actions ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="setactlabels") {
      helpString.push_back("bool setactlabels(const yarp::os::Bottle& labels) ");
      helpString.push_back("Sets the labels for all the possible actions ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getactlabels") {
      helpString.push_back("yarp::os::Bottle getactlabels() ");
      helpString.push_back("Returns the labels of all possible actions ");
    }
    if (functionName=="setlabel") {
      helpString.push_back("int32_t setlabel(const std::string& label) ");
      helpString.push_back("Activates a category (or creates if note previously exisiting), for which affordance data (action success rate) can be updated. ");
      helpString.push_back("@return the index of the active (new or not) label ");
    }
    if (functionName=="getlabel") {
      helpString.push_back("std::string getlabel() ");
      helpString.push_back("Returns the active label (object/tool/cateagory/etc, user defined). ");
    }
    if (functionName=="updateAff") {
      helpString.push_back("double updateAff(const int32_t act, const double eff, const int32_t labI = -1) ");
      helpString.push_back("For the active label, update its affordance success rate for a given action, based on its effect. ");
      helpString.push_back("@returns the updated success rate for that action and label ");
    }
    if (functionName=="getAffs") {
      helpString.push_back("yarp::os::Bottle getAffs(const std::string& label = \"active\") ");
      helpString.push_back("Returns the success rate for all actions in the repertoire for a given known label (the active one by default). ");
      helpString.push_back("if label == 'all', returns a concatenated vector with all konwn affordances. ");
    }
    if (functionName=="getAffHist") {
      helpString.push_back("yarp::os::Bottle getAffHist(const std::string& label = \"active\", const int32_t act = -1) ");
      helpString.push_back("Returns the history of effects for a given action and known label (the active one by default). ");
    }
    if (functionName=="selectTool") {
      helpString.push_back("std::string selectTool(const int32_t action) ");
      helpString.push_back(" Based on the previously learnt affordances, returns the best label for a given action/task. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="activeExp") {
      helpString.push_back("std::string activeExp(const std::string& label = \"active\") ");
      helpString.push_back("Returns the label and action with less certainty of known ones, based on variance. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="balanceExp") {
      helpString.push_back("std::string balanceExp(const std::string& label = \"active\") ");
      helpString.push_back("Returns the label and action that has been tried the least. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="reset") {
      helpString.push_back("bool reset(const std::string& label = \"active\") ");
      helpString.push_back("Clears all the learnt affordances of the active label, and sets it to unknown. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="clearAll") {
      helpString.push_back("bool clearAll() ");
      helpString.push_back("Clears all the learnt affordances all labels, and sets them to unknown. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="forgetAll") {
      helpString.push_back("std::string forgetAll() ");
      helpString.push_back("Removes all teh content from the memory file. Use with precaution (requires confirmation) ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="savetofile") {
      helpString.push_back("bool savetofile(const std::string& file = \"default\") ");
      helpString.push_back("Saves the known affordances history in a given file. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="readfile") {
      helpString.push_back("bool readfile(const std::string& file = \"default\") ");
      helpString.push_back("Reads labels and aff histories from files 'fileLables.txt' and the affHist in another 'fileHist.txt' ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="verbose") {
      helpString.push_back("bool verbose(const bool verb) ");
      helpString.push_back("Activates/Deactivates more verbose execution of the module. ");
      helpString.push_back("@return true/false on success/failure ");
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


