// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <blobSpotter_IDLServer.h>
#include <yarp/os/idl/WireTypes.h>



class blobSpotter_IDLServer_newHist : public yarp::os::Portable {
public:
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("newHist",1,1)) return false;
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

class blobSpotter_IDLServer_getCurHist : public yarp::os::Portable {
public:
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("getCurHist",1,1)) return false;
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

class blobSpotter_IDLServer_getTotHist : public yarp::os::Portable {
public:
  int32_t _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("getTotHist",1,1)) return false;
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

class blobSpotter_IDLServer_setHist : public yarp::os::Portable {
public:
  int32_t index;
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("setHist",1,1)) return false;
    if (!writer.writeI32(index)) return false;
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

class blobSpotter_IDLServer_reset : public yarp::os::Portable {
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

class blobSpotter_IDLServer_quit : public yarp::os::Portable {
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

int32_t blobSpotter_IDLServer::newHist() {
  int32_t _return = 0;
  blobSpotter_IDLServer_newHist helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t blobSpotter_IDLServer::newHist()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t blobSpotter_IDLServer::getCurHist() {
  int32_t _return = 0;
  blobSpotter_IDLServer_getCurHist helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t blobSpotter_IDLServer::getCurHist()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
int32_t blobSpotter_IDLServer::getTotHist() {
  int32_t _return = 0;
  blobSpotter_IDLServer_getTotHist helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","int32_t blobSpotter_IDLServer::getTotHist()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool blobSpotter_IDLServer::setHist(const int32_t index) {
  bool _return = false;
  blobSpotter_IDLServer_setHist helper;
  helper.index = index;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool blobSpotter_IDLServer::setHist(const int32_t index)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool blobSpotter_IDLServer::reset() {
  bool _return = false;
  blobSpotter_IDLServer_reset helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool blobSpotter_IDLServer::reset()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool blobSpotter_IDLServer::quit() {
  bool _return = false;
  blobSpotter_IDLServer_quit helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool blobSpotter_IDLServer::quit()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool blobSpotter_IDLServer::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  reader.expectAccept();
  if (!reader.readListHeader()) { reader.fail(); return false; }
  yarp::os::ConstString tag = reader.readTag();
  while (!reader.isError()) {
    // TODO: use quick lookup, this is just a test
    if (tag == "newHist") {
      int32_t _return;
      _return = newHist();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getCurHist") {
      int32_t _return;
      _return = getCurHist();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "getTotHist") {
      int32_t _return;
      _return = getTotHist();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeI32(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "setHist") {
      int32_t index;
      if (!reader.readI32(index)) {
        reader.fail();
        return false;
      }
      bool _return;
      _return = setHist(index);
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

std::vector<std::string> blobSpotter_IDLServer::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("newHist");
    helpString.push_back("getCurHist");
    helpString.push_back("getTotHist");
    helpString.push_back("setHist");
    helpString.push_back("reset");
    helpString.push_back("quit");
    helpString.push_back("help");
  }
  else {
    if (functionName=="newHist") {
      helpString.push_back("int32_t newHist() ");
      helpString.push_back("Initializes a histogram. ");
      helpString.push_back("This enables the input from the roi port, ");
      helpString.push_back("then waits for the user to select region ");
      helpString.push_back("on image. ");
      helpString.push_back("@return i32 of histogram index ");
    }
    if (functionName=="getCurHist") {
      helpString.push_back("int32_t getCurHist() ");
      helpString.push_back("Gets the current index of histograms ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="getTotHist") {
      helpString.push_back("int32_t getTotHist() ");
      helpString.push_back("Gets the total number of available histograms ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="setHist") {
      helpString.push_back("bool setHist(const int32_t index) ");
      helpString.push_back("Sets which histogram to use ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="reset") {
      helpString.push_back("bool reset() ");
      helpString.push_back(" Resets all the histograms ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="quit") {
      helpString.push_back("bool quit() ");
      helpString.push_back("Quit the module. ");
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


