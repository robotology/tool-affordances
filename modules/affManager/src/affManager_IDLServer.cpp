// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <affManager_IDLServer.h>
#include <yarp/os/idl/WireTypes.h>



class affManager_IDLServer_start : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("start",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_goHome : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("goHome",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_lookAtTool : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("lookAtTool",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_lookAtRack : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("lookAtRack",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_lookAtPoint : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("lookAtPoint",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_lookAtObject : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("lookAtObject",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_askForTool : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("askForTool",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_reachTool : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("reachTool",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_graspTool : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("graspTool",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_observeTool : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("observeTool",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_observeObj : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("observeObj",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_findTools : public yarp::os::Portable {
public:
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("findTools",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readI32(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_selectTool : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("selectTool",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

class affManager_IDLServer_quit : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("quit",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.readBool(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

bool affManager_IDLServer::start() {
  bool _return = false;
  affManager_IDLServer_start helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::start()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::goHome() {
  bool _return = false;
  affManager_IDLServer_goHome helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::goHome()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::lookAtTool() {
  bool _return = false;
  affManager_IDLServer_lookAtTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::lookAtTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::lookAtRack() {
  bool _return = false;
  affManager_IDLServer_lookAtRack helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::lookAtRack()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::lookAtPoint() {
  bool _return = false;
  affManager_IDLServer_lookAtPoint helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::lookAtPoint()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::lookAtObject() {
  bool _return = false;
  affManager_IDLServer_lookAtObject helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::lookAtObject()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::askForTool() {
  bool _return = false;
  affManager_IDLServer_askForTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::askForTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::reachTool() {
  bool _return = false;
  affManager_IDLServer_reachTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::reachTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::graspTool() {
  bool _return = false;
  affManager_IDLServer_graspTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::graspTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::observeTool() {
  bool _return = false;
  affManager_IDLServer_observeTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::observeTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::observeObj() {
  bool _return = false;
  affManager_IDLServer_observeObj helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::observeObj()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t affManager_IDLServer::findTools() {
  int32_t _return = 0;
  affManager_IDLServer_findTools helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t affManager_IDLServer::findTools()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::selectTool() {
  bool _return = false;
  affManager_IDLServer_selectTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::selectTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::quit() {
  bool _return = false;
  affManager_IDLServer_quit helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::quit()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool affManager_IDLServer::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  reader.expectAccept();
  if (!reader.readListHeader()) { reader.fail(); return false; }
  yarp::os::ConstString tag = reader.readTag();
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
    if (tag == "goHome") {
      bool _return;
      _return = goHome();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "lookAtTool") {
      bool _return;
      _return = lookAtTool();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "lookAtRack") {
      bool _return;
      _return = lookAtRack();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "lookAtPoint") {
      bool _return;
      _return = lookAtPoint();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "lookAtObject") {
      bool _return;
      _return = lookAtObject();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "askForTool") {
      bool _return;
      _return = askForTool();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "reachTool") {
      bool _return;
      _return = reachTool();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "graspTool") {
      bool _return;
      _return = graspTool();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "observeTool") {
      bool _return;
      _return = observeTool();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "observeObj") {
      bool _return;
      _return = observeObj();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "findTools") {
      int32_t _return;
      _return = findTools();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "selectTool") {
      bool _return;
      _return = selectTool();
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

std::vector<std::string> affManager_IDLServer::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("start");
    helpString.push_back("goHome");
    helpString.push_back("lookAtTool");
    helpString.push_back("lookAtRack");
    helpString.push_back("lookAtPoint");
    helpString.push_back("lookAtObject");
    helpString.push_back("askForTool");
    helpString.push_back("reachTool");
    helpString.push_back("graspTool");
    helpString.push_back("observeTool");
    helpString.push_back("observeObj");
    helpString.push_back("findTools");
    helpString.push_back("selectTool");
    helpString.push_back("quit");
    helpString.push_back("help");
  }
  else {
    if (functionName=="start") {
      helpString.push_back("bool start() ");
      helpString.push_back("Start the module ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="goHome") {
      helpString.push_back("bool goHome() ");
      helpString.push_back("Adopt home position ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="lookAtTool") {
      helpString.push_back("bool lookAtTool() ");
      helpString.push_back("Performs several routines until it finds a tool ");
      helpString.push_back("@return true/false on success/failure of finding a tool in any of its possible locations ");
    }
    if (functionName=="lookAtRack") {
      helpString.push_back("bool lookAtRack() ");
      helpString.push_back("Look at predefined rack position ");
      helpString.push_back("@return true/false on success/failure of looking at that position ");
    }
    if (functionName=="lookAtPoint") {
      helpString.push_back("bool lookAtPoint() ");
      helpString.push_back("Look at user given coordinates ");
      helpString.push_back("@return true/false on success/failure of looking at that position ");
    }
    if (functionName=="lookAtObject") {
      helpString.push_back("bool lookAtObject() ");
      helpString.push_back("Look at the target object ");
      helpString.push_back("@return true/false on success/failure of finding/looking at object ");
    }
    if (functionName=="askForTool") {
      helpString.push_back("bool askForTool() ");
      helpString.push_back("Asks for tool and move the arm to receiving position ");
      helpString.push_back("@return true/false on success/failure on going to receive position ");
    }
    if (functionName=="reachTool") {
      helpString.push_back("bool reachTool() ");
      helpString.push_back("Robot reaches for the tool without grasping ");
      helpString.push_back("@return true/false on success/failure on computing and reaching the desired point ");
    }
    if (functionName=="graspTool") {
      helpString.push_back("bool graspTool() ");
      helpString.push_back("Closes hand on tool previously reached or received. ");
      helpString.push_back("@return true/false on success/failure on holding the tool. ");
    }
    if (functionName=="observeTool") {
      helpString.push_back("bool observeTool() ");
      helpString.push_back("Finds tool in hand and observes it extracting features) ");
      helpString.push_back("@return true/false on success/failure finding and extracting feats from tool ");
    }
    if (functionName=="observeObj") {
      helpString.push_back("bool observeObj() ");
      helpString.push_back("Observes (extracts some features) of target object ");
      helpString.push_back("@return true/false on success/failure finding and extracting feats from object ");
    }
    if (functionName=="findTools") {
      helpString.push_back("int32_t findTools() ");
      helpString.push_back("Attempts to recognise a tool on the actual visual field ");
      helpString.push_back("@returns the number of potential tools found. ");
    }
    if (functionName=="selectTool") {
      helpString.push_back("bool selectTool() ");
      helpString.push_back("Selects one tool among many ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="quit") {
      helpString.push_back("bool quit() ");
      helpString.push_back("Quit the module ");
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


