#include "headers.h"
using namespace std;

long Write(int __fd, const char* __buf) {
    return ::write(__fd, (const void*)__buf, strlen(__buf));
}

void SendHeader200OK(int client, struct stat* FileInfo) {
    Write(client, "HTTP/1.0 200 OK\r\n");
    Write(client, SERVER_STRING);
    Write(client, (string("Content-Length: ") + to_string((*FileInfo).st_size) + string("\r\n")).c_str());
    Write(client, "Content-Type: text/html\r\n");
    Write(client, "\r\n");
    return;
}

void SendHeader404Unfound(int client) {
    Write(client, "HTTP/1.0 404 NOT FOUND\r\n");
    Write(client, SERVER_STRING);
    Write(client, "Content-Type: text/html\r\n");
    Write(client, "\r\n");
    Write(client, "<!DOCTYPE html><TITLE>Not Found</TITLE>\r\n");
    Write(client, "<BODY><P>The server could not fulfill\r\n");
    Write(client, "your request because the resource specified\r\n");
    Write(client, "is unavailable or nonexistent.</P></BODY>\r\n");

}

int ReadLine(int Socket, string* Buf) {
    char c = 0;
    int LengthRecv = 0;
    for (; c != '\n'; ++LengthRecv) {
        if (recv(Socket, &c, 1, 0) > 0) {
            if (c == '\r') {
                if (recv(Socket, &c, 1, MSG_PEEK) > 0 && c == '\n')
                    recv(Socket, &c, 1, 0);
                else
                    c = '\n';
            }
            (*Buf).push_back(c);
        }
        else c = '\n';
    }
    (*Buf).push_back(c);
    return LengthRecv;
}

int SendFile(int Socket, std::string filename) {
    string line;
    ifstream file(filename);
    while(std::getline(file, line))
        Write(Socket, line.c_str());
    return 0;
}