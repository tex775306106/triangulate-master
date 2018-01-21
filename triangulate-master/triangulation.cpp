#include "triangulation.h"
#include "intersection.h"
//
//given a polygon, compute a list of triangles that partition the polygon
//

//triangulate -s 10 polygons/simple1.poly
void Triangulator::triangulate(c_polygon& poly, vector<triangle>& triangles )
{
	this->m_ploy.copy(poly);
	this->m_polys.push_back(this->m_ploy);
	findMonotonicPolygons(poly);

  //Loop through monotonic polygons and create triangles
	for(auto iter=this->m_polys.begin();iter!=this->m_polys.end();++iter)
	{
		if(iter->getIsSplit()) continue;
		findTriangles(*iter, triangles);
	}
	


}


struct cmp {
	bool operator()(ply_vertex* a, ply_vertex* b) {
		double x0 = a->getPos()[0];
		double x1 = b->getPos()[0];
		double y0 = a->getPos()[1];
		double y1 = b->getPos()[1];
		if (y0 == y1) {
			if (x0 < x1)
				return false;
			else
				return true;
		}
		else {
			if (y0 > y1)
				return false;
			else
				return true;
		}
	}
};
//
// given a polygon, split it into monotonic polygons
//
bool ifleft(Point2d a, Point2d b, Point2d e) {
	Point2d abVector = { b[0]-a[0], b[1]-a[1] };
	Point2d beVector = { e[0] - b[0],e[1] - b[1] };
	
	double abxbe = (abVector[0] * beVector[1] - abVector[1] * beVector[0]);
	if (abxbe>0)
		return true;
	return false;
}
void Triangulator::findMonotonicPolygons(c_polygon& poly)
{
	//Your implementation here

	//create a the priority queue based on y coordinate
	priority_queue<ply_vertex*,vector<ply_vertex*>,cmp> pq;
	int psize;
	for (auto iter = poly.begin();iter != poly.end();++iter)
	{

		psize= iter->getSize();
		ply_vertex* currentV = iter->getHead();
		ply_vertex* headV = currentV;
		ply_vertex* preV, nextV;
		do {
			pq.push(currentV);
			currentV = currentV->getNext();
		} while (currentV != headV);
		
	}
	priority_queue<ply_vertex*, vector<ply_vertex*>, cmp> pqCopy = pq;;


	//determine the type of each vertex
	ply_edge** edges = new ply_edge*[psize];
	int* T = new int[pq.size()];
	int ti = 0;
	while (!pqCopy.empty()) {
		T[ti] = 0;
		ply_vertex* tempV = pqCopy.top();
		edges[tempV->getVID()] = new ply_edge(tempV, (tempV->getNext()));
		edges[tempV->getVID()]->setHelper(tempV);
		edges[tempV->getVID()]->setKeyValue(tempV->getNext()->getPos()[1]);

		ply_vertex* v = pqCopy.top();
		pqCopy.pop();
		double preX = v->getPre()->getPos()[0];
		double preY = v->getPre()->getPos()[1];
		double nextX = v->getNext()->getPos()[0];
		double nextY = v->getNext()->getPos()[1];
		double currentX = v->getPos()[0];
		double currentY = v->getPos()[1];
		double k1; 
		double k2; 
		if (currentX != preX)
			k1 = (currentY - preY) / (currentX - preX);
		else
			k1 = 0.123456;
		if (currentX != nextX)
			k2 = (nextY - currentY) / (nextX - currentX);
		else
			k2 = 0.123456;

		if (k1 != 0.123456&&k2!= 0.123456) {
			
			if (currentY >= preY&&currentY > nextY) {
				if ((k1 <= 0 && k2>=0) || (k1 >= k2&&k2 >= 0)) {
					//start point

					v->START;
					if (v->getPre()->getType() == 0)
						v->getPre()->REGULAR_UP;
				}
				else if (k1 > 0 && k2 < 0) {
					//split point

					v->SPLIT;
				}
				else {
					//cout << "Unkonwn 1" << endl;
				}
			}
			else if (currentY <= preY&&currentY <= nextY) {
				if (k1 <= 0) {
					//end point

					v->END;
					if (v->getPre()->getType() == 1) {
						v->getPre()->REGULAR_DOWN;
					}
				}
				else if ((k1 > 0 && k2 < 0) || (k2 > k1&&k1 > 0)) {
					//merge point

					v->MERGE;
				}
				else {
					//cout << "Unkonwn 2" << endl;
				}
			}
			else if (currentY >= preY&&currentY < nextY) {
				//regular up

				v->REGULAR_UP;
			}
			else if (currentY<=preY&&currentY>nextY) {
				//regular down

				v->REGULAR_DOWN;
			}
			else {
				//cout << "Unkonwn 3" << endl;
			}

		}
		else if (k1==0.123456&&k2!=0.123456||k1!=0.123456&&k2==0.123456) {
			if (currentY >= preY&&currentY > nextY) {
				if (k1<=0&&k2==0.123456||k1==0.123456&&k2>0) {
					//start point

					v->START;
					if (v->getPre()->getType() == 0)
						v->getPre()->REGULAR_UP;
				}
				else if (k1 ==0.123456&& k2 < 0||k1>0&&k2==0.123456) {
					//split point

					v->SPLIT;
				}
				else {
					//cout << "Unkonwn 1" << endl;
				}
			}
			else if (currentY <= preY&&currentY <= nextY) {
				if (k1 <= 0 && k2==0.123456||k1==0.123456&&k2>=0) {
					//end point

					v->END;
					if (v->getPre()->getType() == 1)
						v->getPre()->REGULAR_DOWN;
				}
				else if (k1==0.123456&&k2<0||k1>0&&k2==0.123456) {
					//merge point


					v->MERGE;
				}
				else {
					//cout << "Unkonwn 2" << endl;
				}
			}
			else if (currentY >= preY&&currentY <= nextY) {
				//regular up

				v->REGULAR_UP;
			}
			else if (currentY<preY&&currentY>nextY) {
				//regular down

				v->REGULAR_DOWN;
			}
			else {
				//cout << "Unkonwn 32" << endl;
			}

		}
		else if (k1 == 0.123456&&k2 == 0.123456) {
			if (currentY >= preY&&currentY < nextY) {
				//regular up

				v->REGULAR_UP;
			}
			else if (currentY<preY&&currentY>nextY) {
				//regular down

				v->REGULAR_DOWN;
			}
			else {
				//cout << "Unkonwn 33" << endl;
			}
		}
	}
	

	
	SplayTree<ply_edge*, double> s;


	while (!pq.empty()) {

		ply_vertex* tempV = pq.top();

		pq.pop();
		int eventType = tempV->getType();
		
		double yKey = tempV->getPos()[1];
		double xKey= tempV->getPos()[0];

		if (eventType == 0) {
			//handle start vertex
			s.Insert(edges[tempV->getVID()]);
			T[tempV->getVID()] = 1;

		}
		else if (eventType == 1) {
			//handle end vertex

			int lid = tempV->getVID() - 1;
			if (lid < 0)
				lid = psize - 1;
			if (edges[lid]->getHelper()->getType() == 3) {
				this->addDiagonal(tempV, edges[lid]->getHelper());
			}
			s.Delete(edges[lid]->keyValue());
			T[lid] = 0;
		}
		else if (eventType == 2) {
			//handle split point
			//int rid = s.Root()->data()->getSource()->getVID();
			//edges[rid]->setKeyValue(yKey);
			for (int i = 0;i < psize;i++) {
				if (T[i] == 1) {
					edges[i]->setKeyValue(yKey);
				}
			}
			BTreeNode<ply_edge*, double>* tempNode;
			if (s.Size() > 1)
				s.FindMaxSmallerThan(xKey, tempNode);
			else
				s.FindMax(tempNode);
			this->addDiagonal(tempV, tempNode->data()->getHelper());
			s.Insert(edges[tempV->getVID()]);
			T[tempV->getVID()] = 1;

		}
		else if (eventType == 3) {
			//handle merge vertex
			int lid = tempV->getVID() - 1;
			if (lid < 0)
				lid = psize - 1;
			if (edges[lid]->getHelper()->getType() == 3) {
				this->addDiagonal(tempV, edges[lid]->getHelper());
			}
			s.Delete(edges[lid]->keyValue());
			T[lid] = 0;
			//int rid = s.Root()->data()->getSource()->getVID();
			//edges[rid]->setKeyValue(yKey);
			//s.Root()->data()->setKeyValue(yKey);
			for (int i = 0;i < psize;i++) {
				if (T[i] == 1)
					edges[i]->setKeyValue(yKey);
			}
			BTreeNode<ply_edge*, double>* tempNode;
			if (s.Size() > 1)
				s.FindMaxSmallerThan(xKey, tempNode);
			else
				s.FindMax(tempNode);
			int jid = tempNode->data()->getSource()->getVID();
			if (edges[jid]->getHelper()->getType() == 3) {
				this->addDiagonal(edges[jid]->getHelper(), tempV);
			}
			edges[jid]->setHelper(tempV);
		}
		else if (eventType==4) {
			//handle regular down vertex
			int lid = tempV->getVID() - 1;
			if (lid < 0)
				lid = psize - 1;
			if (edges[lid]->getHelper()->getType() == 3) {
				this->addDiagonal(tempV, edges[lid]->getHelper());
			}
			s.Delete(edges[lid]->keyValue());
			T[lid] = 0;
			s.Insert(edges[tempV->getVID()]);
			T[tempV->getVID()] = 1;
		}
		else if (eventType == 5) {
			//handle regular up vertex
			//int rid = s.Root()->data()->getSource()->getVID();
			//edges[rid]->setKeyValue(yKey);
			//s.Root()->data()->setKeyValue(yKey);
			for (int i = 0;i < psize;i++) {
				if (T[i] == 1) {
					edges[i]->setKeyValue(yKey);
				}
			}
			BTreeNode<ply_edge*, double>* tempNode;
			if (s.Size() > 1)
				s.FindMaxSmallerThan(xKey, tempNode);
			else
				s.FindMax(tempNode);
			int jid = tempNode->data()->getSource()->getVID();
			if (edges[jid]->getHelper()->getType() == 3) {
				this->addDiagonal(tempV, edges[jid]->getHelper());
			}
			edges[jid]->setHelper(tempV);
		}
		
	}


}

