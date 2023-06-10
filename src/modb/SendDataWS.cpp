// websocket 
#include <modb/SendDataWS.h>
#include <csignal>

using modb::SendDataWS;
using boost::asio::ip::tcp;


SendDataWS::SendDataWS() :
    m_address{ "127.0.0.1" },
    m_pipeLocation{ "/tmp/wsPipe" } ,
    m_port{ 8080 }{}

// SendDataWS::SendDataWS(const SendDataWS& other) :
//     m_address{ other.m_address },
//     m_pipeLocation{ other.m_pipeLocation } ,
//     m_port{ other.m_port }{}

// SendDataWS::SendDataWS(SendDataWS&& other) :
//     m_address{ std::move(other.m_address) },
//     m_pipeLocation{ std::move(other.m_pipeLocation) } ,
//     m_port{ std::move(other.m_port) } {}



SendDataWS::SendDataWS(const std::string& address,  const std::string& pipeLocation, const int& port) :
    m_address{ address },
    m_pipeLocation{pipeLocation},
    m_port{ port }
    {
    isPipeWriterFree.store(false);
    isJoined.store(false);

    int result = mkfifo(pipeLocation.c_str(), 0666);

    if (result == -1) {
        std::cout << "fifo is not created. Already exist " << std::endl;
    }
    
    
    m_openedPipeThread = std::thread([&]() { // i think m_pipeLocation properly initialized data pipeLocation gives error
        m_pipeWriter.open(pipeLocation);
        // int pipefd = open(pipeLocation.c_str(), O_WRONLY);
        // if (pipefd == -1) {
        //     std::cerr << "Failed to open the named pipe for writing." << std::endl;
        // }

    });

    std::cout << "burası" << std::endl;

}



// void signalHandler(int signum) {
//     if (signum == SIGPIPE) {
//         std::cout << "Caught SIGPIPE signal." << std::endl;
//         // Handle the broken pipe error here
//         // ...
//     }
// }

SendDataWS::~SendDataWS() 
{
    m_openedPipeThread.join();
    // m_pipeWriter.clear();
    try
    {
        /* code */
        // struct sigaction sigAction;
        // sigAction.sa_handler = signalHandler;
        // sigemptyset(&sigAction.sa_mask);
        // sigAction.sa_flags = 0;

        // // Set the action for SIGPIPE to SIG_IGN (ignore)
        // sigaction(SIGPIPE, &sigAction, nullptr);

        m_pipeWriter.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void SendDataWS::startDataSendProcess()
{
    auto const address = boost::asio::ip::make_address(m_address);
    auto const port = m_port.load();

    boost::asio::io_context ioc{1};

    tcp::acceptor acceptor{ioc, { address, static_cast<short unsigned int>(port) }};

    
    tcp::socket socket{ioc};
    acceptor.accept(socket);
    std::cout << "socket accepted." << std::endl;

    std::ifstream file{m_pipeLocation};
    boost::beast::websocket::stream<tcp::socket> ws {std::move(const_cast<tcp::socket&>(socket))};

    // block all thread
    ws.accept();
    

    while (!isJoined)
    {

        try {
            // boost::beast::flat_buffer buffer_read;

            // ws.read(buffer_read);

            // auto out = boost::beast::buffers_to_string(buffer_read.cdata());

            // std::cout << out << std::endl; // message from client

            // take data from pipe
            std::string line;

            // conditional variable
            {
            // std::lock_guard<std::mutex> lock(pipeWriterLock);
                isPipeWriterFree.store(true);
            }
            cv.notify_all();

            if (std::getline(file, line)) {
                // Process the buffer data as needed
                std::cout << "Received data: " << line << std::endl;
                // write data to pipe
                ws.write(boost::asio::buffer(line));
            }


        }
        catch (boost::beast::system_error const& e)
        {
            if (e.code() != boost::beast::websocket::error::closed)
                std::cerr << e.what() << '\n';
        }
    }
    file.close();
//.detach();

}


std::ofstream& SendDataWS::pipeWriter() 
{

    // this->m_pipeWriter.open(m_pipeLocation);
    return this->m_pipeWriter;
}



