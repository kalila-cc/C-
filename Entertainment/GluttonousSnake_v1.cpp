#include <iostream>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#define pass ;
using namespace std;
enum Direction { 
    Left, Right, Up, Down
};
struct Position {
    int x, y;
    Position(){}
    Position(int x, int y): x(x), y(y){}
    bool operator!=(Position &position) {
        return position.x != x || position.y != y;
    }
    bool operator==(Position &position) {
        return position.x == x && position.y == y;
    }
    void move(Direction direction) {
        switch (direction) {
            case Left:      y--;    break;
            case Right:     y++;    break;
            case Up:        x--;    break;
            case Down:      x++;    break;
        }
    }
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
class GluttonousSnake {
private:
    char **board;
    int row, col, bean_num, snake_len;
    Position head, tail, *beans;
    queue<Direction> path;
    const static char _empty = ' ', _head = 'Q', _body = 'o', _bean = '*';
public:
    GluttonousSnake(int row, int col, int bean_num, int snake_len) : row(row), col(col), bean_num(bean_num), snake_len(snake_len) {
        srand((unsigned)time(NULL));
        board = new char*[row];
        beans = new Position[bean_num];
        for (int i = 0; i < row; i++) {
            board[i] = new char[col];
            for (int j = 0; j < col; j++)
                board[i][j] = _empty;
        }
    }
    ~GluttonousSnake() {
        for (int i = 0; i < row; i++)
            delete[] board[i];
        delete[] board;
        delete[] beans;
    }
    void run() {
        init();
        while (true) {
            displayBoard();
            Direction direction = motionMonitor();
            if (!isValid(head.nextPosition(direction)))
                break;
            updateBoard(direction);
        }
        cout << endl << "You have died!" << endl;
        system("pause");
    }
private:
    void init() {
        createSnake();
        for (int i = 0; i < bean_num; i++)
            createBean();
    }
    bool isValid(Position position) {
        int x = position.x, y = position.y;
        if (x < 0 || x >= row || y < 0 || y >= col)
            return false;
        if (board[x][y] == _body && (Position(x, y) != tail || tail.nextPosition(path.front()) == head))
            return false;
        return true;
    }
    bool isBean(Position position) {
        return board[position.x][position.y] == _bean;
    }
    void createSnake() {
        tail.x = rand() % row, tail.y = rand() % col; 
        board[tail.x][tail.y] = _body;
        Position now = tail;
        for (int i = 1; i < snake_len; i++) {
            Direction direction = toDirection(rand() % 4);
            while (!isValid(now.nextPosition(direction)))
                direction = toDirection(rand() % 4);
            path.push(direction);
            now.move(direction);
            board[now.x][now.y] = _body;
        }
        head = now;
        board[head.x][head.y] = _head;
    }
    void createBean() {
        int x = rand() % row;
        int y = rand() % col;
        while (board[x][y] != _empty)
            x = rand() % row, y = rand() % col;
        board[x][y] = _bean;
    }
    void updateBoard(Direction direction) {
        bool hasBean = isBean(head.nextPosition(direction));
        board[head.x][head.y] = _body;
        head.move(direction);
        board[head.x][head.y] = _head;
        path.push(direction);
        if (!hasBean) {
            board[tail.x][tail.y] = _empty;
            tail.move(path.front());
            path.pop();
        } else {
            createBean();
        }
    }
    void displayBoard() {
        system("cls");
        for (int i = (3 * col - 21) / 2; i > 0; i--)
            cout << '-';
        cout << " Gluttonous Snake Game ";
        for (int i = (3 * col - 21) / 2; i > 0; i--)
            cout << '-';
        if (!(col % 2))
            cout << '-';
        cout << endl;
        for (int r = 0; r < row; r++) {
            cout << '|';
            for (int c = 0; c < col; c++)
                cout << ' ' << board[r][c] << ' ';
            cout << '|' << endl;
        }
        for (int i = 3 * col + 2; i > 0; i--)
            cout << '-';
        cout << endl;
    }
    Direction toDirection(int code) {
        switch (code) {
            case 0:     return Left;
            case 1:     return Right;
            case 2:     return Up;
            case 3:     return Down;
            default:    return Left;
        }
    }
    Direction motionMonitor() {
        char motion;
        fflush(stdin);
        do {
            motion = getch();
            switch(motion) {
                case 'A':
                case 'a':   return Left;
                case 'D':
                case 'd':   return Right;
                case 'W':
                case 'w':   return Up;
                case 'S':
                case 's':   return Down;
                default:    break;
            }
        } while (true);
    }
};
int main() {
    int row, col, bean_num, snake_len;
    row = 14;
    col = 16;
    bean_num = 7;
    snake_len = 7;
    GluttonousSnake gs(row, col, bean_num, snake_len);
    gs.run();
    return 0;
}