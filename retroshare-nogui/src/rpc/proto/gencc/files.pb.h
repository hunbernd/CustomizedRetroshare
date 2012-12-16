// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: files.proto

#ifndef PROTOBUF_files_2eproto__INCLUDED
#define PROTOBUF_files_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2004000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2004001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#include "core.pb.h"
// @@protoc_insertion_point(includes)

namespace rsctrl {
namespace files {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_files_2eproto();
void protobuf_AssignDesc_files_2eproto();
void protobuf_ShutdownFile_files_2eproto();

class FileTransfer;
class RequestTransferList;
class ResponseTransferList;
class RequestControlDownload;
class ResponseControlDownload;

enum RequestControlDownload_Action {
  RequestControlDownload_Action_ACTION_START = 1,
  RequestControlDownload_Action_ACTION_CONTINUE = 2,
  RequestControlDownload_Action_ACTION_WAIT = 3,
  RequestControlDownload_Action_ACTION_PAUSE = 4,
  RequestControlDownload_Action_ACTION_RESTART = 5,
  RequestControlDownload_Action_ACTION_CHECK = 6,
  RequestControlDownload_Action_ACTION_CANCEL = 7
};
bool RequestControlDownload_Action_IsValid(int value);
const RequestControlDownload_Action RequestControlDownload_Action_Action_MIN = RequestControlDownload_Action_ACTION_START;
const RequestControlDownload_Action RequestControlDownload_Action_Action_MAX = RequestControlDownload_Action_ACTION_CANCEL;
const int RequestControlDownload_Action_Action_ARRAYSIZE = RequestControlDownload_Action_Action_MAX + 1;

const ::google::protobuf::EnumDescriptor* RequestControlDownload_Action_descriptor();
inline const ::std::string& RequestControlDownload_Action_Name(RequestControlDownload_Action value) {
  return ::google::protobuf::internal::NameOfEnum(
    RequestControlDownload_Action_descriptor(), value);
}
inline bool RequestControlDownload_Action_Parse(
    const ::std::string& name, RequestControlDownload_Action* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RequestControlDownload_Action>(
    RequestControlDownload_Action_descriptor(), name, value);
}
enum RequestMsgIds {
  MsgId_RequestTransferList = 1,
  MsgId_RequestControlDownload = 2
};
bool RequestMsgIds_IsValid(int value);
const RequestMsgIds RequestMsgIds_MIN = MsgId_RequestTransferList;
const RequestMsgIds RequestMsgIds_MAX = MsgId_RequestControlDownload;
const int RequestMsgIds_ARRAYSIZE = RequestMsgIds_MAX + 1;

const ::google::protobuf::EnumDescriptor* RequestMsgIds_descriptor();
inline const ::std::string& RequestMsgIds_Name(RequestMsgIds value) {
  return ::google::protobuf::internal::NameOfEnum(
    RequestMsgIds_descriptor(), value);
}
inline bool RequestMsgIds_Parse(
    const ::std::string& name, RequestMsgIds* value) {
  return ::google::protobuf::internal::ParseNamedEnum<RequestMsgIds>(
    RequestMsgIds_descriptor(), name, value);
}
enum ResponseMsgIds {
  MsgId_ResponseTransferList = 1,
  MsgId_ResponseControlDownload = 2
};
bool ResponseMsgIds_IsValid(int value);
const ResponseMsgIds ResponseMsgIds_MIN = MsgId_ResponseTransferList;
const ResponseMsgIds ResponseMsgIds_MAX = MsgId_ResponseControlDownload;
const int ResponseMsgIds_ARRAYSIZE = ResponseMsgIds_MAX + 1;

const ::google::protobuf::EnumDescriptor* ResponseMsgIds_descriptor();
inline const ::std::string& ResponseMsgIds_Name(ResponseMsgIds value) {
  return ::google::protobuf::internal::NameOfEnum(
    ResponseMsgIds_descriptor(), value);
}
inline bool ResponseMsgIds_Parse(
    const ::std::string& name, ResponseMsgIds* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ResponseMsgIds>(
    ResponseMsgIds_descriptor(), name, value);
}
enum Direction {
  DIRECTION_UPLOAD = 1,
  DIRECTION_DOWNLOAD = 2
};
bool Direction_IsValid(int value);
const Direction Direction_MIN = DIRECTION_UPLOAD;
const Direction Direction_MAX = DIRECTION_DOWNLOAD;
const int Direction_ARRAYSIZE = Direction_MAX + 1;

const ::google::protobuf::EnumDescriptor* Direction_descriptor();
inline const ::std::string& Direction_Name(Direction value) {
  return ::google::protobuf::internal::NameOfEnum(
    Direction_descriptor(), value);
}
inline bool Direction_Parse(
    const ::std::string& name, Direction* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Direction>(
    Direction_descriptor(), name, value);
}
// ===================================================================

class FileTransfer : public ::google::protobuf::Message {
 public:
  FileTransfer();
  virtual ~FileTransfer();
  
