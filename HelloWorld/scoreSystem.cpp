#include "scoreSystem.h"
#include <fstream>
#include <iostream>

// Return number in which position of high score should provide score be at
// Also return last position if score is lower then scores in list but list is not fully filled
// Return -1 if score is lower than all numbers in score list
int PositionInHighScore(int score, ScoreSystem scoreSys)
{
	for (int i = 0; i < HIGH_SCORE_LIST_SIZE; i++)
	{
		// Is higher
		if (scoreSys.highScoreList > score)
			return i;

		// Empty position
		if (scoreSys.highScoreList == 0)
			return i;
	}

	return -1;
}

// Check and update score in high score list 
// Return true only if given score can be added to high score list
bool UpdateHighScoreList(int score, ScoreSystem& scoreSys)
{
	int position = PositionInHighScore(scoreSys.score, scoreSys);

	// Don't update
	if (position == -1)
		return false;

	// Update 
	scoreSys.highScoreList[position] = score;
	return true;
}

// Write current high score list values score to file of given name
void SaveHighScore(const char* filename, ScoreSystem scoreSys)
{
	std::ofstream file;
	file.open(filename);

	for (int i = 0; i < HIGH_SCORE_LIST_SIZE; i++)
	{
		file << scoreSys.highScoreList[i] << "\n";
	}

	file.close();
}

// Read file of given name and stores results in high score list
void LoadHighScore(const char* filename, ScoreSystem scoreSys)
{
	char* line;
	std::ifstream file;
	file.open(filename);

	/*
	if (file.is_open())
	{
		while (file.getline(line))
		{
			cout << line << '\n';
		}
		myfile.close();
	}
	*/
}