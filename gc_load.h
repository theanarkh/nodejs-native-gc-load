#ifndef GC_LOAD_H
#define GC_LOAD_H

#include <node.h>
#include <uv.h>
#include <node_object_wrap.h>

namespace GC {

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

class GCLoad : public node::ObjectWrap {
 public:
  static void New(const FunctionCallbackInfo<Value>& args);
  static void Start(const FunctionCallbackInfo<Value>& args);
  static void Total(const FunctionCallbackInfo<Value>& args);
  static void Stop(const FunctionCallbackInfo<Value>& args);
  u_int64_t start_time = 0;
  u_int64_t total_time = 0;
  u_int8_t current_gc_type = 0;
 private:
  explicit GCLoad();
};

}

#endif