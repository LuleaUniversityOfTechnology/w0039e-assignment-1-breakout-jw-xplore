#include "game.h"

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	StartGame();
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	// Background
	Play::ClearDrawingBuffer( Play::cBlack );

	// Object updates
	StepFrame(elapsedTime);

	// Draw
	Play::PresentDrawingBuffer();
	
	return Play::KeyDown( Play::KEY_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void )
{
	Play::DestroyManager();
	return PLAY_OK;
}
