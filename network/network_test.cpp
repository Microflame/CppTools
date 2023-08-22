#include <iostream>
#include <sstream>
#include <thread>

#include <misc/exception.hpp>

#include "network.hpp"

using vtools::Socket;
using vtools::SockResult;

int main() {
    Socket server;
    server.Bind("localhost", 7880);
    server.Listen(8);
    Socket client = server.Accept();

    std::string buffer(4096, '\0');
    SockResult res = client.Recv(buffer.data(), buffer.size());
    VTOOLS_ASSERT(res);
    buffer.resize(res.size);

    std::cout << "Recv:\n" << buffer << '\n';

    std::string response_headers =
        "HTTP/1.0 200 OK\r\n"
        // Server: BaseHTTP/0.6 Python/3.10.12
        // Date: Tue, 22 Aug 2023 00:33:04 GMT
        "Content-type: text/html\r\n"
        "\r\n"
    ;
    client.SendAll(response_headers.data(), response_headers.size());

    std::string word;
    std::stringstream ss("A quick brown fox jumps over the lazy dog");
    char space = ' ';
    while (std::getline(ss, word, ' ')) {
        client.SendAll(word.data(), word.size());
        client.SendAll(&space, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    client.Reset();
    return 0;
}
