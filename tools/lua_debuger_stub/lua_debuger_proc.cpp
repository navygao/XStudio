#include "lua_debuger.h"
#include "lua_debuger_sock.h"
#include "lua_debuger_proc.h"


/// 栈帧信息
struct stackframe
{
	int		line;
	string	file;
	string	func;
	string	what;
};

// 设置断点
bool cmd_bp( lua_State* L, int argc, char *argv[] );
// 清除断点
bool cmd_bc( lua_State* L, int argc, char *argv[] );
// 列出断点
bool cmd_bl( lua_State* L, int argc, char *argv[] );
// 列出堆栈
bool cmd_bt( lua_State* L, int argc, char *argv[] );

// 单步跟踪
bool cmd_stp( lua_State* L, int argc, char *argv[] );
// 跟进
bool cmd_sti( lua_State* L, int argc, char *argv[] );
// 跟出
bool cmd_sto( lua_State* L, int argc, char *argv[] );
// 监视变量
bool cmd_var( lua_State* L, int argc, char *argv[] );
// 清除监视
bool cmd_clr( lua_State* L, int argc, char *argv[] );
// 继续运行
bool cmd_run( lua_State* L, int argc, char *argv[] );

// 查看变量 - 局部
bool cmd_loc( lua_State* L, int argc, char *argv[] );

// 查看变量 - 全局
bool cmd_glb( lua_State* L, int argc, char *argv[] );

// 使用空格分隔的参数列表，若参数中带空格，则参数需使用引号（""）
long parse_cmd( char* cmd, long argc, char** argv )
{
	long icnt = 0;

	bool quot = false;
	bool find = true;

	while( *cmd && icnt < argc )
	{
		if( find )
		{
			if( cmd[0] == '"' && quot == true )
			{
				cmd[0] = '\0';
				quot = false;
				find = false;
			}

			if( cmd[0] == ' ' && quot == false )
			{
				cmd[0] = '\0';
				find = false;
			}
		}
		else if( cmd[0] == '"' )
		{
			find = true;
			quot = true;

			argv[icnt] = cmd + 1;
		}
		else if( cmd[0] != ' ' )
		{
			find = true;
			argv[icnt] = cmd;
		}

		++cmd;
	}

	return icnt;
}

/// 执行命令
bool execute_cmd( lua_State* L )
{
	// 定义命令缓冲
	char cmd[1024], *ptr = cmd;
	
	// 缓冲长度不够的，则重新分配
	if( dbg.ipkg > sizeof( cmd ) )
	{
		if( NULL == ( ptr = (char*)malloc( dbg.ipkg ) ) )
		{
			resp( "error memory bad_alloc. size = %d", dbg.ipkg.load() );
			// wait next command
			return true;
		}
	}

	// 跳过数据头
	memcpy( cmd, dbg.recv_buffer + sizeof( int ), dbg.ipkg - sizeof( int ) );

	char *argv[32];
	long argc = XGC_COUNTOF(argv);
	bool bret = true;

	argc = parse_cmd( cmd, argc, argv );

	// 处理指令
	if( strcasecmp( "bp", argv[0] ) == 0 )
		bret =  cmd_bp( L, argc, argv );
	else if( strcasecmp( "bl" , argv[0] ) == 0 )
		bret =  cmd_bl( L, argc, argv );
	else if( strcasecmp( "bc" , argv[0] ) == 0 )
		bret =  cmd_bc( L, argc, argv );
	else if( strcasecmp( "bt" , argv[0] ) == 0 )
		bret =  cmd_bt( L, argc, argv );
	else if( strcasecmp( "stp", argv[0] ) == 0 )
		bret =  cmd_stp( L, argc, argv );
	else if( strcasecmp( "sti", argv[0] ) == 0 )
		bret =  cmd_sti( L, argc, argv );
	else if( strcasecmp( "sto", argv[0] ) == 0 )
		bret =  cmd_sto( L, argc, argv );
	else if( strcasecmp( "var", argv[0] ) == 0 )
		bret =  cmd_var( L, argc, argv );
	else if( strcasecmp( "clr", argv[0] ) == 0 )
		bret =  cmd_clr( L, argc, argv );
	else if( strcasecmp( "run", argv[0] ) == 0 )
		bret =  cmd_run( L, argc, argv );
	else if( strcasecmp( "loc", argv[0] ) == 0 )
		bret =  cmd_loc( L, argc, argv );
	else if( strcasecmp( "glb", argv[0] ) == 0 )
		bret =  cmd_glb( L, argc, argv );

	if( ptr != cmd )
		free( ptr );

	return bret;
}

bool cmd_bp( lua_State* L, int argc, char *argv[] )
{
	int line = str2numeric< int >( argv[1] );
	auto ln = dbg.breakpoints_reg[line];
	auto it = ln.find( argv[2] );
	if( it == ln.end() )
	{
		ln.insert( argv[2] );
		dbg.breakpoints_ids[dbg.breakpoints_ids_max] = std::make_tuple( line, argv[2] );
	}

	resp( "status ok\n" );

	return true;
}

bool cmd_bc( lua_State* L, int argc, char *argv[] )
{
	int id = str2numeric< int >( argv[1] );
	auto it = dbg.breakpoints_ids.find( id );
	if( it != dbg.breakpoints_ids.end() )
	{
		auto ln = dbg.breakpoints_reg.find( std::get< 0 >( it->second ) );
		if( ln != dbg.breakpoints_reg.end() )
		{
			auto &file = std::get< 1 >( it->second );
			ln->second.erase( file );
		}
	}

	resp( "status ok\n" );
	return true;
}

