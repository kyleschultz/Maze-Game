/*
	Kyle Schultz
	Homework 1

	This file implements the Board class and Maze class
*/

#include "Maze.h"
#include <ctime>

/*
Returns a string emoji of the SquareType

@param sq The SquareType to returned as an emoji string.
@return The stringified emoji
*/
std::string SquareTypeStringify(SquareType sq)
{
	std::string square_string;
	switch(sq)
	{
		case SquareType::Wall		: square_string = "\u274C"; break;		//X emoji
		case SquareType::Human		: square_string = "\U0001F601"; break;	//Happy face emoji
		case SquareType::Exit		: square_string = "\u274E"; break;		//Checkmark emoji
		case SquareType::Empty		: square_string = "\u2B1C"; break;		//Blank square emoji
		case SquareType::Enemy		: square_string = "\U0001F40D"; break;	//Snake emoji
		case SquareType::Treasure	: square_string = "\u2728"; break;		//Sparckles emoji
		case SquareType::Cherries	: square_string = "\U0001F352"; break;	//Cherry emoji (2nd Treasure)
		case SquareType::Pizza		: square_string = "\U0001F355"; break;	//Pizza emoji (3rd Treasure)
		default 					: square_string = "\u2B1C"; break;		//Default == Empty
	}
	return square_string;
}

/*
	Default constructor for Board class

	did not implement because board is set to empty in maze constructor
*/
Board::Board()
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			arr_[i][j] = SquareType::Empty;
		}
		
	}
}

/*
Returns SquareType at the given position

@param Position of square where pos.row and pos.col are the indecies in the 2D array (maze)
@return SquareType of index in the 2D array
*/
SquareType Board::get_square_value(Position pos) const
{
	return arr_[pos.row][pos.col];
}

/*
Sets The SquareType of the given position in the array

@param Position of square where to set the SquareType, value SquareType to set the given position to
@return None
*/
void Board::SetSquareValue(Position pos, SquareType value)
{
	int sq_row = pos.row;
	int sq_col = pos.col;
	arr_[sq_row][sq_col] = value;

}


/*
Returns all possible Positions a player can move to in a vector

@param *p Player object to be moved (pointer)
@return vector of Positions that are valid moves for the player
*/
std::vector<Position> Board::GetMoves(Player *p)
{
	std::vector<Position> moves;
	Position player_pos = p->get_position();
	
	if((player_pos.row+1) < 4 && (arr_[player_pos.row+1][player_pos.col] != SquareType::Wall)) 	   //If move down is not out of bounds or a Wall
	{
		player_pos.row = player_pos.row+1;	//DOWN is a valid move
		moves.push_back(player_pos);
	}
	Position player_pos1 = p->get_position();
	if((player_pos1.col+1) < 4 && (arr_[player_pos1.row][player_pos1.col+1] != SquareType::Wall))  //If move right is not out of bounds or a Wall
	{
		player_pos1.col = player_pos1.col+1;	//RIGHT is a valid move
		moves.push_back(player_pos1);
	}
	Position player_pos2 = p->get_position();
	if((player_pos2.row-1) >= 0 && (arr_[player_pos2.row-1][player_pos2.col] != SquareType::Wall)) //If move up is not out of bouuds or a Wall
	{
		player_pos2.row = player_pos2.row-1;	//UP is a valid move
		moves.push_back(player_pos2);
	}
	Position player_pos3 = p->get_position();
	if((player_pos3.col-1) >= 0 && (arr_[player_pos3.row][player_pos3.col-1] != SquareType::Wall)) //If move left is not out of bouunds or a Wall
	{
		player_pos3.col = player_pos3.col-1;	//LEFT is a valid move
		moves.push_back(player_pos3);
	}
	return moves;
}

