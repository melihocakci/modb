#include <memory>
#include <modb/MyVisitor.h>
#include <spatialindex/SpatialIndex.h>


void modb::MyVisitor::visitNode(const SpatialIndex::INode& n)
{
    if (n.isLeaf()) m_leafIO++;
    else m_indexIO++;
}
void modb::MyVisitor::visitData(const SpatialIndex::IData& d)
{
    SpatialIndex::IShape* pS;
    d.getShape(&pS);
    // do something.
    delete pS;

    // data should be an array of characters representing a Region as a string.
    uint8_t* pData = nullptr;
    uint32_t cLen = 0;
    d.getData(cLen, &pData);
    // do something.
    //string s = reinterpret_cast<char*>(pData);
    //cout << s << endl;
    delete[] pData;

    std::cout << d.getIdentifier() << std::endl;
    // the ID of this data entry is an answer to the query. I will just print it to stdout.


    // m_indexes.array.push_back(d.getIdentifier());
    // // std::cout <<  << std::endl;
    // // the ID of this data entry is an answer to the query. I will just print it to stdout.
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
    std::unique_ptr<std::string> result= std::make_unique<std::string>();
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
