//Answers to questions are in the readme file

#include<iostream>
#include<fstream>
#include<cmath>
#include<queue>
#include<stack>
#include<vector>
#include<algorithm>
#include "tile.h"

using namespace std;

//function used to calculate the manhattan distance given the tile piece
int manhattan_calc(tile t);
int misplaced_calc(tile t);
void find_neighbors(int, int, vector<int> &, vector<int> &);

//overloading greater operator for the priority queue to use
bool operator>(const tile & t1,const tile & t2)
{ 
	return ( t1.get_f() > t2.get_f());
}

////////START OF MAIN FUNCTION/////////////////////////////////////////////////////
int main(int argc, char *argv[])
{	
	int h_value=0, g_value=0, f_value=0, num_moves=0;
	int zero_row=0, zero_column=0, row_changing_to, column_changing_to;
	bool found_result = false;
	
	stack<tile> solutions_near_zero;
	tile lowest_f_tile;
	vector<int> neighbor_row, neighbor_column;
	
	priority_queue< tile, vector<tile>, greater<tile> > unexplored_tiles;

	//////////getting the initial input//////////////////////
	int file_num;
	ifstream fin;
	fin.open(argv[1]);
	tile init_tile; //used just for the initial board given
	if(fin) //if the file is there and was opened
	{
		//board is in following coordinate configuration
		//(0,1) (1,2) (2,3)
		//(0,0) (1,1) (2,2) (3,3)
		//(1,0) (2,1) (3,2)
		
		//initializing first row of board
		int k=1;
		for(int i=0;i<3;i++)
		{
			fin>>file_num;
			if (file_num == 0)
			{
				init_tile.set_zero_row(i);
				init_tile.set_zero_column(k);
			}
			init_tile.set_number(file_num,i,k++);
		}
		//initializing second row of board
		k=0;
		for(int i=0;i<4;i++)
		{
			fin>>file_num;
			if (file_num == 0)
			{
				init_tile.set_zero_row(i);
				init_tile.set_zero_column(k);
			}
			init_tile.set_number(file_num,i,k++);
		}
		//initializing third row of board
		k=0;
		for(int i=1;i<4;i++)
		{
			fin>>file_num;
			if (file_num == 0)
			{
				init_tile.set_zero_row(i);
				init_tile.set_zero_column(k);
			}
			init_tile.set_number(file_num,i,k++);
		}
		
		//manhattan distance for the intial board//
		h_value = manhattan_calc(init_tile);
		
		//if the puzzle is already solved, then stop here
		if (h_value == 0)
		{
			cout<<endl<<"Puzzle already solved."<<endl;
			cout<<"Manhattan Distance: "<<endl;
			cout<<"Sequence of tiles to be moved: ";
			cout<<endl<<"Number of moves: "<<0<<endl<<endl;
			cout<<"Number of Misplaced Tiles: "<<endl;
			cout<<"Sequence of tiles to be moved: ";
			cout<<endl<<"Number of moves: "<<0<<endl<<endl;
			return 0;
		}
		f_value = h_value;
		init_tile.set_f(f_value);
		init_tile.set_g(0);
		init_tile.set_h(h_value);
		init_tile.set_prev_row(-1);
		init_tile.set_prev_column(-1);
		
		unexplored_tiles.push(init_tile);
	}
	
	else //if the file could not open
	{
		cout<<endl<<"Could not find file. Program aborted."<<endl;
		return 0;
	}
	
	////////calculating the manhattan heuristic for the remaining tiles and moves///////////
	while(!unexplored_tiles.empty())
	{		
		zero_row = unexplored_tiles.top().get_zero_row();
		zero_column = unexplored_tiles.top().get_zero_column();
		g_value = unexplored_tiles.top().get_g()+1; //used later to calculate the manhattan distance
		
		//looking at all possible combinations next to the 0 on the board		
		find_neighbors(zero_row, zero_column, neighbor_row, neighbor_column);
		while(!neighbor_row.empty())
		{
			lowest_f_tile = unexplored_tiles.top();
			//keeping track of where zero was before so that it doesn't go in a loop in the same place
			lowest_f_tile.set_prev_row(zero_row);
			lowest_f_tile.set_prev_column(zero_column);
			
			row_changing_to = neighbor_row.back();
			column_changing_to = neighbor_column.back();
			neighbor_row.pop_back();
			neighbor_column.pop_back();
			
			if(!(unexplored_tiles.top().get_prev_row()==row_changing_to && unexplored_tiles.top().get_prev_column() == column_changing_to))
			{
				//storing the number of tile changed
				lowest_f_tile.print_list.push(unexplored_tiles.top().get_number(row_changing_to,column_changing_to));
						
				//swapping the values in the areas
				lowest_f_tile.set_number(unexplored_tiles.top().get_number(row_changing_to,column_changing_to), zero_row,zero_column);
				lowest_f_tile.set_number(0, row_changing_to,column_changing_to);
						
				//making sure that the row and column are noted of the zero
				lowest_f_tile.set_zero_row(row_changing_to);
				lowest_f_tile.set_zero_column(column_changing_to);
						
				h_value = manhattan_calc(lowest_f_tile);
				f_value = g_value + h_value;
				lowest_f_tile.set_f(f_value);
				lowest_f_tile.set_g(g_value);
				lowest_f_tile.set_h(h_value);
						
				//don't want to push into priority queue until later, so put in stack for now
				solutions_near_zero.push(lowest_f_tile);
						
				//if it solves the problem with moving this tile
				if (h_value == 0)
				{	
					while(!unexplored_tiles.empty())
					{
						unexplored_tiles.pop();
					}
					while(!solutions_near_zero.empty())
					{
						solutions_near_zero.pop();
					}
					found_result = true; 
					break; //used to break out of row while-loop
				}
					
			}
		} //while-loop for exploring the neighboring tiles	
		if(found_result) //used to break out of while loop
		{
			found_result = false;
			break;	
		}
		
		unexplored_tiles.pop();
		
		while(!solutions_near_zero.empty())
		{
			unexplored_tiles.push(solutions_near_zero.top());
			solutions_near_zero.pop();
		}
	} //end of while loop
	
	//printing output for Manhattan distance
	cout<<endl<<"Manhattan Distance:"<<endl;
	cout<<"Sequence of tiles to be moved: ";
	num_moves = lowest_f_tile.print_list.size();
	while(!lowest_f_tile.print_list.empty())
	{
		cout<<lowest_f_tile.print_list.front()<<" ";
		lowest_f_tile.print_list.pop();
	}
	cout<<endl<<"Number of moves: "<<num_moves<<endl<<endl;


	///////////////////////NUMBER OF TILES MISPLACED HEURISITC/////////////////////////////////////////////
	h_value = misplaced_calc(init_tile);
	f_value = h_value;
	init_tile.set_f(f_value);
	init_tile.set_h(h_value);
	unexplored_tiles.push(init_tile);
	
	while(!unexplored_tiles.empty())
	{		
		zero_row = unexplored_tiles.top().get_zero_row();
		zero_column = unexplored_tiles.top().get_zero_column();
		g_value = unexplored_tiles.top().get_g()+1; //used later to calculate the manhattan distance
		
		//looking at all possible combinations next to the 0 on the board
		find_neighbors(zero_row, zero_column, neighbor_row, neighbor_column);
		while(!neighbor_row.empty())
		{
			lowest_f_tile = unexplored_tiles.top();
			
			//keeping track of where zero was before so that it doesn't go in a loop in the same place
			lowest_f_tile.set_prev_row(zero_row);
			lowest_f_tile.set_prev_column(zero_column);
			
			row_changing_to = neighbor_row.back();
			column_changing_to = neighbor_column.back();
			neighbor_row.pop_back();
			neighbor_column.pop_back();
			
			if(!(unexplored_tiles.top().get_prev_row()==row_changing_to && unexplored_tiles.top().get_prev_column() == column_changing_to))
			{
				//storing the number of tile changed
				lowest_f_tile.print_list.push(unexplored_tiles.top().get_number(row_changing_to,column_changing_to));
						
				//swapping the values in the areas
				lowest_f_tile.set_number(unexplored_tiles.top().get_number(row_changing_to,column_changing_to), zero_row,zero_column);
				lowest_f_tile.set_number(0, row_changing_to,column_changing_to);
						
				//making sure that the row and column are noted of the zero
				lowest_f_tile.set_zero_row(row_changing_to);
				lowest_f_tile.set_zero_column(column_changing_to);

				h_value = misplaced_calc(lowest_f_tile);
				f_value = g_value + h_value;
				lowest_f_tile.set_f(f_value);
				lowest_f_tile.set_g(g_value);
				lowest_f_tile.set_h(h_value);
						
				//don't want to push into priority queue until later, so put in stack for now
				solutions_near_zero.push(lowest_f_tile);
						
				//if it solves the problem with moving this tile
				if (h_value == 0)
				{	
					found_result = true; 
					break; //used to break out of row while-loop
				}
					
			}
		} //while-loop for exploring the neighboring tiles	
		if(found_result) //used to break out of while loop
		{
			found_result = false;
			break;	
		}
		
		unexplored_tiles.pop();
		
		//finally putting in all of the neighbors into the priority queue
		while(!solutions_near_zero.empty())
		{
			unexplored_tiles.push(solutions_near_zero.top());
			solutions_near_zero.pop();
		}
	} //end of while loop
	
	//printing output for misplaced tiles distance
	cout<<"Number of Misplaced Tiles:"<<endl;
	cout<<"Sequence of tiles to be moved: ";
	num_moves = lowest_f_tile.print_list.size();
	while(!lowest_f_tile.print_list.empty())
	{
		cout<<lowest_f_tile.print_list.front()<<" ";
		lowest_f_tile.print_list.pop();
	}
	cout<<endl<<"Number of moves: "<<num_moves<<endl<<endl;


	return 0;
}
/////////////////////END OF MAIN FUNCTION/////////////////////////


