// clickable link component implementation
#include "a.h"

A::A(const Gdiplus::RectF& rect, std::string url, std::wstring text)
    : Div(ElementTag::A, rect), url(std::move(url)), text(std::move(text)) {
    this->onClickCallback = [this](HWND hwnd) {
        ShellExecuteA(hwnd, "open", this->url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    };
};

// Draws a clickable link area (simple rectangle and underline)
void A::Draw(const Gdiplus::Graphics* graphics, DrawContext& ctx) {
    // Draw rectangle border
    // ...existing code for drawing border...
    // Draw underline or highlight (optional)
    // ...existing code for drawing underline...
    // Draw URL text
    // ...existing code for drawing text...
    // (具体绘制细节可根据你的Graphics类实现)
}