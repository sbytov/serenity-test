// SerenityTestTasks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <cassert>

#include "polygon.h"
#include "labyrinth.h"
#include "secureLogger.h"

int main()
{
    //1.Понять, является ли многоугольник выпуклым
    assert(isConvexPolygon({ {0,0}, {5,5}, {10,0} }));
    assert(isConvexPolygon({ {0,0}, {-5,-5}, {-10,0} }));
    assert(isConvexPolygon({ {0,0}, {0,5}, {5,5}, {5,0} }) );
    assert(!isConvexPolygon({ {0,0}, {0,5}, {2.5, 2.5}, {5,5}, {5,0} }));
    assert(!isConvexPolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {2.5, 2.5} }));
    assert(isConvexPolygon({ {0,0}, {0,2}, {0,5}, {2,5}, {5,5}, {5,2}, {5,0}, {3,0}}));
    assert(!isConvexPolygon({ {0,0}, {5,5} }));
    assert(!isConvexPolygon({ {0,0} }));
    assert(!isConvexPolygon({ }));

    //2.Понять, принадлежит ли точка многоугольнику
    assert(isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {2.5, 2.5} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {6, 2} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {2, 6} }));
    assert(isPointInsidePolygon({ {0,0}, {-5,5}, {-10,0}, {-5, 1} }));
    assert(!isPointInsidePolygon({ {0,0}, {-5,5}, {-10,0}, {5, 1} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {2.5, 2.5}, {5,5}, {5,0}, {2.5, 3} }));
    assert(isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {0, 0} }));
    assert(isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {2, 0} }));
    assert(isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {0, 2} }));
    assert(isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {5, 2} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {6, 2} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {-1, 2} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {1, 6} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {1, -1} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {6, 0} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {0, 6} }));
    assert(isPointInsidePolygon({ {0,0}, {2,2}, {4,0}, {1,1} }));
    assert(isPointInsidePolygon({ {0,0}, {2,2}, {4,0}, {3,1} }));
    assert(isPointInsidePolygon({ {0,0}, {2,2}, {4,0}, {2,2} }));
    assert(!isPointInsidePolygon({ {0,0}, {2,2}, {4,0}, {3,3} }));
    assert(!isPointInsidePolygon({ {0,0}, {2,2}, {4,0}, {2,3} }));
    assert(!isPointInsidePolygon({ {0,0}, {2,2}, {4,0}, {3,2} }));
    assert(!isPointInsidePolygon({ {0,0}, {0,5}, {5,5}, {5,0}, {4,0}, {4,2}, {1,2}, {1, 0}, {2, 0} }));

    //3.Лабиринт
    double maze1[5][5] = { {2, 7, 7, 1, 5}, {4, 0, 3, 2, 1}, {1, 2, 3, 4, 5}, {5, 4, 3, 2, 1}, {0, 0, 4, 3, 7} };
    auto path = Labyrinth<5>{ maze1 }.PathAsString(4, 4);
    assert(path == "0-0;1-1;2-2;3-3;4-4;");
    double maze2[5][5] = { {0, 0, 0, 0, 0}, {4, 1, 3, 0, 1}, {1, 2, 0, 4, 5}, {5, 0, 3, 2, 1}, {0, 0, 0, 0, 7} };
    path = Labyrinth<5>{ maze2 }.PathAsString(4, 4);
    assert(path == "0-0;1-0;2-0;3-1;2-2;1-3;2-4;3-4;4-4;");
    double maze3[5][5] = { {2, 7, 0, 0, 0}, {0, 7, 0, 7, 0}, {0, 7, 0, 7, 0}, {0, 0, 7, 7, 0}, {0, 7, 7, 7, 7} };
    path = Labyrinth<5>{ maze3}.PathAsString(4, 4);
    assert(path == "0-0;0-1;0-2;1-3;2-2;2-1;3-0;4-1;4-2;4-3;4-4;");

    //4.Защищенная система журналирования
    try
    {
        {
            SecureLogger logger("log.txt", "password", "sessions.bin");
            logger.Log("message1");
            logger.Log("message2");
            logger.Log("message3");
        }
        {
            SecureLogger logger("log.txt", "password", "sessions.bin");
            logger.Log("message4");
            logger.Log("message5");
        }
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what();
        return -1;
    }

}
