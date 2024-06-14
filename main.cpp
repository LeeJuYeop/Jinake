#include <ncurses.h>
#include "snake_game.hpp"
#include <chrono>
#include <thread>

using namespace snake;

int main()
{   
    // ncurses init 및 초기화. 입력된 키 미표시. 커서 미표시.
    initscr();
    refresh();
    noecho();
    curs_set(0);

    SnakeGame game;     // SnakeGame 객체 생성. 게임 시작.

    // 시작 시간 구하기. (초마다 아이템 업데이트를 위해 필요.)
    auto start_time = std::chrono::steady_clock::now();
    // =============================================

    // 게임 오버가 될 때까지
    while (!game.checkOver())
    {   
        // fetchUserInput :: 입력 값으로 상태를 조정할 수 있음.
        game.fetchUserInput();

        // runGame :: 게임 중에 실행되야할 코드들 집합.
        game.runGame();

        // 새로고침(창에 표시)
        game.redraw();

        //현재 시간 구함 (초마다 아이템 업데이트를 위해 필요.)
        auto now = std::chrono::steady_clock::now();

        // 시작 시간부터 얼마나 흘렀는지 저장 (초마다 아이템 업데이트를 위해 필요.)
        auto mytime= std::chrono::duration_cast<std::chrono::seconds>(now-start_time).count();
        
        // 7초마다 아이템 업데이트.
        if(mytime >= 7){
            game.updateItem();
            start_time=now;
        }
    }



    // 게임 오버로 while문 빠져나왔을 때 스테이지가 4면 clear(game clear)이며 아니면 실패.(game over)
    if (game.getStage() == 4)
    {
        printw("%d %d", game.thisisyMax()/2, (game.thisisxMax()/2)-5);
        mvprintw(game.thisisyMax()/2, (game.thisisxMax()/2)-5, "Game Clear!!!");
    }
    else
    {
        mvprintw(game.thisisyMax()/2, (game.thisisxMax()/2)-5, "Game Over!!!");
    }
    
    getch();        // 새로운 입력이 있을 떄까지 대기(게임이 다 끝나고 종료 메시지가 출력되고 대기중인 상태.)
    endwin();       // 아무 키나 누르면 키를 누르면 게임 종료.
}
