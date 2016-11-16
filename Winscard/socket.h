/* 
   Socket.h

   Copyright (C) 2002-2004 Ren� Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

/*

Altered by P. Svenda

/**/


#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <WinSock2.h>

#include <string>

/*#ifndef UNICODE
#define UNICODE
#endif

#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#endif*/

#if defined (UNICODE) && defined (_WIN32)
typedef std::wstring string_type;
typedef std::wifstream ifstream_type;
typedef std::wofstream ofstream_type;
typedef std::wfstream fstream_type;
typedef wchar_t char_type;
#define _CONV(x) L ##x
#else 
typedef std::string string_type;
typedef std::ifstream ifstream_type;
typedef std::ofstream ofstream_type;
typedef std::fstream fstream_type;
typedef char char_type;
#define _CONV(x) x
#endif

enum TypeSocket {BlockingSocket, NonBlockingSocket};

class Socket {
public:

  virtual ~Socket();
  Socket(const Socket&);
  Socket& operator=(Socket&);

  int recvtimeout(SOCKET s, char *buf, int len, int timeout);

  string_type ReceiveLine(int timeout = 0);
  string_type ReceiveBytes();

  string_type ReceiveResponse(string_type endSeq, int timeout);
  int ReceiveLineToFile(string_type filePath, int timeout, int* pWrittenValues = NULL);
  int ReceiveLineToMemory(INT_DATA_BLOB* pData, int timeout, int bytesPerSample = 2);

  void   Close();

  // The parameter of SendLine is not a const reference
  // because SendLine modifes the std::string passed.
  void   SendLine (string_type);

  // The parameter of SendBytes is a const reference
  // because SendBytes does not modify the std::string passed 
  // (in contrast to SendLine).
  void   SendBytes(const string_type&);

protected:
  friend class SocketServer;
  friend class SocketSelect;

  Socket(SOCKET s);
  Socket();


  SOCKET s_;

  int* refCounter_;

private:
  static void Start();
  static void End();
  static int  nofSockets_;
};

class SocketClient : public Socket {
public:
  SocketClient(const string_type& host, int port);
};


class SocketServer : public Socket {
public:
  SocketServer(int port, int connections, TypeSocket type=BlockingSocket);

  Socket* Accept();

};

// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/wsapiref_2tiq.asp
class SocketSelect {
  public:
    SocketSelect(Socket const * const s1, Socket const * const s2=NULL, TypeSocket type=BlockingSocket);

    bool Readable(Socket const * const s);

  private:
    fd_set fds_;
}; 



#endif
