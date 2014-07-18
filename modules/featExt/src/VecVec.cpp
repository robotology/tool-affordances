// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <VecVec.h>

bool VecVec::read(yarp::os::idl::WireReader& reader) {
  {
    content.clear();
    uint32_t _size6;
    yarp::os::idl::WireState _etype9;
    reader.readListBegin(_etype9, _size6);
    content.resize(_size6);
    uint32_t _i10;
    for (_i10 = 0; _i10 < _size6; ++_i10)
    {
      if (!reader.readNested(content[_i10])) {
        reader.fail();
        return false;
      }
    }
    reader.readListEnd();
  }
  return !reader.isError();
}

bool VecVec::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListHeader(1)) return false;
  return read(reader);
}

bool VecVec::write(yarp::os::idl::WireWriter& writer) {
  {
    if (!writer.writeListBegin(BOTTLE_TAG_LIST, static_cast<uint32_t>(content.size()))) return false;
    std::vector<Feature> ::iterator _iter11;
    for (_iter11 = content.begin(); _iter11 != content.end(); ++_iter11)
    {
      if (!writer.writeNested((*_iter11))) return false;
    }
    if (!writer.writeListEnd()) return false;
  }
  return !writer.isError();
}

bool VecVec::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  return write(writer);
}
