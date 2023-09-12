
#include "classes.h"
#include "helpers.h"

int main() {

	printAsciiArt();
	std::cout << "                                   Press Enter to continue...";
	while (getchar() != '\n');

	curlpp::Cleanup myCleanup; // init curlpp
	std::string username = getUsername();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int noWait = 0; 

	auto allPlayerData = loadLarge("https://api.sleeper.app/v1/players/nfl", "C:\\\\FantasyFolder\\allPlayerData.json", 86400);
	auto userData = loadLarge("https://api.sleeper.app/v1/user/" + username, "C:\\\\FantasyFolder\\userData.json", 86400);
	auto userId = userData["user_id"].get<std::string>();
	auto leaguesData = loadLarge("https://api.sleeper.app/v1/user/" + userId + "/leagues/nfl/2023", "C:\\\\FantasyFolder\\leagueData.json", 86400);
	auto tradeValueTable = loadLarge("https://statics.sportskeeda.com/skm/assets/trade-analyzer/players-json-list/v2/playersLists.json", "C:\\\\FantasyFolder\\tradeValueData.json", 21600);
	findLeagueType(tradeValueTable); 
	const std::vector<std::vector<int>> positionalScarcityAll = { {3, 7, 11, 10, 10}, {4, 9, 17, 27, 39}, {4, 9, 15, 21, 31}, {1, 3, 7, 13, 12} };
	std::unordered_map<std::string, Player*> playerMap;
	std::unordered_map<std::string, int> tradeMap;
	loadTradeMap(tradeValueTable, tradeMap);
	int leagueNum = leagueSelection(leaguesData); 
	
	auto leagueId = leaguesData[leagueNum - 1]["league_id"].get<std::string>();
	auto leagueUsers = load("https://api.sleeper.app/v1/league/" + leagueId + "/users");
	auto leagueTeams = load("https://api.sleeper.app/v1/league/" + leagueId + "/rosters");

	std::vector<Team> teams = constructTeams(leagueUsers, leagueTeams, allPlayerData, tradeValueTable, tradeMap, &playerMap, 86400);

	std::map<float, Player*> qbMap;
	std::map<float, Player*> wrMap;
	std::map<float, Player*> rbMap;
	std::map<float, Player*> teMap;

	mapInit(qbMap, wrMap, rbMap, teMap, playerMap); 

	while (true) {
		int input;
		std::cout << "\n\n\nPRESS ENTER TO EXIT\n";
		std::cout << "PRESS 1 FOR RANKINGS \n";
		std::cout << "PRESS 2 FOR ALL TEAMS \n";
		std::cout << "PRESS 3 FOR ANALYZER\n";
		std::cout << "PRESS 4 FOR REAL ANALYZER\n";
		std::cout << "PRESS 5 FOR TEAM RANKINGS:\n";
		std::cin >> input;
		switch (input) {
		case 1: rankings(qbMap, wrMap, rbMap, teMap, hConsole, positionalScarcityAll);
			break;
		case 2: printTeams(teams, hConsole);
			break;
		case 3: tradeAnalyzer(playerMap, tradeMap, tradeValueTable, hConsole);
			break;
		case 4: realTradeAnalyzer(teams, userId, hConsole);
			break;
		case 5: teamRankings(teams, hConsole);
			break;
		case 6: reloadTeams(teams, qbMap, wrMap, rbMap, teMap, playerMap, leagueUsers, leagueTeams, allPlayerData, tradeValueTable, &tradeMap, 1);
		default: break;
		}
	}
	return 0;
}