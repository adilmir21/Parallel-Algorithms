//#pragma once
#include<iostream>
#include<Windows.h>
using namespace std;
#include<ctime>
#include<stdlib.h>
#include<queue>
#include<omp.h>
#include<chrono>
bool** visited;
queue<pair<int, int> > path;
int rows = 25;
int cols = 50;
char** puzzle;
char** puzzle2;
int start;
int Ends;
int start1;
int end1;
pair<int, int>pred[100][100];

void mazeGenerator(int& r, int& c)
{
	for (int i = 0;i < r;i++)
	{
		for (int j = 0;j < c;j++)
		{
			if (i == 0)
			{
				puzzle[i][j] = '-';
			}
			if (i > 0 && i < cols - 1)
			{
				puzzle[i][0] = '|';

			}
			if (i == rows - 1)
			{
				puzzle[i][j] = '-';
			}
			if (j == cols - 1 && (i > 0 && i < cols - 1))
			{

				puzzle[i][cols - 1] = '|';
			}
			if (j > 0 && i > 0 && j < cols - 1 && i < rows - 1)
			{
				puzzle[i][j] = ' ';
			}
			/*if (i == start)
			{
				puzzle[start][0] = '+';
			}
			if (i == Ends)
			{
				puzzle[Ends][cols - 1] = '+';
			}*/
		}
	}
	puzzle[start][start1] = '+';
	puzzle[Ends][end1] = '+';
}

void printMaze(char**p)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			cout << p[i][j];
		}
		cout << endl;
	}
}
void hurdlesCreator()
{
	srand(time(NULL));

	int randomRows[10];
	int randomCols[10];

	for (int i = 0;i < 10;i++)
	{
		int r1 = 2 + rand() % (rows - 11);//generates statring ith position of hurdle
		int r2 = 2 + rand() % (cols - 13);//generates ending jth position of hurdle

		randomRows[i] = r1;
		randomCols[i] = r2;

	}

	for (int i = 0;i < 10;i++)
	{
		int r = 0 + rand() % 2;
		for (int j = 0;j < 10;j++)
		{


			if (r == 0)
			{
				if (puzzle[randomRows[i]][randomCols[i] + j] != '+')
					puzzle[randomRows[i]][randomCols[i] + j] = '*';
			}
			if (r == 1)
			{
				if (puzzle[randomRows[i] + (j)][randomCols[i]] != '+')
					puzzle[randomRows[i] + (j)][randomCols[i]] = '*';
			}

		}

	}
}



void initializing()
{
	visited = new bool* [rows];
	for (int i = 0;i < rows;i++)
	{
		visited[i] = new bool[cols];
	}
}

// Direction vectors
int dRow[] = { -1, 0, 1, 0 };
int dCol[] = { 0, 1, 0, -1 };

// Function to check if a cell
// is be visited or not

bool isValid(int row, int col,char**p)
{
	// If cell lies out of bounds
	if (row < 0 || col < 0 || row >= rows || col >= cols)
		return false;
	if (p[row][col] == '*' || p[row][col] == '|' || p[row][col] == '-')
	{
		return false;
	}
	// If cell is already visited
	if (visited[row][col] == true)
		return false;

	// Otherwise
	return true;
}
// Function to perform the BFS traversal
void BFS(int row, int col, char**p)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	omp_set_nested(1);
	int total = omp_get_num_procs();
	omp_set_num_threads(total);

	// Stores indices of the matrix cells
	queue<pair<int, int> > q; //Pair is used to combine together

	// Mark the starting cell as visited
	// and push it into the queue
	q.push({ row, col+1 });

	visited[row][col+1] = true;

	// Iterate while the queue
	// is not empty
	int x, y;
	//while (!q.empty()) {

	int size = q.size();

#pragma parallel for private(x,y) shared(q)
	{

		for (int i = 0;i < 1000;i++)
		{
			if (q.empty())
			{
				break;
			}
#pragma omp critical
			{
				pair<int, int> cell = q.front();
				x = cell.first; //extracts index of row
				y = cell.second; // extracts index of col
				p[x][y] = '.';

				q.pop();
			}

			// Go to the adjacent cells
			if (isValid(x + 1, y,p) == true)
			{
#pragma omp critical
				{
					q.push({ x + 1, y });
					p[x][y] = '.';
					pred[x + 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x + 1][y] = true;
				}
			}
			if (isValid(x - 1, y,p) == true)
			{
#pragma omp critical
				{
					q.push({ x - 1, y });
					p[x][y] = '.';
					pred[x - 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x - 1][y] = true;
				}
			}
			if (isValid(x, y + 1,p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y + 1 });
					p[x][y] = '.';
					pred[x][y + 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y + 1] = true;
				}
			}
			if (isValid(x, y - 1,p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y - 1 });
					p[x][y] = '.';
					pred[x][y - 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y - 1] = true;
				}
			}
			if (p[x + 1][y] == '+')
			{
				break;
			}
			if (p[x - 1][y] == '+')
			{
				break;
			}
			if (p[x][y + 1] == '+')
			{
				break;
			}
		}
	}
}
void BFSStatic(int row, int col, char** p)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	omp_set_nested(1);
	int total = omp_get_num_procs();
	omp_set_num_threads(total);

	// Stores indices of the matrix cells
	queue<pair<int, int> > q; //Pair is used to combine together
	//two values that may be different in type. 
	//Pair provides a way to store two heterogeneous objects as a single unit.
	//It is basically used if we want to store tuples.

	// Mark the starting cell as visited
	// and push it into the queue
	q.push({ row, col + 1 });

	visited[row][col + 1] = true;

	// Iterate while the queue
	// is not empty
	int x, y;
	//while (!q.empty()) {

	int size = q.size();

