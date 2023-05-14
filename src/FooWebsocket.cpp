// websocket 
#include <boost/beast/core.hpp> 
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

int main() {

    auto const address = boost::asio::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(std::atoi("8083"));


    boost::asio::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};

    while(1) {
      tcp::socket socket{ioc};
      acceptor.accept(socket);
      std::cout << "socket accepted." << std::endl;

      std::thread{[q {std::move(socket)} ]() {
        boost::beast::websocket::stream<tcp::socket> ws {std::move(const_cast<tcp::socket&>(q))};
        
        ws.accept();

        while(1) 
        {

            try {
                boost::beast::flat_buffer buffer;

                ws.read(buffer);

                auto out = boost::beast::buffers_to_string(buffer.cdata());

                std::cout << out << std::endl;

                ws.write(buffer.data());

            } catch (boost::beast::system_error const& e) 
            {
                if(e.code() != boost::beast::websocket::error::closed)
                std::cerr << e.what() << '\n';
            }
        }

      }}.detach();
    }

}