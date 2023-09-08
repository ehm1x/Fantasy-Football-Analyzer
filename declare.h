#pragma once

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

const std::string FOLDER_PATH;
const std::string INFO_FILE_PATH;
const std::string TIMESTAMP_FILE_PATH;

void printAsciiArt();

std::time_t getFileLastModifiedTime(const std::string& filePath);

bool fileOutdated(const std::string& filePath, std::time_t maxAgeSeconds);

nlohmann::json loadFromFile(const std::string& filePath);

nlohmann::json loadLarge(const std::string& url, const std::string& filePath, std::time_t maxAgeSeconds);

bool fileExists(const std::string& filePath);

void createFile(const std::string& filePath, const nlohmann::json& data);

nlohmann::json load(const std::string& url);
