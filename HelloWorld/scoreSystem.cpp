#include "scoreSystem.h"
#include <fstream>
#include <iostream>
#include "constants.h"
#include <sstream>
#include <string>

ScoreSystem::ScoreSystem()
{
	// Try to load high score
	if (!LoadHighScore(DEFAUL_HIGH_SCORE_FILENAME))
	{
		// Defult fill array
		highScoreList = new unsigned int[HIGH_SCORE_LIST_SIZE];
		highScoreListSize = HIGH_SCORE_LIST_SIZE;

		for (int i = 0; i < highScoreListSize; i++)
		{
			highScoreList[i] = 0;
		}
	}
}

ScoreSystem::~ScoreSystem()
{
	delete highScoreList;
}

// Return number in which position of high score should provide score be at
// Also return last position if score is lower then scores in list but list is not fully filled
// Return -1 if score is lower than all numbers in score list
int ScoreSystem::PositionInHighScore(unsigned int score)
{
	for (int i = 0; i < highScoreListSize; i++)
	{
		// Is higher
		if (highScoreList[i] < score)
			return i;
	}

	return -1;
}

// Check and update score in high score list 
// Return true only if given score can be added to high score list
bool ScoreSystem::UpdateHighScoreList(unsigned int score)
{
	int position = PositionInHighScore(currentScore);

	// Don't update
	if (position == -1)
		return false;

	// Move list
	for (int i = highScoreListSize - 1; i > position; i--)
	{
		highScoreList[i] = highScoreList[i - 1];
	}

	// Update 
	highScoreList[position] = score;
	return true;
}

// Write current high score list values score to file of given name
void ScoreSystem::SaveHighScore(const char* filename)
{
	std::ofstream file;
	file.open(filename);

	for (int i = 0; i < highScoreListSize; i++)
	{
		file << highScoreList[i] << "\n";
	}

	file.close();
}

// Read file of given name and stores results in high score list
bool ScoreSystem::LoadHighScore(const char* filename)
{
	char* line;
	std::ifstream file;
	file.open(filename);

	// Define array
	int lines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');

	highScoreList = new unsigned int[lines];
	highScoreListSize = lines;

	// Restart line reading
	int currentLine = 0;
	file.clear();
	file.seekg(0);

	// Read file
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			highScoreList[currentLine] = std::stoi(line);
			currentLine++;
		}

		file.close();
		return true;
	}

	// Fail to read
	return false;
}