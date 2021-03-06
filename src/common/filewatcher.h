///
/// CopyRight ? 2016 X Studio
/// \file filewatcher.h
/// \date 八月 2016
///
/// \author albert.xu windxu@126.com
/// \brief 文件变更监视辅助
/// 

#pragma once
#ifndef _FILEWATCHER_H_
#define _FILEWATCHER_H_
#include "defines.h"
#include "exports.h"

#include <functional>
namespace xgc
{
	namespace common
	{
		#define FILE_WATCHER_SUCCESS        0
		#define FILE_WATCHER_ERRORUNKNOWN   1
		#define FILE_WATCHER_ERRORNOTINIT   2
		#define FILE_WATCHER_ERROROUTOFMEM  3
		#define FILE_WATCHER_ERROROPENFILE  4
		#define FILE_WATCHER_ERRORADDTOIOCP 5
		#define FILE_WATCHER_ERRORREADDIR   6
		#define FILE_WATCHER_NOCHANGE       7
		#define FILE_WATCHER_ERRORDEQUE     8

		///
		/// \brief 文件监视通知回调，在do_filewatcher_notify中被调用
		///
		/// \author albert.xu
		/// \date 2016/08/23 11:05
		///
		typedef std::function< xgc_void( xgc_lpcstr path, xgc_lpcstr filename, xgc_ulong action ) > filewatch_notifier;

		///
		/// \brief 初始化文件监视器
		///
		/// \prarm thread_count 线程数量
		/// \param thread_interval 线程等待间隔，线程数为0时无效
		///
		/// \author albert.xu
		/// \date 2016/08/10 15:45
		///
		COMMON_API xgc_bool init_filewatcher( xgc_ulong thread_count, xgc_ulong thread_interval = 0xffffffff );

		///
		/// \brief 清理文件监视器
		///
		/// \author albert.xu
		/// \date 2016/08/10 15:45
		///
		COMMON_API xgc_void fini_filewatcher();

		///
		/// \brief 新增一个文件监视器
		///
		/// \author albert.xu
		/// \date 2016/08/10 15:46
		///
		COMMON_API xgc_long add_filewatcher( xgc_lpcstr path, xgc_ulong filter, xgc_bool watch_subtree, const filewatch_notifier &notify_fn, xgc_bool auto_merger = true );

		///
		/// \brief 删除一个文件监视器
		///
		/// \author albert.xu
		/// \date 2016/08/10 15:49
		///
		COMMON_API xgc_void del_filewatcher( xgc_lpcstr path );

		///
		/// \brief 处理已上报的通知
		///
		/// \author albert.xu
		/// \date 2016/08/10 16:37
		///
		COMMON_API xgc_long do_filewatcher_notify( xgc_ulong timeout = 0 );
	}
}

#endif // _FILEWATCHER_H_