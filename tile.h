#ifndef TILE_H
#define TILE_H
#include<iostream>

using namespace std;

class tile
{
	public:
		//setters
		void set_zero_row(int r);
		void set_zero_column(int c);
		void set_number(int n, int r, int c);
		void set_f(int f);
		void set_g(int g);
		void set_h(int h);
		void set_prev_row(int pr);
		void set_prev_column(int pc);
		
		//getters
		int get_zero_row() const;
		int get_zero_column() const;
		int get_number(int r, int c) const;
		int get_f() const;
		int get_g() const;
		int get_h() const;
		int get_prev_row() const;
		int get_prev_column() const;
		
		//print queue
		queue<int> print_list;
		
	private:
		int column, row, f_tile,g_tile,h_tile;
		int tile_board[4][4];
		int changed_number;
		int previous_row, previous_column;
		
};
#endif

void tile::set_number(int n, int r, int c)
{
	tile_board[r][c] = n;
}
int tile::get_number(int r, int c) const
{
	return tile_board[r][c];
}	
void tile::set_zero_row(int r)
{
	row = r;
}
int tile::get_zero_row() const
{
	return row;
}
void tile::set_zero_column(int c)
{
	column = c;
}
int tile::get_zero_column() const
{
	return column;
}

void tile::set_f(int f)
{
	f_tile = f;
}
void tile::set_g(int g)
{
	g_tile = g;
}
void tile::set_h(int h)
{
	h_tile = h;
}
int tile::get_f() const
{
	return f_tile;
}
int tile::get_g() const
{
	return g_tile;
}
int tile::get_h() const
{
	return h_tile;
}
/*
void tile::set_changed_number(int ch)
{
	changed_number = ch;
}
int tile::get_changed_number() const
{
	return changed_number;
}*/
void tile::set_prev_row(int pr)
{
	previous_row = pr;
}
void tile::set_prev_column(int pc)
{
	previous_column = pc;
}
int tile::get_prev_row() const
{
	return previous_row;
}
int tile::get_prev_column() const
{
	return previous_column;
}
