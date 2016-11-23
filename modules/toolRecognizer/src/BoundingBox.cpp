// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#include <BoundingBox.h>

bool BoundingBox::read_tlx(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(tlx)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::nested_read_tlx(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(tlx)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::read_tly(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(tly)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::nested_read_tly(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(tly)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::read_brx(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(brx)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::nested_read_brx(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(brx)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::read_bry(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(bry)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::nested_read_bry(yarp::os::idl::WireReader& reader) {
  if (!reader.readDouble(bry)) {
    reader.fail();
    return false;
  }
  return true;
}
bool BoundingBox::read(yarp::os::idl::WireReader& reader) {
  if (!read_tlx(reader)) return false;
  if (!read_tly(reader)) return false;
  if (!read_brx(reader)) return false;
  if (!read_bry(reader)) return false;
  return !reader.isError();
}

bool BoundingBox::read(yarp::os::ConnectionReader& connection) {
  yarp::os::idl::WireReader reader(connection);
  if (!reader.readListHeader(4)) return false;
  return read(reader);
}

bool BoundingBox::write_tlx(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(tlx)) return false;
  return true;
}
bool BoundingBox::nested_write_tlx(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(tlx)) return false;
  return true;
}
bool BoundingBox::write_tly(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(tly)) return false;
  return true;
}
bool BoundingBox::nested_write_tly(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(tly)) return false;
  return true;
}
bool BoundingBox::write_brx(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(brx)) return false;
  return true;
}
bool BoundingBox::nested_write_brx(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(brx)) return false;
  return true;
}
bool BoundingBox::write_bry(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(bry)) return false;
  return true;
}
bool BoundingBox::nested_write_bry(yarp::os::idl::WireWriter& writer) {
  if (!writer.writeDouble(bry)) return false;
  return true;
}
bool BoundingBox::write(yarp::os::idl::WireWriter& writer) {
  if (!write_tlx(writer)) return false;
  if (!write_tly(writer)) return false;
  if (!write_brx(writer)) return false;
  if (!write_bry(writer)) return false;
  return !writer.isError();
}

bool BoundingBox::write(yarp::os::ConnectionWriter& connection) {
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(4)) return false;
  return write(writer);
}
bool BoundingBox::Editor::write(yarp::os::ConnectionWriter& connection) {
  if (!isValid()) return false;
  yarp::os::idl::WireWriter writer(connection);
  if (!writer.writeListHeader(dirty_count+1)) return false;
  if (!writer.writeString("patch")) return false;
  if (is_dirty_tlx) {
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeString("set")) return false;
    if (!writer.writeString("tlx")) return false;
    if (!obj->nested_write_tlx(writer)) return false;
  }
  if (is_dirty_tly) {
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeString("set")) return false;
    if (!writer.writeString("tly")) return false;
    if (!obj->nested_write_tly(writer)) return false;
  }
  if (is_dirty_brx) {
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeString("set")) return false;
    if (!writer.writeString("brx")) return false;
    if (!obj->nested_write_brx(writer)) return false;
  }
  if (is_dirty_bry) {
    if (!writer.writeListHeader(3)) return false;
    if (!writer.writeString("set")) return false;
    if (!writer.writeString("bry")) return false;
    if (!obj->nested_write_bry(writer)) return false;
  }
  return !writer.isError();
}
bool BoundingBox::Editor::read(yarp::os::ConnectionReader& connection) {
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
      if (field=="tlx") {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString("double tlx")) return false;
      }
      if (field=="tly") {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString("double tly")) return false;
      }
      if (field=="brx") {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString("double brx")) return false;
      }
      if (field=="bry") {
        if (!writer.writeListHeader(1)) return false;
        if (!writer.writeString("double bry")) return false;
      }
    }
    if (!writer.writeListHeader(5)) return false;
    writer.writeString("*** Available fields:");
    writer.writeString("tlx");
    writer.writeString("tly");
    writer.writeString("brx");
    writer.writeString("bry");
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
    if (key == "tlx") {
      will_set_tlx();
      if (!obj->nested_read_tlx(reader)) return false;
      did_set_tlx();
    } else if (key == "tly") {
      will_set_tly();
      if (!obj->nested_read_tly(reader)) return false;
      did_set_tly();
    } else if (key == "brx") {
      will_set_brx();
      if (!obj->nested_read_brx(reader)) return false;
      did_set_brx();
    } else if (key == "bry") {
      will_set_bry();
      if (!obj->nested_read_bry(reader)) return false;
      did_set_bry();
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

yarp::os::ConstString BoundingBox::toString() {
  yarp::os::Bottle b;
  b.read(*this);
  return b.toString();
}
