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
#include <unordered_map>

class httpReq {
  public:
    httpReq();
    ~httpReq();
    // Map of http header 
    std::string header(const std::string);
    std::string parameter(const std::string);
    // Print conf
    std::string getBody() {
      return body;
    }
    void print();
    // Process request
    http::Code process(const std::string& buffer);
  private:
    // Process first line
    // Then header
    http::Code processHeader(const std::string& line);
    // Set field in map
    void processField(const std::string& line);
    void processParams(const std::string& params);
    // Helper to process params
    void addMultiValueParameters(const std::string& field, const std::string& values);
    // Local map
    std::unordered_map<std::string, std::string> headers;
    std::map<std::string, std::string> params;
    // std::multimap<std::string, std::string> params;
    std::string body;
};
