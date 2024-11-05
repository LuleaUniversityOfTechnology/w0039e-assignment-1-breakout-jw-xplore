#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include <cmath>

enum ObjectType
{
	TYPE_PADDLE,
	TYPE_BALL,
	TYPE_BRICK
};

int DISPLAY_WIDTH = 640;
int DISPLAY_HEIGHT = 360;
int DISPLAY_SCALE = 2;

int BRICKS_COUNT_X = 40;
int BRICKS_COUNT_Y = 5;

// Ids
int playerId;
std::vector<int> ballIds = {};
std::vector<int> brickIds = {};

// TODO: Change this to ball object properties
int balldirx = 1;
int balldiry = 1;

//----------------------------------------------------------------------
// Stuctures
//----------------------------------------------------------------------

struct Paddle
{
	int width;
	int height;
	int speed;
	int posX = DISPLAY_WIDTH / 2;
	int posY = 20;

	Paddle()
	{

	}

	Paddle(int w, int h, int s)
	{
		width = w;
		height = h;
		speed = s;
	}

	Point2D TopRight()
	{
		return Point2D(posX, posY);
	}

	Point2D BottomLeft()
	{
		return Point2D(posX + width, posY + height);
	}

	void Move()
	{
		if (Play::KeyDown(KEY_RIGHT))
			posX += speed;
		else if (Play::KeyDown(KEY_LEFT))
			posX -= speed;
	}
};

Paddle paddle;

//----------------------------------------------------------------------
// Custom functions
//----------------------------------------------------------------------

void DrawPaddle()
{
	Play::DrawRect(paddle.TopRight(), paddle.BottomLeft(), Play::cWhite, true);
}

bool IsColliding(GameObject& obj, Point2D paddleTopLeft, Point2D paddleBottomRight)
{
	const float dx = obj.pos.x - max(paddleTopLeft.x, min(obj.pos.x, paddleBottomRight.x));
	const float dy = obj.pos.y - max(paddleTopLeft.y, min(obj.pos.y, paddleBottomRight.y));
	return (dx * dx + dy * dy) < (obj.radius * obj.radius);
}

void UpdatePaddle()
{
	// Movement
	paddle.Move();

	// Check paddle collision
	GameObject& ball = Play::GetGameObject(ballIds.at(0));
	

	if (IsColliding(ball, paddle.TopRight(), paddle.BottomLeft()))
		balldiry = -balldiry;

	DrawPaddle();
}

// TODO: Handle collisions here, make sure ball direction is changed based on collision direction
void BallCollision()
{
}

void UpdateBall()
{
	GameObject& ball = Play::GetGameObject(ballIds.at(0));
	ball.velocity = normalize({ balldirx, balldiry }) * 4;

	// Check boundaries
	if (ball.pos.x <= 0) 
		balldirx = 1;

	if (ball.pos.x >= DISPLAY_WIDTH)
		balldirx = -1;

	if (ball.pos.y <= 0)
	{
		ball.pos.x = DISPLAY_WIDTH / 2;
		ball.pos.y = DISPLAY_HEIGHT / 2;
	}

	Play::UpdateGameObject(ball);
	Play::DrawObject(ball);
}

void UpdateBrick()
{
	for (int i = 0; i < brickIds.size(); i++)
	{
		GameObject& brick = Play::GetGameObject(brickIds.at(i));
		Play::UpdateGameObject(brick);
		Play::DrawObject(brick);

		// Check ball collision
		for (int ib = 0; ib < ballIds.size(); ib++)
		{
			GameObject& ball = Play::GetGameObject(ballIds.at(ib));
		
			// Destroy brick on ball collision
			if (Play::IsColliding(brick, ball))
			{
				Play::DestroyGameObject(brickIds.at(i));
				brickIds.push_back(brickIds.at(i));
				balldiry = -balldiry;
				return;
			}

		}
	}
}

//----------------------------------------------------------------------
// Core functions
//----------------------------------------------------------------------

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );

	// Create player paddle 
	paddle = Paddle(60, 10, 5);

	// Register ball object
	Play::CreateGameObject(ObjectType::TYPE_BALL, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 4, "ball");
	ballIds = Play::CollectGameObjectIDsByType(ObjectType::TYPE_BALL);

	// Register bricks
	for (int x = 0; x < BRICKS_COUNT_X; x++)
	{
		for (int y = 0; y < BRICKS_COUNT_Y; y++)
		{
			Play::CreateGameObject(ObjectType::TYPE_BRICK, { 18 * x, DISPLAY_HEIGHT - 12 * y }, 4, "brick");
		}
	}

	brickIds = Play::CollectGameObjectIDsByType(ObjectType::TYPE_BRICK);
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Background
	Play::ClearDrawingBuffer( Play::cBlack );

	// Object updates
	UpdatePaddle();
	UpdateBall();
	UpdateBrick();

	// Draw
	Play::PresentDrawingBuffer();
	return Play::KeyDown( KEY_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}
