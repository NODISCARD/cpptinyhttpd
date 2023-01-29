#ifndef _H_AUXILLIARY
#define _H_AUXILLIARY

long Write(int __fd, const char* __buf);
int ReadLine(int Socket, std::string* Buf);
int SendFile(int Socket, std::string filename);
void SendHeader200OK(int client, struct stat* FileInfo);
void SendHeader404Unfound(int client);

#endif