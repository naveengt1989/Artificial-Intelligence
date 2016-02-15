
///////////////////////////////////// 

// File Name :

// Purpose :

// Creation Date : 08-03-2015

// Last Modified : <modified_date>

// Created By : Naveen Thomas 

//////////////////////////////////////
// References:
// http://en.wikipedia.org/wiki/Reversi

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

int dx[]= {-1,0,1};
int dy[]= {-1,0,1};
#define PARITY_WEIGHT 1
#define STABILITY_WEIGHT 55 
#define POSSIBLE_MOVES_WEIGHT 7 


int N; int Board_depth; char Color;
struct Cell
{
	int x;
	int y;	
	Cell(int a, int b)
	{
		x=a; y=b;
	}
	bool operator<(const Cell& c1) const
	{
		return x>c1.x || (x==c1.x && y < c1.y);
	}
	void print()
	{
		cout<<"("<<x<<","<<y<<")\n";
	}
};

class Board
{
public:
	vector<vector<char> > cells;	
	set<Cell> black;
	set<Cell> white;
	// heuristic evaluation parameters
	int white_count;	// count of white cells
	int black_count;	// count of black cells
	int white_moves;	// count of possible moves for white cells(estimate)
	int black_moves;	// count of possible moves for black cells(estimate)
	int black_stability_factor; // calculated based on number of corner, corner-adjacent black cells. 
	int white_stability_factor; // calculated based on number of corner, corner-adjacent white cells.
	double heuristic_weight;
	Board()
	{
		white_count=0;
		black_count=0;
		white_moves=0;
		black_moves=0;
		black_stability_factor=0;
		white_stability_factor=0;
		heuristic_weight =0.0;
	}	
	void addCell(char P, Cell c);	
	void createBoard();
	void printBoard();
	void reset();
	void initHeuristics();

};

void Board::initHeuristics()
{
	white_count=0;
	black_count=0;
	white_moves=0;
	black_moves=0;
	black_stability_factor=0;
	white_stability_factor=0;
	heuristic_weight =0.0;
}

void Board::addCell(char P, Cell x)
{
		if(P == 'B')
			black.insert(x);
		else
			white.insert(x);
}


void Board::createBoard()
{
	for(int i=0; i<N; i++)
	{
		cells.push_back(vector<char>());
		for(int j=0; j<N; j++)
		{
			cells[i].push_back('.');	
		}
	}
	cells[N/2 -1][N/2 -1] = 'W'; white.insert(Cell(N/2-1,N/2-1)); 
	cells[N/2 -1][N/2] = 'B'; black.insert(Cell(N/2-1, N/2));
	cells[N/2][N/2 -1] = 'B'; black.insert(Cell(N/2, N/2-1)); 
	cells[N/2][N/2] = 'W'; white.insert(Cell(N/2, N/2));
	white_count=2; 
	black_count=2; 
}
void Board::printBoard()
{
	for(int i=0; i<N; i++)
	{
		cout<<"# ";
		for(int j=0; j<N; j++)
		{
			cout<<cells[i][j]<<" ";	
		}
		cout<<"\n";
	}
	cout<<"# Heuristic Weight(+ in favor of Black Cells, - in favor of White Cells) --> "<<heuristic_weight<<"\n";
	cout<<"# White Cells Count --> "<<white_count<<"\n";
	cout<<"# Black Cells Count --> "<<black_count<<"\n";
	cout<<"# White Cells Possible Moves --> "<<white_moves<<"\n";
	cout<<"# Black Cells Possible Moves --> "<<black_moves<<"\n";
	cout<<"# White Cells Stability Factor --> "<<white_stability_factor<<"\n";
	cout<<"# Black Cells Stability Factor --> "<<black_stability_factor<<"\n";
	cout<<"# --------------------------------\n";
}


