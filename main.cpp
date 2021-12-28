#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>

using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State {KEmpty,KObstacle,KClosed, KPath, KStart, KGoal};


 
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
            cout<<line<<"\n";
            board.push_back(ParseLine(line));
        }
    }
    return board;
}



int main()
{
   ReadBoardFile("nodes.txt"); 
}