#pragma parallel for private(x,y) shared(q) schedule(static,125)
	{

		for (int i = 0;i < 1000;i++)
		{
			if (q.empty())
			{
				break;
			}
#pragma omp critical
			{
				pair<int, int> cell = q.front();
				x = cell.first; //extracts index of row
				y = cell.second; // extracts index of col
				p[x][y] = '.';

				q.pop();
			}

			// Go to the adjacent cells
			if (isValid(x + 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x + 1, y });
					p[x][y] = '.';
					pred[x + 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x + 1][y] = true;
				}
			}
			if (isValid(x - 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x - 1, y });
					p[x][y] = '.';
					pred[x - 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x - 1][y] = true;
				}
			}
			if (isValid(x, y + 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y + 1 });
					p[x][y] = '.';
					pred[x][y + 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y + 1] = true;
				}
			}
			if (isValid(x, y - 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y - 1 });
					p[x][y] = '.';
					pred[x][y - 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y - 1] = true;
				}
			}
			if (p[x + 1][y] == '+')
			{
				break;
			}
			if (p[x - 1][y] == '+')
			{
				break;
			}
			if (p[x][y + 1] == '+')
			{
				break;
			}
		}
	}
}
void BFSDynamic(int row, int col, char** p)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	omp_set_nested(1);
	int total = omp_get_num_procs();
	omp_set_num_threads(total);

	// Stores indices of the matrix cells
	queue<pair<int, int> > q; //Pair is used to combine together
	//two values that may be different in type. 
	//Pair provides a way to store two heterogeneous objects as a single unit.
	//It is basically used if we want to store tuples.

	// Mark the starting cell as visited
	// and push it into the queue
	q.push({ row, col + 1 });

	visited[row][col + 1] = true;

	// Iterate while the queue
	// is not empty
	int x, y;
	//while (!q.empty()) {

	int size = q.size();

#pragma parallel for private(x,y) shared(q) schedule(dynamic,125)
	{

		for (int i = 0;i < 1000;i++)
		{
			if (q.empty())
			{
				break;
			}
#pragma omp critical
			{
				pair<int, int> cell = q.front();
				x = cell.first; //extracts index of row
				y = cell.second; // extracts index of col
				p[x][y] = '.';

				q.pop();
			}

			// Go to the adjacent cells
			if (isValid(x + 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x + 1, y });
					p[x][y] = '.';
					pred[x + 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x + 1][y] = true;
				}
			}
			if (isValid(x - 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x - 1, y });
					p[x][y] = '.';
					pred[x - 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x - 1][y] = true;
				}
			}
			if (isValid(x, y + 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y + 1 });
					p[x][y] = '.';
					pred[x][y + 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y + 1] = true;
				}
			}
			if (isValid(x, y - 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y - 1 });
					p[x][y] = '.';
					pred[x][y - 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y - 1] = true;
				}
			}
			if (p[x + 1][y] == '+')
			{
				break;
			}
			if (p[x - 1][y] == '+')
			{
				break;
			}
			if (p[x][y + 1] == '+')
			{
				break;
			}
		}
	}
}
void BFSGuided(int row, int col, char** p)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	omp_set_nested(1);
	int total = omp_get_num_procs();
	omp_set_num_threads(total);

	// Stores indices of the matrix cells
	queue<pair<int, int> > q; //Pair is used to combine together
	//two values that may be different in type. 
	//Pair provides a way to store two heterogeneous objects as a single unit.
	//It is basically used if we want to store tuples.

	// Mark the starting cell as visited
	// and push it into the queue
	q.push({ row, col + 1 });

	visited[row][col + 1] = true;

	// Iterate while the queue
	// is not empty
	int x, y;
	//while (!q.empty()) {

	int size = q.size();

