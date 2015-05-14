// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <VecVec.h>

bool VecVec::read_content(yarp::os::idl::WireReader& reader) {
  {
    content.clear();
    uint32_t _size12;
    yarp::os::idl::WireState _etype15;
    reader.readListBegin(_etype15, _size12);
    content.resize(_size12);
    uint32_t _i16;
    for (_i16 = 0; _i16 < _size12; ++_i16)
    {
      if (!reader.readNested(content[_i16])) {
        reader.fail();
        return false;
      }
    }
    reader.readListEnd();
  }
  return true;
}
bool VecVec::nested_read_content(yarp::os::idl::WireReader& reader) {
  {
    content.clear();
    uint32_t _size17;
    yarp::os::idl::WireState _etype20;
    reader.readListBegin(_etype20, _size17);
    content.resize(_size17);
    uint32_t _i21;
    for (_i21 = 0; _i21 < _size17; ++_i21)
    {
      if (!reader.readNested(content[_i21])) {
        reader.fail();
        return false;
      }
    }
    reader.readListEnd();
  }
  return true;
}
bool VecVec::read(yarp::os::idl::WireReader& reader) {
  if (!read_content(reader)) return false;
  return !reader.isError();
}

bool VecVec::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListHeader(1)) return false;
  return read(reader);
}

bool VecVec::write_content(yarp::os::idl::WireWriter& writer) {
  {
    if (!writer.writeListBegin(BOTTLE_TAG_LIST, static_cast<uint32_t>(content.size()))) return false;
    std::vector<Feature> ::iterator _iter22;
    for (_iter22 = content.begin(); _iter22 != content.end(); ++_iter22)
    {
      if (!writer.writeNested((*_iter22))) return false;
    }
    if (!writer.writeListEnd()) return false;
  }
  return true;
}
bool VecVec::nested_write_content(yarp::os::idl::WireWriter& writer) {
  {
    if (!writer.writeListBegin(BOTTLE_TAG_LIST, static_cast<uint32_t>(content.size()))) return false;
    std::vector<Feature> ::iterator _iter23;
    for (_iter23 = content.begin(); _iter23 != content.end(); ++_iter23)
    {
      if (!writer.writeNested((*_iter23))) return false;
    }
    if (!writer.writeListEnd()) return false;
  }
  return true;
}
bool VecVec::write(yarp::os::idl::WireWriter& writer) {
  if (!write_content(writer)) return false;
  return !writer.isError();
}

bool VecVec::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(1)) return false;
  return write(writer);
}
bool VecVec::Editor::write(yarp::os::ConnectionWriter& connection) {
  if (!isValid()) return false;
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(dirty_count+1)) return false;
  if (!writer.writeString("patch")) return false;
  if (is_dirty_content) {
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeString("set")) return false;
    if (!writer.writeString("content")) return false;
    if (!obj->nested_write_content(writer)) return false;
  }
  return !writer.isError();
}
bool VecVec::Editor::read(yarp::os::ConnectionReader& connection) {
  if (!isValid()) return false;
  yarp::os::idl::WireReader reader(connection);
  reader.expectAccept();
  if (!reader.readListHeader()) return false;
  int len = reader.getLength();
  if (len==0) {
    yarp::os::idl::WireWriter writer(reader);
    if (writer.isNull()) return true;
    if (!writer.writeListHeader(1)) return false;
    writer.writeString("send: 'help' or 'patch (param1 val1) (param2 val2)'");
    return true;
  }
  yarp::os::ConstString tag;
  if (!reader.readString(tag)) return false;
  if (tag=="help") {
    yarp::os::idl::WireWriter writer(reader);
    if (writer.isNull()) return true;
    if (!writer.writeListHeader(2)) return false;
    if (!writer.writeTag("many",1, 0)) return false;
    if (reader.getLength()>0) {
      yarp::os::ConstString field;
      if (!reader.readString(field)) return false;
      if (field=="content") {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString("std::vector<Feature>  content")) return false;
      }
    }
    if (!writer.writeListHeader(2)) return false;
    writer.writeString("*** Available fields:");
    writer.writeString("content");
    return true;
  }
  bool nested = true;
  bool have_act = false;
  if (tag!="patch") {
    if ((len-1)%2 != 0) return false;
    len = 1 + ((len-1)/2);
    nested = false;
    have_act = true;
  }
  for (int i=1; i<len; i++) {
    if (nested && !reader.readListHeader(3)) return false;
    yarp::os::ConstString act;
    yarp::os::ConstString key;
    if (have_act) {
      act = tag;
    } else {
      if (!reader.readString(act)) return false;
    }
    if (!reader.readString(key)) return false;
    // inefficient code follows, bug paulfitz to improve it
    if (key == "content") {
      will_set_content();
      if (!obj->nested_read_content(reader)) return false;
      did_set_content();
    } else {
      // would be useful to have a fallback here
    }
  }
  reader.accept();
  yarp::os::idl::WireWriter writer(reader);
  if (writer.isNull()) return true;
  writer.writeListHeader(1);
  writer.writeVocab(VOCAB2('o','k'));
  return true;
}

yarp::os::ConstString VecVec::toString() {
  yarp::os::Bottle b;
  b.read(*this);
  return b.toString();
}
