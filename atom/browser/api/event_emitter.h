// Copyright (c) 2014 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_API_EVENT_EMITTER_H_
#define ATOM_BROWSER_API_EVENT_EMITTER_H_

#include <vector>

#include "native_mate/wrappable.h"

namespace content {
class WebContents;
}

namespace IPC {
class Message;
}

namespace mate {

// Provide helperers to emit event in JavaScript.
class EventEmitter : public Wrappable {
 public:
  typedef std::vector<v8::Local<v8::Value>> ValueArray;

 protected:
  EventEmitter();

  // this.emit(name, new Event(), args...);
  template<typename... Args>
  bool Emit(const base::StringPiece& name, const Args&... args) {
    return EmitWithSender(name, nullptr, nullptr, args...);
  }

  // this.emit(name, new Event(sender, message), args...);
  template<typename... Args>
  bool EmitWithSender(const base::StringPiece& name,
                      content::WebContents* sender,
                      IPC::Message* message,
                      const Args&... args) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Locker locker(isolate);
    v8::HandleScope handle_scope(isolate);

    ValueArray converted = { ConvertToV8(isolate, args)... };
    return CallEmit(isolate, name, sender, message, converted);
  }

 private:
  // Lower level implementations.
  bool CallEmit(v8::Isolate* isolate,
                const base::StringPiece& name,
                content::WebContents* sender,
                IPC::Message* message,
                ValueArray args);

  DISALLOW_COPY_AND_ASSIGN(EventEmitter);
};

}  // namespace mate

#endif  // ATOM_BROWSER_API_EVENT_EMITTER_H_
