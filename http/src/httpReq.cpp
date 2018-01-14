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
#include "httpReq.hpp"
#include <iostream>
//
httpReq::httpReq() {}
httpReq::~httpReq() {}
//
void httpReq::print() {
  std::cout << header("Method") << " /" << header("Ressource");
  bool firstParam = true;
  for(auto it:params) {
    if(firstParam) {
      firstParam = false;
      std::cout << "?";
    }
    std::cout << "&" << it.first << "=" << it.second;
  }
  std::cout << " HTTP/" << header("Version") << std::endl; 
  for(auto it:headers) {
    if(it.first != "Method" && it.first != "Ressource" && it.first != "Version")
      std::cout << it.first << " : " << it.second << std::endl;
  }
  std::cout << std::endl;
  // std::cout << "________________________________" << std::endl;
  if(!body.empty()) {
    std::cout << body << std::endl;
    // std::cout << "________________________________" << std::endl;
  }
}
//
std::string httpReq::header(const std::string h) {
  if(headers.count(h)>0) {
    return headers[h];
  } else {
    return std::string("");
  }
}
//
std::string httpReq::getBody() {
  return body;
}
//
std::queue<std::string> httpReq::parameter(const std::string h) {
  std::queue<std::string> queue;
  if(params.count(h)>0) {
    auto range = params.equal_range(h);
    for(auto it=range.first; it!=range.second;++it) {
      queue.emplace(it->second);
    }
  }
  return queue;
}
//
http::Code httpReq::process(const std::string& buffer) {
  http::Code result = http::OK;
  // size_t length = buffer.size();
  size_t endHeader = buffer.find(endOf);
  if(endHeader ==  std::string::npos) {
    result = http::BAD_REQUEST;
  }
  result = processHeader(buffer.substr(0, endHeader));
  body = buffer.substr(endHeader+endOf.size());
  size_t endBody = body.find(endOf);
  if(endBody == std::string::npos) {
    body = "";
  } else {
    body = body.substr(0,endBody);
    if(header("Method") == "POST") {
      processParams(body);
    }
  }
  return result;
}
//
void httpReq::processParams(const std::string& param) {
  std::string remainingParams(param);
  std::string field;
  std::string value;
  size_t paramPos = remainingParams.find(paramSep);
  while(paramPos != std::string::npos) { 
    field = remainingParams.substr(0,paramPos);
    remainingParams = remainingParams.substr(paramPos+paramSep.size());
    paramPos = remainingParams.find(paramSep);
    size_t fieldPos = field.find(valueSep);
    value = field.substr(fieldPos+valueSep.size());
    field = field.substr(0, fieldPos);
    addMultiValueParameters(field, value);
  }
  field = remainingParams.substr(0,paramPos);
  size_t fieldPos = field.find(valueSep);
  value = field.substr(fieldPos+valueSep.size());
  field = field.substr(0, fieldPos);
  addMultiValueParameters(field, value);
}
//
void httpReq::addMultiValueParameters(const std::string& field, const std::string& multiValues) {
  std::string value(multiValues);
  size_t multiValuePos = value.find(multiValueSep);
  if(multiValuePos == std::string::npos) {
    params.insert(std::make_pair(field, value));
  } else {
    std::string localValue = value.substr(0, multiValuePos);
    params.insert(std::make_pair(field, localValue));
    value = value.substr(multiValuePos+multiValueSep.size());
    addMultiValueParameters(field, value);
  }
}
//
http::Code httpReq::processHeader(const std::string& header) {
  http::Code result = http::OK;
  size_t firstLinePos = header.find(eol);
  std::string firstLine = header.substr(0, firstLinePos);
  {
    // Process first firstLine
    std::size_t pos = firstLine.find("/"); 
    std::string requestType = firstLine.substr(0, pos-1);
    if(restMethods.count(requestType)<=0) {
      // Unknwon methods
      result = http::BAD_REQUEST;
    }
    headers["Method"]   = requestType;
    //
    std::string tmp = firstLine.substr(pos);
    std::size_t typePos = tmp.find(" "); 
    // Remove / and " " 
    std::string path = tmp.substr(1, typePos-1);
    if(requestType == "GET") {
      size_t paramPos = path.find("?");
      if(paramPos != std::string::npos) {
        std::string params = path.substr(paramPos+1);
        path = path.substr(0, paramPos);
        headers["Ressource"] = path;
        processParams(params);
      } else {
        headers["Ressource"] = path;
      }
    } else {
      headers["Ressource"] = path;
    }
    std::size_t versionPos= firstLine.find(HTTP); 
    std::string version = firstLine.substr(versionPos);
    std::size_t versNumberPos = version.find("/");
    std::string type = version.substr(0,versNumberPos);
    std::string versionNumber = version.substr(versNumberPos+1);
    // headers["Protocol"] = HTTP+"/"+versionNumber;
    headers["Version"] = versionNumber;
  }
  {
    std::string line = header.substr(firstLinePos+eol.size());
    std::string content;
    // Process rest of the header
    bool lastLine = false;
    while(!lastLine) {
      size_t pos = line.find(eol);
      if( pos == std::string::npos) {
        // No more new line
        lastLine = true;
        break;
      }
      content = line.substr(0, pos);
      processField(content);
      line = line.substr(pos+eol.size());
    }
  }
  return result;
}
//
void httpReq::processField(const std::string& line) {
  std::size_t pos = line.find(":");
  if(pos != std::string::npos) {
    std::string field;
    std::string data;
    field = line.substr(0,pos);
    data = line.substr(pos+2);
    // std::cout << __func__ << "headers[" << field << "] ("<< data.size() << ") = \"" << data << "\"" << std::endl; 
    headers[field] = data;
  }
}
