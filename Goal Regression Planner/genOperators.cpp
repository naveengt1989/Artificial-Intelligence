
///////////////////////////////////// 

// File Name :

// Purpose :

// Creation Date : 05-05-2015

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


int main()
{
	vector<string> vars; 
	vars.push_back("a");
	vars.push_back("b");
	vars.push_back("c");
	vars.push_back("d");
	vars.push_back("table");
	for(int i=0; i< vars.size()-1; i++)
	{
		for(int j=0; j< vars.size(); j++)
		{
			if(i==j)
				continue;
			// pickup(X,Y)
			cout<<"OPER pickup("<<vars[i]<<","<<vars[j]<<")\n";		
			cout<<"precond: clear("<<vars[i]<<") "<<"on("<<vars[i]<<","<<vars[j]<<") gripper_empty()\n";
			cout<<"addlist: holding("<<vars[i]<<")";
			if(vars[j]!= "table")
				cout<<" clear("<<vars[j]<<")";
			cout<<"\n";
			cout<<"dellist: clear("<<vars[i]<<") on("<<vars[i]<<","<<vars[j]<<") gripper_empty()\n";
			cout<<"conflict:";
			for(int k=0; k<vars.size(); k++)
			{
				if(k==i)
					continue;
				cout<<" on("<<vars[i]<<","<<vars[k]<<")";
				if(vars[k]!= "table")
				{
					cout<<" on("<<vars[k]<<","<<vars[i]<<")";
					cout<<" holding("<<vars[k]<<")";
				}
			}
			cout<<"\nEND\n";
			// puton(X,Y)
			cout<<"OPER puton("<<vars[i]<<","<<vars[j]<<")\n";		
			cout<<"precond:";
			if(vars[j]!= "table")
				cout<<" clear("<<vars[j]<<")";
			cout<< " holding("<<vars[i]<<")\n";
			cout<<"addlist: on("<<vars[i]<<","<<vars[j]<<") clear("<<vars[i]<<") gripper_empty()\n";
			cout<<"dellist:";
			if(vars[j]!= "table")
				cout<<" clear("<<vars[j]<<")";
			cout<<" holding("<<vars[i]<<")\n";
			cout<<"conflict:";
			for(int k=0; k<vars.size()-1; k++)
			{
				if(k==i)
					continue;
				cout<<" holding("<<vars[k]<<")";
				if(vars[j]=="table")
					continue;
				cout<<" on("<<vars[k]<<","<<vars[i]<<")";
				if(k!= j)
					cout<<" on("<<vars[k]<<","<<vars[j]<<")";
			}
			cout<<"\nEND\n";
		}
	}


	return 0;
}


