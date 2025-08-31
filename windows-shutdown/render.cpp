#include "framework.h"
#include "app-state.h"
#include "ui.h"
#include "render.h"

void DrawToMemoryDC(HDC hdcMem, int w, int h, BYTE alpha) {
  auto& appState = AppState::getInstance();
  Graphics graphics(hdcMem);
  graphics.SetSmoothingMode(SmoothingModeAntiAlias);
  graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
  // Draw semi-transparent white background
  SolidBrush bgBrush(Color(77, 255, 255, 255));
  graphics.FillRectangle(&bgBrush, 0, 0, w, h);

  if (appState.isCountingDown) {
    // Draw countdown in center
    std::wstring countdownText = std::to_wstring(appState.countdownSeconds);
    std::wstring actionText =
        appState.isRestartCountdown ? L"Restarting in " : L"Shutting down in ";
    std::wstring fullText = actionText + countdownText + L" seconds...";

    // Large font for countdown
    FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 48, FontStyleBold);

    // Get text bounds
    RectF layoutRect(0, 0, (REAL)w, (REAL)h);
    RectF boundingBox;
    graphics.MeasureString(fullText.c_str(), -1, &font, layoutRect,
                           &boundingBox);

    // Center the text
    REAL x = (w - boundingBox.Width) / 2;
    REAL y = (h - boundingBox.Height) / 2;

    // Draw main text with outline
    DrawTextWithOutline(graphics, fullText.c_str(), font, PointF(x, y), 3);

    // Draw cancel instruction with outline
    Gdiplus::Font smallFont(&fontFamily, 16, FontStyleRegular);
    std::wstring cancelText = L"Click anywhere or press any key to cancel";
    RectF cancelBounds;
    graphics.MeasureString(cancelText.c_str(), -1, &smallFont, layoutRect,
                           &cancelBounds);
    REAL cancelX = (w - cancelBounds.Width) / 2;
    REAL cancelY = y + boundingBox.Height + 20;

    // Draw cancel text with thinner outline
    DrawTextWithOutline(graphics, cancelText.c_str(), smallFont,
                        PointF(cancelX, cancelY), 2);
  } else {
    // Draw image buttons (original logic)
    for (int i = 0; i < 5; ++i) {
      Bitmap* bmp =
          LoadPngFromResource(appState.hInst, appState.buttons[i].resId);
      if (bmp) {
        int x = appState.buttons[i].x - appState.buttons[i].r;
        int y = appState.buttons[i].y - appState.buttons[i].r;
        int size = appState.buttons[i].r * 2;
        graphics.DrawImage(bmp, x, y, size, size);
        // If hovered, overlay a semi-transparent white
        if (i == appState.hoveredIndex) {
          SolidBrush highlightBrush(Color(28, 255, 255, 255));
          graphics.FillEllipse(&highlightBrush, x, y, size, size);
        }
        delete bmp;
      }
    }
  }
}

// Helper function to draw text with outline effect
void DrawTextWithOutline(Graphics& graphics, const wchar_t* text,
                                const Gdiplus::Font& font,
                                const PointF& position, int outlineWidth) {
  SolidBrush outlineBrush(Color(255, 0, 0, 0));     // Black outline
  SolidBrush textBrush(Color(255, 255, 255, 255));  // White text

  // Draw outline by rendering text in multiple directions
  for (int dx = -outlineWidth; dx <= outlineWidth; dx++) {
    for (int dy = -outlineWidth; dy <= outlineWidth; dy++) {
      if (dx != 0 || dy != 0) {  // Skip center position
        graphics.DrawString(text, -1, &font,
                            PointF(position.X + dx, position.Y + dy),
                            &outlineBrush);
      }
    }
  }

  // Draw main text on top
  graphics.DrawString(text, -1, &font, position, &textBrush);
}

void UpdateLayered(HWND hWnd, BYTE alpha) {
  RECT rc;
  GetClientRect(hWnd, &rc);
  int w = rc.right - rc.left;
  int h = rc.bottom - rc.top;
  CenterButtons(w, h);
  HDC hdcScreen = GetDC(NULL);
  HDC hdcMem = CreateCompatibleDC(hdcScreen);
  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = w;
  bmi.bmiHeader.biHeight = -h;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  void* pvBits = nullptr;
  HBITMAP hBitmap =
      CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
  if (hBitmap == NULL) {
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    return;
  }

  HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
  DrawToMemoryDC(hdcMem, w, h, alpha);
  POINT ptWin = {0, 0};
  SIZE sizeWin = {w, h};
  BLENDFUNCTION blend = {AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA};
  UpdateLayeredWindow(hWnd, hdcScreen, &ptWin, &sizeWin, hdcMem, &ptWin, 0,
                      &blend, ULW_ALPHA);
  SelectObject(hdcMem, oldBmp);
  DeleteObject(hBitmap);
  DeleteDC(hdcMem);
  ReleaseDC(NULL, hdcScreen);
}