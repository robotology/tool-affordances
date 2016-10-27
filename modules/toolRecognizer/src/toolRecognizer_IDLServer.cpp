// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <toolRecognizer_IDLServer.h>
#include <yarp/os/idl/WireTypes.h>



class toolRecognizer_IDLServer_start : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class toolRecognizer_IDLServer_quit : public yarp::os::Portable {
public:
  bool _return;
  void init();
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class toolRecognizer_IDLServer_train : public yarp::os::Portable {
public:
  std::string label;
  double tlx;
  double tly;
  double brx;
  double bry;
  bool _return;
  void init(const std::string& label, const double tlx, const double tly, const double brx, const double bry);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

class toolRecognizer_IDLServer_recognize : public yarp::os::Portable {
public:
  double tlx;
  double tly;
  double brx;
  double bry;
  std::string _return;
  void init(const double tlx, const double tly, const double brx, const double bry);
  virtual bool write(yarp::os::ConnectionWriter& connection);
  virtual bool read(yarp::os::ConnectionReader& connection);
};

bool toolRecognizer_IDLServer_start::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("start",1,1)) return false;
  return true;
}

bool toolRecognizer_IDLServer_start::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void toolRecognizer_IDLServer_start::init() {
  _return = false;
}

bool toolRecognizer_IDLServer_quit::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  if (!writer.writeTag("quit",1,1)) return false;
  return true;
}

bool toolRecognizer_IDLServer_quit::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void toolRecognizer_IDLServer_quit::init() {
  _return = false;
}

bool toolRecognizer_IDLServer_train::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(6)) return false;
  if (!writer.writeTag("train",1,1)) return false;
  if (!writer.writeString(label)) return false;
  if (!writer.writeDouble(tlx)) return false;
  if (!writer.writeDouble(tly)) return false;
  if (!writer.writeDouble(brx)) return false;
  if (!writer.writeDouble(bry)) return false;
  return true;
}

bool toolRecognizer_IDLServer_train::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readBool(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void toolRecognizer_IDLServer_train::init(const std::string& label, const double tlx, const double tly, const double brx, const double bry) {
  _return = false;
  this->label = label;
  this->tlx = tlx;
  this->tly = tly;
  this->brx = brx;
  this->bry = bry;
}

bool toolRecognizer_IDLServer_recognize::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(5)) return false;
  if (!writer.writeTag("recognize",1,1)) return false;
  if (!writer.writeDouble(tlx)) return false;
  if (!writer.writeDouble(tly)) return false;
  if (!writer.writeDouble(brx)) return false;
  if (!writer.writeDouble(bry)) return false;
  return true;
}

bool toolRecognizer_IDLServer_recognize::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListReturn()) return false;
  if (!reader.readString(_return)) {
    reader.fail();
    return false;
  }
  return true;
}

void toolRecognizer_IDLServer_recognize::init(const double tlx, const double tly, const double brx, const double bry) {
  _return = "";
  this->tlx = tlx;
  this->tly = tly;
  this->brx = brx;
  this->bry = bry;
}

toolRecognizer_IDLServer::toolRecognizer_IDLServer() {
  yarp().setOwner(*this);
}
bool toolRecognizer_IDLServer::start() {
  bool _return = false;
  toolRecognizer_IDLServer_start helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool toolRecognizer_IDLServer::start()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool toolRecognizer_IDLServer::quit() {
  bool _return = false;
  toolRecognizer_IDLServer_quit helper;
  helper.init();
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool toolRecognizer_IDLServer::quit()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool toolRecognizer_IDLServer::train(const std::string& label, const double tlx, const double tly, const double brx, const double bry) {
  bool _return = false;
  toolRecognizer_IDLServer_train helper;
  helper.init(label,tlx,tly,brx,bry);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","bool toolRecognizer_IDLServer::train(const std::string& label, const double tlx, const double tly, const double brx, const double bry)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::string toolRecognizer_IDLServer::recognize(const double tlx, const double tly, const double brx, const double bry) {
  std::string _return = "";
  toolRecognizer_IDLServer_recognize helper;
  helper.init(tlx,tly,brx,bry);
  if (!yarp().canWrite()) {
    yError("Missing server method '%s'?","std::string toolRecognizer_IDLServer::recognize(const double tlx, const double tly, const double brx, const double bry)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool toolRecognizer_IDLServer::read(yarp::os::ConnectionReader& connection) {
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
    if (tag == "train") {
      std::string label;
      double tlx;
      double tly;
      double brx;
      double bry;
      if (!reader.readString(label)) {
        reader.fail();
        return false;
      }
      if (!reader.readDouble(tlx)) {
        tlx = 0;
      }
      if (!reader.readDouble(tly)) {
        tly = 0;
      }
      if (!reader.readDouble(brx)) {
        brx = 0;
      }
      if (!reader.readDouble(bry)) {
        bry = 0;
      }
      bool _return;
      _return = train(label,tlx,tly,brx,bry);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "recognize") {
      double tlx;
      double tly;
      double brx;
      double bry;
      if (!reader.readDouble(tlx)) {
        tlx = 0;
      }
      if (!reader.readDouble(tly)) {
        tly = 0;
      }
      if (!reader.readDouble(brx)) {
        brx = 0;
      }
      if (!reader.readDouble(bry)) {
        bry = 0;
      }
      std::string _return;
      _return = recognize(tlx,tly,brx,bry);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString(_return)) return false;
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

std::vector<std::string> toolRecognizer_IDLServer::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("start");
    helpString.push_back("quit");
    helpString.push_back("train");
    helpString.push_back("recognize");
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
    if (functionName=="train") {
      helpString.push_back("bool train(const std::string& label, const double tlx = 0, const double tly = 0, const double brx = 0, const double bry = 0) ");
      helpString.push_back("Command to train tools by their label ");
      helpString.push_back("@return true/false on success/failure to train classifiers. ");
    }
    if (functionName=="recognize") {
      helpString.push_back("std::string recognize(const double tlx = 0, const double tly = 0, const double brx = 0, const double bry = 0) ");
      helpString.push_back("Checks whether the hand is full or empty ");
      helpString.push_back("@return true/false  corresponding to full or empty hand ");
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


