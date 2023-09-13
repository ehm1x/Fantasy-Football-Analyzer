
#include "helpers.h"
#include "classes.h"

void printAsciiArt() {
	std::cout << " ___  ________  ___          ________ ________  ________   _________  ________  ________       ___    ___\n";
	std::cout << "|\\  \\|\\_____  \\|\\  \\        |\\  _____\\\\   __  \\|\\   ___  \\\\___   ___\\\\   __  \\|\\   ____\\     |\\  \\  /  /|\n";
	std::cout << "\\ \\  \\|___/  / /\\ \\  \\       \\ \\  \\__/\\ \\  \\|\\  \\ \\  \\\\ \\  \\|___ \\  \\_\\ \\  \\|\\  \\ \\  \\___|_    \\ \\  \\/  / /\n";
	std::cout << " \\ \\  \\   /  / /\\ \\  \\       \\ \\   __\\\\ \\   __  \\ \\  \\\\ \\  \\   \\ \\  \\ \\ \\   __  \\ \\_____  \\    \\ \\    / /\n";
	std::cout << "  \\ \\  \\ /  /_/__\\ \\  \\       \\ \\  \\_| \\ \\  \\ \\  \\ \\  \\\\ \\  \\   \\ \\  \\ \\ \\  \\ \\  \\|____|\\  \\    \\/  /  /\n";
	std::cout << "   \\ \\__\\\\________\\ \\__\\       \\ \\__\\   \\ \\__\\ \\__\\ \\__\\\\ \\__\\   \\ \\__\\ \\ \\__\\ \\__\\____\\_\\  __/  / /\n";
	std::cout << "    \\|__|\\|_______|\\|__|        \\|__|    \\|__|\\|__|\\|__| \\|__|    \\|__|  \\|__|\\|__|\\_________\\/___/ /\n";
	std::cout << "                                                                                  \\|_________\\|___|/ \n";
}

std::time_t getFileLastModifiedTime(const std::string& filePath) {
	std::filesystem::path path(filePath);
	if (!std::filesystem::exists(path)) {
		return 0;
	}
	const auto fileTime = std::filesystem::last_write_time(filePath);
	const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fileTime);
	const auto time = std::chrono::system_clock::to_time_t(systemTime);
	return time;
}

void printAllTeams(std::vector<Team>& teams, HANDLE& hConsole) {
	for (auto& team : teams) {
		printTeam(team, hConsole); 
	}
}

void myTeam(std::vector<Team>& teams, std::string user_id, HANDLE& hConsole) {
	for (auto& team : teams) {
		if (team.owner_id == user_id) {
			printTeam(team, hConsole); 
		}
	}
}

void printTeam(Team& team, HANDLE& hConsole) {
	std::cout << "Team: " << team.teamName << "\n\n";
	std::cout << std::left << std::setw(10) << "Position" << std::setw(22) << "Name" << std::setw(10) << "ADP" << std::setw(15) << "Season Total" << std::setw(20) << "Avg PTS per Week" << std::setw(10) << "Trade Value:\n";
	std::sort(team.roster.begin(), team.roster.end(), [](const Player* a, const Player* b) {
		return a->position < b->position;
		});

	for (const auto& player : team.roster) {
		SetConsoleTextAttribute(hConsole, player->colorByTradeValue);
		std::cout << std::left << std::setw(10) << player->position << std::setw(22) << player->name << std::setw(10) << player->adp << std::setw(15) << player->projTotal << std::setw(20) << player->avgProj << std::setw(10) << player->tradeValue << "\n";
		SetConsoleTextAttribute(hConsole, DEFAULT);
	}


	std::cout << "\nAVERAGE TEAM ADP: " << std::fixed << std::setprecision(2) << team.find_adp() << "\n";
	std::cout << "AVERAGE PTS PER WEEK: " << std::fixed << std::setprecision(2) << team.avgWeekly() << "\n";
	std::cout << "TOTAL POINTS PROJECTED: " << std::fixed << std::setprecision(2) << team.calcTotalPts() << "\n";
	std::cout << "TOTAL TRADE VALUE OF TEAM: " << std::fixed << std::setprecision(2) << team.calcTotalTrade() << "\n\n";
}

