#pragma once

#include <spatialindex/SpatialIndex.h>
#include "gnuplot-iostream.h"

template <>
class MyQueryStrategy : public SpatialIndex::IQueryStrategy
{
private:
	gnuplotio::Gnuplot gp;
	std::queue<SpatialIndex::id_type> ids;

public:
	MyQueryStrategy() : gp()
	{
		gp << "set xrange [-0.05:1.15]\nset yrange [-0.05:1.15]\n";
	}

	~MyQueryStrategy()
	{
		gp << "plot -1 notitle\n";
	}

	void getNextEntry(const SpatialIndex::IEntry &entry, SpatialIndex::id_type &nextEntry, bool &hasNext) override
	{
		SpatialIndex::IShape *ps;
		entry.getShape(&ps);
		SpatialIndex::Region *pr = dynamic_cast<SpatialIndex::Region *>(ps);

		const SpatialIndex::INode *n = dynamic_cast<const SpatialIndex::INode *>(&entry);

		if (n != nullptr)
		{
			if (n->getLevel() >= 1)
			{
				for (uint32_t cChild = 0; cChild < n->getChildrenCount(); cChild++)
				{
					ids.push(n->getChildIdentifier(cChild));
				}
			}
			else if (n->getLevel() == 0)
			{
				for (uint32_t cChild = 0; cChild < n->getChildrenCount(); ++cChild)
				{
					SpatialIndex::IShape *childShape;
					n->getChildShape(cChild, &childShape);

					SpatialIndex::Region childRegion;
					childShape->getMBR(childRegion);

					gp << "set object " << n->getIdentifier() << cChild
					   << " rectangle from "
					   << childRegion.getLow(0) << "," << childRegion.getLow(1)
					   << " to " << childRegion.getHigh(0) << "," << childRegion.getHigh(1)
					   << " fillstyle empty border lc rgb 'blue' lw 1\n";

					std::cerr << "set object " << n->getIdentifier() << cChild
							  << " rectangle from "
							  << childRegion.getLow(0) << "," << childRegion.getLow(1)
							  << " to " << childRegion.getHigh(0) << "," << childRegion.getHigh(1)
							  << " fillstyle empty border lc rgb 'blue' lw 1\n";
				}

				gp << "set object " << entry.getIdentifier()
				   << " rectangle from "
				   << pr->m_pLow[0] << "," << pr->m_pLow[1]
				   << " to " << pr->m_pHigh[0] << "," << pr->m_pHigh[1]
				   << " fillstyle empty border lc rgb 'black' lw 2\n";

				std::cerr << "set object " << entry.getIdentifier()
						  << " rectangle from "
						  << pr->m_pLow[0] << "," << pr->m_pLow[1]
						  << " to " << pr->m_pHigh[0] << "," << pr->m_pHigh[1]
						  << " fillstyle empty border lc rgb 'black' lw 2\n";
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