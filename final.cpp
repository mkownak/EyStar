#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


using namespace std;

struct Kratka
{
    int x;
    int y;
    double g;
    double h;
    double f;
    bool obstacle;
    vector<Kratka>neighbors;
    pair<int,int>rodzic;

    Kratka(int x, int y) : x(x), y(y), g(0), h(0), f(0), obstacle(false){}


    void add_neighbors(vector<vector<Kratka>>grid)
    {
        if(x-1 >= 0 && grid[x-1][y].obstacle == false) //up
        {
            neighbors.push_back(grid[x-1][y]);
        }

        if(x+1 < 20 && grid[x+1][y].obstacle == false) //down
        {
            neighbors.push_back(grid[x+1][y]);
        }

        if(y-1 >= 0 && grid[x][y-1].obstacle == false) //left
        {
            neighbors.push_back(grid[x][y-1]);
        }

        if(y+1 < 20 && grid[x][y+1].obstacle == false) //right
        {
            neighbors.push_back(grid[x][y+1]);
        }
    }
};

double calculate_h(Kratka next, Kratka cel)
{
    return sqrt(pow(cel.x - next.x,2) + pow(cel.y - next.y,2));
}

void show_grid(vector<vector<int>>grid)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            cout << grid[i][j]<<" ";
        }
        cout << "\n";
    }
}

void display_pos(Kratka kratka)
{
    cout<<"x:"<<kratka.x<<" "<<"y:"<<kratka.y<<" "<<"g:"<<kratka.g<<" "<<"h:"<<kratka.h<<" "<<"f:"<<kratka.f<<endl;
}

vector<vector<int>>load_grid(string file_name)
{
    vector<vector<int>>mapa(20,vector<int>(20));
    ifstream grid(file_name);

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            grid >> mapa[i][j];
        }
    }
    return mapa;
}

vector<vector<Kratka>>kratka_grid(vector<vector<int>>grid)
{
    vector<vector<Kratka>>new_grid;

    for(int x=0; x<grid.size(); x++)
    {
        vector<Kratka>row;
        for(int y=0; y<grid.size(); y++)
        {
            Kratka kratka(x,y);
            if(grid[x][y]==5)
            {
                kratka.obstacle = true;
            }
            row.push_back(kratka);
        }
        new_grid.push_back(row);
    }
    return new_grid;
}

bool check(vector<Kratka>vect, Kratka kratka)
{
    bool temp=false;
    for(int i=0; i<vect.size(); i++)
    {
        if(vect[i].x == kratka.x && vect[i].y == kratka.y)
        {
            temp = true;
        }
    }
    return temp;
}


int main()
{
    vector<vector<int>>grid = load_grid("grid.txt");
    show_grid(grid);

    vector<vector<Kratka>>grid_kratka = kratka_grid(grid);

    vector<Kratka>open_list;
    vector<Kratka>closed_list;
    vector<Kratka>path;

    Kratka start = grid_kratka[19][0];
    Kratka cel = grid_kratka[0][19];

    if(start.obstacle)
    {
        cout<<"start jest przeszkoda, nie ma sciezki"<<endl;
        return 0;
    }

    if(cel.obstacle)
    {
        cout<<"cel jest przeszkoda, nie ma sciezki"<<endl;
        return 0;
    }

    open_list.push_back(start);

    while(!open_list.empty()) //!open_list.empty()
    {
        int best_index=0;
        for(int i=0; i<open_list.size(); i++)
        {
            if(open_list[i].f < open_list[best_index].f)
            {
                best_index=i;
            }

            if(open_list[i].f == open_list[best_index].f)
            {
                if(open_list[i].g > open_list[best_index].g)
                {
                    best_index = i;
                }
            }
        }

        Kratka aktualna = open_list[best_index];

        if(aktualna.x == cel.x && aktualna.y == cel.y)
        {
            grid[aktualna.x][aktualna.y]=3;
            while(aktualna.x != start.x || aktualna.y != start.y)
            {
                grid[aktualna.rodzic.first][aktualna.rodzic.second]=3;
                aktualna = grid_kratka[aktualna.rodzic.first][aktualna.rodzic.second];
            }
            cout<<"znaleziono sciezke"<<endl;
            show_grid(grid);
            return 0;
        }

        open_list.erase(open_list.begin() + best_index);
        closed_list.push_back(aktualna);

        aktualna.add_neighbors(grid_kratka);
        vector<Kratka>neighbors=aktualna.neighbors;
        for(int i=0;i<neighbors.size(); i++)
        {
            Kratka neighbor = neighbors[i];

            bool in_closed=check(closed_list, neighbor);
            bool in_open=check(open_list, neighbor);

            if(in_closed==true)
            {
                continue;
            }

            neighbor.g = aktualna.g + 1;
            neighbor.h = calculate_h(neighbor, cel);
            neighbor.f = neighbor.g + neighbor.h;
            neighbor.rodzic = make_pair(aktualna.x, aktualna.y);
            grid_kratka[neighbor.x][neighbor.y].rodzic = make_pair(aktualna.x, aktualna.y);

            if(in_open == true)
            {
                if(neighbor.g > aktualna.g)
                {
                    continue;
                }
            }

            open_list.push_back(neighbor);
        }
    }
    cout<<"nie znaleziono sciezki."<<endl;
    return 0;
}
