#include <spatialindex/SpatialIndex.h>
#include <gnuplot-iostream.h>

#include <iostream>
#include <cstring>

class MyQueryStrategy: public SpatialIndex::IQueryStrategy
{
private:
    gnuplotio::Gnuplot gp;
    std::queue<SpatialIndex::id_type> ids;

public:
    MyQueryStrategy(): gp() {
        gp << "set xrange [-0.05:1.15]\nset yrange [-0.05:1.15]\n";
    }

    ~MyQueryStrategy()
    {
        gp << "plot -1 notitle\n";
    }

    void getNextEntry(const SpatialIndex::IEntry& entry, SpatialIndex::id_type& nextEntry, bool& hasNext) override
    {
        SpatialIndex::IShape* ps;
        entry.getShape(&ps);
        SpatialIndex::Region* pr = dynamic_cast<SpatialIndex::Region*>(ps);

        const SpatialIndex::INode* n = dynamic_cast<const SpatialIndex::INode*>(&entry);

        if (n == nullptr)
        {
        }
        else if (n->getLevel() >= 1)
        {
            for (uint32_t cChild = 0; cChild < n->getChildrenCount(); cChild++)
            {
                ids.push(n->getChildIdentifier(cChild));
            }
        }

        if (n->getLevel() == 0) {
            // Draw the rectangle
            std::cerr << "set object " << entry.getIdentifier()
                << " rectangle from "
                << pr->m_pLow[0] << "," << pr->m_pLow[1]
                << " to " << pr->m_pHigh[0] << "," << pr->m_pHigh[1]
                << " fillstyle empty border lc rgb 'black' lw 1\n";

            gp << "set object " << entry.getIdentifier()
                << " rectangle from "
                << pr->m_pLow[0] << "," << pr->m_pLow[1]
                << " to " << pr->m_pHigh[0] << "," << pr->m_pHigh[1]
                << " fillstyle empty border lc rgb 'black' lw 1\n";
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
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: " << argv[0] << " tree_file" << std::endl;
            return -1;
        }

        std::string baseName = argv[1];
        SpatialIndex::IStorageManager* diskfile = SpatialIndex::StorageManager::loadDiskStorageManager(baseName);

        SpatialIndex::StorageManager::IBuffer* file = SpatialIndex::StorageManager::createNewRandomEvictionsBuffer(*diskfile, 10, false);

        SpatialIndex::ISpatialIndex* tree = SpatialIndex::RTree::loadRTree(*file, 1);

        MyQueryStrategy qs;
        tree->queryStrategy(qs);

        delete tree;
        delete file;
        delete diskfile;
    }
    catch (Tools::Exception& e)
    {
        std::cerr << "******ERROR******" << std::endl;
        std::string s = e.what();
        std::cerr << s << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "******ERROR******" << std::endl;
        std::cerr << "other exception" << std::endl;
        return -1;
    }

    return 0;
}
