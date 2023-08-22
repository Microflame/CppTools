#include "network.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>

#include <misc/defer.hpp>
#include <misc/exception.hpp>

namespace vtools
{

static int SetFileFlag(int fd, int flag) {
    int old = fcntl(fd, F_GETFL);
    if (old == -1) return -1;
    return fcntl(fd, F_SETFL, old | flag);
}

static int SetSockOpt(int fd, int opt, int level, int enable = 1) {
    return setsockopt(fd, level, opt, &enable, sizeof(enable));
}

static in_addr_t ResolveHostname(const char* hostname) {
    addrinfo* getaddrinfo_res = nullptr;
    addrinfo hints = {};
    hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    VTOOLS_THROW_IF_POSIX_ERR(getaddrinfo(hostname, nullptr, &hints, &getaddrinfo_res));
    VTOOLS_DEFER { freeaddrinfo(getaddrinfo_res); };

    VTOOLS_ASSERT(getaddrinfo_res);
    VTOOLS_ASSERT(getaddrinfo_res->ai_addr);
    in_addr_t res = {};
    VTOOLS_THROW_IF_POSIX_ERR(inet_pton(AF_INET, getaddrinfo_res->ai_addr->sa_data, &res));
    return res;
}

static sockaddr_in MakeSockaddr(const char* hostname, int port) {
    sockaddr_in sockaddr = {};
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = ResolveHostname(hostname);
    return sockaddr;
}

static SockResult SendAll(int fd, const char* data, size_t size) {
    size_t num_sent_total = 0;
    while (num_sent_total < size) {
        int num_sent = send(fd, data + num_sent_total, size - num_sent_total, MSG_NOSIGNAL);
        VTOOLS_ASSERT(num_sent != 0);
        if (num_sent == -1) {
            switch (errno) {
#if EAGAIN != EWOULDBLOCK
                case EAGAIN:
#endif
                case EWOULDBLOCK: return { num_sent_total, SockResult::WOULD_BLOCK };
                case ECONNRESET: return { num_sent_total, SockResult::DISCONNECTED };
            }
            VTOOLS_THROW_IF_POSIX_ERR(-1);
        }
        num_sent_total += num_sent;
    }
    return { num_sent_total, SockResult::OK };
}

static SockResult Recv(int fd, char* data, size_t size) {
    int num_recv = recv(fd, data, size, 0);

    if (num_recv == 0) {
        return { 0, SockResult::DISCONNECTED };
    }
    if (num_recv == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return { 0, SockResult::WOULD_BLOCK };
        }
        VTOOLS_THROW_IF_POSIX_ERR(-1);
    }

    return { size_t(num_recv), SockResult::OK };
}


Socket::Socket() {}

Socket::Socket(int fd) : fd_(fd) {}

Socket::Socket(Socket&& other) {
    Swap(other);
}

Socket::~Socket() {
    Reset();
}


Socket& Socket::operator=(Socket&& other) {
    Swap(other);
    return *this;
}

void Socket::Swap(Socket& other) {
    std::swap(fd_, other.fd_);
}

void Socket::Reset() {
    if (fd_) {
        close(fd_);
        fd_ = 0;
    }
}

void Socket::Connect(const char* host, int port) {
    Reset();
    VTOOLS_THROW_IF_POSIX_ERR(fd_ = socket(AF_INET, SOCK_STREAM, 0));
    sockaddr_in server_sockaddr = MakeSockaddr(host, port);
    VTOOLS_THROW_IF_POSIX_ERR(connect(fd_, (const struct sockaddr*) &server_sockaddr, sizeof(server_sockaddr)));
}

void Socket::Bind(const char* host, int port) {
    Reset();
    VTOOLS_THROW_IF_POSIX_ERR(fd_ = socket(AF_INET, SOCK_STREAM, 0));
    SetReuseAddr(); // TODO: fix
    sockaddr_in server_socked_addr = MakeSockaddr(host, port);
    VTOOLS_THROW_IF_POSIX_ERR(bind(fd_, (const struct sockaddr*) &server_socked_addr, sizeof(server_socked_addr)));
}

void Socket::Listen(int queue_size) const {
    VTOOLS_THROW_IF_POSIX_ERR(listen(fd_, queue_size));
}

Socket Socket::Accept() const {
    int client_fd = 0;
    VTOOLS_THROW_IF_POSIX_ERR(client_fd = accept(fd_, nullptr, nullptr));
    return Socket(client_fd);
}


void Socket::SetReuseAddr() const {
    VTOOLS_THROW_IF_POSIX_ERR(SetSockOpt(fd_, SO_REUSEADDR, SOL_SOCKET));
}

void Socket::SetNoDelay() const {
    VTOOLS_THROW_IF_POSIX_ERR(SetSockOpt(fd_, TCP_NODELAY, IPPROTO_TCP));
}


SockResult Socket::SendAll(const char* data, size_t size) const {
    return ::vtools::SendAll(fd_, data, size);
}

SockResult Socket::Recv(char* data, size_t size) const {
    return ::vtools::Recv(fd_, data, size);
}


} // namespace vtools
