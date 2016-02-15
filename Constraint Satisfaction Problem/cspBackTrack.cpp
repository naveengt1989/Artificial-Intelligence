
///////////////////////////////////// 

// File Name :

// Purpose :

// Creation Date : 15-02-2015

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
#define NUM_JOBS 8
#define NUM_PERSONS 4

enum Constants
{
	// Problem 1
	CHEF=0,GUARD=1,NURSE=2,CLERK=3,POLICE=4,TEACHER=5,ACTOR=6,BOXER=7,
	MALE=8,FEMALE=9,UNKNOWN=100,ROBERTA=0,THELMA=1,STEVE=2,PETE=3,
	// Problem 2
	ENGLISHMAN=0, SPANIARD=1, NORWEGIAN=2, UKRANIAN=3, JAPANESE=4,
	DOG=0, FOX=1,HORSE=2,SNAIL=3,ZEBRA=4,
	RED=0,IVORY=1,GREEN=2,BLUE=3,YELLOW=4,
	HERSHEY=0,KITKATS=1,SMARTIES=2,SNICKERS=3,MILKYWAY=4,
	MILK=0,COFFEE=1,TEA=2,ORANGE_JUICE=3,WATER=4,
	PETS=0, HOUSES=1, HOUSE_NUMBERS=2, EATERIES=3, DRINKS=4,
	ONE=0, TWO=1, THREE=2, FOUR=3, FIVE=4,
};

string person_names[] = {"ROBERTA", "THELMA", "STEVE", "PETE"};
string job_names[] = {"CHEF", "GUARD", "NURSE", "CLERK", "POLICE", "TEACHER", "ACTOR", "BOXER"};
string pet_names[] ={"DOG", "FOX", "HORSE", "SNAIL", "ZEBRA"};
string house_names[]={"RED","IVORY", "GREEN", "BLUE", "YELLOW"};
string eatery_names[] ={"HERSHEY", "KITKAT", "SMARTIES", "SNICKERS", "MILKYWAY"};
string drink_names[]={"MILK","COFFEE","TEA","ORANGE_JUICE", "WATER"};
string country_names[]={"ENGLISHMAN", "SPANIARD","NORWEGIAN","UKRANIAN","JAPANESE"};

struct Job
{
	Constants job;
	Constants gender;
	Job(Constants x, Constants y)
	{
		job=x;
		gender=y;
	}
	Job()
	{
		job = UNKNOWN;	
		gender= UNKNOWN;
	}
	bool operator<(const Job& jd) const
	{
		return this->job< jd.job;
	}
};


vector<Job> jobs(NUM_JOBS);
set<Job> domain[NUM_PERSONS];
struct State
{
	pair<Job, Job> Person[NUM_PERSONS];
};

void addConstraints()
{
	jobs[NURSE] = Job(NURSE, MALE);	
	jobs[GUARD] = Job(GUARD, UNKNOWN);	
	jobs[CHEF] = Job(CHEF, FEMALE);	
	jobs[CLERK] = Job(CLERK, MALE);	
	jobs[POLICE] = Job(POLICE, UNKNOWN);	
	jobs[TEACHER] = Job(TEACHER, UNKNOWN);	
	jobs[ACTOR] = Job(ACTOR, MALE);	
	jobs[BOXER] = Job(BOXER, UNKNOWN);	
	
	domain[ROBERTA].insert(jobs[GUARD]);	
	domain[ROBERTA].insert(jobs[CLERK]);	
	domain[ROBERTA].insert(jobs[TEACHER]);	
	domain[ROBERTA].insert(jobs[ACTOR]);	

	domain[THELMA].insert(jobs.begin(), jobs.end());	

	domain[STEVE].insert(jobs.begin(), jobs.end());	
	
	domain[PETE].insert(jobs[GUARD]);	
	domain[PETE].insert(jobs[CHEF]);	
	domain[PETE].insert(jobs[CLERK]);	
	domain[PETE].insert(jobs[ACTOR]);	
	domain[PETE].insert(jobs[BOXER]);	
}

