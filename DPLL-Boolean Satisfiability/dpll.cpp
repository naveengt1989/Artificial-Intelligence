
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

enum status
{
	UNKNOWN=-1,
	FALSE=0,
	TRUE=1,
};
string boolValues[]={"FALSE", "TRUE"};
int num_nodes=0;

void printClause(int x, vector<CL>& clauses, map<string, status>& variables)
{
	cout<<"( ";
	for(int i=0; i< clauses[x].size(); i++)
	{
		if(i!=0)
			cout<<" v ";
		string var = (clauses[x][i][0]=='-')?clauses[x][i].substr(1, clauses[x][i].length()):clauses[x][i];
		cout<<clauses[x][i];
	}
	cout<<" )";
}
void printClauses(vector<CL>& clauses, map<string, status>& variables)
{
	cout<<"Model={ \n";
	/*for(int i=0; i< clauses.size(); i++)
	{
		printClause(i, clauses, variables);
	}*/
	for(map<string, status>::iterator it=variables.begin(); it != variables.end(); it++)
	{
		if(it->second != UNKNOWN)
			cout<<" "<<it->first<<": "<<boolValues[it->second]<<";\n";
	}
	cout<<" }\n";
}
/*void printClause(CL& cl)
{
	cout<<"(";
	for(int i=0; i< cl.size(); i++)
	{
		if(i!=0)
			cout<<" v ";
		string var = (cl[i][0]=='-')?cl[i].substr(1, cl[i].length()):cl[i];
		cout<<cl[i]<<"("<<variables.find(var)->second<<")";
	}
	cout<<")";
}*/
#define INVALID 9999
void deleteClauses(set<int>& toBeDeleted, vector<CL>& clauses, map<string,status>& variables)
{
	num_nodes++;
	vector<CL> newClauses;
	for(int i=0; i< clauses.size(); i++)
	{	
		if(toBeDeleted.find(i)==toBeDeleted.end())
		{
			newClauses.push_back(clauses[i]);
		}
		else
		{
			//cout<<"Removed-->";
			//printClause(i, clauses, variables);	
		}
	}
	clauses = newClauses;
	toBeDeleted.clear();
}

void updateClauses(set<int>& toBeDeleted, vector<CL>& clauses, map<string,status>& variables)
{
	num_nodes++;
	//cout<<"Update Clauses\n";
	// update clauses
	for(int i=0; i< clauses.size(); i++)
	{
		if(toBeDeleted.find(i)!=toBeDeleted.end())
			continue;
		CL newCl;
		for(int j=0; j< clauses[i].size(); j++)
		{
			string prop = (clauses[i][j][0]=='-')?clauses[i][j].substr(1, clauses[i][j].length()):clauses[i][j];
			map<string, status>::iterator it = variables.find(prop);
			if((it->second == TRUE && clauses[i][j][0]!='-') || (it->second == FALSE && clauses[i][j][0]=='-'))
			{
				toBeDeleted.insert(i); break;
			}
			else if(it->second == UNKNOWN)
				newCl.push_back(clauses[i][j]);
		}
		clauses[i]  = newCl;
	}
}

bool removeUnitClauses(vector<CL>& clauses, map<string,status>& variables)
{
	//cout<<"Remove Unit Clauses\n";
	bool continueIter=true;
	set<int> toBeDeleted;
	while(continueIter)
	{ 
		continueIter=false;
		for(int i=0; i< clauses.size(); i++)
		{	
			if(toBeDeleted.find(i)!=toBeDeleted.end())
				continue;
			if(clauses[i].size() == 1)
			{
				continueIter=true;
				string prop = (clauses[i][0][0]=='-')?clauses[i][0].substr(1, clauses[i][0].length()):clauses[i][0];
				if(clauses[i][0][0] == '-')
				{
#ifdef DEBUG
					cout<<" Residual Unit Clause: "; printClause(i, clauses, variables);cout<<" implies "<<prop<<" = FALSE\n";
#endif
					variables.find(prop)->second=FALSE;
					//cout<<"Updating Variable: "<<variables.find(prop)->first<<" - "<<variables.find(prop)->second<<"\n";
				}
				else
				{
#ifdef DEBUG
					cout<<" Residual Unit Clause: "; printClause(i, clauses, variables);cout<<" implies "<<prop<<" = TRUE\n";
#endif
					variables.find(prop)->second=TRUE;
					//cout<<"Updating Variable: "<<variables.find(prop)->first<<" - "<<variables.find(prop)->second<<"\n";
				}
				toBeDeleted.insert(i);
			}
			else if(clauses[i].size()==0)
			{
				//printClauses(clauses, variables);
				return false;
			}
		}
		updateClauses(toBeDeleted, clauses, variables);
		deleteClauses(toBeDeleted, clauses, variables);	
	}
	return true;
}


