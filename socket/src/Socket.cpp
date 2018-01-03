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
#include "Socket.hpp"
#include "socket_error.hpp"
#include <errno.h>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <cstring>
//
void defaultMsgHandler(Socket* sock) {
  char buffer[1024];
  int length = sock->recv(buffer, 1024);
  printf("Socke::recv(%i bytes)... %s\n",length, buffer);
}

//
void handleError(const char* func, bool throwExcept) {
  std::string err(func);
  err += " failed";
  perror(err.c_str());
  if(throwExcept) {
    throw socket_error(err.c_str());
  }
}
// Constructor
Socket::Socket() :
  sockInet()
{
  this->_hasConnected = false;
  this->_listening = false;
  this->_msgRecv = false;
  this->_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if(_sockfd == -1) {
    handleError(__PRETTY_FUNCTION__);
  }
}
Socket::Socket(uint16_t port, const char* ip): 
  sockInet(ip, port)
{
  this->_hasConnected = false;
  this->_listening = false;
  this->_msgRecv = false;
  this->_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if(_sockfd == -1) {
    handleError(__PRETTY_FUNCTION__);
  }
}
Socket::Socket(const Inet& inet) :
  sockInet(inet) 
{
  this->_hasConnected = false;
  this->_listening = false;
  this->_msgRecv = false;
  this->_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if(_sockfd == -1) {
    handleError(__PRETTY_FUNCTION__);
  }
}
// Construct around an already formed file descriptor
Socket::Socket(int sd, uint16_t port, const char* ip): 
  sockInet(ip, port)
{
  this->_hasConnected = false;
  this->_listening = false;
  this->_msgRecv = false;
  this->_sockfd = sd ;
  if(_sockfd == -1) {
    handleError(__PRETTY_FUNCTION__);
  }
}
Socket::Socket(int sd, const Inet& inet) :
  sockInet(inet) 
{
  this->_hasConnected = false;
  this->_listening = false;
  this->_msgRecv = false;
  this->_sockfd = sd; 
  if(_sockfd == -1) {
    handleError(__PRETTY_FUNCTION__);
  }
}
// TODO
Socket::~Socket() {
  if(_listening) {
    close();
  }
}
// get and clear any error
int Socket::getSocketErrno() {
  int err = 1;
  socklen_t len = sizeof err;
  if(getsockopt(_sockfd, SOL_SOCKET, SO_ERROR, (char *)&err, &len) == -1) {
    handleError(__PRETTY_FUNCTION__);
    err = errno;
  }
  return err;
}
// Connect socket
int Socket::connect() {
  if(!_hasConnected) {
    // printf("_sockfd = %i \"%s\" \n", _sockfd, __PRETTY_FUNCTION__);
    struct sockaddr_in s_in;
    memset(&s_in,0,sizeof(s_in));
    s_in.sin_family = AF_INET;
    s_in.sin_port = this->sockInet.getInPort();
    s_in.sin_addr = this->sockInet.getInAddr();
    // memcpy(&s_in, sockInet.getSaddrIn_p(), sizeof(sockInet.getSaddrIn()));
    if(::connect(this->_sockfd, reinterpret_cast<struct sockaddr*>(&s_in), sizeof(s_in)) == -1) {
      handleError(__PRETTY_FUNCTION__);
      return errno;
    }
    _hasConnected = true;
  }
  return 0;
}
// Close socket
void Socket::close() {
  // printf("_sockfd = %i \"%s\" \n", _sockfd, __PRETTY_FUNCTION__);
  _msgRecv = false;
  if(listenThread.joinable()) {
    listenThread.join();
  }
  _listening = false;

}
// Read the socket
int Socket::recv(void* buffer, size_t maxSize) {
  // printf("_sockfd = %i \"%s\" \n", _sockfd, __PRETTY_FUNCTION__);
  int length = 0;
  length = ::recv(_sockfd,buffer, maxSize, 0);
  if(length == -1) {
    handleError(__PRETTY_FUNCTION__);
    return length;
  } else if(length == 0) {
    // Close connection
    _msgRecv = false;
  }
  return length;
}
// Peek at the socket
int Socket::peek(void* buffer, size_t maxSize) {
  int length = 0;
  length = ::recv(_sockfd,buffer, maxSize, MSG_PEEK);
  if(length == -1) {
    handleError(__PRETTY_FUNCTION__);
    return length;
  }
  return length;
}
// Send buffer through the socket
int Socket::send(const void* data, size_t maxSize) {
  int length = 0;
  if(maxSize > 0) {
    length = ::send(_sockfd, data, maxSize, MSG_NOSIGNAL);
    if(length == -1) {
      handleError(__PRETTY_FUNCTION__, HANDLE_ERROR_NO_THROW);
      return length;
    }
  }
  return length;
}
void Socket::open(std::function<void(Socket*)> msgHandler) {
  if(!_listening) {
    messageHandler = msgHandler;
    _msgRecv = true;
    _listening = true;
    listenThread = std::thread(&Socket::listenImpl, this);
  }
}
// Listen function
void Socket::listen(std::function<void(Socket*)> msgHandler) {
  this->open(msgHandler);
  waitListening();
}
void Socket::listen() {
  this->open();
  waitListening();
}
void Socket::waitListening() {
  struct timespec req;
  struct timespec rem;
  req.tv_sec = 0;
  req.tv_nsec = 5*1e8;
  do {
    nanosleep(&req, &rem);
  } while(_listening);
  // Close
  if(_msgRecv) {
    _msgRecv = false;
  }
}
// Thread functions
void Socket::listenImpl() {
  // printf("_sockfd = %i \"%s\" -- BEGIN -- \n", _sockfd, __PRETTY_FUNCTION__);
  struct pollfd fds;
  fds.fd = _sockfd;
  fds.events = POLLIN;
  int timeout = (500); // 0.5 sec
  int rc = 0;
  do {
    rc = poll(&fds, 1, timeout);
    if (rc < 0) {
      handleError(__PRETTY_FUNCTION__);
    } else if (rc == 0) {
      // Timeout
    } else if(fds.revents ==0) {
    } else if(fds.revents & POLLIN) {
      if(_msgRecv)
        this->messageHandler(this);
      fds.revents = 0;
    } else {
      // Error case
    }
  } while(_msgRecv);
  if(_sockfd >= 0) {
    getSocketErrno(); // first clear any errors, which can cause close to fail
    if (::shutdown(_sockfd, SHUT_RDWR) <= -1) {// secondly, terminate the 'reliable' delivery
      if (errno != ENOTCONN && errno != EINVAL) {// SGI causes EINVAL
        handleError(__PRETTY_FUNCTION__);
      }
      if (::close(_sockfd) <= -1) {// finally call close()
        handleError(__PRETTY_FUNCTION__);
      }
    }
  }
  // printf("_sockfd = %i \"%s\" -- END -- \n", _sockfd, __PRETTY_FUNCTION__);
}
