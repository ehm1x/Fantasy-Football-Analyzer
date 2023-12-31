#pragma once 
#include <vector>
#include <string> 
#include <iostream>
#include <sstream>


static const int RED = 12;
static const int GREEN = 10;
static const int BLUE = 9;
static const int PURPLE = 13;
static const int YELLOW = 14;
static const int DEFAULT = 7;
static const int DARK_YELLOW = 15;


class Player {
public:

	std::string name;
	float adp;
	std::vector<float> weeklyProj = {};
	std::vector<float> weeklyActualPts = {}; 
	float projTotal = 0;
	std::string position = "undef";
	float avgProj = 0;
	float tradeValue;;
	int positionalRanking = 0;
	int colorByTradeValue = DEFAULT;
	int colorByPosition = DEFAULT; 

	Player(std::string pname, const float adp1, const float projTotal1, std::string position1, std::vector<float> pointsProj, const float trade) {
		name = pname;
		adp = adp1;
		projTotal = projTotal1;
		position = position1;
		weeklyProj = pointsProj;
		avgProj = projTotal1 / 17;
		tradeValue = trade;
	}
	Player(std::string pname, const float adp1) {
		name = pname;
		adp = adp1;
	}
	Player() {
		name = "Unidentified ";
		adp = 0;
		projTotal = 0;
		position = "Undef";
	}

	void printWeekly() {
		int i = 1;
		std::cout << this->name << "\n";
		for (float x : this->weeklyProj) {
			std::cout << i << ".  " << x << "\n";
			i++;
		}
	}
};


class Team {
public:
	std::string teamName;
	float avgADP = 0;
	float totalPts = 0;
	float totalWeekly = 0.0; 
	std::vector<Player*> roster = {};
	std::string owner_id;
	float totalTradeValue = 0.0; 

	Team(std::string tName, std::string owner) {
		teamName = tName;
		owner_id = owner;
	}
	Team() {};

	void initPlayer(Player* player) {
		roster.push_back(player);
	}

	float find_adp() {
		int size = this->roster.size() - 1;
		float total = 0.0;
		for (auto& player : this->roster) {
			if (!player->adp) size--;
			total += player->adp;
		}
		if (size) total /= size;
		else {
			return 999.99f;
		}
		return total;
	}

	float calcTotalPts() {
		float total = 0;
		for (const auto& player : this->roster) {
			total += player->projTotal;
		}
		this->totalPts = total;
		return total;
	}

	float avgWeekly() {
		float avgWeek = 0;
		for (const auto& player : this->roster) {
			avgWeek += player->avgProj;
		}
		this->totalWeekly = avgWeek;
		return avgWeek;
	}

	float calcTotalTrade() {
		float total = 0.0;
		for (const auto& player : this->roster) {
			total += player->tradeValue;
		}
		this->totalTradeValue = total; 
		return total;
	}
	void destroyPlayers() {
		for (Player* player : roster) {
			delete player; 
		}
		roster.clear(); 
		roster.shrink_to_fit(); 
	}
};




