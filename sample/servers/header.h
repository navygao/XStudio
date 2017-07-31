#pragma once
#ifndef _CLIENT_HEADER_H_ 
#define _CLIENT_HEADER_H_ 

#if defined( _WINDOWS) && defined( _DEBUG )
#	include "vld.h"
#endif

#include "defines.h"
#include "xutility.h"
#include "xsystem.h"
#include "ini_reader.h"
#include "md5.h"

#include "database.h"
#include "net.h"
#include "net_module.h"

#include "curses.h"
#include "panel.h"

using namespace xgc;
using namespace xgc::common;
using namespace xgc::sql;
using namespace xgc::net;

#ifdef _LINUX
#	include <arpa/inet.h>
#	include <endian.h>
#endif

#ifdef _WINDOWS
#	include <WinSock2.h>
#	pragma comment( lib, "ws2_32.lib" )
#endif // _WINDOWS

#include <sys/stat.h>
#include <fcntl.h>

#include <thread>
#include <fstream>
#include <algorithm>

#include "protocol.h"

#include "ServerBase.h"
#endif // _CLIENT_HEADER_H_