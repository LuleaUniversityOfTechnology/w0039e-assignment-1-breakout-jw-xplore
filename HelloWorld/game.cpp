#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "game.h"
#include "Play.h"
#include "constants.h"
#include "Paddle.h"
#include "scoreSystem.h"
#include <string.h>

ScoreSystem scoreSystem;
bool isGameRunning = true;

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

		if (ballGo.pos.y >= DISPLAY_HEIGHT)
			SetBallDirection(ballGo, ballGo.velocity.x, -abs(ballGo.velocity.y));

		// Hitting ground
		if (ballGo.pos.y <= 0)
		{
			SetBallDirection(ballGo, ballGo.velocity.x, abs(ballGo.velocity.y));

			// End game
			EndGame();
			return;
		}

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
			brickIds.erase(brickIds.begin() + brickToRemove);

			// Update score 
			scoreSystem.currentScore++;
		}
			
	}
}

//----------------------------------------------------------------------
// Score system
//----------------------------------------------------------------------

void DrawScore()
{
	// Show current score
	Play::Point2D position = { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 20 };
	std::string tmp = "Score: " + std::to_string(scoreSystem.currentScore);
	char const* txt = tmp.c_str();
	Play::DrawDebugText(position, txt, Play::cWhite);

	// Show list of high scores
	if (isGameRunning)
		return;

	Play::Point2D hSPosition = { 20, DISPLAY_HEIGHT - 20 };
	Play::DrawDebugText(hSPosition, "Top score:", Play::cWhite, false);

	for (int i = 0; i < scoreSystem.highScoreListSize; i++)
	{
		tmp = std::to_string(scoreSystem.highScoreList[i]);
		char const* scoreTxt = tmp.c_str();
		hSPosition.y = hSPosition.y - 20;
		Play::DrawDebugText(hSPosition, scoreTxt, Play::cWhite, false);
	}
	
}

//----------------------------------------------------------------------
// General functions
//----------------------------------------------------------------------

void StartGame()
{
	isGameRunning = true;

	// Setup player paddle
	paddle.hasControl = true;
	paddle.posX = DISPLAY_WIDTH / 2;

	// Setup scene
	CreateBricks(40, 5);
	SpawnBall({ DISPLAY_WIDTH / 2 + 100, DISPLAY_HEIGHT / 2 + 50 });
	scoreSystem.currentScore = 0;
}

void StepFrame(float deltaT)
{
	UpdatePaddle();
	UpdateBalls();
	UpdateBricks();
	DrawScore();

	// Update game end 
	if (!isGameRunning)
	{
		// Show restart option
		Play::DrawDebugText({DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2}, "R - Restart game", Play::cWhite);
		if (Play::KeyDown(Play::KeyboardButton::KEY_R))
		{
			StartGame();
		}
	}
}

void EndGame()
{
	// Disable player control
	paddle.hasControl = false;

	// Handle score
	if (scoreSystem.UpdateHighScoreList(scoreSystem.currentScore))
	{
		scoreSystem.SaveHighScore(DEFAUL_HIGH_SCORE_FILENAME);
	}

	// Destroy balls
	for (int i = 0; i < ballIds.size(); i++)
	{
		Play::DestroyGameObject(ballIds[i]);
	}

	ballIds.clear();

	// Remove bricks
	for (int i = 0; i < brickIds.size(); i++)
	{
		Play::DestroyGameObject(brickIds[i]);
	}

	brickIds.clear();

	isGameRunning = false;
}