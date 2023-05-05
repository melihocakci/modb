
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

SpatialIndex::id_type modb::MyVisitor::convertOid2Id(std::string & oid){
    
}

std::string& modb::MyVisitor::convertId2Oid(SpatialIndex::id_type){

}

void modb::MyVisitor::visitData(std::vector<const SpatialIndex::IData*>& v) 
{
    std::cout << v[0]->getIdentifier() << " " << v[1]->getIdentifier() << std::endl;
}
