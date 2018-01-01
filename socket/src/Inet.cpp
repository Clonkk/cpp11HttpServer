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
#include "Inet.hpp"
#include <cstring>
#include <cassert>

Inet::Inet() {
  std::memset(&s_in, '\0', sizeof(s_in));
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(defaultPort);
  int retVal = inet_aton("0.0.0.0", &s_in.sin_addr);
  assert(retVal == 1 && "Error in inet_aton wrong IP");
}
Inet::Inet(const char* ip, uint16_t port) {
  std::memset(&s_in, '\0', sizeof(s_in));
  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(port);
  int retVal = inet_aton(ip, &s_in.sin_addr);
  assert(retVal == 1 && "Error in inet_aton wrong IP");
}
Inet::Inet(struct sockaddr_in socketAdress) {
  std::memset(&s_in, '\0', sizeof(s_in));
  std::memcpy(&(this->s_in), &(socketAdress), sizeof(s_in));
  s_in.sin_family = AF_INET;
}
Inet::Inet(const Inet& inet) {
  std::memset(&s_in, '\0', sizeof(s_in));
  std::memcpy(&(this->s_in), &(inet.s_in), sizeof(s_in));
}
Inet::~Inet() {}
const char* Inet::getIpv4() const {
  return inet_ntoa(s_in.sin_addr);
}
uint16_t Inet::getPort() const {
  return ntohs(s_in.sin_port);
}
const char* Inet::readIp() const {
  static char buf[23];
  snprintf(buf, 23, "%s:%i",this->getIpv4(), this->getPort());
  return buf;
}
struct sockaddr_in Inet::getSaddrIn() {
  return s_in;
}
struct sockaddr_in * Inet::getSaddrIn_p() {
  return &s_in;
}
struct in_addr Inet::getInAddr() {
  return s_in.sin_addr;
}
in_port_t Inet::getInPort() {
  return s_in.sin_port;
}
void Inet::setIpv4(const char* ip) {
  int retVal = inet_aton(ip, &s_in.sin_addr);
  assert(retVal == 1 && "Error in inet_aton wrong IP");
}
void Inet::setPort(uint16_t port) {
  s_in.sin_port = htons(port);
}
Inet& Inet::operator=(const Inet& inet) {  
  std::memcpy(&(this->s_in), &(inet.s_in), sizeof(s_in));
  return *this;
}
Inet& Inet::operator=(struct sockaddr_in socketAdress) {
  std::memcpy(&(this->s_in), &(socketAdress), sizeof(s_in));
  s_in.sin_family = AF_INET;
  return *this;
}
bool operator==(const Inet& lhs, const Inet& rhs) {
  if(lhs.getPort() != rhs.getPort()) {
    return false;
  } else if(strcmp(lhs.getIpv4(), rhs.getIpv4())==0) {
    return true;
  } else {
    return false;
  }
}
