#include <iostream>
#include <string>
#include <fstream>
#include <random> 
#include <time.h>
//#include "Stack.h"
#include "Stack.cpp"  // BECAUSE OF TRANSLATION UNIT ERRORS CAUSED BY TEMPLATING THE STACK CLASS

using namespace std;

// MEHMET ENES BATTAL - HW1

// V2- MAZE IS GENERATED, NEED TO FIND A PATH

struct mazeCell
{
	// coordinates
	unsigned int row;
	unsigned int col;

	// walls and status
	unsigned int l; // left
	unsigned int r; // right
	unsigned int u; // up
	unsigned int d; // down
	bool visited;

	// default constructor
	mazeCell():row(0), col(0), l(1), r(1), u(1), d(1), visited(false){}
	// constructor
	mazeCell(unsigned int a, unsigned int b, unsigned int c, unsigned int d, 
		unsigned int e, unsigned int f, bool vs):row(a), col(b), l(c), r(d), u(e), d(f), visited(vs){}
};

int random_range(const int & min, const int & max) 
{     
	static mt19937 generator(time(0));     
	uniform_int_distribution<int> distribution(min, max);     
	return distribution(generator); 
}

mazeCell** initializeMaze(int m, int n)
{
	mazeCell** head = new mazeCell* [m];

	for(int i = 0; i < m; i++)
	{
		head[i] = new mazeCell[n];

		for(int j = 0; j < n; j++)
		{
			head[i][j].row = i; // so bottom left corner is (0,0)
			head[i][j].col = j;
		}
	}

	return head;
}

void deleteMaze(mazeCell** head, int m)
{
	for (int i = 0; i < m; i++) 
	{
		delete [] head[i];
	}
	delete [] head;
}

bool allVisited(mazeCell** head, int m, int n)
{
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(!head[i][j].visited)
				return false;
		}
	}
	return true;
}

bool checkNeighbour(mazeCell** maze, mazeCell current, int m, int n)
{
	int row = current.row;
	int col = current.col;

	if(row == 0 && col == 0)
		return maze[row+1][col].visited && maze[row][col+1].visited;

	else if(row == 0 && col == n-1)
		return maze[row][col-1].visited && maze[row+1][col].visited;

	else if(row == m-1 && col == n-1)
		return maze[row][col-1].visited && maze[row-1][col].visited;

	else if(row == m-1 && col == 0)
		return maze[row-1][col].visited && maze[row][col+1].visited;

	else if(row == 0)
		return maze[row][col-1].visited && maze[row+1][col].visited && maze[row][col+1].visited;

	else if(col == 0)
		return maze[row-1][col].visited && maze[row][col+1].visited && maze[row+1][col].visited;

	else if(row == m-1)
		return maze[row][col-1].visited && maze[row-1][col].visited && maze[row][col+1].visited;

	else if(col == n-1)
		return maze[row-1][col].visited && maze[row][col-1].visited && maze[row+1][col].visited;

	else
		return maze[row][col-1].visited && maze[row+1][col].visited &&
		maze[row][col+1].visited && maze[row-1][col].visited;
}

int chooseNeighbour(mazeCell** maze, mazeCell current, int m, int n)
{
	int row = current.row;
	int col = current.col;
	int dir;

	while(true)
	{
		dir = random_range(1, 4);

		// check if chosen is not out of boundary and is not visited
		if(dir == 1 && current.col !=0 && !maze[row][col-1].visited) 
			return dir;
		else if(dir == 2 && current.col != n-1 && !maze[row][col+1].visited)
			return dir;
		else if(dir == 3 && current.row != 0 && !maze[row-1][col].visited)
			return dir;
		else if( dir == 4 && current.row != m-1 && !maze[row+1][col].visited)
			return dir;
	}
}

void generateMaze(mazeCell** maze , int m, int n)
{
	Stack<mazeCell> backtrack;
	mazeCell current;

	maze[0][0].visited = true;
	backtrack.push(maze[0][0]);

	while(!backtrack.isEmpty())
	{
		current = backtrack.topAndPop();

		if(!checkNeighbour(maze, current, m ,n))
		{
			backtrack.push(current);
			int direction = chooseNeighbour(maze, current, m, n);

			if(direction == 1)
			{
				maze[current.row][current.col].l = 0;
				maze[current.row][current.col-1].r = 0;
				maze[current.row][current.col-1].visited = true;

				backtrack.push(maze[current.row][current.col-1]);
			}

			else if(direction == 2)
			{
				maze[current.row][current.col].r = 0;
				maze[current.row][current.col+1].l = 0;
				maze[current.row][current.col+1].visited = true;

				backtrack.push(maze[current.row][current.col+1]);
			}

			else if(direction == 3)
			{
				maze[current.row][current.col].u = 0;
				maze[current.row-1][current.col].d = 0;
				maze[current.row-1][current.col].visited = true;

				backtrack.push(maze[current.row-1][current.col]);
			}

			else if(direction == 4)
			{
				maze[current.row][current.col].d = 0;
				maze[current.row+1][current.col].u = 0;
				maze[current.row+1][current.col].visited = true;

				backtrack.push(maze[current.row+1][current.col]);
			}
			
		}
		
	}
}


// NOT FINISHEED
void findPath(mazeCell** maze, int entryR, int entryC, int exitR, int exitC)
{
	Stack<mazeCell> visited;
	visited.push(maze[entryR][entryC]);

	mazeCell current = visited.top();

	while(current.row != exitR && current.col != exitC)
	{



	}


}

int main()
{
	int k, m, n;

	cout << "Enter the number of mazes: ";
	cin >> k;

	cout << "Enter the number of rows and columns (M and N): ";
	cin >> m >> n;	

	mazeCell*** mazeList = new mazeCell**[k];
	mazeCell** myMaze;
	
	for (int i = 0; i < k; i++)
	{
		myMaze = initializeMaze(m, n);

		generateMaze(myMaze, m, n);

		ofstream output;		
		output.open("maze_" + to_string(i + 1) + ".txt");

		output << m << " " << n << endl;

		for (int i = 0; i < m; i++) 
		{
			for(int j = 0; j < n; j++)
			{
				output << "x=" << j << " y=" << m-i-1 << " l=" << myMaze[i][j].l << 
					" r=" << myMaze[i][j].r << " u=" << myMaze[i][j].u << " d=" << myMaze[i][j].d << endl;
			}
		}

		output.close();

		mazeList[i] = myMaze;

	}






	// delete dynamic memory allocations
	for(int i = 0; i < k; i++)
	{
		deleteMaze(mazeList[i], m);
	}
	delete []mazeList;

	return 0;
}