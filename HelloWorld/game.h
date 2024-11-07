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
	int posY;

	Paddle(int w, int h, int s)
	{
		width = w;
		height = h;
		speed = s;
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
		if (Play::KeyDown(Play::KeyboardButton::KEY_RIGHT))
			posX += speed;
		else if (Play::KeyDown(Play::KeyboardButton::KEY_LEFT))
			posX -= speed;
	}
};

// Functions
//bool ObjectAreaCollission(Play::GameObject& obj, Play::Point2D paddleTopLeft, Play::Point2D paddleBottomRight);

void UpdatePaddle();
void DrawPaddle();
void SpawnBall();
void UpdateBall();
void CreateBricks(int countX, int countY);
void UpdateBricks();

void StepFrame(float deltaT);