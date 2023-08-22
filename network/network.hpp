#pragma once

#include <string_view>

namespace vtools
{

struct SockResult {
    size_t size;
    enum Type {
        OK,
        DISCONNECTED,
        BROKEN,
        INSUFFICIENT_BUFFER,
        WOULD_BLOCK
    } error;

    operator bool() const { return error == OK; }
};

class Socket {
    int fd_ = 0;

public:
    Socket();
    Socket(int fd);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other);
    ~Socket();

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other);

    void Swap(Socket& other);
    void Reset();

    void Connect(const char* host, int port);
    void Bind(const char* host, int port);
    void Listen(int queue_size) const;
    Socket Accept() const;

    void SetReuseAddr() const;
    void SetNoDelay() const;

    SockResult SendAll(const char* data, size_t size) const;
    SockResult Recv(char* data, size_t size) const;
};

} // namespace vtools
