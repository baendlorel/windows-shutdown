#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN          
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>

// order below must be preserved
#include <objidl.h>
#include <gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;