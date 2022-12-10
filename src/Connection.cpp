#include "Connection.hpp"

namespace tcpdae {

// ==================== PUBLIC ====================

void Connection::start() {
    syslog(LOG_DEBUG, "Connection-start");
    processRequest();
}

// ==================== PRIVATE ====================

void Connection::processRequest() {
    auto self = shared_from_this();
    boost::asio::async_read_until(
        mSocket, mRequest, "\n", [this, self](const boost::system::error_code ec, const size_t) {
            syslog(LOG_DEBUG, "async read entry");
            if (!ec || ec == boost::asio::error::eof) {
                std::string str((std::istreambuf_iterator<char>(&mRequest)), std::istreambuf_iterator<char>());
                syslog(LOG_DEBUG, "%s", str.c_str());
            }
        });
}

}  // namespace tcpdae