int findFlipGain(Cell c, char P, Board& bd)
{
	char Q;
	if(P == 'B')
		Q = 'W';
	else
		Q = 'B';
	int gain =0;
	for(int i=0; i< sizeof(dx)/sizeof(int); i++)
	{
		for(int j=0; j< sizeof(dy)/sizeof(int); j++)
		{
			if(dx[i]==0 && dy[j]==0)
				continue;			
			int delta_x = c.x+dx[i]; int delta_y = c.y+dy[j]; int store_gain=0;
			while(delta_x >= 0 && delta_x < N && delta_y >= 0 && delta_y < N && bd.cells[delta_x][delta_y] == Q)
			{
				delta_x += dx[i];
				delta_y += dy[j];
				store_gain++;
			}		
			if(delta_x >= 0 && delta_x < N && delta_y >= 0 && delta_y < N && store_gain > 0 && bd.cells[delta_x][delta_y] == P)
			{	
				gain += store_gain;
				if(P == 'B')
				{
					bd.black.insert(Cell(c.x, c.y));
					//bd.black_count++;
				}
				else
				{
					bd.white.insert(Cell(c.x, c.y));
					//bd.white_count++;
				}
				bd.cells[c.x][c.y] = P; 
				delta_x = c.x+dx[i]; delta_y = c.y+dy[j];
				while(bd.cells[delta_x][delta_y] != P )
				{

					if(P == 'B')
					{
						bd.black.insert(Cell(delta_x, delta_y));
						bd.white.erase(Cell(delta_x, delta_y));
						//bd.black_count++;
						//bd.white_count--;
					}
					else
					{
						bd.white.insert(Cell(delta_x, delta_y));
						bd.black.erase(Cell(delta_x, delta_y));
						//bd.white_count++;
						//bd.black_count--;
					}
					bd.cells[delta_x][delta_y] = P;
					delta_x += dx[i];
					delta_y += dy[j];
				}
			}
		}
	}

	

	if(gain != 0) // valid board	
	{
		// find stability factor for black/white cells in the resultant board
		if( bd.cells[0][0] == 'B' || bd.cells[0][N-1] == 'B' || bd.cells[N-1][0] == 'B' || bd.cells[N-1][N-1] == 'B')// corner occupancy
			bd.black_stability_factor += 1;
		if( bd.cells[0][0] == 'W' || bd.cells[0][N-1] == 'W' || bd.cells[N-1][0] == 'W' || bd.cells[N-1][N-1] == 'W')// corner occupancy
			bd.white_stability_factor += 1;
		if( bd.cells[0][0] == '.' && (bd.cells[0][1] == 'W' || bd.cells[1][0] == 'W' || bd.cells[1][1] == 'W'))// corner adjacency occupancy
			bd.white_stability_factor += -1;
		if( bd.cells[0][0] == '.' && (bd.cells[0][1] == 'B' || bd.cells[1][0] == 'B' || bd.cells[1][1] == 'B'))// corner adjacency occupancy
			bd.black_stability_factor += -1;
		if( bd.cells[0][N-1] == '.' && (bd.cells[0][1] == 'W' || bd.cells[1][0] == 'W' || bd.cells[1][1] == 'W'))// corner adjacency occupancy
			bd.white_stability_factor += -1;
		if( bd.cells[0][N-1] == '.' && (bd.cells[0][1] == 'B' || bd.cells[1][0] == 'B' || bd.cells[1][1] == 'B'))// corner adjacency occupancy
			bd.black_stability_factor += -1;
		if( bd.cells[N-1][0] == '.' && (bd.cells[0][1] == 'W' || bd.cells[1][0] == 'W' || bd.cells[1][1] == 'W'))// corner adjacency occupancy
			bd.white_stability_factor += -1;
		if( bd.cells[N-1][0] == '.' && (bd.cells[0][1] == 'B' || bd.cells[1][0] == 'B' || bd.cells[1][1] == 'B'))// corner adjacency occupancy
			bd.black_stability_factor += -1;
		if( bd.cells[N-1][N-1] == '.' && (bd.cells[0][1] == 'W' || bd.cells[1][0] == 'W' || bd.cells[1][1] == 'W'))// corner adjacency occupancy
			bd.white_stability_factor += -1;
		if( bd.cells[N-1][N-1] == '.' && (bd.cells[0][1] == 'B' || bd.cells[1][0] == 'B' || bd.cells[1][1] == 'B'))// corner adjacency occupancy
			bd.black_stability_factor += -1;

		for(int i=0; i< N; i++)
		{
			for(int j=0; j< N; j++)
			{
				if(bd.cells[i][j]=='.')
					continue;

				if(bd.cells[i][j] == 'B')
					bd.black_count++;
				else if(bd.cells[i][j] == 'W')
					bd.white_count++;

				for(int k=0; k< sizeof(dx)/sizeof(int); k++)
				{
					for(int l=0; l< sizeof(dy)/sizeof(int); l++)
					{
						if(dx[k]==0 && dy[l]==0)
							continue;			

						if(i+dx[k] >= 0 && i+dx[k] < N && j+dy[l]>=0 && j+dy[l] < N)
						{
							if( bd.cells[i][j] == 'B'  &&  bd.cells[i+dx[k]][j+dy[l]]== '.')
								bd.white_moves++;
							if(bd.cells[i][j] == 'W'  &&  bd.cells[i+dx[k]][j+dy[l]]== '.')
								bd.black_moves++;
						}
					}
				}
			}
		}
	}
	return gain;
}
bool putPiece(char P, Cell c, Board& bd)
{
	bd.initHeuristics();
	if(bd.cells[c.x][c.y] == '.' && findFlipGain(c, P, bd) > 0)
		return true; 
	else 	
		return false;
}


