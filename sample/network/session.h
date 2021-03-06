#pragma once

#ifndef _NETSESSION_H_ 
#define _NETSESSION_H_ 

class CNetSession : public net::INetworkSession
{
protected:
	/// 网络句柄
	net::network_t handle_;
	/// 网络延迟
	xgc_ulong pingpong_;
	/// 最后一次接收ping消息的时间
	xgc_time64 pinglast_;
	/// 用户数据
	xgc_lpvoid userdata_;
public:
	///
	/// \brief 构造
	///
	/// \author albert.xu
	/// \date 2017/03/14 10:51
	///
	CNetSession();

	///
	/// \brief 析构
	///
	/// \author albert.xu
	/// \date 2017/03/14 10:51
	///
	virtual ~CNetSession();

	///
	/// \brief 获取网络句柄
	///
	/// \author albert.xu
	/// \date 2017/03/14 10:53
	///
	net::network_t GetHandle()const
	{
		return handle_;
	}

	///
	/// \brief 数据包是否
	/// \return	0 ~ 成功, -1 ~ 失败
	///
	virtual int OnParsePacket( const void* data, xgc_size size ) override;

	///
	/// \brief 连接建立
	///
	/// \author albert.xu
	/// \date 2017/02/28 11:09
	///
	virtual xgc_void OnAccept( net::network_t handle ) override;

	///
	/// \brief 连接建立
	///
	/// \author albert.xu
	/// \date 2017/02/28 11:09
	///
	virtual xgc_void OnConnect( net::network_t handle ) override;

	///
	/// \brief 连接错误
	///
	/// \author albert.xu
	/// \date 2017/02/28 11:09
	///
	virtual xgc_void OnError( xgc_int16 error_type, xgc_int16 error_code ) override;

	///
	/// \brief 连接关闭
	///
	/// \author albert.xu
	/// \date 2017/02/28 11:10
	///
	virtual xgc_void OnClose() override;

	///
	/// \brief 网络保活事件
	///
	/// \author albert.xu
	/// \date 2017/03/03 10:41
	///
	virtual xgc_void OnAlive() override;

	///
	/// \brief 接收数据
	///
	/// \author albert.xu
	/// \date 2017/02/28 11:10
	///
	virtual xgc_void OnRecv( xgc_lpvoid data, xgc_size size ) override;

	///
	/// \brief 设置用户数据
	///
	/// \author albert.xu
	/// \date 2017/03/27 11:35
	///
	xgc_void SetUserdata( xgc_lpvoid userdata )
	{
		userdata_ = userdata;
	}

	///
	/// \brief 获取用户数据
	///
	/// \author albert.xu
	/// \date 2017/03/27 11:36
	///
	xgc_lpvoid GetUserdata()
	{
		return userdata_;
	}

	///
	/// \brief 发送消息
	///
	/// \author albert.xu
	/// \date 2017/03/24 17:28
	///
	xgc_void SendPacket( xgc_lpvoid data, xgc_size size );
};


template< class T >
void SendPacket( CNetSession* net, xgc_uint16 type, xgc_uint16 id, T &req )
{
	MessageHeader header;
	header.type = type;
	header.code = id;
	header.length = htons( (xgc_uint16)( sizeof( header ) + sizeof( req ) ) );

	net::SendPacketChains( net->GetHandle(), {
		net::MakeBuffer( header ),
		net::MakeBuffer( req ),
	} );
}

XGC_INLINE void SendPacket( CNetSession* net, xgc_uint16 type, xgc_uint16 id, xgc_lpvoid data, xgc_size size )
{
	MessageHeader header;
	header.type = type;
	header.code = id;
	header.length = htons( xgc_uint16( sizeof( header ) + size ) );

	net::SendPacketChains( net->GetHandle(), {
		net::MakeBuffer( header ),
		net::MakeBuffer( &data, size ),
	} );
}

#endif // _NETSESSION_H_ 

