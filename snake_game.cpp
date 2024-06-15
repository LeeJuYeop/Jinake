#include "snake_game.hpp"
#include <chrono>

namespace snake{
    // 생성자가 호출될 시 초기화하는 로직, 기본적으로 stage 번호는 0(1단계)로 시작
    // appleScore와 trapScore는 0으로 초기화
    SnakeGame::SnakeGame()
    {
        initialize(0);
        appleScore = 0;
        trapScore = 0;
        warpScore = 0;
    }

    // 소멸자가 호출될 시 진행하는 로직
    SnakeGame::~SnakeGame()
    {
        delete nextPosition;
        delete apple;
        delete trap;
        delete speed;
        delete warp1;
        delete warp2;
    }

    // 게임 시작할 시(= 생성자 호출 될 시) 진행하는 로직
    void SnakeGame::initialize(int stageNum)
    {   
        srand(time(nullptr));

        appleScore = 0;
        trapScore = 0;
        warpScore = 0;
        missionClear = false;
        sizeClear = false;
        appleClear = false;
        trapClear = false;
        warpClear = false;
        // 게임 창을 stageNum에 따라 초기화
        board.initializeBoard(stageNum);

        //게임을 시작하면 snake의 몸통 위치를 좌측상단에 놓는다
        snake.initialize();

        // 게임 창에 초기화 된 snake 위치 표시한다
        board.addChar(1, 4, '%');
        board.addChar(1, 3, '#');
        board.addChar(1, 2, '#');
        board.addChar(1, 1, '#');

        // 사과랑 trap 아이템을 생성.
        nextPosition = nullptr;
        createApple();
        createTrap();
        createSpeed();
        createWarp();
    }

    // 사과 아이템을 생성하는 함수
    void SnakeGame::createApple()
    {
        int y, x;

        // 아이템이 생성될 수 있는 위치 가져오기
        board.searchItemPos(y, x);

        apple = new Apple(y, x);
        // 사과 아이템이 생성될 위치에다가 'A' addChar
        board.addChar(y, x, 'A');
    }

    // trap 아이템을 만드는 함수
    void SnakeGame::createTrap()
    {
        int y, x;

        // 아이템이 생성될 수 있는 위치 get하기
        board.searchItemPos(y, x);

        trap = new Trap(y, x);
        // trap 아이템이 생성될 위치에다가 'T' addChar
        board.addChar(y, x, 'T');
    }

    // 스핏드 아이템을 만드는 함수
    void SnakeGame::createSpeed() {
        int y, x;
        board.searchItemPos(y, x);
        speed = new Speed(y, x);
        board.addChar(y, x, 'S');
    }

    // 워프 컴포넌트를 만드는 함수
    void SnakeGame::createWarp()
    {
        int y, x;

        board.searchWarpPos(y, x);
        warp1 = new Warp(y, x, board.getChar(y, x));
        board.addChar(y, x, 'W');

        board.searchWarpPos(y, x);
        warp2 = new Warp(y, x, board.getChar(y, x));
        board.addChar(y, x, 'W');
    }

    // 입력 받은 값에 따라 방향을 설정하는 함수
    void SnakeGame::fetchUserInput()
    {
        char input = board.getInput();

        switch (input)
        {
            // w a s d 키로 뱀 움직이기
            // setDirection함수는 은 현재 진행 방향과 반대 방향 키를 누르면 false 반환하고 그 외에는 true 반환한다
            case 'w':
                if (!snake.setDirection(up)) { game_over = true; }
                break;
            case 's':
                if (!snake.setDirection(down)) { game_over = true; }
                break;
            case 'd':
                if (!snake.setDirection(right)) { game_over = true; }
                break;
            case 'a':
                if (!snake.setDirection(left)) { game_over = true; }
                break;
        }
    }

