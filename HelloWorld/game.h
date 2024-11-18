#pragma once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "constants.h"

// Functions 
void UpdatePaddle();
void DrawPaddle();
void SpawnBall(Play::Point2D pos);
void UpdateBalls();
void SetBallDirection(Play::GameObject& ballGo, float x, float y);
void CreateBricks(int countX, int countY);
void UpdateBricks();

void StartGame();
void StepFrame(float deltaT);
void EndGame();