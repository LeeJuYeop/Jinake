#pragma once
#include <ncurses.h>

namespace snake {

    class Item {
    public:
        Item(int y = 0, int x = 0);

        int getX();

        int getY();

        void setIcon(char ch);   

    protected:
        int row, col;
        char icon;
    };

    // Item 상속(몸길이 증가)
    class Apple : public Item {
    public:
        Apple(int y = 0, int x = 0);
    };

    // Item 상속(몸길이 감소)
    class Bomb : public Item {
    public:
        Bomb(int y = 0, int x = 0);
    };

    // Item 상속(속도 변화)
    class Speed : public Item {
    public:
        Speed(int y = 0, int x = 0);
    };

    // Item 상속(게이트 기능)
    class Warp : public Item {
    public:
        Warp(int y = 0, int x = 0, char c = ' ');
        char getExitIcon();
        
    private:
        // 'W' 변환 전 기존 char
        char exitIcon;
    };
}
