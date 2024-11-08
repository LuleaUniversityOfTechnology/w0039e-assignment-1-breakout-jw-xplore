#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "game.h"
#include "Play.h"
#include "constants.h"
#include "Paddle.h"

float Max(float a, float b)
{
	return a > b ? a : b;
}

float Min(float a, float b)
{
	return a < b ? a : b;
}

bool ObjectAreaCollission(Play::GameObject& obj, Play::Point2D paddleTopLeft, Play::Point2D paddleBottomRight)
{
	const float dx = obj.pos.x - Max(paddleTopLeft.x, Min(obj.pos.x, paddleBottomRight.x));
	const float dy = obj.pos.y - Max(paddleTopLeft.y, Min(obj.pos.y, paddleBottomRight.y));
	return (dx * dx + dy * dy) < (obj.radius * obj.radius);
}

//----------------------------------------------------------------------
// Pddle functions
//----------------------------------------------------------------------

Paddle paddle = Paddle(60, 10, 6);

void UpdatePaddle()
{
	paddle.Move();
	DrawPaddle();
}

void DrawPaddle()
{
	Play::DrawRect(paddle.TopRight(), paddle.BottomLeft(), Play::cWhite, true);
}

//----------------------------------------------------------------------
// Ball functions
//----------------------------------------------------------------------

std::vector<int> ballIds = {};

void SpawnBall(Play::Point2D pos)
{
	int id = Play::CreateGameObject(ObjectType::TYPE_BALL, pos, 4, "ball");
	ballIds = Play::CollectGameObjectIDsByType(ObjectType::TYPE_BALL); 

	GameObject& ballGo = Play::GetGameObject(id);
	SetBallDirection(ballGo, 1, 1);
}

void UpdateBalls()
{
	for (int i = 0; i < ballIds.size(); i++)
	{
		// Find game object
		GameObject& ballGo = Play::GetGameObject(ballIds.at(i));

		// Check boundaries
		if (ballGo.pos.x <= 0)
			SetBallDirection(ballGo, abs(ballGo.velocity.x), ballGo.velocity.y);

		if (ballGo.pos.x >= DISPLAY_WIDTH)
			SetBallDirection(ballGo, -abs(ballGo.velocity.x), ballGo.velocity.y);

		if (ballGo.pos.y <= 0)
			SetBallDirection(ballGo, ballGo.velocity.x, abs(ballGo.velocity.y));

		if (ballGo.pos.y >= DISPLAY_HEIGHT)
			SetBallDirection(ballGo, ballGo.velocity.x, -abs(ballGo.velocity.y));

		// Ball player collision
		if (ObjectAreaCollission(ballGo, paddle.TopRight(), paddle.BottomLeft()))
			SetBallDirection(ballGo, ballGo.velocity.x, -ballGo.velocity.y);

		// Update and render
		Play::UpdateGameObject(ballGo);
		Play::DrawObject(ballGo);
	}
}

void SetBallDirection(GameObject& ballGo, float x, float y)
{
	ballGo.velocity = normalize({ x, y }) * BALL_SPEED;
}

//----------------------------------------------------------------------
// Bricks functions
//----------------------------------------------------------------------

std::vector<int> brickIds = {};

void CreateBricks(int countX, int countY)
{
	// Create game objects
	for (int x = 0; x < countX; x++)
	{
		for (int y = 0; y < countY; y++)
		{
			Play::CreateGameObject(ObjectType::TYPE_BRICK, { BRICK_WIDTH * x, DISPLAY_HEIGHT - BRICK_HEIGHT * y }, 4, "brick");
		}
	}

	// Register
	brickIds = Play::CollectGameObjectIDsByType(ObjectType::TYPE_BRICK);
}

void UpdateBricks()
{
	for (int i = 0; i < brickIds.size(); i++)
	{
		// Update and render
		Play::GameObject& brick = Play::GetGameObject(brickIds.at(i));
		Play::UpdateGameObject(brick);
		Play::DrawObject(brick);

		int brickToRemove = -1;

		// Check ball collision
		for (int ib = 0; ib < ballIds.size(); ib++)
		{
			Play::GameObject& ballGo = Play::GetGameObject(ballIds.at(ib));

			// Destroy brick on ball collision
			if (Play::IsColliding(brick, ballGo))
			{
				SetBallDirection(ballGo, ballGo.velocity.x, -ballGo.velocity.y);
				brickToRemove = i;
			}
		}

		// Remove brick id
		if (brickToRemove != -1)
		{
			Play::DestroyGameObject(brickIds.at(brickToRemove));
			brickIds.push_back(brickIds.at(brickToRemove));
		}
			
	}
}


//----------------------------------------------------------------------
// General functions
//----------------------------------------------------------------------

void StepFrame(float deltaT)
{
	UpdatePaddle();
	UpdateBalls();
	UpdateBricks();
}