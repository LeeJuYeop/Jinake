#include "snake.hpp"

namespace snake{
    SnakePiece::SnakePiece(int y, int x, char ch) : row(y), col(x), icon(ch) {}

    int SnakePiece::getX()
    {
        return col;
    }

    int SnakePiece::getY()
    {
        return row;
    }

    void SnakePiece::setIcon(char ch)
    {
        icon = ch;
    }
    

    // 기본은 right 방향으로 설정
    Snake::Snake()
    {
        cur_direction = right;
    }

    // 초기화 진행 / snake 큐 다 없애고 ###@ 형태로 추가, 방향은 오른쪽
    void Snake::initialize()
    {
        while (!snake.empty())
        {
            snake.pop();
        }

        snake.push(SnakePiece(1, 1, '#'));
        snake.push(SnakePiece(1, 2, '#'));
        snake.push(SnakePiece(1, 3, '#'));
        snake.push(SnakePiece(1, 4, '%'));

        cur_direction = right;
    }

    void Snake::addBody(SnakePiece piece)
    {
        snake.push(piece);
    }

    void Snake::removeBody()
    {
        snake.pop();
    }

    SnakePiece Snake::tail()
    {
        return snake.front();
    }

    SnakePiece Snake::head()
    {
        return snake.back();
    }

    bool Snake::setDirection(Direction direct)
    {
        if (cur_direction + direct == 3)
        {
            return false;
        }
        cur_direction = direct;
        return true;
    }
    bool Snake::setD_warp(Direction d)
    {
        cur_direction = d;
        return true;
    }
    // 뱀 몸통 길이는 queue 전체 길이에서 머리 부분 뺀 값
    int Snake::getSize()
    {
        return snake.size() - 1;
    }

    Direction Snake::getDirection()
    {
        return cur_direction;
    }

    // 다음으로 나아가야하는 위치 값을 가진 SnakePiece 반환
    SnakePiece Snake::nexthead()
    {
        int row = head().getY();
        int col = head().getX();

        switch (cur_direction)
        {
            case down:
                row++;
                break;
            case up:
                row--;
                break;
            case left:
                col--;
                break;
            case right:
                col++;
                break;
        }

        return SnakePiece(row, col);
    }
}