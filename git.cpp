#include "Header.h"

#define HTTP 80
#define HTTPS 443

char* GET(char* URL) {
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN sockAddr;
	hostent* hostent;


	string host = "";
	string path = "/";
	bool isPath = false;
	for (int i = 0; i < strlen(URL); i++) {

		if (isPath) {
			path += URL[i];
		}
		else {
			if (URL[i] == '/') {
				isPath = true;
				continue;
			}
			host += URL[i];
		}
	}

	string request = "GET " + path + " HTTP/1.1\n";
	request += "Host: " + host + "\n";
	request += "Connection: close\n\n";


	printf(request.c_str());

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed.\n");
		return NULL;
	}

	s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL, 0, 0);
	if (s == INVALID_SOCKET) {
		printf("Error connect. %d\n", WSAGetLastError());
		return NULL;
	}

	hostent = gethostbyname(host.c_str());
	if (hostent == NULL) {
		printf("Error gethostbyname. %d\n", WSAGetLastError());
		closesocket(s);
		return NULL;
	}
	sockAddr.sin_port = htons(HTTP);
	sockAddr.sin_family = AF_INET;
	memcpy(&sockAddr.sin_addr, hostent->h_addr, hostent->h_length);

	if (connect(s, (SOCKADDR*)(&sockAddr), sizeof(sockAddr)) != 0) {
		printf("Error connect. %d\n", WSAGetLastError());
		closesocket(s);
		return NULL;
	}

	if (send(s, request.c_str(), request.size(), MSG_OOB) != request.size()) {
		printf("Error sending. %d\n", WSAGetLastError());
		closesocket(s);
		return NULL;
	}

	int nDataLength;
	char buffer[BUFSIZ];
	string result = "";
	while ((nDataLength = recv(s, buffer, BUFSIZ, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
			result += buffer[i];
			i += 1;
			printf("%c", buffer[i]);
		}
	}

	closesocket(s);
	WSACleanup();

	char* temp = (char*)calloc(result.size() + 1, sizeof(char));
	if (temp) memcpy(temp, result.c_str(), result.size());
	return temp;
}