bool cmd_bl( lua_State* L, int argc, char *argv[] )
{
	for( auto &it : dbg.breakpoints_ids )
		resp( "%02d - %s:%d", it.first, std::get< 1 >( it.second ).c_str(), std::get< 0 >( it.second ) );

	resp( "status ok\n" );
	return true;
}

bool cmd_bt( lua_State* L, int argc, char *argv[] )
{
	lua_Debug frame;
	int level = 1;
	while( lua_getstack( L, level, &frame ) )
	{
		lua_getinfo( L, "Sln", &frame );

		int line = frame.currentline;
		const char *func = frame.name;
		const char *file = frame.source;
		const char *what = frame.what;

		resp( "%s(%d): %s", file, line, func ? func : "?" );
		++level;
	}
	
	resp( "status ok\n" );
	return true;
}

bool cmd_stp( lua_State* L, int argc, char *argv[] )
{
	dbg.mode = debug_mode::e_step;
	return false;
}

bool cmd_sti( lua_State* L, int argc, char *argv[] )
{
	dbg.mode = debug_mode::e_step_in;
	return false;
}

bool cmd_sto( lua_State* L, int argc, char *argv[] )
{
	dbg.mode = debug_mode::e_step_out;
	return false;
}

bool cmd_var( lua_State* L, int argc, char *argv[] )
{
	if( argc > 1 ) dbg.watchs.insert( argv[1] );
	return false;
}

bool cmd_clr( lua_State* L, int argc, char *argv[] )
{
	if( argc > 1 ) dbg.watchs.erase( argv[1] );
	return false;
}

bool cmd_run( lua_State* L, int argc, char *argv[] )
{
	dbg.mode = debug_mode::e_run;
	return true;
}

// 查看变量 - 局部
bool cmd_loc( lua_State* L, int argc, char *argv[] )
{
	return false;
}

// 查看变量 - 全局
bool cmd_glb( lua_State* L, int argc, char *argv[] )
{
	return false;
}

//void LuaDebuger::makestack( lua_State *L, lua_Debug *ar )
//{
//	m_pImpl->lstack.L = L;
//	m_pImpl->stop_level = m_pImpl->call_level;
//
//	char szFull[_MAX_DIR+_MAX_PATH+_MAX_FNAME+_MAX_EXT];
//	for ( int level = 0; lua_getstack(L, level, ar ); level++)
//	{
//		lua_getinfo( L, "Slnu", ar );
//		Impl::stackframe* sf = new Impl::stackframe();
//		sf->currentline	= ar->currentline;
//		sf->funcname	= ar->name?XA2T( ar->name ):_T("");
//		if( ar->source[0] != '@' || _fullpath(szFull, ar->source+1, _countof(szFull) ) == 0 ) szFull[0] = 0;
//		sf->filename	= XA2T(szFull);
//		std::transform( sf->filename.begin(), sf->filename.end(), sf->filename.begin(), tolower );
//		sf->what		= ar->what?XA2T( ar->what ):_T("");
//		const char* varname		= NULL;
//		std::string varvalue;
//		for( int index = 1; varname = lua_getlocal( L, ar, index ); ++index )
//		{
//			int top = lua_gettop(L);
//			int t = lua_type( L, top );
//			switch( t )
//			{
//			case LUA_TBOOLEAN:
//				varvalue = lua_toboolean( L, top )?"true":"false";
//				break;
//			case LUA_TNUMBER:
//				varvalue = lua_tostring( L, top );
//				break;
//			case LUA_TSTRING:
//				varvalue = std::string("\"") + lua_tostring( L, top ) + "\"";
//				break;
//			case LUA_TTABLE:
//				varvalue = "table";
//				break;
//			case LUA_TUSERDATA:
//				varvalue = "userdata";
//				break;
//			case LUA_TLIGHTUSERDATA:
//				varvalue = "lightuserdata";
//				break;
//			case LUA_TNIL:
//				varvalue = "nil";
//				break;
//			case LUA_TFUNCTION:
//				varvalue = "function";
//				break;
//			case LUA_TTHREAD:
//				varvalue = "thread";
//				break;
//			}
//			sf->variants.push_back( Impl::variant( varname, varvalue.c_str(), index ) );
//			lua_pop(L,1);
//		}
//		m_pImpl->lstack.push_back( sf );
//	}
//
//	// 输出当前行
//	Impl::stackframe* sf = m_pImpl->lstack.front();
//	if( sf )
//	{
//		m_pImpl->begin = sf->currentline;
//		m_pImpl->strFilename = sf->filename;
//		autolock _l( m_pImpl->breakmap_lock );
//
//		Impl::break_map::const_iterator c = m_pImpl->breakpoints.find( sf->filename );
//		if( c != m_pImpl->breakpoints.end() )
//		{
//			if( sf->currentline >= 0 && sf->currentline < (int)c->second.file.size() )
//			{
//				output( _T("@break \"%s\" %d"), sf->filename.c_str(), sf->currentline );
//				output( _T("$%04d > %s"), sf->currentline, c->second.file[sf->currentline].c_str() );
//			}
//		}
//	}
//}

