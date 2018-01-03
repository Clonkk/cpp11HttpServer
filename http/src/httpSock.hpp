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
#include "Inet.hpp"
class httpReq;
class httpRes;
class httpServer;

class httpSock:public Socket {
  public:
    httpSock(int fd, const Inet& inet, httpServer* serv);
    virtual ~httpSock();
    bool isWebSocket();
    void upgradeToWs(std::function<std::string(const std::string&)> func);
    void send(httpRes& res);
		void handleWebSocketMessage(const std::string& rawMsg);
  private:
    //
    httpServer* server;
    volatile bool isWs;
    volatile unsigned int _opCode;
    std::function<std::string(const std::string& msg)> webSocketMessageHandler;
		std::string encodeWsMessage(const std::string& rawMsg);
		std::string decodeWsMessage(const std::string& rawMsg);
};
