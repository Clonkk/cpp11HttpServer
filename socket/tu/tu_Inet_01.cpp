#include "Inet.hpp"
#include <iostream>
#include <cstring>
const char* const IP = "192.168.10.134";
const uint16_t PORT = 10174;

int compareIpPort(const char* localIp, uint16_t localPort) {
  int retVal = EXIT_SUCCESS;
  if(strcmp(localIp,IP)!=0) {
    std::cout << "TEST FAILED ! " << localIp << " != " << IP << std::endl;
    retVal = EXIT_FAILURE;
  } else {
    std::cout << "TEST SUCCESSFUL ! " << localIp << " == " << IP << std::endl;
  }
  if(localPort != PORT) {
    std::cout << "TEST FAILED ! " << localPort << " != " << PORT << std::endl;
    retVal = EXIT_FAILURE;
  } else {
    std::cout << "TEST SUCCESSFUL ! " << localPort << " == " << PORT << std::endl;
  }
  return retVal;
}

int main() {
  Inet inet(IP,PORT);
  const char* localIp = inet.getIpv4();
  uint16_t localPort = inet.getPort();
  int retVal = compareIpPort(localIp, localPort);

  Inet inet2 = inet;
  localIp = inet2.getIpv4();
  localPort = inet2.getPort();
  retVal = compareIpPort(localIp, localPort);

  Inet inet3(inet2);
  localIp = inet3.getIpv4();
  localPort = inet3.getPort();
  retVal = compareIpPort(localIp, localPort);

  Inet inet4(inet.getSaddrIn());
  localIp = inet4.getIpv4();
  localPort = inet4.getPort();
  retVal = compareIpPort(localIp, localPort);
  return retVal;
}
