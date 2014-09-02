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

class affManager_IDLServer_goHomeNoHands : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("goHomeNoHands",1,1)) return false;
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

class affManager_IDLServer_findToolDims : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("findToolDims",1,1)) return false;
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

class affManager_IDLServer_attachTool : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("attachTool",1,1)) return false;
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

class affManager_IDLServer_trackObj : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("trackObj",1,1)) return false;
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

class affManager_IDLServer_locateObj : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("locateObj",1,1)) return false;
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

class affManager_IDLServer_slideAction : public yarp::os::Portable {
public:
  int32_t approach;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("slideAction",1,1)) return false;
    if (!writer.writeI32(approach)) return false;
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

class affManager_IDLServer_computeEffect : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("computeEffect",1,1)) return false;
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

class affManager_IDLServer_reset : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("reset",1,1)) return false;
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

class affManager_IDLServer_getTool : public yarp::os::Portable {
public:
  int32_t tool;
  int32_t deg;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeTag("getTool",1,1)) return false;
    if (!writer.writeI32(tool)) return false;
    if (!writer.writeI32(deg)) return false;
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

class affManager_IDLServer_doAction : public yarp::os::Portable {
public:
  int32_t approach;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("doAction",1,1)) return false;
    if (!writer.writeI32(approach)) return false;
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

class affManager_IDLServer_trainDraw : public yarp::os::Portable {
public:
  int32_t tool;
  int32_t deg;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeTag("trainDraw",1,1)) return false;
    if (!writer.writeI32(tool)) return false;
    if (!writer.writeI32(deg)) return false;
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

class affManager_IDLServer_trainObserve : public yarp::os::Portable {
public:
  int32_t tool;
  int32_t deg;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeTag("trainObserve",1,1)) return false;
    if (!writer.writeI32(tool)) return false;
    if (!writer.writeI32(deg)) return false;
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

class affManager_IDLServer_observeAndDo : public yarp::os::Portable {
public:
  int32_t tool;
  int32_t deg;
  int32_t trials;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(4)) return false;
    if (!writer.writeTag("observeAndDo",1,1)) return false;
    if (!writer.writeI32(tool)) return false;
    if (!writer.writeI32(deg)) return false;
    if (!writer.writeI32(trials)) return false;
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

