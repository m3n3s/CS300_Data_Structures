#include <iostream>
#include <string>
#include <fstream>
#include "randgen.h"
//#include "Stack.h"
#include "Stack.cpp"  // BECAUSE OF TRANSLATION UNIT ERRORS CAUSED BY TEMPLATING THE STACK CLASS

using namespace std;

// MEHMET ENES BATTAL - HW1

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

int randomWall(const mazeCell & current , int m, int n)
{
	// GIVES A VALID RANDOM WALL
	RandGen d;
	int result;
	bool found = false;

	do
	{		
		result = d.RandInt(1,4);

		// checks the 4 corners
		if(current.row == 0 && current.col == 0 && (result == 2 || result == 4))
			found = true;

		else if(current.row == 0 && current.col == n-1 && (result == 1 || result == 4))
			found = true;

		else if(current.row == m-1 && current.col == 0 && (result == 2 || result == 3))
			found = true;

		else if(current.row == m-1 && current.col == n-1 && (result == 1 || result == 3))
			found = true;

		// checks the 4 sides
		else if(current.row == 0 && result != 3)
			found = true;

		else if(current.col == 0 && result != 1)
			found = true;

		else if(current.row == m-1 && result != 4)
			found = true;

		else if(current.col == n-1 && result !=2)
			found = true;


	}while(!found);

	return result;
}

bool hasWall(mazeCell cell, int m, int n)
{
	// tells if current cell has a wall to knock down

	if(cell.row == 0 && cell.col == 0 && cell.u == 0 && cell.r == 0)
		return false;
	else if(cell.row == 0 && cell.col == n-1 && cell.u == 0 && cell.l == 0)
		return false;
	else if(cell.row == m-1 && cell.col == 0 && cell.d == 0 && cell.r == 0)
		return false;
	else if(cell.row == m-1 && cell.col == n-1 && cell.d == 0 && cell.l == 0)
		return false;
	else if(cell.row == 0 && cell.l == 0 && cell.u == 0 && cell.r == 0)
		return false;
	else if(cell.col == 0 && cell.u == 0 && cell.r == 0 && cell.d == 0)
		return false;
	else if(cell.row == m-1 && cell.l == 0 && cell.d == 0 && cell.r == 0)
		return false;
	else if(cell.col == n-1 && cell.u == 0 && cell.l == 0 && cell.d == 0)
		return false;
	else if(cell.l == 0 && cell.r == 0 && cell.u == 0 && cell.d == 0)
		return false;
	else 
		return true;
}

void generateMaze(mazeCell** maze, int m, int n)
{
	Stack<mazeCell> backtrack;

	maze[0][0].visited = true;
	backtrack.push(maze[0][0]);

	mazeCell currentCell = backtrack.top();
	int wall;

	while(!allVisited(maze, m, n))
	{
		if(hasWall(currentCell, m, n))
		{		
			RandGen d;
			wall = d.RandInt(1,4);
			//wall = randomWall(currentCell, m, n);

			if(wall == 1 && currentCell.col != 0)
			{
				maze[currentCell.row][currentCell.col].l = 0;
				maze[currentCell.row][currentCell.col-1].r = 0;
				maze[currentCell.row][currentCell.col-1].visited = true;
				currentCell = maze[currentCell.row][currentCell.col-1];
				backtrack.push(currentCell);
			}

			else if(wall == 2 && currentCell.col != n-1)
			{
				maze[currentCell.row][currentCell.col].r = 0;
				maze[currentCell.row][currentCell.col+1].l = 0;
				maze[currentCell.row][currentCell.col+1].visited = true;
				currentCell = maze[currentCell.row][currentCell.col+1];
				backtrack.push(currentCell);
			}

			else if(wall == 3 && currentCell.row != 0)
			{
				maze[currentCell.row][currentCell.col].u = 0;
				maze[currentCell.row-1][currentCell.col].d = 0;
				maze[currentCell.row-1][currentCell.col].visited = true;
				currentCell = maze[currentCell.row-1][currentCell.col];
				backtrack.push(currentCell);
			}

			else if(wall == 4 && currentCell.row != m-1)
			{
				maze[currentCell.row][currentCell.col].d = 0;
				maze[currentCell.row+1][currentCell.col].u = 0;
				maze[currentCell.row+1][currentCell.col].visited = true;
				currentCell = maze[currentCell.row+1][currentCell.col];
				backtrack.push(currentCell);
			}
		}

		else
		{
			backtrack.pop();
			currentCell = backtrack.top();
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
	
	for (int i = 0; i < k; i++)
	{
		mazeCell** myMaze = initializeMaze(m, n);

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


		// delete dynamic memory (MAZE)
		deleteMaze(myMaze, m);
	}



	return 0;
}