#pragma parallel for private(x,y) shared(q) schedule(guided,125)
	{

		for (int i = 0;i < 1000;i++)
		{
			if (q.empty())
			{
				break;
			}
#pragma omp critical
			{
				pair<int, int> cell = q.front();
				x = cell.first; //extracts index of row
				y = cell.second; // extracts index of col
				p[x][y] = '.';

				q.pop();
			}

			// Go to the adjacent cells
			if (isValid(x + 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x + 1, y });
					p[x][y] = '.';
					pred[x + 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x + 1][y] = true;
				}
			}
			if (isValid(x - 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x - 1, y });
					p[x][y] = '.';
					pred[x - 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x - 1][y] = true;
				}
			}
			if (isValid(x, y + 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y + 1 });
					p[x][y] = '.';
					pred[x][y + 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y + 1] = true;
				}
			}
			if (isValid(x, y - 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y - 1 });
					p[x][y] = '.';
					pred[x][y - 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y - 1] = true;
				}
			}
			if (p[x + 1][y] == '+')
			{
				break;
			}
			if (p[x - 1][y] == '+')
			{
				break;
			}
			if (p[x][y + 1] == '+')
			{
				break;
			}
		}
	}
}
void BFSRuntime(int row, int col, char** p)
{
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	omp_set_nested(1);
	int total = omp_get_num_procs();
	omp_set_num_threads(total);

	// Stores indices of the matrix cells
	queue<pair<int, int> > q; //Pair is used to combine together
	//two values that may be different in type. 
	//Pair provides a way to store two heterogeneous objects as a single unit.
	//It is basically used if we want to store tuples.

	// Mark the starting cell as visited
	// and push it into the queue
	q.push({ row, col + 1 });

	visited[row][col + 1] = true;

	// Iterate while the queue
	// is not empty
	int x, y;
	//while (!q.empty()) {

	int size = q.size();

#pragma parallel for private(x,y) shared(q) schedule(auto,125)
	{

		for (int i = 0;i < 1000;i++)
		{
			if (q.empty())
			{
				break;
			}
#pragma omp critical
			{
				pair<int, int> cell = q.front();
				x = cell.first; //extracts index of row
				y = cell.second; // extracts index of col
				p[x][y] = '.';

				q.pop();
			}

			// Go to the adjacent cells
			if (isValid(x + 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x + 1, y });
					p[x][y] = '.';
					pred[x + 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x + 1][y] = true;
				}
			}
			if (isValid(x - 1, y, p) == true)
			{
#pragma omp critical
				{
					q.push({ x - 1, y });
					p[x][y] = '.';
					pred[x - 1][y] = make_pair(x, y);
					path.push({ x,y });

					visited[x - 1][y] = true;
				}
			}
			if (isValid(x, y + 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y + 1 });
					p[x][y] = '.';
					pred[x][y + 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y + 1] = true;
				}
			}
			if (isValid(x, y - 1, p) == true)
			{
#pragma omp critical
				{
					q.push({ x, y - 1 });
					p[x][y] = '.';
					pred[x][y - 1] = make_pair(x, y);
					path.push({ x,y });

					visited[x][y - 1] = true;
				}
			}
			if (p[x + 1][y] == '+')
			{
				break;
			}
			if (p[x - 1][y] == '+')
			{
				break;
			}
			if (p[x][y + 1] == '+')
			{
				break;
			}
		}
	}
}
void PATH()
{
	//while (!path.empty())
	{
		//pair<int, int>node = path.front();
		//puzzle[node.first][node.second] = '+';
		//path.pop();
	}
	Sleep(0.9);
	int count = 0;
	int lasti = 0;
	int lastj = 0;
	for (int i = 0;i < 100;i++)
	{
		for (int j = 0;j < 100;j++)
		{
		}
	}
}

void SimpleBFS(int row, int col,char**p)
{

	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}

	// Stores indices of the matrix cells
	queue<pair<int, int> > q; //Pair is used to combine together
	//two values that may be different in type. 
	//Pair provides a way to store two heterogeneous objects as a single unit.
	//It is basically used if we want to store tuples.

	// Mark the starting cell as visited
	// and push it into the queue
	q.push({ row, col+1 });

	visited[row][col+1] = true;

	// Iterate while the queue
	// is not empty
	while (!q.empty()) {
		
		pair<int, int> cell = q.front();
		int x = cell.first; //extracts index of row
		int y = cell.second; // extracts index of col
		p[x][y] = '.';

		q.pop();

		// Go to the adjacent cells
		if (isValid(x + 1, y,p) == true)
		{
			q.push({ x + 1, y });
			p[x][y] = '.';
			pred[x + 1][y] = make_pair(x, y);
			path.push({ x,y });

			visited[x + 1][y] = true;
		}
		PATH();
		if (isValid(x - 1, y,p) == true)
		{
			q.push({ x - 1, y });
			p[x][y] = '.';
			pred[x - 1][y] = make_pair(x, y);
			path.push({ x,y });

			visited[x - 1][y] = true;
		}
		if (isValid(x, y + 1,p) == true)
		{
			q.push({ x, y + 1 });
			p[x][y] = '.';
			pred[x][y + 1] = make_pair(x, y);
			path.push({ x,y });

			visited[x][y + 1] = true;
		}
		if (isValid(x, y - 1,p) == true)
		{
			q.push({ x, y - 1 });
			p[x][y] = '.';
			pred[x][y - 1] = make_pair(x, y);
			path.push({ x,y });

			visited[x][y - 1] = true;
		}
		if (p[x+1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			break;
		}
		if (p[x][y+1] == '+')
		{
			break;
		}
	}
}

