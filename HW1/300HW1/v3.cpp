#include <iostream>
#include <string>
#include <fstream>
#include <random> 
#include <time.h>
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

	bool visited; // for maze generation
	bool isVisited; // for path finding

	// default constructor
	mazeCell():row(0), col(0), l(1), r(1), u(1), d(1), visited(false), isVisited(false){}
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

// not necessary
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
Stack<mazeCell> findPath(mazeCell** maze, int entryR, int entryC, int exitR, int exitC)
{
	Stack<mazeCell> path;
	mazeCell current;
	//int dir;

	maze[entryR][entryC].isVisited = true;
	path.push(maze[entryR][entryC]);
	current = path.top();
	
	while(!(current.row == exitR && current.col == exitC))
	{
		bool found = false;
		//dir = random_range(1, 4);

		for(int dir = 1; dir < 5 && !found; dir++)
		{

			// check if chosen is accesible
			if(dir == 1 && current.l == 0 && !maze[current.row][current.col-1].isVisited)
			{
				maze[current.row][current.col-1].isVisited = true;
				current = maze[current.row][current.col-1];
				path.push(current);
				found = true;
			}
			
			else if(dir == 2 && current.r == 0 && !maze[current.row][current.col+1].isVisited)
			{
				maze[current.row][current.col+1].isVisited = true;
				current = maze[current.row][current.col+1];
				path.push(current);
				found = true;
			}

			else if(dir == 3 && current.u == 0 && !maze[current.row-1][current.col].isVisited)
			{
				maze[current.row-1][current.col].isVisited = true;
				current = maze[current.row-1][current.col];
				path.push(current);
				found = true;
			}

			else if( dir == 4 && current.d == 0 && !maze[current.row+1][current.col].isVisited)
			{
				maze[current.row+1][current.col].isVisited = true;
				current = maze[current.row+1][current.col];
				path.push(current);
				found = true;
			}

		}

		if(!found  && !path.isEmpty())
		{	
			//backtrack using stack
			path.pop();
			current = path.top(); 
		}


	} // end-while	

	return path;
}

void printStack(Stack<mazeCell> & p, ofstream & out, int m)
{
	// a recursive function to print stack
	// this allows me to print the starting cell first which is at the bottom of the stack

	if (p.isEmpty())  
        return;       
  
    mazeCell temp = p.topAndPop(); 

    printStack(p, out, m); 

	out << temp.col << " " << m - temp.row - 1 << endl; 
      
    p.push(temp); // to preserve the stack 
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
	
	for (int idx = 0; idx < k; idx++)
	{
		myMaze = initializeMaze(m, n);

		generateMaze(myMaze, m, n);

		ofstream output;		
		output.open("maze_" + to_string(idx + 1) + ".txt");

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

		mazeList[idx] = myMaze;
	}

	int id, entryX, entryY, exitX, exitY;

	cout << "Enter a maze ID between 1 to " << k << " inclusive to find a path: ";
	cin >> id;

	cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
	cin >> entryX >> entryY;

	cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
	cin >> exitX >> exitY;

	// arguments of the below function call is respectively as follows:
	// maze pointer, entry row, entry column, exit row, exit column
	Stack<mazeCell> path = findPath(mazeList[id-1], m-entryY-1, entryX, m-exitY-1, exitX);
	
	// write path into file
	ofstream pathOutput;

	pathOutput.open("maze_"+to_string(id)+"_path_"+to_string(entryX)+
		"_"+to_string(entryY)+"_"+to_string(exitX)+"_"+to_string(exitY)+".txt");

	printStack(path, pathOutput, m);

	pathOutput.close();

	// delete dynamic memory allocations
	for(int i = 0; i < k; i++)
	{
		deleteMaze(mazeList[i], m);
	}
	delete [] mazeList;

	return 0;
}