#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "game.h"
#include "Play.h"
#include "constants.h"

bool ObjectAreaCollission(Play::GameObject& obj, Play::Point2D paddleTopLeft, Play::Point2D paddleBottomRight)
{
	const float dx = obj.pos.x - max(paddleTopLeft.x, min(obj.pos.x, paddleBottomRight.x));
	const float dy = obj.pos.y - max(paddleTopLeft.y, min(obj.pos.y, paddleBottomRight.y));
	return (dx * dx + dy * dy) < (obj.radius * obj.radius);
}

//----------------------------------------------------------------------
// Pddle functions
//----------------------------------------------------------------------

Paddle paddle = Paddle(60, 10, 6);

void UpdatePaddle()
{
	// Movement
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

Ball ball = Ball(1, 0.75, 5);
std::vector<int> ballIds = {};

void SpawnBall()
{
	Play::CreateGameObject(ObjectType::TYPE_BALL, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 4, "ball");
	ballIds = Play::CollectGameObjectIDsByType(ObjectType::TYPE_BALL); 
}

void UpdateBall()
{
	GameObject& ballGo = Play::GetGameObject(ballIds.at(0));
	ballGo.velocity = normalize({ ball.dirX, ball.dirY }) * ball.speed;

	// Check boundaries
	if (ballGo.pos.x <= 0)
		ball.dirX = abs(ball.dirX);

	if (ballGo.pos.x >= DISPLAY_WIDTH)
		ball.dirX = -abs(ball.dirX);

	if (ballGo.pos.y <= 0)
		ball.dirY = abs(ball.dirY);

	if (ballGo.pos.y >= DISPLAY_HEIGHT)
		ball.dirY = -abs(ball.dirY);

	Play::UpdateGameObject(ballGo);
	Play::DrawObject(ballGo);
}

void SetBallDirection(float x, float y)
{
	ball.dirX = x;
	ball.dirY = y;
}

//----------------------------------------------------------------------
// Bricks functions
//----------------------------------------------------------------------

std::vector<int> brickIds = {};

void CreateBricks(int countX, int countY)
{
	for (int x = 0; x < countX; x++)
	{
		for (int y = 0; y < countY; y++)
		{
			Play::CreateGameObject(ObjectType::TYPE_BRICK, { 18 * x, DISPLAY_HEIGHT - 12 * y }, 4, "brick");
		}
	}

	brickIds = Play::CollectGameObjectIDsByType(ObjectType::TYPE_BRICK);
}

void UpdateBricks()
{
	for (int i = 0; i < brickIds.size(); i++)
	{
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
				Play::DestroyGameObject(brickIds.at(i));
				SetBallDirection(ball.dirX, -ball.dirY);
				brickToRemove = i;
			}
		}

		// Remove brick id
		if (brickToRemove != -1)
			brickIds.push_back(brickIds.at(brickToRemove));
	}
}


//----------------------------------------------------------------------
// General functions
//----------------------------------------------------------------------

void StepFrame(float deltaT)
{
	UpdatePaddle();
	UpdateBall();
	UpdateBricks();

	// Ball player collision
	GameObject& ballGo = Play::GetGameObject(ballIds.at(0));

	if (ObjectAreaCollission(ballGo, paddle.TopRight(), paddle.BottomLeft()))
		ball.dirY = -ball.dirY;
}