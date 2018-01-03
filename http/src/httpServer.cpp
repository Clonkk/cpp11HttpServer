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
#include "httpServer.hpp"
#include "httpValue.hpp"
#include "httpReq.hpp"
#include "httpRes.hpp"
#include <cstring>
void defaultHttpServerFunction(Socket*) {}

httpServer::httpServer(uint16_t port, const char* ip):Server(port, ip) {
}
httpServer::httpServer(const Inet& inet) : Server(inet) {
}
httpServer::~httpServer() {
}
void httpServer::open(std::function<void(Socket*)> func) {
  std::function<void(Socket*)> localRequestHandler = std::bind(&httpServer::requestHandler, this, std::placeholders::_1);
  Server::open(localRequestHandler);
}
void httpServer::requestHandler(Socket* s) {
  httpSock* socket = dynamic_cast<httpSock*>(s);
  char buffer[4096];
  int length;
  length = socket->recv(buffer, 4096);
  if(length > 0) {
    std::string strReq;
    strReq.assign(buffer, length);
    if(socket->isWebSocket()==false) {
      httpRequestHandler(socket, strReq);
    } else {//if(socket->isWebSocket == true) 
      webSocketRequestHandler(socket, strReq);
    }
  } else {
    // Error on receive
    // Ignore request
    return;
  }
}
void httpServer::webSocketRequestHandler(httpSock* socket, const std::string& strReq) {
  // Decode message
  // Execute callback
  // encode result
  // send
  socket->handleWebSocketMessage(strReq);
}
void httpServer::httpRequestHandler(httpSock* socket, const std::string& strReq) {
  httpReq req;
  httpRes res;
  // Normal http request
  http::Code result = req.process(strReq);
  // req.print();
  if(result != http::OK) {
    // Return http::BAD_REQUEST in httpRes
    res.setHttpCode(result);
    socket->send(res);
    // send res
  } else {
    if(req.header("Method") == "GET" && req.header("Connection") == "Upgrade" && req.header("Upgrade") == "websocket") {
      if(checkWsRessource(req.header("Ressource"))) {
        // Websocket case
        res.setHttpCode(http::SWITCHING_PROTOCOLS);
        res.handleWsResponse(req);
        socket->send(res);
        socket->upgradeToWs(wsCallMap[req.header("Ressource")]); 
      } else {
        // Ressource no found
        res.setHttpCode(http::NOT_FOUND);
        socket->send(res);
      }
    }else {
      res.setHeader("Content-Length", std::to_string(req.getBody().size()));
      res.setHeader(std::string("Access-Control-Allow-Origin"),std::string("*"));
      res.setHeader("Content-Type", "text/plain");
      if(req.header("Connection")=="keep-alive") {
        res.addHeader("Connection", req.header("Connection"));
      }
      if(checkRestRessource(req.header("Method"), req.header("Ressource"))) {
        // Normal http callback
        // Send an accepting response to indicate request is ok ?
        res.setHttpCode(http::OK);
        // Execute http callack
        executeHttpCallback(req, res);
        socket->send(res);
      } else {
        // Ressource no found
        res.setHttpCode(http::NOT_FOUND);
        socket->send(res);
      }
    }
  }
}
bool httpServer::checkRestRessource(const std::string& method, const std::string& ressource) {
  if(httpCallMap.count(ressource) > 0) {
    return true;
  } else {
    return false;
  }
}
bool httpServer::checkWsRessource(const std::string& ressource) {
  if(wsCallMap.count(ressource)>0) {
    return true;
  }  else {
    return false;
  }
}
void httpServer::handleIncoming(int new_sockfd, const Inet& inet) {
  std::unique_ptr<httpSock> p_httpSock(new httpSock(new_sockfd, inet, this));
  p_httpSock->open(messageHandler);
  std::unique_ptr<Socket> p_sock(std::move(p_httpSock));
  storeSocket_p(inet, p_sock);
  Server::addClient();
}
void httpServer::addWsFunction(const std::string& path, wsCallback func) {
  wsCallMap[path] = func;
}
void httpServer::addRestFunction(const std::string& path, httpCallback func) {
  std::string fullRessource = "GET "+path; 
  httpCallMap[fullRessource] = func;
}
void httpServer::addRestFunction(const std::string& method, const std::string& path, httpCallback func) {
  std::string fullRessource = method+" "+path; 
  httpCallMap[fullRessource] = func;
}
void httpServer::executeHttpCallback(httpReq& req, httpRes& res) {
  std::string ressourceKey = req.header("Method")+" "+req.header("Ressource");
  httpCallMap[ressourceKey](req, res);
}
