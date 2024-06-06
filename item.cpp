#include "item.hpp"

namespace snake{
    Item::Item(int y, int x) : row(y), col(x), icon(' ') {}

    int Item::getX()
    {
        return col;
    }

    int Item::getY()
    {
        return row;
    }

    void Item::setIcon(char ch)
    {
        icon = ch;
    }


    Apple::Apple(int y, int x) : Item(y, x)
    {
        icon = 'A';
    }
    
    Bomb::Bomb(int y, int x) : Item(y, x)
    {
        icon = 'B';
    }

    Warp::Warp(int y, int x, char c) : Item(y, x)
    {
        icon = 'W';
        exitIcon = c;
    }

    char Warp::getExitIcon()
    {
        return exitIcon;
    }
}