int manhattan_calc(tile t)
{
	int row_distance=0, column_distance=0;
	int h=0;
	for (int a=0;a<4;a++) //row
	{
		for(int b=0;b<4;b++) //column
		{
			//these are the cases that don't exist on the board so don't iterate through them
			if((a==0 && b==2) || (a==0 && b==3) || (a==1 && b==3) || (a==2 && b==0) || (a==3 && b==0) || (a==3 && b==1))
			{
				continue;
			}
			
			//SWITCH STATEMENT USED INSTEAD FOR FIGURING OUT MANHATTAN DISTANCE			
			switch(t.get_number(a,b))
			{
				case 1: 
					//remember calculating on the hex board, not a normal square board
					row_distance = a;
					column_distance = abs(b-1);
					//special case, where we know it is always going to be two away according to geometry of board
					if(a==1 && b==0)
						row_distance = 2;
					break;
				case 2:
					row_distance = abs(a-1);
					column_distance = abs(b-2);
					if(a==2 && b==1)
						row_distance = 2;
					break;
				case 3:
					row_distance = abs(a-2);
					column_distance = abs(b-3);
					if(a==3 && b==2)
						row_distance = 2;
					break;
				case 4:
					row_distance = a;
					column_distance = b;
					break;
				case 5:
					row_distance = abs(a-1);
					column_distance = abs(b-1);
					break;
				case 6:
					row_distance = abs(a-2);
					column_distance = abs(b-2);
					break;
				case 7:
					row_distance = abs(a-3);
					column_distance = abs(b-3);
					break;
				case 8:
					row_distance = abs(a-1);
					column_distance = b;
					if(a==0 && b==1)
						row_distance = 2;
					break;
				case 9:
					row_distance = abs(a-2);
					column_distance = abs(b-1);
					if(a==1 && b==2)
						row_distance = 2;
					break;
				case 0: 
					row_distance = 0;
					column_distance = 0;
					break;
			}
			h += max(row_distance,column_distance);
		}
	}
	return h;
}

