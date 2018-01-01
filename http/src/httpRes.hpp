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
#include <string>
#include "httpValue.hpp"
#include "httpReq.hpp" 

class httpRes {
  public:
    httpRes();
    ~httpRes() {}//;
    // Function to create your response
    void setHttpCode(http::Code code);
    void setContent(const std::string& userContent);
    // Append to existing header if any
    void addHeader(const std::string& field, const std::string& value);
    // Override existing header
    void setHeader(const std::string& field, const std::string& value);
    std::string formatResponse();
    // WS function
    void handleWsResponse(httpReq& req);
    // Calculate hash function of handshake
    std::string calculateHandshake(const std::string& key);
  private:
    // Attribute of response
    http::Code httpCode;
    std::string httpVersion;
    std::string content;
    std::unordered_map<std::string, std::string> resHeader;
};
