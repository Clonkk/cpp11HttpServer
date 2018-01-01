/* 
 *   Copyright (C) 2017-2018 Régis Caillaud 
 *
 *   This source code is provided 'as-is', without any express or implied
 *   warranty. In no event will the author be held liable for any damages
 *   arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *   including commercial applications, and to alter it and redistribute it
 *   freely, subject to the following restrictions:
 *
 *   1. The origin of this source code must not be misrepresented; you must not
 *   claim that you wrote the original source code. If you use this source code
 *   in a product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be
 *   misrepresented as being the original source code.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *
 */
#include "Server.hpp"
#include "socket_error.hpp"
#include <errno.h>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <poll.h>
#include <cstring>
#include <unordered_map>
//
void defaultEchoHandler(Socket* sock) {
  char buffer[1024];
  int length = sock->recv(buffer, 1024);
  // sock->send(buffer, length);
  std::string str(buffer, length);
  printf("Server::recv (%i bytes)... %s\n", length, str.c_str());
  sock->send(str.c_str(), str.size());
}
// Constructor
Server::Server():Socket() {
  int on = 1;
  int rc = ::setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc < 0) {
    ::close(_sockfd);
    handleError("setsockopt() failed");
  }
}
Server::Server(uint16_t port, const char* ip):Socket(port, ip) {
  int on = 1;
  int rc = ::setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc < 0) {
    ::close(_sockfd);
    handleError("setsockopt() failed");
  }
}
Server::Server(const Inet& inet) : Socket(inet) {
  int on = 1;
  int rc = ::setsockopt(_sockfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc < 0) {
    ::close(_sockfd);
    handleError("setsockopt() failed");
  }
}
// TODO
Server::~Server() {
  if(_listening) {
    close();
  }
}
// Bind socket
int Server::bind() {
  struct sockaddr_in s_in;
  memset(&s_in,0,sizeof(s_in));
  s_in.sin_family = AF_INET;
  s_in.sin_port = this->sockInet.getInPort();
  s_in.sin_addr = this->sockInet.getInAddr();
  int rc = ::bind(this->_sockfd, reinterpret_cast<struct sockaddr*>(&s_in), sizeof(s_in));
  if(rc == -1) {
    handleError(__PRETTY_FUNCTION__);
    return errno;
  } else {
    rc = ::listen(_sockfd, MAX_CLIENT);
    if (rc < 0) {
      ::close(_sockfd);
      handleError("listen() failed");
    }
    _hasConnected = true;
    return 0;
  }
}
void Server::open(std::function<void(Socket* sock)> func) {
  if(!_hasConnected) {
    this->bind();
  }
  if(!_listening) {
    messageHandler =  func;
    _msgRecv = true;
    _listening = true;
    listenThread = std::thread(&Server::listenImpl, this);
  }
}
void Server::listen(std::function<void(Socket*)> func) {
  open(func);
  this->waitListening();
}
void Server::listen() {
  open();
  this->waitListening();
}
void Server::listenImpl() {
  // printf("_sockfd = %i \"%s\" -- BEGIN -- \n", _sockfd, __PRETTY_FUNCTION__);
  int timeout = (500);
  struct pollfd fds;
  memset(&fds, 0 , sizeof(fds));
  fds.fd = _sockfd;
  fds.events = POLLIN;
  int new_sockfd;
  struct sockaddr_in inetSockClient;
  memset(&inetSockClient, 0, sizeof(inetSockClient));
  socklen_t sock_in_len = sizeof(sockaddr_in);
  int rc = 0;
  do {
    rc = poll(&fds, 1, timeout);
    if (rc < 0) {
      handleError(__PRETTY_FUNCTION__);
    }
    else if (rc == 0) {
      // Timeout
    } else {
      new_sockfd = ::accept(_sockfd, reinterpret_cast<struct sockaddr*>(&inetSockClient), &sock_in_len);
      if (new_sockfd < 0) {
        handleError(__PRETTY_FUNCTION__);
      }
      Inet sockInet(inetSockClient);
      // printf("Incoming connection... %s\n", sockInet.readIp());
      handleIncoming(new_sockfd, sockInet);
      fds.revents = 0;
    }
  } while(_msgRecv);
  // printf("_sockfd = %i \"%s\" -- END -- \n", _sockfd, __PRETTY_FUNCTION__);
}
void Server::handleIncoming(int new_sockfd, const Inet& inet) {
  std::unique_ptr<Socket> sock(new Socket(new_sockfd, sockInet));
  sock->open(messageHandler);
  storeSocket_p(sockInet, sock);
  addClient();
}
void Server::storeSocket_p(const Inet& inet, std::unique_ptr<Socket> &sock_p) {
  muSockStorage.lock();
  socketStorage.emplace(
      std::make_pair(
        Inet(inet), 
        std::unique_ptr<Socket>(std::move(sock_p))
        )
      );
  muSockStorage.unlock();
}
void Server::addClient() {
  ++numClient;
}
void Server::delClient() {
  --numClient;
}