  FileTransfer(const FileTransfer& from);
  
  inline FileTransfer& operator=(const FileTransfer& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const FileTransfer& default_instance();
  
  void Swap(FileTransfer* other);
  
  // implements Message ----------------------------------------------
  
  FileTransfer* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FileTransfer& from);
  void MergeFrom(const FileTransfer& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .rsctrl.core.File file = 1;
  inline bool has_file() const;
  inline void clear_file();
  static const int kFileFieldNumber = 1;
  inline const ::rsctrl::core::File& file() const;
  inline ::rsctrl::core::File* mutable_file();
  inline ::rsctrl::core::File* release_file();
  
  // required .rsctrl.files.Direction direction = 2;
  inline bool has_direction() const;
  inline void clear_direction();
  static const int kDirectionFieldNumber = 2;
  inline rsctrl::files::Direction direction() const;
  inline void set_direction(rsctrl::files::Direction value);
  
  // required float fraction = 3;
  inline bool has_fraction() const;
  inline void clear_fraction();
  static const int kFractionFieldNumber = 3;
  inline float fraction() const;
  inline void set_fraction(float value);
  
  // required float rate_kBs = 4;
  inline bool has_rate_kbs() const;
  inline void clear_rate_kbs();
  static const int kRateKBsFieldNumber = 4;
  inline float rate_kbs() const;
  inline void set_rate_kbs(float value);
  
  // @@protoc_insertion_point(class_scope:rsctrl.files.FileTransfer)
 private:
  inline void set_has_file();
  inline void clear_has_file();
  inline void set_has_direction();
  inline void clear_has_direction();
  inline void set_has_fraction();
  inline void clear_has_fraction();
  inline void set_has_rate_kbs();
  inline void clear_has_rate_kbs();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::rsctrl::core::File* file_;
  int direction_;
  float fraction_;
  float rate_kbs_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
  friend void  protobuf_AddDesc_files_2eproto();
  friend void protobuf_AssignDesc_files_2eproto();
  friend void protobuf_ShutdownFile_files_2eproto();
  
  void InitAsDefaultInstance();
  static FileTransfer* default_instance_;
};
// -------------------------------------------------------------------

class RequestTransferList : public ::google::protobuf::Message {
 public:
  RequestTransferList();
  virtual ~RequestTransferList();
  
  RequestTransferList(const RequestTransferList& from);
  
