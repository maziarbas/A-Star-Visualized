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
const int delta[4][2]{{-1,0},{0,-1},{1,0},{0,1}};

vector<State> ParseLine(string line)
{
    int n;
    vector<State> row;
    istringstream sline(line);
    while (sline>>n)
    {
        switch (n)
        {
        case 0:
            row.push_back(State::KEmpty);
            break;
        
        default:
            row.push_back(State::KObstacle);
            break;
        }    }
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
            //cout<<line<<"\n";
            board.push_back(ParseLine(line));
        }
    }
    return board;
}

int Heuristic(int x1, int y1, int x2, int y2){
    return (abs(x2-x1)+abs(y2-y1));
}
// function to compare the f value of each node, f=g+h;
bool Compare(const vector<int> a , const vector<int> b)
{
    int f1 = a[2]+a[3];
    int f2 = a[2]+a[3];
    return f1>f2;
}

// sorts the cells in the open cell vector 
void CellSort(vector<vector<int>> *v )
{
    sort(v->begin(),v->end(),Compare);
}

bool CheckValidCell(int x, int y, vector<vector<State>> &board)
{
    bool x_on_grid = (x>=0 && x<board.size());
    bool y_on_grid = (y>=0 && y<board[0].size());

    if(x_on_grid && y_on_grid)
    {
        return board[x][y]==State::KEmpty;
    }
    return false;
}
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid)
{
    openlist.push_back({x,y,g,h});
    grid[x][y] =State::KClosed;
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
        int y2 = x+delta[i][2];

        if (CheckValidCell(x2, y2, grid))
        {
            int h2 = Heuristic(x2, y2, goal[0],goal[1]);
            int g2 = g++;
            AddToOpen(x2,y2,g2,h2,openlist,grid);
        }
    }
}

vector<vector<State>> Search(vector<vector<State>> &grid, int init[2], int goal[2])
{
    vector<vector<int>> open {};

    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0],goal[1]);

    AddToOpen(x,y,g,h,open,grid);

    // checking the next node
    while(open.size()>0)
    {
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        int x_current = current[0];
        int y_current = current[1];
        grid[x_current][y_current]=State::KPath;

        if(x_current==goal[0] && y_current==goal[1])
        {
            grid[init[0]][init[1]]=State::KStart;
            grid[goal[0]][goal[1]]=State::KGoal;
            return grid;
        }
        ExpandNeighbors(current,goal,open,grid);
    }
    cout << "No path found!" << "\n";
  return std::vector<vector<State>>{};
} 

int main()
{
   ReadBoardFile("nodes.txt");
}
