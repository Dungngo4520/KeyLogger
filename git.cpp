#include "Header.h"

#define host "api.github.com"
#define path "/repos/dungngo4520/log"

BOOL ConnectAuthSocket(SOCKET* s, CredHandle* hCred, struct _SecHandle* hcText);
BOOL DoAuthentication(SOCKET s);
BOOL GenClientContext(BYTE* pIn, DWORD cbIn, BYTE* pOut, DWORD* pcbOut, BOOL* pfDone, SEC_WCHAR* pszTarget, CredHandle* hCred, struct _SecHandle* hcText);
PBYTE DecryptThis(PBYTE pBuffer, LPDWORD pcbMessage, struct _SecHandle* hCtxt, ULONG cbSecurityTrailer);
PBYTE VerifyThis(PBYTE pBuffer,LPDWORD pcbMessage,struct _SecHandle* hCtxt,ULONG cbMaxSignature);
void PrintHexDump(DWORD length,PBYTE buffer);

struct _SecHandle  hcText;
CredHandle hCred;




char* GET(char* URL) {
	SOCKET Client_Socket;
	BYTE Data[BUFSIZ];
	PCHAR pMessage;
	WSADATA wsaData;
	CredHandle hCred;
	struct _SecHandle hCtxt;
	SECURITY_STATUS ss;
	DWORD cbRead;
	ULONG cbMaxSignature;
	ULONG cbSecurityTrailer;
	SecPkgContext_Sizes SecPkgContextSizes;
	SecPkgContext_NegotiationInfo SecPkgNegInfo;
	BOOL DoAuthentication(SOCKET s);

	if (WSAStartup(0x0101, &wsaData)) {
		printf("Could not initialize winsock ");
		return NULL;
	}

	if (!ConnectAuthSocket(&Client_Socket, &hCred, &hcText)) {
		printf("Authenticated server connection ");
		return NULL;
	}

	ss = QueryContextAttributes(&hcText, SECPKG_ATTR_NEGOTIATION_INFO, &SecPkgNegInfo);

	if (ss < 0) {
		printf("QueryContextAttributes failed ");
		return NULL;
	}

	cbMaxSignature = SecPkgContextSizes.cbMaxSignature;
	cbSecurityTrailer = SecPkgContextSizes.cbSecurityTrailer;
	printf("InitializeSecurityContext result = 0x%08x\n", ss);

	if (!ReceiveBytes(Client_Socket, Data, BUFSIZ, &cbRead)) {
		printf("No response from server ");
		return NULL;
	}

	if (0 == cbRead) {
		printf("Zero bytes received ");
		return "";
	}

	pMessage = (PCHAR)DecryptThis(Data, &cbRead, &hcText, cbSecurityTrailer);

	printf("The message from the server is \n ->  %.*s \n", cbRead, pMessage);

	DeleteSecurityContext(&hcText);
	FreeCredentialHandle(&hCred);
	shutdown(Client_Socket, 2);
	closesocket(Client_Socket);
	if (SOCKET_ERROR == WSACleanup()) {
		printf("Problem with socket cleanup ");
	}


}


