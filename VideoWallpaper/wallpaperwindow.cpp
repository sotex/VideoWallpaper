#include "wallpaperwindow.hpp"
#include <QDebug>

#if defined(WIN32)

// 背景窗体句柄
static HWND s_hProgmanWnd = nullptr;
static HWND s_hWorkerWnd = nullptr;

BOOL CALLBACK EnumWindowProcFindDesktopWindow(HWND hTop, LPARAM lparam)
{
    // 查找 SHELLDLL_DefView 窗体句柄
    // 存在多个WorkerW窗体，只有底下存在SHELLDLL_DefView的才是
    HWND hWndShl = ::FindWindowExW(
        hTop, nullptr, L"SHELLDLL_DefView", nullptr);
    if (hWndShl == nullptr) { return true; }

    // XP 直接查找SysListView32窗体
    // g_hWorker = ::FindWindowEx(hWndShl, 0, _T("SysListView32"),_T("FolderView"));

    // win7或更高版本
    // 查找 WorkerW 窗口句柄(以桌面窗口为父窗口)
    s_hWorkerWnd = ::FindWindowExW(nullptr, hTop, L"WorkerW", nullptr);
    return s_hWorkerWnd == nullptr;
}

WINDOWID findWallpaperLayerWindow(std::string& errmsg)
{
    WINDOWID wid = 0;
    // 遍历桌面所有顶层窗口去查找桌面背景窗口句柄
    if (s_hProgmanWnd == nullptr) {
        // https://www.codeproject.com/articles/856020/draw-behind-desktop-icons-in-windows
        // 先找到Progman 窗口
        s_hProgmanWnd = ::FindWindowExW(GetDesktopWindow(), nullptr, L"Progman", L"Program Manager");
        if (s_hProgmanWnd == nullptr) {
            errmsg.assign(u8"找不到Progman窗体");
            qDebug() << errmsg.c_str();
            return wid;
        }
        qDebug()<<s_hProgmanWnd;
        // 发送消息到Program Manager窗口
        // 要在桌面图标和壁纸之间触发创建WorkerW窗口，必须向Program Manager发送一个消息
        // 这个消息没有一个公开的WindowsAPI来执行，只能使用SendMessageTimeout来发送0x052C
        // win10 1903 下未能成功（无法分离）
        DWORD_PTR lpdwResult = 0;
        ::SendMessage(s_hProgmanWnd, 0x052C, 0xD, 0);
        ::SendMessage(s_hProgmanWnd, 0x052C, 0xD, 1);
        //        ::SendMessageTimeoutW(s_hProgmanWnd, 0x052C, 0, 0, SMTO_NORMAL, 1000, &lpdwResult);
        //        ::SendMessageTimeoutW(s_hProgmanWnd, 0x052C, 0, 1, SMTO_NORMAL, 1000, &lpdwResult);

        // 查找到 WorkerW 窗口，设置显示
        EnumWindows(EnumWindowProcFindDesktopWindow, 0);
        // ::ShowWindowAsync(s_hWorkerWnd, SW_HIDE);
        ::ShowWindow(s_hWorkerWnd, SW_NORMAL);
    }
    if(s_hWorkerWnd == nullptr) {
        ::SendMessage(s_hProgmanWnd, 0x052C, 0, 0);
        EnumWindows(EnumWindowProcFindDesktopWindow, 0);
    }
    if(s_hWorkerWnd != nullptr) {
        // 检测是否是 win8 后版本
        OSVERSIONINFOEX osvi;
        DWORDLONG dwlConditionMask = 0;

        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        osvi.dwMajorVersion = 6;
        osvi.dwMinorVersion = 2;

        VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
        VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

        if(!VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask)){
            qDebug()<<" is window 7";
            // 检测到 win7 系统，隐藏 WorkerW
            ShowWindow(s_hWorkerWnd, SW_HIDE);
            s_hWorkerWnd = s_hProgmanWnd;
        }
    }
    if (s_hWorkerWnd == nullptr) {
        errmsg.assign(u8"找不到桌面壁纸层窗体");
        qDebug() << errmsg.c_str();
        return wid;
    }
    wid = s_hWorkerWnd;
    return wid;
}


void leaveWallpaperWindow()
{
    if(s_hProgmanWnd != nullptr) {
        // ::SendMessage(s_hProgmanWnd, 0x052C, 0xD, 0);
        // 发送下面消息后，桌面会重新绘制
        ::SendMessage(s_hProgmanWnd, 0x052C, 0xD, 1);
    }
}


#else

WINDOWID findWallpaperLayerWindow(std::string& errmsg)
{
    return -1;
}

void leaveWallpaperWindow()
{

}
#endif

