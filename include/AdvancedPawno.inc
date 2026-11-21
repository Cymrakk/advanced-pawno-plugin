// AdvancedPawno Plugin (APP)
// AdvancedPawno.inc  |  made by Cymrakk
// GitHub: https://github.com/Cymrakk/advanced-pawno-plugin
// License: MIT

#if defined _advancedpawno_included
    #endinput
#endif
#define _advancedpawno_included

#define LOG_TYPE_INFO    (0)
#define LOG_TYPE_WARNING (1)
#define LOG_TYPE_ERROR   (2)

native SendClientFormatMessage(playerid, color, const fmat[], {Float, bool, _}:...);
native SendClientFormatMessageToAll(color, const fmat[], {Float, bool, _}:...);
native SendLog(const file[], type = LOG_TYPE_INFO, bool:console = true, const fmat[], {Float, bool, _}:...);
