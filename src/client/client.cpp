#include <boost/asio.hpp>
#include <cstdio>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // port and host of server
    static constexpr int kPort{5001};
    static constexpr char kHost[]{"127.0.0.1"};
    // using to shorten code
    using asiotcp = boost::asio::ip::tcp;
    boost::asio::io_context context;
    asiotcp::socket socket(context);
    // try connecting, if it doesn't work exit
    try {
        socket.connect(asiotcp::endpoint(boost::asio::ip::address::from_string(kHost), kPort));
    } catch (const std::exception& e) {
        std::cout << "Server is not running!" << std::endl;
        return 1;
    }
    // read input from user
    std::string req;
    std::cout << "Type your command: ";
    std::cin >> req;
    std::cout << "Your command is: " << req << std::endl;
    boost::system::error_code ec;
    // add newline to signify end of input
    req += "\n";
    boost::asio::write(socket, boost::asio::buffer(req), ec);
    if (!ec) {
        std::cout << "Message sent" << std::endl;
    } else {
        std::cout << "Send failed: " << ec.message() << std::endl;
    }
    // getting response from server
    boost::asio::streambuf recBuf;
    boost::asio::read_until(socket, recBuf, "\n", ec);
    if (ec && ec != boost::asio::error::eof) {
        std::cout << "Receive failed: " << ec.message() << std::endl;
    } else {
        const char* data = boost::asio::buffer_cast<const char*>(recBuf.data());
        std::cout << "Received: " << data << std::endl;
    }
    return 0;
}
