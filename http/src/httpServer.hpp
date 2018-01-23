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
#include "Server.hpp"
#include "httpSock.hpp"
#include <unordered_map>
#include <string>
//
class httpReq;
class httpRes;
//
typedef std::function<void(httpReq&, httpRes&)> httpCallback;
typedef std::function<std::string(std::string)> wsCallback;
//
extern void defaultHttpServerFunction(Socket*);
//
class httpServer: public Server {
  public:
    httpServer(uint16_t port, const char* ip);
    httpServer(const Inet& intet);
    virtual ~httpServer();
    virtual void open(std::function<void(Socket*)> func = defaultHttpServerFunction);
    // callback handling with http prototype
    void addRestFunction(const std::string& path, httpCallback);
    void addRestFunction(const std::string& method, const std::string& path, httpCallback);
    void addWsFunction(const std::string& path, wsCallback);
    // Websocket asynchrone functions
    void sendWsMsg(const std::string& wsmsg);
    void sendWsMsg(const std::string& topic, const std::string& wsmsg);
    void removeWs(const std::string& topic);
  protected:
    virtual void handleIncoming(int new_sockfd, const Inet& inet);
    void requestHandler(Socket* socket);
    // callback handling pruvate functions
    void executeHttpCallback(httpReq& req, httpRes& res);
    // Sanity check
    bool checkRestRessource(const std::string& method, const std::string& ressource);
    bool checkWsRessource(const std::string& ressource);
    // handle requests parsing and format
    void httpRequestHandler(httpSock* socket, const std::string& strReq);
    void webSocketRequestHandler(httpSock* socket, const std::string& strReq);
    // data structure of ressource/callback and ressource/ip (used for asynchrone messaging in ws)
    std::unordered_map<std::string, httpCallback> httpCallMap;
    std::unordered_map<std::string, wsCallback> wsCallMap;
    // Base class has a map of <Inet, Socket> which in this case will contain the websocket instance
    // Parsing unordered_map is more efficient in complexity when you have no insertion to do but it uses more ram
    std::unordered_map<std::string, Inet> webSocketMap;
};
