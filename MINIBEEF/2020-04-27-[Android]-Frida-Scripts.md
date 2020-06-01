---
title: "[Android] Frida Scripts"
date: 2020-04-27
categories: Android
sitemap :
  changefreq : daily
  priority : 1.0
---



`Android Hooking`에 필요한 테크닉을 정리하였다. 아이디어가 생기면 계속해서 추가할 것



# Hook Java Method

```javascript
Java.perform(function() {
    TargetClass = Java.use('com.target.class');
	TargetClass.Method.implementation = function() { // overload()
        console.log('[*] method hooked!');
    }
});
```

 Dex 분석을 통해 얻어낸  Method 심볼로 해당 함수 내용을 대체하거나 무력화한다.



# Hook Java Instance

```javascript
Java.perform(function() {
    Java.choose("com.examplePackage.exampleClass",
    {
        /* when instance founded, 'onMatch` function will called */
        onMatch: function(instance)
        {
            console.log("[+] Instance Found! Hook Start");
        },    
        
        /* when their action is finished, 'onComplete' function will called */
        onComplete: function()
        {
            console.log("[*] Instance Finished");
        }
    });
});
```

`Java.Choose`을 `onMatch`로 돌린다. 해당 Class가 생성한 인스턴스가 생기면 이를 후킹한다.



# Hook Constructor

```javascript
Java.use('java.lang.StringBuilder').$init.overload('java.lang.String').implementation = function(stringArgument) {
      console.log("c'tor");
      return this.$init(stringArgument);
};
```

생성자를 후킹



# Trace Method Arguments

```javascript
Java.perform(function() { 
  YourClass.Method.implementation = function(arg1, arg2) { // rely on the number of args.
    console.log("arg1 : " + arg1);
    console.log("arg2 : " + arg2);
    return this.Method;
  }
});
```

 메서드가 호출될 때 기존의 프로그램 동작은 건들지 않으면서 전달되는 인자를 보고싶을 때 사용



# Trace Method Caller

```javascript
/* trace call stack and caller using current thread */
Java.perform(function() {
  function arrange(stack) {
    var ret = "";
    for(var i = 0; i < stack.length; ++i)
      ret += stack[i].toString() + "\n";  
    
    return ret;
  }
  
  var Thread = Java.use("java.lang.Thread");
  var ThreadInstance = Thread.$new();
  
  YourClass.Method.implementation = function() { // or using Interceptor.attach
    var stack = ThreadInstance.currentThread().getStackTrace(); // stack[3] is caller
    console.log(stack[3] + " Call This Method!");
    
    /* if u want seeing call stack */
    var full_call_stack = arrange(stack);
    console.log("Call Stack is : " + full_call_stack); 
  }
}
```

 해당 메서드를 호출한 패키지/클래스들의 행적을 추적할 때 사용



# Hook JNI by Address

```javascript
var moduleName = "libfoo.so"; 
var nativeFuncAddr = 0x1234;
Interceptor.attach(Module.findExportByName(null, "dlopen"), {
    onEnter: function(args) {
        this.lib = Memory.readUtf8String(args[0]);
        console.log("dlopen called with: " + this.lib);
    },
    onLeave: function(retval) {
        if (this.lib.endsWith(moduleName)) {
            console.log("ret: " + retval);
            var baseAddr = Module.findBaseAddress(moduleName);
            Interceptor.attach(baseAddr.add(nativeFuncAddr), {
                onEnter: function(args) {
                    console.log("[-] hook invoked");
                    console.log(JSON.stringify({
                        a1: args[1].toInt32(),
                        a2: Memory.readUtf8String(Memory.readPointer(args[2])),
                        a3: Boolean(args[3])
                    }, null, '\t'));
                }
            });
        }
    }
});
```

 동적 라이브러리 분석을 통해 후킹하고자 하는 함수의 RVA를 알아냈을 때 native를 후킹하는 방법



# Shared Preferences updates

```javascript
function notifyNewSharedPreference() {
  Java.use('android.app.SharedPreferencesImpl$EditorImpl').putString.overload('java.lang.String', 'java.lang.String').implementation = function(k, v) {
    console.log('[SharedPreferencesImpl]', k, '=', v);
    return this.putString(k, v);
  }
}
```

Shared Preferences를 런타임에서 건들고 싶을 때



# Log SQLite query

```javascript
Interceptor.attach(Module.findExportByName('libsqlite.so', 'sqlite3_prepare16_v2'), {
      onEnter: function(args) {
          console.log('DB: ' + Memory.readUtf16String(args[0]) + '\tSQL: ' + Memory.readUtf16String(args[1]));
      }
});
```

 서버로 전송되는 쿼리를 조회하고 싶을때 사용함. 상황에 따라 쿼리를 조작할 수 있다.