// clickable link component implementation
#include "a.h"
#include <shellapi.h>
#include <format>

A::A(const Gdiplus::RectF& rect, std::wstring url, std::wstring text)
    : Div(ElementTag::A, rect), url(std::move(url)), text(std::move(text)) {
    this->onClickCallback = [this](HWND hwnd) {
        ShellExecuteW(hwnd, L"open", this->url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    };
};

void A::Draw(Gdiplus::Graphics& graphics, DrawParams& params) {
    // Check if textParams is nullptr, show error message if so
    if (!params.textParams) {
        std::wstring m =
            std::format(L"DrawContext.textParams is nullptr! A::Draw: url='{}', text='{}'",
                        this->url, this->text);
        MessageBoxW(nullptr, m.c_str(), L"Error", MB_ICONERROR);
        return;
    }
    DrawCachedUIText(graphics, *params.textParams);
}