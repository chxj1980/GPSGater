#include "stdafx.h"
#include "Socket.h"

#pragma comment(lib, "ws2_32.lib")
#include <errno.h>

Socket::Socket(UINT mtu)
:m_Mtu(mtu)
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD( 2, 2 );
	WSAStartup(wVersionRequested, &wsaData);

	m_Socket = NULL;
	memset(&m_BindAddr, 0, sizeof(SOCKADDR_IN));
	memset(&m_SendAddr, 0, sizeof(SOCKADDR_IN));

	m_isOpen = FALSE;
}

Socket::~Socket()
{
	Close();

	WSACleanup();
}

void Socket::Close()
{
	if (m_Socket)
		closesocket(m_Socket);
	m_Socket = NULL;

	m_isOpen = FALSE;
}

int Socket::Read(BYTE* pBuffer, int readSize, UINT nTimeOut)
{
	int selectState;
	int recvSize;

	int len = sizeof(SOCKADDR_IN);

	if (!pBuffer || !m_isOpen)
		return -1;

	selectState = Select(SELECT_MODE_READY, nTimeOut);
	if (SELECT_STATE_TIMEOUT == selectState)
		return 0;

	if (SELECT_STATE_READY == selectState)
	{
		recvSize = recv(m_Socket, (char*)pBuffer, readSize, 0);
		if (recvSize <= 0)
			return -1;
		return recvSize;
	}
	return -1;
}

int Socket::Read_UDP(BYTE* pBuffer, int readSize,char **pFromAddr,int &nFromPort, UINT nTimeOut)
{
	int selectState;
	int recvSize;

	int len = sizeof(SOCKADDR_IN);

	if (!pBuffer || !m_isOpen)
		return -1;

	selectState = Select(SELECT_MODE_READY, nTimeOut);
	if (SELECT_STATE_TIMEOUT == selectState)
		return 0;

	if (SELECT_STATE_READY == selectState)
	{
		//recvSize = recv(m_Socket, (char*)pBuffer, readSize, 0);
		sockaddr_in   sa;
		int  fromlen = sizeof(sa);
		recvSize = recvfrom(m_Socket,(char*)pBuffer,readSize,0,(SOCKADDR *) &sa,&fromlen);
		*pFromAddr = inet_ntoa(sa.sin_addr);
		nFromPort = sa.sin_port;
		if (recvSize <= 0)
			return -1;
		return recvSize;
	}
	return -1;
}

int Socket::Write(PBYTE pBuffer, int writeSize, UINT nTimeOut)
{
	int selectState = 0;
	int sendSize = 0;

	if (!pBuffer || !m_isOpen)
		return -1;

	selectState = Select(SELECT_MODE_WRITE, nTimeOut);
	if (selectState == SELECT_STATE_TIMEOUT)
		return 0;

	if (selectState == SELECT_STATE_READY)
	{
		sendSize = sendto(m_Socket, (char*)pBuffer, writeSize, 0, (SOCKADDR*)&m_SendAddr, sizeof(SOCKADDR_IN));
		if (sendSize <= 0)
			return -1;
		return sendSize;
	}	

	return -1;
}

SOCKADDR_IN Socket::GetBindAddr()
{
	return m_BindAddr;
}

SOCKADDR_IN Socket::GetConnectAddr()
{
	return m_SendAddr;
}

UINT Socket::GetMTU()
{
	return m_Mtu;
}


//  Waits for a file descriptor/socket to change status.
// 
//  network input plugins should use this function in order to
//  not freeze the engine.
// 
//  params :
//    mode           SELECT_MODE_READY, SELECT_MODE_WRITE
//    timeout_usec   timeout in microsecond
// 
//  return value :
//    SELECT_STATE_READY     the file descriptor is ready for cmd
//    SELECT_STATE_ERROR     an i/o error occured
//    SELECT_STATE_ABORTED   command aborted by an other thread
//    SELECT_STATE_TIMEOUT   the file descriptor is not ready after timeout_usec microsecond

int Socket::Select( int iMode, int nTimeoutUsec )
{
	fd_set fdset;
	fd_set *readSet, *writeSet;
	timeval selectTimeout;
	int ret;

	selectTimeout.tv_sec  = 0;
	selectTimeout.tv_usec = nTimeoutUsec;

	FD_ZERO (&fdset);
	FD_SET  (m_Socket, &fdset);

	readSet = (iMode & SELECT_MODE_READY) ? &fdset : NULL;
	writeSet = (iMode & SELECT_MODE_WRITE) ? &fdset : NULL;

	ret = select ( (int)m_Socket + 1, readSet, writeSet, NULL, &selectTimeout);

	if (ret == 1) 
		return SELECT_STATE_READY;

	if (ret == SOCKET_ERROR) 
	{
		if ( errno == EINTR)
			return SELECT_STATE_ABORTED;

		ReportError();
		return SELECT_STATE_ERROR;
	} 

	return SELECT_STATE_TIMEOUT;
}

void Socket::ReportError()
{
	int isErr = WSAGetLastError();
	printf("Socket error is:%d\n", isErr);

	//Close();
}

BOOL Socket::GetLocalIPList(vector<string>& vIPList)
{ 
	WORD wVersionRequested; 
	WSADATA wsaData; 
	int err; 
	
	wVersionRequested = MAKEWORD( 2, 2 ); 
	err = WSAStartup( wVersionRequested, &wsaData ); 
	if ( err != 0 ) 
	{
		printf("GetLocalIPList	: WSAStartup failed !"); 
		return FALSE; 
	} 

	char szhn[256]; 
	int nStatus = gethostname(szhn, sizeof(szhn)); 
	if (nStatus == SOCKET_ERROR ) 
	{ 
		printf("Socket	: Gethostname failed, Error code: %d", WSAGetLastError()); 
		return FALSE; 
	} 
	
	HOSTENT *host = gethostbyname(szhn); 
	if (host != NULL) 
	{ 
		for ( int i=0; ; i++ ) 
		{ 
			vIPList.push_back( inet_ntoa( *(IN_ADDR*)host->h_addr_list[i] ) ) ; 
			if ( host->h_addr_list[i] + host->h_length >= host->h_name ) 
				break; 
		}
	}
		
	WSACleanup(); 

	return TRUE; 
}
