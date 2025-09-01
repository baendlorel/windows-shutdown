#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>

#include <string>

// order below must be preserved
#include <objidl.h>
#include <gdiplus.h>

#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;