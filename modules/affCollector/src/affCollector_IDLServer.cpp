// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <affCollector_IDLServer.h>
#include <yarp/os/idl/WireTypes.h>



class affCollector_IDLServer_start : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_quit : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_setlabel : public yarp::os::Portable {
public:
  std::string label;
  int32_t _return;
  void init(const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_getlabel : public yarp::os::Portable {
public:
  std::string _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_updateAff : public yarp::os::Portable {
public:
  int32_t act;
  double eff;
  int32_t labI;
  double _return;
  void init(const int32_t act, const double eff, const int32_t labI);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_getAffs : public yarp::os::Portable {
public:
  std::string label;
  std::vector<double>  _return;
  void init(const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_getAffHist : public yarp::os::Portable {
public:
  int32_t act;
  std::string label;
  std::vector<double>  _return;
  void init(const int32_t act, const std::string& label);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_selectTool : public yarp::os::Portable {
public:
  int32_t action;
  std::string _return;
  void init(const int32_t action);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_clear : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_clearAll : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class affCollector_IDLServer_verbose : public yarp::os::Portable {
public:
  bool verb;
  bool _return;
  void init(const bool verb);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
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
  {
    _return.clear();
    uint32_t _size0;
    yarp::os::idl::WireState _etype3;
    reader.readListBegin(_etype3, _size0);
    _return.resize(_size0);
    uint32_t _i4;
    for (_i4 = 0; _i4 < _size0; ++_i4)
    {
      if (!reader.readDouble(_return[_i4])) {
        reader.fail();
        return false;
      }
    }
    reader.readListEnd();
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
  if (!writer.writeI32(act)) return false;
  if (!writer.writeString(label)) return false;
  return true;
}

bool affCollector_IDLServer_getAffHist::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  {
    _return.clear();
    uint32_t _size5;
    yarp::os::idl::WireState _etype8;
    reader.readListBegin(_etype8, _size5);
    _return.resize(_size5);
    uint32_t _i9;
    for (_i9 = 0; _i9 < _size5; ++_i9)
    {
      if (!reader.readDouble(_return[_i9])) {
        reader.fail();
        return false;
      }
    }
    reader.readListEnd();
  }
  return true;
}

void affCollector_IDLServer_getAffHist::init(const int32_t act, const std::string& label) {
  this->act = act;
  this->label = label;
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

bool affCollector_IDLServer_clear::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("clear",1,1)) return false;
  return true;
}

bool affCollector_IDLServer_clear::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void affCollector_IDLServer_clear::init() {
  _return = false;
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
std::vector<double>  affCollector_IDLServer::getAffs(const std::string& label) {
  std::vector<double>  _return;
  affCollector_IDLServer_getAffs helper;
  helper.init(label);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::vector<double>  affCollector_IDLServer::getAffs(const std::string& label)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::vector<double>  affCollector_IDLServer::getAffHist(const int32_t act, const std::string& label) {
  std::vector<double>  _return;
  affCollector_IDLServer_getAffHist helper;
  helper.init(act,label);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::vector<double>  affCollector_IDLServer::getAffHist(const int32_t act, const std::string& label)");
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
bool affCollector_IDLServer::clear() {
  bool _return = false;
  affCollector_IDLServer_clear helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool affCollector_IDLServer::clear()");
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
      std::vector<double>  _return;
      _return = getAffs(label);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        {
          if (!writer.writeListBegin(BOTTLE_TAG_DOUBLE, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<double> ::iterator _iter10;
          for (_iter10 = _return.begin(); _iter10 != _return.end(); ++_iter10)
          {
            if (!writer.writeDouble((*_iter10))) return false;
          }
          if (!writer.writeListEnd()) return false;
        }
      }
      reader.accept();
      return true;
    }
    if (tag == "getAffHist") {
      int32_t act;
      std::string label;
      if (!reader.readI32(act)) {
        reader.fail();
        return false;
      }
      if (!reader.readString(label)) {
        label = "active";
      }
      std::vector<double>  _return;
      _return = getAffHist(act,label);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        {
          if (!writer.writeListBegin(BOTTLE_TAG_DOUBLE, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<double> ::iterator _iter11;
          for (_iter11 = _return.begin(); _iter11 != _return.end(); ++_iter11)
          {
            if (!writer.writeDouble((*_iter11))) return false;
          }
          if (!writer.writeListEnd()) return false;
        }
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
    if (tag == "clear") {
      bool _return;
      _return = clear();
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
    helpString.push_back("setlabel");
    helpString.push_back("getlabel");
    helpString.push_back("updateAff");
    helpString.push_back("getAffs");
    helpString.push_back("getAffHist");
    helpString.push_back("selectTool");
    helpString.push_back("clear");
    helpString.push_back("clearAll");
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
      helpString.push_back("std::vector<double>  getAffs(const std::string& label = \"active\") ");
      helpString.push_back("Returns the success rate for all actions in the repertoire for a given known label (the active one by default). ");
      helpString.push_back("if label == 'all', returns a concatenated vector with all konwn affordances. ");
    }
    if (functionName=="getAffHist") {
      helpString.push_back("std::vector<double>  getAffHist(const int32_t act, const std::string& label = \"active\") ");
      helpString.push_back("Returns the history of effects for a given action and known label (the active one by default). ");
    }
    if (functionName=="selectTool") {
      helpString.push_back("std::string selectTool(const int32_t action) ");
      helpString.push_back(" Based on the previously learnt affordances, returns the best label for a given action/task. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="clear") {
      helpString.push_back("bool clear() ");
      helpString.push_back("Clears all the learnt affordances of the active label, and sets it to unknown. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="clearAll") {
      helpString.push_back("bool clearAll() ");
      helpString.push_back("Clears all the learnt affordances all labels, and sets them to unknown. ");
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


