#include "ui.h"

void CenterButtons(int w, int h) {
  int centerX = w / 2;
  int centerY = h / 2;
  int spacing = 100;
  int r = 60;
  buttons[0] = {centerX - spacing, centerY, r, L"Restart", IDB_RESTARTPNG};
  buttons[1] = {centerX + spacing, centerY, r, L"Shutdown", IDB_SHUTDOWNPNG};
}