#if DEBUG
    #define DebugPrint			dbgprintf
#else
    #define DebugPrint(...)		while(0)
#endif
