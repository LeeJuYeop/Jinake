#pragma once
#include <ncurses.h>
#include <queue>

namespace snake{

    // 방향을 각각 다음과 같이 정의함
    enum Direction
    {
        up = 0,
        right = 1,
        down = 3,
        left = 2
    };

    // snake 조각 클래스 / snake 큐는 이 조각들로 이루어짐
    class SnakePiece
    {
    public:
        // 생성자 호출 되면 y x icon 저장함
        SnakePiece(int y = 0, int x = 0, char ch = '#');

        int getX();

        int getY();

        void setIcon(char ch);

    private:
        int row, col;
        char icon;
    };

    class Snake
    {   
        // SnakePiece 타입의 snake 큐 생성
        std::queue<SnakePiece> snake;
        // Direction 타입의 cur_direction 생성
        Direction cur_direction;

    public:
        // 기본은 right 방향으로 설정
        Snake();

        // 초기화 진행 / snake 큐 다 없애고 ###@ 형태로 추가, 방향은 오른쪽
        void initialize();

        void addBody(SnakePiece piece);

        void removeBody();

        SnakePiece tail();

        SnakePiece head();

        bool setDirection(Direction direct);
        bool setD_warp(Direction d);

        // 뱀 몸통 길이는 queue 전체 길이에서 머리 부분 뺀 값
        int getSize();

        Direction getDirection();

        // 다음으로 나아가야하는 위치 값을 가진 SnakePiece 반환
        SnakePiece nexthead();

    };
}