
///////////////////////////////////// 

// File Name :

// Purpose :

// Creation Date : 06-05-2015

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
#include <fstream>
#define CLR(a, x) memset( a, x, sizeof( a ) )
using namespace std;
#define getcx getchar_unlocked
#define ULL unsigned long long

enum options
{
	PRECOND=0,
	ADDLIST=1,
	DELLIST=2,
	CONFLICT=3,
};

struct oper
{
	string name;
	set<string> options[4];
	oper(string name)
	{
		this->name=name;
	}
	void print()
	{
		cout<<name<<"\n";
		for(int i=0; i<4; i++)
		{
			for(set<string>::iterator it= options[i].begin(); it != options[i].end(); it++ )	
			{
				cout<<*it<<" ";
			}
			cout<<"\n";
		}
	}
};

void parseInput(vector<oper>& operations, char* filename)
{
	ifstream fp(filename);
	string line;
	options opt;
	while(getline(fp, line))
	{
		istringstream iss(line);
		while(iss)
		{
			string word; iss >> word;	
			if(word=="OPER")
			{
				iss >> word;
				operations.push_back(oper(word));
			}
			else if(word=="precond:")
				opt=PRECOND;
			else if(word=="addlist:")
				opt=ADDLIST;
			else if(word=="dellist:")
				opt=DELLIST;
			else if(word=="conflict:")
				opt=CONFLICT;	
			else if(word!= "" && word!= "END")
			{
				operations[operations.size()-1].options[opt].insert(word);
			}	
		}
	}
}

struct plan
{
	set<string> goals;
	vector<string> actions;
	bool operator<(const plan& pn) const
	{
		return (actions.size()>pn.actions.size());
	}
	plan(set<string> gl)
	{
		goals=gl;
	}
	void printGoals()
	{
		//cout<<"#########################################################\n";
		//cout<<"GOALS:\n";
		for(set<string>::iterator it=goals.begin(); it!= goals.end(); it++)	
		{
			cout<<*it<<" ";
		}
	}
	void printActions()
	{
		//cout<<"\nACTIONS:\n";
		for(int i=actions.size()-1; i>=0; i--)
		{
			cout<<actions[i]<<"\n";
		}	
		//cout<<"\n###########################################################\n";*/
	}
};

void parseInput(set<string>& init, char* filename)
{
	ifstream fp(filename);
	string line;
	options opt;
	while(getline(fp, line))
	{
		istringstream iss(line);
		while(iss)
		{
			string word; iss >> word;	
			if(word!="")
				init.insert(word);
		}
	}
}

void parseInput(set<string>& goals, string input)
{
	istringstream iss(input);
	while(iss)
	{
		string word; iss >> word;	
		if(word!="")
			goals.insert(word);
	}
}

bool checkIfSatisfied(plan& pn, set<string>& init)
{
	for(set<string>::iterator it=pn.goals.begin(); it!= pn.goals.end(); it++)
	{
		if(init.find(*it) == init.end())
			return false;
	}	
	return true;
}

bool checkIfIntersection(plan& pn, set<string>& dellist, set<string>& conflict)
{
	for(set<string>::iterator it=pn.goals.begin(); it!= pn.goals.end(); it++)
	{
		if((dellist.find(*it) != dellist.end()) || (conflict.find(*it) != conflict.end()))
			return true;
	}
	return false;
}

void regress(plan& pn, set<string>& addlist, set<string>& precond, string action)
{
	for(set<string>::iterator it=addlist.begin(); it!= addlist.end(); it++)
	{
		pn.goals.erase(*it);
	}
	for(set<string>::iterator it=precond.begin(); it!= precond.end(); it++)
	{
		pn.goals.insert(*it);
	}
	pn.actions.push_back(action);	
}

bool goal_regression(set<string>& goals, vector<oper>& operations, set<string>& init)
{
	set<set<string> > visited;
	unsigned long long iter=0;
	priority_queue<plan> qt; 
	plan initial = plan(goals);
	qt.push(initial);
	visited.insert(initial.goals);
	initial.printGoals();
	while(!qt.empty())
	{
		iter++;
		cout<<"iter="<<iter<<", queue="<<qt.size()<<"\n";
		plan cur = qt.top(); qt.pop();
		cout<<"context: ";
		cur.printActions();
		cout<<"\ngoal stack: ";
		cur.printGoals();
		cout<<"\n";		
		//cur.print();
		if(checkIfSatisfied(cur, init))
		{
			cout<<"solution found!\n";
			cout<<"### plan:\n";
			cur.printActions();
			return true;
		}
		else
		{	
			for(set<string>::iterator it=cur.goals.begin(); it!= cur.goals.end(); it++)
			{
				for(int i=0; i< operations.size(); i++)
				{
#ifdef DEBUG
					cout<<"considering using "<<operations[i].name<<" to achieve "<<*it<<"\n";
#endif
					// check if current operation achieves goal -> (*it)
					if(operations[i].options[ADDLIST].find(*it) == 	operations[i].options[ADDLIST].end() )
						continue;
					// check if current operation-dellist & conflictlist intersects with any current sub-goals in the plan
					if(checkIfIntersection(cur, operations[i].options[DELLIST], operations[i].options[CONFLICT]))
						continue;
					plan newPlan = cur;
					regress(newPlan,  operations[i].options[ADDLIST], operations[i].options[PRECOND], operations[i].name);	
					if(visited.find(newPlan.goals) == visited.end())
					{
						qt.push(newPlan);
						visited.insert(newPlan.goals);
					}
				}	
			}
		}
	}
	return false;
}

int main(int argc, char* argv[])
{
	if(argc<2)
	{
		cout<<"Invalid arguments..\n";
		return 0;
	}
#ifndef DEBUG
	cout<<"Compile with -DDEBUG flag for detailed prints\n";
#endif
	vector<oper> operations;
	set<string> init; 
	parseInput(operations, argv[1]);
	parseInput(init, argv[2]);
	while(1)
	{
		cout<<"> ";
		string input;	
		getline(cin,input);
		if(input=="quit")
			break;
		cout<<"initiating goal regression...\n";
		set<string> goals;
		parseInput(goals, input);
		cout<<"PROBLEM: "<<input<<"\n";
		//goals.insert("on(a,d)");
		//goals.insert("holding(b)");
		//goals.insert("on(d,b)");
		if(goal_regression(goals, operations, init))
			cout<<"SUCCESS\n\n";
		else
			cout<<"FAILURE\n\n";
	}
	//operations[0].print();
	return 0;
}

