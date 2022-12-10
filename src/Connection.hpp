#ifndef TCPDAE_CONNECTION_HPP
#define TCPDAE_CONNECTION_HPP

#include <syslog.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

namespace tcpdae {

class Connection : public boost::enable_shared_from_this<Connection> {
   public:
    using asiotcp = boost::asio::ip::tcp;
    using ptr = boost::shared_ptr<Connection>;

    static ptr create(boost::asio::io_context& io_context) { return ptr(new Connection(io_context)); }

    asiotcp::socket& socket() { return mSocket; }

    void start() {
        syslog(LOG_DEBUG, "Connection-start");
        mMessage = "TESTINGMESSAGE";

        boost::asio::async_write(
            mSocket, boost::asio::buffer(mMessage),
            boost::bind(&Connection::handle_write, shared_from_this(), boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

   private:
    Connection(boost::asio::io_context& io_context) : mSocket(io_context) {}

    void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/) {}

    asiotcp::socket mSocket;
    std::string mMessage;
};

}  // namespace tcpdae

#endif  // TCPDAE_CONNECTION_HPP
