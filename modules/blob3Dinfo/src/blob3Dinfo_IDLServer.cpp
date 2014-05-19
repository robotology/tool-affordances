// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <blob3Dinfo_IDLServer.h>
#include <yarp/os/idl/WireTypes.h>



class blob3Dinfo_IDLServer_start : public yarp::os::Portable {
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

class blob3Dinfo_IDLServer_stop : public yarp::os::Portable {
public:
  bool _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("stop",1,1)) return false;
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

class blob3Dinfo_IDLServer_blobsInRange : public yarp::os::Portable {
public:
  double range;
  std::vector<int32_t>  _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("blobsInRange",1,1)) return false;
    if (!writer.writeDouble(range)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    {
      _return.clear();
      uint32_t _size6;
      yarp::os::idl::WireState _etype9;
      reader.readListBegin(_etype9, _size6);
      _return.resize(_size6);
      uint32_t _i10;
      for (_i10 = 0; _i10 < _size6; ++_i10)
      {
        if (!reader.readI32(_return[_i10])) {
          reader.fail();
          return false;
        }
      }
      reader.readListEnd();
    }
    return true;
  }
};

class blob3Dinfo_IDLServer_get3Dinfo : public yarp::os::Portable {
public:
  info3D _return;
  virtual bool write(yarp::os::ConnectionWriter& connection) {
    yarp::os::idl::WireWriter writer(connection);
    if (!writer.writeListHeader(1)) return false;
    if (!writer.writeTag("get3Dinfo",1,1)) return false;
    return true;
  }
  virtual bool read(yarp::os::ConnectionReader& connection) {
    yarp::os::idl::WireReader reader(connection);
    if (!reader.readListReturn()) return false;
    if (!reader.read(_return)) {
      reader.fail();
      return false;
    }
    return true;
  }
};

bool blob3Dinfo_IDLServer::start() {
  bool _return = false;
  blob3Dinfo_IDLServer_start helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool blob3Dinfo_IDLServer::start()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
bool blob3Dinfo_IDLServer::stop() {
  bool _return = false;
  blob3Dinfo_IDLServer_stop helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","bool blob3Dinfo_IDLServer::stop()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
std::vector<int32_t>  blob3Dinfo_IDLServer::blobsInRange(const double range) {
  std::vector<int32_t>  _return;
  blob3Dinfo_IDLServer_blobsInRange helper;
  helper.range = range;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","std::vector<int32_t>  blob3Dinfo_IDLServer::blobsInRange(const double range)");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}
info3D blob3Dinfo_IDLServer::get3Dinfo() {
  info3D _return;
  blob3Dinfo_IDLServer_get3Dinfo helper;
  if (!yarp().canWrite()) {
    fprintf(stderr,"Missing server method '%s'?\n","info3D blob3Dinfo_IDLServer::get3Dinfo()");
  }
  bool ok = yarp().write(helper,helper);
  return ok?helper._return:_return;
}

bool blob3Dinfo_IDLServer::read(yarp::os::ConnectionReader& connection) {
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
    if (tag == "stop") {
      bool _return;
      _return = stop();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeBool(_return)) return false;
      }
      reader.accept();
      return true;
    }
    if (tag == "blobsInRange") {
      double range;
      if (!reader.readDouble(range)) {
        reader.fail();
        return false;
      }
      std::vector<int32_t>  _return;
      _return = blobsInRange(range);
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(1)) return false;
        {
          if (!writer.writeListBegin(BOTTLE_TAG_INT, static_cast<uint32_t>(_return.size()))) return false;
          std::vector<int32_t> ::iterator _iter11;
          for (_iter11 = _return.begin(); _iter11 != _return.end(); ++_iter11)
          {
            if (!writer.writeI32((*_iter11))) return false;
          }
          if (!writer.writeListEnd()) return false;
        }
      }
      reader.accept();
      return true;
    }
    if (tag == "get3Dinfo") {
      info3D _return;
      _return = get3Dinfo();
      yarp::os::idl::WireWriter writer(reader);
      if (!writer.isNull()) {
        if (!writer.writeListHeader(3)) return false;
        if (!writer.write(_return)) return false;
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

std::vector<std::string> blob3Dinfo_IDLServer::help(const std::string& functionName) {
  bool showAll=(functionName=="--all");
  std::vector<std::string> helpString;
  if(showAll) {
    helpString.push_back("*** Available commands:");
    helpString.push_back("start");
    helpString.push_back("stop");
    helpString.push_back("blobsInRange");
    helpString.push_back("get3Dinfo");
    helpString.push_back("help");
  }
  else {
    if (functionName=="start") {
      helpString.push_back("bool start() ");
      helpString.push_back("Start the module ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="stop") {
      helpString.push_back("bool stop() ");
      helpString.push_back("Stop the module. ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="blobsInRange") {
      helpString.push_back("std::vector<int32_t>  blobsInRange(const double range) ");
      helpString.push_back("Compute once the 3D position ");
      helpString.push_back("@return true/false on success/failure ");
    }
    if (functionName=="get3Dinfo") {
      helpString.push_back("info3D get3Dinfo() ");
      helpString.push_back("Compute once the 3D position ");
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


