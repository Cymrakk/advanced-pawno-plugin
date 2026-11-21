// AdvancedPawno Plugin | Messages Header
// messages.h | made by Cymrakk
// GitHub: https://github.com/Cymrakk/advanced-pawno-plugin
// License: MIT

#pragma once

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

cell AMX_NATIVE_CALL n_SendClientFormatMessage(AMX* amx, cell* params);
cell AMX_NATIVE_CALL n_SendClientFormatMessageToAll(AMX* amx, cell* params);
