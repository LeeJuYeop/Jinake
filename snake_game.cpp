#include "snake_game.hpp"
#include <chrono>

namespace snake{
    // 생성자 호출 시 게임 창을 stage 0번으로 초기화
    // appleScore와 bombScore 0으로 초기화.
    SnakeGame::SnakeGame()
    {
        initialize(0);
        appleScore = 0;
        bombScore = 0;
        warpScore = 0;
    }

    // 소멸자 호출 시 동적으로 할당했던 변수 delete
    SnakeGame::~SnakeGame()
    {
        delete tmp_next;
        delete apple;
        delete bomb;
        delete speed;
        delete warp1;
        delete warp2;
    }

    // 게임 시작할 시(= 생성자 호출 될 시) 진행하는 로직
    void SnakeGame::initialize(int stageNum)
    {   
        srand(time(nullptr));

        appleScore = 0;
        bombScore = 0;
        warpScore = 0;
        missionClear = false;
        sizeClear = false;
        appleClear = false;
        bombClear = false;
        warpClear = false;
        // 게임 창을 stageNum 번째 stage로 초기화
        board.initialize(stageNum);

        // snake queue를 게임 창 좌측 상단에 ###@ 형태로 초기화
        snake.initialize();

        // 게임 창에 메모리 상으로 초기화 된 snake 위치 표시한다
        board.add(1, 4, '%');
        board.add(1, 3, '#');
        board.add(1, 2, '#');
        board.add(1, 1, '#');

        // 사과랑 폭탄 만든다
        tmp_next = nullptr;
        createApple();
        createBomb();
        createSpeed();
        createWarp();
    }

    // 사과 만드는 함수
    void SnakeGame::createApple()
    {
        int y, x;

        // 아이템이 생성될 수 있는 위치 get하기
        board.getItemPos(y, x);

        apple = new Apple(y, x);

        // 게임 창의 메모리 상으로 사과 A 추가
        board.add(y, x, 'A');
    }

    // 폭탄 만드는 함수
    void SnakeGame::createBomb()
    {
        int y, x;

        // 아이템이 생성될 수 있는 위치 get하기
        board.getItemPos(y, x);

        bomb = new Bomb(y, x);

        // 게임 창의 메모리 상으로 사과 B 추가
        board.add(y, x, 'B');
    }

    // 스피드 만드는 함수
    void SnakeGame::createSpeed() {
        int y, x;
        board.getItemPos(y, x);
        speed = new Speed(y, x);
        board.add(y, x, 'S');
    }

    // 워프 만드는 함수
    void SnakeGame::createWarp()
    {
        int y, x;

        board.getWarpPos(y, x);
        warp1 = new Warp(y, x, board.getCharAt(y, x));
        board.add(y, x, 'W');

        board.getWarpPos(y, x);
        warp2 = new Warp(y, x, board.getCharAt(y, x));
        board.add(y, x, 'W');
    }

    // 입력 받은 값에 따라 작동을 달리하는 로직
    void SnakeGame::getInputState()
    {
        char input = board.getInput();

        switch (input)
        {
            // w a s d로 뱀 움직이기
            // setDirection(direction)은 현재 방향과 반대 방향 키를 누르면 false 반환, 아니면 true 반환한다
            // 반대 방향 키를 누르면 게임 오버가 되도록 설정
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
            
            // n 누르면 다음 스테이지로 넘어감(디버깅 용)
            case 'n':
                // 현재 게임의 stage 번호를 num에 저장
                int num = getStageNum();

                // 다음 스테이지로 넘어가야하므로 ++num 한 것
                initialize(++num);
                
                break;
        }
    }

