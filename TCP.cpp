#include "TCP.h"


TCP::TCP(QWidget *parent)
{
	this->parent = parent;
	ConnectSocket = INVALID_SOCKET;
	result = NULL;
	ptr = NULL;
}


TCP::~TCP()
{
}

int TCP::connectTCP()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		QMessageBox::critical(parent, QString("Error"), QString("WSAStartup failed with error: %d\n") + QString::number(iResult));
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(IP, PORT, &hints, &result);
	if (iResult != 0) 
	{
		QMessageBox::critical(parent, QString("Error"), QString("getaddrinfo failed with error: %d\n") + QString::number(iResult));
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
		{
			QMessageBox::critical(parent, QString("Error"), QString("socket failed with error: %ld\n") + QString::number(iResult));
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) 
	{
		QMessageBox::critical(parent, QString("Error"), QString("Unable to connect to server!\n") + QString::number(iResult));
		WSACleanup();
		return 1;
	}

	return 0;
}

int TCP::closeTCP()
{
	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		QMessageBox::critical(parent, QString("Error"), QString("shutdown failed with error: %d\n") + QString::number(iResult));
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

QString TCP::receiveMessage()
{
	char recbuf[RECVBYTES];
	if (recv(ConnectSocket, recbuf, RECVBYTES, 0) < 0)
	{
		QMessageBox::critical(parent, QString("Error"), QString("send failed with error: %d\n") + QString::number(iResult));
		closesocket(ConnectSocket);
		WSACleanup();
		return QString("");
	}
	return QString::fromLatin1(recbuf);
}

int TCP::sendMessage(const char *str, bool zeroSensitive, int actualLenght)
{
	// Send an initial buffer
	int lenght = strlen(str);
	if (zeroSensitive)
		lenght = actualLenght;

	iResult = send(ConnectSocket, str, lenght, 0);
	if (iResult == SOCKET_ERROR)
	{
		QMessageBox::critical(parent, QString("Error"), QString("send failed with error: %d\n") + QString::number(iResult));
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}