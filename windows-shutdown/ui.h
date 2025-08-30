#pragma once
#include "Resource.h"

static void CenterButtons(int w, int h);

struct ImageButton {
  int x, y, r;
  const wchar_t* label;
  int resId;
};
ImageButton buttons[2] = {{0, 0, 60, L"Restart", IDB_RESTARTPNG},
                          {0, 0, 60, L"Shutdown", IDB_SHUTDOWNPNG}};