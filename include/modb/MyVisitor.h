#ifndef MYVISITOR_H
#define MYVISITOR_H

#include <spatialindex/SpatialIndex.h>
#include <modb/List.h>

namespace modb
{

	class MyVisitor: public SpatialIndex::IVisitor
	{
	public:
		modb::List<std::string> m_indexes { };
		size_t m_indexIO{ 0 };
		size_t m_leafIO{ 0 };

	public:
		MyVisitor() = default;

		void visitNode(const SpatialIndex::INode& n) override;
		void visitData(const SpatialIndex::IData& d) override;

		void visitData(std::vector<const SpatialIndex::IData*>& v) override;
		
		SpatialIndex::id_type encodeOid2Id(std::string & oid) const;
		// SpatialIndex::id_type convertOid2Id(std::string oid) const;

		SpatialIndex::id_type encodeOid2Id(std::string&& oid) const ;
		std::unique_ptr<std::string> decodeId2Oid(SpatialIndex::id_type);

	private:
		const int m_charBase = 256;

	};

}

#endif