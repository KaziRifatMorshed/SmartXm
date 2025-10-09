#include "db_xampp.h"

// Static member definition
localDB* localDB::_dbInstance = nullptr;



/*
/home/noobcod3r-rtx/Documents/GitHub/SmartXm-Cross-Platform-Desktop-Exam-Management-with-Integrated-Judge/src/SmartXm-CSEKU/build/Desktop_Qt_6_9_2-Debug/welcomewindow.o:-1: error: first defined here
:-1: error: linker command failed with exit code 1 (use -v to see invocation)
clang++: error: linker command failed with exit code 1 (use -v to see invocation)
make: *** [Makefile:339: SmartXm-CSEKU] Error 1


This error means that **`localDB* localDB::_dbInstance` is being defined in more than one translation unit (object file)**—most likely, you have placed the definition in a header file (`db_xampp.h`) that is included in multiple `.cpp` files.

**Static members of a class must be defined exactly once in a `.cpp` file, NOT in a header.**

---

## How to fix

### 1. **Remove this line from your header (`db_xampp.h`):**
```cpp
localDB* localDB::_dbInstance = nullptr;
```

### 2. **Add it to a source file (e.g., `db_xampp.cpp`):**
If you don't already have a `db_xampp.cpp`, create one and put:

```cpp name=db_xampp.cpp
#include "db_xampp.h"

// Static member definition
localDB* localDB::_dbInstance = nullptr;
```

---

## TL;DR

- **Header (`db_xampp.h`)**: Only declare `static localDB* _dbInstance;` inside the class.
- **Source (`db_xampp.cpp`)**: Define `localDB* localDB::_dbInstance = nullptr;` exactly once.

---

### If you cannot add a new `.cpp` file for some reason, put the definition in ONE `.cpp` file that already includes `db_xampp.h`, but **never in the header**.

---

 **After this change, you will no longer get "first defined here" or duplicate symbol linker errors.**
 */
