// AdvancedPawno Plugin | Logger Source
// logger.cpp | made by Cymrakk
// GitHub: https://github.com
// License: MIT

#define _CRT_SECURE_NO_WARNINGS

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif
#include <fstream>
#include <string>

#include "logger.h"
#include "utils.h"

typedef void (*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;

static AMX_NATIVE pfn_format = nullptr;

static void EnsureDirectories(const std::string& file_path)
{
    size_t pos = 0;
    while ((pos = file_path.find_first_of("/\\", pos)) != std::string::npos)
    {
        std::string dir = file_path.substr(0, pos);
        if (!dir.empty())
        {
        #ifdef _WIN32
            CreateDirectoryA(dir.c_str(), nullptr);
        #else
            mkdir(dir.c_str(), 0755);
        #endif
        }
        pos++;
    }
}

// native SendLog(const file[], type = LOG_TYPE_INFO, bool:console = true, const fmat[], {Float, bool, _}:...);
cell AMX_NATIVE_CALL n_SendLog(AMX* amx, cell* params)
{
    int num_args = params[ 0 ] / sizeof(cell);
    if (num_args < 4) return 0;

    char file_path[ 256 ];
    cell* phys_file = nullptr;
    amx_GetAddr(amx, params[ 1 ], &phys_file);
    amx_GetString(file_path, phys_file, 0, sizeof(file_path));

    if (strlen(file_path) == 0 || strstr(file_path, "..") != nullptr)
        return 0;

    int type = static_cast<int>(params[ 2 ]);
    bool to_console = (params[ 3 ] != 0);

    const char* prefix = "";
    if (type == 1) prefix = "[WARNING] ";
    else if (type == 2) prefix = "[ERROR] ";

    char message[ 1024 ];

    if (num_args == 4)
    {
        cell* phys_msg = nullptr;
        amx_GetAddr(amx, params[ 4 ], &phys_msg);
        amx_GetString(message, phys_msg, 0, sizeof(message));
    }
    else
    {
        if (!pfn_format) pfn_format = FindNative(amx, "format");
        if (!pfn_format) return 0;

        cell amx_addr = 0, *phys_addr = nullptr;
        if (amx_Allot(amx, 1024, &amx_addr, &phys_addr) != AMX_ERR_NONE)
            return 0;

        cell fmt_params[ 64 ];
        int fmt_count = num_args - 1;
        if (fmt_count > 63) fmt_count = 63;

        fmt_params[ 0 ] = fmt_count * sizeof(cell);
        fmt_params[ 1 ] = amx_addr;
        fmt_params[ 2 ] = 1024;
        fmt_params[ 3 ] = params[ 4 ];

        for (int i = 5; i <= num_args && (i - 1) <= 63; i++)
        {
            fmt_params[i - 1] = params[i];
        }

        pfn_format(amx, fmt_params);
        amx_GetString(message, phys_addr, 0, sizeof(message));
        amx_Release(amx, amx_addr);
    }

    if (to_console && logprintf)
    {
        logprintf("%s%s", prefix, message);
    }

    try
    {
        EnsureDirectories(file_path);

        std::ofstream out(file_path, std::ios::app);
        if (!out.is_open()) return 0;

        std::time_t now = std::time(nullptr);
        std::tm* tm_info = std::localtime(&now);
        char time_buf[32]; // <-- проверь, что здесь [32]
        std::strftime(time_buf, sizeof(time_buf), "[%d.%m.%Y %H:%M:%S] ", tm_info);

        out << time_buf << prefix << message << '\n';
        out.close();
    }
    catch (...)
    {
        return 0;
    }

    return 1;
}

