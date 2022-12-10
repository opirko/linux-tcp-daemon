#include "Connection.hpp"

namespace tcpdae {

constexpr char Connection::kClassName[];

// ==================== PUBLIC ====================

void Connection::start() {
    syslog(LOG_DEBUG, "%s::%s", kClassName, __func__);
    processRequest();
}

// ==================== PRIVATE ====================

void Connection::processRequest() {
    auto self = shared_from_this();
    boost::asio::async_read_until(
        mSocket, mRequest, "\n", [this, self](const boost::system::error_code ec, const size_t) {
            if (!ec || ec == boost::asio::error::eof) {
                // convert request stream to std string
                std::string str((std::istreambuf_iterator<char>(&mRequest)), std::istreambuf_iterator<char>());
                // do command without the "\n0"
                std::string ret = doCommand(str.substr(0, str.size() - 3));
                // send data
            }
        });
}

std::string Connection::doCommand(const std::string& cmd) {
    syslog(LOG_INFO, "Executing command : %s", cmd.c_str());
    return "";
}

}  // namespace tcpdae
