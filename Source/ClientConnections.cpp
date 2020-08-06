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

#include "ClientConnections.h"

using namespace RakNet;

ClientConnections::ClientConnections() : peerHandler(0), maxBuffLen(0)
{
}

ClientConnections::~ClientConnections()
{
	int size = peers.Size();
	if (0 == size)
		return;
	for (int i = 0; i < size; ++i)
		RakPeerInterface::DestroyInstance(peers[i]);
	peers.Clear();
}

int ClientConnections::CreateRakPeerHandler()
{
	RakPeerInterface* peer = RakPeerInterface::GetInstance();
	if (!peer)
		return -1;
	++peerHandler;
	if (peerHandler < 0)
		peerHandler = 1;
	if (peers.Has(peerHandler))
	{
		RakPeerInterface::DestroyInstance(peer);
		return -1;
	}
	peers.SetNew(peerHandler, peer);
	return peerHandler;
}

void ClientConnections::DestoryRakPeerHandler(int handler)
{
	RakPeerInterface* peer = GetPeer(handler);
	if (!peer)
		return;
	RakPeerInterface::DestroyInstance(peer);
	peers.Delete(handler);
}

StartupResult ClientConnections::Startup(int handler)
{
	RakPeerInterface* peer = GetPeer(handler);
	if (!peer)
		return INVALID_SOCKET_DESCRIPTORS;
	SocketDescriptor sd;
	//use simple 1:1 socket
	return peer->Startup(1, &sd, 1); 
}

ConnectionAttemptResult ClientConnections::Connect(int handler, const char* host, unsigned short remotePort, const char* passwordData, unsigned int timeout, unsigned int maxBuffLen)
{
	RakPeerInterface* peer = GetPeer(handler);
	if (!peer)
		return INVALID_PARAMETER;
	this->maxBuffLen = maxBuffLen;
	return peer->Connect(host, remotePort, passwordData, strlen(passwordData), NULL, 0, 10, 500, timeout);
}

void ClientConnections::CloseConnection(int handler)
{
	RakPeerInterface* peer = GetPeer(handler);
	if (!peer)
		return;
	return peer->CloseConnection(peer->GetGUIDFromIndex(0), true);
}

unsigned ClientConnections::Send(int handler, const unsigned char* data, const int dataLen)
{
	RakPeerInterface* peer = GetPeer(handler);
	if (!peer)
		return -1;

	bs.Reset();
	bs.Write((unsigned char)ID_USER_PACKET_ENUM);
	bs.Write((const char*)data, dataLen);

	return peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetGUIDFromIndex(0), false);
}

DefaultMessageIDTypes ClientConnections::Receive(int handler, unsigned char* buff, int* dataLen)
{
	RakPeerInterface* peer = GetPeer(handler);
	if (!peer)
		return ID_MESSAGEID_NULL;
	Packet* pck = peer->Receive();
	if (!pck || !pck->data || pck->length == 0)
		return ID_MESSAGEID_NULL;
	DefaultMessageIDTypes ret = (DefaultMessageIDTypes)pck->data[0];
	*dataLen = pck->length - 1;
	if (pck->length >= maxBuffLen)
	{
		peer->DeallocatePacket(pck);
		return ID_MESSAGEID_OVERFLOW_RECV_BUFF;
	}
	memcpy(buff, (char*)pck->data + 1, pck->length - 1);	
	peer->DeallocatePacket(pck);
	return ret;
}

RakPeerInterface* ClientConnections::GetPeer(int handler)
{
	if (peers.Has(handler))
		return peers.Get(handler);
	return NULL;
}

