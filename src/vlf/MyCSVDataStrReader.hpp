#pragma once

#include "MySyst.hpp"

#include "MyTimerLogger.hpp"

struct MyCSVDataStrReader
{
	map<string, int> keyTabl;

	vector<string> keyVect;

	list<vector<string>> dataStr;	

	MyCSVDataStrReader(ifstream& ifs, const string& razd);

	MyCSVDataStrReader();

	void readCSVData(ifstream& ifs, const string& strRAZD);

	int getPos(const string& posName) const;

	static vector<string> csvStrDecompose(string& workStr, const string& strRAZD);

	static list<vector<string>> csvFileDecompose(ifstream& ifs, const string& strRAZD);

	static string tochka2zpt(const string& str1);

	static void str2str(string& str1, const string& from1, const string& to1);

	static string str2strrc(const string& str1, const string& from1, const string& to1);

};

