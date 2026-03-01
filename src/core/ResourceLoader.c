#define WIN32_LEAN_AND_MEAN
#define NOGDI             // Disables GDI (removes the Windows 'Rectangle' symbol)

#include <windows.h>
#include "ResourceLoader.h"

RawResource LoadResourceData(const char* resourceName) {
    HRSRC res = FindResource(NULL, resourceName, RT_RCDATA);
    if (!res) return (RawResource){0};

    HGLOBAL data = LoadResource(NULL, res);
    return (RawResource){
        .data = LockResource(data),
        .size = SizeofResource(NULL, res)
    };
}