    // 게임을 플레이하는 동안 진행하는 로직
    void SnakeGame::playingState()
    {
        // 스테이지가 4라는 것은 스테이지 끝이라는 뜻
        if (getStageNum() == 4)
        {   
            // 게임 오버로 하자
            game_over = true;
        }
        // next는 snake가 다음 어디로 가야할지 그 위치의 값을 가진 SnakePiece이다
        SnakePiece next = snake.nexthead();

        if (board.getCharAt(next.getY(), next.getX()) == 'W')
        {
            if ((next.getY() == warp1->getY()) && (next.getX() == warp1->getX()))
            {
                checkWarp(next, warp2);
            }
            else if ((next.getY() == warp2->getY()) && (next.getX() == warp2->getX()))
            {
                checkWarp(next, warp1);
            }
            handleNext(next);
        }
        else
            // next 라는 SnakePiece를 가지고 뱀을 조종
            handleNext(next);
        
        if((tmp_next!=nullptr)&&(board.getCharAt(tmp_next->getY(), tmp_next->getX()) !='#')&& (board.getCharAt(tmp_next->getY(), tmp_next->getX()) !='%'))
            endWarp();

        // 만약 사과나 폭탄이 nullptr 상태라면 생성해야 한다
        if (apple == nullptr)
        {
            createApple();
        }

        if (bomb == nullptr)
        {
            createBomb();
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
        if (bombScore>=5){bombClear = true;}
        if (warpScore>=3){warpClear = true;}

        if (sizeClear&&appleClear&&bombClear&&warpClear){missionClear = true;}

        // 모든 미션 클리어 시 다음 스테이지로 넘어가기.
        if (missionClear){
            // 현재 게임의 stage 번호를 num에 저장
            int num = getStageNum();

            // 다음 스테이지로 넘어가야하므로 ++num 한 것
            initialize(++num);
            
        }

        // 만약 snake의 몸 길이가 3 미만이라면 게임 오버로 간주
        if (snake.getSize() < 3)
        {
            game_over = true;
        }
    }

    // 뱀이 다음 위치로 어떻게 나아가야하는지 icon에 따라 조종하는 함수
    void SnakeGame::handleNext(SnakePiece next)
    {   
        int nextRow = next.getY();
        int nextCol = next.getX();

        // 만약 다음으로 나아갈 위치가 ' '이라면
        // 이하 "뱀이 앞으로 나아가는 로직" 이라고 칭함
        if (board.getCharAt(nextRow, nextCol) == ' ')
        {   
            // snake의 꼬리 위치에다가 ' ' add 한다
            board.add(snake.tail().getY(), snake.tail().getX(), ' ');
            // snake의 꼬리를 없앤다
            snake.removeBody();

            // 현 상태의 snake의 머리 부분의 icon을 '#'으로 설정
            snake.head().setIcon('#');
            // 게임 창에도 똑같이 snake의 머리 부분 위치를 '#'으로 바꿈(=add)
            board.add(snake.head().getY(), snake.head().getX(), '#');

            // snake에다가 next 추가
            snake.addBody(next);
            // next를 추가한 최종 snake의 머리 부분의 icon을 '@'로 설정
            snake.head().setIcon('%');
            // 게임 창에도 똑같이 반영
            board.add(snake.head().getY(), snake.head().getX(), '%');
        }

        // 사과를 먹는다면
        else if (board.getCharAt(nextRow, nextCol) == 'A')
        {
            // 사과 먹는 함수 실행
            eatApple();
            appleScore++;


            // 뱀이 앞으로 나아가는 로직에서 꼬리 부분을 없애는 과정을 건너뛰면 된다
            snake.head().setIcon('#');
            board.add(snake.head().getY(), snake.head().getX(), '#');

            snake.addBody(next);
            snake.head().setIcon('%');
            board.add(snake.head().getY(), snake.head().getX(), '%');
        }

        // 폭탄을 먹는다면
        else if (board.getCharAt(nextRow, nextCol) == 'B')
        {  
            // 폭탄을 먹는 함수 실행
            eatBomb();
            bombScore++;

            // 꼬리를 한번 더 삭제하고 앞으로 나아가는 로직 진행
            board.add(snake.tail().getY(), snake.tail().getX(), ' ');
            snake.removeBody();

            board.add(snake.tail().getY(), snake.tail().getX(), ' ');
            snake.removeBody();

            snake.head().setIcon('#');
            board.add(snake.head().getY(), snake.head().getX(), '#');

            snake.addBody(next);
            snake.head().setIcon('%');
            board.add(snake.head().getY(), snake.head().getX(), '%');
        }

        else if (board.getCharAt(nextRow, nextCol) == 'S') {
            eatSpeed();
            
            int curSpeed = board.getSpeed();
            int sArr[10] = {75, 100, 125, 150, 175, 200, 225, 250, 275, 300};
            int newSpeed = sArr[rand() % 10];
            while (curSpeed == newSpeed) { newSpeed = sArr[rand() % 10]; }
            board.setSpeed(newSpeed);
        }

        // Wall(= '1')을 만났을 때, 뱀 몸통(=자기 자신)을 만났을 때
        else
        {
            game_over = true;
        }
    }

    // 사과를 없애는(=먹는) 함수
    void SnakeGame::eatApple()
    {   
        // 우선 보드에 기존의 사과 위치에다가 ' ' add한다
        board.add(apple->getY(), apple->getX(), ' ');

        // 동적할당 했었던 apple 없애고 apple을 nullptr로 하자
        delete apple;
        apple = nullptr;

    }

    // 폭탄를 없애는(=먹는) 함수
    void SnakeGame::eatBomb()
    {   
        // 우선 보드에 기존의 폭탄 위치에다가 ' ' add한다
        board.add(bomb->getY(), bomb->getX(), ' ');

        // 동적할당 했었던 bomb 없애고 bomb을 nullptr로 하자
        delete bomb;
        bomb = nullptr;
    }

    void SnakeGame::eatSpeed() {
        board.add(speed->getY(), speed->getX(), ' ');
        delete speed;
        speed = nullptr;
    }

    // stage 번호에 따라서 게임 창을 초기화하는 함수
    void SnakeGame::setStage(int stageNum)
    {
        board.initialize(stageNum);
    }

    // 현재 게임이 스테이지 몇인지 get하는 함수
    int SnakeGame::getStageNum()
    {
        return board.getStageNum();
    }

    // 게임 오버 됐는지 확인하는 함수
    bool SnakeGame::isOver()
    {
        return game_over;
    }

    // 게임 새로고침
    void SnakeGame::redraw()
    {   
        board.recording(snake.getSize(), getAppleScore(), getBombScore(), warpScore,
        sizeClear, appleClear, bombClear, warpClear);
        board.refresh();
    }

    // item update per 7seconds
    void SnakeGame::ItemUpdate()
    {
        if (apple != nullptr) {
            // board.add(apple->getY(), apple->getX(), ' ');
            // delete apple;
            // apple = nullptr;

            // 기존 코드가 eatApple()과 똑같아서 대체함
            eatApple();
            createApple();
        }

        if (bomb != nullptr) {
            // board.add(bomb->getY(), bomb->getX(), ' ');
            // delete bomb;
            // bomb = nullptr;

            // 기존 코드가 eatBomb()과 똑같아서 대체함
            eatBomb();
            createBomb();
        }
    }

    // appleScore, bombScore 반환해주는 함수.
    int SnakeGame::getAppleScore(){
        return appleScore;
    }
    int SnakeGame::getBombScore(){
        return bombScore;
    }
    // xMax, yMax board에서 가져오는 함수. snake_game에서 얘 호출함.
    int SnakeGame::thisisxMax(){
        return board.getxMax();
    }
    int SnakeGame::thisisyMax(){
        return board.getyMax();
    }

    // ==============================================
    void SnakeGame::endWarp()
    {
        board.add(warp1->getY(), warp1->getX(), warp1->getExitIcon());
        delete warp1;
        warp1=nullptr;

        board.add(warp2->getY(), warp2->getX(), warp2->getExitIcon());
        delete warp2;
        warp2=nullptr;

        warpScore++;
        delete tmp_next;
        tmp_next=nullptr;
    }
    
    void SnakeGame::checkWarp(SnakePiece& next, Warp *warp)
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

            //out of map
            if((tmpY<0)|| (tmpX<0)|| (tmpY>20) ||(tmpX>38))
                continue;
            //if(board.stage[board.stageNum][tmp_y][tmp_x]=='$')
            if(board.getCharAt(tmpY, tmpX)=='+' || board.getCharAt(tmpY, tmpX)=='#')
                continue;

            //if find empty
            snake.setD_warp(dirArr[idx]);
            next = SnakePiece(tmpY, tmpX);
            tmp_next= new SnakePiece(next);
            break;
        }
    }

    void deleteAll(Apple* a, Bomb*b, Warp* w1, Warp* w2) {
        delete a;
        delete b;
        delete w1;
        delete w2;
    }
}
