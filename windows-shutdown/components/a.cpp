// clickable link component implementation
#include "a.h"

// Draws a clickable link area (simple rectangle and underline)
void A::Draw(Gdiplus::Graphics* graphics, const Gdiplus::RectF& rect, const std::wstring& url) {
    // Draw rectangle border
    // ...existing code for drawing border...
    // Draw underline or highlight (optional)
    // ...existing code for drawing underline...
    // Draw URL text
    // ...existing code for drawing text...
    // (具体绘制细节可根据你的Graphics类实现)
}

// Handles mouse click, returns true if clicked and opened
bool A::OnClick(const POINT& pt, const RECT& rect, const std::wstring& url) {
    // Check if click is inside rect
    if (pt.x >= rect.left && pt.x <= rect.right && pt.y >= rect.top && pt.y <= rect.bottom) {
        // Use CreateProcessW to open the url in default browser
        std::wstring cmd = L"cmd.exe /c start " + url;
        STARTUPINFOW si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        if (CreateProcessW(NULL, (LPWSTR)cmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return true;
        }
    }
    return false;
}

bool A::MouseHit(int mx, int my) {
    return false;
}