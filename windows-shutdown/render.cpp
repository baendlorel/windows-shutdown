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
    std::wstring actionText;

    if (appState.isSleepCountdown) {
      actionText = L"Sleeping in ";
    } else if (appState.isRestartCountdown) {
      actionText = L"Restarting in ";
    } else {
      actionText = L"Shutting down in ";
    }

    std::wstring fullText = actionText + countdownText + L" seconds...";

    // Large font for countdown
    FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, COUNT_DOWN_FONT_SIZE, FontStyleBold);

    // Get text bounds
    RectF layoutRect(0, 0, (REAL)w, (REAL)h);
    RectF boundingBox;
    graphics.MeasureString(fullText.c_str(), -1, &font, layoutRect,
                           &boundingBox);

    // Center the text
    REAL x = (w - boundingBox.Width) / 2;
    REAL y = (h - boundingBox.Height) / 2;

    // Draw main text with beautiful rendering
    DrawBeautifulText(graphics, fullText.c_str(), font, PointF(x, y));

    // Draw cancel instruction with beautiful rendering
    Gdiplus::Font smallFont(&fontFamily, INSTRUCTION_FONT_SIZE,
                            FontStyleRegular);
    std::wstring cancelText = L"Click anywhere or press any key to cancel";
    RectF cancelBounds;
    graphics.MeasureString(cancelText.c_str(), -1, &smallFont, layoutRect,
                           &cancelBounds);
    REAL cancelX = (w - cancelBounds.Width) / 2;
    REAL cancelY = y + boundingBox.Height + 20;

    // Draw cancel text with beautiful rendering
    DrawBeautifulText(graphics, cancelText.c_str(), smallFont,
                     PointF(cancelX, cancelY), 
                     Color(255, 220, 220, 220), Color(100, 0, 0, 0));
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
    
    // Draw instruction text below buttons
    FontFamily fontFamily(L"Arial");
    Gdiplus::Font instructionFont(&fontFamily, INSTRUCTION_FONT_SIZE,
                                  FontStyleRegular);
    std::wstring instructionText = L"Press any key or click background to exit";
    
    RectF layoutRect(0, 0, (REAL)w, (REAL)h);
    RectF textBounds;
    graphics.MeasureString(instructionText.c_str(), -1, &instructionFont, layoutRect, &textBounds);
    
    // Position text below the buttons
    REAL textX = (w - textBounds.Width) / 2;
    REAL textY = (h / 2) + BUTTON_RADIUS + 40; // Below buttons with some margin
    
    // Draw text with beautiful rendering
    DrawBeautifulText(graphics, instructionText.c_str(), instructionFont, 
                     PointF(textX, textY), 
                     Color(255, 200, 200, 200), Color(80, 0, 0, 0));
  }
}

// 新的漂亮文字绘制函数 - 使用阴影效果替代粗糙的轮廓
void DrawBeautifulText(Graphics& graphics, const wchar_t* text,
                      const Gdiplus::Font& font, const PointF& position,
                      const Color& textColor, const Color& shadowColor) {
  // 设置高质量渲染
  graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
  graphics.SetSmoothingMode(SmoothingModeAntiAlias);
  graphics.SetCompositingQuality(CompositingQualityHighQuality);
  
  // 创建画刷
  SolidBrush shadowBrush(shadowColor);
  SolidBrush textBrush(textColor);
  
  // 绘制阴影 (右下偏移)
  PointF shadowPos(position.X + 2, position.Y + 2);
  graphics.DrawString(text, -1, &font, shadowPos, &shadowBrush);
  
  // 绘制主文字
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