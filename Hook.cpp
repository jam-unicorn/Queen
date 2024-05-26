#include "hook.h"
#include <windows.h>
#include <iostream>
#include "trayicon.h"
#include <QDebug>

static HHOOK mouseHook = nullptr;
static auto hook = std::make_unique<Hook>();

Hook::Hook(QObject *parent)
    : QObject{parent}
{

}

void moveToMousePosition(QWidget *window, int x, int y)
{
    // 手动调整的偏移
    x += 16;
    y += 20;
    // 获取系统的 DPI 缩放比例
    HDC screen = GetDC(NULL);
    int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
    int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
    ReleaseDC(NULL, screen);

    float scaleFactorX = dpiX / 96.0;
    float scaleFactorY = dpiY / 96.0;

    // 调整鼠标坐标以考虑 DPI 缩放
    int adjustedX = static_cast<int>(x / scaleFactorX);
    int adjustedY = static_cast<int>(y / scaleFactorY);

    // 调整窗口位置以考虑窗口边框和标题栏
    RECT windowRect;
    GetWindowRect((HWND)window->winId(), &windowRect);
    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    // 移动窗口到指定位置
    window->move(adjustedX - windowWidth / 2, adjustedY - windowHeight / 2);
    SetWindowPos((HWND)window->winId(), HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

void CtrlC(){
    INPUT inputs[4] = {};

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'C';

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'C';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    Sleep(100);
}

// 鼠标钩子
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    LPMSLLHOOKSTRUCT p = (LPMSLLHOOKSTRUCT)lParam;
    POINT   pt = p->pt;

    static POINT begin;
    static POINT end;
    if (nCode >= 0)
    {
        if (wParam == WM_LBUTTONDOWN)
        {
            if (!TrayIcon::Instance()->getMainWidget()->underMouse())
            {
                TrayIcon::Instance()->hideMainWidget();
            }
            begin = pt;
        }
        else if (wParam == WM_MOUSEHWHEEL || wParam == WM_MOUSEWHEEL){
            TrayIcon::Instance()->hideMainWidget();
        }
        else if (wParam == WM_LBUTTONUP)
        {
            end = pt;
            int deltaX = end.x - begin.x;
            int deltaY = end.y - begin.y;
            double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
            if (distance > 0)
            {
                std::string previousClipboardText;

                if (OpenClipboard(NULL))
                {
                    if (IsClipboardFormatAvailable(CF_TEXT))
                    {
                        HANDLE hData = GetClipboardData(CF_TEXT);
                        if (hData)
                        {
                            char* pszText = static_cast<char*>(GlobalLock(hData));
                            if (pszText)
                            {
                                previousClipboardText = pszText;
                                GlobalUnlock(hData);
                            }
                            EmptyClipboard();
                        }
                    }
                    CloseClipboard();
                }
                CtrlC();
                if (OpenClipboard(NULL))
                {
                    if (IsClipboardFormatAvailable(CF_TEXT))
                    {
                        HANDLE hData = GetClipboardData(CF_TEXT);
                        if (hData != NULL)
                        {
                            char* pszText = static_cast<char*>(GlobalLock(hData));
                            QString text = pszText;
                            GlobalUnlock(hData);
                            if(!text.trimmed().isEmpty()){
                                hook->setText(text);
                                moveToMousePosition(TrayIcon::Instance()->getMainWidget(), pt.x, pt.y);
                                TrayIcon::Instance()->showMainWidget();
                            }
                            EmptyClipboard();

                            if (!previousClipboardText.empty())
                            {
                                HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, previousClipboardText.size() + 1);
                                if (hGlob)
                                {
                                    memcpy(GlobalLock(hGlob), previousClipboardText.c_str(), previousClipboardText.size() + 1);
                                    GlobalUnlock(hGlob);
                                    SetClipboardData(CF_TEXT, hGlob);
                                }
                            }
                        }
                    }
                    CloseClipboard();
                }
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void Hook::setText(QString& text){
    this->text = text;
}

QString Hook::getText() const{
    return this->text;
}

void Hook::installHook(){
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0);
    hook.reset(this);
}

void Hook::uninstallHook(){
    UnhookWindowsHookEx(mouseHook);
    hook = nullptr;
}

void Hook::sendSignal(bool flag){
    emit setWindowOpen(flag);
}
