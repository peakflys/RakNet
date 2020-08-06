/*
*  Copyright (c) 2017-2020, jefftian
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
#ifndef __CLIENT_CONNECTIONS_H
#define __CLIENT_CONNECTIONS_H

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "DS_Map.h"

namespace RakNet
{
	class ClientConnections
	{
	public:
		inline static ClientConnections* GetInstance()
		{
			static ClientConnections conn;
			return &conn;
		}
		~ClientConnections();

		int CreateRakPeerHandler();
		void DestoryRakPeerHandler(int handler);
		StartupResult Startup(int handler);
		ConnectionAttemptResult Connect(int handler, const char* host, unsigned short remotePort, const char *passwordData, unsigned int timeout, unsigned int maxBuffLen);
		void CloseConnection(int handler);
		unsigned Send(int handler, const unsigned char* data, const int dataLen);
		DefaultMessageIDTypes Receive(int handler, unsigned char* buff, int* dataLen);
	private:
		ClientConnections();
		ClientConnections(ClientConnections&);
		ClientConnections& operator=(const ClientConnections&);

		RakPeerInterface* GetPeer(int handler);

		DataStructures::Map<int, RakPeerInterface*> peers;
		int peerHandler;
		int maxBuffLen;
		BitStream bs;
	};
	
}; // namespace RakNet

#endif //__CLIENT_CONNECTIONS_H
