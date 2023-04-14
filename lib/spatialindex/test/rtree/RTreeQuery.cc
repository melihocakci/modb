/******************************************************************************
 * Project:  libspatialindex - A C++ library for spatial indexing
 * Author:   Marios Hadjieleftheriou, mhadji@gmail.com
 ******************************************************************************
 * Copyright (c) 2002, Marios Hadjieleftheriou
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
******************************************************************************/

// NOTE: Please read README.txt before browsing this code.

#include <cstring>

// include library header file.
#include <spatialindex/SpatialIndex.h>
#include "gnuplot-iostream.h"

using namespace SpatialIndex;
using namespace std;

#define INSERT 1
#define DELETE 0
#define QUERY 2

// example of a Visitor pattern.
// findes the index and leaf IO for answering the query and prints
// the resulting data IDs to stdout.
class MyVisitor: public IVisitor
{
public:
	size_t m_indexIO{ 0 };
	size_t m_leafIO{ 0 };

public:
	MyVisitor() = default;

	void visitNode(const INode& n) override
	{
		if (n.isLeaf()) m_leafIO++;
		else m_indexIO++;
	}

	void visitData(const IData& d) override
	{
		IShape* pS;
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

		cout << d.getIdentifier() << endl;
		// the ID of this data entry is an answer to the query. I will just print it to stdout.
	}

	void visitData(std::vector<const IData*>& v) override
	{
		cout << v[0]->getIdentifier() << " " << v[1]->getIdentifier() << endl;
	}
};

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

		if (n != nullptr) {
			if (n->getLevel() >= 1)
			{
				for (uint32_t cChild = 0; cChild < n->getChildrenCount(); cChild++)
				{
					ids.push(n->getChildIdentifier(cChild));
				}
			}
			else if (n->getLevel() == 0) {
				for (uint32_t cChild = 0; cChild < n->getChildrenCount(); ++cChild)
				{
					SpatialIndex::IShape* childShape;
					n->getChildShape(cChild, &childShape);

					SpatialIndex::Region childRegion;
					childShape->getMBR(childRegion);

					gp << "set object " << n->getIdentifier() << cChild
						<< " rectangle from "
						<< childRegion.getLow(0) << "," << childRegion.getLow(1)
						<< " to " << childRegion.getHigh(0) << "," << childRegion.getHigh(1)
						<< " fillstyle empty border lc rgb 'blue' lw 1\n";
					
					cerr << "set object " << n->getIdentifier() << cChild
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

				cerr << "set object " << entry.getIdentifier()
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


int main(int argc, char** argv)
{
	try
	{
		if (argc != 4)
		{
			cerr << "Usage: " << argv[0] << " query_file tree_file query_type [intersection | 10NN | selfjoin]." << endl;
			return -1;
		}

		uint32_t queryType = 0;

		if (strcmp(argv[3], "intersection") == 0) queryType = 0;
		else if (strcmp(argv[3], "10NN") == 0) queryType = 1;
		else if (strcmp(argv[3], "selfjoin") == 0) queryType = 2;
		else
		{
			cerr << "Unknown query type." << endl;
			return -1;
		}

		ifstream fin(argv[1]);
		if (!fin)
		{
			cerr << "Cannot open query file " << argv[1] << "." << endl;
			return -1;
		}

		string baseName = argv[2];
		IStorageManager* diskfile = StorageManager::loadDiskStorageManager(baseName);
		// this will try to locate and open an already existing storage manager.

		StorageManager::IBuffer* file = StorageManager::createNewRandomEvictionsBuffer(*diskfile, 10, false);
		// applies a main memory random buffer on top of the persistent storage manager
		// (LRU buffer, etc can be created the same way).

		// If we need to open an existing tree stored in the storage manager, we only
		// have to specify the index identifier as follows
		ISpatialIndex* tree = RTree::loadRTree(*file, 1);

		size_t count = 0;
		size_t indexIO = 0;
		size_t leafIO = 0;
		id_type id;
		uint32_t op;
		double x1, x2, y1, y2;
		double plow[2], phigh[2];

		while (fin)
		{
			fin >> op >> id >> x1 >> y1 >> x2 >> y2;
			if (!fin.good()) continue; // skip newlines, etc.

			if (op == QUERY)
			{
				plow[0] = x1; plow[1] = y1;
				phigh[0] = x2; phigh[1] = y2;

				MyVisitor vis;

				if (queryType == 0)
				{
					Region r = Region(plow, phigh, 2);
					tree->intersectsWithQuery(r, vis);
					// this will find all data that intersect with the query range.
				}
				else if (queryType == 1)
				{
					Point p = Point(plow, 2);
					tree->nearestNeighborQuery(10, p, vis);
					// this will find the 10 nearest neighbors.
				}
				else
				{
					Region r = Region(plow, phigh, 2);
					tree->selfJoinQuery(r, vis);
				}

				indexIO += vis.m_indexIO;
				leafIO += vis.m_leafIO;
				// example of the Visitor pattern usage, for calculating how many nodes
				// were visited.
			}
			else
			{
				cerr << "This is not a query operation." << endl;
			}

			if ((count % 1000) == 0) cerr << count << endl;

			count++;
		}

		MyQueryStrategy qs;
		tree->queryStrategy(qs);

		// cerr << "Indexed space: " << qs.m_indexedSpace << endl;
		cerr << "Operations: " << count << endl;
		cerr << *tree;
		cerr << "Index I/O: " << indexIO << endl;
		cerr << "Leaf I/O: " << leafIO << endl;
		cerr << "Buffer hits: " << file->getHits() << endl;

		delete tree;
		delete file;
		delete diskfile;
		// delete the buffer first, then the storage manager
		// (otherwise the the buffer will fail writting the dirty entries).
	}
	catch (Tools::Exception& e)
	{
		cerr << "******ERROR******" << endl;
		std::string s = e.what();
		cerr << s << endl;
		return -1;
	}
	catch (...)
	{
		cerr << "******ERROR******" << endl;
		cerr << "other exception" << endl;
		return -1;
	}

	return 0;
}
