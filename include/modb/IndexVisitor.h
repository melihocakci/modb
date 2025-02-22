#pragma once

#include <spatialindex/SpatialIndex.h>

namespace modb
{
    class IndexVisitor : public SpatialIndex::IVisitor
    {
    public:
        size_t m_leafIO;
        size_t m_indexIO;
        std::vector<SpatialIndex::id_type> m_queryResult; // this indexes can be sorted for binary search purpose

        IndexVisitor();

        void visitNode(const SpatialIndex::INode& n) override;
        void visitData(const SpatialIndex::IData& d) override;

        void visitData(std::vector<const SpatialIndex::IData*>& v) override;
    };
}
