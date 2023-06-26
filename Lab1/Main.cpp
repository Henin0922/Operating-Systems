#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include "employee.h"

int main(int argc, char* argv[]) {

	std::string binFileName;
	std::cout << "输入bin文件名 Enter bin file name: ";
	getline(std::cin, binFileName);
	std::cout << "输入记录数 Enter records count: ";
	int recordsCount;
	std::cin >> recordsCount;

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	std::string command = "Creator.exe " + binFileName + " " + std::to_string(recordsCount);
	TCHAR commandLine[MAX_PATH];
	_tcscpy_s(commandLine, MAX_PATH, command.c_str());
	/*std::wstring widestr = std::wstring(command.begin(), command.end());
	const wchar_t* comm = widestr.c_str();
	_tcscpy_s(commandLine, MAX_PATH, comm);*/

	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		std::cerr << "创建过程出错 Error creating process (Creator)";
		return 1;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	std::ifstream readFile(binFileName, std::ios::binary);
	if (!readFile) {
		std::cerr << "错误：无法打开 bin 文件 Error: Could not open bin file";
		return 1;
	}

	employee person;
	std::cout << "\nEmployee list:\n";
	while (readFile.read(reinterpret_cast<char*>(&person), sizeof(person))) {
		std::cout << "ID: " << person.num << '\n';
		std::cout << "Name: " << person.name << '\n';
		std::cout << "Hours: " << person.hours << '\n';
		std::cout << '\n';
	}

	std::cout << "\n为报告文件写入名称 Write name for Report file: ";
	std::string reportFile;
	std::cin >> reportFile;
	double salary;
	std::cout << "写工资 Write salary: ";
	std::cin >> salary;
	readFile.close();

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	command = "Reporter.exe " + binFileName + " " + reportFile + " " + std::to_string(salary);
	_tcscpy_s(commandLine, MAX_PATH, command.c_str());
	/*std::wstring widestrr = std::wstring(command.begin(), command.end());
	const wchar_t* commm = widestrr.c_str();
	_tcscpy_s(commandLine, MAX_PATH, commm);*/

	if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		std::cerr << "创建过程出错（记者） Error creating process (Reporter)" << '\n';
		return 1;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	readFile.open(reportFile);
	if (!readFile) {
		std::cerr << "错误：无法打开记者文件 Error: Could not open reporter file" << '\n';
		return 1;
	}

	std::string line;
	std::cout << '\n';
	while (std::getline(readFile, line))
		std::cout << line << '\n';
	return 0;
}
