
///////////////////////////////////// 

// File Name : trackPath.cpp

// Purpose : Programming Assignment1- CSCE 625

// Creation Date : 26-01-2015

// Last Modified : <modified_date>

// Created By : Naveen Thomas 

//////////////////////////////////////
#include <string>
#include <vector>
#include <map>
#include <list>
#include <iterator>
#include <set>
#include <queue>
#include <iostream>
#include <sstream>
#include <stack>
#include <deque>
#include <cmath>
#include <memory.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <utility> 
#define CLR(a, x) memset( a, x, sizeof( a ) )
using namespace std;
#define getcx getchar_unlocked
#define ULL unsigned long long
// Statistics/Performance Metrics
int total_iterations;
int max_frontier_size;
int vertices_visited;
int path_length;
////////////////////////////////
struct Point
{
	int x;
	int y;
	Point(int a, int b)
	{
		x=a;
		y=b;
	}
	bool operator==(const Point& pt) const
	{
		return (pt.x == x && pt.y ==y);
	}
};

struct DistanceNode
{
	int pointId;
	int distance;
	int depth;
	DistanceNode(int a, int b, int c)
	{
		pointId=a;
		distance=b;
		depth =c;
	}
	bool operator<(const DistanceNode& dt) const
	{
		return (distance > dt.distance);
	}
};

int findDistance(const Point& a, const Point& b)
{
	return (abs(a.x-b.x)*abs(a.x-b.x) + abs(a.y-b.y)*abs(a.y-b.y));
}
class Graph
{
	public:
	int V;
	vector<Point> vertexPos;
	vector<int>* adj;

	Graph(int vertices)
	{
		V=vertices;
		adj = new vector<int>[V];
	}
};

struct QNode
{
	int id;
	int depth;
	QNode(int x, int y)
	{
		id =x;
		depth=y;
	}
};

bool *visited;
int *parent;

void printPath(stack<int>& path, const Graph& gt, string searchAlgo)
{
	cout<<"Solution Path("<<searchAlgo<<")\n";
	while(!path.empty())
	{
		cout<<"vertex "<<path.top()<<" ("<<gt.vertexPos[path.top()].x<<","<<gt.vertexPos[path.top()].y<<")\n";
		path.pop();
	}
}

bool gbfs_search(const Graph& gt,const Point& src,const Point& destination)
{
	priority_queue<DistanceNode> dt; int srcId=-1; int destinationId=-1;
	// find vertex id of src, destination points.
	for(int i=0; i< gt.vertexPos.size(); i++)
	{
		if(src == gt.vertexPos[i])
			srcId = i;
		else if(destination == gt.vertexPos[i])
			destinationId = i;	
	
		if(srcId != -1 && destinationId != -1)
			break;
	}	
#ifdef DEBUG
	cout<<"start=("<<src.x<<","<<src.y<<"), goal=("<<destination.x<<","<<destination.y<<"), vertices: "<<srcId<<" and "<<destinationId<<"\n";
#endif
	dt.push(DistanceNode(srcId, findDistance(gt.vertexPos[srcId], gt.vertexPos[destinationId]), 0));
	visited[srcId]=true;
	parent[srcId]=srcId;
	while(!dt.empty())
	{
		total_iterations++;
		DistanceNode dn = dt.top();
		int cur_depth = dn.depth;
		dt.pop();
#ifdef DEBUG
		cout<<"iter="<<total_iterations<<", frontier="<<dt.size()<<", popped="<<dn.pointId<<" ("<<gt.vertexPos[dn.pointId].x<<","<<gt.vertexPos[dn.pointId].y<<"), depth="<<cur_depth<<", dist2goal="<<sqrt(findDistance(gt.vertexPos[dn.pointId],destination))<<"\n";
#endif
		for(int i=0; i< gt.adj[dn.pointId].size(); i++)
		{
			if(visited[gt.adj[dn.pointId][i]] != true)
			{
				visited[gt.adj[dn.pointId][i]]=true;
				vertices_visited++;
				parent[gt.adj[dn.pointId][i]]=dn.pointId;
				if(gt.adj[dn.pointId][i] == destinationId)
				{
					stack<int> graphPath;
					while(parent[destinationId] != destinationId)
					{
						graphPath.push(destinationId);
						destinationId = parent[destinationId];
					}
					graphPath.push(srcId);
					path_length = graphPath.size()-1;
					printPath(graphPath, gt, "GBFS");
					return true;
				}
				dt.push(DistanceNode(gt.adj[dn.pointId][i], findDistance(gt.vertexPos[destinationId], gt.vertexPos[gt.adj[dn.pointId][i]]), cur_depth+1));
#ifdef DEBUG
				cout<<"pushed "<<gt.adj[dn.pointId][i]<<" ("<<gt.vertexPos[gt.adj[dn.pointId][i]].x<<","<<gt.vertexPos[gt.adj[dn.pointId][i]].y<<")\n";
#endif
				max_frontier_size = max(max_frontier_size, (int)dt.size());
			}
		}
	}
	cout<<"NO PATH EXISTS BETWEEN ("<<src.x<<","<<src.y<<") AND ("<<destination.x<<","<<destination.y<<")\n";
	return false;
}


