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

// Functions
void UpdatePaddle();
void DrawPaddle();
void SpawnBall(Play::Point2D pos);
void UpdateBalls();
void SetBallDirection(Play::GameObject& ballGo, float x, float y);
void CreateBricks(int countX, int countY);
void UpdateBricks();

void StepFrame(float deltaT);