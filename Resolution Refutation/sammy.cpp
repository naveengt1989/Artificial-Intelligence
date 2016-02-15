
///////////////////////////////////// 

// File Name :

// Purpose :

// Creation Date : 21-03-2015

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
#include <fstream>
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
#define CL vector<string> 

vector<CL > clauses;
set<CL> visited;
struct CL_PAIR
{
	int i;
	int j;
	int pos_i;
	int pos_j;
	CL_PAIR(int x, int y, int p, int q)
	{
		i=x;		
		j=y;
		pos_i = p;
		pos_j = q;
	}
	bool operator<(const CL_PAIR& x) const
	{
		return min(clauses[i].size(), clauses[j].size()) > min(clauses[x.i].size(), clauses[x.j].size()); 
	}
};

priority_queue<CL_PAIR> candidates;
map<int, pair<int, int> > parent;

void printClause(int x)
{
	cout<<"(";
	for(int i=0; i< clauses[x].size(); i++)
	{
		if(i!=0)
			cout<<" v ";
		cout<<clauses[x][i];
	}
	cout<<")";
	cout<<"\n";
}
void printClauses()
{
	for(int i=0; i< clauses.size(); i++)
	{
		printClause(i);
	}
}
void printClause(CL& cl)
{
	cout<<"(";
	for(int i=0; i< cl.size(); i++)
	{
		if(i!=0)
			cout<<" v ";
		cout<<cl[i];
	}
	cout<<")";
}

bool cmpClauses(const string& x, const string& y)
{
	string str1=x;
	string str2=y;
	if(x[0]=='-')
		str1 = x.substr(1, x.length());
	if(y[0]=='-')
		str2 = y.substr(1, y.length());
	if(str1==str2 && x[0]=='-')
		return true;
	if(str1==str2 && y[0]=='-')
		return false;
	return str1<str2;	
}

void formCandidatePairs(int x, int y)
{
	int i=0;  int j=0;
	while(i!=clauses[x].size() && j!=clauses[y].size())
	{
		string str1 = clauses[x][i]; string str2 = clauses[y][j];
		if(clauses[x][i][0] == '-')
			str1= clauses[x][i].substr(1, clauses[x][i].length());
		if(clauses[y][j][0] == '-')
			str2= clauses[y][j].substr(1, clauses[y][j].length());

		if(str1<str2)
			i++;
		else if(str1>str2)
			j++;
		else
		{
			if((clauses[x][i][0]=='-'&& clauses[y][j][0]!='-') || (clauses[x][i][0]!='-'&& clauses[y][j][0]=='-'))
				candidates.push(CL_PAIR(x,y,i,j));
			i++; j++;
		}
	}
}

bool checkIfVisited(CL clause)
{
	int size = clause.size();
	for(int i=0; i< size; i++)
	{
		if(visited.find(clause)!= visited.end())
			return true;
		clause.pop_back();
	}	
	return false;
}


void printPath(int cl, string space)
{
	cout<<space<<cl<<": ";
	printClause(clauses[cl]);

	if(parent.find(cl) != parent.end())
	{
		cout<<" ["<<(parent.find(cl)->second).first<<","<<(parent.find(cl)->second).second<<"]\n";
		space += "  ";
		printPath((parent.find(cl)->second).first, space);
		printPath((parent.find(cl)->second).second, space);
	}
	else
		cout<<" input\n";	
}

bool resolve(CL_PAIR current)
{
#ifdef DEBUG 
	string proposition;
	if(clauses[current.i][current.pos_i][0] == '-')
		proposition = clauses[current.j][current.pos_j];
	else
		proposition = clauses[current.i][current.pos_i];
		
	cout<<"[Qsize="<<candidates.size()<<"] "<<"resolving "<<current.i<<" and "<<current.j<<" on "<<proposition<<": ";
	printClause(clauses[current.i]);
	cout<<" and ";
	printClause(clauses[current.j]);
	
	//cout<<"###########################################\n";
	//printClause(current.i);
	//printClause(current.j);
	//cout<<"Resolving on proposition: "<<clauses[current.i][current.pos_i]<<" "<<clauses[current.j][current.pos_j]<<"\n";	
#endif
	// form new clause by resolution
	CL newClause;
	set<string> seen;
	for(int i=0; i<clauses[current.i].size(); i++)
	{
		if(i != current.pos_i)
		{
			newClause.push_back(clauses[current.i][i]);
			seen.insert(clauses[current.i][i]);
		}
	}
	for(int i=0; i<clauses[current.j].size(); i++)
	{
		if(i != current.pos_j && seen.find(clauses[current.j][i])==seen.end() )
			newClause.push_back(clauses[current.j][i]);
	}
	if(newClause.size()==0)
	{
		clauses.push_back(newClause);
#ifdef DEBUG
		cout<<"  --> ";
		printClause(clauses[clauses.size()-1]);
#endif
#ifdef DEBUG
		cout<<"\n"<<clauses.size()-1<<": ";
		printClause(clauses.size()-1);
#endif
		parent.insert(make_pair(clauses.size()-1,make_pair(current.i, current.j)));
		visited.insert(newClause);
		cout<<"success - empty clause!\n";
		cout<<"-------------------------\n";
		cout<<"proof trace:\n";
		printPath(clauses.size()-1, "");
		return true;
	}
	sort(newClause.begin(), newClause.end(), cmpClauses);
	//if(visited.find(newClause) == visited.end())
	if(!checkIfVisited(newClause))
	{
		clauses.push_back(newClause);
#ifdef DEBUG
		cout<<"  --> ";
		printClause(clauses[clauses.size()-1]);
#endif
		parent.insert(make_pair(clauses.size()-1,make_pair(current.i, current.j)));
		visited.insert(newClause);
#ifdef DEBUG
		cout<<"\n"<<clauses.size()-1<<": ";
		printClause(clauses.size()-1);
#endif
		// for new clause, generate clause pairs with existing clauses;
		for(int i=0; i< clauses.size()-1; i++)
			formCandidatePairs(i, clauses.size()-1);
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
	ifstream fp(argv[1]);
	string line;
	while(getline(fp, line))
	{
		if(line != "\n" && line[0] != '#')
		{
			istringstream iss(line);
			clauses.push_back(CL());
			while(iss)
			{
				string word; iss >> word;
				if(word != "")
					clauses[clauses.size()-1].push_back(word);
			};
			sort(clauses[clauses.size()-1].begin(), clauses[clauses.size()-1].end(), cmpClauses);
			// for new clause, generate clause pairs with existing clauses;
			for(int i=0; i< clauses.size()-1; i++)
				formCandidatePairs(i, clauses.size()-1);
		}
	}
#ifdef DEBUG
	cout<<"Initial Clauses\n";	
	printClauses();
#endif
	bool result=false;
	while(!candidates.empty())
	{
		CL_PAIR current = candidates.top();
		candidates.pop();
		if(resolve(current))
		{
			result = true;
			break;
		}
	}
	if(result)
		cout<<"SUCCESS\n";
	else
		cout<<"\nFAILURE\n";	

	return 0;
}