  inline RequestTransferList& operator=(const RequestTransferList& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const RequestTransferList& default_instance();
  
  void Swap(RequestTransferList* other);
  
  // implements Message ----------------------------------------------
  
  RequestTransferList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RequestTransferList& from);
  void MergeFrom(const RequestTransferList& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .rsctrl.files.Direction direction = 1;
  inline bool has_direction() const;
  inline void clear_direction();
  static const int kDirectionFieldNumber = 1;
  inline rsctrl::files::Direction direction() const;
  inline void set_direction(rsctrl::files::Direction value);
  
  // @@protoc_insertion_point(class_scope:rsctrl.files.RequestTransferList)
 private:
  inline void set_has_direction();
  inline void clear_has_direction();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  int direction_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_files_2eproto();
  friend void protobuf_AssignDesc_files_2eproto();
  friend void protobuf_ShutdownFile_files_2eproto();
  
  void InitAsDefaultInstance();
  static RequestTransferList* default_instance_;
};
// -------------------------------------------------------------------

class ResponseTransferList : public ::google::protobuf::Message {
 public:
  ResponseTransferList();
  virtual ~ResponseTransferList();
  
  ResponseTransferList(const ResponseTransferList& from);
  
  inline ResponseTransferList& operator=(const ResponseTransferList& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ResponseTransferList& default_instance();
  
  void Swap(ResponseTransferList* other);
  
  // implements Message ----------------------------------------------
  
  ResponseTransferList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ResponseTransferList& from);
  void MergeFrom(const ResponseTransferList& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .rsctrl.core.Status status = 1;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 1;
  inline const ::rsctrl::core::Status& status() const;
  inline ::rsctrl::core::Status* mutable_status();
  inline ::rsctrl::core::Status* release_status();
  
  // repeated .rsctrl.files.FileTransfer transfers = 2;
  inline int transfers_size() const;
  inline void clear_transfers();
  static const int kTransfersFieldNumber = 2;
  inline const ::rsctrl::files::FileTransfer& transfers(int index) const;
  inline ::rsctrl::files::FileTransfer* mutable_transfers(int index);
  inline ::rsctrl::files::FileTransfer* add_transfers();
  inline const ::google::protobuf::RepeatedPtrField< ::rsctrl::files::FileTransfer >&
      transfers() const;
  inline ::google::protobuf::RepeatedPtrField< ::rsctrl::files::FileTransfer >*
      mutable_transfers();
  
  // @@protoc_insertion_point(class_scope:rsctrl.files.ResponseTransferList)
 private:
  inline void set_has_status();
  inline void clear_has_status();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::rsctrl::core::Status* status_;
  ::google::protobuf::RepeatedPtrField< ::rsctrl::files::FileTransfer > transfers_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_files_2eproto();
  friend void protobuf_AssignDesc_files_2eproto();
  friend void protobuf_ShutdownFile_files_2eproto();
  
  void InitAsDefaultInstance();
  static ResponseTransferList* default_instance_;
};
// -------------------------------------------------------------------

class RequestControlDownload : public ::google::protobuf::Message {
 public:
  RequestControlDownload();
  virtual ~RequestControlDownload();
  
  RequestControlDownload(const RequestControlDownload& from);
  
