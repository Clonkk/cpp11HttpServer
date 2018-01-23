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
#include "httpRes.hpp"
#include "encode64.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <openssl/sha.h>
#include <cassert>
httpRes::httpRes() {
  httpVersion = "HTTP/1.1";
  // Some default header
}
//
void httpRes::handleWsResponse(httpReq& req) {
  std::string str;
  httpCode = http::SWITCHING_PROTOCOLS;
  addHeader("Upgrade", std::string("websocket"));
  addHeader("Connection", std::string("Upgrade"));
  // Calculate handshake
  std::string wsKey = calculateHandshake(req.header("Sec-WebSocket-Key"));
  assert(!wsKey.empty());
  addHeader("Sec-WebSocket-Accept", wsKey);
  content= "";
}
void httpRes::setHeader(const std::string& field, const std::string& value) {
  resHeader[field] = value;
}
void httpRes::addHeader(const std::string& field, const std::string& value) {
  if(resHeader.count(field) == 0) {
    resHeader[field] = value;
  } else {
    std::string existingValue = resHeader[field];
    resHeader[field] = existingValue+", "+value;
  }
}
//
std::string httpRes::calculateHandshake(const std::string& key) {
  std::string wsKey = key+magic;
  // Calculate sha1sum
  unsigned char hash[20];
  SHA1(
      reinterpret_cast<const unsigned char*>(wsKey.c_str()),
      wsKey.size(),
      hash
      );
  // encode on 64-bit string
  std::string encoded = base64_encode(hash, 20);
  return encoded;
}
void httpRes::setHttpCode(http::Code code) {
  httpCode = code;
}
void httpRes::setContent(const std::string& userContent) {
  content = userContent;
}
std::string httpRes::formatResponse() {
  std::string str;
  str += httpVersion+" "+std::to_string(httpCode)+" "+httpCodeMap[httpCode]+eol;
  for(auto it: resHeader) {
    str+= it.first+" : "+it.second+eol;
  }
  str += eol;
  if(!content.empty()) {
    str += content+eol;
    str += eol;
  }
  return str;
}
std::string httpRes::header(const std::string& h) {
  try {
    return resHeader.at(h);
  } catch(std::out_of_range &e) {
    return std::string("");
  }
}