//
// given a monotonic polygon, compute a list of triangles that partition the polygon
//
void Triangulator::findTriangles(c_polygon& poly, vector<triangle>& triangles)
{
	//Your implementation here
	int psize;
	for (auto iter = this->m_ploy.begin();iter != this->m_ploy.end();++iter)
		psize = iter->getSize();


	int* side = new int[psize];   //-1-top,0-left,1-right,2-bottom

	for (auto iter = poly.begin();iter != poly.end();++iter)
	{
		for (int i = 0;i < psize;i++)
			side[i] = 0;
		priority_queue<ply_vertex*, vector<ply_vertex*>, cmp> pq;

		ply_vertex* tempV = iter->getHead();
		ply_vertex* headV = tempV;
		ply_vertex* topV = tempV;
		ply_vertex* bottomV = tempV;

		for (int i = 0;i < iter->getSize();i++) {
			pq.push(tempV);
			if ((tempV->getPos()[1] < bottomV->getPos()[1]) || (tempV->getPos()[1] == bottomV->getPos()[1]) && (tempV->getPos()[0]> bottomV->getPos()[0]))
				bottomV = tempV;
			if (tempV->getNext() != NULL)
				tempV = tempV->getNext();
		}
		topV = pq.top();
		side[topV->getVID()] = -1;
		side[bottomV->getVID()] = 2;

		//find vertexed of right chain
		tempV = bottomV;
		while (tempV->getNext() != topV) {
			if (tempV->getNext() == NULL) {
				tempV = headV;
			}
			else
				tempV = tempV->getNext();
			side[tempV->getVID()] = 1;
		}

		
		deque <ply_vertex*> stack;
		stack.push_front(pq.top());
		pq.pop();
		stack.push_front(pq.top());
		pq.pop();
		while (!pq.empty()) {

			tempV = pq.top();
			pq.pop();

			if (pq.size()==0&&stack.size()>=2) {
				//pq is empty
				if (side[tempV->getVID()] == side[stack[1]->getVID()]) {
					if (side[stack[1]->getVID()] == 1)
						side[tempV->getVID()] = -side[stack[1]->getVID()];
					else
						side[tempV->getVID()] = 1;
				}
				
			}
			if (side[tempV->getVID()] != side[stack[0]->getVID()]) {
				//e and v2 are from different sides

				ply_vertex* x = stack.front();
				stack.pop_front();
				ply_vertex* a = x;
				
				while (stack.size() >= 1) {
					ply_vertex* b = stack.front();
					stack.pop_front();
					triangles.push_back(triangle(a->getVID(), b->getVID(), tempV->getVID()));
					if (stack.size() == 0) {
						side[x->getVID()] = -1;
					}
					a = b;				
				}

				stack.push_front(x);
				stack.push_front(tempV);

			}
			else {
				//same side
				bool loop = true;
				while (loop&&stack.size() >= 2) {
					ply_vertex* a,* b;
					a = stack.front();
					stack.pop_front();
					b = stack.front();
					stack.pop_front();
					

					ply_vertex * ta, *tb;
					if (a->getPos()[0]<b->getPos()[0]) {
						ta = b;
						tb = a;
					}
					else
					{
						ta = a;
						tb = b;
					}
					if ( ifleft(ta->getPos(), tb->getPos(), tempV->getPos()) ){
						triangles.push_back(triangle(a->getVID(), b->getVID(), tempV->getVID()));
						if (stack.size() == 0)
							side[b->getVID()] = -1;
						stack.push_front(b);
					}
					else {
						stack.push_back(tempV);
						stack.push_back(a);
						stack.push_back(b);
						loop = false;;
					}
				}
				if (loop) {
					stack.push_front(tempV);
				}
			}
		}
		
	}
}