  inline RequestControlDownload& operator=(const RequestControlDownload& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const RequestControlDownload& default_instance();
  
  void Swap(RequestControlDownload* other);
  
  // implements Message ----------------------------------------------
  
  RequestControlDownload* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RequestControlDownload& from);
  void MergeFrom(const RequestControlDownload& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  typedef RequestControlDownload_Action Action;
  static const Action ACTION_START = RequestControlDownload_Action_ACTION_START;
  static const Action ACTION_CONTINUE = RequestControlDownload_Action_ACTION_CONTINUE;
  static const Action ACTION_WAIT = RequestControlDownload_Action_ACTION_WAIT;
  static const Action ACTION_PAUSE = RequestControlDownload_Action_ACTION_PAUSE;
  static const Action ACTION_RESTART = RequestControlDownload_Action_ACTION_RESTART;
  static const Action ACTION_CHECK = RequestControlDownload_Action_ACTION_CHECK;
  static const Action ACTION_CANCEL = RequestControlDownload_Action_ACTION_CANCEL;
  static inline bool Action_IsValid(int value) {
    return RequestControlDownload_Action_IsValid(value);
  }
  static const Action Action_MIN =
    RequestControlDownload_Action_Action_MIN;
  static const Action Action_MAX =
    RequestControlDownload_Action_Action_MAX;
  static const int Action_ARRAYSIZE =
    RequestControlDownload_Action_Action_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Action_descriptor() {
    return RequestControlDownload_Action_descriptor();
  }
  static inline const ::std::string& Action_Name(Action value) {
    return RequestControlDownload_Action_Name(value);
  }
  static inline bool Action_Parse(const ::std::string& name,
      Action* value) {
    return RequestControlDownload_Action_Parse(name, value);
  }
  
  // accessors -------------------------------------------------------
  
  // required .rsctrl.core.File file = 1;
  inline bool has_file() const;
  inline void clear_file();
  static const int kFileFieldNumber = 1;
  inline const ::rsctrl::core::File& file() const;
  inline ::rsctrl::core::File* mutable_file();
  inline ::rsctrl::core::File* release_file();
  
  // required .rsctrl.files.RequestControlDownload.Action action = 2;
  inline bool has_action() const;
  inline void clear_action();
  static const int kActionFieldNumber = 2;
  inline ::rsctrl::files::RequestControlDownload_Action action() const;
  inline void set_action(::rsctrl::files::RequestControlDownload_Action value);
  
  // @@protoc_insertion_point(class_scope:rsctrl.files.RequestControlDownload)
 private:
  inline void set_has_file();
  inline void clear_has_file();
  inline void set_has_action();
  inline void clear_has_action();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::rsctrl::core::File* file_;
  int action_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_files_2eproto();
  friend void protobuf_AssignDesc_files_2eproto();
  friend void protobuf_ShutdownFile_files_2eproto();
  
  void InitAsDefaultInstance();
  static RequestControlDownload* default_instance_;
};
// -------------------------------------------------------------------

class ResponseControlDownload : public ::google::protobuf::Message {
 public:
  ResponseControlDownload();
  virtual ~ResponseControlDownload();
  
  ResponseControlDownload(const ResponseControlDownload& from);
  
  inline ResponseControlDownload& operator=(const ResponseControlDownload& from) {
    CopyFrom(from);
    return *this;
  }
  
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }
  
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }
  
  static const ::google::protobuf::Descriptor* descriptor();
  static const ResponseControlDownload& default_instance();
  
  void Swap(ResponseControlDownload* other);
  
  // implements Message ----------------------------------------------
  