// minmax function -- returnMax flag denotes whether minimum/maximum is to be returned from the function.
Board findLegalMoves(char P, Board bd, bool returnMax, int depth)
{
	char Q;
	int alpha=-9999; int beta=9999;  // used for alpha-beta pruning
	// find all ~P cells and check its neighbours 
	set<Cell>::iterator start,end;
	set<Cell> visited;
	if(P == 'B')
	{
		Q='W';
		start = bd.white.begin();
		end = bd.white.end();
	}
	else
	{
		Q='B';
		start = bd.black.begin();
		end = bd.black.end();
	}
	vector<Board> validSuccessors;
	int max =0;
	for(; start!= end; start++)
	{
		Cell current = *start; 	
		// check all its neighbours
		for(int i=0; i< sizeof(dx)/sizeof(int); i++)
		{
			for(int j=0; j< sizeof(dy)/sizeof(int); j++)
			{
				int new_x = current.x +dx[i];
				int new_y = current.y+ dy[j];
				if((i==0 && j==0) || new_x < 0 || new_x > N-1 || new_y < 0 || new_y > N-1 )
					continue;
				if( (visited.find(Cell(new_x, new_y)) == visited.end()) && (bd.cells[new_x][new_y] == '.'))
				{
					//potential next candidate for color P  
					Board new_board=bd;
					new_board.initHeuristics();
					if(findFlipGain(Cell(new_x, new_y), P, new_board) != 0)
					{	
						new_board.heuristic_weight = PARITY_WEIGHT*(new_board.black_count-new_board.white_count) + POSSIBLE_MOVES_WEIGHT*(new_board.black_moves-new_board.white_moves) + STABILITY_WEIGHT*(new_board.black_stability_factor - new_board.white_stability_factor);
						validSuccessors.push_back(new_board);
#ifdef DEBUG	
						if(Board_depth == depth)
							cout<<"Considering : ("<<new_x<<","<<new_y<<"),"<<"heursitic_value="<<new_board.heuristic_weight<<"\n";
#endif
					}
					visited.insert(Cell(new_x, new_y));
				}
			}
		}
	}
	Board best, curSolution, bestSuccessor; int max_value=-9999; int min_value=9999;
	for(int i=0; i< validSuccessors.size(); i++)
	{
		if(depth==1)
		{
			curSolution = validSuccessors[i];
			//curSolution.printBoard();
		}
		else
		{
			if(returnMax==true)
			{
				if(alpha < validSuccessors[i].heuristic_weight)
				{
					alpha = validSuccessors[i].heuristic_weight;
					curSolution = findLegalMoves(Q, validSuccessors[i],(returnMax)?false:true, depth-1);
				}
				else if(0.65*alpha < (double)validSuccessors[i].heuristic_weight)
					curSolution = findLegalMoves(Q, validSuccessors[i],(returnMax)?false:true, depth-1);
				else 
					continue;
			}
			else
			{
				if(beta > validSuccessors[i].heuristic_weight)
				{
					beta = validSuccessors[i].heuristic_weight;
					curSolution = findLegalMoves(Q, validSuccessors[i],(returnMax)?false:true, depth-1);
				}
				else if(1.35*beta > (double)validSuccessors[i].heuristic_weight)
					curSolution = findLegalMoves(Q, validSuccessors[i],(returnMax)?false:true, depth-1);
				else
					continue;
			}
		}

		if(returnMax==true && P=='B' && max_value < curSolution.heuristic_weight)
		{
			max_value =curSolution.heuristic_weight;
			best =  curSolution;
			bestSuccessor  = validSuccessors[i];
		}
		else if(returnMax==true && P=='W' && max_value < -curSolution.heuristic_weight)
		{
			max_value =-curSolution.heuristic_weight;
			best =  curSolution;
			bestSuccessor  = validSuccessors[i];
		}
		else if(returnMax==false && P=='B' && min_value > curSolution.heuristic_weight)
		{
			min_value =curSolution.heuristic_weight;
			best =  curSolution;
			bestSuccessor  = validSuccessors[i];
		}
		else if(returnMax==false && P=='W' && min_value > -curSolution.heuristic_weight)
		{
			min_value =curSolution.heuristic_weight;
			best =  curSolution;
			bestSuccessor  = validSuccessors[i];
		}
	}
	if(depth==Board_depth)
	{
		if(validSuccessors.size()==0)
			return bd; 
		//cout<<"# Best MinMax Solution\n";
		//best.printBoard();
		cout<<"# Best Successor\n";
		bestSuccessor.printBoard();
		return bestSuccessor;
	}
	else
	{
		if(validSuccessors.size()==0)
			return bd; 
		return best;
	}

}

