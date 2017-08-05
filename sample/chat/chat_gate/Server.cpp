#include "header.h"
#include "Server.h"

#include "CliSession.h"
#include "SrvSession.h"
CServer theServer;

CServer::CServer()
{

}

CServer::~CServer()
{

}

///
/// \brief ���÷�����
///
/// \author albert.xu
/// \date 2017/08/05
///
xgc_bool CServer::Setup( xgc_lpcstr lpConfigFile )
{
	char conf_file[XGC_MAX_FNAME] = { 0 };
	char conf_path[XGC_MAX_PATH] = { 0 };

	if( xgc_nullptr == get_absolute_path( conf_file, lpConfigFile ) )
	{
		fprintf( stderr, "format conf path error %s", conf_file );
		return false;
	}

	ini_reader ini;
	if( false == ini.load( conf_file ) )
	{
		fprintf( stderr, "conf load error %s", conf_file );
		return false;
	}

	path_dirs( conf_path, conf_file );

	struct net::Param_SetBufferSize param1;
	param1.recv_buffer_size = 64 * 1024;
	param1.send_buffer_size = 64 * 1024;

	if( 0 != net::ExecuteState( Operator_SetBufferSize, &param1 ) )
	{
		return false;
	}

	struct net::Param_SetPacketSize param2;
	param2.recv_packet_size = 64 * 1024;
	param2.send_packet_size = 64 * 1024;

	if( 0 != net::ExecuteState( Operator_SetPacketSize, &param2 ) )
	{
		return false;
	}

	auto addr = ini.get_item_value( "GameServer", "Bind", "0.0.0.0" );
	strcpy_s( mGateAddr, addr );
	auto port = ini.get_item_value( "GameServer", "Port", 40001 );
	mGatePort = port;

	xgc_char user_file[XGC_MAX_FNAME];

	auto path = ini.get_item_value( "GameServer", "User", "user.csv" );
	if( is_absolute_path( path ) )
		strcpy_s( user_file, path );
	else
		get_absolute_path( user_file, "%s/%s", conf_path, path );

	csv_reader csv;
	if( csv.load( user_file, ',', false ) )
	{
		auto rows = csv.get_rows();
		for( decltype( rows ) row = 0; row < rows; ++row )
		{
			Account account;

			account.user_id  = csv.get_value<xgc_uint64>( row, size_t( 0 ), 0 );
			account.username = csv.get_value( row, size_t( 1 ), xgc_nullptr );
			account.password = csv.get_value( row, size_t( 2 ), xgc_nullptr );

			auto ib = mAccountMap.insert( std::make_pair( account.username, std::move( account ) ) );
			XGC_ASSERT( ib.second );
		}
	}

	addr = ini.get_item_value( "GameServer", "ChatAddr", "127.0.0.1" );
	strcpy_s( mChatAddr, addr );
	port = ini.get_item_value( "ChatServer", "SrvPort", 50002 );
	mChatPort = port;

	return true;
}

///
/// \brief ��֤�û�
///
/// \author albert.xu
/// \date 2017/08/05
///
xgc_long CServer::VerificationUser( const xgc_string &username, const xgc_string &password, xgc_uint64 &user_id )
{
	auto it = mAccountMap.find( username );
	if( it == mAccountMap.end() )
		return -1;

	if( it->second.password != password )
		return -2;

	user_id = it->second.user_id;

	return 0;
}

xgc_void CServer::Run()
{
	if( false == net::CreateNetwork( 1 ) )
		return;

	mListener = net::StartServer( mGateAddr, mGatePort, 0, [](){ return XGC_NEW CClientSession(); } );
	if( xgc_nullptr == mListener )
		return;

	auto nOption = NET_CONNECT_OPTION_ASYNC | NET_CONNECT_OPTION_RECONNECT | NET_CONNECT_OPTION_TIMEOUT;
	mChatLink = Connect( mChatAddr, mChatPort, nOption, 3000, this );
	if( mChatLink == INVALID_NETWORK_HANDLE )
		return;

	fprintf( stdout, "gate is running.\n" );
	int n = 0;
	while( mRunning )
	{
		if( net::ProcessNetEvent( 100 ) == 100 )
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}
	}

	net::CloseLink( mChatLink );
	net::CloseServer( mListener );
	net::DestroyNetwork();
}

int main( int argc, char* argv[] )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	auto pLocale = setlocale( LC_ALL, "chs" );
	if( pLocale )
		printf( "locale set with %s\n", pLocale );
	else
		printf( "locale set failed\n" );

	auto pConfigFile = "chat-server.ini";
	if( argc > 1 )
		pConfigFile = argv[1];

	if( false == theServer.Setup( pConfigFile ) )
		return -1;

	theServer.Run();

	return 0;
}