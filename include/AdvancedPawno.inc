// AdvancedPawno Plugin (APP)
// AdvancedPawno.inc  |  made by Cymrakk
// GitHub: https://github.com/Cymrakk/advanced-pawno-plugin
// License: MIT

#if defined _advanced_pawno_included
    #endinput
#endif
#define _advanced_pawno_included

#if !defined _samp_included
    #error "Please include <a_samp> before <AdvancedPawno>"
#endif

// 0.3.7 SendClientFormatMessage
native SendClientFormatMessage(playerid, color, const fmat[], {Float, bool, _}:...);

// 0.3.7 SendClientFormatMessageToAll
native SendClientFormatMessageToAll(color, const fmat[], {Float, bool, _}:...);