/*
Move a player to a specified positon, will always return true because of way it works with Maze::TakeTurn(Player *p)

@param *p Player object to be moved, pos Position to move the player to
@return Alwyas true because this function will only be called if a move if valid
*/
bool Board::MovePlayer(Player *p, Position pos)
{
	SquareType get_player_sq = get_square_value(p->get_position());	//Get the SqaureType of player being move. Will be Human or enemy
	SquareType move_to_sq = get_square_value(pos);				//Get SquareType of position being moved to

	if(get_square_value(pos) == SquareType::Treasure)			//if the SquareType of the position being moved to is Treasure
	{
		int points;
		points = p->get_points();
		points += 100;											//add 100 points to player
		p->ChangePoints(points);
	}
	if(get_square_value(pos) == SquareType::Cherries)			//if the SquareType of the position being moved to is Cherries
	{
		int points;
		points = p->get_points();
		points += 50;											//add 50 points to player for Cherries
		p->ChangePoints(points);
	}
	if(get_square_value(pos) == SquareType::Pizza)				//if the SquareType of the position being moved to is Pizza
	{
		int points;
		points = p->get_points();
		points += 75;											//add 75 points to player for Pizza
		p->ChangePoints(points);
	}
	if(p->is_human() == false && move_to_sq == SquareType::Enemy)	//if an Enemy moves into another enemy dont change the return true without moving
	{
		return true;
	}
	if (get_square_value(pos) == SquareType::Enemy)				//if a human moves into an Enemy
	{
		SetSquareValue(p->get_position(), SquareType::Empty);	//set the square player was on to Empty
		SetSquareValue(pos, SquareType::Enemy);					//and set the sqaure moved to as Enemy. IsGameOver() will handle checking if the
		p->SetPosition(pos);									//player has been eaten. Players new postiton is set
	}
	else if(p->is_human() == false && p->get_position().row == 3 && p->get_position().col == 3)	//if an enemy is in the exit square
	{
		SetSquareValue(p->get_position(), SquareType::Exit);	//Set the exit sqaure back to Exit
		SetSquareValue(pos, SquareType::Enemy);					//and move the enemy. 
		p->SetPosition(pos);
	}
	else{
		SetSquareValue(p->get_position(), SquareType::Empty);	//if the player moves to an open space or treasure set sqaure they were on to Empty
		SetSquareValue(pos, get_player_sq);							//set the new postion to whatever SquareType the player is
		p->SetPosition(pos);
	}

	
	return true;	//always return true
}

/*
	Returns the SquareType of the bottom right corner of the array where the exit is

	@param None
	@return SquareType of where the exit space is. Will be Human is the Human makes it to the exit.
*/
SquareType Board::GetExitOccupant()
{
	return arr_[3][3];
}

/*
	Overloaded operator used to print out Board objects

	@param os Output stream object, b Board object
	@return Output stream object to print the stringified Board/Maze
*/
std::ostream& operator<<(std::ostream& os, const Board &b)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			os << SquareTypeStringify(b.arr_[i][j]) << " ";	//convert each position in the array to equivalent SquareType
		}
		os << std::endl;
	}
	return os;
}

/*
	Maze default constructor. 
	Initializes the Maze's board to all Empty SquareTpyes 
*/
Maze::Maze()
{
	turn_count_ = 0;
	board_ = new Board();
}

/*
	Destructor for maze class.
	Deletes dynamically allocated memory for enemies.
*/
Maze::~Maze()
{
	turn_count_ = 0;
	for(unsigned int i = 1; i < players_.size(); i++)
	{
		delete players_[i];
	}
	delete board_;
}

/*
	Puts the Human and enemies in starting positions and generates random maze

	@param *human Player object that is human, enemies Number of enemies to be added to the board
	@return None
*/
void Maze::NewGame(Player *human, const int enemies)
{
	srand(time(NULL));				//seed time so srand() is always random

	Position human_pos;
	human_pos.row = 0; 
	human_pos.col = 0;
	human->SetPosition(human_pos);	//put human in top right corner
	players_.push_back(human);
	board_->SetSquareValue(human->get_position(), SquareType::Human);	

	Position exit_pos;
	exit_pos.row = 3;
	exit_pos.col = 3;
	board_->SetSquareValue(exit_pos, SquareType::Exit);	//put exit in bottom right corner

	for(int j = 0; j < 4; j++)	//loop through array to randomly add tresures to walls
	{
		for(int k = 0; k < 4; k++)
		{
			Position curr_p;
			curr_p.row = j;
			curr_p.col = k;
			if(board_->get_square_value(curr_p) != SquareType::Human && board_->get_square_value(curr_p) != SquareType::Exit)	//check if the position is alreay set as Human or exit
			{
				int probability = 2;					//Walls appear with 20% chance
				int result = std::rand() % 10;			//get an random int less than 
				if(result <= probability)				//if its less than or equal to 2, place the wall
				{
					board_->SetSquareValue(curr_p, SquareType::Wall);
				}
			}
			if(board_->get_square_value(curr_p) != SquareType::Human && board_->get_square_value(curr_p) != SquareType::Exit && board_->get_square_value(curr_p) != SquareType::Wall)	//check if the position is alreay set as Human or exit or wall
			{
				int probability2 = 1;				//Treasures appear with 10% chance
				int result2 = std::rand() % 10;		//get an random int less than 
				if(result2 <= probability2)			//if its less than or equal to 1, place the Treasure. Same method is applied for other 2 treasures (Cherries and Pizza). 
				{
					board_->SetSquareValue(curr_p, SquareType::Treasure);
				}
			}
			if(board_->get_square_value(curr_p) != SquareType::Human && board_->get_square_value(curr_p) != SquareType::Exit && board_->get_square_value(curr_p) != SquareType::Wall)
			{
				int probability2 = 1;
				int result2 = std::rand() % 10;
				if(result2 <= probability2)
				{
					board_->SetSquareValue(curr_p, SquareType::Cherries);
				}
			}
			if(board_->get_square_value(curr_p) != SquareType::Human && board_->get_square_value(curr_p) != SquareType::Exit && board_->get_square_value(curr_p) != SquareType::Wall)
			{
				int probability2 = 1;
				int result2 = std::rand() % 10;
				if(result2 <= probability2)
				{
					board_->SetSquareValue(curr_p, SquareType::Pizza);
				}
			}

		}
	}

	for(int i = 0; i < enemies; i++)										//add the number of enemies passed to the Maze
	{
		Player * enemy = new Player("Snake" + std::to_string(i), false);	//dynamically create a new Player so it can be add to the vector of Player pointers
		Position enemy_pos;													//enemies will be placed first at the top right row and wil go down horizontally towards bottom left
		enemy_pos.row = i;
		enemy_pos.col = 3-i;
		enemy->SetPosition(enemy_pos);
		players_.push_back(enemy);
		board_->SetSquareValue(enemy_pos, SquareType::Enemy);
	}


}

