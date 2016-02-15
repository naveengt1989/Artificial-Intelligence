
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

#define NUM_CITIES 53
#define DEGREE_TO_RAD 3.14159/180.0
struct City
{
	string name;
	double lat;
	double lon;
	City(string nm, double lt, double ln)
	{
		name = nm;
		lat = lt;
		lon = ln;
	}
};


double calcDistance(City& x, City& y)
{
	double lat1 = x.lat;
	double lat2 = y.lat;
	double lon1 = x.lon;
	double lon2 = y.lon;

	double dlon = (lon2 - lon1)*DEGREE_TO_RAD;
	double dlat = (lat2 - lat1)*DEGREE_TO_RAD;

	double latitudeH = sin(dlat * 0.5);  
	latitudeH *= latitudeH;  
	double lontitudeH = sin(dlon * 0.5);  
	lontitudeH *= lontitudeH;  
	double tmp = cos(lat1*DEGREE_TO_RAD) * cos(lat2*DEGREE_TO_RAD);  
	return 2.0 * asin(sqrt(latitudeH + tmp*lontitudeH))*3961; 

}

double evalSolution(vector<City>& cities)
{
	double sum=0;
	for(int i=0; i< cities.size()-1; i++)
	{
		sum += calcDistance(cities[i], cities[i+1]);
	}
	sum += calcDistance(cities[0], cities[cities.size()-1]);
	return sum;
}

int genRandomNumber2(int min, int max, int& num1, int& num2)
{

	if(min==max)
		return min;
	int range = max - min;

	num1 = (rand() % range) + min;
	num2 = (rand() % range) + min;
	//cout<<"RANDOMS: "<<num1<<" "<<num2<<"\n";
	return num1;
}
double genRandomNumber(int min, int max)
{
	if(min==max)
		return min;
	int range = max - min;

	int num = (rand() % range) + min;

	return num;
}

double decisionFunc(double cur_value, double next_value, double T)
{
	double delta_D = next_value - cur_value;
	

	if(delta_D <= 0) // negative
		return 1;
	else
	{
		double eulers_coeff = -delta_D/(1*T);
		//cout<<"EULERS COEFF: "<<delta_D<<"\n";
		double accept_rate = exp(eulers_coeff);
		//cout<<"ACCEPT RATE: "<<accept_rate<<"\n";
		return accept_rate;
	}

}

void init()
{
	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);
	//srand(1);
}
// 50 - 0.001 - 0.0005  - 35  - 2000 - 1000 (4700-5100)
// 50 - 0.0001 - 0.00005  - 40  - 2000 - 1000 (4600-5000)
// 50 - 0.001 - 0.0005  - 75  - 1000 - 2000 (4600-5000)
#define INITIAL_TEMP 40
#define FINAL_TEMP 0.001
#define COOLING_RATE 0.0005
#define RESTARTS  70
#define VARIANCE_VALUE 2000
#define VARIANCE_ITER 2000
int main()
{
	init();	
	vector<City> cities;
	vector<City> bestList;
	vector<City> tempList;
	// input /////////////////////////////////
	ifstream myfile("input.txt", ifstream::in);
	if(myfile==NULL)
	{
		cout<<"ERROR !! input.txt NOT FOUND in current path\n";
		return 0;
	}
	for(int i=1; i<=NUM_CITIES; i++)
	{	
		string name; double x; double y;
		myfile >> name >> x >>y;
		cities.push_back(City(name, x, y));
	}
	//////////////////////////////////////////

	double cur_value = evalSolution(cities);
	double best_value = cur_value;
	cout<<"initial state, tour length="<<best_value<<" miles\n";
	int id1=-1;
	int id2= -1;
	double threshold;
	long long iterations=0;
	long long best_iteration=0;
	double temperature = INITIAL_TEMP;
	double restart_temp=temperature;
	int count_restarts=0;
	while(temperature > FINAL_TEMP)
	{
		iterations++;
		//select 2 random states to swap
		genRandomNumber2(1, NUM_CITIES, id1, id2);	
		swap(cities[id1], cities[id2]);
		double next_value = evalSolution(cities);
		threshold = (genRandomNumber(0, 100)/100);
#ifdef DEBUG
		cout<<"iter="<<iterations<<" len="<<cur_value<<" newlen="<<next_value<<" delta="<<(next_value-cur_value)<<" temp="<<temperature<<"\n";
#endif
		if(decisionFunc(cur_value, next_value, temperature)  >= threshold)
		{	
#ifdef DEBUG
			cout<<"Update! len="<<next_value<<"\n";
#endif
			//cout<<" cur_value: "<<cur_value<<" next_value: "<<next_value<<" best_value "<<best_value<<" ACCEPT\n";
			//accept
			cur_value = next_value;
			if(best_value > cur_value)
			{
				best_value = cur_value;
				bestList = cities;
				restart_temp= temperature;
				best_iteration = iterations;
			}
		}
		else
		{
			//cout<<" cur_value: "<<cur_value<<" next_value: "<<next_value<<" best_value "<<best_value<<" REJECT\n";
			// reverse swapping
			swap(cities[id1], cities[id2]);
		
		}
		temperature = temperature*(1-COOLING_RATE);
		int variance_value = VARIANCE_VALUE;
		if(count_restarts< RESTARTS && (iterations-best_iteration > VARIANCE_ITER) && (cur_value - best_value > variance_value) )
		{
			//cout<<"RESTARTING("<< count_restarts<<") AT: "<<best_value<<" \n";
			count_restarts++; cities=bestList;  cur_value= best_value; temperature = restart_temp;
		}
		
	}
	cout<<"\n\nBest Tour Length Found: "<<best_value<<" miles\n";
	cout<<"Total Iterations: "<<iterations<<"\n";
	cout<<"Best Tour: \n";
	for(int i=0; i< bestList.size(); i++)
	{
			cout<<bestList[i].name<<"\n";
	}

	return best_value;
}