// handle start vertex
/*void Triangulator::handleStartVertex(ply_vertex* vertex)
{
	//Your implementation here
}

// handle end vertex
void Triangulator::handleEndVertex(ply_vertex* vertex)
{
	//Your implementation here
}

// handle merge vertex
void Triangulator::handleMergeVertex(ply_vertex* vertex)
{
	//Your implementation here
}

// handle split vertex
void Triangulator::handleSplitVertex(ply_vertex* vertex)
{
	//Your implementation here
}


// handle regular up vertex, P lies on left of the vertex
void Triangulator::handleRegularUpVertex(ply_vertex* vertex)
{
	//Your implementation here
}

// handle regular down vertex, P lies on right of the vertex
void Triangulator::handleRegularDownVertex(ply_vertex* vertex)
{
	//Your implementation here
}*/



//-----------------------------------------------------------------------------
//
//
// Private methods below, do not change
//
//
//-----------------------------------------------------------------------------



void Triangulator::addTriangle(vector<triangle>& triangles, int vid1, int vid2, int vid3)
{
	triangle tri(vid1, vid2, vid3);

	triangles.push_back(tri);
}

void Triangulator::addDiagonal(ply_vertex* source, ply_vertex* target)
{
	ply_edge diagonal(source, target);
	this->m_diagonals.push_back(diagonal);

	c_polygon* poly = this->getPolygonByDiagonal(source->getVID(), target->getVID());
	c_polygon ccw = this->findSubPolygonByDiagonal(*poly, source->getVID(), target->getVID(), true);
	c_polygon cw = this->findSubPolygonByDiagonal(*poly, source->getVID(), target->getVID(), false);

	this->m_polys.push_back(cw);
	this->m_polys.push_back(ccw);
}

