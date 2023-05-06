#include <memory>
#include <modb/MyVisitor.h>
#include <spatialindex/SpatialIndex.h>


void modb::MyVisitor::visitNode(const SpatialIndex::INode& n)
{
    if (n.isLeaf()) {

        m_leafIO++; // for debug purpose
    }
    else m_indexIO++; // for debug prupose
}
void modb::MyVisitor::visitData(const SpatialIndex::IData& d)
{
    std::cout << *decodeId2Oid(d.getIdentifier()) << std::endl;
    m_indexes.collection.insert(d.getIdentifier());    // the ID of this data entry is an answer to the query. I will just print it to stdout.
}

// base conversion encode decode
SpatialIndex::id_type modb::MyVisitor::encodeOid2Id(std::string&& oid ) const {
    SpatialIndex::id_type id = 0; 
    for ( char c : oid ) 
    {
        id = id * m_charBase + (c);
    }
    return id; 
}

SpatialIndex::id_type modb::MyVisitor::encodeOid2Id(std::string& oid) const {
    SpatialIndex::id_type id = 0; 
    for ( char c : oid ) 
    {
        id = id * m_charBase + (c);
    }
    return id;
}

std::unique_ptr<std::string> modb::MyVisitor::decodeId2Oid(SpatialIndex::id_type n){
    std::unique_ptr<std::string> result = std::make_unique<std::string>();
    while (n > 0) {
        *result += static_cast<char>(n % m_charBase);
        n /= m_charBase;
    }
    return result;
}

void modb::MyVisitor::visitData(std::vector<const SpatialIndex::IData*>& v) 
{
    std::cout << v[0]->getIdentifier() << " " << v[1]->getIdentifier() << std::endl;
}