int iterations=0;
bool checkConsistencyJobs(State& st)
{
	iterations++;
	//remove already alloZEBRAed states	
	for(int i=0; i< NUM_PERSONS; i++)
	{
		if (st.Person[i].first.gender != UNKNOWN && st.Person[i].second.gender != UNKNOWN && st.Person[i].first.gender != st.Person[i].second.gender)
			return false;

		if ((st.Person[i].first.job == CHEF && st.Person[i].second.job == POLICE) || (st.Person[i].first.job == POLICE && st.Person[i].second.job == CHEF) )
			return false;
	}

}

void printSolution(const State& st)
{
	cout<<"PROBLEM 1 Solution: <Person_Name> - <Job1>, <Job2>\n";
	for(int i=0; i< NUM_PERSONS; i++)
	{
		cout<<person_names[i]<<" - "<<job_names[st.Person[i].first.job]<<", "<<job_names[st.Person[i].second.job]<<"\n";
	}
	cout<<"Total Iterations: "<<iterations<<"\n";
	cout<<"----------------------------------\n";
}

bool searchSolution(State& st, int personId)
{
	// MRV Heuristics
	int min_valid_count=100; int min_i=-1; 
	for(int i=0; i< NUM_PERSONS; i++)
	{		
		int valid_count=0; int reqd_count;
		if(st.Person[i].first.job == UNKNOWN && st.Person[i].second.job == UNKNOWN )
			reqd_count=2;
		else if(st.Person[i].second.job == UNKNOWN)
			reqd_count=1;
		else
			continue;

		for(set<Job>::iterator j= domain[i].begin(); j!= domain[i].end(); j++)
		{	
			if(reqd_count==2)
				st.Person[i].first = *j;
			else
				st.Person[i].second = *j;

			if(checkConsistencyJobs(st) == true)
				valid_count++;
		}
		if(reqd_count==2)
			st.Person[i].first.job = UNKNOWN;
		else
			st.Person[i].second.job = UNKNOWN;

		if(valid_count < reqd_count)
			return false;
#ifndef MRV
		else
		{min_i=i; break;}	
#endif
		if(min_valid_count > (valid_count-reqd_count))
		{min_valid_count=valid_count-reqd_count; min_i =i; }
	}
	/////////////////////////////////////////////////////
	if(min_i == -1)
	{
		printSolution(st); return true;
	}
	Job prev_value;
	for(set<Job>::iterator i= domain[min_i].begin(); i!= domain[min_i].end(); )
	{
		if(i != domain[min_i].begin())
			i= domain[min_i].upper_bound(prev_value);
		prev_value=*i;

		if(st.Person[min_i].first.job == UNKNOWN)
			st.Person[min_i].first = *i;
		else
			st.Person[min_i].second = *i;

		bool resetFlag[NUM_PERSONS];
		memset(resetFlag, 0, sizeof(resetFlag));
		for(int k=0; k< NUM_PERSONS; k++)
		{
			if(domain[k].find(*i) != domain[k].end())	
			{
				domain[k].erase(*i);	
				resetFlag[k]=true;
			}
		}	
		if(checkConsistencyJobs(st) == true)
			if(searchSolution(st, personId+1))
				return true;
		for(int k=0; k< NUM_PERSONS; k++)
		{
			if(resetFlag[k]== true)
			{
				domain[k].insert(*i);
			}
		}
		if(st.Person[min_i].second.job == UNKNOWN)
			st.Person[min_i].first.job = UNKNOWN;
		else
			st.Person[min_i].second.job = UNKNOWN;
	}
} 

#define NUM_COUNTRIES 5
#define NUM_ASSIGNMENTS 5
 
struct Node
{
	Constants values[NUM_COUNTRIES][NUM_ASSIGNMENTS];
	set<Constants> domains[NUM_ASSIGNMENTS];
};

Constants Pets[] = {DOG, FOX, HORSE, SNAIL, ZEBRA};
Constants Houses[] = {RED, IVORY, GREEN, BLUE, YELLOW};
Constants House_Numbers[] = {ONE,TWO,THREE,FOUR,FIVE};
Constants Eateries[] = {HERSHEY, KITKATS, SMARTIES, SNICKERS, MILKYWAY};
Constants Drinks[] = {MILK, COFFEE, TEA, ORANGE_JUICE, WATER};

