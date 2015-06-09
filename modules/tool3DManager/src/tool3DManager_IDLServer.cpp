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

class tool3DManager_IDLServer_goHome : public yarp::os::Portable {
public:
  bool hands;
  bool _return;
  void init(const bool hands);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_getTool : public yarp::os::Portable {
public:
  int32_t tool;
  int32_t deg;
  double disp;
  bool _return;
  void init(const int32_t tool, const int32_t deg, const double disp);
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

class tool3DManager_IDLServer_slide : public yarp::os::Portable {
public:
  double thetha;
  double radius;
  bool _return;
  void init(const double thetha, const double radius);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_pull : public yarp::os::Portable {
public:
  double thetha;
  double radius;
  bool _return;
  void init(const double thetha, const double radius);
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

class tool3DManager_IDLServer_runToolPose : public yarp::os::Portable {
public:
  int32_t toolI;
  int32_t graspOr;
  double graspDisp;
  int32_t numAct;
  bool _return;
  void init(const int32_t toolI, const int32_t graspOr, const double graspDisp, const int32_t numAct);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_runToolTrial : public yarp::os::Portable {
public:
  int32_t toolI;
  bool _return;
  void init(const int32_t toolI);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class tool3DManager_IDLServer_runExp : public yarp::os::Portable {
public:
  int32_t toolIni;
  int32_t toolEnd;
  bool _return;
  void init(const int32_t toolIni, const int32_t toolEnd);
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

bool tool3DManager_IDLServer_getTool::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(4)) return false;
  if (!writer.writeTag("getTool",1,1)) return false;
  if (!writer.writeI32(tool)) return false;
  if (!writer.writeI32(deg)) return false;
  if (!writer.writeDouble(disp)) return false;
  return true;
}

bool tool3DManager_IDLServer_getTool::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_getTool::init(const int32_t tool, const int32_t deg, const double disp) {
  _return = false;
  this->tool = tool;
  this->deg = deg;
  this->disp = disp;
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

bool tool3DManager_IDLServer_pull::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("pull",1,1)) return false;
  if (!writer.writeDouble(thetha)) return false;
  if (!writer.writeDouble(radius)) return false;
  return true;
}

bool tool3DManager_IDLServer_pull::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void tool3DManager_IDLServer_pull::init(const double thetha, const double radius) {
  _return = false;
  this->thetha = thetha;
  this->radius = radius;
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

bool tool3DManager_IDLServer_runToolPose::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(5)) return false;
  if (!writer.writeTag("runToolPose",1,1)) return false;
  if (!writer.writeI32(toolI)) return false;
  if (!writer.writeI32(graspOr)) return false;
  if (!writer.writeDouble(graspDisp)) return false;
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

void tool3DManager_IDLServer_runToolPose::init(const int32_t toolI, const int32_t graspOr, const double graspDisp, const int32_t numAct) {
  _return = false;
  this->toolI = toolI;
  this->graspOr = graspOr;
  this->graspDisp = graspDisp;
  this->numAct = numAct;
}

bool tool3DManager_IDLServer_runToolTrial::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  if (!writer.writeTag("runToolTrial",1,1)) return false;
  if (!writer.writeI32(toolI)) return false;
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

void tool3DManager_IDLServer_runToolTrial::init(const int32_t toolI) {
  _return = false;
  this->toolI = toolI;
}

bool tool3DManager_IDLServer_runExp::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  if (!writer.writeTag("runExp",1,1)) return false;
  if (!writer.writeI32(toolIni)) return false;
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

void tool3DManager_IDLServer_runExp::init(const int32_t toolIni, const int32_t toolEnd) {
  _return = false;
  this->toolIni = toolIni;
  this->toolEnd = toolEnd;
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
bool tool3DManager_IDLServer::getTool(const int32_t tool, const int32_t deg, const double disp) {
  bool _return = false;
  tool3DManager_IDLServer_getTool helper;
  helper.init(tool,deg,disp);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::getTool(const int32_t tool, const int32_t deg, const double disp)");
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
bool tool3DManager_IDLServer::pull(const double thetha, const double radius) {
  bool _return = false;
  tool3DManager_IDLServer_pull helper;
  helper.init(thetha,radius);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::pull(const double thetha, const double radius)");
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
bool tool3DManager_IDLServer::runToolPose(const int32_t toolI, const int32_t graspOr, const double graspDisp, const int32_t numAct) {
  bool _return = false;
  tool3DManager_IDLServer_runToolPose helper;
  helper.init(toolI,graspOr,graspDisp,numAct);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::runToolPose(const int32_t toolI, const int32_t graspOr, const double graspDisp, const int32_t numAct)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::runToolTrial(const int32_t toolI) {
  bool _return = false;
  tool3DManager_IDLServer_runToolTrial helper;
  helper.init(toolI);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::runToolTrial(const int32_t toolI)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool tool3DManager_IDLServer::runExp(const int32_t toolIni, const int32_t toolEnd) {
  bool _return = false;
  tool3DManager_IDLServer_runExp helper;
  helper.init(toolIni,toolEnd);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool tool3DManager_IDLServer::runExp(const int32_t toolIni, const int32_t toolEnd)");
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
    if (tag == "getTool") {
      int32_t tool;
      int32_t deg;
      double disp;
      if (!reader.readI32(tool)) {
        tool = 0;
      }
      if (!reader.readI32(deg)) {
        deg = 0;
      }
      if (!reader.readDouble(disp)) {
        disp = 0;
      }
      bool _return;
      _return = getTool(tool,deg,disp);
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
    if (tag == "pull") {
      double thetha;
      double radius;
      if (!reader.readDouble(thetha)) {
        thetha = 0;
      }
      if (!reader.readDouble(radius)) {
        radius = 0;
      }
      bool _return;
      _return = pull(thetha,radius);
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
    if (tag == "runToolPose") {
      int32_t toolI;
      int32_t graspOr;
      double graspDisp;
      int32_t numAct;
      if (!reader.readI32(toolI)) {
        reader.fail();
        return false;
      }
      if (!reader.readI32(graspOr)) {
        graspOr = 0;
      }
      if (!reader.readDouble(graspDisp)) {
        graspDisp = 0;
      }
      if (!reader.readI32(numAct)) {
        numAct = 8;
      }
      bool _return;
      _return = runToolPose(toolI,graspOr,graspDisp,numAct);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "runToolTrial") {
      int32_t toolI;
      if (!reader.readI32(toolI)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = runToolTrial(toolI);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "runExp") {
      int32_t toolIni;
      int32_t toolEnd;
      if (!reader.readI32(toolIni)) {
        toolIni = 1;
      }
      if (!reader.readI32(toolEnd)) {
        toolEnd = 50;
      }
      bool _return;
      _return = runExp(toolIni,toolEnd);
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
    helpString.push_back("goHome");
    helpString.push_back("getTool");
    helpString.push_back("getToolFeats");
    helpString.push_back("slide");
    helpString.push_back("pull");
    helpString.push_back("compEff");
    helpString.push_back("runToolPose");
    helpString.push_back("runToolTrial");
    helpString.push_back("runExp");
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
    if (functionName=="goHome") {
      helpString.push_back("bool goHome(const bool hands = 0) ");
      helpString.push_back("Adopt home position ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getTool") {
      helpString.push_back("bool getTool(const int32_t tool = 0, const int32_t deg = 0, const double disp = 0) ");
      helpString.push_back("Performs the sequence to get the tool: \n ");
      helpString.push_back("- On the simulator calls simtoolloader which creates the tool  <i>tool</i> at the orientation <i>deg</i> and displacement on the -Y hand axis <i>disp</i>. Uses magnet to hold it to hand. ");
      helpString.push_back("- Moreover, the tool end effector is located and attached to the kinematic chain with karmaMotor and shown with karmaToolFinder. ");
      helpString.push_back("- On the real robot moves hand to receiving position and closes hand on tool grasp. In this case  <i>tool</i> and <i> deg</i> should correspond to the way in which the tool is given ");
      helpString.push_back("@return true/false on success/failure of looking at that position ");
    }
    if (functionName=="getToolFeats") {
      helpString.push_back("bool getToolFeats() ");
      helpString.push_back("Queries toolFeatExt module to extract the feaures of the loaded module \n ");
      helpString.push_back("@return true/false on success/failure to extract features ");
    }
    if (functionName=="slide") {
      helpString.push_back("bool slide(const double thetha = 0, const double radius = 0) ");
      helpString.push_back("Performs a slide action along the diameter of the circle of radius and center on the object, from theta to -theta. \n ");
      helpString.push_back("The trial consist on locating the object and executing the slide action ");
      helpString.push_back("@return true/false on success/failure to do Action ");
    }
    if (functionName=="pull") {
      helpString.push_back("bool pull(const double thetha = 0, const double radius = 0) ");
      helpString.push_back("Performs a pull action from the object to the direction indicated by theta and radius. \n ");
      helpString.push_back("The trial consist on locating the object and executing the slide action ");
      helpString.push_back("@return true/false on success/failure to do Action ");
    }
    if (functionName=="compEff") {
      helpString.push_back("bool compEff() ");
      helpString.push_back("Computes the effect of the action in terms of distance displaced, angle of displacement and rotation exerted on the object. \n ");
      helpString.push_back("@return true/false on success/failure to compute Effect ");
    }
    if (functionName=="runToolPose") {
      helpString.push_back("bool runToolPose(const int32_t toolI, const int32_t graspOr = 0, const double graspDisp = 0, const int32_t numAct = 8) ");
      helpString.push_back("Runs N actions with the given tool on the given pose and computes the effect. \n ");
      helpString.push_back("@return true/false on success/failure to perform all actions ");
    }
    if (functionName=="runToolTrial") {
      helpString.push_back("bool runToolTrial(const int32_t toolI) ");
      helpString.push_back("For the given tool, performs N actions for each toolpose. Tries all toolposes as combinations ");
      helpString.push_back("of grasp orientation {-90, 0, 90} and displacements { -2, 0, 2} cm. \n ");
      helpString.push_back("@return true/false on success/failure to perfomr all actions on all toolPoses ");
    }
    if (functionName=="runExp") {
      helpString.push_back("bool runExp(const int32_t toolIni = 1, const int32_t toolEnd = 50) ");
      helpString.push_back("Runs full trials for all tool with indices between toolini and toolEnd. \n ");
      helpString.push_back("@return true/false on success/failure to perform all actions ");
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

