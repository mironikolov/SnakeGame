#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <io.h>
#include<time.h> 

bool gameOver;
const int width = 20, height = 20;
int fruitX, fruitY, score = 0, speed=500;
enum eDirection {
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
eDirection dir = STOP;

struct snakePart {
	const snakePart* prevPart = nullptr;
	int x = 0;
	int y = 0;
	int lastX = 0;
	int lastY = 0;

	snakePart()
	{
		x = width / 2;
		y = height / 2;
	}

	snakePart( const snakePart* t)
	{
		prevPart = t;
		x = t->x;
		y = t->y;
	}

	void Move() {
		
		lastX = x;
		lastY = y;
		if (prevPart != nullptr)
		{
			x = prevPart->lastX;
			y = prevPart->lastY;
		}
	}
};

std::vector<snakePart*> snake;

void Setup() {
	srand(time(0));
	fruitX = rand() % width;
	fruitY = rand() % height;

	CONSOLE_FONT_INFOEX cfi = {sizeof(cfi)};
	cfi.dwFontSize = {12, 12};
	HANDLE hConsoleOutput = CreateFileA("CONOUT$", GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetCurrentConsoleFontEx(hConsoleOutput, false, &cfi);

	snakePart* head = new snakePart();
	snake.push_back(head);
}

void Draw() {
	system("cls");
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == 0 || i == height - 1) std::cout << "#";
			else 
			{

				bool breakIteration = false;
				for (snakePart* part : snake) {
					if (part->x == j && part->y == i) {
						std::cout << "O"; 
						breakIteration = true;
					} 
				}
				if (breakIteration) continue;
				if (j == 0 || j == width - 1) std::cout << "#";
				else if (j == fruitX && i == fruitY) std::cout << "F";
				else std::cout << " ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "Score: " << score << std::endl;
	Sleep(speed);
}

void Input() {
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 32:
			gameOver = true;
			break;
		default:
			break;
		}
	}
}

void Logic() {
	for (snakePart* part : snake) part->Move();

	switch (dir)
	{
	case LEFT:
		snake[0]->x--;
		break;
	case RIGHT:
		snake[0]->x++;
		break;
	case UP:
		snake[0]->y--;
		break;
	case DOWN:
		snake[0]->y++;
		break;
	default:
		break;
	}

	if (snake[0]->x == 0 || snake[0]->y == 0 || snake[0]->x == width - 1 || snake[0]->y == height - 1)
	{
		gameOver = true;
	}
	for (int i = 1; i < snake.size(); i++)
	{
		if (snake[0]->x == snake[i]->x && snake[0]->y == snake[i]->y)
		{
			gameOver = true;
		}
	}

	if (snake[0]->x == fruitX && snake[0]->y == fruitY)
	{
		score += 10;
		fruitX = rand() % (width - 1);
		if(fruitX == 0) fruitX++;
		fruitY = rand() % (height - 1);
		if (fruitY == 0) fruitY++;

		snake.push_back( new snakePart( snake.back() ) );
		if (speed > 0 )
		{
			speed -= 50;
		}
	}

}

int main()
{
	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
	}
	std::cout << "GAME OVER!!!" << std::endl;
	std::cin.get();
}

