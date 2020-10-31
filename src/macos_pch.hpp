#if DEBUG
	#include <os/log.h>
	#define DebugPrint(format, ...)	os_log_with_type(OS_LOG_DEFAULT, OS_LOG_TYPE_DEFAULT, format, ##__VA_ARGS__)
#else
	#define DebugPrint(...)		while(0)
#endif