int misplaced_calc(tile t)
{
	int hm=0;
	for (int a=0;a<4;a++) //row
	{
		for(int b=0;b<4;b++) //column
		{
			//these are the cases that don't exist on the board so don't iterate through them
			if((a==0 && b==2) || (a==0 && b==3) || (a==1 && b==3) || (a==2 && b==0) || (a==3 && b==0) || (a==3 && b==1))
			{
				continue;
			}
			
			//SWITCH STATEMENT USED INSTEAD FOR FIGURING OUT MISPLACED TILES			
			switch(t.get_number(a,b))
			{
				case 1: 
					// according to hex board coordinates
					if(!(a==0 && b==1))
						hm+=1;
					break;
				case 2:
					if(!(a==1 && b==2))
						hm+=1;
					break;
				case 3:
					if(!(a==2 && b==3))
						hm+=1;
					break;
				case 4:
					if(!(a==0 && b==0))
						hm+=1;
					break;
				case 5:
					if(!(a==1 && b==1))
						hm+=1;
					break;
				case 6:
					if(!(a==2 && b==2))
						hm+=1;
					break;
				case 7:
					if(!(a==3 && b==3))
						hm+=1;
					break;
				case 8:
					if(!(a==1 && b==0))
						hm+=1;
					break;
				case 9:
					if(!(a==2 && b==1))
						hm+=1;
					break;
				case 0: 
					break;
			}
		}
	}
	return hm;
}

