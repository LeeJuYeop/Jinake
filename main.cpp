#include <ncurses.h>
#include "snake_game.hpp"
#include <chrono>
#include <thread>

using namespace snake;

int main()
{   
    // ncurses 시작
    initscr();
    // 일단 초기화 진행
    refresh();
    // 입력된 키 표시 안함
    noecho();
    // 커서 안보이게 설정
    curs_set(0);

    // 게임 스타트
    SnakeGame game;

    // ==혜인님 코드 (일정 시간 지나면 아이템 업데이트 기능)==
    // 시작 시간 구함
    auto start_time = std::chrono::steady_clock::now();
    // =============================================

    // 게임 오버가 될 때까지
    while (!game.isOver())
    {   
        // 게임에서 입력 값에 따라 바뀌는 로직
        game.getInputState();

        // 게임하는 중 진행되는 로직
        game.playingState();

        // 게임 새로고침(= 실제로 창에 표시하는 단계)
        game.redraw();

        // ==혜인님 코드 (일정 시간 지나면 아이템 업데이트 기능)==
        //특정 시간 구함
        auto end_time = std::chrono::steady_clock::now();

        //시간 계산(특정 시간 - 시작 시간)
        auto temp= std::chrono::duration_cast<std::chrono::seconds>(end_time-start_time).count();
        
        //7초가 됐을 때
        if(temp>=7){
            game.ItemUpdate();
            start_time=end_time;
        }
        // ==============================================
    }

    // 게임 오버 되어서 빠져 나왔을 때 스테이지 넘버가 4라는 것은 클리어 했다는 뜻
    if (game.getStageNum() == 4)
    {
        printw("%d %d", game.thisisyMax()/2, (game.thisisxMax()/2)-5);
        mvprintw(game.thisisyMax()/2, (game.thisisxMax()/2)-5, "Game Clear!!!");
    }
    // 그게 아니라면 진짜로 게임 오버라는 뜻
    else
    {
        mvprintw(game.thisisyMax()/2, (game.thisisxMax()/2)-5, "Game Over!!!");
    }
    
    // 입력 값 받을 때 까지 대기
    getch();
    // ncurses 종료
    endwin();
}