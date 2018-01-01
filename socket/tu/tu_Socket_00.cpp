#include <iostream>
#include <cstring>
#include <csignal>
#include <thread>
#include "Socket.hpp"
#include "Server.hpp"
#include "unistd.h"

// const char* const IP = "127.0.0.1";
const char* const IP = "192.168.1.12";
const uint16_t PORT = 10175;

//Global variable
volatile bool goOn= true;
// Sig handler
Server *server;
//
void sighandler(int theSignal) {
  goOn = false;
}
void printServer(Socket* sock) {
  char buffer[360];
  int length;
  length = sock->recv(buffer, 360);
  printf("%s\n", buffer);
}
//
void serverFunc() {
  server->listen(printServer);
}
//
int main() {
  struct sigaction action;
  /* Reset Sigaction. */
  std::memset(&action, 0, sizeof(action));        // Empty mask by default/
  action.sa_handler   = sighandler;           // Set the handler function.
  action.sa_flags     = 0;                    // Remove any flag from sa_flag.
  sigaction(SIGINT, &action, nullptr);

  server = new Server(PORT, IP);
  server->bind();
  // server->open(printServer);
  std::thread t0(serverFunc);
  while(goOn);
  server->close();
  t0.join();
  delete server;
}