void Triangulator::insertEdgeIntoBST(ply_vertex* vertex)
{
	double yPos = vertex->getPos()[1];
	ply_edge* edge = new ply_edge(vertex, (vertex->getNext()));
	edge->setHelper(vertex);
	edge->setKeyValue(yPos);

	this->m_edges[vertex->getVID()] = edge;

	this->m_bst.Insert(edge);
}

void Triangulator::deleteEdgeFromBST(ply_edge* edge)
{
	this->m_bst.Delete(edge->keyValue());
}


c_polygon* Triangulator::getPolygonByDiagonal(uint source_vid, uint target_vid)
{
	typedef vector<c_polygon>::iterator PIT;
	for(PIT it = m_polys.begin(); it!= m_polys.end(); ++it)
	{
		if(it->getIsSplit()) continue;

		int count = 0;

		for(int i=0;i<it->getSize();i++)
		{
			ply_vertex* vertex = (*it)[i];
			if(vertex->getVID() == source_vid || vertex->getVID() == target_vid)
				count++;
			if(count == 2) break;
		}

		if(count == 2)
		{
			c_polygon* poly = &(*it);

			poly->setIsSplit(true);

			return poly;
		}
	}

	cerr<<"! Error: Cannot found edge "<<source_vid<<" - "<<target_vid<<endl;

	assert(false);

	return NULL;
}

c_polygon Triangulator::findSubPolygonByDiagonal(c_polygon& poly, uint source_vid, uint target_vid, bool CCW)
{
	ply_vertex* sourceOrg = poly.getVertexByVID(source_vid);
	ply_vertex* targetOrg = poly.getVertexByVID(target_vid);
	ply_vertex* startVetex = CCW ? targetOrg : sourceOrg;
	ply_vertex* endVetex = CCW ? sourceOrg : targetOrg;
	ply_vertex* current = startVetex;

	c_ply sub_poly(c_ply::POUT);
	sub_poly.beginPoly();

	while(current->getVID() != endVetex->getVID())
	{
		sub_poly.addVertex(current->getPos()[0], current->getPos()[1], current->getVID());
		current = current->getNext();
	}

	sub_poly.addVertex(endVetex->getPos()[0], endVetex->getPos()[1], endVetex->getVID());
	sub_poly.endPoly(true);

	c_polygon new_poly;

	new_poly.push_back(sub_poly);

	return new_poly;
}