void find_neighbors(int r, int c, vector<int>& new_row, vector<int>& new_column)
{
	//looking at the places around the zero to find the valid neighbors
	new_row.erase(new_row.begin(), new_row.end());
	new_column.erase(new_column.begin(), new_column.end());
	switch(r)
	{
		case 0:
			if(c==0)
			{
				new_row.push_back(0);
				new_row.push_back(1);
				new_row.push_back(1);
				
				new_column.push_back(1);
				new_column.push_back(1);
				new_column.push_back(0);
			}
			else if(c==1)
			{
				new_row.push_back(0);
				new_row.push_back(1);
				new_row.push_back(1);
				
				new_column.push_back(0);
				new_column.push_back(1);
				new_column.push_back(2);
			}
			break;
		case 1:
			if(c==0)
			{
				new_row.push_back(0);
				new_row.push_back(1);
				new_row.push_back(2);
				
				new_column.push_back(0);
				new_column.push_back(1);
				new_column.push_back(1);
			}
			else if(c==1)
			{
				new_row.push_back(0);
				new_row.push_back(0);
				new_row.push_back(1);
				new_row.push_back(2);
				new_row.push_back(2);
				new_row.push_back(1);
				
				new_column.push_back(0);
				new_column.push_back(1);
				new_column.push_back(2);
				new_column.push_back(2);
				new_column.push_back(1);
				new_column.push_back(0);
			}
			else if(c==2)
			{
				new_row.push_back(0);
				new_row.push_back(1);
				new_row.push_back(2);
				new_row.push_back(2);
				
				new_column.push_back(1);
				new_column.push_back(1);
				new_column.push_back(2);
				new_column.push_back(3);
			}
			break;
		case 2:
			if (c==1)
			{
				new_row.push_back(1);
				new_row.push_back(1);
				new_row.push_back(2);
				new_row.push_back(3);
				
				new_column.push_back(0);
				new_column.push_back(1);
				new_column.push_back(2);
				new_column.push_back(2);
			}
			else if(c==2)
			{
				new_row.push_back(1);
				new_row.push_back(1);
				new_row.push_back(2);
				new_row.push_back(3);
				new_row.push_back(3);
				new_row.push_back(2);
				
				new_column.push_back(1);
				new_column.push_back(2);
				new_column.push_back(3);
				new_column.push_back(3);
				new_column.push_back(2);
				new_column.push_back(1);
			}
			else if(c==3)
			{
				new_row.push_back(1);
				new_row.push_back(2);
				new_row.push_back(3);
				
				new_column.push_back(2);
				new_column.push_back(2);
				new_column.push_back(3);
			}
			break;
		case 3:
			if(c==2)
			{
				new_row.push_back(2);
				new_row.push_back(2);
				new_row.push_back(3);
				
				new_column.push_back(1);
				new_column.push_back(2);
				new_column.push_back(3);
			}
			else if(c==3)
			{
				new_row.push_back(2);
				new_row.push_back(2);
				new_row.push_back(3);
				
				new_column.push_back(3);
				new_column.push_back(2);
				new_column.push_back(2);
			}
			break;
	}
	return;

}
