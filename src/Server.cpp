#include "Server.hpp"

#include <syslog.h>

#include <chrono>
#include <thread>

namespace tcpdae {

using asiotcp = boost::asio::ip::tcp;

// ==================== PUBLIC ====================

// construct socket, start listening and accepting
Server::Server() : mAcceptor(mContext, asiotcp::endpoint(asiotcp::v4(), 5001)) {
    mAcceptor.listen();
    startAccept();
}

void Server::run() {
    /*std::async(std::launch::async, [this]() {
        std::this_thread::sleep_for(std::chrono::seconds{15});
        syslog(LOG_DEBUG, "Stopping ASIO IO context");
        mContext.stop();
    });*/
    syslog(LOG_DEBUG, "Running ASIO IO context");
    // blocks current thread
    mContext.run();
}

// ==================== PRIVATE ====================

void Server::startAccept() {
    syslog(LOG_DEBUG, "startAccept start");
    auto con = std::make_shared<Connection>(mContext);

    mAcceptor.async_accept(con->getSocket(),
                           boost::bind(&Server::handleAccept, this, con, boost::asio::placeholders::error));
    syslog(LOG_DEBUG, "startAccept end");
}

void Server::handleAccept(std::shared_ptr<Connection> con, const boost::system::error_code& error) {
    syslog(LOG_DEBUG, "handleAccept start");
    if (!error) {
        con->start();
    }
    startAccept();
    syslog(LOG_DEBUG, "handleAccept end");
}

}  // namespace tcpdae