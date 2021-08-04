#include <iostream>
#include <queue>
#include <cstdio>
#include <ctime>
#include <windows.h>
using namespace std;

// ���ع��ĺ���
void hideCur() {
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
// ���� x��y ���꣬���ù��λ��
void setCur(int x, int y) {
	COORD setCur; //WINDOWS API�еĽṹ�壬��ʾ�ַ��ڿ���̨�ϵ�����
	HANDLE hOutput;
	setCur.X = x;
	setCur.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);  //���ú�������һ���������
	SetConsoleCursorPosition(hOutput, setCur);	//���ù��λ��
}
// �����࣬ÿ�ΰ��·�������ܹ�ӳ���һ�� Direction ֵ
enum Direction {    
    Left, Right, Up, Down
};
// λ����
struct Position {
    int x, y;
    Position(){}
    // ������ĳ�ʼ��
    Position(int x, int y): x(x), y(y){}
    // �ж�����λ���Ƿ����
    bool operator!=(Position &position) {
        return position.x != x || position.y != y;
    }
    // �ж�����λ���Ƿ����
    bool operator==(Position &position) {
        return position.x == x && position.y == y;
    }
    // ����ǰ��λ����ĳ�������ƶ�����Ӧ����������ļӼ����ú�����ı�����λ�ã��޷���ֵ
    void move(Direction direction) {
        switch (direction) {
            case Left:      y--;    break;
            case Right:     y++;    break;
            case Up:        x--;    break;
            case Down:      x++;    break;
        }
    }
    // Ԥ�⵱ǰλ������ĳ�������ƶ������λ�ã��ú�����������ƶ���λ�ã�ֻ�Ƿ���Ԥ��λ��
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
// ̰������
class GluttonousSnake {
private:
    // ��ά����
    char **board;
    // row���У�col���У�bean_num������������snake_len���ߵĳ��ȣ�score������
    int row, col, bean_num, snake_len, score;
    //  head�� ��ͷλ�ã�tail����βλ�ã�*beans��������ӵ�λ��
    Position head, tail;
    // ��ǰ��ͷ�ƶ��ķ���
    Direction curDir;
    // ÿ�β����ķ����ռ�������ɶ��У����µĲ����ڶ�β
    queue<Direction> path;
    // _empty�����صĿյأ�_head��ͷ���ķ��ţ�_tail����β���ķ��ţ�_bean�����ӵķ���
    char _empty, _head, _body, _bean;
public:
    // ͨ���С��С�����������ʼ�߳��Ƚ��г�ʼ��
    GluttonousSnake(int row, int col, int bean_num, int snake_len):
    row(row), col(col), bean_num(bean_num), snake_len(snake_len), score(0) {
        // ���������
        srand((unsigned)time(NULL));
        // �����������
        _empty = ' ';
        _head = 'Q';
        _body = 'o';
        _bean = '*';
        // ���ɳ���
        board = new char*[row];
        // ��ʼ������Ϊ��
        for (int i = 0; i < row; i++) {
            board[i] = new char[col];
            for (int j = 0; j < col; j++)
                board[i][j] = _empty;
        }
    }
    // �ͷų��صĴ洢�ռ�
    ~GluttonousSnake() {
        for (int i = 0; i < row; i++)
            delete[] board[i];
        delete[] board;
    }
    // �ⲿ���õĺ����ӿڣ��ⲿ�޷��������������˽�к���
    void run() {
        // ��ʼ����Ϸ
        init();
        // չʾ��ǰ�ĳ������
        displayBoard();
        while (true) {
            // motionMonitor() ����û�����ķ��򣬲���������� curDir
            motionMonitor();
            // �������λ�ò��Ϸ�����Ϸ����
            if (!isValid(head.nextPosition(curDir)))
                break;
            // ͨ������ķ�����³���
            updateBoard(curDir);
        }
        // ��Ϸ����
        setCur(0, row + 5);
        cout << "You have died!" << endl;
        system("pause");
    }
private:
    // ��ʼ������
    void init() {
        // ��ʼ���������ȵ���������
        createSnake();
        // ��ʼ�����������Ķ���
        for (int i = 0; i < bean_num; i++)
            createBean();
        // ���ع��
        hideCur();
    }
    // �жϸ�λ���Ƿ��ܹ�ʹ�õĺ���
    bool isValid(Position position) {
        // �����Ϊ x, y �������
        int x = position.x, y = position.y;
        // �������ط�Χ������ʹ��
        if (x < 0 || x >= row || y < 0 || y >= col)
            return false;
        // ���������Ҳ���β�������ж�Ϊ�Ƿ�
        if (board[x][y] == _body && position != tail)
            return false;
        // ���������������㣬���ж�λ���ǺϷ���
        return true;
    }
    // �жϵ�ǰλ���Ƿ��Ƕ��ӵĺ���
    bool isBean(Position position) {
        return board[position.x][position.y] == _bean;
    }
    // �����������ȵ�����ĺ���
    void createSnake() {
        // �����ʼ��β�͵�λ������������
        tail.x = rand() % row, tail.y = rand() % col;
        while (tail.x < row / 3 || tail.x > 2 * row / 3 || tail.y < col / 3 || tail.y > 2 * col / 3)
            tail.x = rand() % row, tail.y = rand() % col;
        // �ѳ����е�β�͵�λ�ø���Ϊ _body ����
        board[tail.x][tail.y] = _body;
        // �����µ����꣬����ʼ��Ϊβ�����ڵ�λ��
        Position now = tail;
        // ����ʣ�� (snake_len - 1) ���ȵ�����
        for (int i = 1; i < snake_len; i++) {
            // toDirection() �� [0, 3] �е��ĸ�������ӳ����ĸ�����
            curDir = toDirection(rand() % 4);
            // �� now ����÷����ƶ����λ�ò��Ϸ������������ɷ���
            while (!isValid(now.nextPosition(curDir)))
                curDir = toDirection(rand() % 4);
            // λ�úϷ����������������еĶ�β
            path.push(curDir);
            // �� now ��÷����ƶ�
            now.move(curDir);
            // ���³��صĶ�Ӧλ�õķ���Ϊ _body
            board[now.x][now.y] = _body;
        }
        // ������ϣ�����Ҫһ����ͷ���� now ��λ�ü�Ϊ�ߵ�ͷ��
        head = now;
        // ���¶�Ӧλ��Ϊ��ͷ
        board[head.x][head.y] = _head;
    }
    // ����һ�����ӵĺ���
    void createBean() {
        // �������һ������
        int x = rand() % row;
        int y = rand() % col;
        // ���ӵ�λ�ñ����� _empty ���У�����ѭ��ֱ���ҵ��յ�
        while (board[x][y] != _empty)
            x = rand() % row, y = rand() % col;
        // ���ҵ��Ŀյظ���Ϊ����
        setCur(3 * y + 2, x + 1);
        cout << _bean;
        board[x][y] = _bean;
    }
    // ͨ������ķ����ƶ���ͷ�������³��صĺ���
    void updateBoard(Direction direction) {
        // ��ȡͷ�����������λ�ò��ж��Ƿ��ж���
        bool hasBean = isBean(head.nextPosition(direction));
        // ͷ�����ᱻ���帲��
        setCur(3 * head.y + 2, head.x + 1);
        cout << _body;
        board[head.x][head.y] = _body;
        // �ƶ�ͷ��
        head.move(direction);
        // ���µ�ͷ����λ�ø���Ϊͷ������
        setCur(3 * head.y + 2, head.x + 1);
        cout << _head;
        board[head.x][head.y] = _head;
        // �������������������еĶ�β
        path.push(direction);
        // ��ǰ�������ڶ��ӣ���β�����ڴ�Ϊ�յأ����ƶ�β�ͣ�ͬʱ������Ĳ�������
        if (!hasBean) {
            setCur(3 * tail.y + 2, tail.x + 1);
            cout << _empty;
            board[tail.x][tail.y] = _empty;
            tail.move(path.front());
            path.pop();
        // ���򣬳Ե����ӣ���������䳤��β������������Ч������䳤����������һ������
        } else {
            score += 100;
            setCur(0, row + 2);
            printf("score: %8d", score);
            createBean();
        }
    }
    // ��ӡ���صĺ���
    void displayBoard() {
        // ����
        system("cls");
        // ͨ��һ������ѧ���������һ�����ȵı߽��ߣ�just for ����
        for (int i = (3 * col - 21) / 2; i > 0; i--)
            cout << '-';
        cout << " Gluttonous Snake Game ";
        for (int i = (3 * col - 21) / 2; i > 0; i--)
            cout << '-';
        if (!(col % 2))
            cout << '-';
        cout << endl;
        // ����ÿһ�е����ݣ�����������ֱ���һ��ǽ
        for (int r = 0; r < row; r++) {
            cout << '|';
            for (int c = 0; c < col; c++)
                cout << ' ' << board[r][c] << ' ';
            cout << '|' << endl;
        }
        // ��Ȼ�Ƿֽ���
        for (int i = 3 * col + 2; i > 0; i--)
            cout << '-';
        cout << endl;
        // ��ʽ��ʼ֮ǰ����ͣ
        setCur(0, row + 2);
        cout << "Press any key to start..." << endl;
        system("pause >> nul");
        setCur(0, row + 2);
        cout << "                         " << endl;
        setCur(0, row + 2);
        printf("score: %8d", score);
    }
    // �� [0, 3] ������ӳ��ɷ����������������ʱ����õ�
    Direction toDirection(int code) {
        switch (code) {
            case 0:     return Left;
            case 1:     return Right;
            case 2:     return Up;
            case 3:     return Down;
            default:    return curDir;
        }
    }
    // ����û�����ķ���ĺ��������û�δ���·������ά��ԭ���򣬰��¿ո����ͣ
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
// ������
int main() {
    // ���峤������������������
    int row, col, bean_num, snake_len;
    // �Զ��������ֵ
    row = 15;
    col = 15;
    bean_num = 5;
    snake_len = 3;
    // ����̰������Ĺ��캯����������Ϸʵ��
    GluttonousSnake gs(row, col, bean_num, snake_len);
    // ͨ�� run() �����ӿ���������Ϸ
    gs.run();
    return 0;
}