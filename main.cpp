#include "utils.h"
/*

Path Finding AI
Author - Devruth Khanna

-Agent navigates in a grid world using supervised learning.
-Modelled as a Markov Decision Process, and uses the Value Iteration Algorithm to find most optimal path from any state.

*/

void ValueIteration(){
	int num_times = 0;
	while (1){
		++num_times;
		vector<vector<double>> old_grid = CopyGrid();
			for (int row = 3; row >= 0; --row){
				for (int col = 3; col >= 0; --col){
					if (row == 3 && col == 3) continue; 
					posn cur_cell;
					cur_cell.row = row;
					cur_cell.col = col;
					string dir = FindMaximumUtilityDirection(cur_cell.row, cur_cell.col);
					double res = CalculateUtilityCell(dir, cur_cell.row, cur_cell.col);
					grid[cur_cell.row][cur_cell.col] = res;
				}
			}
		vector<vector<double>> new_grid = CopyGrid();
		if (Converge(old_grid, new_grid)) break;
	}
	PrintGrid();
	cout << endl;
	cout << "Number of Iterations: " << num_times << endl;
}





void main(int argc, char **argv) {
	ValueIteration();
}