    // 게임을 플레이하는 동안 진행하는 로직
    void SnakeGame::runGame()
    {
        // 마지막 스테이지(=4단계)라면 게임 오버로 간주
        if (getStage() == 4)
        {   
            game_over = true;
        }
        // next는 snake가 다음 어디로 가야할지 그 위치의 값을 가진 SnakeSegment이다
        SnakeSegment next = snake.nexthead();

        if (board.getChar(next.getY(), next.getX()) == 'W')
        {
            if ((next.getY() == warp1->getY()) && (next.getX() == warp1->getX()))
            {
                checkWarp(next, warp2);
            }
            else if ((next.getY() == warp2->getY()) && (next.getX() == warp2->getX()))
            {
                checkWarp(next, warp1);
            }
            controlNext(next);
        }
        else
            // next 라는 SnakeSegment를 가지고 뱀을 조종
            controlNext(next);
        
        if((nextPosition!=nullptr)&&(board.getChar(nextPosition->getY(), nextPosition->getX()) !='#')&& (board.getChar(nextPosition->getY(), nextPosition->getX()) !='%'))
            endWarp();

        // 만약 사과나 trap 아이템이 nullptr 상태라면 화면에 다시 생성
        if (apple == nullptr)
        {
            createApple();
        }

        if (trap == nullptr)
        {
            createTrap();
        }

        if (speed == nullptr)
        {
            createSpeed();
        }

        if ((warp1 == nullptr) && (warp2 == nullptr))
        {
            createWarp();
        }

        // 미션들 조건 맞으면 클리어로 바꾸기
        if (snake.getSize()>=10){sizeClear = true;}
        if (appleScore>=7){appleClear = true;}
        if (trapScore>=5){trapClear = true;}
        if (warpScore>=3){warpClear = true;}

        if (sizeClear&&appleClear&&trapClear&&warpClear){missionClear = true;}

        // 모든 미션 클리어 시 다음 스테이지로 넘어가기.
        if (missionClear){
            // 현재 게임의 stage 번호를 num에 저장
            int num = getStage();

            // 다음 스테이지로 넘어가야하므로 ++num 한 것
            initialize(++num);
            
        }

        // 만약 snake의 몸 길이가 3 미만이라면 게임 오버로 간주
        if (snake.getSize() < 3)
        {
            game_over = true;
        }
    }

    // 다음으로 나아갈 위치에 대한 로직
    void SnakeGame::controlNext(SnakeSegment next)
    {   
        int nextRow = next.getY();
        int nextCol = next.getX();

        // 만약 다음으로 나아갈 위치가 ' '이라면
        if (board.getChar(nextRow, nextCol) == ' ')
        {   
            // snake의 꼬리 위치에 있는 icon을 ' '로 설정
            board.addChar(snake.tail().getY(), snake.tail().getX(), ' ');
            // snake의 꼬리 부분을 없앤다
            snake.removeBody();

            // snake의 머리 부분의 icon을 '#'로 설정
            snake.head().setIcon('#');
            // 게임 창에도 똑같이 반영
            board.addChar(snake.head().getY(), snake.head().getX(), '#');
            // snake의 머리 부분을 next로 설정
            snake.addBody(next);
            // snake의 머리 부분의 icon을 '%'로 설정
            snake.head().setIcon('%');
            // 게임 창에도 똑같이 반영
            board.addChar(snake.head().getY(), snake.head().getX(), '%');
        }

        // 만약 다음으로 나아갈 위치가 사과 아이템('A')라면
        else if (board.getChar(nextRow, nextCol) == 'A')
        {
            // 사과 아이템을 먹는 함수 실행
            eatApple();
            appleScore++;


            // 꼬리를 없애는 로직을 생략하고 앞으로 나아가는 로직을 진행
            snake.head().setIcon('#');
            board.addChar(snake.head().getY(), snake.head().getX(), '#');

            snake.addBody(next);
            snake.head().setIcon('%');
            board.addChar(snake.head().getY(), snake.head().getX(), '%');
        }

        // trap아이템을 먹는다면
        else if (board.getChar(nextRow, nextCol) == 'T')
        {  
            // trap 아이템을 먹는 함수 실행
            eatTrap();
            trapScore++;

            // 꼬리를 한번 더 없애는 로직을 진행한 후 앞으로 나아가는 로직을 진행
            board.addChar(snake.tail().getY(), snake.tail().getX(), ' ');
            snake.removeBody();

            board.addChar(snake.tail().getY(), snake.tail().getX(), ' ');
            snake.removeBody();

            snake.head().setIcon('#');
            board.addChar(snake.head().getY(), snake.head().getX(), '#');

            snake.addBody(next);
            snake.head().setIcon('%');
            board.addChar(snake.head().getY(), snake.head().getX(), '%');
        }

        else if (board.getChar(nextRow, nextCol) == 'S') {
            eatSpeed();
            
            int curSpeed = board.getSpeed();
            int sArr[10] = {75, 100, 125, 150, 175, 200, 225, 250, 275, 300};
            int newSpeed = sArr[rand() % 10];
            while (curSpeed == newSpeed) { newSpeed = sArr[rand() % 10]; }
            board.setSpeed(newSpeed);
        }

        // 만약 다음으로 나아갈 위치가 벽이라면 게임 오버로 간주, 자신의 몸통을 만나도 게임 오버
        else
        {
            game_over = true;
        }
    }

