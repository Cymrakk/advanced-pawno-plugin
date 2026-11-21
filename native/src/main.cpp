// AdvancedPawno Plugin | Source Code
// main.cpp | made by Cymrakk
// GitHub: https://github.com/Cymrakk/advanced-pawno-plugin
// License: MIT

#include <cstdio>
#include <cstring>
#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

typedef void (*logprintf_t)(const char* format, ...);
logprintf_t logprintf;
extern void *pAMXFunctions;

AMX_NATIVE pfn_format = nullptr;
AMX_NATIVE pfn_SendClientMessage = nullptr;
AMX_NATIVE pfn_SendClientMessageToAll = nullptr;

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

// native SendClientFormatMessage(playerid, color, const fmat[], {Float, bool, _}:...);
cell AMX_NATIVE_CALL n_SendClientFormatMessage(AMX* amx, cell* params)
{
    int num_args = params[0] / sizeof(cell);
    if (num_args < 3) return 0;

    if (!pfn_format) pfn_format = FindNative(amx, "format");
    if (!pfn_SendClientMessage) pfn_SendClientMessage = FindNative(amx, "SendClientMessage");
    if (!pfn_format || !pfn_SendClientMessage) return 0;

    if (num_args == 3)
    {
        cell scm_params[] = { 3 * sizeof(cell), params[1], params[2], params[3] };
        return pfn_SendClientMessage(amx, scm_params);
    }

    cell amx_addr = 0, *phys_addr = nullptr;
    amx_Allot(amx, 144, &amx_addr, &phys_addr);

    cell fmt_params[64];
    int fmt_count = num_args;
    if (fmt_count > 63) fmt_count = 63;

    fmt_params[0] = fmt_count * sizeof(cell);
    fmt_params[1] = amx_addr;      // output[]
    fmt_params[2] = 144;           // len
    fmt_params[3] = params[3];     // const format[]

    for (int i = 4; i <= num_args && i <= 63; i++)
    {
        fmt_params[i] = params[i];
    }

    pfn_format(amx, fmt_params);

    cell scm_params[] = { 3 * sizeof(cell), params[1], params[2], amx_addr };
    cell ret = pfn_SendClientMessage(amx, scm_params);

    amx_Release(amx, amx_addr);
    return ret;
}

// native SendClientFormatMessageToAll(color, const fmat[], {Float, bool, _}:...);
cell AMX_NATIVE_CALL n_SendClientFormatMessageToAll(AMX* amx, cell* params)
{
    int num_args = params[0] / sizeof(cell);
    if (num_args < 2) return 0;

    if (!pfn_format) pfn_format = FindNative(amx, "format");
    if (!pfn_SendClientMessageToAll) pfn_SendClientMessageToAll = FindNative(amx, "SendClientMessageToAll");
    if (!pfn_format || !pfn_SendClientMessageToAll) return 0;

    if (num_args == 2)
    {
        cell scm_params[] = { 2 * sizeof(cell), params[1], params[2] };
        return pfn_SendClientMessageToAll(amx, scm_params);
    }

    cell amx_addr = 0, *phys_addr = nullptr;
    amx_Allot(amx, 144, &amx_addr, &phys_addr);

    cell fmt_params[64];
    int fmt_count = num_args + 1;
    if (fmt_count > 63) fmt_count = 63;

    fmt_params[0] = fmt_count * sizeof(cell);
    fmt_params[1] = amx_addr;
    fmt_params[2] = 144;
    fmt_params[3] = params[2];

    for (int i = 3; i <= num_args && i <= 62; i++)
    {
        fmt_params[i + 1] = params[i];
    }

    pfn_format(amx, fmt_params);

    cell scm_params[] = { 2 * sizeof(cell), params[1], amx_addr };
    cell ret = pfn_SendClientMessageToAll(amx, scm_params);

    amx_Release(amx, amx_addr);
    return ret;
}

AMX_NATIVE_INFO PluginNatives[] =
{
    { "SendClientFormatMessage", n_SendClientFormatMessage },
    { "SendClientFormatMessageToAll", n_SendClientFormatMessageToAll },
    { 0, 0 }
};

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void** ppData)
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
    logprintf("  >> AdvancedPawno Plugin successfully loaded!");
    logprintf("  >> Plugin was made by Cymrakk.");
    logprintf("  >> GitHub: https://github.com/Cymrakk/advanced-pawno-plugin");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
    logprintf("  >> AdvancedPawno Plugin unloaded.");
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
    return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX* amx)
{
    return AMX_ERR_NONE;
}
