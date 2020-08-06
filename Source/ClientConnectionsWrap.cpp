/*
*  Copyright (c) 2017-2020 jefftian
*  All rights reserved.
*
*  This source code is licensed under the BSD-style license found in the
*  LICENSE file in the root directory of this source tree. An additional grant
*  of patent rights can be found in the PATENTS file in the same directory.
*
*  author jefftian
*  create time Mon Nov 13 11:28:42 CST 2017
*
*/

#ifndef RAKEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define RAKEXPORT
#   else
#     define RAKEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define RAKEXPORT __attribute__ ((visibility("default")))
#   else
#     define RAKEXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef RAKSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define RAKSTDCALL __stdcall
# else
#   define RAKSTDCALL
# endif
#endif

#include "ClientConnections.h"
using namespace RakNet;

#ifdef __cplusplus
extern "C" {
#endif

	RAKEXPORT int RAKSTDCALL CreateRakPeerHandler() 
	{
		return ClientConnections::GetInstance()->CreateRakPeerHandler();
	}

	RAKEXPORT void RAKSTDCALL DestoryRakPeerHandler(int handler)
	{
		return ClientConnections::GetInstance()->DestoryRakPeerHandler(handler);
	}

	RAKEXPORT StartupResult RAKSTDCALL Startup(int handler)
	{
		return ClientConnections::GetInstance()->Startup(handler);
	}

	RAKEXPORT ConnectionAttemptResult RAKSTDCALL Connect(int handler, const char* host, unsigned short remotePort, const char* passwordData, unsigned int timeout, unsigned int maxBuffLen)
	{
		return ClientConnections::GetInstance()->Connect(handler, host, remotePort, passwordData, timeout, maxBuffLen);
	}

	RAKEXPORT void RAKSTDCALL CloseConnection(int handler)
	{
		return ClientConnections::GetInstance()->CloseConnection(handler);
	}

	RAKEXPORT unsigned RAKSTDCALL Send(int handler, const unsigned char* data, const int dataLen)
	{
		return ClientConnections::GetInstance()->Send(handler, data, dataLen);
	}

	RAKEXPORT DefaultMessageIDTypes RAKSTDCALL Receive(int handler, unsigned char* buff, int* dataLen)
	{
		return ClientConnections::GetInstance()->Receive(handler, buff, dataLen);
	}

#ifdef __cplusplus
}
#endif
