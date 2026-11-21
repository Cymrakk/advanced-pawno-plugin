// AdvancedPawno Plugin | Messages Source
// messages.cpp | made by Cymrakk
// GitHub: https://github.com
// License: MIT

#include <cstdint>
#include "messages.h"
#include "utils.h"

static AMX_NATIVE pfn_format = nullptr;
static AMX_NATIVE pfn_SendClientMessage = nullptr;
static AMX_NATIVE pfn_SendClientMessageToAll = nullptr;

// native SendClientFormatMessage(playerid, color, const fmat[], {Float, bool, _}:...);
cell AMX_NATIVE_CALL n_SendClientFormatMessage(AMX* amx, cell* params)
{
    int num_args = params[ 0 ] / sizeof(cell);
    if (num_args < 3) return 0;

    if (!pfn_format) pfn_format = FindNative(amx, "format");
    if (!pfn_SendClientMessage) pfn_SendClientMessage = FindNative(amx, "SendClientMessage");
    if (!pfn_format || !pfn_SendClientMessage) return 0;

    if (num_args == 3)
    {
        cell scm_params[] = { 3 * sizeof(cell), params[ 1 ], params[ 2 ], params[ 3 ] };
        return pfn_SendClientMessage(amx, scm_params);
    }

    cell amx_addr = 0, *phys_addr = nullptr;
    if (amx_Allot(amx, 144, &amx_addr, &phys_addr) != AMX_ERR_NONE)
        return 0;

    cell fmt_params[ 64 ];
    int fmt_count = num_args;
    if (fmt_count > 63) fmt_count = 63;

    fmt_params[ 0 ] = fmt_count * sizeof(cell);
    fmt_params[ 1 ] = amx_addr;
    fmt_params[ 2 ] = 144;
    fmt_params[ 3 ] = params[ 3 ];

    for (int i = 4; i <= num_args && i <= 63; i++)
    {
        fmt_params[i] = params[i];
    }

    pfn_format(amx, fmt_params);

    cell scm_params[] = { 3 * sizeof(cell), params[ 1 ], params[ 2 ], amx_addr };
    cell ret = pfn_SendClientMessage(amx, scm_params);

    amx_Release(amx, amx_addr);
    return ret;
}

// native SendClientFormatMessageToAll(color, const fmat[], {Float, bool, _}:...);
cell AMX_NATIVE_CALL n_SendClientFormatMessageToAll(AMX* amx, cell* params)
{
    int num_args = params[ 0 ] / sizeof(cell);
    if (num_args < 2) return 0;

    if (!pfn_format) pfn_format = FindNative(amx, "format");
    if (!pfn_SendClientMessageToAll) pfn_SendClientMessageToAll = FindNative(amx, "SendClientMessageToAll");
    if (!pfn_format || !pfn_SendClientMessageToAll) return 0;

    if (num_args == 2)
    {
        cell scm_params[] = { 2 * sizeof(cell), params[ 1 ], params[ 2 ] };
        return pfn_SendClientMessageToAll(amx, scm_params);
    }

    cell amx_addr = 0, *phys_addr = nullptr;
    if (amx_Allot(amx, 144, &amx_addr, &phys_addr) != AMX_ERR_NONE)
        return 0;

    cell fmt_params[ 64 ];
    int fmt_count = num_args + 1;
    if (fmt_count > 63) fmt_count = 63;

    fmt_params[ 0 ] = fmt_count * sizeof(cell);
    fmt_params[ 1 ] = amx_addr;
    fmt_params[ 2 ] = 144;
    fmt_params[ 3 ] = params[ 2 ];

    for (int i = 3; i <= num_args && i <= 62; i++)
    {
        fmt_params[i + 1] = params[i];
    }

    pfn_format(amx, fmt_params);

    cell scm_params[] = { 2 * sizeof(cell), params[ 1 ], amx_addr };
    cell ret = pfn_SendClientMessageToAll(amx, scm_params);

    amx_Release(amx, amx_addr);
    return ret;
}
