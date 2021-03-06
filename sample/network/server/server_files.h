#pragma once
#ifndef _SERVER_FILE_H_ 
#define _SERVER_FILE_H_ 
class CServerFiles
{
friend CServerFiles& getServerFiles();

private:
	struct fileinfo
	{
		int fd;
		/// 文件路径
		xgc_char path[XGC_MAX_PATH];
		/// 文件名
		xgc_char name[XGC_MAX_FNAME];
		/// 文件序号
		xgc_uint32 sequence;
		/// 文件大小
		xgc_uint64 size;
		/// 文件缓冲区
		xgc_char file[1];
	};

	/// 文件映射表 - 序列号
	std::unordered_map< xgc_uint32, fileinfo* > files_seq_;

	/// 文件映射表 - 文件名
	std::unordered_map< xgc::string, xgc_uint32 > files_;
	/// 文件序列号
	xgc_uint32 sequence_;

	/// 文件忽略列表
	std::list< xgc::string > ignore_files;
	
private:
	CServerFiles();
	~CServerFiles();

public:
	///
	/// \brief 生成忽略列表
	///
	/// \author albert.xu
	/// \date 2017/03/30 18:03
	///
	xgc_long GenIgnoreList( xgc_lpcstr root );

	///
	/// \brief 是否是忽略的文件
	///
	/// \author albert.xu
	/// \date 2017/03/30 18:25
	///
	xgc_bool IsIgnoreFile( xgc_lpcstr path, xgc_lpcstr name );

	///
	/// \brief 生成文件列表
	///
	/// \return file sequence
	///
	/// \author albert.xu
	/// \date 2017/03/27 15:25
	///
	xgc_long GenFileList( xgc_lpcstr root );

	///
	/// \brief 获取文件信息
	///
	/// \return file sequence
	///
	/// \author albert.xu
	/// \date 2017/03/27 15:25
	///
	xgc_uint32 GetFileInfo( xgc_lpcstr path, xgc_lpcstr name, xgc_uint64 *length = xgc_nullptr );

	///
	/// \brief 获取文件数据
	///
	/// \author albert.xu
	/// \date 2017/03/27 15:25
	///
	xgc_int32 GetFileData( xgc_uint32 sequence, xgc_int64 offset, xgc_lpvoid buffer, xgc_int32 length );
};

CServerFiles& getServerFiles();
#endif // _SERVER_FILE_H_ 

