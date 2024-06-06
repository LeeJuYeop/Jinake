#pragma once
#include <ncurses.h>
#include "snake.hpp"
#include "board.hpp"
#include "item.hpp"

namespace snake{
class SnakeGame
{
public:
    // 생성자 호출 시 게임 창을 stage 0번으로 초기화
    SnakeGame();

    // 소멸자 호출 시 동적으로 할당했던 변수 delete
    ~SnakeGame();

    // 게임 시작할 시(= 생성자 호출 될 시) 진행하는 로직
    void initialize(int stageNum);

    // 사과 만드는 함수
    void createApple();

    // 폭탄 만드는 함수
    void createBomb();

    // 스피드 만드는 함수
    void createSpeed();

    // 워프 만드는 함수
    void createWarp();
    void endWarp();
    void checkWarp(SnakePiece& next, Warp *warp);

    // 입력 받은 값에 따라 작동을 달리하는 로직
    void getInputState();

    // 게임을 플레이하는 동안 진행하는 로직
    void playingState();

    // 뱀이 다음 위치로 어떻게 나아가야하는지 icon에 따라 조종하는 함수
    void handleNext(SnakePiece next);

    // 사과를 없애는(=먹는) 함수
    void eatApple();

    // 폭탄를 없애는(=먹는) 함수
    void eatBomb();

    // 스피드를 없애는(=먹는) 함수
    void eatSpeed();

    // stage 번호에 따라서 게임 창을 초기화하는 함수
    void setStage(int stageNum);

    // 현재 게임이 스테이지 몇인지 get하는 함수
    int getStageNum();

    // 게임 오버 됐는지 확인하는 함수
    bool isOver();

    // 게임 새로고침
    void redraw();

    //item update per 7seconds
    void ItemUpdate();
    // =============================================

    // 사과 먹은 갯수, 폭탄 먹은 갯수 리턴
    int getAppleScore();
    int getBombScore();

    //yMax, xMax main에 넘겨주기 위한 함수
    int thisisxMax();
    int thisisyMax();

    void deleteAll(Apple* a, Bomb* b, Warp* w1, Warp* w2);

private:
    SnakePiece *tmp_next;
    Board board;
    Snake snake;

    bool game_over = false;

    Apple *apple;
    Bomb *bomb;
    Speed *speed;
    Warp *warp1;
    Warp *warp2;

    int appleScore;
    int bombScore;
    int warpScore;

    bool missionClear;
    bool sizeClear;
    bool appleClear;
    bool bombClear;
    bool warpClear;

    };
}
