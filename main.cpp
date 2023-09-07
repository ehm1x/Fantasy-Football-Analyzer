#include <nlohmann/json.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "classes.cpp"
#include <Windows.h>
#include <iomanip> 

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

//
//nlohmann::json load(const std::string& url) {
//
//	std::ostringstream responseData;
//	responseData << curlpp::options::Url(url);
//
//	return nlohmann::json::parse(responseData.str());
//}

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
	auto allPlayerData = load("https://api.sleeper.app/v1/players/nfl");
	auto allADP = load("https://fantasyfootballcalculator.com/api/v1/adp/ppr?teams=12&year=2023&position=all");
	auto userData = load("https://api.sleeper.app/v1/user/" + username);
	auto userId = userData["user_id"].get<std::string>();
	auto leaguesData = load("https://api.sleeper.app/v1/user/" + userId + "/leagues/nfl/2023");

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
	std::cout << leagueId; 
	std::vector<Team> teams; 
	
	for (const auto& userTeam : leagueUsers) { // get the team info this doesn't contain roster
		Team team(userTeam["display_name"], userTeam["user_id"]);
		for (const auto& roster : leagueTeams) { // get the roster
			if (roster["owner_id"] == userTeam["user_id"]) {
				for (const auto& player : roster["starters"]) { // each starter in the roster 
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
					Player player(currentName, adp, seasonTotal, position, projpts);
					team.roster.push_back(player);

				}
			}

		}
		teams.push_back(team);
		std::cout << "[LOADED] team " << team.teamName << " has been loaded. \n";
	}

	//for (auto& team : teams) {
	//	std::cout << "============================================================================================\n";
	//	std::cout << "Team: " << team.teamName << "\n\n";

	//	std::cout << std::left << std::setw(10) << "Position" << std::setw(20) << "Name" << std::setw(10) << "ADP" << std::setw(15) << "Season Total" << std::setw(20) << "Avg PTS per Week" << "\n";

	//	for (const auto& player : team.roster) {
	//		std::cout << std::left << std::setw(10) << player.position << std::setw(20) << player.name << std::setw(10) << player.adp << std::setw(15) << player.projTotal << std::setw(20) << player.avgProj << "\n";
	//	}

	//	std::cout << "\nAVERAGE TEAM ADP: " << std::fixed << std::setprecision(2) << team.find_adp() << "\n";
	//	std::cout << "AVERAGE PTS PER WEEK: " << std::fixed << std::setprecision(2) << team.avgWeekly() << "\n";
	//	std::cout << "TOTAL POINTS PROJECTED: " << std::fixed << std::setprecision(2) << team.calcTotalPts() << "\n\n";
	//}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout << "got handle\n"; 
	for (auto& team : teams) {
		std::cout << "============================================================================================\n";
		std::cout << "Team: " << team.teamName << "\n\n";
		std::cout << std::left << std::setw(10) << "Position" << std::setw(20) << "Name" << std::setw(10) << "ADP" << std::setw(15) << "Season Total" << std::setw(20) << "Avg PTS per Week" << "\n";

		for (const auto& player : team.roster) {
			int color = DEFAULT; // Default color

			if (player.avgProj < 9.0) {
				color = RED;
			}
			else if (player.avgProj >= 9.0 && player.avgProj < 12.0) {
				color = GREEN;
			}
			else if (player.avgProj >= 12.0 && player.avgProj < 14.0) {
				color = BLUE;
			}
			else if (player.avgProj >= 14.0 && player.avgProj < 18.0) {
				color = PURPLE;
			}
			else if (player.avgProj >= 18.0) {
				color = YELLOW;
			}

			SetConsoleTextAttribute(hConsole, color); // Set the text color

			// Print player information
			std::cout << std::left << std::setw(10) << player.position << std::setw(20) << player.name << std::setw(10) << player.adp << std::setw(15) << player.projTotal << std::setw(20) << player.avgProj << "\n";

			// Reset text color to default
			SetConsoleTextAttribute(hConsole, DEFAULT);
		}


		std::cout << "\nAVERAGE TEAM ADP: " << std::fixed << std::setprecision(2) << team.find_adp() << "\n";
		std::cout << "AVERAGE PTS PER WEEK: " << std::fixed << std::setprecision(2) << team.avgWeekly() << "\n";
		std::cout << "TOTAL POINTS PROJECTED: " << std::fixed << std::setprecision(2) << team.totalPts << "\n\n";
	}
	

	return 0;
}


