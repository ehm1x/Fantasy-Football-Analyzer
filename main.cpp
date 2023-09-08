#include "classes.cpp"
#include "declare.h"

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
	tradeValueTable = tradeValueTable["playersListsCollections"][5]["playersList"];
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

	//now we can load users now that we know said league 

	std::cout << "Choose league by number\n";
	std::cin >> leagueNum;
	system("cls");
	std::cout << "Loading teams from league " << leaguesData[leagueNum - 1]["name"].get<std::string>() << "\n\n";
	auto leagueId = leaguesData[leagueNum - 1]["league_id"].get<std::string>();

	auto leagueUsers = load("https://api.sleeper.app/v1/league/" + leagueId + "/users");
	auto leagueTeams = load("https://api.sleeper.app/v1/league/" + leagueId + "/rosters");
	std::vector<Team> teams;
	for (const auto& userTeam : leagueUsers) { // get the team info this doesn't contain roster
		Team team(userTeam["display_name"], userTeam["user_id"]);
		for (const auto& roster : leagueTeams) { // get the roster
			if (roster["owner_id"] == userTeam["user_id"]) {
				for (const auto& player : roster["players"]) { // each starter in the roster 
					//declare stuff we will need to assemble player 

					const auto& currentPlayer = allPlayerData[player.get<std::string>()];
					std::string currentName = currentPlayer["first_name"].get<std::string>() + " " + currentPlayer["last_name"].get<std::string>();
					auto weeklyProj = load("https://api.sleeper.com/projections/nfl/player/" + currentPlayer["player_id"].get<std::string>() + "?season_type=regular&season=2023&grouping=week");
					auto seasonProj = load("https://api.sleeper.com/projections/nfl/player/" + currentPlayer["player_id"].get<std::string>() + "?season_type=regular&season=2023&grouping=season");
					std::vector<float>  projpts(19);
					for (const auto& week : weeklyProj) {
						if (week.is_object()) {
							int weekNum = week["week"].get<int>();
							projpts[weekNum] = week["stats"]["pts_ppr"].get<float>();
						}
					}
					float adp = seasonProj["stats"]["adp_ppr"].get<float>();
					float seasonTotal = seasonProj["stats"]["pts_ppr"].get<float>();
					std::string position = seasonProj["player"]["fantasy_positions"][0];
					std::string tradeIndex = tradeValueTable[tradeMap[currentName]][6];
					float tradeValue = std::stof(tradeIndex);
					Player player(currentName, adp, seasonTotal, position, projpts, tradeValue);
					team.roster.push_back(player);
					playerMap[currentName] = &player;
				}
			}

		}
		teams.push_back(team);
		std::cout << "[LOADED] team " << team.teamName << " has been loaded. \n";
	}

	std::map<float, Player*> qbMap;
	std::map<float, Player*> wrMap;
	std::map<float, Player*> rbMap;
	std::map<float, Player*> teMap;

	for (const auto& pair : playerMap) {
		auto player = pair.second; 
		if (player->position == "QB"){
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
	for (auto& team : teams) {
		std::cout << "============================================================================================\n";
		std::cout << "Team: " << team.teamName << "\n\n";
		std::cout << std::left << std::setw(10) << "Position" << std::setw(22) << "Name" << std::setw(10) << "ADP" << std::setw(15) << "Season Total" << std::setw(20) << "Avg PTS per Week" << std::setw(10) << "Trade Value:\n";
		std::sort(team.roster.begin(), team.roster.end(), [](const Player& a, const Player& b) {
			return a.position < b.position;
			});

		for (const auto& player : team.roster) {
			int color = DEFAULT;

			if (player.tradeValue < 9.0) {
				color = RED;
			}
			else if (player.tradeValue >= 10.0 && player.tradeValue < 25.0) {
				color = GREEN;
			}
			else if (player.tradeValue >= 25.0 && player.tradeValue < 50.0) {
				color = BLUE;
			}
			else if (player.tradeValue >= 50.0 && player.tradeValue < 75.0) {
				color = PURPLE;
			}
			else if (player.tradeValue >= 75.0) {
				color = YELLOW;
			}

			SetConsoleTextAttribute(hConsole, color); // Set the text color

			// Print player information
			std::cout << std::left << std::setw(10) << player.position << std::setw(22) << player.name << std::setw(10) << player.adp << std::setw(15) << player.projTotal << std::setw(20) << player.avgProj << std::setw(10) << player.tradeValue << "\n";

			// Reset text color to default
			SetConsoleTextAttribute(hConsole, DEFAULT);
		}


		std::cout << "\nAVERAGE TEAM ADP: " << std::fixed << std::setprecision(2) << team.find_adp() << "\n";
		std::cout << "AVERAGE PTS PER WEEK: " << std::fixed << std::setprecision(2) << team.avgWeekly() << "\n";
		std::cout << "TOTAL POINTS PROJECTED: " << std::fixed << std::setprecision(2) << team.calcTotalPts() << "\n";
		std::cout << "TOTAL TRADE VALUE OF TEAM: " << std::fixed << std::setprecision(2) << team.calcTotalTrade() << "\n\n";
	}


	for (auto it = qbMap.rbegin(); it != qbMap.rend(); ++it) {
		float tradeValue = it->first;
		Player* player = it->second;
		std::cout << "QB RANKINGS: \N";
		std::cout << std::setw(20) << player->name << std::setw(5) << tradeValue << "\n"; 
	}
	return 0;
}

