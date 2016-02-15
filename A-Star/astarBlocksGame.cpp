
///////////////////////////////////// 

// File Name :

// Purpose :

// Creation Date : 28-01-2015

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
using namespace std;

int NUM_STACKS;
int NUM_BLOCKS;

int calcHeuristicCost(const vector<vector<char> >& stacks)
{
	int sorted_stack0=0;
	int notSorted_stack0=0;
	int sorted_NotStack0=0;
	int notSorted_NotStack0=0;

	char start = 'A'; int i;
	for(i=0; i < stacks[0].size(); i++)
	{
		if(stacks[0][i] > start)
			break;
		else
			start++;
	}
	sorted_stack0 = i;
	notSorted_stack0 = stacks[0].size()-i;

	for(int j=1; j < stacks.size(); j++)
	{
		vector<char> heap;
		for(int k=0; k< stacks[j].size(); k++)
		{
			heap.push_back(stacks[j][k]);
		}	
		make_heap(heap.begin(), heap.end());
		int l;
		for(l=0; l< stacks[j].size(); l++)
		{
			if(stacks[j][l] == heap[l])
			{
				pop_heap(heap.begin(), heap.end());
				heap.pop_back();
			}
			else
				break;
		}
		sorted_NotStack0 = l;
		notSorted_NotStack0 = stacks[j].size()-l;
	}
	
//	return  (2*notSorted_stack0 + sorted_NotStack0 + 2*notSorted_NotStack0 );
	return  (2*notSorted_stack0 + sorted_NotStack0 + 2*notSorted_NotStack0 - 2*sorted_stack0);

}

bool checkOptimality(const vector<vector<char> >& cur_stacks, const vector<vector<char> >& child_stacks)
{
	int sorted_curstack0=0;
	char start = 'A'; int i;
	for(i=0; i < cur_stacks[0].size(); i++)
	{
		if(cur_stacks[0][i] > start)
			break;
		else
			start++;
	}
	sorted_curstack0 = i;

	int sorted_childstack0=0;
	start = 'A'; 
	for(i=0; i < child_stacks[0].size(); i++)
	{
		if(child_stacks[0][i] > start)
			break;
		else
			start++;
	}
	sorted_childstack0 = i;

	if(sorted_curstack0 > sorted_childstack0)
		return false;
	else
		return true;

}

struct State
{
	vector<vector<char> > stacks;
	long long g;
	long long h;
	pair<int, int> track;
	State(vector<vector<char> >& st, int x, int y)
	{
		stacks = st;
		g=x;
		h=y;
		track = make_pair(-1, -1);
	}
	State(int x, int y)
	{
		g=x;
		h=y;
		track = make_pair(-1, -1);
	}
	bool operator<(const State& p)const 
	{
		return (h+g) > (p.g + p.h);
	}
};


