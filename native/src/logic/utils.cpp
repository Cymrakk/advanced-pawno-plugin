// AdvancedPawno Plugin | Utils Source
// utils.cpp | made by Cymrakk
// GitHub: https://github.com/Cymrakk/advanced-pawno-plugin
// License: MIT

#include "utils.h"

AMX_NATIVE FindNative(AMX* amx, const char* name)
{
    int index = 0;
    if (amx_FindNative(amx, name, &index) == AMX_ERR_NONE)
    {
        AMX_HEADER* hdr = (AMX_HEADER*)amx->base;
        AMX_FUNCSTUB* func = (AMX_FUNCSTUB*)((unsigned char*)hdr + hdr->natives + index * hdr->defsize);
        return (AMX_NATIVE)func->address;
    }
    return nullptr;
}
