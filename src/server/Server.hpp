#ifndef TCPDAE_SERVER_HPP
#define TCPDAE_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>

#include "Connection.hpp"

namespace tcpdae {

class Server final {
   public:
    ///@brief construct a new Server object
    Server();

    ///@brief destroy the Server object
    ~Server() = default;

    // deleted copy, move constructors and assignment operators
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    /// @brief run application
    void run();

   private:
    // methods
    /// @brief Starts the connection accept process
    void startAccept();

    /// @brief Handles accepted connection, also starts a new accept
    void handleAccept(std::shared_ptr<Connection> con, const boost::system::error_code& error);

    // variables
    static constexpr char kClassName[]{"Server"};
    static constexpr int kPort{5001};
    boost::asio::io_context mContext;
    boost::asio::ip::tcp::acceptor mAcceptor;
};

}  // namespace tcpdae

#endif  // TCPDAE_SERVER_HPP