    // 사과아이템을 없애는(=먹는) 함수
    void SnakeGame::eatApple()
    {   
        //게임 화면에 기존의 사과 아이템 위치에 빈칸 addChar
        board.addChar(apple->getY(), apple->getX(), ' ');

        // 동적할당 했었던 apple 객체를 없애고 apple을 nullptr로 설정
        delete apple;
        apple = nullptr;

    }

    // trap 아이템을 없애는(=먹는) 함수
    void SnakeGame::eatTrap()
    {   
        // 게임 화면에 기존의 trap 위치에다가 빈칸 addChar
        board.addChar(trap->getY(), trap->getX(), ' ');

        // 동적할당 했었던 trap 객체를 없애고 trap을 nullptr로 하자
        delete trap;
        trap = nullptr;
    }

    void SnakeGame::eatSpeed() {
        board.addChar(speed->getY(), speed->getX(), ' ');
        delete speed;
        speed = nullptr;
    }

    // stage 번호에 따라서 게임 창을 초기화하는 함수
    void SnakeGame::setStage(int stageNum)
    {
        board.initializeBoard(stageNum);
    }

    // 현재 게임이 스테이지 몇인지 get하는 함수
    int SnakeGame::getStage()
    {
        return board.getStageNum();
    }

    // 게임 오버 여부 확인
    bool SnakeGame::checkOver()
    {
        return game_over;
    }

    // 게임 새로고침
    void SnakeGame::redraw()
    {   
        board.recording(snake.getSize(), getAppleScore(), getTrapScore(), warpScore,
        sizeClear, appleClear, trapClear, warpClear);
        board.refreshBoard();
    }

    //아이템 위치를 화면에서 바꿔주는 함수 main에서 생성주기 결정
    void SnakeGame::updateItem()
    {
        // 사과 아이템이 nullptr 상태라면 화면에 다시 생성
        if (apple != nullptr) {
            eatApple();
            createApple();
        }
        // trap 아이템이 nullptr 상태라면 화면에 다시 생성
        if (trap != nullptr) {
            eatTrap();
            createTrap();
        }
    }

    // appleScore, trapScore 반환
    int SnakeGame::getAppleScore(){
        return appleScore;
    }
    int SnakeGame::getTrapScore(){
        return trapScore;
    }
    // xMax, yMax board에서 가져오는 함수. snake_game에서 얘 호출함.
    int SnakeGame::thisisxMax(){
        return board.getxMax();
    }
    int SnakeGame::thisisyMax(){
        return board.getyMax();
    }

    void SnakeGame::endWarp()
    {
        board.addChar(warp1->getY(), warp1->getX(), warp1->getExitIcon());
        delete warp1;
        warp1=nullptr;

        board.addChar(warp2->getY(), warp2->getX(), warp2->getExitIcon());
        delete warp2;
        warp2=nullptr;

        warpScore++;
        delete nextPosition;
        nextPosition=nullptr;
    }
    
    void SnakeGame::checkWarp(SnakeSegment& next, Warp *warp)
    {
        int dDir[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        Direction dirArr[4] = {up, right, down, left};
        Direction curDir = snake.getDirection();
        int idx;

        for (idx=0; idx<4; idx++)
        {
            if (dirArr[idx] == curDir) { break; }
        }

        for (int i=0; i<4; i++)
        {
            idx = (idx + i) % 4;
            int tmpY = warp->getY() + dDir[idx][0];
            int tmpX = warp->getX() + dDir[idx][1];

            // 멥을 벗어나면 continue
            if((tmpY<0)|| (tmpX<0)|| (tmpY>20) ||(tmpX>38))
                continue;
            // 만약 벽이거나 뱀의 몸통이 있다면 continue
            if(board.getChar(tmpY, tmpX)=='+' || board.getChar(tmpY, tmpX)=='#')
                continue;

            snake.setD_warp(dirArr[idx]);
            next = SnakeSegment(tmpY, tmpX);
            nextPosition= new SnakeSegment(next);
            break;
        }
    }

    void deleteAll(Apple* a, Trap*b, Warp* w1, Warp* w2) {
        delete a;
        delete b;
        delete w1;
        delete w2;
    }
}
