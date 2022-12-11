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
                std::string inp((std::istreambuf_iterator<char>(&mRequest)), std::istreambuf_iterator<char>());
                // remove "\n" from string
                if (inp.back() == '\n') {
                    inp = inp.substr(0, inp.size() - 1);
                }
                std::string ret = doCommand(inp);
                syslog(LOG_DEBUG, "Sending message %s to request %s", ret.c_str(), inp.c_str());
                // send data
            }
        });
}

std::string Connection::doCommand(const std::string& cmd) const {
    syslog(LOG_INFO, "Executing command : %s", cmd.c_str());
    if (cmd == "cpu") {
        return getCpu();
    } else if (cmd == "mem") {
        return getMem();
    } else {
        return "Invalid command";
    }
}

std::string Connection::getCpu() const { return std::to_string(50.5f); }

std::string Connection::getMem() const { return std::to_string(4000); }

}  // namespace tcpdae