void addConstraints2(Node& initial)
{
	for(int i=0; i< NUM_COUNTRIES; i++)
	{
		for(int j=0; j< NUM_ASSIGNMENTS; j++)
		{
			initial.values[i][j] = UNKNOWN;
		}
		initial.domains[PETS].insert(Pets[i]);
		initial.domains[HOUSES].insert(Houses[i]);
		initial.domains[HOUSE_NUMBERS].insert(House_Numbers[i]);
		initial.domains[EATERIES].insert(Eateries[i]);
		initial.domains[DRINKS].insert(Drinks[i]);
	}
	initial.values[ENGLISHMAN][HOUSES]=RED;
	initial.domains[HOUSES].erase(RED);
	initial.values[SPANIARD][PETS]=DOG;
	initial.domains[PETS].erase(DOG);
	initial.values[NORWEGIAN][HOUSE_NUMBERS]=ONE;
	initial.domains[HOUSE_NUMBERS].erase(ONE);
	initial.values[UKRANIAN][DRINKS]=TEA;
	initial.domains[DRINKS].erase(TEA);
	initial.values[JAPANESE][EATERIES]=MILKYWAY;
	initial.domains[EATERIES].erase(MILKYWAY);
	
}

bool check_consistencyhouses(Node& initial)
{
	iterations++;
	Constants green_house_no= UNKNOWN;
	Constants ivory_house_no=UNKNOWN;
	Constants hershey_house_no=UNKNOWN;
	Constants fox_house_no=UNKNOWN;
	Constants kitkat_house_no=UNKNOWN;
	Constants horse_house_no=UNKNOWN;
	for(int i=0; i< NUM_COUNTRIES; i++)
	{
		if(initial.values[i][HOUSES] == GREEN)
		{
			if(initial.values[i][HOUSE_NUMBERS] == ONE)
				return false;		
			else if(initial.values[i][HOUSE_NUMBERS] != UNKNOWN)	
				green_house_no = initial.values[i][HOUSE_NUMBERS];
		}
		if(initial.values[i][HOUSES] == IVORY)
		{
			if(initial.values[i][HOUSE_NUMBERS] == ONE)
				return false;		
			else if(initial.values[i][HOUSE_NUMBERS] != UNKNOWN)	
				ivory_house_no = initial.values[i][HOUSE_NUMBERS];
		}

		if(ivory_house_no != UNKNOWN && green_house_no != UNKNOWN && (green_house_no != ivory_house_no +1))
			return false;
		

		if(initial.values[i][HOUSES] == BLUE && initial.values[i][HOUSE_NUMBERS] != UNKNOWN && initial.values[i][HOUSE_NUMBERS] != TWO)
			return false;	

		if(initial.values[i][EATERIES] == SMARTIES && initial.values[i][PETS] != UNKNOWN && initial.values[i][PETS] != SNAIL)
			return false;
	
		if(initial.values[i][PETS] == SNAIL && initial.values[i][EATERIES] != UNKNOWN && initial.values[i][EATERIES] != SMARTIES)
			return false;

		if(initial.values[i][EATERIES] == SNICKERS && initial.values[i][DRINKS] != UNKNOWN && initial.values[i][DRINKS] != ORANGE_JUICE)
			return false;
	
		if(initial.values[i][DRINKS] == ORANGE_JUICE && initial.values[i][EATERIES] != UNKNOWN && initial.values[i][EATERIES] != SNICKERS)
			return false;

		if(initial.values[i][DRINKS] == COFFEE && initial.values[i][HOUSES] != UNKNOWN && initial.values[i][HOUSES] != GREEN)
			return false;
	
		if(initial.values[i][HOUSES] == GREEN && initial.values[i][DRINKS] != UNKNOWN && initial.values[i][DRINKS] != COFFEE)
			return false;

		if(initial.values[i][HOUSE_NUMBERS] == THREE && initial.values[i][DRINKS] != UNKNOWN && initial.values[i][DRINKS] != MILK)
			return false;
	
		if(initial.values[i][DRINKS] == MILK && initial.values[i][HOUSE_NUMBERS] != UNKNOWN && initial.values[i][HOUSE_NUMBERS] != THREE)
			return false;

		if(initial.values[i][EATERIES] == KITKATS && initial.values[i][HOUSES] != UNKNOWN && initial.values[i][HOUSES] != YELLOW)
			return false;
	
		if(initial.values[i][HOUSES] == YELLOW && initial.values[i][EATERIES] != UNKNOWN && initial.values[i][EATERIES] != KITKATS)
			return false;

		
		if(initial.values[i][EATERIES] == KITKATS && initial.values[i][HOUSE_NUMBERS] != UNKNOWN)
			kitkat_house_no = initial.values[i][HOUSE_NUMBERS];

		if(initial.values[i][PETS] == HORSE && initial.values[i][HOUSE_NUMBERS] != UNKNOWN)
			horse_house_no = initial.values[i][HOUSE_NUMBERS];

		if(horse_house_no != UNKNOWN && kitkat_house_no != UNKNOWN && abs(kitkat_house_no-horse_house_no)!=1)
			return false;

		if(initial.values[i][EATERIES] == HERSHEY && initial.values[i][HOUSE_NUMBERS] != UNKNOWN)
			hershey_house_no = initial.values[i][HOUSE_NUMBERS];

		if(initial.values[i][PETS] == FOX && initial.values[i][HOUSE_NUMBERS] != UNKNOWN)
			fox_house_no = initial.values[i][HOUSE_NUMBERS];

		if(fox_house_no != UNKNOWN && hershey_house_no != UNKNOWN && abs(fox_house_no-hershey_house_no)!=1)
			return false;
	}
	return true;

}

