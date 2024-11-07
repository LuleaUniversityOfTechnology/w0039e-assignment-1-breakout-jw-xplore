#pragma once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"

struct Ball
{
	float dirX = 1;
	float dirY = 1;
	int speed = 5;

	Ball(float dx, float dy, int sp)
	{
		dirX = dx;
		dirY = dy;
		speed = sp;
	}
};

struct Paddle
{
	int width;
	int height;
	int speed;
	int posX;
	int posY = 10;

	Paddle(int w, int h, int s)
	{
		width = w;
		height = h;
		speed = s;

		posX = DISPLAY_WIDTH / 2 - width / 2;
	}
	
	Play::Point2D TopRight()
	{
		return Play::Point2D(posX, posY);
	}

	Play::Point2D BottomLeft()
	{
		return Play::Point2D(posX + width, posY + height);
	}

	void Move()
	{
		if (Play::KeyDown(Play::KeyboardButton::KEY_RIGHT) && posX < DISPLAY_WIDTH - width)
			posX += speed;
		else if (Play::KeyDown(Play::KeyboardButton::KEY_LEFT) && posX > 0)
			posX -= speed;
	}
};

// Functions
void UpdatePaddle();
void DrawPaddle();
void SpawnBall(Play::Point2D pos);
void UpdateBalls();
void SetBallDirection(Play::GameObject& ballGo, float x, float y);
void CreateBricks(int countX, int countY);
void UpdateBricks();

void StepFrame(float deltaT);