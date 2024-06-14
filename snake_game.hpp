#pragma once
#include <ncurses.h>
#include "snake.hpp"
#include "board.hpp"
#include "item.hpp"

namespace snake{
class SnakeGame
{
public:
    // 생성자가 호출될 시 초기화하는 로직, 기본적으로 stage 번호는 0(1단계)로 시작
    SnakeGame();

    // 소멸자가 호출될 시 진행하는 로직
    ~SnakeGame();

    // 게임 시작할 시(= 생성자 호출 될 시) 실행
    void initialize(int stageNum);

    // 사과 아이템을 생성하는 함수
    void createApple();

    // 폭탄 아이템을 생성하는 함수
    void createBomb();

    // 스피드 아이템을 생성하는 함수
    void createSpeed();

    // 워프 컴포넌트를 생성하는 함수
    void createWarp();
    void endWarp();
    void checkWarp(SnakePiece& next, Warp *warp);

    // 게임을 플레이하는 동안 사용자로부터 입력을 받는 함수
    void getInputState();

    // 게임을 플레이하는 동안 진행하는 함수
    void playingState();

    // 뱀이 다음 위치로 어떻게 나아가야하는지 조종하는 함수

    void handleNext(SnakePiece next);

    // 사과 아이템을 먹을시 화면에서 없애주는 함수
    void eatApple();

    // 폭탄 아이템을 먹을시 화면에서 없애주는 함수
    void eatBomb();

    // 스피드 아이템을 먹을시 화면에서 없애주는 함수
    void eatSpeed();

    // stage 번호에 따라서 게임 플레이 화면을 그려주는 함수
    void setStage(int stageNum);

    // 현재 게임이 스테이지 번호를 리턴하는 함수
    int getStage();

    // 게임 오버 여부를 리턴하는 함수
    bool checkOver();

    // 게임 새로고침
    void redraw();

    //아이템 위치를 화면에서 바꿔주는 함수 main에서 생성주기 결정
    void ItemUpdate();

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
