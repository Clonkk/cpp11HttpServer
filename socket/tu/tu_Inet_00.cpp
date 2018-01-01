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
  Inet inet;
  const char* localIp = inet.getIpv4();
  uint16_t localPort = inet.getPort();
  int retVal = EXIT_SUCCESS;
  if(strcmp(localIp,"0.0.0.0")!=0) {
    std::cout << "TEST FAILED ! " << localIp << " != 0.0.0.0" << std::endl;
    retVal = EXIT_FAILURE;
  } else {
    std::cout << "TEST SUCCESSFUL ! " << localIp << " == 0.0.0.0" << std::endl;
  }
  if(localPort != 2222) {
    std::cout << "TEST FAILED ! " << localPort << " != 2222" << std::endl;
    retVal = EXIT_FAILURE;
  } else {
    std::cout << "TEST SUCCESSFUL ! " << localPort << " == 2222" << std::endl;
  }

  inet.setIpv4(IP);
  inet.setPort(PORT);
  localIp = inet.getIpv4();
  localPort = inet.getPort();
  retVal = compareIpPort(localIp, localPort);
  return retVal;
}
