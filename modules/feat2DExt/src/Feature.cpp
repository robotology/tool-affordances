// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <Feature.h>

bool Feature::read_name(yarp::os::idl::WireReader& reader) {
  if (!reader.readString(name)) {
    reader.fail();
    return false;
  }
  return true;
}
bool Feature::nested_read_name(yarp::os::idl::WireReader& reader) {
  if (!reader.readString(name)) {
    reader.fail();
    return false;
  }
  return true;
}
bool Feature::read_content(yarp::os::idl::WireReader& reader) {
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
  return true;
}
bool Feature::nested_read_content(yarp::os::idl::WireReader& reader) {
  {
    content.clear();
    uint32_t _size5;
    yarp::os::idl::WireState _etype8;
    reader.readListBegin(_etype8, _size5);
    content.resize(_size5);
    uint32_t _i9;
    for (_i9 = 0; _i9 < _size5; ++_i9)
    {
      if (!reader.readDouble(content[_i9])) {
        reader.fail();
        return false;
      }
    }
    reader.readListEnd();
  }
  return true;
}
bool Feature::read(yarp::os::idl::WireReader& reader) {
  if (!read_name(reader)) return false;
  if (!read_content(reader)) return false;
  return !reader.isError();
}

bool Feature::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListHeader(2)) return false;
  return read(reader);
}

bool Feature::write_name(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeString(name)) return false;
  return true;
}
bool Feature::nested_write_name(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeString(name)) return false;
  return true;
}
bool Feature::write_content(yarp::os::idl::WireWriter& writer) {
  {
    if (!writer.writeListBegin(BOTTLE_TAG_DOUBLE, static_cast<uint32_t>(content.size()))) return false;
    std::vector<double> ::iterator _iter10;
    for (_iter10 = content.begin(); _iter10 != content.end(); ++_iter10)
    {
      if (!writer.writeDouble((*_iter10))) return false;
    }
    if (!writer.writeListEnd()) return false;
  }
  return true;
}
bool Feature::nested_write_content(yarp::os::idl::WireWriter& writer) {
  {
    if (!writer.writeListBegin(BOTTLE_TAG_DOUBLE, static_cast<uint32_t>(content.size()))) return false;
    std::vector<double> ::iterator _iter11;
    for (_iter11 = content.begin(); _iter11 != content.end(); ++_iter11)
    {
      if (!writer.writeDouble((*_iter11))) return false;
    }
    if (!writer.writeListEnd()) return false;
  }
  return true;
}
bool Feature::write(yarp::os::idl::WireWriter& writer) {
  if (!write_name(writer)) return false;
  if (!write_content(writer)) return false;
  return !writer.isError();
}

bool Feature::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(2)) return false;
  return write(writer);
}
bool Feature::Editor::write(yarp::os::ConnectionWriter& connection) {
  if (!isValid()) return false;
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(dirty_count+1)) return false;
  if (!writer.writeString("patch")) return false;
  if (is_dirty_name) {
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeString("set")) return false;
    if (!writer.writeString("name")) return false;
    if (!obj->nested_write_name(writer)) return false;
  }
  if (is_dirty_content) {
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeString("set")) return false;
    if (!writer.writeString("content")) return false;
    if (!obj->nested_write_content(writer)) return false;
  }
  return !writer.isError();
}
bool Feature::Editor::read(yarp::os::ConnectionReader& connection) {
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
      if (field=="name") {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString("std::string name")) return false;
      }
      if (field=="content") {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString("std::vector<double>  content")) return false;
      }
    }
    if (!writer.writeListHeader(3)) return false;
    writer.writeString("*** Available fields:");
    writer.writeString("name");
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
    if (key == "name") {
      will_set_name();
      if (!obj->nested_read_name(reader)) return false;
      did_set_name();
    } else if (key == "content") {
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

yarp::os::ConstString Feature::toString() {
  yarp::os::Bottle b;
  b.read(*this);
  return b.toString();
}
