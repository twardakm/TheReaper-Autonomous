#pragma once

/*
C++ program is a TCP client and sends data to LabView program

https://msdn.microsoft.com/en-us/library/windows/desktop/ms737591%28v=vs.85%29.aspx
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <qmessagebox.h>
#include <qwidget.h>
#include <qstring.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define BUFLEN 512 // buffor lenght
#define PORT "5001" // port to connect. Camera connect port 5000
#define IP "192.168.1.3"
#define RECVBYTES 10


class TCP
{
public:
	TCP(QWidget *parent);
	~TCP();

	int connectTCP();
	int closeTCP();
	int sendMessage(const char *str, bool zeroSensitive = false, int actualLenght = 0);
	QString receiveMessage();

private:
	QWidget *parent;
	WSADATA wsaData;
	SOCKET ConnectSocket;
	struct addrinfo *result, *ptr, hints;

	int iResult;
};

