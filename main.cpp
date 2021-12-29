#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>

using std::sort;
using std::abs;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State {KEmpty,KObstacle,KClosed, KPath, KStart, KGoal};
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

vector<State> ParseLine(string line)
{
    int n;
    char c;
    vector<State> row;
    istringstream sline(line);
    while (sline >> n >> c && c == ',')
    {
        switch (n)
        {
        case 0:
            row.push_back(State::KEmpty);
            break;
        
        default:
            row.push_back(State::KObstacle);
            break;
        } 
    }
    return row;    
}

vector<vector<State>> ReadBoardFile(string path)
{
    ifstream board_file(path);
    vector<vector<State>> board;
    if(board_file)
    {
        string line;
        while(getline(board_file,line))
        {
            board.push_back(ParseLine(line));
        }
    }
    return board;
}

int Heuristic(int x1, int y1, int x2, int y2){
    return abs(x2-x1)+abs(y2-y1);
}
// function to compare the f value of each node, f=g+h;
bool Compare(const vector<int> a , const vector<int> b)
{
    int f1 = a[2]+a[3];
    int f2 = b[2]+b[3];
    return f1>f2;
}

// sorts the cells in the open cell vector 
void CellSort(vector<vector<int>> *v )
{
    sort(v->begin(),v->end(),Compare);
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid)
{
    bool x_on_grid = (x>=0 && x<grid.size());
    bool y_on_grid = (y>=0 && y<grid[0].size());

    if(x_on_grid && y_on_grid)
    {
        return grid[x][y] == State::KEmpty;
    }
    return false;
}
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  // Add node to open vector, and mark grid cell as closed.
  openlist.push_back(vector<int>{x, y, g, h});
  grid[x][y] = State::KClosed;
}

// checking on neghbor nodes and if not the goal add to open list, g++ 
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid)
{
    int x = current[0];
    int y = current[1];
    int g= current[2];

    for(int i = 0; i<4;i++)
    {
        int x2 = x+delta[i][0];
        int y2 = y+delta[i][1];

        if (CheckValidCell(x2, y2, grid))
        {
            int h2 = Heuristic(x2, y2, goal[0],goal[1]);
            int g2 = g+1;
            AddToOpen(x2,y2,g2,h2,openlist,grid);
        }
    }
}

vector<vector<State>> Search(vector<vector<State>> &grid, int init[2], int goal[2])
{
    vector<vector<int>> open{};

    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0],goal[1]);

    AddToOpen(x,y,g,h,open,grid);

    // checking the next node
    while (open.size() > 0) {
    // Get the next node
    CellSort(&open);
    auto current = open.back();
    open.pop_back();
    x = current[0];
    y = current[1];
    grid[x][y] = State::KPath;

    // Check if we're done.
    if (x == goal[0] && y == goal[1]) {
      grid[init[0]][init[1]] = State::KStart;
      grid[goal[0]][goal[1]] = State::KGoal;
      return grid;
    }
    
    // If we're not done, expand search to current node's neighbors.
    ExpandNeighbors(current, goal, open, grid);
  }
  
  // We've run out of new nodes to explore and haven't found a path.
  cout << "No path found!" << "\n";
  return std::vector<vector<State>>{};
    
} 

string CellString(State cell) {
    
    switch(cell) {
    case State::KObstacle: return "#   ";
    case State::KPath: return "%   ";
    case State::KStart: return ">   ";
    case State::KGoal: return "*   ";
    default: return  "O   ";
  }
}

// function to visulaize the path 
void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
        cout<<CellString(board[i][j]);
    }
    cout << "\n";
  }
}

int main()
{
    int init[2]{0,0};
    int goal[2]{4,5};

    auto board = ReadBoardFile("1.board");
    auto path = Search(board, init,goal);
    PrintBoard(path);

}
