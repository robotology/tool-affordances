// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <info3D.h>

bool info3D::read(yarp::os::idl::WireReader& reader) {
  if (!reader.readBool(ACK)) {
    reader.fail();
    return false;
  }
  if (!reader.readString(msg)) {
    reader.fail();
    return false;
  }
  {
    content.clear();
    uint32_t _size0;
    yarp::os::idl::WireState _etype3;
    reader.readListBegin(_etype3, _size0);
    content.resize(_size0);
    uint32_t _i4;
    for (_i4 = 0; _i4 < _size0; ++_i4)
    {
      if (!reader.readDouble(content[_i4])) {
        reader.fail();
        return false;
      }
    }
    reader.readListEnd();
  }
  return !reader.isError();
}

bool info3D::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListHeader(3)) return false;
  return read(reader);
}

bool info3D::write(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeBool(ACK)) return false;
  if (!writer.writeString(msg)) return false;
  {
    if (!writer.writeListBegin(BOTTLE_TAG_DOUBLE, static_cast<uint32_t>(content.size()))) return false;
    std::vector<double> ::iterator _iter5;
    for (_iter5 = content.begin(); _iter5 != content.end(); ++_iter5)
    {
      if (!writer.writeDouble((*_iter5))) return false;
    }
    if (!writer.writeListEnd()) return false;
  }
  return !writer.isError();
}

bool info3D::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(3)) return false;
  return write(writer);
}