void tradeAnalyzer(std::unordered_map<std::string, Player*>& playerMap, std::unordered_map<std::string, int>& tradeMap, nlohmann::json& tradeValueTable, HANDLE& hConsole) {
	std::vector<Player*> team1;
	std::vector<Player*> team2;
	while (true) {
		std::string playerName;
		std::cout << "\nEnter Player name to give away or press 1 to move to recieving: ";
		if (std::cin.peek() == '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		std::getline(std::cin, playerName);
		if (playerName == "1")break;
		else {
			if (playerMap[playerName]) {
				Player* currentPlayer = playerMap[playerName];
				team1.push_back(currentPlayer);
				std::cout << playerName << " has been added to team 1";
			}
			else {
				std::cout << "\nInvalid Name, try agian please \n";
			}
		}
	}
	while (true) {
		std::string playerName;
		std::cout << "\nEnter Player name to recieve or press 1 to analzye: ";
		if (std::cin.peek() == '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		std::getline(std::cin, playerName);
		if (playerName == "1")break;
		else {
			if (playerMap[playerName]) {
				Player* currentPlayer = playerMap[playerName];
				team2.push_back(currentPlayer);
				std::cout << playerName << " has been added to team 2\n";
			}
			else {
				std::cout << "\nInvalid Name, try agian please \n";
			}
		}
	}
	float team1Trade = 0;
	float team1ppw = 0;

	float team2Trade = 0;
	float team2ppw = 0;

	for (const auto& player : team1) {
		team1Trade += player->tradeValue;
		team1ppw += player->avgProj;
	}
	for (const auto& player : team2) {
		team2Trade += player->tradeValue;
		team2ppw += player->avgProj;
	}

	system("cls");
	displayTeamStats(team1, "TEAM 1", hConsole);
	displayTeamStats(team2, "TEAM 2", hConsole);

	std::cout << std::left << std::setw(12) << "Team 1 Trade Value: " << std::setw(7) << team1Trade << std::setw(4) << "PPG: " << team1ppw << "\n";
	std::cout << std::left << std::setw(12) << "Team 2 Trade Value: " << std::setw(7) << team2Trade << std::setw(4) << "PPG: " << team2ppw << "\n\n";
}

void displayAndSelectRoster(const Team& team, const std::string& teamName, std::vector<Player*>& selectedPlayers, HANDLE& hConsole) {
	std::cout << "Roster for " << teamName << ":\n";
	for (int i = 0; i < team.roster.size(); i++) {
		Player* player = team.roster[i];
		
		setConsoleColor(hConsole, player->colorByTradeValue);
		std::cout << std::left << std::setw(2) << i + 1 << ". " << std::setw(15) << player->name << " " << std::setw(13) << "TRADE VALUE: " << std::setw(4) << player->tradeValue << "\n";
		resetConsoleColor(hConsole);
	}

	int selection;
	do {
		std::cout << "Enter the number of the player you want to add to the trade (0 to finish): ";
		std::cin >> selection;
		if (selection == 0) break;
		selectedPlayers.push_back(team.roster[selection - 1]);
		Player* player = team.roster[selection - 1];
		std::cout << "\nSuccessfully added ";
		setConsoleColor(hConsole, player->colorByTradeValue);
		std::cout << player->name;
		resetConsoleColor(hConsole);
		std::cout << "\n";
	} while (true);
}

void calculateTeamStats(const std::vector<Player*>& team, float& teamTradeValue, float& teamPPW) {
	teamTradeValue = 0;
	teamPPW = 0;
	for (const auto& player : team) {
		teamTradeValue += player->tradeValue;
		teamPPW += player->avgProj;
	}
}

void realTradeAnalyzer(std::vector<Team>& teams, std::string& userId, HANDLE& hConsole) {
	std::vector<Player*> team1;
	std::vector<Player*> team2;

	// Find and select the user's team
	for (const auto& team : teams) {
		if (team.owner_id == userId) {
			std::cout << "YOUR TEAM \n";
			displayAndSelectRoster(team, "Your Team", team1, hConsole);
		}
	}

	// Select a trade partner
	std::cout << "SELECT THE TEAM YOU WANT TO TRADE WITH \n";
	int partnerIndex;
	for (int i = 0; i < teams.size(); i++) {
		std::cout << i + 1 << ". " << teams[i].teamName << "\n";
	}
	std::cin >> partnerIndex;
	auto tradePartner = teams[partnerIndex - 1];

	
	displayAndSelectRoster(tradePartner, "Trade Partner's Team", team2, hConsole);

	float team1Trade = 0;
	float team1ppw = 0;
	float team2Trade = 0;
	float team2ppw = 0;

	
	calculateTeamStats(team1, team1Trade, team1ppw);
	calculateTeamStats(team2, team2Trade, team2ppw);

	system("cls");
	displayTeamStats(team1, "TEAM 1", hConsole);
	displayTeamStats(team2, "TEAM 2", hConsole);

	std::cout << std::left << std::setw(12) << "Team 1 Trade Value: " << std::setw(7) << team1Trade << std::setw(4) << "PPG: " << team1ppw << "\n";
	std::cout << std::left << std::setw(12) << "Team 2 Trade Value: " << std::setw(7) << team2Trade << std::setw(4) << "PPG: " << team2ppw << "\n\n";
}

void displayTeamStats(const std::vector<Player*>& team, const std::string& teamName, HANDLE& hConsole) {
	std::cout << "\n" << teamName << ":\n";
	std::cout << std::left << std::setw(15) << "Name" << std::setw(4) << "POS " << std::setw(8) << "Trade" << "PPG\n";

	for (const auto& player : team) {
		int color = player->colorByTradeValue;
		SetConsoleTextAttribute(hConsole, color);
		std::cout << std::left << std::setw(15) << player->name << std::fixed << std::setw(4) << player->position << std::setprecision(2) << std::setw(8) << player->tradeValue << std::setw(10) << player->avgProj << "\n";
		SetConsoleTextAttribute(hConsole, DEFAULT);
	}
	std::cout << "\n\n";
}

std::vector<Team> constructTeams(nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>& tradeMap, std::unordered_map<std::string, Player*>* playerMap, int wait, HANDLE& hConsole) {
	std::vector<Team> teams;
	for (const auto& userTeam : leagueUsers) {
		Team team(userTeam["display_name"], userTeam["user_id"]);
		for (const auto& roster : leagueTeams) {
			if (roster["owner_id"] == userTeam["user_id"]) {
				int totalPlayers = roster["players"].size();
				int playerCount = 0;
				for (const auto& player : roster["players"]) {
					// Print loading progress
					std::cout << "[LOADING] Player " << (playerCount + 1) << " of " << totalPlayers << "\r";
					std::cout.flush(); // Flush the output buffer

					const auto& currentPlayer = allPlayerData[player.get<std::string>()];
					std::string currentName = currentPlayer["first_name"].get<std::string>() + " " + currentPlayer["last_name"].get<std::string>();
					auto weeklyProj = loadLarge("https://api.sleeper.com/projections/nfl/player/" + currentPlayer["player_id"].get<std::string>() + "?season_type=regular&season=2023&grouping=week", "C:\\\\FantasyFolder\\playerData\\"+ currentPlayer["player_id"].get<std::string>() + "weekly.json", wait);
					auto seasonProj = load("https://api.sleeper.com/projections/nfl/player/" + currentPlayer["player_id"].get<std::string>() + "?season_type=regular&season=2023&grouping=season");
					std::vector<float> projpts(19);
					for (const auto& week : weeklyProj) {
						if (week.is_object()) {
							int weekNum = week["week"].get<int>();
							projpts[weekNum] = week["stats"]["pts_ppr"].get<float>();
						}
					}
					float adp = seasonProj["stats"]["adp_ppr"].get<float>();
					float seasonTotal = 0; 
					if (seasonProj["stats"].contains("pts_ppr")) {
						seasonTotal = seasonProj["stats"]["pts_ppr"].get<float>();
					}
					std::string position = seasonProj["player"]["fantasy_positions"][0];
					int tradeIndex = tradeMap[currentName];
					std::string tradeFloat = tradeValueTable[tradeIndex][6];
					float tradeValue = std::stof(tradeFloat);
					Player* player = new Player(currentName, adp, seasonTotal, position, projpts, tradeValue);
					team.roster.push_back(player);
					(*playerMap)[currentName] = player; // Use arrow operator to access the map
					playerCount++;
					std::cout << "\x1b[K";
				}
			}
		}
		teams.push_back(team);
		SetConsoleTextAttribute(hConsole, GREEN);
		std::cout << "[LOADED] ";
		SetConsoleTextAttribute(hConsole, DEFAULT);
		std::cout << "Team " << team.teamName << " has been loaded. " << std::endl;
	}
	return teams;
}

bool fileOutdated(const std::string& filePath, std::time_t maxAgeSeconds) {
	std::time_t lastModifiedTime = getFileLastModifiedTime(filePath);
	if (lastModifiedTime == 0) {
		return true;
	}

	std::time_t currentTime = std::time(nullptr);
	return (currentTime - lastModifiedTime) > maxAgeSeconds;
}
nlohmann::json loadFromFile(const std::string& filePath) {
	try {
		std::ifstream ifs(filePath);
		if (!ifs.is_open()) {
			throw std::runtime_error("Failed to open file: " + filePath);
		}

		nlohmann::json jsonData;
		ifs >> jsonData;
		ifs.close();

		return jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error loading data from file: " << e.what() << std::endl;
		exit(1);
	}
}
nlohmann::json loadLarge(const std::string& url, const std::string& filePath, time_t maxAgeSeconds) {
	try {
		if (!fileOutdated(filePath, maxAgeSeconds)) {
			return loadFromFile(filePath);
		}

		std::ostringstream responseData;
		responseData << curlpp::options::Url(url);
		nlohmann::json jsonData = nlohmann::json::parse(responseData.str());

		createFile(filePath, jsonData);
		return jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error loading data: " << e.what() << std::endl;
		exit(1);
	}
}

bool fileExists(const std::string& filePath) {
	return std::filesystem::exists(filePath);
}

void createFile(const std::string& filePath, const nlohmann::json& data) {
	std::filesystem::path path(filePath);
	std::filesystem::create_directories(path.parent_path());
	std::ofstream ofs(path);
	ofs << data;
	ofs.close();
}

nlohmann::json load(const std::string& url) {
	try {
		std::ostringstream responseData;
		responseData << curlpp::options::Url(url);
		return nlohmann::json::parse(responseData.str());
	}
	catch (const std::exception& e) {
		std::cerr << "Error loading data: " << e.what() << std::endl;
		exit(1);
	}
}

void setConsoleColor(HANDLE hConsole, int color) {
	SetConsoleTextAttribute(hConsole, color);
}

void resetConsoleColor(HANDLE hConsole) {
	SetConsoleTextAttribute(hConsole, DEFAULT);
}

//void teamRankings(std::vector<Team>& teams, HANDLE hConsole) {
//	std::unordered_map<float, std::string> leagueTrade;
//	std::unordered_map<float, std::string> leaguePoints;
//
//	for (auto& roster : teams) {
//		leagueTrade[roster.calcTotalTrade()] = roster.teamName;
//		leaguePoints[roster.calcTotalPts()] = roster.teamName;
//	}
//
//	std::map<float, std::string> sortedTrade(leagueTrade.begin(), leagueTrade.end());
//	std::map<float, std::string> sortedPoints(leaguePoints.begin(), leaguePoints.end());
//
//	int rank = 1;
//	for (const auto& pair : sortedTrade) {
//		int color = DEFAULT; // Default color
//		if (rank == 1) {
//			color = YELLOW; // Redefine the meaning of GOLD based on your color codes
//		}
//		else if (rank == 2 || rank == 3) {
//			color = PURPLE;
//		}
//		else if (rank >= 4 && rank <= 7) {
//			color = BLUE;
//		}
//		else if (rank >= 8 && rank <= 10) {
//			color = GREEN;
//		}
//		else if (rank >= 11 && rank <= 12) {
//			color = RED;
//		}
//
//		setConsoleColor(hConsole, color); // Set the text color using the provided console handle
//		std::cout << std::setw(10) << std::left << pair.second << ": " << std::setw(8) << std::right << pair.first << std::endl;
//		resetConsoleColor(hConsole); // Reset the text color using the provided console handle
//		rank++;
//	}
//
//	rank = 1;
//	for (const auto& pair : sortedPoints) {
//		int color = DEFAULT; // Default color
//		if (rank == 1) {
//			color = YELLOW; // Redefine the meaning of GOLD based on your color codes
//		}
//		else if (rank == 2 || rank == 3) {
//			color = PURPLE;
//		}
//		else if (rank >= 4 && rank <= 7) {
//			color = BLUE;
//		}
//		else if (rank >= 8 && rank <= 10) {
//			color = GREEN;
//		}
//		else if (rank >= 11 && rank <= 12) {
//			color = RED;
//		}
//
//		setConsoleColor(hConsole, color); // Set the text color using the provided console handle
//		std::cout << std::setw(10) << std::left << pair.second << ": " << std::setw(8) << std::right << pair.first << std::endl;
//		resetConsoleColor(hConsole); // Reset the text color using the provided console handle
//		rank++;
//	}
//}

void initTeamTotals(std::vector<Team>& teams) {
	for (auto& team : teams) {
		team.calcTotalPts();
		team.calcTotalTrade(); 
	}
}

struct CompareByTrade {
	bool operator()(const Player* p1, const Player* p2) {
		return p1->tradeValue > p2->tradeValue;
	}
};

struct CompareByPoints {
	bool operator()(const Team& t1, const Team& t2) {
		return t1.totalPts > t2.totalPts;
	}
};

struct CompareTeamsByPoints {
	bool operator()(Team& t1, Team& t2) {
		return t1.calcTotalPts() < t2.calcTotalPts();
	}
};

struct CompareTeamsByTrade {
	bool operator()(Team& t1, Team& t2) {
		return t1.calcTotalTrade() < t2.calcTotalTrade();
	}
};

void teamRankings(std::vector<Team>& teams, HANDLE hConsole) {

	std::priority_queue<Team, std::vector<Team>, CompareTeamsByTrade> tradeRankings;

	std::priority_queue<Team, std::vector<Team>, CompareTeamsByPoints> pointRankings;

	for (Team& team : teams) {
		tradeRankings.push(team);
		pointRankings.push(team);
	}

	std::cout << "\nTRADE VALUE RANKINGS:\n\n";

	int rank = 1;

	while (!tradeRankings.empty()) {
		Team team = tradeRankings.top();
		tradeRankings.pop();

		int color = DEFAULT;
		if (rank <= 3) {
			color = YELLOW;
		}
		else if (rank <= 7) {
			color = PURPLE;
		}
		else if (rank <= 10) {
			color = BLUE;
		}
		else {
			color = GREEN;
		}

		setConsoleColor(hConsole, color);
		std::cout << std::left << std::setw(2) << rank << ". " << std::setw(18) << team.teamName<< std::setw(8) << team.calcTotalTrade() << "\n";
		resetConsoleColor(hConsole);

		rank++;
	}


	rank = 1;
	std::cout << "\nPTS RANKINGS:\n\n";

	while (!pointRankings.empty()) {

		Team team = pointRankings.top();
		pointRankings.pop();

		int color = DEFAULT;
		if (rank <= 3) {
			color = YELLOW;
		}
		else if (rank <= 7) {
			color = PURPLE;
		}
		else if (rank <= 10) {
			color = BLUE;
		}
		else {
			color = GREEN;
		}

		setConsoleColor(hConsole, color);
		std::cout << std::left << std::setw(2) << rank << ". " << std::setw(18) << team.teamName << std::setw(8) << team.calcTotalPts() << "\n";
		resetConsoleColor(hConsole);
		rank++;
	}

}


void reloadTeams(std::vector<Team>& teams, std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, std::unordered_map<std::string, Player*>& playerMap,
	nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>* tradeMap, int wait, HANDLE& hConsole) {

	for (auto& team : teams) {
		team.destroyPlayers();
	}
	teams.clear();
	teams.shrink_to_fit();
	qbMap.clear();
	wrMap.clear();
	rbMap.clear();
	teMap.clear();
	playerMap.clear();
	tradeValueTable.clear();
	tradeValueTable = load("https://statics.sportskeeda.com/skm/assets/trade-analyzer/players-json-list/v2/playersLists.json");
	findLeagueType(tradeValueTable);
	teams = constructTeams(leagueUsers, leagueTeams, allPlayerData, tradeValueTable, *tradeMap, &playerMap, wait, hConsole);
	mapInit(qbMap, wrMap, rbMap, teMap, playerMap);
}



nlohmann::json findLeagueType(nlohmann::json& tradeValueTable) {
	int leagueType = -1;
	for (int i = 0; i <= 11; i++) {
		if (tradeValueTable["playersListsCollections"][i]["sheetName"] == "redraft___1qb_ppr") {
			leagueType = i;
			std::cout << "[SUCCESS] Found league type. \n";
		}
	}
	tradeValueTable = tradeValueTable["playersListsCollections"][leagueType]["playersList"];
	return tradeValueTable; 
}

void loadTradeMap(nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>& tradeMap) {
	for (int i = 1; i < tradeValueTable.size(); i++) {
		const auto& item = tradeValueTable[i];
		if (item.is_array() && item.size() > 0) {
			tradeMap[item[0]] = i;
		}
	}
}

int leagueSelection(nlohmann::json& leaguesData) {
	int leagueNum = 1;
	for (const auto& league : leaguesData) {

		std::cout << leagueNum << ". " << league["name"].get<std::string>() << "\n";
		leagueNum++;
	}
	std::cout << "Choose league by number\n";
	std::cin >> leagueNum;
	system("cls");
	std::cout << "Loading teams from league " << leaguesData[leagueNum - 1]["name"].get<std::string>() << "\n\n";
	return leagueNum; 
}

std::string getUsername() {
	std::string username; 
	std::cout << "Enter your username \n";
	std::cin >> username;
	system("cls");
	std::cout << "Loading all leagues for user " << username << "\n";
	return username; 
}

void rankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, HANDLE& hConsole) {
	std::cout << "Choose a number for the rankings \n1. QB\n2. WR\n3. RB\n4. TE\n";
	int choice = 0;
	std::cin >> choice;
	if (choice < 1 || choice > 4) {
		std::cout << "Invalid choice. Please choose a number between 1 and 4.\n";
		return;
	}

	std::map<float, Player*>* above = nullptr;
	switch (choice) {
	case 1:
		above = &qbMap;
		break;
	case 2:
		above = &wrMap;
		break;
	case 3:
		above = &rbMap;
		break;
	case 4:
		above = &teMap;
		break;
	}

	std::cout << "RANKINGS: \n";
	bool rankByPosition;
	std::cout << "Press 1 to color code based off overall trade value, press 0 to color code based on positional value: ";
	std::cin >> rankByPosition;
	if (rankByPosition != 0 && rankByPosition != 1) {
		std::cout << "Invalid choice. Please press 0 or 1.\n";
		return;
	}

	std::cout << std::left << std::setw(3) << "#" << std::setw(22) << "NAME" << std::setw(12) << "TRADE VALUE" << std::setw(5) << "PPG\n";
	for (auto it = above->rbegin(); it != above->rend(); ++it) {
		Player* player = it->second;	
		int color = rankByPosition ? player->colorByTradeValue : player->colorByPosition;

		SetConsoleTextAttribute(hConsole, color);
		std::cout << std::setw(3) << player->positionalRanking << std::setw(22) << player->name << std::setw(12) << std::fixed << std::setprecision(2) << player->tradeValue << std::setw(5) << player->avgProj << "\n";
		SetConsoleTextAttribute(hConsole, DEFAULT);
	}
}

void initPositionalRankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap) {
	std::map<float, Player*>* maps[] = { &qbMap, &wrMap, &rbMap, &teMap };
	for (auto& playerMap : maps) {
		int rank = 1;
		for (auto it = playerMap->rbegin(); it != playerMap->rend(); ++it) {
			Player* player = it->second;
			player->positionalRanking = rank++;
		}
	}
}

