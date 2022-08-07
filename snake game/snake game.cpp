#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <string>
using namespace std;

// Variables 
bool gameOver;
bool invalidCoord;
const int width = 100;
const int height = 20;
int x, y, fruitX, fruitY, score, tailLength;
int tailX[100], tailY[100];

//Used for arrow key controls
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void ClearScreen()
{
    // Clears screen without flickering
    COORD cursorPosition;   cursorPosition.X = 0;   cursorPosition.Y = 0;   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void Setup()
{   
    gameOver = false;
    dir = STOP;
    srand(time(0));
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;

}

void Draw() // Drawing playing field, snake and fruits
{
    ClearScreen();

    // Draws top border
    for (int i = 0; i < width + 2; i++)
        cout << '-';
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            // Left border
            if (k == 0)
                cout << '|';
            // Snake's head
            if (i == y && k == x)
                cout << '0';
            // Fruit
            else if (i == fruitY && k == fruitX)
                cout << '$';

            else
            {
                // Checks if there is a tail block and draws it
                bool printTail = false;
                for (int j = 0; j < tailLength; j++)
                {
                    if (tailX[j] == k && tailY[j] == i)
                    {
                        cout << 'o';
                        printTail = true;
                    }
                }
                // Draws blank space
                if (!printTail)
                    cout << ' ';
            }

            // Right border
            if (k == width - 1)
                cout << '|';

        }
        cout << endl;
    }

    // Draws bottom border
    for (int i = 0; i < width + 2; i++)
        cout << '-';
    cout << endl;

    // Displays player's score
    cout << endl;
    cout << "Score: " << score << endl;

}
void Input()
{
    // Changes snake's direction 
    if (_kbhit())
    {
        switch (_getch())
        {
//WASD CONTROLS
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;

//ARROW KEYS
        case KEY_UP:
            if (dir != DOWN)
                dir = UP;
            break;
        case KEY_LEFT:
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case KEY_DOWN:
            if (dir != UP)
                dir = DOWN;
            break;
        case KEY_RIGHT:
            if (dir != LEFT)
                dir = RIGHT;
            break;
//END GAME
        case 'm':
            gameOver = true;
            break;
        }

    }
}

void Logic()
{
    int prevx = tailX[0];
    int prevy = tailY[0];
    int prev2x, prev2y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < tailLength; i++)
    {
        prev2x = tailX[i];
        prev2y = tailY[i];
        tailX[i] = prevx;
        tailY[i] = prevy;
        prevx = prev2x;
        prevy = prev2y;
    }
    // Changes snake's head coordinates depending on a direction
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    // Detects collision with a tail
    for (int i = 0; i < tailLength; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    // Detects collision with a fruit
    if (x == fruitX && y == fruitY)
    {
        score += 1;
        fruitX = rand() % width;
        fruitY = rand() % height;
        // Generate new fruit position if it collides with snake's tail position 
        for (int i = 0; i < tailLength; )
        {
            invalidCoord = false;
            if (tailX[i] == fruitX && tailY[i] == fruitY) {
                invalidCoord = true;
                fruitX = rand() % width;
                fruitY = rand() % height;
                break;
            }
            if (!invalidCoord)
                i++;
        }
        tailLength++;
    }

    if (x > width || x < 0 || y > height || y < 0)
        gameOver = true;

    //To win the game
    if (score >= 90)
    {
        gameOver = TRUE;
    }

}

void Info() {
    cout << "\n\n            Welcome to the game of snake!" << endl;
    cout << "\n            Your goal is to eat as many \"$\" as you can without touching the walls." << endl;
    cout << "\n            For every \"$\" eaten you will have 1 point added to your score; earn 90 points to win! " << endl;
    cout << "\n            If you touch a wall or run into your own tail the game will end." << endl;
    cout << "\n            You may exit the game anytime by pressing the \"m\" key." << endl;
    cout << "\n\n            You can use the arrow keys or W,A,S,D to control the snake" << endl;
    cout << "\n\n            Press the \"ENTER\" key to continue." << endl;
    cout << "\n\n\n\n\n            $  0oooooooooooooooooooo" << endl;
    string holder;
    getline(cin, holder);
    system("cls");
}

int main()
{
    Info();
    Setup();
    while (!gameOver) // Game mainloop 
    {
        Draw();
        if (dir == UP || dir == DOWN)
            Sleep(40); // Helps to equate vertical snake movement speed and horizontal speed
        Sleep(20);
        Input();
        Logic();
    }

    //After user wins or loses

    HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
    //Change font size
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.dwFontSize.X = 30;   // Width 
    cfi.dwFontSize.Y = 30;   // Height
    SetCurrentConsoleFontEx(screen, FALSE, &cfi);

    if (score < 90) {
        int color = 12;
        SetConsoleTextAttribute(screen, color);
        system("cls");
        cout << "\n                GAME OVER" << endl;
        cout << "\n                Final Score: " << score;
    }

    else if (score >= 90) {
        int color = 10;
        SetConsoleTextAttribute(screen, color);
        system("cls");
        cout << "\n                YOU WIN!!!" << endl;
        cout << "\n                Final Score: " << score;
    }

    system("pause > 0");
}