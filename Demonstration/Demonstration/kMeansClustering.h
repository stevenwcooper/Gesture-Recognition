/*****************************************************************************

	Author:Steven Cooper
	Project: Computer Vision for Interactive Applications
	Version:1.0 Date:30/01/2014 details: creation of k means clustering class

*****************************************************************************/

#ifndef _K_MEANS_CLUSTERING_

#define _K_MEANS_CLUSTERING_

#include <vector>

#define PYTHAG(nodeX, nodeY, pointX, pointY) (sqrt(((nodeX - pointX) * (nodeX - pointX)) + ((nodeY - pointY) * (nodeY - pointY))))

using namespace std;

struct Node
{
	int x,y,count;
};

struct Point 
{ 
	int x, y; 
	Node closestNode;
};

class kMeansClustering
{
public:
	kMeansClustering(int nNodes);
	~kMeansClustering();
protected:
	void GenerateRandomNodePoints(int nNodes);

	void CalculateDistance();

	void CalculateMeans();

	void clearNodes();
private:
	vector<Node> Nodes;
	vector<Point> Points;

	int minX;
	int minY;
	int maxX;
	int maxY;

	bool change;
};

kMeansClustering::kMeansClustering(int nNodes)
{
	change = false;

	//add points to Points? already generated?

	GenerateRandomNodePoints(nNodes);

	do
	{
		CalculateDistance();
		CalculateMeans();
	}
	while(change);


	//do something with Nodes
}

kMeansClustering::~kMeansClustering()
{

}

void kMeansClustering::GenerateRandomNodePoints(int nNodes)
{
	for(int i = 0; i < nNodes; i++)
	{
		Node newNode;
		newNode.x = rand() % (maxX - minX) + minX;
		newNode.y = rand() % (maxY - minY) + minY;
		newNode.count = 0;
		Nodes.push_back(newNode);
	}
}

void kMeansClustering::CalculateDistance()
{
	vector<Node> tempNodes = Nodes;
	
	clearNodes();

	for(int i = 0; i < Points.size(); i++)
	{
		int distance = 0, tempNode;
		for(int j = 0; j < tempNodes.size(); j++)
		{
			int tempdist = PYTHAG(tempNodes.at(j).x, tempNodes.at(j).y, Points.at(i).x, Points.at(i).y);
			if(tempdist > distance)
			{
				Points.at(i).closestNode = tempNodes.at(j);
				distance = tempdist;
				tempNode = j;
				change = true;
			}
		}
		Nodes.at(tempNode).x += Points.at(i).x;
		Nodes.at(tempNode).y += Points.at(i).y;
		Nodes.at(tempNode).count++;
	}
}


void kMeansClustering::CalculateMeans()
{
	for(int i = 0; i < Nodes.size(); i++)
	{
		Nodes.at(i).x /= Nodes.at(i).count;
		Nodes.at(i).y /= Nodes.at(i).count;
	}
}

void kMeansClustering::clearNodes()
{
	for(int i = 0; i < Nodes.size(); i++)
	{
		Nodes.at(i).x = 0;
		Nodes.at(i).y = 0;
		Nodes.at(i).count = 0;
	}
}
#endif // !_K_MEANS_CLUSTERING_
