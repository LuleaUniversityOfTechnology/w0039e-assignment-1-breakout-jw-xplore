#pragma once

const int DISPLAY_WIDTH = 640;
const int DISPLAY_HEIGHT = 360;
const int DISPLAY_SCALE = 2;

const int BALL_SPEED = 5;

const int BRICK_WIDTH = 18;
const int BRICK_HEIGHT = 12;

const char* const DEFAUL_HIGH_SCORE_FILENAME = "highScore.txt";
const int HIGH_SCORE_LIST_SIZE = 5;

enum ObjectType
{
	TYPE_BALL,
	TYPE_BRICK
};