class affManager_IDLServer_runExp : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("runExp",1,1)) return false;
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
bool affManager_IDLServer::quit() {
  bool _return = false;
  affManager_IDLServer_quit helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::quit()");
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
bool affManager_IDLServer::goHomeNoHands() {
  bool _return = false;
  affManager_IDLServer_goHomeNoHands helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::goHomeNoHands()");
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
bool affManager_IDLServer::graspTool() {
  bool _return = false;
  affManager_IDLServer_graspTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::graspTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::findToolDims() {
  bool _return = false;
  affManager_IDLServer_findToolDims helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::findToolDims()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::attachTool() {
  bool _return = false;
  affManager_IDLServer_attachTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::attachTool()");
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
bool affManager_IDLServer::observeTool() {
  bool _return = false;
  affManager_IDLServer_observeTool helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::observeTool()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::trackObj() {
  bool _return = false;
  affManager_IDLServer_trackObj helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::trackObj()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::locateObj() {
  bool _return = false;
  affManager_IDLServer_locateObj helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::locateObj()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::slideAction(const int32_t approach) {
  bool _return = false;
  affManager_IDLServer_slideAction helper;
  helper.approach = approach;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::slideAction(const int32_t approach)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::computeEffect() {
  bool _return = false;
  affManager_IDLServer_computeEffect helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::computeEffect()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::reset() {
  bool _return = false;
  affManager_IDLServer_reset helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::reset()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::getTool(const int32_t tool, const int32_t deg) {
  bool _return = false;
  affManager_IDLServer_getTool helper;
  helper.tool = tool;
  helper.deg = deg;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::getTool(const int32_t tool, const int32_t deg)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::doAction(const int32_t approach) {
  bool _return = false;
  affManager_IDLServer_doAction helper;
  helper.approach = approach;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::doAction(const int32_t approach)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::trainDraw(const int32_t tool, const int32_t deg) {
  bool _return = false;
  affManager_IDLServer_trainDraw helper;
  helper.tool = tool;
  helper.deg = deg;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::trainDraw(const int32_t tool, const int32_t deg)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::trainObserve(const int32_t tool, const int32_t deg) {
  bool _return = false;
  affManager_IDLServer_trainObserve helper;
  helper.tool = tool;
  helper.deg = deg;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::trainObserve(const int32_t tool, const int32_t deg)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::observeAndDo(const int32_t tool, const int32_t deg, const int32_t trials) {
  bool _return = false;
  affManager_IDLServer_observeAndDo helper;
  helper.tool = tool;
  helper.deg = deg;
  helper.trials = trials;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::observeAndDo(const int32_t tool, const int32_t deg, const int32_t trials)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool affManager_IDLServer::runExp() {
  bool _return = false;
  affManager_IDLServer_runExp helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool affManager_IDLServer::runExp()");
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
    if (tag == "goHomeNoHands") {
      bool _return;
      _return = goHomeNoHands();
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
    if (tag == "findToolDims") {
      bool _return;
      _return = findToolDims();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "attachTool") {
      bool _return;
      _return = attachTool();
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
    if (tag == "locateObj") {
      bool _return;
      _return = locateObj();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "slideAction") {
      int32_t approach;
      if (!reader.readI32(approach)) {
        approach = 0;
      }
      bool _return;
      _return = slideAction(approach);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "computeEffect") {
      bool _return;
      _return = computeEffect();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "reset") {
      bool _return;
      _return = reset();
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
      if (!reader.readI32(tool)) {
        tool = 5;
      }
      if (!reader.readI32(deg)) {
        deg = 0;
      }
      bool _return;
      _return = getTool(tool,deg);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "doAction") {
      int32_t approach;
      if (!reader.readI32(approach)) {
        approach = 0;
      }
      bool _return;
      _return = doAction(approach);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "trainDraw") {
      int32_t tool;
      int32_t deg;
      if (!reader.readI32(tool)) {
        tool = 5;
      }
      if (!reader.readI32(deg)) {
        deg = 0;
      }
      bool _return;
      _return = trainDraw(tool,deg);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "trainObserve") {
      int32_t tool;
      int32_t deg;
      if (!reader.readI32(tool)) {
        tool = 5;
      }
      if (!reader.readI32(deg)) {
        deg = 0;
      }
      bool _return;
      _return = trainObserve(tool,deg);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "observeAndDo") {
      int32_t tool;
      int32_t deg;
      int32_t trials;
      if (!reader.readI32(tool)) {
        tool = 5;
      }
      if (!reader.readI32(deg)) {
        deg = 0;
      }
      if (!reader.readI32(trials)) {
        trials = 1;
      }
      bool _return;
      _return = observeAndDo(tool,deg,trials);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "runExp") {
      bool _return;
      _return = runExp();
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
    helpString.push_back("quit");
    helpString.push_back("goHome");
    helpString.push_back("goHomeNoHands");
    helpString.push_back("askForTool");
    helpString.push_back("graspTool");
    helpString.push_back("findToolDims");
    helpString.push_back("attachTool");
    helpString.push_back("lookAtTool");
    helpString.push_back("observeTool");
    helpString.push_back("trackObj");
    helpString.push_back("locateObj");
    helpString.push_back("slideAction");
    helpString.push_back("computeEffect");
    helpString.push_back("reset");
    helpString.push_back("getTool");
    helpString.push_back("doAction");
    helpString.push_back("trainDraw");
    helpString.push_back("trainObserve");
    helpString.push_back("observeAndDo");
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
      helpString.push_back("bool goHome() ");
      helpString.push_back("Adopt home position ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="goHomeNoHands") {
      helpString.push_back("bool goHomeNoHands() ");
      helpString.push_back("Adopt home position while keeping hand pose ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="askForTool") {
      helpString.push_back("bool askForTool() ");
      helpString.push_back("Asks for tool and move the arm to receiving position ");
      helpString.push_back("@return true/false on success/failure on going to receive position ");
    }
    if (functionName=="graspTool") {
      helpString.push_back("bool graspTool() ");
      helpString.push_back("Closes hand on tool previously reached or received. ");
      helpString.push_back("@return true/false on success/failure on holding the tool. ");
    }
    if (functionName=="findToolDims") {
      helpString.push_back("bool findToolDims() ");
      helpString.push_back("Uses active exploration and non-linear optimization to compute the tool dimensions ");
      helpString.push_back("Makes use of KarmaMotor, KarmaToolProjection and KarmaToolFinder ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="attachTool") {
      helpString.push_back("bool attachTool() ");
      helpString.push_back("Attaches its end-effector relative position (provided by findToolDims or else wise) to the robot's kinematic chain. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="lookAtTool") {
      helpString.push_back("bool lookAtTool() ");
      helpString.push_back("Moves the tool in hand to a position where it can be observed fully, i.e., in front of iCub eyes ");
      helpString.push_back("@return true/false on success/failure of bringing the tool in front ");
    }
    if (functionName=="observeTool") {
      helpString.push_back("bool observeTool() ");
      helpString.push_back("Finds tool in hand and observes it extracting features ");
      helpString.push_back("@return true/false on success/failure finding and extracting feats from tool ");
    }
    if (functionName=="trackObj") {
      helpString.push_back("bool trackObj() ");
      helpString.push_back("Gets position of the object from the user, and uses the template to train the particle filter tracker. ");
      helpString.push_back("@return true/false on success/failure of finding/looking at object ");
    }
    if (functionName=="locateObj") {
      helpString.push_back("bool locateObj() ");
      helpString.push_back("Look and locate in 3D the target object ");
      helpString.push_back("@return true/false on success/failure of finding/looking at object ");
    }
    if (functionName=="slideAction") {
      helpString.push_back("bool slideAction(const int32_t approach = 0) ");
      helpString.push_back("Executes a sliding action using the end-effector (tool or hand) ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="computeEffect") {
      helpString.push_back("bool computeEffect() ");
      helpString.push_back("Computes the effect of the action as the difference in the position of the object before and after the slide action. ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="reset") {
      helpString.push_back("bool reset() ");
      helpString.push_back("Sets the experiment flow flags to false (action done, object located, tip on view). ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="getTool") {
      helpString.push_back("bool getTool(const int32_t tool = 5, const int32_t deg = 0) ");
      helpString.push_back("performs the sequence to get the tool from user, look at it and extract its features. ");
      helpString.push_back("@return true/false on success/failure of looking at that position ");
    }
    if (functionName=="doAction") {
      helpString.push_back("bool doAction(const int32_t approach = 0) ");
      helpString.push_back("Executes the sequence to clear the visual field, look at the object, perform the action and observe the effect. ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="trainDraw") {
      helpString.push_back("bool trainDraw(const int32_t tool = 5, const int32_t deg = 0) ");
      helpString.push_back("Performs the drawing action a given number of times to learn the mapping ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="trainObserve") {
      helpString.push_back("bool trainObserve(const int32_t tool = 5, const int32_t deg = 0) ");
      helpString.push_back("Performs the feature Extraction on the tool a given number of times from slighlty different prespectives ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="observeAndDo") {
      helpString.push_back("bool observeAndDo(const int32_t tool = 5, const int32_t deg = 0, const int32_t trials = 1) ");
      helpString.push_back("Performs once the whole routine of looking at the tool getting its features ad then performing an action, getting also parameters and effect of the action ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
    }
    if (functionName=="runExp") {
      helpString.push_back("bool runExp() ");
      helpString.push_back("Performs ObserveAndDo for all the possible tools, each in the 3 poses ");
      helpString.push_back("@return true/false on success/failure ");
      helpString.push_back("to select ");
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


