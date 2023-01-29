// cpptinyhttpd.cpp: This file defines the entry point of
// the server program and some of auxillary procedures.
#include "headers.h"
#include "Auxilliary.h"
using namespace std;

int StartUp(u_short port);
int OnError(string reason);
void* AcceptRequest(void* arg);

int main(void)
{
    cout << "CPP Tiny httpd starting up" << endl;
    sockaddr_in ClientName;
    int ServerSocket = -1, ClientSocket = -1;

    socklen_t ClientSocketLen = sizeof(ClientSocketLen);
    pthread_t NewThread;

    ServerSocket = StartUp(8080);

    cout << "Server running at port 8080" << endl;

    for (;;) {
        if ((ClientSocket = accept(ServerSocket, (sockaddr*)&ClientName, &ClientSocketLen)) == -1) {
            OnError("Error from server: unable to accept cilent request");
        }
        if (pthread_create(&NewThread, NULL, &AcceptRequest, (void*)&ClientSocket)) {
            cerr << "Error from server: Unable to create a thread" << endl;
        }
    }
    close(ServerSocket);

    return 0;
}

int StartUp(u_short port)
{
    int socket_fd = 0;
    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        OnError("Error from server: unable to create a socket for connect");
    sockaddr_in sockAddr;
    bzero(&sockAddr, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
        OnError("Error from server: unable to bind informations to the socket");
    if (listen(socket_fd, LISTENQ) < 0) OnError("Error from server: Unable to make the socket listening");
    return socket_fd;
}

void *AcceptRequest(void* socketHD_ptr) {
    sockaddr_in CilentSocketInfo;
    socklen_t length = sizeof(CilentSocketInfo);
    int clientHD = *((int*)socketHD_ptr);
    if (getsockname(clientHD, (sockaddr*)&CilentSocketInfo, &length) == -1)
        OnError("Error from server: unable to get connection info.");
    cout << "Server info: received a request from " << inet_ntoa(CilentSocketInfo.sin_addr) << endl;

    string method;
    string LineRecv;
    string url;
    // Parse request
    ReadLine(clientHD, &LineRecv);
    cout << LineRecv << endl;
    auto SpacePos = LineRecv.find(" ");
    method = LineRecv.substr(0, SpacePos);
    for (; LineRecv[SpacePos] == ' '; ++SpacePos);
    url = LineRecv.substr(SpacePos, LineRecv.find(" ", SpacePos) - 4);
    if (url[url.length() - 1] == '/') {          // When a request ends with a / then
        url += "index.html";                     // we assume it is requesting for the
    }                                            // index.html under that directory.
    cout << "Requesting for " << url << endl;
    url = "." + url;
    cout << url << endl;
    struct stat FileInfo;
    if (stat(url.c_str(), &FileInfo) == -1) {
        cout << "File requested unfound. Returning status: 404 Not found" << endl;
        SendHeader404Unfound(clientHD);
        close(clientHD);
    }
    SendHeader200OK(clientHD, &FileInfo);
    SendFile(clientHD, url);
    close(clientHD);

    return NULL;
}

int OnError(string reason) {
    cerr << reason << endl;
    exit(1);
}