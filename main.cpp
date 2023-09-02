#include <nlohmann/json.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

nlohmann::json load(const std::string& url) {

	std::ostringstream responseData; 
	responseData << curlpp::options::Url(url);

	return nlohmann::json::parse(responseData.str());
}


int main() {
	
	curlpp::Cleanup myCleanup; // init curlpp

	std::string username;
	std::cout << "Enter your username \n";
	std::cin >> username;

	auto allPlayerData = load("https://api.sleeper.app/v1/players/nfl"); 

	auto userData = load("https://api.sleeper.app/v1/user/" + username);
	auto userId = userData["user_id"].get<std::string>();
	auto leaguesData = load("https://api.sleeper.app/v1/user/" + userId + "/leagues/nfl/2023");
	

	int leagueNum = 1;
	for (const auto& league : leaguesData) {

		std::cout << leagueNum << ". " << league["name"].get<std::string>() << " " << league["league_id"] << "\n";
		leagueNum++;
	}

	std::cout << "Choose league by number\n";
	std::cin >> leagueNum;
	std::cout << "Loading teams from league " << leaguesData[leagueNum-1]["name"] << "\n";
	auto leagueId = leaguesData[leagueNum - 1]["league_id"].get<std::string>(); 

	auto leagueTeams = load("https://api.sleeper.app/v1/league/" + leagueId + "/rosters"); 

	for (const auto& roster : leagueTeams) {
		if (roster["owner_id"] != userId) {
			continue;
		}
		for (const auto& player : roster["starters"]) {
			const auto& currentPlayer = allPlayerData[player.get<std::string>()]; 
			std::cout << currentPlayer["position"].get<std::string>() << " " << currentPlayer["first_name"].get<std::string>() << " " << currentPlayer["last_name"].get<std::string>() <<  "\n";
		}
		break; 
	}

	return 0;
}