void print(const State& state)
{
	cout<<"#########################\n";
	for(int i=0; i< state.stacks.size(); i++)
	{
		cout<<"("<<i+1<<")";
		for(int j=0; j< state.stacks[i].size(); j++)
		{
			cout<<state.stacks[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"#########################\n";
}	

void init(State& initialState)
{
	for (int i=0; i< NUM_STACKS; i++)
	{
		initialState.stacks.push_back(vector<char>());
	}
}

int genRandomNumber(int min, int max)
{
	srand(time(0)); // Ensures rand will generate different numbers at different times
	if(min==max)
		return min;
	int range = max - min;

	int num = (rand() % range) + min;
	return num;
}

void assign(State& initialState)
{
	vector<char> randomList;
	for(int i=0; i< NUM_BLOCKS; i++)
	{
		srand ( unsigned ( time(0) ) );
		randomList.push_back(i+'A');
		random_shuffle(randomList.begin(), randomList.end());	
	}
	// compute distribution random list of blocks among stacks
	vector<int> distPoints;
	for(int i=0; i< NUM_BLOCKS; i++)
	{
		distPoints.push_back(i);
	}
	random_shuffle(distPoints.begin(), distPoints.end());
	distPoints.erase(distPoints.begin()+NUM_STACKS-1, distPoints.end());
	sort(distPoints.begin(), distPoints.end());
	int cnt=0;
	for(int i=0; i< distPoints.size(); i++)
	{
		while(cnt<distPoints[i])
		{
			initialState.stacks[i].push_back(randomList[cnt]);
			cnt++;
		}
	}
	while(cnt<NUM_BLOCKS)
	{
		initialState.stacks[NUM_STACKS-1].push_back(randomList[cnt]);
		cnt++;
	}

	/*initialState.stacks[0].push_back('B');
	initialState.stacks[1].push_back('C');
	initialState.stacks[1].push_back('E');
	initialState.stacks[2].push_back('A');
	initialState.stacks[2].push_back('D');*/

	/*initialState.stacks[0].push_back('D');
	initialState.stacks[1].push_back('E');
	initialState.stacks[1].push_back('F');
	initialState.stacks[1].push_back('I');
	initialState.stacks[1].push_back('J');
	initialState.stacks[2].push_back('B');
	initialState.stacks[2].push_back('G');
	initialState.stacks[3].push_back('C');
	initialState.stacks[3].push_back('H');
	initialState.stacks[4].push_back('A');*/

}

bool isGoal(const State& st)
{
	for(int i=1; i< st.stacks[0].size(); i++)
	{
		if(st.stacks[0][i] < st.stacks[0][i-1])
			return false;
	}
	for(int i=1; i< NUM_STACKS; i++)
	{
		if(st.stacks[i].size() != 0)
		{
			return false;
		}
	}
	return true;
}

void createCopy(const State& src, State& dest)
{
	for(int i=0; i< src.stacks.size(); i++)
	{
		for(int j=0; j< src.stacks[i].size(); j++)
		{
			dest.stacks[i].push_back(src.stacks[i][j]);
		}
	}	
}


void printPath(map<vector<vector<char> >, pair<int, int> >& visited, State& goal)
{
	map<vector<vector<char> >, pair<int, int> >::iterator it;
	it = visited.find(goal.stacks);
	stack<State> states;
	while(it->second.first != -1 && it->second.second != -1)
	{
		states.push(goal);
		//print(goal);
		// go to parent
		goal.stacks[it->second.second].push_back(goal.stacks[it->second.first].back());
		goal.stacks[it->second.first].pop_back();
		it = visited.find(goal.stacks);
	}
	states.push(goal);
	//print(goal);
	cout<<"PATH LENGTH = "<<states.size()-1<<"\n";
	while(!states.empty())
	{
		print(states.top());
		states.pop();
	}

}

int main(int argc, char* argv[])
{
	if(argc<3)
	{
		cout<<"INVALID INPUT...Enter ./a.out <No. Stacks> <No. Blocks>\n";
		return 0;
	}
	else
	{
		NUM_STACKS = atoi(argv[1]);
		NUM_BLOCKS = atoi(argv[2]);
	}
	
	State initialState(0,0);
	init(initialState);
	assign(initialState);// assign random blocks configuration to stacks
#ifdef DEBUG
	cout<<"Initial State:\n";
	print(initialState);
#endif
	// performance metrics
	long long num_iterations=0;
	long long max_queue_size=0;
	/////////////////////
	

	priority_queue<State> qt;
	map<vector<vector<char> >, pair<int, int> > visited;
	initialState.h = calcHeuristicCost(initialState.stacks);	
	qt.push(initialState);
	visited.insert(make_pair(initialState.stacks, make_pair(-1, -1)));
	State current(0, 0);
	while(!qt.empty())
	{
		current = qt.top();
		num_iterations++;// goal tests
		qt.pop();
#ifdef DEBUG
		cout<<"iter="<<num_iterations<<", queue="<<qt.size()<<", f=g+h="<<(current.g+current.h)<<", depth="<<current.g<<"\n";
#endif
		if(isGoal(current)) break;
		// generate all possible successor states	
		for(int i=0; i< NUM_STACKS; i++)
		{
			if(current.stacks[i].size() != 0)
			{
				for(int j=(i+1)%NUM_STACKS; j!=i; j= (j+1)%NUM_STACKS)
				{
					State child(0,0);
					init(child);					
					createCopy(current, child);
					child.stacks[j].push_back(child.stacks[i].back());
					child.stacks[i].pop_back();
					bool checkOptimal = checkOptimality(current.stacks, child.stacks);
					child.g = current.g +1;
					child.h = calcHeuristicCost(child.stacks);
					if(visited.find(child.stacks) == visited.end() && checkOptimal==true)
					{
						qt.push(child);
						max_queue_size = max(max_queue_size, (long long)qt.size());
						visited.insert(make_pair(child.stacks, make_pair(j, i)));
					}
				}
			}
		}
	}
	//print(current);	
	cout<<" NUMBER OF ITERATIONS = "<<num_iterations<<" MAX QUEUE SIZE =  "<<max_queue_size<<"\n";
	printPath(visited, current);
	return 1;




}


