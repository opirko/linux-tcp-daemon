#include "Server.hpp"

#include <syslog.h>

#include <chrono>
#include <thread>

namespace tcpdae {

using asiotcp = boost::asio::ip::tcp;

// ==================== PUBLIC ====================

Server::Server() : mAcceptor(mContext, asiotcp::endpoint(asiotcp::v4(), 5001)) { startAccept(); }

void Server::run() {
    syslog(LOG_DEBUG, "Doing work");
    std::async(std::launch::async, [this]() {
        std::this_thread::sleep_for(std::chrono::seconds{15});
        syslog(LOG_DEBUG, "Stopping ASIO IO context");
        mContext.stop();
    });
    // blocks current thread
    mContext.run();
    syslog(LOG_DEBUG, "Done work");
}

// ==================== PRIVATE ====================

void Server::startAccept() {
    syslog(LOG_DEBUG, "startAccept start");
    Connection::ptr new_connection = Connection::create(mContext);

    mAcceptor.async_accept(new_connection->socket(),
                           boost::bind(&Server::handleAccept, this, new_connection, boost::asio::placeholders::error));
    syslog(LOG_DEBUG, "startAccept end");
}

void Server::handleAccept(Connection::ptr new_connection, const boost::system::error_code& error) {
    syslog(LOG_DEBUG, "handleAccept start");
    if (!error) {
        new_connection->start();
    }
    startAccept();
    syslog(LOG_DEBUG, "handleAccept end");
}

}  // namespace tcpdae