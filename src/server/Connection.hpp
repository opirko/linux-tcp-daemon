#ifndef TCPDAE_CONNECTION_HPP
#define TCPDAE_CONNECTION_HPP

#include <syslog.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

namespace tcpdae {

/// @brief Connection handler
/// Can get a shared ptr to itself to control lifetime
class Connection : public std::enable_shared_from_this<Connection> {
   public:
    using asiotcp = boost::asio::ip::tcp;

    /// @brief constructor
    /// @param context asio context
    Connection(boost::asio::io_context& context) : mSocket(context) {}

    // no default ctor
    Connection() = delete;

    ///@brief destroy the Connection object
    ~Connection() = default;

    // deleted copy, move constructors and assignment operators
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection(Connection&&) = delete;
    Connection& operator=(Connection&&) = delete;

    /// @brief getter for socket
    /// @return socket ref
    asiotcp::socket& getSocket() { return mSocket; }

    /// @brief Handle connection
    void start();

   private:
    // Methods
    /// @brief Reads input from client
    void processRequest();

    /// @brief Executes command given as parameter
    /// @param cmd command to execute
    /// @return result of the command
    std::string doCommand(const std::string& cmd) const;

    /// @brief Returns cpu usage
    /// @return String containing cpu usage
    /// see https://stackoverflow.com/questions/3017162/how-to-get-total-cpu-usage-in-linux-using-c
    std::string getCpu() const;

    /// @brief Returns memory usage
    /// @return String containing memory usage
    std::string getMem() const;

    // Variables
    static constexpr char kClassName[]{"Connection"};
    asiotcp::socket mSocket;
    std::string mMessage;
    boost::asio::streambuf mRequest;
};

}  // namespace tcpdae

#endif  // TCPDAE_CONNECTION_HPP
