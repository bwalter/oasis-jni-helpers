🏝 Oasis JNI helpers
===============

Simple JNI wrapper using the RAII idiom to ensure that all local and global
references are automatically deleted.


## Classes

- `JniContext`: wraps the JNI environment and all JNI methods
- `JniLocalRef`: auto-released local reference
- `JniGlobalRef`: auto-released global reference
- `JStringLocalRef`: auto-released jstring, support for UTF-8/16 conversions from/to
Java
- `JArrayLocalRef`: auto-released jarray (of primitives) with read or read/write
access to array elements
- `JObjectLocalRef`: auto-released jobjectArray with read or read/write access to
array elements via JniLocalRef
- `JValue`: wraps Java primitives (via jvalue) or objets (via JniLocalRef)


## Main features

- access through a single JniContext instance
- forwards all JNI function calls from JniContext to JNIEnv
- enforces using local/global references wrappers (no implicit cast from
JniLocalRef/JniGlobalRef to raw jobject)
- optimized for minimal usage of new local/references


## Performance

These JNI helpers are optimized to minimize the number of new local/global references
created. It uses a shared pointer for sharing the references when copying
JniLocalRef/JniGlobalRefs (instead of creating new ones).

JValue explicitly wraps a JniLocalRef to make sure that local references are properly
released when needed.

Except for the very small memory cost of using a shared pointer to manage shared
references, the wrapper is extremly lightweight and usually just forwards the
calls to the raw JNI functions.


## JniContext lifecycle and single vs multithreading

The JniContext lifecycle superseeds the one of the JNIEnv and it can (and should) be
stored as long as your application needs it.

There are 2 ways to manage the JNIEnv reference:
- Manual mode (`JniContext(JNIEnv *, EnvironmentSource::Manual)`): this only works  
in a single-thread environment. The JNIEnv instance is given as constructor parameter
and can be updated when needed via `JniContext::setCurrentJNIEnv(JNIEnv *)`.
- Automatic mode (`JniContext(JNIEnv *, EnvironmentSource::JvmAuto)`): this can be
used in multi-threaded environments. The JNIEnv instanced is fetched when needed
via the JavaVM.


## Usage

Just include the .h and .cpp files into your project.

Note: C++17 required!

```c++
auto jniContext = JniContext(jniEnv);

{
  JNiLocalRef<jobject> ref1(jniContext, jobject1);  // from (raw) jobject
  ...
}  // ref1 auto-released

// New Java string from C string
JStringLocalRef js(jniContext, "input text");

// Call Java method
JNiLocalRef<jclass> stringClass = jniContext->findClass("java/lang/String");
jmethodID capitalize = jniContext->findMethodId(stringClass, "capitalize", "()Ljava/lang/String;");
std::string res = jniContext->callStringMethod(js, capitalize).toUtf8Chars();

std::cout << "Capitalized string = " << res.toUtf8Chars() << std::endl;
```

## Example

See https://github.com/prosiebensat-1-digital-gmbh/oasis-jsbridge


## Test

```
$ cd test
$ mkdir build
$ cmake ..
$ cmake --build .
$ ctest
```

(Google Test will be automatically downloaded and compiled via cmake)


## License

```
Copyright (C) 2019 Benoit Walter
Copyright (C) 2018-2019 ProSiebenSat1.Digital GmbH

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

