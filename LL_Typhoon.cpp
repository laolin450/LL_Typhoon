#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <cstdlib>
#include <vector>
#include <thread>
#ifdef _WIN32

#define SET_COLOR_RED() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY)
#define SET_COLOR_GREEN() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define RESET_COLOR() SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#else
#define SET_COLOR_RED() std::cout << "\033[31m"
#define SET_COLOR_GREEN() std::cout << "\033[32m"
#define RESET_COLOR() std::cout << "\033[0m"
#endif
void CursorVisible(bool visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible; // false=隐藏，true=显示
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void loading(bool *found) {
	while(1){
		for (int i = 0; i < 3; i++) {
			std::cout << ".";
			std::cout.flush();
			Sleep(500);
		}
		if (*found) {
			return ;
		}
		std::cout << "\b\b\b   \b\b\b";
	}
}
struct toat {
	std::string time;
	std::string clas;
	std::string lv;
	std::string lat;
	std::string lon;
	std::string prss;
};
int typhoonnumber;
int main(){
index:
	CursorVisible(true);
	system("cls");
	SET_COLOR_GREEN();
	std::cout << "Please Input The Typhoon Number:";
	RESET_COLOR();
	std::cin >> typhoonnumber;
	std::ifstream file("bst_all.txt");
	CursorVisible(false);
	if (!file.is_open()) {
		SET_COLOR_RED();
		std::cerr << "Error: Could not open file 'bst_all.txt'." << std::endl;
		Sleep(2000);
		RESET_COLOR();
		return 1;
	}
	system("cls");
	std::cout << "loading";
	int n;
	file >> n;
	bool found = false;
	std::thread load_thread(loading, &found);
	load_thread.detach();
	std::string number;
	while (file >> number) {
		if (number == std::to_string(typhoonnumber)) {
			std::string str;
			found = true;
			std::string n[4];
			file >> n[0] >> n[1] >> n[2] >> n[3];
			std::string name;
			file >> name;
			if (!isalpha(static_cast<unsigned char>(name[0]))) {
				name = "NULL";
			}
			std::getline(file, str);
			struct toat first;
			struct toat last;
			struct toat max;
			file >> max.time;
			file >> max.clas;
			file >> max.lv;
			if (max.lv == "6") max.lv = "-1";
			if (max.lv == "9") max.lv = "-2";
			file >> max.lat;
			file >> max.lon;
			file >> max.prss;
			first = max;
			std::getline(file, max.clas);
			while (!file.eof()) {
				struct toat ty;
				file >> ty.time;
				if (ty.time == "66666") {
					break;
				}
				file >> ty.clas;
				file >> ty.lv;
				file >> ty.lat;
				file >> ty.lon;
				file >> ty.prss;
				std::getline(file, ty.clas);
				if (ty.lv > max.lv || (ty.lv == max.lv && ty.prss < max.prss)) max = ty;
				last = ty;
			}
			system("cls");
			SET_COLOR_GREEN();
			std::cout << "name:" << name << std::endl;
			std::cout << "generation time:" << first.time << std::endl;
			std::cout << "dissipation time:" << last.time << std::endl;
			std::cout << "max intensity time:" << max.time << std::endl;
			std::cout << "max level:" << max.lv << std::endl;
			std::cout << "min pressure:" << max.prss << std::endl;
			RESET_COLOR();
			std::cout << "\nPress any key to continue...";
			system("pause > nul");
			goto index;
		}
		else {
			std::string temp;
			for (int i = 0; i < 6; i++) {
				if (not (file >> temp)) break;
			}
			while (file >> temp) {
				if (temp == "66666") break;
			}
		}
	}
	found = true;
	SET_COLOR_RED();
	system("cls");
	std::clog << "Error: Typhoon number " << typhoonnumber << " not found." << std::endl;
	Sleep(2000);
	goto index;
}