/*
	Shows the user all valid moves and promts them for choice. If it is valid case insensitive choice, the player is moved

	@param *p Player object whose turn it is
	@return None
*/
void Maze::TakeTurn(Player *p)
{
	std::vector<Position> player_moves = board_->GetMoves(p);

	std::cout << p->get_name() << " can go: ";							//print valid moves to user
	for(unsigned int i = 0; i < player_moves.size(); i++)
	{
		std::cout << p->ToRelativePosition(player_moves[i]) << " ";
	}
	std::cout << std::endl;
	std::string choice;
	std::cout << "Please enter your choice: ";							//user enters choice
	std::cin >> choice;
	std::string upper_choice;

	for(char c : choice)												
	{
		upper_choice += toupper(c);										//make the user's input all uppercase
	}

	if(player_moves.empty() == true)
	{
		return;															//no places to move
	}

	bool success = false;												//boolean to see if the given player has been moved. Initialized as false 
	for(Position pos : player_moves)									//because if will only become true if the user put in a valid input 
	{																	
		if(upper_choice == p->ToRelativePosition(pos))					//match user's choice with the string returned from player's ToRelativePosition function
		{
			success = board_->MovePlayer(p, pos);						//true if player moved successfully or enemy moved into another enemy
		}
	}
	
	if(success == true)													//increment count if player moved or enemy moved into another enemy
	{
		turn_count_++;
	}
	else
	{
		TakeTurn(p);													//call the function again if user did not have input
	}
	
}
/*
	Returns the player whos's turn it is

	@param None
	@return Player who's turn it is
*/
Player * Maze::GetNextPlayer()
{
	int player_num = 0;
	player_num = turn_count_ % 3;	//0 if human, 1 if first enemy, 2 if second enemy...
	return players_[player_num];
}

/*
	Returns true if a game ending condition has occured, false otherwise. Prints out final condition of board.
 
	@param None
	@return True or false on if there is a game ending contition
*/
bool Maze::IsGameOver()
{
	for (unsigned int i = 0; i < players_.size(); i++)		//loop through each player to check for game ending condition
	{
		if(players_[i]->is_human() == false && board_->get_square_value(players_[i]->get_position()) == SquareType::Human)	//if an enemy moves onto a square with a human
		{
			std::cout << *board_;
			return true;
		}
		else if(players_[i]->is_human() == true && board_->get_square_value(players_[i]->get_position()) == SquareType::Enemy)	//if a human moves onto a square with an enemy
		{

			std::cout << *board_;
			return true;
		}
		std::vector<Position> player_moves = board_->GetMoves(players_[i]);		//end the game if one of the players is trapped at the beginning of a new maze
		if(player_moves.empty() == true)
		{
			std::cout << *board_;
			return true;
		}
	}
	
	if(board_->GetExitOccupant() == SquareType::Human)							//if the human has reached the exit
	{
		int points;
		points = players_[0]->get_points();
		points++;																//add one point
		players_[0]->ChangePoints(points);
		std::cout << *board_;
		return true;
	}
	else																		
	{
		return false;
	}
}

/*
	Returns a string informing the user that the game is over and shows each players points

	@param None
	@return String of game over and players points
*/
std::string Maze::GenerateReport()
{
	std::string report;
	report = "Game Over!\n";
	for(Player * p : players_)
	{
		report += p->Stringify() + "\n";
	}

	
	return report;
}

/*
	Overloaded operator used to call the overloaded operator of Board

	@param os Output stream object, m Maze object
	@return Output stream object to cout the board from main
*/
std::ostream& operator<<(std::ostream& os, const Maze &m)
{
	os << *(m.board_);
	return os;
}








