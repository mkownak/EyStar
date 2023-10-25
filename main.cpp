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
    bool przeszkoda;
    Kratka* rodzic;


    Kratka(int x, int y) : x(x), y(y), g(0), h(0), f(0), przeszkoda(false), rodzic(nullptr) {}
};

double calculate_g(Kratka *next, Kratka *start)
{
    return sqrt(pow(start->x - next->x,2)) + sqrt(pow(start->y - next->y,2));
}

double calculate_h(Kratka *next, Kratka *cel)
{
    return sqrt(pow(cel->x - next->x,2)) + sqrt(pow(cel->y - next->y,2));
}

void show_map(vector<vector<int>>map)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            std::cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

vector<vector<int>>load_mapa_int(string file_name)
{
    vector<vector<int>> mapa(20,vector<int>(20));
    ifstream grid(file_name);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            grid >> mapa[i][j];
        }
    }
    return mapa;
}

vector<vector<Kratka>>load_mapa(string file_name)
{
    vector<vector<Kratka>>grid;

    ifstream file(file_name);

    for (int x = 0; x < 20; x++)
    {
        vector<Kratka>row;
        for (int y = 0; y < 20; y++)
        {
            char cellState;
            file >> cellState;

            Kratka cell(x, y);

            cell.przeszkoda = (cellState == '5');

            row.push_back(cell);
        }
        grid.push_back(row);
    }

    return grid;
}



vector<Kratka*>add_neighbor(Kratka *kratka, vector<vector<Kratka>>mapa)
{
    vector<Kratka*>neighbors;
    vector<int>x{1,0,-1,0};
    vector<int>y{0,-1,0,1};

    for(int i=0; i<4; i++)
    {
        int new_x = kratka->x + x[i];
        int new_y = kratka->y + y[i];

        if(new_x >= 0 && new_x < mapa.size() && new_y >= 0 && new_y < mapa[0].size())
        {
            Kratka *neighbor = &mapa[new_x][new_y];

            if(neighbor->przeszkoda)
            {
                continue;
            }
            else
            {
                neighbor->rodzic=kratka;
                neighbors.push_back(neighbor);
            }
        }
    }
    return neighbors;
}

int main()
{
    vector<vector<int>>mapa_int = load_mapa_int("grid.txt");
    show_map(mapa_int);
    vector<vector<Kratka>>mapa=load_mapa("grid.txt");

    Kratka* start = &mapa[19][0];
    Kratka* cel = &mapa[0][19];

    vector<Kratka*>lista_otwarta;
    vector<Kratka*>lista_zamknieta;
    lista_otwarta.push_back(start);

    while(!lista_otwarta.empty())
    {
        Kratka *aktualna = lista_otwarta[0];
        int index_optymalna = 0;

        for(int i=0; i<lista_otwarta.size(); i++)
        {
            if(lista_otwarta[i]->f < lista_otwarta[index_optymalna]->f)
            {
                index_optymalna=i;
            }
        }

        aktualna = lista_otwarta[index_optymalna];
        lista_otwarta.erase(lista_otwarta.begin() + index_optymalna);
        lista_zamknieta.push_back(aktualna);

        if(aktualna == cel)
        {
            cout<<"gz";
            break;
        }

        vector<Kratka*>neighbors=add_neighbor(aktualna,mapa);

        for(int i=0; i<neighbors.size(); i++)
        {
            if(find(lista_zamknieta.begin(), lista_zamknieta.end(), neighbors[i])!=lista_zamknieta.end())
            {
                continue;
            }

            neighbors[i]->g = aktualna->g + 1;
            neighbors[i]->h = calculate_h(neighbors[i],cel);
            neighbors[i]->f = neighbors[i]->g + neighbors[i]->h;


            if (find(lista_otwarta.begin(), lista_otwarta.end(), neighbors[i]) != lista_otwarta.end())
            {
                continue;
            }

            lista_otwarta.push_back(neighbors[i]);
        }
    }

    return 0;
}