bool dfs_search(const Graph& gt,const Point& src,const Point& destination)
{
	stack<QNode> st; int srcId=-1; int destinationId=-1;
	// find vertex id of src, destination points.
	for(int i=0; i< gt.vertexPos.size(); i++)
	{
		if(src == gt.vertexPos[i])
			srcId = i;
		else if(destination == gt.vertexPos[i])
			destinationId = i;	
	
		if(srcId != -1 && destinationId != -1)
			break;
	}	
#ifdef DEBUG
	cout<<"start=("<<src.x<<","<<src.y<<"), goal=("<<destination.x<<","<<destination.y<<"), vertices: "<<srcId<<" and "<<destinationId<<"\n";
#endif
	st.push(QNode(srcId, 0));
	visited[srcId]=true;
	parent[srcId]=srcId;
	while(!st.empty())
	{
		total_iterations++;
		int ptId = st.top().id;
		int cur_depth = st.top().depth;
		st.pop();
#ifdef DEBUG
		cout<<"iter="<<total_iterations<<", frontier="<<st.size()<<", popped="<<ptId<<" ("<<gt.vertexPos[ptId].x<<","<<gt.vertexPos[ptId].y<<"), depth="<<cur_depth<<", dist2goal="<<sqrt(findDistance(gt.vertexPos[ptId],destination))<<"\n";
#endif
		for(int i=0; i< gt.adj[ptId].size(); i++)
		{
			if(visited[gt.adj[ptId][i]] != true)
			{
				visited[gt.adj[ptId][i]]=true;
				vertices_visited++;
				parent[gt.adj[ptId][i]]=ptId;
				if(gt.adj[ptId][i] == destinationId)
				{
					stack<int> graphPath;
					while(parent[destinationId] != destinationId)
					{
						graphPath.push(destinationId);
						destinationId = parent[destinationId];
					}
					graphPath.push(srcId);
					path_length = graphPath.size()-1;
					printPath(graphPath, gt, "DFS");
					return true;
				}
				st.push(QNode(gt.adj[ptId][i], cur_depth+1));
#ifdef DEBUG
				cout<<"pushed "<<gt.adj[ptId][i]<<" ("<<gt.vertexPos[gt.adj[ptId][i]].x<<","<<gt.vertexPos[gt.adj[ptId][i]].y<<")\n";
#endif
				max_frontier_size = max(max_frontier_size, (int)st.size());
			}
		}
	}
	cout<<"NO PATH EXISTS BETWEEN ("<<src.x<<","<<src.y<<") AND ("<<destination.x<<","<<destination.y<<")\n";
	return false;
}

