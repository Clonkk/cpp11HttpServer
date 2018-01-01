#include <iostream>
#include <cstring>
#include <csignal>
#include <thread>
#include "Socket.hpp"
#include "Server.hpp"
#include "unistd.h"

const char* const IP = "192.168.1.12";
const uint16_t PORT = 10174;

//Global variable
volatile bool goOn= true;
// Sig handler
Server *server;
Socket *client;
//
void sighandler(int theSignal) {
  goOn = false;
  client->close();
}
//
void clientFunc() {
  int rc = client->connect();
  printf("Connect : %i \n", rc);
  client->open();
  printf("open() \n");
  char buf[20] = "MaximiliusInDaPlace";
  int length = strlen(buf);
  while(goOn) {
    printf("Sending... \n");
    client->send(buf, length);
    sleep(2);
  }
}
void serverFunc() {
  server->listen();
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
  server->open();
  std::thread t0(serverFunc);
  sleep(2);
  client = new Socket(PORT, IP);
  std::thread t1(clientFunc);
  t1.join();
  server->close();
  t0.join();
  delete client;
  delete server;
}
