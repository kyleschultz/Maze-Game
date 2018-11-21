/*
	Kyle Schultz
	Homework 1

	This file implements the Player class
*/


#include "Player.h"
#include <sstream>
#include <string>

/*
	Constructor. Sets players name and if they are human

	@param name String name of a player, is_human Booloean if the player is a human or enemy
*/
Player::Player(const std::string name, const bool is_human)
{
	points_ = 0;
	name_ = name;
	is_human_ = is_human;
}

/*
	Changes the players points to the given value
	
	@param x Value of new points 
	@return None
*/
void Player::ChangePoints(const int x)
{
	points_ = x;
}

/*
	Sets the position of the player to the given position
	
	@param pos Position in the 2D array to move to
	@return None
*/
void Player::SetPosition(Position pos)
{
	pos_ = pos;
}

/*
	Returns a string version of the direction a player can move in the maze
	
	@param other Relative direction the player can move to
	@return String of the relative direction
*/
std::string Player::ToRelativePosition(Position other)
{
	std::string relative_position;
	if(pos_.row+1 == other.row)
	{
		relative_position = "DOWN";
	}
	if(pos_.col+1 == other.col)
	{
		relative_position = "RIGHT";
	}
	if(pos_.row-1 == other.row)
	{
		relative_position = "UP";
	}
	if(pos_.col-1 == other.col)
	{
		relative_position = "LEFT";
	}
	return relative_position;
}

/*
	
	
	
*/
std::string Player::Stringify()
{
	std::string name = get_name();
	int pts = get_points();
	std::stringstream ss;
	ss << pts;
	std::string pt_str = ss.str();
	std::string space = " ";
	std::string pts_promt = "Points: ";


	return (name + space + pts_promt + pt_str);

}