#include<bits/stdc++.h>
#include<conio.h> 
#include<windows.h>

using namespace std;

#define MAX_LENGTH 1000

// Phuong huong di chuyen
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        cerr << "Loi khoi tao man hinh." << endl;
        exit(1);
    }
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point {
    int xCoord;
    int yCoord;
    Point() {}
    Point(int x, int y) {
        xCoord = x;
        yCoord = y;
    }
};

class Snake {
    int length;
    char direction;
public:
    Point body[MAX_LENGTH];
    Snake(int x, int y) {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    int getLength() {
        return length;
    }

    void changeDirection(char newDirection) {
        if (newDirection == DIR_UP && direction != DIR_DOWN) direction = newDirection;
        else if (newDirection == DIR_DOWN && direction != DIR_UP) direction = newDirection;
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT) direction = newDirection;
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT) direction = newDirection;
    }

    bool move(Point food) {
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        switch (direction) {
            case DIR_UP: body[0].yCoord--; break;
            case DIR_DOWN: body[0].yCoord++; break;
            case DIR_RIGHT: body[0].xCoord++; break;
            case DIR_LEFT: body[0].xCoord--; break;
        }

        // Kiem tra va cham voi tuong
        if (body[0].xCoord < 0 || body[0].yCoord < 0 || body[0].xCoord >= consoleWidth || body[0].yCoord >= consoleHeight) {
            return false; // Ran cham vao tuong
        }

        // Kiem tra va tram voi than ran
        for (int i = 1; i < length; i++) {
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord) {
                return false;
            }
        }

        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord) {
            body[length] = Point(body[length - 1].xCoord, body[length - 1].yCoord);
            length++;
        }

        return true;
    }
};

class Board {
    Snake *snake;
    const char SNAKE_BODY = 'O';
    Point food;
    const char FOOD = 'o';
    const char BORDER = '*';
    int score;
    int speed;
public:
    Board() {
        spawnFood();
        snake = new Snake(consoleWidth / 2, consoleHeight / 2);
        score = 0;
        speed = 50; // Toc do ran ban dau
    }

    ~Board() {
        delete snake;
    }

    int getScore() {
        return score;
    }

    void spawnFood() {
        int x = rand() % (consoleWidth - 2) + 1;
        int y = rand() % (consoleHeight - 2) + 1;
        food = Point(x, y);
    }

    void displayCurrentScore() {
        gotoxy(consoleWidth / 2, 0);
        cout << "Diem so: " << score;
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void draw() {
        system("cls");

        // Ve bien gioi
        for (int x = 0; x < consoleWidth; x++) {
            gotoxy(x, 0); cout << BORDER;
            gotoxy(x, consoleHeight - 1); cout << BORDER;
        }
        for (int y = 0; y < consoleHeight; y++) {
            gotoxy(0, y); cout << BORDER;
            gotoxy(consoleWidth - 1, y); cout << BORDER;
        }

        // Ve ran
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << SNAKE_BODY;
        }

        // Ve thuc an
        gotoxy(food.xCoord, food.yCoord);
        cout << FOOD;

        // Hien thi diem so
        displayCurrentScore();
    }

    bool update() {
        bool isAlive = snake->move(food);
        if (!isAlive) {
            return false;
        }

        // Sinh thuc an moi khi ran an duoc thuc an
        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord) {
            score++;
            spawnFood();

            // Tang toc do
            if (speed > 10) speed -= 5;
        }
        return true;
    }

    void getInput() {
        if (kbhit()) {
            int key = getch();
            if (key == 'w' || key == 'W') snake->changeDirection(DIR_UP);
            else if (key == 'a' || key == 'A') snake->changeDirection(DIR_LEFT);
            else if (key == 's' || key == 'S') snake->changeDirection(DIR_DOWN);
            else if (key == 'd' || key == 'D') snake->changeDirection(DIR_RIGHT);
        }
    }

    int getSpeed() {
        return speed;
    }
};

void displayMenu() {
    system("cls");
    cout << "Menu tro choi ran san moi:" << endl;
    cout << "1. Choi" << endl;
    cout << "2. Huong dan cach choi" << endl;
    cout << "3. Thoat game " << endl;
}

int main() {
    srand(time(0));
    initScreen();
    Board *board;

    while (true) {
        displayMenu();

        char choice;
        cin >> choice;

        if (choice == '1') {
            board = new Board();
            while (board->update()) {
                board->getInput();
                board->draw();
                Sleep(board->getSpeed());
            }

            system("cls");
            cout << "Ket thuc tro choi" << endl;
            cout << "Diem so cuoi cung la: " << board->getScore() << endl;
            cout << "An Enter de tiep tuc..." << endl;
            cin.ignore();
            cin.get(); // Doi nguoi dung an Enter
            delete board;
        } 
        else if (choice == '2') {
            system("cls");
            cout << "Huong dan cach choi:\n";
            cout << " - Su dung W, A, S, D de dieu khien ran.\n";
            cout << " - An thuc an de tang diem.\n";
            cout << " - Tranh dam vao tuong hoac dam vao than ran.\n";
            cout << "An bat ki phim nao de quay lai menu." << endl;
            _getch(); // Doi nguoi dung an phim
        } 
        else if (choice == '3') {
            break;
        } 
        else {
            cout << "Lua chon khong hop le!" << endl;
        }
    }

    return 0;
}


