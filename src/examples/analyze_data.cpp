#include <cstring>
#include <iostream>

#include <spatialindex/SpatialIndex.h>
#include <gnuplot-iostream.h>

#include <modb/DatabaseResource.h>

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
                std::cerr << ss.str();

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
                std::cerr << ss.str();

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
                    std::cerr << ss.str();
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


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage:\nanalyze_db <db-name>" << std::endl;
        return -1;
    }

    try
    {
        modb::DatabaseResource db{argv[1], DB_BTREE, DB_READ_COMMITTED};

        MyQueryStrategy qs;
        db.queryStrategy(qs);
    }
    catch (Tools::Exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}