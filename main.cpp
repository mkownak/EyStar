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
    Kratka* rodzic;

    Kratka(int x, int y) : x(x), y(x), g(0), h(0), f(0), rodzic(nullptr) {}
};

double calculate_g(Kratka *next, Kratka *start)
{
    return sqrt(pow(start->x - next->x,2)) + sqrt(pow(start->y - next->y,2));
}

double calculate_h(Kratka *next, Kratka *cel)
{
    sqrt(pow(cel->x - next->x,2)) + sqrt(pow(cel->y - next->y,2));
}

void show_map(vector<vector<int>>map) {
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            std::cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

vector<vector<int>>load_map(string file_name) {
    vector<vector<int>> map(20,vector<int>(20));
    ifstream grid(file_name);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            grid >> map[i][j];
        }
    }
    return map;
}

int main() {

    vector<std::vector<int>> map = load_map("grid.txt");
    show_map(map);

    return 0;
}
