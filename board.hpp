#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <string>

namespace snake{

    class Board
    {
    public:
        // 보드 생성자 호출.
        Board();

        // 스테이지 넘버로 보드 초기화.
        void initializeBoard(int num);

        // Board 특정 위치에 문자 추가
        void addChar(int y, int x, char ch);

        // Board의 특정 위치 문자 불러오기
        char getChar(int y, int x);

        // 입력 값 받기.
        char getInput();

        // 스테이지 번호 받기.
        int getStageNum();

        // 게임 속도 조절하기
        void setTimeout(int tick);

        // Board를 새로고침
        void refreshBoard();

        // item 배치 가능한 좌표 찾기
        void searchItemPos(int &y, int &x);

        // 워프 배치 가능한 좌표 찾기
        void searchWarpPos(int &y, int &x);

        // 스코어 보드와 미션 보드에 점수 기록하기
        void recording(int size, int appleS, int TrapS, int warpS, bool sizeC, bool appleC, bool TrapC, bool warpC);

        // 내 화면 상의 창의 가로길이, 세로길이 받기.
        int getxMax();
        int getyMax();

        int getSpeed();
        void setSpeed(int S);

    private:
        int gameSpeed;
        int xMax, yMax;
        WINDOW *gameBoard;
        int stageNum;
        WINDOW *scoreBoard;
        WINDOW *missionBoard;
    };
}
