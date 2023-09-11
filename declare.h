#pragma once

#define NOMINMAX
#include <string>
#include <ctime>
#include <nlohmann/json.hpp>
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

const std::string FOLDER_PATH;
const std::string INFO_FILE_PATH;
const std::string TIMESTAMP_FILE_PATH;

class Team;
class Player; 
//
//void printAsciiArt();
//
//std::time_t getFileLastModifiedTime(const std::string& filePath);
//
//bool fileOutdated(const std::string& filePath, std::time_t maxAgeSeconds);
//
//nlohmann::json loadFromFile(const std::string& filePath);
//
//nlohmann::json loadLarge(const std::string& url, const std::string& filePath, std::time_t maxAgeSeconds);
//
//bool fileExists(const std::string& filePath);
//
//void createFile(const std::string& filePath, const nlohmann::json& data);
//
//nlohmann::json load(const std::string& url);

void printAsciiArt();
std::time_t getFileLastModifiedTime(const std::string& filePath);
void printTeams(std::vector<Team>& teams, HANDLE& hConsole);
void displayTeamStats(const std::vector<Player*>& team, const std::string& teamName, HANDLE& hConsole);
std::vector<Team> constructTeams(nlohmann::json& leagueUsers, nlohmann::json& leagueTeams, nlohmann::json& allPlayerData, nlohmann::json& tradeValueTable, std::unordered_map<std::string, int>* tradeMap, std::unordered_map<std::string, Player*>* playerMap);
void rankings(std::map<float, Player*>& qbMap, std::map<float, Player*>& wrMap, std::map<float, Player*>& rbMap, std::map<float, Player*>& teMap, HANDLE& hConsole, const std::vector < std::vector<int>> &positionalScarcityAll);
bool fileOutdated(const std::string& filePath, std::time_t maxAgeSeconds);
void realTradeAnalyzer(std::vector<Team>& teams, std::string& userId, HANDLE& hConsole);
void tradeAnalyzer(std::unordered_map<std::string, Player*>& playerMap, std::unordered_map<std::string, int>& tradeMap, nlohmann::json& tradeValueTable, HANDLE& hConsole);
nlohmann::json loadFromFile(const std::string& filePath);
nlohmann::json loadLarge(const std::string& url, const std::string& filePath, time_t maxAgeSeconds);
bool fileExists(const std::string& filePath);
void createFile(const std::string& filePath, const nlohmann::json& data);
nlohmann::json load(const std::string& url);
