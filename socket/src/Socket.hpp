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
#pragma once
#include "Inet.hpp"
#include <functional>
#include <thread>

// Override macro to #define no_throw true to remove throwing exception
#define HANDLE_ERROR_NO_THROW false
#define HANDLE_ERROR_THROW true 
extern void handleError(const char* func, bool throwExcept=HANDLE_ERROR_THROW);
class Socket;
extern void defaultMsgHandler(Socket* sock);

class Socket {
	public:
		Socket();
		Socket(uint16_t port, const char* ip);
		Socket(int sd, uint16_t port, const char* ip);
		Socket(const Inet& inet);
		Socket(int sd, const Inet& inet);
		virtual ~Socket();
		// return errno
		virtual void close();
    virtual void open(std::function<void(Socket* sock)> sock = defaultMsgHandler);
		virtual int connect();
		// return number of byte read / peek
		virtual int recv(void* buffer, size_t size);
		virtual int peek(void* buffer, size_t size);
		virtual int send(const void* buffer, size_t size);
		//Clear and get any error
		int getSocketErrno();
		// Wait for data 
		virtual void listen(std::function<void(Socket* sock)> func); 
		virtual void listen(); 
	protected:
    // socket parameter
		int _sockfd;
		Inet sockInet;
    // Callback func & thread
		std::function<void(Socket* sock)> messageHandler;
    std::thread listenThread;
    //Boolean falg
		volatile bool _listening;
		volatile bool _msgRecv;
    volatile bool _hasConnected;
    //
    virtual void waitListening();
		virtual void listenImpl();
};
