#ifndef INDEXVISITOR_H
#define INDEXVISITOR_H

#include <spatialindex/SpatialIndex.h>
#include <modb/Collections.h>

namespace modb
{

    class IndexVisitor : public SpatialIndex::IVisitor
    {
    public:
        modb::Set<SpatialIndex::id_type> m_indexes{ }; // this indexes can be sorted for binary search purpose
        size_t m_indexIO{ 0 };
        size_t m_leafIO{ 0 };

    public:
        IndexVisitor() = default;

        void visitNode(const SpatialIndex::INode& n) override;
        void visitData(const SpatialIndex::IData& d) override;

        void visitData(std::vector<const SpatialIndex::IData*>& v) override;

        std::unique_ptr<std::string> decodeId2Oid(SpatialIndex::id_type);

    private:
        const int m_charBase = 256;

    };

}

#endif
