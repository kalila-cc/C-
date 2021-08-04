#include <iostream>
#include <queue>
#include <cstdio>
#include <ctime>
#include <windows.h>
using namespace std;

// 隐藏光标的函数
void hideCur() {
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
// 传入 x，y 坐标，设置光标位置
void setCur(int x, int y) {
	COORD setCur; //WINDOWS API中的结构体，表示字符在控制台上的坐标
	HANDLE hOutput;
	setCur.X = x;
	setCur.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);  //调用函数返回一个句柄对象
	SetConsoleCursorPosition(hOutput, setCur);	//设置光标位置
}
// 方向类，每次按下方向控制能够映射成一个 Direction 值
enum Direction {    
    Left, Right, Up, Down
};
// 位置类
struct Position {
    int x, y;
    Position(){}
    // 带坐标的初始化
    Position(int x, int y): x(x), y(y){}
    // 判断两个位置是否不相等
    bool operator!=(Position &position) {
        return position.x != x || position.y != y;
    }
    // 判断两个位置是否相等
    bool operator==(Position &position) {
        return position.x == x && position.y == y;
    }
    // 将当前的位置向某个方向移动，对应的正是坐标的加减，该函数会改变坐标位置，无返回值
    void move(Direction direction) {
        switch (direction) {
            case Left:      y--;    break;
            case Right:     y++;    break;
            case Up:        x--;    break;
            case Down:      x++;    break;
        }
    }
    // 预测当前位置在向某个方向移动后的新位置，该函数不会真的移动其位置，只是返回预测位置
    Position nextPosition(Direction direction) {
        switch (direction) {
            case Left:      return Position(x, y - 1);
            case Right:     return Position(x, y + 1);
            case Up:        return Position(x - 1, y);
            case Down:      return Position(x + 1, y);
            default:        return Position(0, 0);
        }
    }
};
// 贪吃蛇类
class GluttonousSnake {
private:
    // 二维场景
    char **board;
    // row：行，col：列，bean_num：豆子数量，snake_len：蛇的长度，score：分数
    int row, col, bean_num, snake_len, score;
    //  head： 蛇头位置，tail：蛇尾位置，*beans：多个豆子的位置
    Position head, tail;
    // 当前蛇头移动的方向
    Direction curDir;
    // 每次操作的方向收集起来组成队列，最新的操作在队尾
    queue<Direction> path;
    // _empty：场地的空地，_head：头部的符号，_tail·：尾部的符号，_bean：豆子的符号
    char _empty, _head, _body, _bean;
public:
    // 通过行、列、豆子数、初始蛇长度进行初始化
    GluttonousSnake(int row, int col, int bean_num, int snake_len):
    row(row), col(col), bean_num(bean_num), snake_len(snake_len), score(0) {
        // 用于随机数
        srand((unsigned)time(NULL));
        // 设置输出符号
        _empty = ' ';
        _head = 'Q';
        _body = 'o';
        _bean = '*';
        // 生成场地
        board = new char*[row];
        // 初始化场地为空
        for (int i = 0; i < row; i++) {
            board[i] = new char[col];
            for (int j = 0; j < col; j++)
                board[i][j] = _empty;
        }
    }
    // 释放场地的存储空间
    ~GluttonousSnake() {
        for (int i = 0; i < row; i++)
            delete[] board[i];
        delete[] board;
    }
    // 外部调用的函数接口，外部无法调用往后的所有私有函数
    void run() {
        // 初始化游戏
        init();
        // 展示当前的场地情况
        displayBoard();
        while (true) {
            // motionMonitor() 监控用户输入的方向，并将方向存入 curDir
            motionMonitor();
            // 当输入的位置不合法，游戏结束
            if (!isValid(head.nextPosition(curDir)))
                break;
            // 通过传入的方向更新场地
            updateBoard(curDir);
        }
        // 游戏结束
        setCur(0, row + 5);
        cout << "You have died!" << endl;
        system("pause");
    }
private:
    // 初始化函数
    void init() {
        // 初始化给定长度的蛇身蛇身
        createSnake();
        // 初始化给定数量的豆子
        for (int i = 0; i < bean_num; i++)
            createBean();
        // 隐藏光标
        hideCur();
    }
    // 判断该位置是否能够使用的函数
    bool isValid(Position position) {
        // 坐标记为 x, y 方便操作
        int x = position.x, y = position.y;
        // 超过场地范围，不能使用
        if (x < 0 || x >= row || y < 0 || y >= col)
            return false;
        // 碰到身体且不是尾部，则判断为非法
        if (board[x][y] == _body && position != tail)
            return false;
        // 以上条件都不满足，则判断位置是合法的
        return true;
    }
    // 判断当前位置是否是豆子的函数
    bool isBean(Position position) {
        return board[position.x][position.y] == _bean;
    }
    // 创建给定长度的蛇身的函数
    void createSnake() {
        // 随机初始化尾巴的位置在中央区域
        tail.x = rand() % row, tail.y = rand() % col;
        while (tail.x < row / 3 || tail.x > 2 * row / 3 || tail.y < col / 3 || tail.y > 2 * col / 3)
            tail.x = rand() % row, tail.y = rand() % col;
        // 把场地中的尾巴的位置更新为 _body 符号
        board[tail.x][tail.y] = _body;
        // 创建新的坐标，并初始化为尾巴所在的位置
        Position now = tail;
        // 生成剩余 (snake_len - 1) 长度的蛇身
        for (int i = 1; i < snake_len; i++) {
            // toDirection() 将 [0, 3] 中的四个的数字映射成四个方向
            curDir = toDirection(rand() % 4);
            // 若 now 在向该方向移动后的位置不合法，就重新生成方向
            while (!isValid(now.nextPosition(curDir)))
                curDir = toDirection(rand() % 4);
            // 位置合法，推入操作方向队列的队尾
            path.push(curDir);
            // 将 now 向该方向移动
            now.move(curDir);
            // 更新场地的对应位置的符号为 _body
            board[now.x][now.y] = _body;
        }
        // 生成完毕，还需要一个蛇头，将 now 的位置记为蛇的头部
        head = now;
        // 更新对应位置为蛇头
        board[head.x][head.y] = _head;
    }
    // 生成一个豆子的函数
    void createBean() {
        // 随机生成一对坐标
        int x = rand() % row;
        int y = rand() % col;
        // 豆子的位置必须是 _empty 才行，不断循环直至找到空地
        while (board[x][y] != _empty)
            x = rand() % row, y = rand() % col;
        // 将找到的空地更新为豆子
        setCur(3 * y + 2, x + 1);
        cout << _bean;
        board[x][y] = _bean;
    }
    // 通过传入的方向，移动蛇头，并更新场地的函数
    void updateBoard(Direction direction) {
        // 获取头部即将到达的位置并判断是否有豆子
        bool hasBean = isBean(head.nextPosition(direction));
        // 头部将会被身体覆盖
        setCur(3 * head.y + 2, head.x + 1);
        cout << _body;
        board[head.x][head.y] = _body;
        // 移动头部
        head.move(direction);
        // 将新的头部的位置更新为头部符号
        setCur(3 * head.y + 2, head.x + 1);
        cout << _head;
        board[head.x][head.y] = _head;
        // 将方向推入操作方向队列的队尾
        path.push(direction);
        // 若前方不存在豆子，置尾巴所在处为空地，并移动尾巴，同时将最早的操作出队
        if (!hasBean) {
            setCur(3 * tail.y + 2, tail.x + 1);
            cout << _empty;
            board[tail.x][tail.y] = _empty;
            tail.move(path.front());
            path.pop();
        // 否则，吃到豆子，可令蛇身变长，尾部不做处理，等效于蛇身变长，重新增加一个豆子
        } else {
            score += 100;
            setCur(0, row + 2);
            printf("score: %8d", score);
            createBean();
        }
    }
    // 打印场地的函数
    void displayBoard() {
        // 清屏
        system("cls");
        // 通过一定的数学运算来输出一定长度的边界线，just for 美观
        for (int i = (3 * col - 21) / 2; i > 0; i--)
            cout << '-';
        cout << " Gluttonous Snake Game ";
        for (int i = (3 * col - 21) / 2; i > 0; i--)
            cout << '-';
        if (!(col % 2))
            cout << '-';
        cout << endl;
        // 画出每一行的内容，最左最右则分别是一堵墙
        for (int r = 0; r < row; r++) {
            cout << '|';
            for (int c = 0; c < col; c++)
                cout << ' ' << board[r][c] << ' ';
            cout << '|' << endl;
        }
        // 依然是分界线
        for (int i = 3 * col + 2; i > 0; i--)
            cout << '-';
        cout << endl;
        // 正式开始之前先暂停
        setCur(0, row + 2);
        cout << "Press any key to start..." << endl;
        system("pause >> nul");
        setCur(0, row + 2);
        cout << "                         " << endl;
        setCur(0, row + 2);
        printf("score: %8d", score);
    }
    // 将 [0, 3] 的数字映射成方向，在随机生成蛇身时候会用到
    Direction toDirection(int code) {
        switch (code) {
            case 0:     return Left;
            case 1:     return Right;
            case 2:     return Up;
            case 3:     return Down;
            default:    return curDir;
        }
    }
    // 监控用户输入的方向的函数，若用户未按下方向键则维持原方向，按下空格可暂停
    Direction motionMonitor() {
        Sleep(200);
        if ((GetAsyncKeyState(VK_UP) && curDir != Down) || ((GetAsyncKeyState('W') && curDir != Down)))
            curDir = Up;
        if ((GetAsyncKeyState(VK_DOWN) && curDir != Up) || (GetAsyncKeyState('S') && curDir != Up))
            curDir = Down;
        if ((GetAsyncKeyState(VK_LEFT) && curDir != Right) || (GetAsyncKeyState('A') && curDir != Right))
            curDir = Left;
        if ((GetAsyncKeyState(VK_RIGHT) && curDir != Left) || (GetAsyncKeyState('D') && curDir != Left))
            curDir = Right;
        if (GetAsyncKeyState(VK_SPACE))
            system("pause >> nul");
        return curDir;
    }
};
// 主函数
int main() {
    // 定义长、宽、豆子数、蛇身长度
    int row, col, bean_num, snake_len;
    // 自定义参数的值
    row = 15;
    col = 15;
    bean_num = 5;
    snake_len = 3;
    // 传入贪吃蛇类的构造函数，创建游戏实例
    GluttonousSnake gs(row, col, bean_num, snake_len);
    // 通过 run() 函数接口来运行游戏
    gs.run();
    return 0;
}