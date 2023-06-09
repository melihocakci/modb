


#include <cstring>
#include <iostream>

#include <spatialindex/SpatialIndex.h>
#include <gnuplot-iostream.h>

#include <modb/DatabaseManager.h>

// websocket 
#include <boost/beast/core.hpp> 
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;


// write data to pipe
class MyQueryStrategy : public SpatialIndex::IQueryStrategy
{
private:
    gnuplotio::Gnuplot gp;
    std::queue<SpatialIndex::id_type> ids;
    int counter;

public:
    MyQueryStrategy() : gp{}, counter{ 1 } {
        gp << "set xrange [-200:200]\n"
            << "set yrange [-100:100]\n"
            << "set terminal png size 3840, 2160\n"
            << "set output 'output.png'\n";
    }

    ~MyQueryStrategy()
    {
        gp << "plot 0 notitle\n";
        std::cerr << counter << " objects in plot\n\n";
    }

    void getNextEntry(const SpatialIndex::IEntry& entry, SpatialIndex::id_type& nextEntry, bool& hasNext) override
    {
        SpatialIndex::IShape* ps;
        entry.getShape(&ps);
        SpatialIndex::Region* pr = dynamic_cast<SpatialIndex::Region*>(ps);

        const SpatialIndex::INode* n = dynamic_cast<const SpatialIndex::INode*>(&entry);

        if (n != nullptr) {
            if (n->getLevel() >= 1)
            {
                std::stringstream ss;

                ss << "set object " << counter++
                    << " rectangle from "
                    << pr->m_pLow[0] << "," << pr->m_pLow[1]
                    << " to " << pr->m_pHigh[0] << "," << pr->m_pHigh[1]
                    << " fillstyle empty border lc rgb 'red' lw 2\n";

                gp << ss.str();

                for (uint32_t cChild = 0; cChild < n->getChildrenCount(); cChild++)
                {
                    ids.push(n->getChildIdentifier(cChild));
                }
            }
            else if (n->getLevel() == 0) {
                std::stringstream ss;

                ss << "set object " << counter++
                    << " rectangle from "
                    << pr->m_pLow[0] << "," << pr->m_pLow[1]
                    << " to " << pr->m_pHigh[0] << "," << pr->m_pHigh[1]
                    << " fillstyle empty border lc rgb 'black' lw 2\n";

                gp << ss.str();

                for (uint32_t cChild = 0; cChild < n->getChildrenCount(); ++cChild)
                {
                    SpatialIndex::IShape* childShape;
                    n->getChildShape(cChild, &childShape);

                    SpatialIndex::Region childRegion;
                    childShape->getMBR(childRegion);

                    std::stringstream ss;

                    ss << "set object " << counter++
                        << " rectangle from "
                        << childRegion.getLow(0) << "," << childRegion.getLow(1)
                        << " to " << childRegion.getHigh(0) << "," << childRegion.getHigh(1)
                        << " fillstyle empty border lc rgb 'blue' lw 1\n";

                    gp << ss.str();
                }
            }
        }

        if (!ids.empty())
        {
            nextEntry = ids.front();
            ids.pop();

            hasNext = true;
        }
        else
        {
            hasNext = false;
        }

        delete ps;
    }
};

// read data from pipe and send buffer using 
// TODO : i need to get rid of irp communication and use salt thread with shared variable
class SendDataWS {
private:
    const std::string& m_address;
    const unsigned short& m_port;
    const std::string& m_pipeLocation;
    std::thread producerConsumerThread;

    bool m_running ;

    int sendData() {

        m_running = true;
        auto const address = boost::asio::ip::make_address(m_address);
        auto const port = m_port;

        boost::asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};

        while(m_running) {
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::cout << "socket accepted." << std::endl;

        std::thread producerConsumerThread{[q {std::move(socket)}, pipeLocation{std::move(m_pipeLocation)} ]() {
            std::ifstream file{pipeLocation};
            boost::beast::websocket::stream<tcp::socket> ws {std::move(const_cast<tcp::socket&>(q))};
            
            ws.accept();

            while(1) 
            {

                try {
                    boost::beast::flat_buffer buffer;

                    ws.read(buffer);

                    auto out = boost::beast::buffers_to_string(buffer.cdata());

                    std::cout << out << std::endl; // message from client

                    // take data from pipe
                    std::string line;
                    // std::cout << "Hey main Program Working" << std::endl;
                    std::getline(file, line);

                    // Convert string to buffer
                    boost::beast::string_view sv{line};
                    boost::asio::const_buffer buffer = boost::asio::buffer(sv.data(), sv.size());


                    // write data to pipe
                    ws.write(buffer.data());

                } catch (boost::beast::system_error const& e) 
                {
                    if(e.code() != boost::beast::websocket::error::closed)
                    std::cerr << e.what() << '\n';
                }
            }

        }};//.detach();
        }

    }

public:
    SendDataWS(const std::string& address, const unsigned short port, const std::string& pipeLocation) : 
    m_address{address},
    m_port{port},
    m_pipeLocation(pipeLocation) {}
    
    void startDataSendProcess () 
    {
        sendData();
    }

    void stopDataSendProcess() 
    {
        if (producerConsumerThread.joinable()) {
            m_running = false;
            producerConsumerThread.join();
        }
    }
};


int main(int argc, char** argv)
{
    std::string pipeLocation = "/tmp/wsPipe";

    int result = mkfifo(pipeLocation.c_str(), 0666);

    SendDataWS sendDataWs{"127.0.0.1", 8083, pipeLocation};    

    if (argc != 2) {
        std::cerr << "usage:\nanalyze_db <db-name>" << std::endl;
        return -1;
    }

    try
    {
        modb::DatabaseManager db{argv[1], DB_BTREE, DB_READ_COMMITTED};
        sendDataWs.startDataSendProcess();

        MyQueryStrategy qs;
        db.queryStrategy(qs);

    }
    catch (Tools::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    // finally
    sendDataWs.stopDataSendProcess();

    return 0;
}
