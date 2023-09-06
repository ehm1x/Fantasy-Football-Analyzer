#include <nlohmann/json.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "classes.cpp"



nlohmann::json load(const std::string& url) {

	std::ostringstream responseData;
	responseData << curlpp::options::Url(url);

	return nlohmann::json::parse(responseData.str());
}


int main() {

	curlpp::Cleanup myCleanup; // init curlpp
	Team team("gg", "gg");
	std::string username;
	std::cout << "Enter your username \n";
	std::cin >> username;

	//load an uncessesary amount of shit 
	auto allPlayerData = load("https://api.sleeper.app/v1/players/nfl");
	auto allADP = load("https://fantasyfootballcalculator.com/api/v1/adp/ppr?teams=12&year=2023&position=all");
	auto userData = load("https://api.sleeper.app/v1/user/" + username);
	auto userId = userData["user_id"].get<std::string>();
	auto leaguesData = load("https://api.sleeper.app/v1/user/" + userId + "/leagues/nfl/2023");

	//loop all leagues 
	int leagueNum = 1;
	for (const auto& league : leaguesData) {

		std::cout << leagueNum << ". " << league["name"].get<std::string>() << " " << league["league_id"] << "\n";
		leagueNum++;
	}

	//now we can load users now that we know said league 
	

	std::cout << "Choose league by number\n";
	std::cin >> leagueNum;
	std::cout << "Loading teams from league " << leaguesData[leagueNum - 1]["name"] << "\n";
	auto leagueId = leaguesData[leagueNum - 1]["league_id"].get<std::string>();

	std::cout << "I made the auto leaguue id \n";
	//std::string league_id = leaguesData[leagueNum - 1]["league_id"];
	auto leagueUsers = load("https://api.sleeper.app/v1/league/" + leagueId + "/users");
	auto leagueTeams = load("https://api.sleeper.app/v1/league/" + leagueId + "/rosters");
	
	std::cout << "I loaded leaguesusers and league teams \n";

	std::vector<Team> teams; 
	std::cout << "I made the teams vector \n";

	for (const auto& userTeam : leagueUsers) { // get the team info this doesn't contain roster
		/*std::cout << userTeam["metadata"]["team_name"]; 
		std::cout << userTeam["user_id"];*/ 
		Team team("test", userTeam["user_id"]);
		std::cout << "I made the first team object \n"; 
		for (const auto& roster : leagueTeams) { // get the roster
			std::cout << " I got the roster from league teams \n"; 
			for (const auto& player : roster["starters"]) { // each starter in the roster 
				//declare stuff we will need to assemble player 
				std::cout << "I got the player from the roster \n"; 
				float adp = 0.0f; 
				const auto& currentPlayer = allPlayerData[player.get<std::string>()];
				std::string currentName = currentPlayer["first_name"].get<std::string>() + " " + currentPlayer["last_name"].get<std::string>();
				//std::cout << currentPlayer["position"].get<std::string>() << " " << currentName;
			
				for (const auto& player : allADP["players"]) { // loop to find players in my other dataset via name 
					if (player["name"] == (currentPlayer["first_name"].get<std::string>() + " " + currentPlayer["last_name"].get<std::string>())) {
						std::cout << " " << "ADP : " << player["adp"] << "\n";
						adp = player["adp"]; 
					}
				}
				Player player(currentName, adp); 
				team.roster.push_back(player); 
			}
			break;
		}
		teams.push_back(team); 
	}

	for (auto& team : teams) {
		std::cout << team.teamName << " \n\n";
		for (const auto& player : team.roster) {
			std::cout << player.name << " " << player.adp << "\n"; 
		}
		std::cout << "AVERAGE TEAM ADP: " << team.find_adp(); 
		std::cout << "\n\n"; 
	}
	

	return 0;
}
