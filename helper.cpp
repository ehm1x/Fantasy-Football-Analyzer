#include "declare.h"
#include "classes.cpp"


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
		return 0; // File doesn't exist
	}
	const auto fileTime = std::filesystem::last_write_time(filePath);
	const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fileTime);
	const auto time = std::chrono::system_clock::to_time_t(systemTime);
	return time;
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