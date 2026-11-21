// AdvancedPawno Plugin | Logger Source
// logger.cpp | made by Cymrakk
// GitHub: https://github.com
// License: MIT

#define _CRT_SECURE_NO_WARNINGS

#include "logger.h"
#include "utils.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>

typedef void (*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;

static AMX_NATIVE pfn_format = nullptr;

// native SendLog(const file[], type = LOG_TYPE_INFO, bool:console = true, const fmat[], {Float, bool, _}:...);
cell AMX_NATIVE_CALL n_SendLog(AMX* amx, cell* params)
{
    int num_args = params[ 0 ] / sizeof(cell);
    if (num_args < 4) return 0;

    // 1. Получаем путь к файлу
    char file_path[ 256 ];
    cell* phys_file = nullptr;
    amx_GetAddr(amx, params[ 1 ], &phys_file);
    amx_GetString(file_path, phys_file, 0, sizeof(file_path));

    if (strlen(file_path) == 0 || strstr(file_path, "..") != nullptr)
        return 0;

    // 2. Тип лога и флаг вывода в консоль
    int type = static_cast<int>(params[ 2 ]);
    bool to_console = (params[ 3 ] != 0);

    const char* prefix = "";
    if (type == 1) prefix = "[WARNING] ";
    else if (type == 2) prefix = "[ERROR] ";

    // 3. Форматируем строку
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
        fmt_params[ 1 ] = amx_addr;      // output[]
        fmt_params[ 2 ] = 1024;          // len
        fmt_params[ 3 ] = params[ 4 ];   // const fmat[]

        for (int i = 5; i <= num_args && (i - 1) <= 63; i++)
        {
            fmt_params[i - 1] = params[i];
        }

        pfn_format(amx, fmt_params);
        amx_GetString(message, phys_addr, 0, sizeof(message));
        amx_Release(amx, amx_addr);
    }

    // 4. Вывод в консоль сервера
    if (to_console && logprintf)
    {
        logprintf("%s%s", prefix, message);
    }

    // 5. Автоматическое создание папок и запись в файл
    try
    {
        std::filesystem::path p(file_path);
        if (p.has_parent_path())
        {
            std::filesystem::create_directories(p.parent_path());
        }

        std::ofstream out(p, std::ios::app);
        if (!out.is_open()) return 0;

        std::time_t now = std::time(nullptr);
        std::tm* tm_info = std::localtime(&now);
        char time_buf[ 32 ];
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
