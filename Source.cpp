#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>

using namespace std;
int speed;
int pxSize;
char keyPress;
bool paused = 0;
bool movement = false;
int tick = 0;

class snek : public olc::PixelGameEngine
{

public:
	snek()
	{
		sAppName = "Snek - ESC = Pause | SPACE = Resume";
	}


public:
	float fTargetFrameTime = 1.0f / (100.0f / speed);
	float fAccumulatedTime = 0.0f;
	int posX = 0, posY = 0, dirX = 0, dirY = 0, snekLen = 0, posFoodX, posFoodY;
	vector<vector<int>> snekBody;
	void snekPositionUpdater(int dirX, int dirY) 
			{
				posX = posX + dirX;
				posY = posY + dirY;
			}

public:

	bool dist(int x1, int y1, int x2, int y2)
	{
		double dist;
		dist = sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
		if (dist<pxSize)
			return 1;
		else
			return 0;
	}

	void movementUpdater()
	{
		tick++;

		if (GetKey(olc::Key::W).bHeld && !(dirY == pxSize) && !movement)
		{
			dirY = pxSize * -1;
			dirX = 0;
			movement = true;
		}

		if (GetKey(olc::Key::A).bHeld && !(dirX == pxSize) && !movement)
		{
			dirX = pxSize * -1;
			dirY = 0;
			movement = true;
		}

		if (GetKey(olc::Key::S).bHeld && !(dirY == (pxSize * -1)) && !movement)
		{
			dirY = pxSize;
			dirX = 0;
			movement = true;
		}

		if (GetKey(olc::Key::D).bHeld && !(dirX == (pxSize * -1)) && !movement)
		{
			dirX = pxSize;
			dirY = 0;
			movement = true;
		}

		if (tick == 2)
		{
			tick = 0;
			movement = false;
		}
	}


	void pauseChecker()
	{
		if (GetKey(olc::Key::ESCAPE).bHeld)
				{
					paused = 1;
				}

		if (GetKey(olc::Key::SPACE).bHeld && (paused == 1))
		{
			paused = 0;
		}
	}

	void edgeHitCheck()
	{
		if (posX >= ScreenWidth())
			posX = 0;
		if (posX < 0)
			posX = ScreenWidth();
		if (posY >= ScreenHeight())
			posY = 0;
		if (posY < 0)
			posY = ScreenHeight();
	}


	bool OnUserCreate() override
	{
		Clear(olc::VERY_DARK_GREY);
		FillRect(posX, posY, pxSize, pxSize, olc::Pixel(0, 255, 0));
		int tempX, tempY;
		tempX = rand() % ScreenWidth();
		tempY = rand() % ScreenHeight();
		posFoodX = (tempX - (tempX%pxSize)) % ScreenWidth();
		posFoodY = (tempY - (tempY%pxSize)) % ScreenHeight();
		FillRect(posFoodX, posFoodY, pxSize, pxSize, olc::Pixel(255, 255, 255));
	
		

	return true;
}

	bool OnUserUpdate(float fElapsedTime) override
	{

		pauseChecker();
		movementUpdater();
		

		// VVVVV --- Lower the virtual FPS --- VVVVV

		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
		}
		else
			return true;

		

		

		if (paused == 0)
		{

			//cout << ScreenWidth() << " "<<posX << " | " << ScreenHeight() << " " << posY << endl;



			if (snekBody.begin() != snekBody.end())
			{
				for (int i = 0; i < snekBody.size() - 1; i++)
				{
					if (snekBody[i][0] == posX && snekBody[i][1] == posY)
						return true;
					snekBody[i] = snekBody[i + 1];
				}
				snekBody[snekLen - 1] = { posX,posY };
			}


			// VVVVV --- Eating "food" and spawning a new one upon it being eaten --- VVVVV

			if (dist(posX, posY, posFoodX, posFoodY))
			{
				sAppName = (string)"Snake game | Highschore : " + to_string(snekLen+1);
				int tempX, tempY;
				tempX = rand() % ScreenWidth();
				tempY = rand() % ScreenHeight();
				posFoodX = (tempX - (tempX % pxSize)) % ScreenWidth();
				posFoodY = (tempY - (tempY % pxSize)) % ScreenHeight();
				snekLen = snekLen + 1;
				vector<int> temp = { posX,posY }; // increases size by adding a temp vector into the main snek body
				snekBody.push_back(temp);
			}


			/*
			
			OLD KEYPRESS DETECTION

			if (_kbhit())
				keyPress = _getch();
			switch (keyPress)
			{

				case 'w':
				{
					if (dirY == pxSize)
						break;
					dirY = pxSize * -1;
					dirX = 0;
					break;
				}

				case 'a':
				{
					if (dirX == pxSize)
						break;
					dirX = pxSize * -1;
					dirY = 0;
					break;
				}

				case 's':
				{
					if (dirY == (pxSize * -1))
						break;
					dirY = pxSize;
					dirX = 0;
					break;
				}

				case 'd':
				{
					if (dirX == (pxSize*-1))
						break;
					dirX = pxSize;
					dirY = 0;
					break;
				}
			}
			*/


			snekPositionUpdater(dirX, dirY);

			edgeHitCheck();

			Clear(olc::BLACK);

			// VVVVV --- Drawing snek body --- VVVVV

			for (int i = 0; i < snekBody.size(); i++)
			{
				/*if (snekBody[i][0] == posX && snekBody[i][1] == posY) {
					while (!GetKey(olc::Key::SPACE).bHeld) {
						return true;
					}
				}*/

				FillRect(snekBody[i][0], snekBody[i][1], pxSize, pxSize, olc::DARK_GREEN);
			}

			// VVVVV --- Drawing snek head and food --- VVVVV

			FillRect(posX, posY, pxSize, pxSize, olc::GREEN);
			FillRect(posFoodX, posFoodY, pxSize, pxSize, olc::RED);
			//Sleep(speed);


			return true;
		}
	else
	{
		return true;
	}
	}
};

int main()
{
	cout << "Warning: Values too high may break the game\nInsert difficulty of the game: (The higher the harder, suggested is 8)\n";
	cin >> pxSize;

	cout << "\nWhat about the speed of the game? (The higher the harder, suggested is 75) \n";
	cin >> speed;

	speed = 500 / speed;
	system("cls");

	
	cout << "Controls:\n"
		 << "W A S D - Movement\n"
		 << "ESC - Pause\n"
		 << "SPACE - Resume\n\n\n";
	
	Sleep(3000);

	cout << "Game Starting in: \n";

	Sleep(1000);

	cout << "3...\n";

	Sleep(1000);

	cout << "2..\n";

	Sleep(1000);

	cout << "1.\n";

	Sleep(1000);
	
	snek game;

	if (game.Construct(200, 200, 3, 3,false,false))
		game.Start();

	cout <<"\n\nHighschore:" << game.snekLen + 1<< "\n\n";

	return 0;
}
