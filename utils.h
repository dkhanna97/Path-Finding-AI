#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
using namespace std;


double grid[4][4] =
{
	{ -1, -1, -1, -1 },
	{ -1, -1, -1, -1 },
	{ -1, -70, -1, -1 },
	{ -1, -1, -1, 100 }
};

struct posn{
	int row;
	int col;
};
void PrintGrid();
double CalculateUtilityCell(string intended_dir, int row, int col);
string FindMaximumUtilityDirection(int row, int col);
vector<posn> FindAdjacentCells(int row, int col);
void PrintAdjacentCells(vector<posn> adjacent_cells);

double gamma = 0.99;
double reward_cur_state = -1;

double probability_intended_dir = 0.8;
double probability_left_lateral = 0.1;
double probability_right_lateral = 0.1;
/*
double probability_intended_dir = 0.9;
double probability_left_lateral = 0.05;
double probability_right_lateral = 0.05;*/


int north_wall = 0;
int south_wall = 3;
int west_wall = 0;
int east_wall = 3;

void PrintGrid(){
	for (int row = 0; row < 4; ++row){
		for (int col = 0; col < 4; ++col)
			cout << grid[row][col] << "       ";
		cout << endl;
	}
}


vector<vector<double>> CopyGrid(){

	vector< vector<double> > vec(4, vector<double>(4));
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			vec[i][j] = grid[i][j];
		}
	}

	return vec;
}

bool Converge(vector<vector<double>> old_grid, vector<vector<double>> new_grid){
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			if (abs(new_grid[i][j] - old_grid[i][j]) > 0.01) return false;
		}
	}
	return true;
}


bool IsGridComplete(){
	for (int row = 0; row < 4; ++row){
		for (int col = 0; col < 4; ++col){
			if (grid[row][col] == -1) return false;
		}
	}
	return true;
}


double CalculateUtilityCell(string intended_dir, int row, int col){

	bool north, south, west, east;
	north = south = west = east = true;
	if (row == north_wall) north = false;
	if (row == south_wall) south = false;
	if (col == west_wall)  west = false;
	if (col == east_wall)  east = false;

	double res, left_lateral_utility, right_lateral_utility, intended_dir_utility;

	if (intended_dir == "east"){
		if (!north){
			left_lateral_utility = probability_left_lateral * grid[row][col];

		}
		else{
			left_lateral_utility = probability_left_lateral * grid[row - 1][col];
		}
		if (!south){
			right_lateral_utility = probability_right_lateral * grid[row][col];
		}
		else{
			right_lateral_utility = probability_right_lateral * grid[row + 1][col];
		}
		if (!east) {
			intended_dir_utility = probability_intended_dir * grid[row][col]; 
		}
		else {
			intended_dir_utility = probability_intended_dir * grid[row][col + 1];
		}
	}
	else if (intended_dir == "north"){

		if (!north){
			intended_dir_utility = probability_intended_dir * grid[row][col]; 
		}
		else{
			intended_dir_utility = probability_intended_dir * grid[row - 1][col];
		}
		if (!west){
			left_lateral_utility = probability_left_lateral * grid[row][col];
		}
		else{
			left_lateral_utility = probability_left_lateral * grid[row][col - 1];
		}
		if (!east){
			right_lateral_utility = probability_left_lateral * grid[row][col]; 
		}
		else{
			right_lateral_utility = probability_left_lateral * grid[row][col + 1];
		}
	}
	else if (intended_dir == "south"){

		if (!south){
			intended_dir_utility = probability_intended_dir * grid[row][col]; 
		}
		else {
			intended_dir_utility = probability_intended_dir * grid[row + 1][col];
		}
		if (!west){
			right_lateral_utility = probability_right_lateral * grid[row][col]; 
		}
		else{
			right_lateral_utility = probability_right_lateral * grid[row][col - 1];
		}
		if (!east){
			left_lateral_utility = probability_left_lateral * grid[row][col]; 
		}
		else{
			left_lateral_utility = probability_left_lateral * grid[row][col + 1];
		}
	}
	else if (intended_dir == "west") {

		if (!west) {
			intended_dir_utility = probability_intended_dir * grid[row][col];
		}
		else {
			intended_dir_utility = probability_intended_dir * grid[row][col - 1];
		}
		if (!north){
			right_lateral_utility = probability_right_lateral * grid[row][col]; 
		}
		else{
			right_lateral_utility = probability_right_lateral * grid[row - 1][col];
		}
		if (!south){
			left_lateral_utility = probability_left_lateral * grid[row][col]; 
		}
		else{
			left_lateral_utility = probability_left_lateral * grid[row + 1][col];
		}

	}

	if (row == 2 && col == 1) res = -70 + (gamma * (intended_dir_utility + left_lateral_utility + right_lateral_utility));
	else res = -1 + (gamma * (intended_dir_utility + left_lateral_utility + right_lateral_utility));

	probability_intended_dir = 0.8;
	probability_left_lateral = 0.1;
	probability_right_lateral = 0.1;

	return res;
}

string FindMaximumUtilityDirection(int row, int col){
	string res;

	double north_utility = CalculateUtilityCell("north", row, col);
	double south_utility = CalculateUtilityCell("south", row, col);
	if (north_utility > south_utility){
		res = "north";
	}
	else{
		res = "south";
	}
	double west_utility = CalculateUtilityCell("west", row, col);
	if ((west_utility > north_utility) && (west_utility > south_utility)){
		res = "west";
	}

	double east_utility = CalculateUtilityCell("east", row, col);

	if ((east_utility > west_utility) && (east_utility > north_utility) && (east_utility > south_utility)){
		res = "east";
	}

	return res;

}


vector<posn> FindAdjacentCells(int row, int col){
	vector<posn> adjacent_cells;
	bool left, right, up, down;
	left = right = up = down = false;

	if (col > west_wall) left = true;
	if (col < east_wall) right = true;
	if (row > north_wall) up = true;
	if (row < south_wall) down = true;

	if (left){
		posn p_left;
		p_left.row = row;
		p_left.col = col - 1;
		if (!(p_left.row == 3 && p_left.col == 3)) adjacent_cells.push_back(p_left);
	}

	if (right){
		posn p_right;
		p_right.row = row;
		p_right.col = col + 1;
		if (!(p_right.row == 3 && p_right.col == 3)) adjacent_cells.push_back(p_right);
	}

	if (up){
		posn p_up;
		p_up.row = row - 1;
		p_up.col = col;
		if (!(p_up.row == 3 && p_up.col == 3)) adjacent_cells.push_back(p_up);
	}
	if (down){
		posn p_down;
		p_down.row = row + 1;
		p_down.col = col;
		if (!(p_down.row == 3 && p_down.col == 3)) adjacent_cells.push_back(p_down);
	}

	return adjacent_cells;
}


void PrintAdjacentCells(vector<posn> adjacent_cells){
	for (int i = 0; i < adjacent_cells.size(); ++i)
		cout << adjacent_cells.at(i).row << " , " << adjacent_cells.at(i).col << endl;
}

