#include <memory>
#include <modb/IndexVisitor.h>
#include <spatialindex/SpatialIndex.h>

modb::IndexVisitor::IndexVisitor():
    m_leafIO{0},
    m_indexIO{0},
    m_queryResult{} {}

void modb::IndexVisitor::visitNode(const SpatialIndex::INode& n)
{
    if (n.isLeaf()) {
        m_leafIO++; // for debug purpose
    }
    else { // for debug prupose
        m_indexIO++;
    }
}

void modb::IndexVisitor::visitData(const SpatialIndex::IData& d)
{
    m_queryResult.push_back(d.getIdentifier());    // the ID of this data entry is an answer to the query. I will just print it to stdout.
}

void modb::IndexVisitor::visitData(std::vector<const SpatialIndex::IData*>& v)
{
    std::cout << v[0]->getIdentifier() << " " << v[1]->getIdentifier() << std::endl;
}
