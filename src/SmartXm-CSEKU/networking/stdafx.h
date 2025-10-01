/**
* @file    AntiRanSomware user mode engine
* @brief
* @ref
* @author  Yonhgwhan, Roh (fixbrain@gmail.com)
* @date    2017/01/21 created.
* @copyright All rights reserved by Yonghwan, Roh.
**/
#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

//
// std
//
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>



// 
// boost
//
//#define BOOST_LIB_DIAGNOSTIC
//#include "boost/lexical_cast.hpp"
//#include "boost/type_traits.hpp"		// boost::remove_pointer
//#include "boost/noncopyable.hpp"
//#include "boost/format.hpp"
//#include "boost/thread.hpp"
//#include "boost/shared_ptr.hpp"

//
// windows headers
//

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <Windows.h>
#include <VersionHelpers.h>
#include <crtdbg.h>
#include <stdint.h>
#include <strsafe.h>
#include <winioctl.h>
#include <dontuse.h>

#include <conio.h>
#include <winioctl.h>
//#include <winnt.h>


//
// _my_lib
// 
//#include "log.h"
//#include "Win32Utils.h"
//#include "scm_context.h"
//#include "RegistryUtil.h"

#pragma once


/*
// -----------------------------------------------------------------------------------------
// <copyright file="stdafx.h" company="Microsoft">
//    Copyright 2013 Microsoft Corporation
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// </copyright>
// -----------------------------------------------------------------------------------------

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#pragma warning(push)
#pragma warning(disable: 4634 4635 4638 4251 4100 4503 4996)

#ifndef NOMINMAX
#define NOMINMAX
#endif

// This is required to support enough number of arguments in VC11, especially for std::bind
#define _VARIADIC_MAX 8

#include <iostream>
#include <algorithm>
#include <math.h>
#include <limits>
#include <functional>
#include <iomanip>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#include "cpprest/http_client.h"
#include "cpprest/filestream.h"
#include "cpprest/producerconsumerstream.h"

#pragma warning(pop)

#pragma warning(disable: 4503)
*/