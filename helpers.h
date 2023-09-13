#pragma once

#define NOMINMAX
#include <string>
#include <ctime>
#include <Windows.h>
#include <iomanip> 
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem> 
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <limits>
#include <cstdio>
#include <thread>
#include <future>
#include <queue>

class Team;
class Player;

//void printAsciiArt();
//std::time_t getFileLastModifiedTime(const std::string& filePath);
//void printTeams(std::vector<Team>& teams, HANDLE& hConsole);
//void displayTeamStats(const std::vector<Player*>& team, const std::string& teamName, HANDLE& hConsole);
//std::vector<Team> constructTeams(nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>& tradeMap, std::unordered_map<std::string, Player*>* playerMap, int wait);
//void rankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, HANDLE& hConsole, const std::vector < std::vector<int>>& positionalScarcityAll);
//bool fileOutdated(const std::string& filePath, std::time_t maxAgeSeconds);
//void realTradeAnalyzer(std::vector<Team>& teams, std::string& userId, HANDLE& hConsole);
//void tradeAnalyzer(std::unordered_map<std::string, Player*>& playerMap, std::unordered_map<std::string, int>& tradeMap, nlohmann::json& tradeValueTable, HANDLE& hConsole);
//nlohmann::json loadFromFile(const std::string& filePath);
//nlohmann::json loadLarge(const std::string& url, const std::string& filePath, time_t maxAgeSeconds);
//bool fileExists(const std::string& filePath);
//void createFile(const std::string& filePath, const nlohmann::json& data);
//nlohmann::json load(const std::string& url);
//void setConsoleColor(HANDLE hConsole, int color);
//void resetConsoleColor(HANDLE hConsole);
//void teamRankings(std::vector<Team>& teams, HANDLE hConsole);
//void reloadTeams(std::vector<Team>& teams, std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, std::unordered_map<std::string, Player*>& playerMap,
//nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>* tradeMap, int wait);
//void mapInit(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, std::unordered_map<std::string, Player*>& playerMap);
//nlohmann::json findLeagueType(nlohmann::json& tradeValueTable);
//void loadTradeMap(nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>& tradeMap);
//int leagueSelection(nlohmann::json& leaguesData);
//std::string getUsername();


//Utility Functions 

void printAsciiArt();
bool fileExists(const std::string& filePath);
void createFile(const std::string& filePath, const nlohmann::json& data);
std::time_t getFileLastModifiedTime(const std::string& filePath);
bool fileOutdated(const std::string& filePath, std::time_t maxAgeSeconds);
nlohmann::json loadFromFile(const std::string& filePath);
nlohmann::json loadLarge(const std::string& url, const std::string& filePath, std::time_t maxAgeSeconds);
nlohmann::json load(const std::string& url);
std::string getUsername();

//Console Handling Functions :

void setConsoleColor(HANDLE hConsole, int color);
void resetConsoleColor(HANDLE hConsole);

//Data Processing Functions :

void teamRankings(std::vector<Team>& teams, HANDLE hConsole);
void printAllTeams(std::vector<Team>& teams, HANDLE& hConsole);
void printTeam(Team& team, HANDLE& hConsole);
void initTeamTotals(std::vector<Team>& teams); 
void myTeam(std::vector<Team>& teams, std::string user_id, HANDLE& hConsole);
void realTradeAnalyzer(std::vector<Team>& teams, std::string& userId, HANDLE& hConsole);
void displayTeamStats(const std::vector<Player*>& team, const std::string& teamName, HANDLE& hConsole);
void rankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, HANDLE& hConsole);
// void rankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, HANDLE& hConsole, const std::vector < std::vector<int>>& positionalScarcityAll);
void tradeAnalyzer(std::unordered_map<std::string, Player*>& playerMap, std::unordered_map<std::string, int>& tradeMap, nlohmann::json& tradeValueTable, HANDLE& hConsole);
void reloadTeams(std::vector<Team>& teams, std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, std::unordered_map<std::string, Player*>& playerMap, nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>* tradeMap, int wait, HANDLE& hConsole);
std::vector<Team> constructTeams(nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>& tradeMap, std::unordered_map<std::string, Player*>* playerMap, int wait, HANDLE& hConsole);


//Data Initialization Functions :

void mapInit(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, std::unordered_map<std::string, Player*>& playerMap);
//void mapInit(
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& qbRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& wrRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& rbRankings,
//	std::priority_queue<Player*, std::vector<Player*>, CompareByTrade>& teRankings,
//	std::unordered_map<std::string, Player*>& playerMap);
void loadTradeMap(nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>& tradeMap);
int leagueSelection(nlohmann::json& leaguesData);
nlohmann::json findLeagueType(nlohmann::json& tradeValueTable);
void initPositionalRankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap);

// Function to display a team's roster and allow player selection
void displayAndSelectRoster(const Team& team, const std::string& teamName, std::vector<Player*>& selectedPlayers, HANDLE& hConsole);

// Function to calculate trade value and average projection for a team
void calculateTeamStats(const std::vector<Player*>& team, float& teamTradeValue, float& teamPPW);

void initializePlayerColors(std::unordered_map<std::string, Player*>& playerMap, const std::vector<std::vector<int>>& positionalScarcityAll);

