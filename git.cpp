#include "Header.h"
#include <string>

using namespace std;

void gitPush(char* message) {
	string user = "dungngo4520";
	string repo = "log";

	string request = "GET ";
	request += "/repos/" + user + "/" + repo + "";
	request += " HTTP/1.1\r\n";
	request += "Host: www.api.github.com\r\n";
	request += "Connection: close\r\n\r\n";
	//request += message;


	printf(request.c_str());

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo hints = { 0 }, *addrs;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int status = getaddrinfo("www.api.github.com", "80", &hints, &addrs);
	if (status != 0) {
		printf("Unable to query hostname");
		return;
	}
	if (addrs->ai_family != AF_INET) {
		printf("hostname does not have IPv4 address");
		return;
	}

	SOCKADDR_IN server = {};
	server.sin_port = htons(80);
	server.sin_family = addrs->ai_family;
	server.sin_addr.s_addr = *((unsigned long *)addrs->ai_addr);

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		printf("Unable to create a socket");
		return;
	}
	//ULONG iMode = 1;
	//ioctlsocket(sock, FIONBIO, &iMode);
	int result = connect(sock, (SOCKADDR *)(&server), sizeof(server));
	if (result < 0) {
		printf("Unable to connect to the server");
		closesocket(sock);
		return;
	}

	const char* ptr = request.c_str();
	int dataLength = request.size();
	do {
		int res = send(sock, ptr, dataLength, 0);
		if (result < 0) {
			printf("Unable to send the request");
			closesocket(sock);
			return;
		}
		ptr += res;
		dataLength -= res;
	} while (dataLength > 0);

	char buffer[BUFSIZ];
	string response = "";
	do {
		dataLength = recv(sock, buffer, sizeof(buffer), 0);
		printf(buffer);
	} while (dataLength > 0);
	


}