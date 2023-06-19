#include "gc_load.h"

namespace GC {

    GCLoad::GCLoad(): node::ObjectWrap() {}

    void GCLoad::New(const FunctionCallbackInfo<Value>& args) {
        GCLoad* obj = new GCLoad();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }

    static void BeforeGCCallback(Isolate* isolate,
                                 v8::GCType gc_type,
                                 v8::GCCallbackFlags flags,
                                 void* data) {
        GCLoad* gc_load = static_cast<GCLoad*>(data);
        if (gc_load->current_gc_type != 0) {
            return;
        }
        gc_load->current_gc_type = gc_type;
        gc_load->start_time = uv_hrtime();
    }

    static void AfterGCCallback(Isolate* isolate,
                                v8::GCType gc_type,
                                v8::GCCallbackFlags flags,
                                void* data) {
        GCLoad* gc_load = static_cast<GCLoad*>(data);
        if (gc_load->current_gc_type != gc_type) {
            return;
        }
        gc_load->current_gc_type = 0;
        gc_load->total_time += uv_hrtime() - gc_load->start_time;
        gc_load->start_time = 0;
    }

    void GCLoad::Start(const FunctionCallbackInfo<Value>& args) {
        GCLoad* obj = ObjectWrap::Unwrap<GCLoad>(args.Holder());
        Isolate::GetCurrent()->AddGCPrologueCallback(BeforeGCCallback,
                                                    static_cast<void*>(obj));
        Isolate::GetCurrent()->AddGCEpilogueCallback(AfterGCCallback,
                                                static_cast<void*>(obj));
    }

    void GCLoad::Total(const FunctionCallbackInfo<Value>& args) {
        GCLoad* obj = ObjectWrap::Unwrap<GCLoad>(args.Holder());
        args.GetReturnValue().Set(v8::Number::New(args.GetIsolate(), static_cast<double>(obj->total_time / 1e3)));
    }

    void GCLoad::Stop(const FunctionCallbackInfo<Value>& args) {
        GCLoad* obj = ObjectWrap::Unwrap<GCLoad>(args.Holder());
        Isolate::GetCurrent()->AddGCPrologueCallback(BeforeGCCallback,
                                                    static_cast<void*>(obj));
        Isolate::GetCurrent()->AddGCEpilogueCallback(AfterGCCallback,
                                                    static_cast<void*>(obj));
    }

    NODE_MODULE_INIT(/* exports, module, context */) {
        Isolate* isolate = exports->GetIsolate();
        Local<String> name = String::NewFromUtf8(isolate, "GCLoad").ToLocalChecked();
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, GCLoad::New);
        tpl->SetClassName(name);
        tpl->InstanceTemplate()->SetInternalFieldCount(1);
        NODE_SET_PROTOTYPE_METHOD(tpl, "start", GCLoad::Start);
        NODE_SET_PROTOTYPE_METHOD(tpl, "total", GCLoad::Total);
        NODE_SET_PROTOTYPE_METHOD(tpl, "stop", GCLoad::Stop);
        exports->Set(context, name, tpl->GetFunction(context).ToLocalChecked());
    }
}
