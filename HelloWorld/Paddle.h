#pragma once
#include "game.h"
#include "constants.h"

struct Paddle
{
	int width;
	int height;
	int speed;
	int posX;
	int posY = 10;

	Paddle(int w, int h, int s);

	void Move();
	Play::Point2D TopRight();
	Play::Point2D BottomLeft();
};