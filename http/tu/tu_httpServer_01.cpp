#include "httpServer.cpp"
#include <iostream>
#include <csignal>
#include <unistd.h>
//
volatile bool goOn = true;
void sighandler(int theSignal) {
  goOn = false;
}
const char* const IP = "192.168.1.12";
// const char* const IP = "127.0.0.1"; 
const uint16_t PORT = 10175;

std::string echoWs(const std::string& msg) {
    std::cout << "WebsocketServer says... " << msg << std::endl;
    return msg;
}
int main() {
  struct sigaction action;
  /* Reset Sigaction. */
  std::memset(&action, 0, sizeof(action));        // Empty mask by default/
  action.sa_handler   = sighandler;           // Set the handler function.
  action.sa_flags     = 0;                    // Remove any flag from sa_flag.
  sigaction(SIGINT, &action, nullptr);

  httpServer server(PORT, IP);
  server.bind();
  server.addWsFunction("echo",echoWs);
  server.open();
  while(goOn) {
    server.sendWsMsg("BROADCAST");
    server.sendWsMsg("echo", "OK_MSG");
    server.sendWsMsg("badtopic", "BAD_TOPIC");
    sleep(2);
  }
  server.close();
}