void checkPureLiterals(vector<CL>& clauses, map<string,status>& variables)
{
	//cout<<"Check Pure Literals\n";
	map<string,status> pureLiterals;
	for(int i=0; i< clauses.size(); i++)
	{
		//printClause(i, clauses, variables);
		for(int j=0; j< clauses[i].size(); j++)
		{
			string prop = (clauses[i][j][0]=='-')?clauses[i][j].substr(1, clauses[i][j].length()):clauses[i][j];
			if(variables.find(prop)->second == UNKNOWN)
			{
				status sign;
				if(clauses[i][j][0]=='-')
					sign = FALSE; 
				else
					sign = TRUE;

				map<string, status>::iterator it = pureLiterals.find(prop);
				if(it == pureLiterals.end())
				{
					//cout<<"Inserting: prop: "<<prop<<" in: ";
					//printClause(i, clauses, variables);
					pureLiterals.insert(make_pair(prop, sign));
					//toBeDeleted.insert(i);
				}
				else
				{
					if((it->second == TRUE && sign==FALSE) || (it->second == FALSE && sign==TRUE))
					{
						//cout<<"Deleting: prop: "<<prop<<" in: ";
						//printClause(i, clauses, variables);
						//toBeDeleted.erase(i);
						it->second=UNKNOWN;
					}
				}
			}
		}
	}
	for(map<string,status>::iterator it=pureLiterals.begin(); it!=pureLiterals.end(); it++)
	{
		if(it->second != UNKNOWN)
		{
#ifdef DEBUG
			if(it->second != UNKNOWN)
				cout<<"Pure Symbol on: "<<it->first<<" = "<<boolValues[it->second]<<"\n";
#endif
			variables.find(it->first)->second = it->second;
			//cout<<"Updating Variable: "<<variables.find(it->first)->first<<" - "<<variables.find(it->first)->second<<"\n";
		}
	}
	set<int> toBeDeleted;
	updateClauses(toBeDeleted, clauses, variables);
	deleteClauses(toBeDeleted, clauses, variables);	
}

bool dpll(vector<CL> clauses, map<string, status> variables)
{
	// update clauses
	set<int> toBeDeleted;
	updateClauses(toBeDeleted, clauses, variables);
	deleteClauses(toBeDeleted, clauses, variables);	

	//check if all consistent clauses
	if(!removeUnitClauses(clauses, variables))
		return false;

	checkPureLiterals(clauses, variables);

	if(!removeUnitClauses(clauses, variables))
		return false;
		
	if(clauses.size()==0)
	{
		// if any nodes are unassigned assign TRUE to them
		for(map<string, status>::iterator it=variables.begin(); it != variables.end(); it++)
		{
			if(it->second == UNKNOWN)
				it->second= TRUE;
		}
		cout<<"Nodes Searched: "<<num_nodes<<"\n";
		printClauses(clauses, variables);
		cout<<"TRUE PROPS:\n";
		for(map<string, status>::iterator it=variables.begin(); it != variables.end(); it++)
		{
			if(it->second == TRUE)
				cout<<it->first<<"; \n";
		}
		return true;
	}
	//find variable value not yet assigned
	map<string,status>::iterator it;
	for(it=variables.begin(); it!=variables.end(); it++)
	{
		if(it->second == UNKNOWN)
			break;
	}
	
	it->second = TRUE;
#ifdef DEBUG
	cout<<"Trying "<<it->first<<" = "<<boolValues[it->second]<<"\n";
#endif
	if(dpll(clauses, variables))
		return true;
	else
	{
		it->second = FALSE;
#ifdef DEBUG
		cout<<"BackTracking..Trying "<<it->first<<" = "<<boolValues[it->second]<<"\n";
#endif
		return dpll(clauses, variables);
	}
}

int main(int argc, char* argv[])
{
	vector<CL> clauses;
	map<string, status> variables;
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
		if(line != "\n" && line[0] != '#' && line != "")
		{
			istringstream iss(line);
			clauses.push_back(CL());
			while(iss)
			{
				string word; iss >> word;
				if(word != "")
				{
					clauses[clauses.size()-1].push_back(word);
					string var = (word[0]=='-')?word.substr(1, word.length()):word;
					if(variables.find(var) == variables.end())
						variables.insert(make_pair(var, UNKNOWN));
				}
			};
			//sort(clauses[clauses.size()-1].begin(), clauses[clauses.size()-1].end(), cmpClauses);
		}
	}
#ifdef DEBUG
	cout<<"Initial Clauses\n";	
	for(int i=0; i< clauses.size(); i++)
	{
		cout<<i<<": ";
		printClause(i, clauses, variables);
		cout<<"\n";
	}
#endif
	if(dpll(clauses, variables))
		cout<<"Result: SUCCESS\n";
	else
		cout<<"Result: FAILURE\n";	

	return 0;
}


