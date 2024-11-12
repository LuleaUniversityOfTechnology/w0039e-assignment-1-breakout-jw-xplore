#include "Paddle.h"
#include "game.h"
#include "constants.h"

Paddle::Paddle(int w, int h, int s)
{
	width = w;
	height = h;
	speed = s;

	posX = DISPLAY_WIDTH / 2 - width / 2;
}

void Paddle::Move()
{
	// Prevent control 
	if (!hasControl)
		return;

	// Control movement
	if (Play::KeyDown(Play::KeyboardButton::KEY_RIGHT) && posX < DISPLAY_WIDTH - width)
		posX += speed;
	else if (Play::KeyDown(Play::KeyboardButton::KEY_LEFT) && posX > 0)
		posX -= speed;
}

Play::Point2D Paddle::TopRight()
{
	return Play::Point2D(posX, posY);
}

Play::Point2D Paddle::BottomLeft()
{
	return Play::Point2D(posX + width, posY + height);
}