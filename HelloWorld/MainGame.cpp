#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"

enum ObjectType
{
	TYPE_BALL
};

int DISPLAY_WIDTH = 640;
int DISPLAY_HEIGHT = 360;
int DISPLAY_SCALE = 2;

// Ids
std::vector<int> ballIds = {};;

//----------------------------------------------------------------------
// Custom functions
//----------------------------------------------------------------------

void UpdateBall()
{

}


//----------------------------------------------------------------------
// Core functions
//----------------------------------------------------------------------

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );

	// Register ball object
	Play::CreateGameObject(ObjectType::TYPE_BALL, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 4, "ball");
	ballIds = Play::CollectGameObjectIDsByType(ObjectType::TYPE_BALL);
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Background
	Play::ClearDrawingBuffer( Play::cBlack );

	// Ball
	GameObject& ball = Play::GetGameObject(ballIds.at(0));
	ball.velocity = normalize({ 1, -1 }) * 0.1f;
	Play::UpdateGameObject(ball);
	Play::DrawObject(ball);

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
