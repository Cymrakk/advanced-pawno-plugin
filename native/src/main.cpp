// AdvancedPawno Plugin | Entry Point
// main.cpp | made by Cymrakk
// GitHub: https://github.com/Cymrakk/advanced-pawno-plugin
// License: MIT
#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"
#include "logic/messages.h"
#include "logic/logger.h"

typedef void (*logprintf_t)(const char* format, ...);
logprintf_t logprintf;
extern void* pAMXFunctions;

AMX_NATIVE_INFO PluginNatives[] =
{
    { "SendClientFormatMessage", n_SendClientFormatMessage },
    { "SendClientFormatMessageToAll", n_SendClientFormatMessageToAll },
    { "SendLog", n_SendLog },
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
