#ifndef WALLPAPERWINDOW_HPP
#define WALLPAPERWINDOW_HPP

#if defined (WIN32)
#include <Windows.h>
typedef HWND WINDOWID;
#else
typedef int WINDOWID;
#endif

#include <string>

WINDOWID findWallpaperLayerWindow(std::string& errmsg);
void leaveWallpaperWindow();

#endif // WALLPAPERWINDOW_HPP
