#pragma once 
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

void printTeams(std::vector<Team>& teams, HANDLE& hConsole) {
	for (auto& team : teams) {
		std::cout << "============================================================================================\n";
		std::cout << "Team: " << team.teamName << "\n\n";
		std::cout << std::left << std::setw(10) << "Position" << std::setw(22) << "Name" << std::setw(10) << "ADP" << std::setw(15) << "Season Total" << std::setw(20) << "Avg PTS per Week" << std::setw(10) << "Trade Value:\n";
		std::sort(team.roster.begin(), team.roster.end(), [](const Player* a, const Player* b) {
			return a->position < b->position;
			});

		for (const auto& player : team.roster) {
			int color = DEFAULT;

			if (player->tradeValue < 9.0) {
				color = RED;
			}
			else if (player->tradeValue >= 10.0 && player->tradeValue < 25.0) {
				color = GREEN;
			}
			else if (player->tradeValue >= 25.0 && player->tradeValue < 50.0) {
				color = BLUE;
			}
			else if (player->tradeValue >= 50.0 && player->tradeValue < 75.0) {
				color = PURPLE;
			}
			else if (player->tradeValue >= 75.0) {
				color = YELLOW;
			}

			SetConsoleTextAttribute(hConsole, color); // Set the text color

			// Print player information
			std::cout << std::left << std::setw(10) << player->position << std::setw(22) << player->name << std::setw(10) << player->adp << std::setw(15) << player->projTotal << std::setw(20) << player->avgProj << std::setw(10) << player->tradeValue << "\n";

			// Reset text color to default
			SetConsoleTextAttribute(hConsole, DEFAULT);
		}


		std::cout << "\nAVERAGE TEAM ADP: " << std::fixed << std::setprecision(2) << team.find_adp() << "\n";
		std::cout << "AVERAGE PTS PER WEEK: " << std::fixed << std::setprecision(2) << team.avgWeekly() << "\n";
		std::cout << "TOTAL POINTS PROJECTED: " << std::fixed << std::setprecision(2) << team.calcTotalPts() << "\n";
		std::cout << "TOTAL TRADE VALUE OF TEAM: " << std::fixed << std::setprecision(2) << team.calcTotalTrade() << "\n\n";
	}
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

void realTradeAnalyzer(std::vector<Team>& teams, std::string& userId, HANDLE& hConsole) {
	std::vector<Player*> team1;
	std::vector<Player*> team2;
	for (const auto& team : teams) {
		if (team.owner_id == userId) {
			std::cout << "YOUR TEAM \n";
			for (int i = 0; i < team.roster.size(); i++) {
				Player* player = team.roster[i];
				std::cout << std::left << std:: setw(2) << i + 1 << ". " << std::setw(15) << player->name << " " << std::setw(13)<<  "TRADE VALUE: " << std::setw(4) << player->tradeValue << "\n";
			}

			int selection;
			std::cout << "Enter the number of the player you want to add to the trade : ";
			std::cin >> selection;
			while (selection != 0) {
				std::cout << "Enter the number of the player you want to add to the trade : ";
				std::cin >> selection;
				if (selection == 0) break;
				team1.push_back(team.roster[selection - 1]);
				std::cout << "\nSuccessfully added " << team.roster[selection - 1]->name << "\n";
			}
		}
	}

	std::cout << "SELECT THE TEAM YOU WANT TO TRADE WITH \n";
	int partnerIndex;
	for (int i = 0; i < teams.size(); i++) {
		std::cout << i + 1 << ". " << teams[i].teamName << "\n";
	}
	std::cin >> partnerIndex;
	auto tradePartner = teams[partnerIndex - 1];
	for (int i = 0; i < tradePartner.roster.size(); i++) {
		Player* player = tradePartner.roster[i];
		std::cout << std::left << std::setw(2) << i + 1 << ". " << std::setw(15) << player->name << " " << std::setw(13) << "TRADE VALUE: " << std::setw(4) << player->tradeValue << "\n";
	}
	int selection;
	do {
		std::cout << "Enter the number of the player you want to add to the trade : ";
		std::cin >> selection;
		if (selection == 0) break;
		team2.push_back(tradePartner.roster[selection - 1]);
		std::cout << "\nSuccessfully added " << tradePartner.roster[selection - 1]->name << "\n";
	} while (selection != 0);

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

void displayTeamStats(const std::vector<Player*>& team, const std::string& teamName, HANDLE& hConsole) {
	std::cout << "\n" << teamName << ":\n";
	std::cout << std::left << std::setw(15) << "Name" << std::setw(4) << "POS " << std::setw(8) << "Trade" << "PPG\n";

	for (const auto& player : team) {
		int color = DEFAULT;

		if (player->tradeValue < 9.0) {
			color = RED;
		}
		else if (player->tradeValue >= 10.0 && player->tradeValue < 25.0) {
			color = GREEN;
		}
		else if (player->tradeValue >= 25.0 && player->tradeValue < 50.0) {
			color = BLUE;
		}
		else if (player->tradeValue >= 50.0 && player->tradeValue < 75.0) {
			color = PURPLE;
		}
		else if (player->tradeValue >= 75.0) {
			color = YELLOW;
		}

		SetConsoleTextAttribute(hConsole, color);
		std::cout << std::left << std::setw(15) << player->name << std::fixed << std::setw(4) << player->position << std::setprecision(2) << std::setw(8) << player->tradeValue << std::setw(10) << player->avgProj << "\n";
		SetConsoleTextAttribute(hConsole, DEFAULT);
	}
	std::cout << "\n\n";
}

std::vector<Team> constructTeams(nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>& tradeMap, std::unordered_map<std::string, Player*>* playerMap, int wait) {
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
		std::cout << "[LOADED] Team " << team.teamName << " has been loaded.  " << std::endl;
	}
	return teams;
}

void rankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, HANDLE& hConsole, const std::vector<std::vector<int>>& positionalScarcityAll) {
	std::cout << "Choose a number in the calculator \n";
	std::map<float, Player*>* above = {};
	int choice = 0;
	std::cout << "1. QB\n";
	std::cout << "2. WR\n";
	std::cout << "3. RB\n";
	std::cout << "4. TE\n";
	std::cin >> choice;

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
	choice--;
	std::cout << "RANKINGS: \n";
	int rank = 1;
	bool rankByPosition;
	std::cout << "Press 1 to color code Relative to position, press 0 to color code based on trade value: ";
	std::cin >> rankByPosition;
	for (auto it = above->rbegin(); it != above->rend(); ++it) {
		float tradeValue = it->first;
		Player* player = it->second;
		player->posotionalRanking = rank;
		int color = DEFAULT;
		if (rankByPosition) {
			if (player->posotionalRanking <= positionalScarcityAll[choice][0]) {
				color = YELLOW;
			}
			else if (player->posotionalRanking <= positionalScarcityAll[choice][1]) {
				color = PURPLE;
			}
			else if (player->posotionalRanking <= positionalScarcityAll[choice][2]) {
				color = BLUE;
			}
			else if (player->posotionalRanking <= positionalScarcityAll[choice][3]) {
				color = GREEN;
			}
			else if (player->posotionalRanking <= positionalScarcityAll[choice][4]) {
				color = DARK_YELLOW;
			}
			else {
				color = RED;
			}
		}
		else {
			if (tradeValue > 75) {
				color = YELLOW;
			}
			else if (tradeValue >= 50) {
				color = PURPLE;
			}
			else if (tradeValue >= 30) {
				color = BLUE;
			}
			else if (tradeValue >= 15) {
				color = GREEN;
			}
			else if (tradeValue >= 9) {
				color = DARK_YELLOW;
			}
			else {
				color = RED;
			}
		}

		SetConsoleTextAttribute(hConsole, color);

		std::cout << std::setw(3) << rank;
		std::cout << std::setw(20) << player->name;
		std::cout << std::setw(5) << tradeValue << "\n";
		SetConsoleTextAttribute(hConsole, DEFAULT);
		rank++;
	}
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

void teamRankings(std::vector<Team>& teams, HANDLE hConsole) {
	std::unordered_map<float, std::string> leagueTrade;
	std::unordered_map<float, std::string> leaguePoints;

	for (auto& roster : teams) {
		leagueTrade[roster.calcTotalTrade()] = roster.teamName;
		leaguePoints[roster.calcTotalPts()] = roster.teamName;
	}

	std::map<float, std::string> sortedTrade(leagueTrade.begin(), leagueTrade.end());
	std::map<float, std::string> sortedPoints(leaguePoints.begin(), leaguePoints.end());

	int rank = 1;
	for (const auto& pair : sortedTrade) {
		int color = DEFAULT; // Default color
		if (rank == 1) {
			color = YELLOW; // Redefine the meaning of GOLD based on your color codes
		}
		else if (rank == 2 || rank == 3) {
			color = PURPLE;
		}
		else if (rank >= 4 && rank <= 7) {
			color = BLUE;
		}
		else if (rank >= 8 && rank <= 10) {
			color = GREEN;
		}
		else if (rank >= 11 && rank <= 12) {
			color = RED;
		}

		setConsoleColor(hConsole, color); // Set the text color using the provided console handle
		std::cout << std::setw(10) << std::left << pair.second << ": " << std::setw(8) << std::right << pair.first << std::endl;
		resetConsoleColor(hConsole); // Reset the text color using the provided console handle
		rank++;
	}

	rank = 1;
	for (const auto& pair : sortedPoints) {
		int color = DEFAULT; // Default color
		if (rank == 1) {
			color = YELLOW; // Redefine the meaning of GOLD based on your color codes
		}
		else if (rank == 2 || rank == 3) {
			color = PURPLE;
		}
		else if (rank >= 4 && rank <= 7) {
			color = BLUE;
		}
		else if (rank >= 8 && rank <= 10) {
			color = GREEN;
		}
		else if (rank >= 11 && rank <= 12) {
			color = RED;
		}

		setConsoleColor(hConsole, color); // Set the text color using the provided console handle
		std::cout << std::setw(10) << std::left << pair.second << ": " << std::setw(8) << std::right << pair.first << std::endl;
		resetConsoleColor(hConsole); // Reset the text color using the provided console handle
		rank++;
	}
}

void reloadTeams(std::vector<Team>& teams, std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, std::unordered_map<std::string, Player*>& playerMap,
	nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>* tradeMap, int wait) {

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
	teams = constructTeams(leagueUsers, leagueTeams, allPlayerData, tradeValueTable, *tradeMap, &playerMap, wait);
	mapInit(qbMap, wrMap, rbMap, teMap, playerMap);
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