void printSolution2(Node& st)
{
	cout<<"PROBLEM 2 Solution: Country_Name - Pet Name - House Name - House Position - Snacks - Drinks\n";
	for(int i=0; i< NUM_COUNTRIES; i++)
	{	
		cout<<country_names[i]<<" - "<<pet_names[st.values[i][0]]<<" - "<<house_names[st.values[i][1]]<<" - "<<st.values[i][2]+1<<" - "<<eatery_names[st.values[i][3]]<<" - "<<drink_names[st.values[i][4]]<<"\n";
	}
	cout<<"Total Iterations: "<<iterations<<"\n";
	cout<<"------------------------------------------------\n";
}
bool searchSolution2(Node& st)
{
	if(st.domains[PETS].size()==0 && st.domains[HOUSES].size()==0 && st.domains[HOUSE_NUMBERS].size()==0 && st.domains[EATERIES].size()==0 && st.domains[DRINKS].size()==0)
	{
		printSolution2(st);
		return true;
	}

	// MRV Heuristics
	int min_valid_count=100; int min_i=-1; int min_j=-1;
	for(int i=0; i< NUM_COUNTRIES; i++)
	{
		for(int j=0; j< NUM_ASSIGNMENTS; j++)
		{	
			if(st.values[i][j]== UNKNOWN)
			{
#ifdef MRV
				int valid_count=0;
				for(set<Constants>::iterator k=st.domains[j].begin(); k != st.domains[j].end(); k++)
				{
					st.values[i][j] = *k;
					if(check_consistencyhouses(st)==true)
						valid_count++;
				}
				if(valid_count ==0)
				{
					st.values[i][j] = UNKNOWN;
					return false;
				}
				else if(valid_count< min_valid_count)
				{
					min_valid_count= valid_count; min_i=i; min_j =j;
				}
				st.values[i][j] = UNKNOWN;
#else
				int valid_count=0;
				for(set<Constants>::iterator k=st.domains[j].begin(); k != st.domains[j].end(); k++)
				{
					st.values[i][j] = *k;
					if(check_consistencyhouses(st)==true)
					{	valid_count++; break;}
				}
				if(valid_count ==0)
				{
					st.values[i][j] = UNKNOWN;
					return false;
				}
				else
				{
					min_i=i; min_j =j; st.values[i][j] = UNKNOWN;break;
				}
				
#endif
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	
	for(set<Constants>::iterator i=st.domains[min_j].begin(); i != st.domains[min_j].end(); i++)
	{
		st.values[min_i][min_j] = *i;
		st.domains[min_j].erase(*i);
		if(check_consistencyhouses(st)==true)
		{
			if(searchSolution2(st))
				return true;	
		}
		st.values[min_i][min_j] = UNKNOWN;
		st.domains[min_j].insert(*i);
	}	
	return false;
}

int main()
{
#ifdef PROB1
	// Problem 1 ///////////////////
	State initial;
	addConstraints();
	searchSolution(initial, ROBERTA);
	/////////////////////////////////
#endif
#ifdef PROB2
	// Problem 2 ///////////////////
	Node initial2;
	addConstraints2(initial2);
	searchSolution2(initial2);
	/////////////////////////////////
#endif
}


