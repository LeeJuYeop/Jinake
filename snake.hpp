#pragma once
#include <ncurses.h>
#include <queue>

namespace snake{

    // 방향 결정 enum
    enum Direction
    {
        up = 0,
        right = 1,
        down = 3,
        left = 2
    };

    // snake의 몸통을 나타내는 클래스/ 각 몸통은 y x icon을 가지고 있음
    class SnakeSegment
    {
    public:
        // 생성자 y x icon을 받아서 초기화
        SnakeSegment(int y = 0, int x = 0, char ch = '#');

        int getX();

        int getY();

        void setIcon(char ch);

    private:
        int row, col;
        char icon;
    };

    class Snake
    {   
        // snake의 몸통을 저장하는 queue 생성
        std::queue<SnakeSegment> snake;
        // 방향을 저장하는 변수
        Direction cur_direction;

    public:
        // 기본은 오른쪽으로 가는 snake 생성
        Snake();

        // 초기화 함수/ snake의 몸통을 초기화
        void initialize();

        void addBody(SnakeSegment piece);

        void removeBody();

        SnakeSegment tail();

        SnakeSegment head();
        // 방향을 설정하는 함수
        bool setDirection(Direction direct);
        bool setD_warp(Direction d);

        int getSize();

        Direction getDirection();

        // 다음으로 이동할 위치 값을 가진 SnakeSegment를 반환하는 함수
        SnakeSegment nexthead();

    };
}