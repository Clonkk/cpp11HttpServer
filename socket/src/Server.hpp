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
#include "Socket.hpp"
#include <string.h>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <memory>

constexpr int MAX_CLIENT = 30;
extern void defaultEchoHandler(Socket* sock);
class Server : public Socket {
  public:
    Server();
    Server(uint16_t port, const char* ip);
    Server(const Inet& inet);
    ~Server();
    //Override open to change the default function
    virtual void open(std::function<void(Socket* sock)> func = defaultEchoHandler);
    virtual int bind();
    virtual void listen(); 
    virtual void listen(std::function<void(Socket* sock)> func); 
  protected:
    // Implementation for server of reception
    virtual void listenImpl();
    // Handle new connection
    // Can override these in derived class to implement your own handling of new connection
    virtual void storeSocket_p(const Inet& inet, std::unique_ptr<Socket> &sock_p);
    virtual void handleIncoming(int new_sockfd, const Inet& inet);
    // Map for storing unique_ptr
    std::mutex muSockStorage;
    std::unordered_map<Inet, std::unique_ptr<Socket>> socketStorage;
    // Manage n° of client
    void addClient();
    void delClient();
    int numClient;
};
