#define WIN32_LEAN_AND_MEAN
#define NOGDI // Disables GDI (removes the Windows 'Rectangle' symbol)
#include "ResourceLoader.h"

#if _WIN32
#include <windows.h>

RawResource LoadResourceData(const char *resourceName)
{
    HRSRC res = FindResource(NULL, resourceName, RT_RCDATA);
    if (!res)
        return (RawResource){0};

    HGLOBAL data = LoadResource(NULL, res);
    return (RawResource){
        .data = LockResource(data),
        .size = SizeofResource(NULL, res)};
}

#else

RawResource LoadResourceData(const char *resourceName)
{
    return (RawResource){0};
}
#endif