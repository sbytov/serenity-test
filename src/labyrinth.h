#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <sstream>

/*
  Лабиринт.
  Имеется квадратное поле клеток 5х5.
  Каждая клетка имеет штраф (>= 0) за переход в нее.
  Движение начинается из левого верхнего угла в правый нижний угол.
  Перемещатся можно в любую соседнюю клетку (прямо и наискосок).
  Найти самый дешевый путь.
*/
template<int N = 5>
class Labyrinth
{
private:
    struct Cell {
        Cell() = default;
        Cell(int x, int y, double c) : X{ x }, Y{ y }, Cost{ c }
        {}
        bool operator<(const Cell& rhs) const {
            return rhs.Dist < Dist;
        }
        int X;
        int Y;
        double Cost;
        double Dist = std::numeric_limits<double>::max();
        Cell* From = nullptr;
    };
    Cell cells[N][N];
    struct CellComparator {
        bool operator()(const Cell* c1, const Cell* c2) const {
            return c1->Dist < c2->Dist;
        }
    };
    std::priority_queue<Cell*, std::vector<Cell*>, CellComparator> pq;
public:
    Labyrinth(const double fields[N][N]) {
        for (auto i = 0; i < N; i++)
        {
            for (auto j = 0; j < N; j++)
            {
                cells[i][j] = Cell{ j, i, fields[i][j] };
            }
        }
        cells[0][0].Dist = 0;
        pq.push(&cells[0][0]);
        auto relax = [this, fields](int x, int y, Cell& cur) {
            if (x >= 0 && x < N && y >= 0 && y < N)
            {
                auto& adj = cells[y][x];
                if (adj.Cost + cur.Dist < adj.Dist)
                {
                    adj.Dist = adj.Cost + cur.Dist;
                    adj.From = &cur;
                    pq.push(&adj);
                }
            }
        };
        while (!pq.empty())
        {
            auto cell = pq.top();
            pq.pop();
            int x = cell->X, y = cell->Y;
            relax(x + 1, y, *cell);
            relax(x, y + 1, *cell);
            relax(x - 1, y, *cell);
            relax(x, y - 1, *cell);
            relax(x - 1, y - 1, *cell);
            relax(x + 1, y + 1, *cell);
            relax(x + 1, y - 1, *cell);
            relax(x - 1, y + 1, *cell);
        }
    }

    std::vector<Cell> PathTo(int x, int y) {
        if (x >= N || y >= N || x<0 || y<0)
            return std::vector<Cell>{};

        std::stack<Cell> reversedPath;
        Cell* from = cells[x][y].From;
        while (from != nullptr)
        {
            reversedPath.push(*from);
            from = from->From;
        }
        std::vector<Cell> path;
        while (!reversedPath.empty())
        {
            path.push_back(reversedPath.top());
            reversedPath.pop();
        }
        path.push_back(cells[x][y]);
        return path;
    }

    std::string PathAsString(int x, int y) {
        std::ostringstream oss;
        auto path = PathTo(x, y);
        for (auto& c : path)
        {
            oss << c.X << "-" << c.Y << ";";
        }
        return oss.str();
    }
};