void initializePlayerColors(std::unordered_map<std::string, Player*>& playerMap, const std::vector<std::vector<int>>& positionalScarcityAll) {
	// Iterate over maps for each position
	for (auto& it : playerMap) {
		Player* player = it.second;
		int i = 0;
		if (player->position == "QB") i = 0;
		if (player->position == "WR") i = 1;
		if (player->position == "RB") i = 2;
		if (player->position == "TE") i = 3;

		// Assign color based on position
		if (player->positionalRanking <= positionalScarcityAll[i][0]) {
			player->colorByPosition = YELLOW;
		}
		else if (player->positionalRanking <= positionalScarcityAll[i][1]) {
			player->colorByPosition = PURPLE;
		}
		else if (player->positionalRanking <= positionalScarcityAll[i][2]) {
			player->colorByPosition = BLUE;
		}
		else if (player->positionalRanking <= positionalScarcityAll[i][3]) {
			player->colorByPosition = GREEN;
		}
		else if (player->positionalRanking <= positionalScarcityAll[i][4]) {
			player->colorByPosition = DARK_YELLOW;
		}
		else {
			player->colorByPosition = RED;
		}

		// Assign color based on trade value
		if (player->tradeValue > 75) {
			player->colorByTradeValue = YELLOW;
		}
		else if (player->tradeValue >= 50) {
			player->colorByTradeValue = PURPLE;
		}
		else if (player->tradeValue >= 30) {
			player->colorByTradeValue = BLUE;
		}
		else if (player->tradeValue >= 15) {
			player->colorByTradeValue = GREEN;
		}
		else if (player->tradeValue >= 9) {
			player->colorByTradeValue = DARK_YELLOW;
		}
		else {
			player->colorByTradeValue = RED;
		}

	}
}


