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
#include "httpSock.hpp"
#include "httpServer.hpp"
#include "httpReq.hpp"
#include "httpRes.hpp"
#include <vector>

httpSock::httpSock(int fd, const Inet& inet, httpServer* serv_):Socket(fd, inet) {
	server = serv_;
	isWs = false;
}
httpSock::~httpSock() {
}
bool httpSock::isWebSocket() {
	return isWs;
}
void httpSock::upgradeToWs(std::function<std::string(const std::string&)> func) {
	isWs = true;
	webSocketMessageHandler = func;
}
// #include <iostream>
void httpSock::send(httpRes& res) {
	std::string strRes = res.formatResponse();
  // std::cout << "___________________________" << std::endl;
  // std::cout << strRes; 
  // std::cout << "___________________________" << std::endl;
	Socket::send(strRes.c_str(),strRes.size());
}
std::string httpSock::encodeWsMessage(const std::string& rawMsg) {
	// Encode a message
	std::string encodedMsg("");
	// 129 is the code for text message
	size_t rawMsgSize = rawMsg.size();
	size_t indexRawMsg;
	encodedMsg.push_back(129);
	if(rawMsgSize<=125){
		// Size can be encoded on 2 byte
		indexRawMsg = 2;
		encodedMsg.push_back(rawMsgSize);
	} else if(rawMsgSize<=65535) {
		// Size can be encoded on 4 byte
		indexRawMsg = 4;
		encodedMsg.push_back(126);
		encodedMsg.push_back((rawMsgSize >> 8*1) & 0xFF);
		encodedMsg.push_back((rawMsgSize) & 0xFF);
	} else {
		// Size will be encoded on 10 byte
		indexRawMsg = 10;
		encodedMsg.push_back(126);
		encodedMsg.push_back((rawMsgSize >> 8*7) & 0xFF);
		encodedMsg.push_back((rawMsgSize >> 8*6) & 0xFF);
		encodedMsg.push_back((rawMsgSize >> 8*5) & 0xFF);
		encodedMsg.push_back((rawMsgSize >> 8*4) & 0xFF);
		encodedMsg.push_back((rawMsgSize >> 8*3) & 0xFF);
		encodedMsg.push_back((rawMsgSize >> 8*2) & 0xFF);
		encodedMsg.push_back((rawMsgSize >> 8*1) & 0xFF);
		encodedMsg.push_back((rawMsgSize) & 0xFF);
	}
	if(encodedMsg.size()!=indexRawMsg) {
		// std::cerr << "Error in " << __func__ << std::endl;
	}
	for(size_t s=0;s<rawMsgSize;++s) {
		encodedMsg.push_back(rawMsg[s]);
	}
	// encodedMsg += rawMsg;
	return encodedMsg;
}
void httpSock::handleWebSocketMessage(const std::string& rawMsg) {
	// Decode message
	// Execute callback
	// encode result
	// send
	std::string res = 
		encodeWsMessage(
				webSocketMessageHandler(
					decodeWsMessage(rawMsg)
					)
				); 
	Socket::send(res.c_str(), res.size());
}
std::string httpSock::decodeWsMessage(const std::string& encodedMsg) {
	std::string decodedMsg;
	// Handle WS here
	unsigned int maskIndex = 2;
	unsigned int dataIndex = maskIndex+4;
	std::vector<unsigned int> mask;
	for(unsigned int i=0;i<encodedMsg.size(); ++i){
		uint8_t c = encodedMsg[i];
		unsigned int data = (unsigned int)c;
		if(i==0) {
			if(data == 129) {
				// Text message
			} else {
				// TODO Binary data
			}
		}
		if(i==1) {
			// Calculate length
			size_t length = data & 127;
			if(length == 126) {
				maskIndex = 4;
				dataIndex = maskIndex+4;
			} else if(length == 127) {
				maskIndex = 10;
				dataIndex = maskIndex+4;
			}
		} else if(i>1 && i<dataIndex) {
			if(i>=maskIndex && i<dataIndex) {
				mask.push_back(data);
			}
		} else if(i>=dataIndex) {
			unsigned int j=i-dataIndex;
			uint8_t decoded = (data xor mask[j%4]);
			decodedMsg+=decoded;
		}
	}
	return decodedMsg;
}
