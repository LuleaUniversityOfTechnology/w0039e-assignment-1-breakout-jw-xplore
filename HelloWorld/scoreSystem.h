#pragma once
#include "constants.h"

/*
Score system assigment
1. Create a stack array that contains the top 5 high scores (unsigned integers).
2. Draw the high score array to the screens bottom right using `Play::DrawDebugText`.
3. Add a variable that represents the current player score.
4. Draw the current player score to the bottom left of the screen.
5. When the ball breaks a brick, increase the current player score by 1.
6. When the ball exits the bottom of the screen, 
store the current score in the high score array at the appropriate place.
Make sure you shift all the previous scores if necessary.
Then remove the ball and all remaining bricks, reset the scene and the current score and spawn a new ball.
*/

struct ScoreSystem
{
	unsigned int currentScore = 0;
	unsigned int* highScoreList[5];

	int PositionInHighScore(unsigned int score);
	bool UpdateHighScoreList(unsigned int score);
	void SaveHighScore(const char* filename);
	void LoadHighScore(const char* filename);
};