  ResponseControlDownload* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ResponseControlDownload& from);
  void MergeFrom(const ResponseControlDownload& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::google::protobuf::Metadata GetMetadata() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required .rsctrl.core.Status status = 1;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 1;
  inline const ::rsctrl::core::Status& status() const;
  inline ::rsctrl::core::Status* mutable_status();
  inline ::rsctrl::core::Status* release_status();
  
  // @@protoc_insertion_point(class_scope:rsctrl.files.ResponseControlDownload)
 private:
  inline void set_has_status();
  inline void clear_has_status();
  
  ::google::protobuf::UnknownFieldSet _unknown_fields_;
  
  ::rsctrl::core::Status* status_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_files_2eproto();
  friend void protobuf_AssignDesc_files_2eproto();
  friend void protobuf_ShutdownFile_files_2eproto();
  
  void InitAsDefaultInstance();
  static ResponseControlDownload* default_instance_;
};
// ===================================================================


// ===================================================================

// FileTransfer

// required .rsctrl.core.File file = 1;
inline bool FileTransfer::has_file() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void FileTransfer::set_has_file() {
  _has_bits_[0] |= 0x00000001u;
}
inline void FileTransfer::clear_has_file() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void FileTransfer::clear_file() {
  if (file_ != NULL) file_->::rsctrl::core::File::Clear();
  clear_has_file();
}
inline const ::rsctrl::core::File& FileTransfer::file() const {
  return file_ != NULL ? *file_ : *default_instance_->file_;
}
inline ::rsctrl::core::File* FileTransfer::mutable_file() {
  set_has_file();
  if (file_ == NULL) file_ = new ::rsctrl::core::File;
  return file_;
}
inline ::rsctrl::core::File* FileTransfer::release_file() {
  clear_has_file();
  ::rsctrl::core::File* temp = file_;
  file_ = NULL;
  return temp;
}

// required .rsctrl.files.Direction direction = 2;
inline bool FileTransfer::has_direction() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void FileTransfer::set_has_direction() {
  _has_bits_[0] |= 0x00000002u;
}
inline void FileTransfer::clear_has_direction() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void FileTransfer::clear_direction() {
  direction_ = 1;
  clear_has_direction();
}
inline rsctrl::files::Direction FileTransfer::direction() const {
  return static_cast< rsctrl::files::Direction >(direction_);
}
inline void FileTransfer::set_direction(rsctrl::files::Direction value) {
  GOOGLE_DCHECK(rsctrl::files::Direction_IsValid(value));
  set_has_direction();
  direction_ = value;
}

// required float fraction = 3;
inline bool FileTransfer::has_fraction() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void FileTransfer::set_has_fraction() {
  _has_bits_[0] |= 0x00000004u;
}
inline void FileTransfer::clear_has_fraction() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void FileTransfer::clear_fraction() {
  fraction_ = 0;
  clear_has_fraction();
}
inline float FileTransfer::fraction() const {
  return fraction_;
}
inline void FileTransfer::set_fraction(float value) {
  set_has_fraction();
  fraction_ = value;
}

// required float rate_kBs = 4;
inline bool FileTransfer::has_rate_kbs() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void FileTransfer::set_has_rate_kbs() {
  _has_bits_[0] |= 0x00000008u;
}
inline void FileTransfer::clear_has_rate_kbs() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void FileTransfer::clear_rate_kbs() {
  rate_kbs_ = 0;
  clear_has_rate_kbs();
}
inline float FileTransfer::rate_kbs() const {
  return rate_kbs_;
}
inline void FileTransfer::set_rate_kbs(float value) {
  set_has_rate_kbs();
  rate_kbs_ = value;
}

// -------------------------------------------------------------------

// RequestTransferList

// required .rsctrl.files.Direction direction = 1;
inline bool RequestTransferList::has_direction() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RequestTransferList::set_has_direction() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RequestTransferList::clear_has_direction() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RequestTransferList::clear_direction() {
  direction_ = 1;
  clear_has_direction();
}
inline rsctrl::files::Direction RequestTransferList::direction() const {
  return static_cast< rsctrl::files::Direction >(direction_);
}
inline void RequestTransferList::set_direction(rsctrl::files::Direction value) {
  GOOGLE_DCHECK(rsctrl::files::Direction_IsValid(value));
  set_has_direction();
  direction_ = value;
}

// -------------------------------------------------------------------

// ResponseTransferList

// required .rsctrl.core.Status status = 1;
inline bool ResponseTransferList::has_status() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ResponseTransferList::set_has_status() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ResponseTransferList::clear_has_status() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ResponseTransferList::clear_status() {
  if (status_ != NULL) status_->::rsctrl::core::Status::Clear();
  clear_has_status();
}
inline const ::rsctrl::core::Status& ResponseTransferList::status() const {
  return status_ != NULL ? *status_ : *default_instance_->status_;
}
inline ::rsctrl::core::Status* ResponseTransferList::mutable_status() {
  set_has_status();
  if (status_ == NULL) status_ = new ::rsctrl::core::Status;
  return status_;
}
inline ::rsctrl::core::Status* ResponseTransferList::release_status() {
  clear_has_status();
  ::rsctrl::core::Status* temp = status_;
  status_ = NULL;
  return temp;
}

// repeated .rsctrl.files.FileTransfer transfers = 2;
inline int ResponseTransferList::transfers_size() const {
  return transfers_.size();
}
inline void ResponseTransferList::clear_transfers() {
  transfers_.Clear();
}
inline const ::rsctrl::files::FileTransfer& ResponseTransferList::transfers(int index) const {
  return transfers_.Get(index);
}
inline ::rsctrl::files::FileTransfer* ResponseTransferList::mutable_transfers(int index) {
  return transfers_.Mutable(index);
}
inline ::rsctrl::files::FileTransfer* ResponseTransferList::add_transfers() {
  return transfers_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::rsctrl::files::FileTransfer >&
ResponseTransferList::transfers() const {
  return transfers_;
}
inline ::google::protobuf::RepeatedPtrField< ::rsctrl::files::FileTransfer >*
ResponseTransferList::mutable_transfers() {
  return &transfers_;
}

// -------------------------------------------------------------------

// RequestControlDownload

// required .rsctrl.core.File file = 1;
inline bool RequestControlDownload::has_file() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RequestControlDownload::set_has_file() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RequestControlDownload::clear_has_file() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RequestControlDownload::clear_file() {
  if (file_ != NULL) file_->::rsctrl::core::File::Clear();
  clear_has_file();
}
inline const ::rsctrl::core::File& RequestControlDownload::file() const {
  return file_ != NULL ? *file_ : *default_instance_->file_;
}
inline ::rsctrl::core::File* RequestControlDownload::mutable_file() {
  set_has_file();
  if (file_ == NULL) file_ = new ::rsctrl::core::File;
  return file_;
}
inline ::rsctrl::core::File* RequestControlDownload::release_file() {
  clear_has_file();
  ::rsctrl::core::File* temp = file_;
  file_ = NULL;
  return temp;
}

// required .rsctrl.files.RequestControlDownload.Action action = 2;
inline bool RequestControlDownload::has_action() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RequestControlDownload::set_has_action() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RequestControlDownload::clear_has_action() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RequestControlDownload::clear_action() {
  action_ = 1;
  clear_has_action();
}
inline ::rsctrl::files::RequestControlDownload_Action RequestControlDownload::action() const {
  return static_cast< ::rsctrl::files::RequestControlDownload_Action >(action_);
}
inline void RequestControlDownload::set_action(::rsctrl::files::RequestControlDownload_Action value) {
  GOOGLE_DCHECK(::rsctrl::files::RequestControlDownload_Action_IsValid(value));
  set_has_action();
  action_ = value;
}

// -------------------------------------------------------------------

// ResponseControlDownload

// required .rsctrl.core.Status status = 1;
inline bool ResponseControlDownload::has_status() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ResponseControlDownload::set_has_status() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ResponseControlDownload::clear_has_status() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ResponseControlDownload::clear_status() {
  if (status_ != NULL) status_->::rsctrl::core::Status::Clear();
  clear_has_status();
}
inline const ::rsctrl::core::Status& ResponseControlDownload::status() const {
  return status_ != NULL ? *status_ : *default_instance_->status_;
}
inline ::rsctrl::core::Status* ResponseControlDownload::mutable_status() {
  set_has_status();
  if (status_ == NULL) status_ = new ::rsctrl::core::Status;
  return status_;
}
inline ::rsctrl::core::Status* ResponseControlDownload::release_status() {
  clear_has_status();
  ::rsctrl::core::Status* temp = status_;
  status_ = NULL;
  return temp;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace files
}  // namespace rsctrl

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::rsctrl::files::RequestControlDownload_Action>() {
  return ::rsctrl::files::RequestControlDownload_Action_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< rsctrl::files::RequestMsgIds>() {
  return rsctrl::files::RequestMsgIds_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< rsctrl::files::ResponseMsgIds>() {
  return rsctrl::files::ResponseMsgIds_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< rsctrl::files::Direction>() {
  return rsctrl::files::Direction_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_files_2eproto__INCLUDED
