#ifndef __XGC_SYSTEM_H__
#define __XGC_SYSTEM_H__

#include <linux/limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define XGC_MAX_PATH 	PATH_MAX
#define XGC_MAX_FNAME 	NAME_MAX

#define xgc_invalid_handle xgc_nullptr

#define gettid() syscall(SYS_gettid)
#endif // __XGC_SYSTEM_H__