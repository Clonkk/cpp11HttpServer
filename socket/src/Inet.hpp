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
#include <functional>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
/*
 * Reminder : 
 *  struct sockaddr_in {
 *  sa_family_t    sin_family; // address family: AF_INET
 *  in_port_t      sin_port;   // port in network byte order
 *  struct in_addr sin_addr;   // internet address
 *  };
 * INADDR_LOOPBACK (127.0.0.1)
 * INADDR_ANY (0.0.0.0)
 * INADDR_BROADCAST (255.255.255.255)
 */
class Inet {
  public:
    //Default
    Inet(); // INADDR_ANY
    // Set information
    Inet(const char* address, uint16_t port);
    Inet(struct in_addr ipAddress, uint16_t port); 
    ~Inet();
    // Copy informartion 
    Inet(const Inet& inet);
    Inet(struct sockaddr_in socketAdress);
    // Operator
    Inet& operator=(const Inet& inet);
    Inet& operator=(struct sockaddr_in socketAdress);

    //Getters
    struct sockaddr_in getSaddrIn();
    // Binary getter
    in_port_t getInPort();
    struct in_addr getInAddr();
    // Human readable getter
    const char* getIpv4() const;
    uint16_t getPort() const;
    //
    const char* readIp() const;
    // Setter
    void setIpv4(const char* ip);
    void setPort(uint16_t port);
  private:
    struct sockaddr_in s_in;
    // Unsafe getter
    struct sockaddr_in * getSaddrIn_p();
    friend class Socket;
    friend class Server;
};
const uint16_t defaultPort = 2222;
bool operator==(const Inet& lhs, const Inet& rhs);
namespace std {
  template<> struct hash<Inet> { 
    std::size_t operator()(const Inet& inet) const noexcept {
      std::string ip = inet.getIpv4();
      std::size_t const h1( std::hash<std::string>{}(ip) );
      std::size_t const h2( std::hash<uint16_t>{}(inet.getPort()) );
      return h1^(h2 << 1);
    }
  };
}
