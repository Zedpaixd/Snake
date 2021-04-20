#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>

using namespace std;

int pxSize;
char keyPress;

class snek : public olc::PixelGameEngine
{
public:
	snek()
	{
		sAppName = "Snek";
	}


public:

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
		if (dist<pxSize+1)
			return 1;
		else
			return 0;
	}

	bool OnUserCreate() override
	{
		Clear(olc::VERY_DARK_GREY);
		FillRect(posX, posY, pxSize, pxSize, olc::Pixel(0, 255, 0));
		posFoodX = rand() % ScreenWidth();
		posFoodY = rand() % ScreenHeight();
		FillRect(posFoodX, posFoodY, pxSize, pxSize, olc::Pixel(255, 255, 255));
	
		

	return true;
}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Sleep(100);

	
		if (posX == ScreenWidth() || posX == -1) {
			posX = 0;
		}

		if (posY == ScreenHeight() || posY == -1) {
			posY = 0;
		}
	
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

		if (dist(posX,posY,posFoodX,posFoodY))
		{
			posFoodX = rand() % ScreenWidth();
			posFoodY = rand() % ScreenHeight();
			snekLen = snekLen + 1;
			vector<int> temp = { posX,posY }; // increases size by adding a temp vector into the main snek body
			snekBody.push_back(temp);
		}
		//cout << dist(posX, posY, posFoodX, posFoodY) << endl;
		if (_kbhit())
			keyPress = _getch();
		switch (keyPress)
		{

			case 'w':
			{
				if (dirY == pxSize)
					return true;
				dirY = pxSize * -1;
				dirX = 0;
				break;
			}

			case 'a':
			{
				if (dirX == pxSize)
					return true;
				dirX = pxSize * -1;
				dirY = 0;
				break;
			}

			case 's':
			{
				if (dirY == (pxSize * -1))
					return true;
				dirY = pxSize;
				dirX = 0;
				break;
			}

			case 'd':
			{
				if (dirX == (pxSize*-1))
					return true;
				dirX = pxSize;
				dirY = 0;
				break;
			}
		}

	
		snekPositionUpdater(dirX, dirY);


		for (int i = 0; i < ScreenWidth(); i++)
			for (int j = 0; j < ScreenHeight(); j++)
				Draw(i, j, olc::Pixel(0, 0, 0));


		for (int i = 0; i < snekBody.size(); i++)
		{
			if (snekBody[i][0] == posX && snekBody[i][1] == posY) {
				while (!GetKey(olc::Key::SPACE).bHeld) {
					return true;
				}
			}
			FillRect(snekBody[i][0], snekBody[i][1], pxSize , pxSize , olc::DARK_GREEN);
	}


		FillRect(posX, posY, pxSize, pxSize, olc::GREEN);
		FillRect(posFoodX, posFoodY, pxSize, pxSize, olc::RED);


	return true;
	}
	
};

int main()
{
	cout << "Warning: Values too high may break the game\nInsert difficulty of the game: (1 being hardest and 5 recommended)\n";
	cin >> pxSize;
	srand(time(NULL));
	snek game;
	if (game.Construct(200, 200, 3, 3,false,true))
		game.Start();
	return 0;
}