int heuristic(int row, int col, int currRow, int currCol)
{
	int h = abs((Ends - abs(currRow)) + (end1 - abs(currCol)));
	//int g = (row + currRow) + (col + currCol);
	return h;
}
void Astar(int row, int col, char** p)
{
	queue<pair<int, int>>q;
	queue<pair<int, int>>minQueue;
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	q.push({ row,col });
	visited[row][col] = true;
	while (!q.empty())
	{
		pair<int, int>a = q.front();

		int x = a.first;
		int y = a.second;
		q.pop();
		//p[x][y] = '.';
		int min = 1000;
		int minRow = x, minCol = y;
		PATH();
		for (int i = 0;i < 4;i++)
		{
			int neighbourx = x + dRow[i];
			int neighboury = y + dCol[i];

			if (isValid(neighbourx, neighboury, p))
			{
				if (heuristic(row, col, neighbourx, neighboury) <= min)
				{
					min = heuristic(row, col, neighbourx, neighboury);
					minRow = neighbourx;
					minCol = neighboury;
				}
				visited[neighbourx][neighboury] = true;
				q.push({ neighbourx,neighboury });
			}
		}
		//q.push({ minRow,minCol });
		p[minRow][minCol] = '.';
		if (p[x + 1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			if ((x - 1) != 0)
			{
				break;
			}
		}
		if (p[x][y + 1] == '+')
		{
			break;
		}

	}
}

void AstarParallel(int row, int col, char** p)
{
	omp_set_nested(1);
	omp_set_num_threads(omp_get_num_procs());
	queue<pair<int, int>>q;
	queue<pair<int, int>>minQueue;
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	q.push({ row,col });
	visited[row][col] = true;
	int x, y;
#pragma parallel for private(x,y) shared(q)
	for (int i = 0;i < 1000;i++)

		//while (!q.empty())
	{
		if (q.empty())
		{
			break;
		}
		pair<int, int>a = q.front();

		x = a.first;
		y = a.second;
		q.pop();
		//p[x][y] = '.';
		int min = 1000;
		int minRow = x, minCol = y;
#pragma parallel for
		{
			for (int i = 0;i < 4;i++)
			{
				int neighbourx = x + dRow[i];
				int neighboury = y + dCol[i];

				if (isValid(neighbourx, neighboury, p))
				{
					if (heuristic(row, col, neighbourx, neighboury) <= min)
					{
						min = heuristic(row, col, neighbourx, neighboury);
						minRow = neighbourx;
						minCol = neighboury;
					}
#pragma critical
					{
						visited[neighbourx][neighboury] = true;
						q.push({ neighbourx,neighboury });
					}
				}
			}
		}
		//q.push({ minRow,minCol });
		p[minRow][minCol] = '.';
		if (p[x + 1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			break;
		}
		if (p[x][y + 1] == '+')
		{
			break;
		}

	}


}

void AstarParallelStatic(int row, int col, char** p)
{
	omp_set_nested(1);
	omp_set_num_threads(omp_get_num_procs());
	queue<pair<int, int>>q;
	queue<pair<int, int>>minQueue;
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	q.push({ row,col });
	visited[row][col] = true;
	int x, y;
#pragma parallel for private(x,y) shared(q) schedule(static,125)
	for (int i = 0;i < 1000;i++)

		//while (!q.empty())
	{
		if (q.empty())
		{
			break;
		}
		pair<int, int>a = q.front();

		x = a.first;
		y = a.second;
		q.pop();
		//p[x][y] = '.';
		int min = 1000;
		int minRow = x, minCol = y;
#pragma parallel for schedule(static,1)
		{
			for (int i = 0;i < 4;i++)
			{
				int neighbourx = x + dRow[i];
				int neighboury = y + dCol[i];

				if (isValid(neighbourx, neighboury, p))
				{
					if (heuristic(row, col, neighbourx, neighboury) <= min)
					{
						min = heuristic(row, col, neighbourx, neighboury);
						minRow = neighbourx;
						minCol = neighboury;
					}
#pragma critical
					{
						visited[neighbourx][neighboury] = true;
						q.push({ neighbourx,neighboury });
					}
				}
			}
		}
		//q.push({ minRow,minCol });
		p[minRow][minCol] = '.';
		if (p[x + 1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			break;
		}
		if (p[x][y + 1] == '+')
		{
			break;
		}

	}


}

void AstarParallelDynamic(int row, int col, char** p)
{
	omp_set_nested(1);
	omp_set_num_threads(omp_get_num_procs());
	queue<pair<int, int>>q;
	queue<pair<int, int>>minQueue;
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	q.push({ row,col });
	visited[row][col] = true;
	int x, y;
#pragma parallel for private(x,y) shared(q) schedule(dynamic,125)
	for (int i = 0;i < 1000;i++)

		//while (!q.empty())
	{
		if (q.empty())
		{
			break;
		}
		pair<int, int>a = q.front();

		x = a.first;
		y = a.second;
		q.pop();
		//p[x][y] = '.';
		int min = 1000;
		int minRow = x, minCol = y;
#pragma parallel for schedule(dynamic,1)
		{
			for (int i = 0;i < 4;i++)
			{
				int neighbourx = x + dRow[i];
				int neighboury = y + dCol[i];

				if (isValid(neighbourx, neighboury, p))
				{
					if (heuristic(row, col, neighbourx, neighboury) <= min)
					{
						min = heuristic(row, col, neighbourx, neighboury);
						minRow = neighbourx;
						minCol = neighboury;
					}
#pragma critical
					{
						visited[neighbourx][neighboury] = true;
						q.push({ neighbourx,neighboury });
					}
				}
			}
		}
		//q.push({ minRow,minCol });
		p[minRow][minCol] = '.';
		if (p[x + 1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			break;
		}
		if (p[x][y + 1] == '+')
		{
			break;
		}

	}


}

void AstarParallelGuided(int row, int col, char** p)
{
	omp_set_nested(1);
	omp_set_num_threads(omp_get_num_procs());
	queue<pair<int, int>>q;
	queue<pair<int, int>>minQueue;
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	q.push({ row,col });
	visited[row][col] = true;
	int x, y;
#pragma parallel for private(x,y) shared(q) scedule(guided,125)
	for (int i = 0;i < 1000;i++)

		//while (!q.empty())
	{
		if (q.empty())
		{
			break;
		}
		pair<int, int>a = q.front();

		x = a.first;
		y = a.second;
		q.pop();
		//p[x][y] = '.';
		int min = 1000;
		int minRow = x, minCol = y;
#pragma parallel for
		{
			for (int i = 0;i < 4;i++)
			{
				int neighbourx = x + dRow[i];
				int neighboury = y + dCol[i];

				if (isValid(neighbourx, neighboury, p))
				{
					if (heuristic(row, col, neighbourx, neighboury) <= min)
					{
						min = heuristic(row, col, neighbourx, neighboury);
						minRow = neighbourx;
						minCol = neighboury;
					}
#pragma critical
					{
						visited[neighbourx][neighboury] = true;
						q.push({ neighbourx,neighboury });
					}
				}
			}
		}
		//q.push({ minRow,minCol });
		p[minRow][minCol] = '.';
		if (p[x + 1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			break;
		}
		if (p[x][y + 1] == '+')
		{
			break;
		}

	}


}

void AstarParallelAuto(int row, int col, char** p)
{
	omp_set_nested(1);
	omp_set_num_threads(omp_get_num_procs());
	queue<pair<int, int>>q;
	queue<pair<int, int>>minQueue;
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}
	q.push({ row,col });
	visited[row][col] = true;
	int x, y;
#pragma parallel for private(x,y) shared(q) schedule(auto)
	for (int i = 0;i < 1000;i++)

		//while (!q.empty())
	{
		if (q.empty())
		{
			break;
		}
		pair<int, int>a = q.front();

		x = a.first;
		y = a.second;
		q.pop();
		//p[x][y] = '.';
		int min = 1000;
		int minRow = x, minCol = y;
#pragma parallel for
		{
			for (int i = 0;i < 4;i++)
			{
				int neighbourx = x + dRow[i];
				int neighboury = y + dCol[i];

				if (isValid(neighbourx, neighboury, p))
				{
					if (heuristic(row, col, neighbourx, neighboury) <= min)
					{
						min = heuristic(row, col, neighbourx, neighboury);
						minRow = neighbourx;
						minCol = neighboury;
					}
#pragma critical
					{
						visited[neighbourx][neighboury] = true;
						q.push({ neighbourx,neighboury });
					}
				}
			}
		}
		//q.push({ minRow,minCol });
		p[minRow][minCol] = '.';
		if (p[x + 1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			break;
		}
		if (p[x][y + 1] == '+')
		{
			break;
		}

	}


}
void sort(int* arr, int size, pair<int, int>x[])
{
	for (int i = 0;i < size;i++)
	{
		for (int j = i + 1;j < size;j++)
		{
			if (arr[j] < arr[i])
			{
				int temp = arr[i];
				arr[i] = arr[j];
				arr[j] = arr[i];

				pair<int, int>temp1 = x[i];
				x[i] = x[j];
				x[j] = temp1;
			}
		}
	}
}
int Cost(int currRow, int currCol)
{
	int c = abs((Ends - abs(currRow)) + (end1 - abs(currCol)));
	return c;

}
void ParallelBestFirstSearch(int row, int col, char** p)
{
	int proc = omp_get_num_procs();
	omp_set_num_threads(proc);

	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			visited[i][j] = false;
		}
	}

	// Stores indices of the matrix cells
	queue<pair<int, int> > q; //Pair is used to combine together

	q.push({ row, col + 1 });

	visited[row][col + 1] = true;

	// Iterate while the queue
	// is not empty
	int x, y;
#pragma omp parallel for private(x,y) shared(q) schedule(static,125)
	for (int k = 0;k < 1000;k++) {

		x = 0, y = 0;

		if (!q.empty())
		{
			pair<int, int> cell = q.front();
			int x = cell.first; //extracts index of row
			int y = cell.second; // extracts index of col
			p[x][y] = '.';

			q.pop();
		}
		else
		{
			break;
		}

		// Go to the adjacent cells
		int count = 0;
		int* arr;
		pair<int, int>temp[4];
		arr = new int[4];

		if (isValid(x + 1, y, p) == true)
		{
#pragma omp critical
			{
				visited[x + 1][y] = true;
				arr[count] = Cost(x + 1, y);
				temp[count] = make_pair(x + 1, y);
				count++;
			}
		}
		if (isValid(x - 1, y, p) == true)
		{
#pragma omp critical
			{
				visited[x - 1][y] = true;
				arr[count] = Cost(x - 1, y);
				temp[count] = make_pair(x - 1, y);
				count++;
			}
		}
		if (isValid(x, y + 1, p) == true)
		{
#pragma omp critical
			{
				visited[x][y + 1] = true;
				arr[count] = Cost(x, y + 1);
				temp[count] = make_pair(x, y + 1);
				count++;
			}
		}
		if (isValid(x, y - 1, p) == true)
		{
#pragma omp critical
			{
				visited[x][y - 1] = true;
				arr[count] = Cost(x, y - 1);
				temp[count] = make_pair(x, y - 1);
				count++;
			}
		}

		//cout << "count "<<count << endl;
#pragma omp critical
		{
			sort(arr, count, temp);
			for (int j = 0;j < count;j++)
			{
				q.push({ temp[j].first, temp[j].second });
				//cout << temp[j].first << " " << temp[j].second << " " << endl;
			}
		}
		count = 0;
		if (p[x + 1][y] == '+')
		{
			break;
		}
		if (p[x - 1][y] == '+')
		{
			break;
		}
		if (p[x][y + 1] == '+')
		{
			break;
		}
	}
}

int main()
{
	cout << "----------Board Details-------------" << endl;
	cout << " Enter Rows : ";
	cin >> rows;
	cout << " Enter Columns : ";
	cin >> cols;

	cout << " Enter Starting (row) Position (Greater than = 0 Less than = "<<rows-1<<" ) : ";
	cin >> start;
	cout << " Enter Starting (col)Position (Greater than 0 = Less than = " << cols - 1 << " ) : ";
	cin >> start1;
	cout << " Enter Ending (row)Position (Greater than 0  = Less than = " << rows - 1 << " ) : ";
	cin >> Ends;
	cout << " Enter Ending (col)Position (Greater than 0  = Less than = " << cols - 1 << " ) : ";
	cin >> end1;

	if (start <1 || start>rows - 1)
	{
		start = 1;
	}
	if (Ends <1 || Ends>rows - 1)
	{
		Ends = 1;
	}


	puzzle = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle[i] = new char[cols];
	}
	puzzle2 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle2[i] = new char[cols];
	}
	char** puzzle3 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle3[i] = new char[cols];
	}
	char** puzzle4 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle4[i] = new char[cols];
	}
	char** puzzle5 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle5[i] = new char[cols];
	}
	char** puzzle6 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle6[i] = new char[cols];
	}
	char** puzzle7 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle7[i] = new char[cols];
	}
	char** puzzle8 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle8[i] = new char[cols];
	}
	char** puzzle9 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle9[i] = new char[cols];
	}
	char** puzzle10 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle10[i] = new char[cols];
	}
	char** puzzle11 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle11[i] = new char[cols];
	}
	char** puzzle12 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle12[i] = new char[cols];
	}
	char** puzzle13 = new char* [rows];
	for (int i = 0;i < rows;i++)
	{
		puzzle13[i] = new char[cols];
	}
	mazeGenerator(rows, cols);
	hurdlesCreator();
	initializing();
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle2[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle3[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle4[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle5[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle6[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle7[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle8[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle9[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle10[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle11[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle12[i][j] = puzzle[i][j];
		}

	}
	for (int i = 0;i < rows;i++)
	{
		for (int j = 0;j < cols;j++)
		{
			puzzle13[i][j] = puzzle[i][j];
		}

	}
	system("cls");

	int option;
	cout << "Do you want to Run Specific Algorithm OR Run all Algorithmn\n1. Specific\n2. All\nEnter your choice: ";
	cin >> option;
	while (option < 1 || option >2)
	{
		cout << "Enter Again : ";
		cin >> option;
	}
	if (option == 1)
	{
		cout << "--------Select----------\n1.  Parallel BFS\n2.Simple BFS\n3. Parallel BFS with Static\n4. Parallel BFS with Guided\n";
		cout << "5. Parallel BFS with Dynamic\n6. Parallel BFS with Runtime\n7. A*\n8. Parallel A*\n";
		cout << "9. Parallel A* with Static\n10.Parallel A* with Dynamic\n11. Parallel A* with Guided\n12. Parallel A* with Runtime\n";
		cout << "13. Parallel Best First Search\n";
		cout << "Enter : ";
		int choose;
		cin >> choose;
		while (choose < 1 || choose>13)
		{
			cout << "Enter Again : ";
			cin >> choose;
		}
		system("cls");
		if (choose == 1)
		{
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			BFS(start, start1, puzzle);
			//PATH();
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			std::cout << "Time of Parallel BFS= " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " msec" << std::endl;
			printMaze(puzzle);
		}
		if (choose == 2)
		{
			std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();

			SimpleBFS(start, start1, puzzle2);
			//PATH();
			std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
			std::cout << "Time of Simple BFS = " << std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1).count() << " msec" << std::endl;
			printMaze(puzzle2);
		}
		if (choose == 3)
		{
			std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
			BFSStatic(start, start1, puzzle9);
			std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
			std::cout << "Static BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() << " msec" << std::endl;
			printMaze(puzzle9);
		}
		if (choose == 4)
		{
			std::chrono::steady_clock::time_point begin3 = std::chrono::steady_clock::now();
			BFSGuided(start, start1, puzzle10);
			std::chrono::steady_clock::time_point end3 = std::chrono::steady_clock::now();
			std::cout << "Guided BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end3 - begin3).count() << " msec" << std::endl;
			printMaze(puzzle10);
		}
		if (choose == 5)
		{
			std::chrono::steady_clock::time_point begin4 = std::chrono::steady_clock::now();
			BFSDynamic(start, start1, puzzle11);
			std::chrono::steady_clock::time_point end4 = std::chrono::steady_clock::now();
			std::cout << "Dynamic BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end4 - begin4).count() << " msec" << std::endl;
			printMaze(puzzle11);
		}

		if (choose == 6)
		{
			std::chrono::steady_clock::time_point begin5 = std::chrono::steady_clock::now();
			BFSRuntime(start, start1, puzzle12);
			std::chrono::steady_clock::time_point end5 = std::chrono::steady_clock::now();
			std::cout << "Runtime BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end5 - begin5).count() << " msec" << std::endl;
			printMaze(puzzle12);
		}
		if (choose == 7)
		{
			std::chrono::steady_clock::time_point begin6 = std::chrono::steady_clock::now();
			Astar(start, start1, puzzle3);
			std::chrono::steady_clock::time_point end6 = std::chrono::steady_clock::now();
			std::cout << "Simple A* " << std::chrono::duration_cast<std::chrono::microseconds>(end6 - begin6).count() << " msec" << std::endl;
			printMaze(puzzle3);
		}
		if (choose == 8)
		{
			std::chrono::steady_clock::time_point begin7 = std::chrono::steady_clock::now();
			AstarParallel(start, start1, puzzle4);
			std::chrono::steady_clock::time_point end7 = std::chrono::steady_clock::now();
			std::cout << "Parallel A* " << std::chrono::duration_cast<std::chrono::microseconds>(end7 - begin7).count() << " msec" << std::endl;
			printMaze(puzzle4);
		}
		if (choose == 9)
		{
			std::chrono::steady_clock::time_point begin8 = std::chrono::steady_clock::now();
			AstarParallelStatic(start, start1, puzzle5);
			std::chrono::steady_clock::time_point end8 = std::chrono::steady_clock::now();
			std::cout << "Parallel A* Static " << std::chrono::duration_cast<std::chrono::microseconds>(end8 - begin8).count() << " msec" << std::endl;
			printMaze(puzzle5);
		}
		if (choose == 10)
		{
			std::chrono::steady_clock::time_point begin9 = std::chrono::steady_clock::now();
			AstarParallelDynamic(start, start1, puzzle6);
			std::chrono::steady_clock::time_point end9 = std::chrono::steady_clock::now();
			std::cout << "Parallel A* Dynamic " << std::chrono::duration_cast<std::chrono::microseconds>(end9 - begin9).count() << " msec" << std::endl;
			printMaze(puzzle6);
		}
		if (choose == 11)
		{
			std::chrono::steady_clock::time_point begin10 = std::chrono::steady_clock::now();
			AstarParallelGuided(start, start1, puzzle7);
			std::chrono::steady_clock::time_point end10 = std::chrono::steady_clock::now();
			std::cout << "Parallel A* Guided" << std::chrono::duration_cast<std::chrono::microseconds>(end10 - begin10).count() << " msec" << std::endl;
			printMaze(puzzle7);
		}

		if (choose == 12)
		{
			std::chrono::steady_clock::time_point begin11 = std::chrono::steady_clock::now();
			AstarParallelAuto(start, start1, puzzle8);
			std::chrono::steady_clock::time_point end11 = std::chrono::steady_clock::now();
			std::cout << "Parallel A* Runtime" << std::chrono::duration_cast<std::chrono::microseconds>(end11 - begin11).count() << " msec" << std::endl;
			printMaze(puzzle8);
		}
		if (choose == 13)
		{
			std::chrono::steady_clock::time_point begin12 = std::chrono::steady_clock::now();
			ParallelBestFirstSearch(start, start1, puzzle13);
			std::chrono::steady_clock::time_point end12 = std::chrono::steady_clock::now();
			std::cout << "Parallel Best First Search :" << std::chrono::duration_cast<std::chrono::microseconds>(end12 - begin12).count() << " msec" << std::endl;
			printMaze(puzzle8);
		}
	}
	else
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		BFS(start, start1, puzzle);
		//PATH();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		std::cout << "Time of Parallel BFS= " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " msec" << std::endl;
		printMaze(puzzle);

		std::chrono::steady_clock::time_point begin1 = std::chrono::steady_clock::now();

		SimpleBFS(start, start1, puzzle2);
		//PATH();
		std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
		std::cout << "Time of Simple BFS = " << std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1).count() << " msec" << std::endl;
		printMaze(puzzle2);

		std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();
		BFSStatic(start, start1, puzzle9);
		std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
		std::cout << "Static BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() << " msec" << std::endl;
		printMaze(puzzle9);

		std::chrono::steady_clock::time_point begin3 = std::chrono::steady_clock::now();
		BFSGuided(start, start1, puzzle10);
		std::chrono::steady_clock::time_point end3 = std::chrono::steady_clock::now();
		std::cout << "Guided BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end3 - begin3).count() << " msec" << std::endl;
		printMaze(puzzle10);

		std::chrono::steady_clock::time_point begin4 = std::chrono::steady_clock::now();
		BFSDynamic(start, start1, puzzle11);
		std::chrono::steady_clock::time_point end4 = std::chrono::steady_clock::now();
		std::cout << "Dynamic BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end4 - begin4).count() << " msec" << std::endl;
		printMaze(puzzle11);


		std::chrono::steady_clock::time_point begin5 = std::chrono::steady_clock::now();
		BFSRuntime(start, start1, puzzle12);
		std::chrono::steady_clock::time_point end5 = std::chrono::steady_clock::now();
		std::cout << "Auto BFS " << std::chrono::duration_cast<std::chrono::microseconds>(end5 - begin5).count() << " msec" << std::endl;
		printMaze(puzzle12);


		std::chrono::steady_clock::time_point begin6 = std::chrono::steady_clock::now();
		Astar(start, start1, puzzle3);
		std::chrono::steady_clock::time_point end6 = std::chrono::steady_clock::now();
		std::cout << "Simple A* " << std::chrono::duration_cast<std::chrono::microseconds>(end6 - begin6).count() << " msec" << std::endl;
		printMaze(puzzle3);


		std::chrono::steady_clock::time_point begin7 = std::chrono::steady_clock::now();
		AstarParallel(start, start1, puzzle4);
		std::chrono::steady_clock::time_point end7 = std::chrono::steady_clock::now();
		std::cout << "Parallel A* " << std::chrono::duration_cast<std::chrono::microseconds>(end7 - begin7).count() << " msec" << std::endl;
		printMaze(puzzle4);


		std::chrono::steady_clock::time_point begin8 = std::chrono::steady_clock::now();
		AstarParallelStatic(start, start1, puzzle5);
		std::chrono::steady_clock::time_point end8 = std::chrono::steady_clock::now();
		std::cout << "Parallel A* Static " << std::chrono::duration_cast<std::chrono::microseconds>(end8 - begin8).count() << " msec" << std::endl;
		printMaze(puzzle5);


		std::chrono::steady_clock::time_point begin9 = std::chrono::steady_clock::now();
		AstarParallelDynamic(start, start1, puzzle6);
		std::chrono::steady_clock::time_point end9 = std::chrono::steady_clock::now();
		std::cout << "Parallel A* Dynamic " << std::chrono::duration_cast<std::chrono::microseconds>(end9 - begin9).count() << " msec" << std::endl;
		printMaze(puzzle6);

		std::chrono::steady_clock::time_point begin10 = std::chrono::steady_clock::now();
		AstarParallelGuided(start, start1, puzzle7);
		std::chrono::steady_clock::time_point end10 = std::chrono::steady_clock::now();
		std::cout << "Parallel A* Guided " << std::chrono::duration_cast<std::chrono::microseconds>(end10 - begin10).count() << " msec" << std::endl;
		printMaze(puzzle7);


		std::chrono::steady_clock::time_point begin11 = std::chrono::steady_clock::now();
		AstarParallelAuto(start, start1, puzzle8);
		std::chrono::steady_clock::time_point end11 = std::chrono::steady_clock::now();
		std::cout << "Parallel A* Auto " << std::chrono::duration_cast<std::chrono::microseconds>(end11 - begin11).count() << " msec" << std::endl;
		printMaze(puzzle8);
	}

}
