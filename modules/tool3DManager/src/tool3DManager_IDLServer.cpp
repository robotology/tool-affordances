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

class tool3DManager_IDLServer_slide : public yarp::os::Portable {
public:
  double thetha;
  double radius;
  bool _return;
  void init(const double thetha, const double radius);
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
    helpString.push_back("slide");
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
    if (functionName=="slide") {
      helpString.push_back("bool slide(const double thetha = 0, const double radius = 0) ");
      helpString.push_back("Performs a slide action from orientation theta and distance radius to the detected center of the object. \n ");
      helpString.push_back("The trial consist on locating the object and executing the slide action ");
      helpString.push_back("@return true/false on success/failure to do Action ");
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


