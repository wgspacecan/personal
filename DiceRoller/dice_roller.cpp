// dice roller
// William Gregory

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>

// dice settings
int number_dice = 1;  // number of dice to roll
int dice_max = 6.0;  // max roll
int dice_min = 1.0;  // min roll

// mode
// 1: user roll
// 2: distribution
int mode = 1;  // mode select

// user roll mode settings
bool ask_roll = true;  // ask before each roll to roll again or not

// distribution mode settings
int run_number = 1000000;
bool display_dist = true;
bool display_rolls = false;

// -----------------------------------

// Generate random dice roll between min and max.
int roll_dice()
{
	int roll_total = 0;
	for (int x=0; x<number_dice; x++)
	{
		int roll = dice_min + (rand() % dice_max);
		roll_total += roll;
	}
	return roll_total;
}

// loop of rolling dice with user check
void run_dice_roll()
{
	bool run_dice = true;
	while (run_dice)
	{
		int roll = roll_dice();
		std::cout << std::endl;
		std::cout << "roll: " << roll << std::endl;
		if (ask_roll)
		{
			// roll again?
			std::string user_val;
			std::cout << std::endl << "roll again? (y or n): ";
			std::cin >> user_val;
			if (user_val == "n")
			{
				run_dice = false;
			}
			else if (user_val == "y") {}
			else
			{
				std::cout << "error with input" << std::endl;
			}
		}
	}
}

void dice_dist()
{
	// roll
	time_t start_time = time(NULL);
	std::vector <int> rolls;
	for (int x=0; x<run_number; x++)
	{
		int temp_roll = roll_dice();
		rolls.push_back(temp_roll);
		if (display_rolls) std::cout << temp_roll << std::endl;
	}
	// find max_val
	int max_val = 0;
	for (int x=0; x<rolls.size(); x++)
	{
		if (rolls.at(x) > max_val) max_val = rolls.at(x);
	}
	// create empty vector size max_val
	std::vector <int> dist;
	for (int x=0; x<max_val; x++)
	{
		dist.push_back(0);
	}
	// map dist
	for (int x=0; x<rolls.size(); x++)
	{
		dist.at(rolls.at(x)-dice_min) += 1;
	}
	// find mean
	double test_mean = 0;
	for (int x=0; x<rolls.size(); x++)
	{
		test_mean += rolls.at(x);
	}
	test_mean = test_mean / rolls.size();
	// desired mean
	double desired_mean = 0;
	for (int x=dice_min; x<=dice_max; x++)
	{
		desired_mean += x;
	}
	desired_mean = desired_mean / (dice_max - (dice_min-1));
	// find mode
	int test_most_common = 0;
	int test_mode = 0;
	for (int x=0; x< dist.size(); x++)
	{
		if (dist.at(x) > test_most_common)
		{
			test_mode = x+dice_min;
			test_most_common = dist.at(x);
		}
	}
	// display
	std::cout << "--------------------" << std::endl;
	std::cout << "Results: " << std::endl;
	std::cout << "min/max: " << dice_min << "/" << dice_max << std::endl;
	std::cout << "rolls: " << run_number << std::endl;
	std::cout << "time taken: " << time(NULL)-start_time << " seconds" << std::endl;
	std::cout << "------------" << std::endl;
	std::cout << "mean (average): " << test_mean << "; desired: " << desired_mean << std::endl;
	std::cout << "mode (most common): " << test_mode << std::endl;
	if (display_dist)
	{
		std::cout << "------------" << std::endl;
		for (int x=0; x<dist.size(); x++)
		{
			std::cout << x+dice_min << ":" << dist.at(x) << " rolls";
			std::cout << "; " << ((double)dist.at(x)*100/run_number) << "%" << std::endl;
		}
	}
	std::cout << "--------------------" << std::endl;
}

int main()
{
	srand(time(NULL));
	if (mode == 1) run_dice_roll();
	else if (mode == 2) dice_dist();
	else std::cout << "Error: mode unknown." << std::endl;
	return 0;
}