bool bfs_search(const Graph& gt, const Point& src, const Point& destination)
{
	queue<QNode> qt; int srcId=-1; int destinationId=-1;
	// find vertex id of src, destination points.
	for(int i=0; i< gt.vertexPos.size(); i++)
	{
		if(src == gt.vertexPos[i])
			srcId = i;
		else if(destination == gt.vertexPos[i])
			destinationId = i;	
	
		if(srcId != -1 && destinationId != -1)
			break;
	}	
#ifdef DEBUG
	cout<<"start=("<<src.x<<","<<src.y<<"), goal=("<<destination.x<<","<<destination.y<<"), vertices: "<<srcId<<" and "<<destinationId<<"\n";
#endif
	qt.push(QNode(srcId, 0));
	visited[srcId]=true;
	parent[srcId]=srcId;
	while(!qt.empty())
	{
		total_iterations++;
		int ptId = qt.front().id;
		int cur_depth=qt.front().depth;
		qt.pop();
#ifdef DEBUG
		cout<<"iter="<<total_iterations<<", frontier="<<qt.size()<<", popped="<<ptId<<" ("<<gt.vertexPos[ptId].x<<","<<gt.vertexPos[ptId].y<<"), depth="<<cur_depth<<", dist2goal="<<sqrt(findDistance(gt.vertexPos[ptId],destination))<<"\n";
#endif
		for(int i=0; i< gt.adj[ptId].size(); i++)
		{
			if(visited[gt.adj[ptId][i]] != true)
			{
				visited[gt.adj[ptId][i]]=true;
				vertices_visited++;
				parent[gt.adj[ptId][i]]=ptId;
				if(gt.adj[ptId][i] == destinationId)
				{
					stack<int> graphPath;
					while(parent[destinationId] != destinationId)
					{
						graphPath.push(destinationId);
						destinationId = parent[destinationId];
					}
					graphPath.push(srcId);
					path_length = graphPath.size()-1;
					printPath(graphPath, gt, "BFS");
					return true;
				}
				qt.push(QNode(gt.adj[ptId][i], cur_depth+1));
#ifdef DEBUG
				cout<<"pushed "<<gt.adj[ptId][i]<<" ("<<gt.vertexPos[gt.adj[ptId][i]].x<<","<<gt.vertexPos[gt.adj[ptId][i]].y<<")\n";
#endif
				max_frontier_size = max(max_frontier_size, (int)qt.size());
			}
		}
	}
	cout<<"NO PATH EXISTS BETWEEN ("<<src.x<<","<<src.y<<") AND ("<<destination.x<<","<<destination.y<<")\n";
	return false;
}
void init(int n)
{
	memset(visited, 0, sizeof(bool)*n);
	memset(parent, -1, sizeof(int)*n);
	total_iterations = 0;
	max_frontier_size = -1;
	vertices_visited=0;
	path_length=0;
}

void printStatistics(string searchAlgo, int V)
{
	cout<<"\nSummary Statistics:\n";
	cout<<"search algorithm = "<<searchAlgo<<"\n";
	cout<<"total iterations = "<<total_iterations<<"\n";
	cout<<"max frontier size = "<<max_frontier_size<<"\n";
	cout<<"vertices visited = "<<vertices_visited<<"/"<<V<<"\n";
	cout<<"path length = "<<path_length<<"\n\n";

}

int main(int argc, char* argv[])
{
	////////////// input graph //////////////	
	if(argc<6)
	{
		cout<<"INVALID INPUT\n";
		return 0;
	}
	FILE *fp = fopen(argv[1], "r+");
	char str[80]; int n;
	fscanf(fp,"%s %d", str, &n);
#ifdef DEBUG
	cout<<"vertices = "<<n<<", ";
#endif
	Graph gt(n);
	visited =  new bool[n];
	parent =  new int[n];
	while(n--)
	{
		int i,x,y;
		fscanf(fp,"%d %d %d", &i, &x, &y);
		gt.vertexPos.push_back(Point(x,y));	
	}
	fscanf(fp,"%s %d", str, &n);
#ifdef DEBUG
	cout<<"edges = "<<n<<"\n";
#endif
	while(n--)
	{
		int i, u, v;
		fscanf(fp,"%d %d %d", &i, &u, &v);
		gt.adj[u].push_back(v);	
		gt.adj[v].push_back(u);	
	}
	Point src(atoi(argv[2]), atoi(argv[3]));
	Point destination(atoi(argv[4]), atoi(argv[5]));
	///////////////////////////////////////////
#ifdef BFS
	init(gt.V);
	bfs_search(gt, src, destination);
	printStatistics("BFS", gt.V);
#endif
#ifdef DFS
	init(gt.V);
	dfs_search(gt, src, destination);
	printStatistics("DFS", gt.V);
#endif
#ifdef GBFS
	init(gt.V);
	gbfs_search(gt, src, destination);
	printStatistics("GBFS", gt.V);
#endif
	return 0;
}


