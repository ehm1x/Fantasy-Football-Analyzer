
#include "helpers.h"
#include "classes.h"

int main() {

	printAsciiArt();
	std::cout << "                                   Press Enter to continue...";
	while (getchar() != '\n');

	curlpp::Cleanup myCleanup; // init curlpp
	std::string username;
	std::cout << "Enter your username \n";
	std::cin >> username;
	system("cls");
	std::cout << "Loading all leagues for user " << username << "\n";
	//load an uncessesary amount of shit 

	auto allPlayerData = loadLarge("https://api.sleeper.app/v1/players/nfl", "C:\\\\FantasyFolder\\allPlayerData.json", 86400);
	auto userData = load("https://api.sleeper.app/v1/user/" + username);
	auto userId = userData["user_id"].get<std::string>();
	auto leaguesData = load("https://api.sleeper.app/v1/user/" + userId + "/leagues/nfl/2023");
	auto tradeValueTable = load("https://statics.sportskeeda.com/skm/assets/trade-analyzer/players-json-list/v2/playersLists.json");
	int leagueType = -1;
	for (int i = 0; i <= 11; i++) {
		if (tradeValueTable["playersListsCollections"][i]["sheetName"] == "redraft___1qb_ppr") {
			leagueType = i;
			std::cout << "[SUCCESS] Found league type. \n";
		}
	}
	if (leagueType == -1) std::cout << "[ERROR] Could not find league type please restart. \n";
	tradeValueTable = tradeValueTable["playersListsCollections"][leagueType]["playersList"];
	const std::vector<std::vector<int>> positionalScarcityAll = { {3, 7, 11, 10, 10}, {4, 9, 17, 27, 39}, {4, 9, 15, 21, 31}, {1, 3, 7, 13, 12} };
	std::unordered_map<std::string, Player*> playerMap;
	std::unordered_map<std::string, int> tradeMap;
	for (int i = 1; i < tradeValueTable.size(); i++) {
		const auto& item = tradeValueTable[i];
		if (item.is_array() && item.size() > 0) {
			tradeMap[item[0]] = i;
		}
	}

	//loop all leagues 
	int leagueNum = 1;
	for (const auto& league : leaguesData) {

		std::cout << leagueNum << ". " << league["name"].get<std::string>() << "\n";
		leagueNum++;
	}

	std::cout << "Choose league by number\n";
	std::cin >> leagueNum;
	system("cls");
	std::cout << "Loading teams from league " << leaguesData[leagueNum - 1]["name"].get<std::string>() << "\n\n";
	auto leagueId = leaguesData[leagueNum - 1]["league_id"].get<std::string>();

	auto leagueUsers = load("https://api.sleeper.app/v1/league/" + leagueId + "/users");
	auto leagueTeams = load("https://api.sleeper.app/v1/league/" + leagueId + "/rosters");
	std::vector<Team> teams = constructTeams(leagueUsers, leagueTeams, allPlayerData, tradeValueTable, &tradeMap, &playerMap);

	std::map<float, Player*> qbMap;
	std::map<float, Player*> wrMap;
	std::map<float, Player*> rbMap;
	std::map<float, Player*> teMap;

	for (const auto& pair : playerMap) {
		auto player = pair.second;
		if (player->position == "QB") {
			qbMap[player->tradeValue] = player;
		}
		else if (player->position == "WR") {
			wrMap[player->tradeValue] = player;
		}
		else if (player->position == "RB") {
			rbMap[player->tradeValue] = player;
		}
		else if (player->position == "TE") {
			teMap[player->tradeValue] = player;
		}
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	while (true) {
		int input;
		std::cout << "\n\n\nPRESS ENTER TO EXIT\n";
		std::cout << "PRESS 1 FOR RANKINGS \n";
		std::cout << "PRESS 2 FOR ALL TEAMS \n";
		std::cout << "PRESS 3 FOR ANALYZER\n";
		std::cout << "PRESS 4 FOR REAL ANALYZER\n";
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
		default: break;
		}
	}


	return 0;
}
