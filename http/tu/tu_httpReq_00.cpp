#include "httpReq.cpp"

const std::string request = 
std::string("GET /test?p1=12&p2=13 HTTP/1.1\r\n")+
std::string("Host: 192.168.1.12:10175\r\n")+
std::string("Connection: keep-alive\r\n")+
std::string("User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36\r\n")+
std::string("Upgrade-Insecure-Requests: 1\r\n")+
std::string("Accept: text/html,application/xhtml+xml\r\n")+
std::string("Accept-Encoding: gzip, deflate\r\n")+
std::string("Accept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\r\n")+
std::string("\r\n");
int main() {
  std::cout << "________________________________" << std::endl;
  std::cout << request;
  std::cout << "________________________________" << std::endl;
  httpReq req;
  req.process(request);
  req.print();
}