void mapInit(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, std::unordered_map<std::string, Player*>& playerMap) {
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
	initPositionalRankings(qbMap, wrMap, rbMap, teMap);
}



//void mapInit(
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& qbRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& wrRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& rbRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& teRankings,
//	std::unordered_map<std::string, Player*>& playerMap) {
//
//	for (const auto& pair : playerMap) {
//		auto player = pair.second;
//
//		if (player->position == "QB") {
//			qbRankings.push(player);
//		}
//		else if (player->position == "WR") {
//			wrRankings.push(player);
//		}
//		else if (player->position == "RB") {
//			rbRankings.push(player);
//		}
//		else if (player->position == "TE") {
//			teRankings.push(player);
//		}
//	}
//
//}
//
//
//std::priority_queue<Player*, std::vector<Player*>, CompareByTrade> qbRankings;
//
//std::priority_queue<Team, std::vector<Team>, CompareByPoints> leagueRankings;
//
//
//void rankings(
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade> qbRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade> wrRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade> rbRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade> teRankings,
//	HANDLE& hConsole) {
//
//	std::cout << "Choose a position:\n";
//	std::cout << "1. QB\n";
//	std::cout << "2. WR\n";
//	std::cout << "3. RB\n";
//	std::cout << "4. TE\n";
//
//	int choice;
//	std::cin >> choice;
//
//	if (choice < 1 || choice > 4) {
//		std::cout << "Invalid choice!\n";
//		return;
//	}
//
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>* queue;
//
//	if (choice == 1) {
//		queue = &qbRankings;
//	}
//	else if (choice == 2) {
//		queue = &wrRankings;
//	}
//	else if (choice == 3) {
//		queue = &rbRankings;
//	}
//	else if (choice == 4) {
//		queue = &teRankings;
//	}
//
//	std::cout << "\nRANKINGS:\n";
//
//	bool rankByTrade;
//
//	std::cout << "1. Rank by trade value\n";
//	std::cout << "2. Rank by position\n";
//
//	std::cin >> rankByTrade;
//
//	if (rankByTrade != 1 && rankByTrade != 2) {
//		std::cout << "Invalid choice!\n";
//		return;
//	}
//
//	while (!queue->empty()) {
//		Player* player = queue->top();
//		queue->pop();
//
//		int color;
//		if (rankByTrade) {
//			color = player->colorByTradeValue;
//		}
//		else {
//			color = player->colorByPosition;
//		}
//
//		setConsoleColor(hConsole, color);
//		std::cout << std::setw(3) << player->positionalRanking << std::setw(20) << player->name
//			<< std::setw(10) << player->tradeValue << std::setw(5) << player->avgProj << "\n";
//
//		resetConsoleColor(hConsole);
//	}
//
//}
//
//