void Board::reset()
{
	cells.clear();
	createBoard();	
	initHeuristics();
}

void findMove(Board& current, Board& next)
{
	for(int i=0; i< N; i++)
	{
		for(int j=0; j< N; j++)
		{
			if(current.cells[i][j] == '.' && next.cells[i][j] != '.')
			{
				Cell(i,j).print(); return;
			}
		}
	}
	cout<<"forfeit\n";	
}

int main(int argc, char* argv[])
{
	if(argc < 4)
	{
		cout<<"# Error!! Format is: othello <Size> <Color> <Depth>\n";
		return 0;
	}
	N = atoi(argv[1]);	
	Color = atoi(argv[2]);	
	Board_depth = atoi(argv[3]);	
	Board bd;

	string option; char P; int x; int y;
	bool flag = true;
	while(flag)
	{
		cout<<"> ";
		cin>>option;
		if(option == "init")
		{
			bd.createBoard();
		}
		else if(option == "put")
		{
			cin>>P>>x>>y;
			if(putPiece(P, Cell(x,y), bd))
				bd.printBoard();
			else
				cout<<"# ERROR Invalid Move!!\n";
		}
		else if(option == "move")
		{
			cin>>P;
			Board successor = findLegalMoves(P, bd, true, Board_depth);
			findMove(bd, successor);
			bd = successor;
		}
		else if(option == "reset")
		{
			bd.reset();
		}
		else if(option == "quit")
		{
			flag=false;
		}
		else if(option == "print")
		{
			bd.printBoard();
		}
		else
		{
			cout<<"# Invalid option Entered!!\n";
		}
	}
	return 1;
}


