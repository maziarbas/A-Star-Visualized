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


 
vector<int> ParseLine(string line)
{
    int n;
    vector<int> row;
    istringstream sline(line);
    while (sline>>n)
    {
        row.push_back(n);
        cout<<n<<"\n";
    }
    return row;    
}

vector<vector<int>> ReadBoardFile(string path)
{
    ifstream board_file(path);
    if(board_file)
    {
        string line;
        while(getline(board_file,line))
        {
            cout<<line<<"\n";
            ParseLine(line);
            
        }
    }
}



int main()
{
   ReadBoardFile("nodes.txt"); 
}