BOOL ConnectAuthSocket(SOCKET* s, CredHandle* hCred, struct _SecHandle* hcText) {
	unsigned long  ulAddress;
	struct hostent* pHost;
	SOCKADDR_IN    sin;

	//--------------------------------------------------------------------
	//  Lookup the server's address.

	ulAddress = inet_addr("");

	if (INADDR_NONE == ulAddress) {
		pHost = gethostbyname(host);
		if (NULL == pHost) {
			printf("Unable to resolve host name ");
			return FALSE;
		}
		memcpy((char FAR*) & ulAddress, pHost->h_addr, pHost->h_length);
	}

	//--------------------------------------------------------------------
	//  Create the socket.

	*s = socket(
		PF_INET,
		SOCK_STREAM,
		0);

	if (INVALID_SOCKET == *s) {
		printf("Unable to create socket");
		return FALSE;
	}
	else {
		printf("Socket created.\n");
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ulAddress;
	sin.sin_port = htons(2000);

	//--------------------------------------------------------------------
	//  Connect to the server.

	if (connect(*s, (LPSOCKADDR)&sin, sizeof(sin))) {
		closesocket(*s);
		printf("Connect failed ");
		return FALSE;
	}

	//--------------------------------------------------------------------
	//  Authenticate the connection. 

	if (!DoAuthentication(*s)) {
		closesocket(*s);
		printf("Authentication ");
		return FALSE;
	}

	return(TRUE);

}

BOOL DoAuthentication(SOCKET s) {
	BOOL        fDone = FALSE;
	DWORD       cbOut = 0;
	DWORD       cbIn = 0;
	PBYTE       pInBuf;
	PBYTE       pOutBuf;


	if (!(pInBuf = (PBYTE)malloc(BUFSIZ))) {
		printf("Memory allocation ");
		return FALSE;
	}

	if (!(pOutBuf = (PBYTE)malloc(BUFSIZ))) {
		printf("Memory allocation ");
		return FALSE;
	}

	cbOut = BUFSIZ;
	if (!GenClientContext(NULL, 0, pOutBuf, &cbOut, &fDone, (SEC_WCHAR*)host, &hCred, &hcText)) {
		return(FALSE);
	}

	if (!SendMsg(s, pOutBuf, cbOut)) {
		printf("Send message failed ");
		return FALSE;
	}

	while (!fDone) {
		if (!ReceiveMsg(s, pInBuf, BUFSIZ, &cbIn)) {
			printf("Receive message failed ");
			return FALSE;
		}

		cbOut = BUFSIZ;

		if (!GenClientContext(pInBuf, cbIn, pOutBuf, &cbOut, &fDone, (SEC_WCHAR*)host, &hCred, &hcText)) {
			printf("GenClientContext failed");
			return FALSE;
		}
		if (!SendMsg(s, pOutBuf, cbOut)) {
			printf("Send message 2  failed ");
			return FALSE;
		}
	}

	free(pInBuf);
	free(pOutBuf);
	return(TRUE);
}

BOOL GenClientContext(BYTE* pIn, DWORD cbIn, BYTE* pOut, DWORD* pcbOut, BOOL* pfDone, SEC_WCHAR* pszTarget, CredHandle* hCred, struct _SecHandle* hcText) {
	SECURITY_STATUS   ss;
	TimeStamp         Lifetime;
	SecBufferDesc     OutBuffDesc;
	SecBuffer         OutSecBuff;
	SecBufferDesc     InBuffDesc;
	SecBuffer         InSecBuff;
	ULONG             ContextAttributes;
	static PTCHAR     lpPackageName = (PTCHAR)NEGOSSP_NAME;

	if (NULL == pIn) {
		ss = AcquireCredentialsHandle(NULL, lpPackageName, SECPKG_CRED_OUTBOUND, NULL, NULL, NULL, NULL, hCred, &Lifetime);

		if (ss < 0) {
			printf("AcquireCreds failed ");
			return FALSE;
		}
	}

	//--------------------------------------------------------------------
	//  Prepare the buffers.

	OutBuffDesc.ulVersion = 0;
	OutBuffDesc.cBuffers = 1;
	OutBuffDesc.pBuffers = &OutSecBuff;

	OutSecBuff.cbBuffer = *pcbOut;
	OutSecBuff.BufferType = SECBUFFER_TOKEN;
	OutSecBuff.pvBuffer = pOut;

	//-------------------------------------------------------------------
	//  The input buffer is created only if a message has been received 
	//  from the server.

	if (pIn) {
		InBuffDesc.ulVersion = 0;
		InBuffDesc.cBuffers = 1;
		InBuffDesc.pBuffers = &InSecBuff;

		InSecBuff.cbBuffer = cbIn;
		InSecBuff.BufferType = SECBUFFER_TOKEN;
		InSecBuff.pvBuffer = pIn;

		ss = InitializeSecurityContext(hCred, hcText, (SEC_CHAR*)pszTarget, ISC_REQ_CONFIDENTIALITY, 0, SECURITY_NATIVE_DREP, &InBuffDesc, 0, hcText, &OutBuffDesc, &ContextAttributes, &Lifetime);
	}
	else {
		ss = InitializeSecurityContext(hCred, NULL, (SEC_CHAR*)pszTarget, ISC_REQ_CONFIDENTIALITY, 0, SECURITY_NATIVE_DREP, NULL, 0, hcText, &OutBuffDesc, &ContextAttributes, &Lifetime);
	}

	if (ss < 0) {
		printf("InitializeSecurityContext failed ");
		return FALSE;
	}

	//-------------------------------------------------------------------
	//  If necessary, complete the token.

	if ((SEC_I_COMPLETE_NEEDED == ss)
		|| (SEC_I_COMPLETE_AND_CONTINUE == ss)) {
		ss = CompleteAuthToken(hcText, &OutBuffDesc);
		if (ss < 0) {
			fprintf(stderr, "complete failed: 0x%08x\n", ss);
			return FALSE;
		}
	}

	*pcbOut = OutSecBuff.cbBuffer;

	*pfDone = !((SEC_I_CONTINUE_NEEDED == ss) ||
		(SEC_I_COMPLETE_AND_CONTINUE == ss));

	printf("Token buffer generated (%lu bytes):\n", OutSecBuff.cbBuffer);
	PrintHexDump(OutSecBuff.cbBuffer, (PBYTE)OutSecBuff.pvBuffer);
	return TRUE;

}

PBYTE DecryptThis(PBYTE pBuffer,LPDWORD pcbMessage,struct _SecHandle* hCtxt,ULONG cbSecurityTrailer) {
	SECURITY_STATUS   ss;
	SecBufferDesc     BuffDesc;
	SecBuffer         SecBuff[2];
	ULONG             ulQop = 0;
	PBYTE             pSigBuffer;
	PBYTE             pDataBuffer;
	DWORD             SigBufferSize;

	//-------------------------------------------------------------------
	//  By agreement, the server encrypted the message and set the size
	//  of the trailer block to be just what it needed. DecryptMessage 
	//  needs the size of the trailer block. 
	//  The size of the trailer is in the first DWORD of the
	//  message received. 

	SigBufferSize = *((DWORD*)pBuffer);
	printf("data before decryption including trailer (%lu bytes):\n",
		*pcbMessage);
	PrintHexDump(*pcbMessage, (PBYTE)pBuffer);

	//--------------------------------------------------------------------
	//  By agreement, the server placed the trailer at the beginning 
	//  of the message that was sent immediately following the trailer 
	//  size DWORD.

	pSigBuffer = pBuffer + sizeof(DWORD);

	//--------------------------------------------------------------------
	//  The data comes after the trailer.

	pDataBuffer = pSigBuffer + SigBufferSize;

	//--------------------------------------------------------------------
	//  *pcbMessage is reset to the size of just the encrypted bytes.

	*pcbMessage = *pcbMessage - SigBufferSize - sizeof(DWORD);

	//--------------------------------------------------------------------
	//  Prepare the buffers to be passed to the DecryptMessage function.

	BuffDesc.ulVersion = 0;
	BuffDesc.cBuffers = 2;
	BuffDesc.pBuffers = SecBuff;

	SecBuff[0].cbBuffer = SigBufferSize;
	SecBuff[0].BufferType = SECBUFFER_TOKEN;
	SecBuff[0].pvBuffer = pSigBuffer;

	SecBuff[1].cbBuffer = *pcbMessage;
	SecBuff[1].BufferType = SECBUFFER_DATA;
	SecBuff[1].pvBuffer = pDataBuffer;

	ss = DecryptMessage(
		hCtxt,
		&BuffDesc,
		0,
		&ulQop);

	if (ss<0) {
		fprintf(stderr, "DecryptMessage failed");
	}

	//-------------------------------------------------------------------
	//  Return a pointer to the decrypted data. The trailer data
	//  is discarded.

	return pDataBuffer;

}

PBYTE VerifyThis(
	PBYTE   pBuffer,
	LPDWORD pcbMessage,
	struct _SecHandle* hCtxt,
	ULONG   cbMaxSignature) {

	SECURITY_STATUS   ss;
	SecBufferDesc     BuffDesc;
	SecBuffer         SecBuff[2];
	ULONG             ulQop = 0;
	PBYTE             pSigBuffer;
	PBYTE             pDataBuffer;

	//-------------------------------------------------------------------
	//  The global cbMaxSignature is the size of the signature
	//  in the message received.

	printf("data before verifying (including signature):\n");
	PrintHexDump(*pcbMessage, pBuffer);

	//--------------------------------------------------------------------
	//  By agreement with the server, 
	//  the signature is at the beginning of the message received,
	//  and the data that was signed comes after the signature.

	pSigBuffer = pBuffer;
	pDataBuffer = pBuffer + cbMaxSignature;

	//-------------------------------------------------------------------
	//  The size of the message is reset to the size of the data only.

	*pcbMessage = *pcbMessage - (cbMaxSignature);

	//--------------------------------------------------------------------
	//  Prepare the buffers to be passed to the signature verification 
	//  function.

	BuffDesc.ulVersion = 0;
	BuffDesc.cBuffers = 2;
	BuffDesc.pBuffers = SecBuff;

	SecBuff[0].cbBuffer = cbMaxSignature;
	SecBuff[0].BufferType = SECBUFFER_TOKEN;
	SecBuff[0].pvBuffer = pSigBuffer;

	SecBuff[1].cbBuffer = *pcbMessage;
	SecBuff[1].BufferType = SECBUFFER_DATA;
	SecBuff[1].pvBuffer = pDataBuffer;

	ss = VerifySignature(
		hCtxt,
		&BuffDesc,
		0,
		&ulQop
	);

	if (ss<0) {
		fprintf(stderr, "VerifyMessage failed");
	}
	else {
		printf("Message was properly signed.\n");
	}

	return pDataBuffer;

}  // end VerifyThis

void PrintHexDump(
	DWORD length,
	PBYTE buffer) {
	DWORD i, count, index;
	CHAR rgbDigits[] = "0123456789abcdef";
	CHAR rgbLine[100];
	char cbLine;

	for (index = 0; length;
		length -= count, buffer += count, index += count) {
		count = (length > 16) ? 16 : length;

		sprintf_s(rgbLine, 100, "%4.4x  ", index);
		cbLine = 6;

		for (i = 0; i < count; i++) {
			rgbLine[cbLine++] = rgbDigits[buffer[i] >> 4];
			rgbLine[cbLine++] = rgbDigits[buffer[i] & 0x0f];
			if (i == 7) {
				rgbLine[cbLine++] = ':';
			}
			else {
				rgbLine[cbLine++] = ' ';
			}
		}
		for (; i < 16; i++) {
			rgbLine[cbLine++] = ' ';
			rgbLine[cbLine++] = ' ';
			rgbLine[cbLine++] = ' ';
		}

		rgbLine[cbLine++] = ' ';

		for (i = 0; i < count; i++) {
			if (buffer[i] < 32 || buffer[i] > 126) {
				rgbLine[cbLine++] = '.';
			}
			else {
				rgbLine[cbLine++] = buffer[i];
			}
		}

		rgbLine[cbLine++] = 0;
		printf("%s\n", rgbLine);
	}
}

BOOL SendMsg(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf) {
	if (0 == cbBuf)
		return(TRUE);

	//----------------------------------------------------------
	//  Send the size of the message.

	if (!SendBytes(s, (PBYTE)&cbBuf, sizeof(cbBuf)))
		return(FALSE);

	//----------------------------------------------------------
	//  Send the body of the message.

	if (!SendBytes(
		s,
		pBuf,
		cbBuf)) {
		return(FALSE);
	}

	return(TRUE);
}

BOOL ReceiveMsg(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf,
	DWORD* pcbRead)

{
	DWORD cbRead;
	DWORD cbData;

	//----------------------------------------------------------
	//  Receive the number of bytes in the message.

	if (!ReceiveBytes(
		s,
		(PBYTE)&cbData,
		sizeof(cbData),
		&cbRead)) {
		return(FALSE);
	}

	if (sizeof(cbData) != cbRead)
		return(FALSE);
	//----------------------------------------------------------
	//  Read the full message.

	if (!ReceiveBytes(
		s,
		pBuf,
		cbData,
		&cbRead)) {
		return(FALSE);
	}

	if (cbRead != cbData)
		return(FALSE);

	*pcbRead = cbRead;
	return(TRUE);
}  // end ReceiveMessage    

BOOL SendBytes(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf) {
	PBYTE pTemp = pBuf;
	int   cbSent;
	int   cbRemaining = cbBuf;

	if (0 == cbBuf)
		return(TRUE);

	while (cbRemaining) {
		cbSent = send(
			s,
			(const char*)pTemp,
			cbRemaining,
			0);
		if (SOCKET_ERROR == cbSent) {
			fprintf(stderr, "send failed: %u\n", GetLastError());
			return FALSE;
		}

		pTemp += cbSent;
		cbRemaining -= cbSent;
	}

	return TRUE;
}

BOOL ReceiveBytes(
	SOCKET  s,
	PBYTE   pBuf,
	DWORD   cbBuf,
	DWORD* pcbRead) {
	PBYTE pTemp = pBuf;
	int cbRead, cbRemaining = cbBuf;

	while (cbRemaining) {
		cbRead = recv(
			s,
			(char*)pTemp,
			cbRemaining,
			0);
		if (0 == cbRead)
			break;
		if (SOCKET_ERROR == cbRead) {
			fprintf(stderr, "recv failed: %u\n", GetLastError());
			return FALSE;
		}

		cbRemaining -= cbRead;
		pTemp += cbRead;
	}

	*pcbRead = cbBuf - cbRemaining;

	return TRUE;
}  // end ReceiveBytes







//char* GET(char* URL) {
//	WSADATA wsaData;
//	SOCKET s;
//	SOCKADDR_IN sockAddr;
//	hostent* hostent;
//
//
//	string host = "";
//	string path = "/";
//	bool isPath = false;
//	for (int i = 0; i < strlen(URL); i++) {
//
//		if (isPath) {
//			path += URL[i];
//		}
//		else {
//			if (URL[i] == '/') {
//				isPath = true;
//				continue;
//			}
//			host += URL[i];
//		}
//	}
//
//	string request = "GET " + path + " HTTP/1.1\r\n";
//	request += "Host: " + host + "\r\n";
//	request += "Connection: close\r\n\r\n";
//
//
//	printf(request.c_str());
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		printf("WSAStartup failed.\n");
//		return NULL;
//	}
//
//	s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL, 0, 0);
//	if (s == INVALID_SOCKET) {
//		printf("Error connect. %d\n", WSAGetLastError());
//		return NULL;
//	}
//	if (WSASetSocketSecurity(s, NULL, sizeof(SOCKET_SECURITY_SETTINGS), NULL, NULL) == SOCKET_ERROR) {
//		printf("Error WSASetSocketSecurity. %d\n", WSAGetLastError());
//		return NULL;
//	}
//	peerTargetName = (SOCKET_PEER_TARGET_NAME*)HeapAlloc(GetProcessHeap(),
//		HEAP_ZERO_MEMORY, peerTargetNameLen);
//	peerTargetName->SecurityProtocol = SOCKET_SECURITY_PROTOCOL_DEFAULT;
//
//
//	hostent = gethostbyname(host.c_str());
//	if (hostent == NULL) {
//		printf("Error gethostbyname. %d\n", WSAGetLastError());
//		closesocket(s);
//		return NULL;
//	}
//	sockAddr.sin_port = htons(80);
//	sockAddr.sin_family = AF_INET;
//	sockAddr.sin_addr.s_addr = *((unsigned long*)hostent->h_addr);
//
//	if (connect(s, (SOCKADDR*)(&sockAddr), sizeof(sockAddr)) != 0) {
//		printf("Error connect. %d\n", WSAGetLastError());
//		closesocket(s);
//		return NULL;
//	}
//
//	if (send(s, request.c_str(), request.size(), 0) != request.size()) {
//		printf("Error sending. %d\n", WSAGetLastError());
//		closesocket(s);
//		return NULL;
//	}
//
//	int nDataLength;
//	char buffer[BUFSIZ];
//	string result = "";
//	while ((nDataLength = recv(s, buffer, BUFSIZ, 0)) > 0) {
//		int i = 0;
//		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
//			result += buffer[i];
//			i += 1;
//			printf("%c", buffer[i]);
//		}
//	}
//
//	closesocket(s);
//	WSACleanup();
//
//	char* temp = (char*)calloc(result.size() + 1, sizeof(char));
//	if (temp) memcpy(temp, result.c_str(), result.size());
//	return temp;
//}