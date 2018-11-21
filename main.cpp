/*
	Kyle Schultz
	Homework 1
	This program is my implementation of the Maze game. This program generates a random
	maze on a board. There is one human and two enemies that can move anywhere but walls in the 
	maze. Red Xs are walls and the smiley face is the human. Snakes are enemies and the green check is the exit.
	Sparkles, cherries, and pizza are the treasures. Treasues worth 100,50,75 points for sparkles, cherries and pizza
	repectively. Game ends when a human runs into a enemy, an enemy runs into a human, or if the human makes it to the exit.
	Game automatically ends if a Player has no moves and is trapped by walls
	Improving game choice: Add 2 treasures with different appearence and point value
	
	Complie the program by typing 'make' into the directory with the .cpp and .h files
	Run the program by typing './maze'
*/
#include "Maze.h"


int main()
{
	Player * human = new Player("Kyle", true);	//create a static varibale so the human will last throughout the program. No need to dynamically allocate

	Maze m;
	std::cout << "after creating maze \n";
	m.NewGame(human, 2);				//create a new game with 2 enemies and a human
	std::cout << "after creating new game \n";
	while(m.IsGameOver() != true)	//continue the game until an ending condition has occured
	{
		std::cout << m;
		m.TakeTurn(m.GetNextPlayer());
	}
	std::cout << m.GenerateReport();

	delete human;
	return 0;

}
