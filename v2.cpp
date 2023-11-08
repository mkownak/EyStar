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
    vector<Kratka*>neighbors;
    Kratka *rodzic;

    Kratka(int x, int y) : x(x), y(y), g(0), h(0), f(0), obstacle(false), rodzic(nullptr){}

    void add_neighbors(vector<vector<Kratka>>grid)
    {
        if(x-1 >= 0 && grid[x-1][y].obstacle == false) //up
        {
            neighbors.push_back(&grid[x-1][y]);
        }

        if(x+1 < 20 && grid[x+1][y].obstacle == false) //down
        {
            neighbors.push_back(&grid[x+1][y]);
        }

        if(y-1 >= 0 && grid[x][y-1].obstacle == false) //left
        {
            neighbors.push_back(&grid[x][y-1]);
        }

        if(y+1 < 20 && grid[x][y+1].obstacle == false) //right
        {
            neighbors.push_back(&grid[x][y+1]);
        }
    }
};

double calculate_h(Kratka *next, Kratka *cel)
{
    return sqrt(pow(cel->x - next->x,2)) + sqrt(pow(cel->y - next->y,2));
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

void display_pos(Kratka* kratka)
{
    cout<<"x:"<<kratka->x<<" "<<"y:"<<kratka->y<<" "<<"g:"<<kratka->g<<" "<<"h:"<<kratka->h<<" "<<"f:"<<kratka->f<<endl;
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


int main()
{
    vector<vector<int>>grid = load_grid("grid.txt");
    show_grid(grid);

    vector<vector<Kratka>>grid_kratka = kratka_grid(grid);

    for(int x=0; x<grid_kratka.size(); x++)
    {
        for(int y=0; y<grid_kratka.size(); y++)
        {
            grid_kratka[x][y].add_neighbors(grid_kratka);
        }
    }

    vector<Kratka*>open_list;
    vector<Kratka*>closed_list;
    vector<Kratka*>path;

    Kratka* start = &grid_kratka[19][0];
    Kratka* cel = &grid_kratka[0][19];

    cout<<grid_kratka[19][1].neighbors.size()<<endl;
    display_pos(grid_kratka[19][1].neighbors[0]);
    display_pos(grid_kratka[19][1].neighbors[1]);


    open_list.push_back(start);
    int n=0;

    while(n<2)  //!open_list.empty()
    {
        Kratka *aktualna;
        int best_index=0;
        for(int i=0; i<open_list.size(); i++)
        {
            if(open_list[i]->f < open_list[best_index]->f)
            {
                best_index = i;
            }

            if(open_list[i]->f == open_list[best_index]->f)
            {
                if(open_list[i]->g > open_list[best_index]->g)
                {
                    best_index = i;
                }
            }
        }

        aktualna = open_list[best_index];
        cout<<"aktualna"<<endl;
        display_pos(aktualna);
        if(aktualna == cel)
        {
            Kratka* temp = aktualna;
            path.push_back(temp);
            while(temp->rodzic)
            {
                path.push_back(temp->rodzic);
                temp = temp->rodzic;
            }

            cout<<"essa"<<endl;
        }

        open_list.erase(open_list.begin() + best_index);
        closed_list.push_back(aktualna);

        vector<Kratka*>neighbors = aktualna->neighbors;
        //cout<<neighbors.size()<<endl;

        cout<<"sasiedzi"<<endl;
        for(int i=0; i<neighbors.size(); i++)
        {
            display_pos(neighbors[i]);
        }

        for(int i=0; i<neighbors.size(); i++)
        {
            Kratka *neighbor = neighbors[i];

            if(find(closed_list.begin(), closed_list.end(), neighbor) == closed_list.end())
            {
                int tempG = aktualna->g + 1;

                if(find(open_list.begin(), open_list.end(), neighbor) == open_list.end())
                {
                    open_list.push_back(neighbor);
                }
                else if(tempG >= neighbor->g)
                {
                    continue;
                }
                neighbor->g = tempG;
                neighbor->h = calculate_h(neighbor, cel);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->rodzic = aktualna;
            }
        }
        n++;
    }



    return 0;
}
