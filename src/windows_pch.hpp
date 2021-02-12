#if DEBUG
    #define DebugPrint			dbgprintf
#else
    #define DebugPrint(...)		while(0)
#endif

#define _WIN32_WINNT 0x0601
