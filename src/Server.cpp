#include "Server.hpp"

#include <syslog.h>

#include <chrono>
#include <thread>

namespace tcpdae {

using asiotcp = boost::asio::ip::tcp;

constexpr char Server::kClassName[];
constexpr int Server::kPort;

// ==================== PUBLIC ====================

// construct socket, start listening and accepting
Server::Server() : mAcceptor(mContext, asiotcp::endpoint(asiotcp::v4(), kPort)) {
    mAcceptor.listen();
    startAccept();
}

void Server::run() {
    // Add signals to process termination
    boost::asio::signal_set signals(mContext);
    signals.add(SIGTERM);
    signals.add(SIGINT);
    // Async wait for signals to occur
    signals.async_wait([this](const boost::system::error_code&, int) { mContext.stop(); });

    syslog(LOG_DEBUG, "Running ASIO IO context");
    // blocks current thread
    mContext.run();
}

// ==================== PRIVATE ====================

void Server::startAccept() {
    syslog(LOG_DEBUG, "%s::%s", kClassName, __func__);
    auto con = std::make_shared<Connection>(mContext);
    mAcceptor.async_accept(con->getSocket(),
                           boost::bind(&Server::handleAccept, this, con, boost::asio::placeholders::error));
}

void Server::handleAccept(std::shared_ptr<Connection> con, const boost::system::error_code& error) {
    syslog(LOG_DEBUG, "%s::%s", kClassName, __func__);
    if (!error) {
        con->start();
    }
    // accept other connections as well
    startAccept();
}

}  // namespace tcpdae