#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

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
    Kratka *rodzic;

    Kratka(int x, int y) : x(x), y(y), g(0), h(0), f(0), obstacle(false), rodzic(nullptr){}

    void add_neighbors(vector<vector<Kratka>>grid)
    {
        if(x-1 >= 0 && grid[x-1][y].obstacle == false) //up
        {
            Kratka ey(x-1,y);
            neighbors.push_back(ey);
        }

        if(x+1 < 20 && grid[x+1][y].obstacle == false) //down
        {
            Kratka ey(x+1,y);
            neighbors.push_back(ey);
        }

        if(y-1 >= 0 && grid[x][y-1].obstacle == false) //left
        {
            Kratka ey(x,y-1);
            neighbors.push_back(ey);
        }

        if(y+1 < 20 && grid[x][y+1].obstacle == false) //right
        {
            Kratka ey(x,y+1);
            neighbors.push_back(ey);
        }
    }
};

double calculate_h(Kratka next, Kratka cel)
{
    return sqrt(pow(cel.x - next.x,2)) + sqrt(pow(cel.y - next.y,2));
}

void show_grid(vector<vector<int>>grid)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            cout << grid[i][j] << " ";
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

    //grid_kratka[0][0].add_neighbors(grid_kratka);
    for(int x=0; x<20; x++)
    {
        for(int y=0; y<20; y++)
        {
            //cout<<x<<" "<<y<<endl;
            grid_kratka[x][y].add_neighbors(grid_kratka);
        }
    }

    vector<Kratka>open_list;
    vector<Kratka>closed_list;
    vector<Kratka>path;

    Kratka start = grid_kratka[19][0];
    Kratka cel = grid_kratka[0][19];

    open_list.push_back(start);

    while(!open_list.empty())
    {
        int best_index=0;
        for(int i=0; i<open_list.size(); i++)
        {
            if(open_list[i].f < open_list[best_index].f)
            {
                best_index=i;
            }
        }

        Kratka aktualna = open_list[best_index];
        display_pos(aktualna);

        if(aktualna.x == cel.x && aktualna.y == cel.y)
        {
            cout<<"gz";
        }

        open_list.erase(open_list.begin() + best_index);
        closed_list.push_back(aktualna);

        vector<Kratka>neighbors=aktualna.neighbors;
        for(int i=0;i<neighbors.size(); i++)
        {
            Kratka neighbor = neighbors[i];
            display_pos(neighbor);
            bool in_closed=check(closed_list, neighbor);
            bool in_open=check(open_list, neighbor);

            if(in_closed == false)
            {
                int tempG = aktualna.g + 1;

                if(in_open == true)
                {
                    if(tempG < neighbor.g)
                    {
                        neighbor.g = tempG;
                    }
                }
                else
                {
                    neighbor.g = tempG;
                    open_list.push_back(neighbor);
                }
                neighbor.h=calculate_h(neighbor,cel);
                neighbor.f=neighbor.g + neighbor.h;
            }

        }
    }
    cout<<open_list.size()<<endl;
    cout<<closed_list.size()<<endl;;
    return 0;
}
