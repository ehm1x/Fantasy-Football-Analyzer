#include <vector>
#include <string>  
class Team; 


class Player {
public:

	std::string name; 
	float adp; 
	std::vector<int> projPoints = {};
	int projTotal = 0;
	std::string position = "undef";

	Player(std::string pname, const float adp1, const int projTotal1, std::string position1) {
		name = pname;
		adp = adp1;
		projTotal = projTotal1;
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
};


class Team {
public:
	std::string teamName;
	const float avgADP = 0;
	std::vector<Player> roster = {};
	std::string owner_id;

	Team(std::string tName, std::string owner) {
		teamName = tName;
		owner_id = owner;
	}
	Team() {}; 

	void initPlayer(Player player) {
		roster.push_back(player);
	}

	float find_adp() {
		int size = this->roster.size() - 1;
		float total = 0.0;
		for (auto& player : this->roster) {
			 player.adp += total;
		}
		if (size) total /= size; 
		else {
			return 999.99f; 
		}
		return total;
	}
};