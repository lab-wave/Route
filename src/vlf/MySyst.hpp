#pragma once

#include <iostream>

#include <fstream>

#include <sstream>

#include <iomanip>

#include <thread>

#include <mutex>

// ����������� ������������ ����:
#include <string>
#include <set>
#include <map>
#include <tuple>
#include <vector>
#include <list>

#include <locale>

#include <codecvt>

#include <algorithm>

#include <windows.h>

#include <direct.h>

#include <utility>

#include <time.h>

#pragma warning(disable : 4996)

// ****************** MySyst ********************

using namespace std;

//***************** ������� MySyst ****************

#define MYMIN(a,x) if((a)>(x)) {a=x;};

#define MYMIN2(a,x) if((a)>(x)) {a=x;};
#define MYMIN3(a,x,y) MYMIN2(a,x); MYMIN2(a,y);
#define MYMIN4(a,x,y,z) MYMIN2(a,x); MYMIN2(a,y); MYMIN2(a,z);

#define MYMAX(a,x) if((a)<(x)) {a=x;};

#define MYMAX2(a,x) if((a)<(x)) {a=x;};
#define MYMAX3(a,x,y) MYMAX2(a,x); MYMAX2(a,y);
#define MYMAX4(a,x,y,z) MYMAX2(a,x); MYMAX2(a,y); MYMAX2(a,z);

#define MYIN(a,min1,max1) if((a)<(min1)) {a=min1;}; if((a)>(max1)) {a=max1;};
#define MYMINMAX(a,b,x) if((a)>(x)) {a=x;}; if((b)<(x)) {b=x;};

#define MYINTMAX 2000000000

namespace MySyst
{
	// ���������

	struct MyUTF8 // ������� ����������� � UTF-8 � ������� � ���������� ��������� � ��� ��� ������ � �����������
	{
		struct my_numpunct : std::numpunct<char>
		{
			std::string do_grouping() const { return ""; };
		};

		// ������ 
		const std::locale utf8_locale; // ������
		const std::locale utf8_locale0; // ������
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>  cX; // �������������� UTF-8 		
		std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t>  cX16; // �������������� UTF-16 	
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>  cX816; // �������������� UTF-16 	

		// ������ �����������
		static MySyst::MyUTF8* one; // ��������� �� ��������� �����������
		
		// ������ �����������
		MyUTF8()
		:utf8_locale(std::locale("Russian"), new std::codecvt_utf8<wchar_t>), // UTF-8 ��� �����-������
		utf8_locale0(utf8_locale, new my_numpunct)
		{
			setlocale(LC_ALL, "Russian");		
			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);
			one = this;
		}; // ����������� ������ � �������    
		
		// ������ �� ����������� - ����������� �������� �������
		inline void operator()(std::ifstream& s) { s.imbue(utf8_locale); };
		inline void operator()(std::ofstream& s) { s.imbue(utf8_locale); };
		inline void operator()(std::wifstream& s) { s.imbue(utf8_locale); };
		inline void operator()(std::wofstream& s) { s.imbue(utf8_locale); };

		inline void loc0(std::ifstream& s) { s.imbue(utf8_locale0); };
		inline void loc0(std::ofstream& s) { s.imbue(utf8_locale0); };
		inline void loc0(std::wifstream& s) { s.imbue(utf8_locale0); };
		inline void loc0(std::wofstream& s) { s.imbue(utf8_locale0); };

		// ������ �� ����������� - �������������� UTF-8 �/�� ����������� ���������
		inline std::wstring w(const std::string& s1)  { return cX.from_bytes(s1); };
		inline std::string s(const std::wstring& s1)  { return cX.to_bytes(s1); };
		inline std::wstring operator()(const std::string& s1)  { return cX.from_bytes(s1); };
		inline std::string  operator()(const std::wstring& s1)  { return cX.to_bytes(s1); };

		std::vector<std::wstring> w(const std::vector<std::string>& s1) ;
		std::vector<std::string>  s(const std::vector<std::wstring>& s1) ;
		std::vector<std::wstring> operator()(const std::vector<std::string>& s1) ;
		std::vector<std::string>  operator()(const std::vector<std::wstring>& s1) ;
		
		
		// ��������������� ����� ������������ � ����-������������� �����������
		// �������� ��� ���������: 1251 - ��� cp1251, CP_UTF8 - ��� UTF-8

		std::wstring static s2w(const std::string& str, int codePage1= CP_UTF8);
				
		std::string static w2s(const std::wstring& wstr, int codePage1= CP_UTF8);
		
		std::string static recode(const std::string& str, int fromCod, int toCod);
		
		// ���� ���������� ��������� ������ � ������������� � UTF-8

		std::vector<std::string> static cmdArgs(int argc, char* argv[]);
		std::vector<std::wstring> static wcmdArgs(int argc, char* argv[]);

		// ������ �� ����������� - ������ � ������ �������� ������� � ������

		bool static readTxt(const std::wstring& ifname1, std::string& target1, int stepcount = 0, int* alreadySize = NULL, double* relSize1=NULL);

		std::string static readTxt(const std::wstring& ifname1, int stepcount = 0, int* alreadySize = NULL, double* relSize1=NULL);
	};
		
	// ������ � ����� � ��������
		
	std::string time2str(unsigned int t); // ������ ���� 2020-04-12--22-04-15

	std::string time33str(unsigned int t); // ������ ���� 2020-04-12 22-04-15

	bool istime33str(std::string s); // ���������, ����� �� ������ s ��� ������ ��������� std::string time33str(unsigned int t)

	int istime33strvlast(std::string s); // ���������, ����� �� ����� ������ s ��� ���� 2020-04-12--22-04-15 v123  ���� ��, ���������� ����� ������, ���� ���, ���������� 0.

	string time2str7(double t, const string& fmt = string("%04i-%02i-%02i--%02i-%02i-%02i-%03i"), int n0 = 0, int n1 = 1, int n2 = 2, int n3 = 3, int n4 = 4, int n5 = 5, int n6 = 6);
	// ��������� ������ �������. t-��� �����, fmt-��������� ������ � ���� sprintf, n0--n6 ���������, ��� �� ������� fmt ���� �������: 
	// n=0 ���, n=1 �����(01--12), n=2 ����, n=3 ���(00--24), n=4 ������, n=5 �������, n=6 ������������

	inline wstring time2wstr7(double t, const string& fmt = string("%04i-%02i-%02i--%02i-%02i-%02i-%03i"), int n0 = 0, int n1 = 1, int n2 = 2, int n3 = 3, int n4 = 4, int n5 = 5, int n6 = 6)
		// ��������� ������ �������. t-��� �����, fmt-��������� ������ � ���� sprintf, n0--n6 ���������, ��� �� ������� fmt ���� �������: 
		// n=0 ���, n=1 �����(01--12), n=2 ����, n=3 ���(00--24), n=4 ������, n=5 �������, n=6 ������������
	{
		return MyUTF8::s2w(time2str7(t, fmt, n0, n1, n2, n3, n4, n5, n6));
	};

	std::tuple<int,int,int,int,int,int> time2tuple(unsigned int t); // ���� ��� 2020,04,12,22,04,15  (���,�����,����, ���, ������, �������)

	std::vector<int> time2vector(unsigned int t); // ���� ��� 2020,04,12,22,04,15  (���,�����,����, ���, ������, �������)

	std::vector<int> time2vector7(double t); // ���� ��� 2020,04,12,22,04,15,452  (���,�����,����, ���, ������, �������, ������������)	

	int date2timei(int year1, int month1 = 1, int day1 = 1, int h1 = 0, int m1 = 0, int s1 = 0);
		// �� ����������� ������� ����, ������, �����, ����, ������, ������� �������� ����� ������ � 1 ������ 1070�.

	int date2timei(vector<int>::iterator it1);
		// �� ����������� ������� *it1, ������ *(it1+1), ����� *(it1+2), ���� *(it1+3), ������ *(it1+5), �������  *(it1+5) �������� ����� ������ � 1 ������ 1070�.

	std::string sec2hhmmssStr(long int t); // ������� � ��:��:�� ��� ���:��:��:��

	tuple<int, int, int, int> sec2dhms(int dt); // ����������� �������� dt ������ � (���, ����, ������, �������)

	tuple<int, int, int, int, double> sec2dhms(double dt); // ����������� �������� dt ������ � (���, ����, ������, �������, ������� ����� ������)

	vector<int> sec2dhms7(double dt); // ����������� �������� dt ������ � (���, ����, ������, �������, ������������)

	string sec2str7(double t, const string& fmt = string("%02i:%02i:%02i"), int n0 = 1, int n1 = 2, int n2 = 3, int n3 = 4, int n4 = 4);
	// ����������� �������� dt ������ � (���, ����, ������, �������, ������������)
	// ��������� ������ �������. t-��� �����, fmt-��������� ������ � ���� sprintf, n0--n4 ���������, ��� �� ������� fmt ���� �������: 
	// n=0 ����, n=1 ���(00--24), n=2 ������, n=3 �������, n=4 ������������

	inline wstring sec2wstr7(double t, const string& fmt = string("%02i:%02i:%02i"), int n0 = 1, int n1 = 2, int n2 = 3, int n3 = 4, int n4 = 4)
	// ����������� �������� dt ������ � (���, ����, ������, �������, ������������)
	// ��������� ������ �������. t-��� �����, fmt-��������� ������ � ���� sprintf, n0--n4 ���������, ��� �� ������� fmt ���� �������: 
	// n=0 ����, n=1 ���(00--24), n=2 ������, n=3 �������, n=4 ������������
	{
		return MyUTF8::s2w(sec2str7(t, fmt, n0,  n1,  n2,  n3,  n4 ));
	};

	double wstrRest62time(std::wstring s); //  � ������ ������ ������� 7 ����� � �������������� ��� ��� ����� ���� ��� ������ ������� ������������. ������������� ����� ���������� ������

	double wstrRest52time(std::wstring s); //  � ������ ������ ������� 6 ����� � �������������� ��� ��� ����� ���� ��� ������ �������. ������������� ����� ���������� ������ 

	double time_d(FILETIME ft); // �� ������ � 1601 � ������� � 1970

	FILETIME time_d(double ftd); // �� ������ � 1970 � ������� � 1601

	double time_d(); // ��������� ����� ������� � 1970

	tuple<string, int, int, int, int, int, int> inline time_of_compilation() // ����� ���������� �������� ������
	{
		int YYYY, MM, DD, h, m, s;
		char MCH[100];
		sscanf(__DATE__, "%s %i %i", MCH, &DD, &YYYY);
		const char* months[] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
		MM = -1;
		for (int l = 0; (l<12) && MM<0; l++)
			if (strcmp(months[l], MCH) == 0) MM = l;
		MM++;

		sscanf(__TIME__, "%s", MCH);
		MCH[2] = MCH[5] = ' ';
		sscanf(MCH, "%i %i %i", &h, &m, &s);

		sprintf(MCH, "%04i-%02i-%02i--%02i-%02i-%02i", YYYY, MM, DD, h, m, s);

		return make_tuple(string(MCH), YYYY, MM, DD, h, m, s);
	};

	// *********** ������ �����, ������� � �.�.*************** 

	vector<vector<wstring>> mySplit(const wstring& wstr, const vector<wchar_t>& wrazdelv, const vector<wchar_t>& wsbrosv = {});
	// ������ ������ �����. 
	// ��� ������� ����� ��������� �� ������� wsbrosv ���������� ���������� ������� ������� ����������, ���� ��������� � ��������� �� �����������, ������� ��������� ������ ������������� � ����
	// ��� ������� ��������� ��������� (�� ���� ������ ������� ����� �������� ��������� ������) �� ������� wrazdelv ���������� ���������� �� �������
	// ������� ������� ���������� � �������� ��������� ������, ���� ��������� � ��������� �� �����������
	// ��� ��������� ������� ����������� ���������� � ������� ������� ����������

	vector<vector<wstring>> mySplit(const wstring& wstr, const vector<wstring>& wrazdelv, const vector<wstring>& wsbrosv = {});
	// ������ ������ �����. 
	// ��� ������� ����� ��������� �� ������� wsbrosv ���������� ���������� ������� ������� ����������, ���� ��������� � ��������� �� �����������, ������� ��������� ������ ������������� � ����
	// ��� ������� ��������� ��������� (�� ���� ������ ������� ����� �������� ��������� ������) �� ������� wrazdelv ���������� ���������� �� �������
	// ������� ������� ���������� � �������� ��������� ������, ���� ��������� � ��������� �� �����������
	// ��� ��������� ������� ����������� ���������� � ������� ������� ����������
	
	// �������������� ����� ����� � ������ � �������

	template<typename ttt1> std::string toStr(ttt1 m, int n = 1) { std::stringstream ss; for(int ln = 0; ln < n; ln++) ss << m; return ss.str(); };

	template<typename ttt1> std::string toStr(vector<ttt1> m, int n = 1, string delim1 = "-") 
	{
		std::string s = toStr<ttt1>(m[0], n);
		for (int l = 1; l < m.size(); l++)	s = s + delim1 + toStr<ttt1>(m[l], n);
		return ws;
	};

	template<typename ttt1> std::string toStr(vector<ttt1> m, string delim1, int n = 1) 
	{
		return toStr<ttt1>(m, n, delim1);
	};
	
	template<typename ttt1> std::wstring toWstr(ttt1 m, int n = 1) { std::wstringstream wss; for(int ln = 0; ln < n; ln++) wss << m; return wss.str(); };

	template<typename ttt1> std::wstring toWstr(vector<ttt1> m, int n = 1, wstring delim1 = L"-")
	{
		std::wstring ws = toWstr<ttt1>(m[0], n);
		for (int l = 1; l < m.size(); l++)	ws = ws + delim1 + toWstr<ttt1>(m[l], n);
		return ws;
	};

	template<typename ttt1> std::wstring toWstr(vector<ttt1> m, wstring delim1, int n = 1)
	{
		return toWstr<ttt1>(m, n, delim1);
	};

	template<class ttt1> std::wstring toWstr(vector<ttt1> m, int w1, wchar_t wc, wstring delim1 = L"-")
	{
		std::wstring ws = toWstr<ttt1>(m[0], w1, wc);
		for (int l = 1; l < m.size(); l++)	ws = ws + delim1 + toWstr<ttt1>(m[l], w1, wc);
		return ws;
	};

	template<typename ttt1> std::wstring toWstr(ttt1 m, int w1, wchar_t wc) { std::wstringstream wss; wss << std::setw(w1) << std::setfill(wc) << m; return wss.str(); };	

	template<typename ttt1, typename ttt2> ttt2 toS12(ttt1 m, int n = 1) { std::stringstream ss; for (int ln = 0; ln < n; ln++) ss << m; ttt2 t; ss >> t; return t; };

	template<typename ttt1, typename ttt2> ttt2 toW12(ttt1 m, int n = 1) { std::wstringstream ss; for (int ln = 0; ln < n; ln++) ss << m; ttt2 t; ss >> t; return t; };

	// ************ ������� ��� SPRINTF

	template<class... tt> string toCharStr(int buflen, string formstr, tt... pars)
	{
		string str(buflen, '\0');
		sprintf(&str[0], formstr.c_str(), pars...);
		return string(&str[0]);
	};

	template<class... tt> wstring toCharWstr(int buflen, string formstr, tt... pars)
	{
		return MySyst::MyUTF8::s2w(toCharStr(buflen, formstr, pars...));
	};

	template<class... tt> string toCharStr(string formstr, tt... pars)
	{		
		return toCharStr(5000, formstr, pars...);
	};

	template<class... tt> wstring toCharWstr(string formstr, tt... pars)
	{
		return toCharWstr(5000, formstr, pars...);
	};


	// ********** �������������� ������ � ��������� ������������ � ������ ����� ******************

	vector<int> wstr2vectint(wstring::iterator wsbegin, wstring::iterator wsend);

	// ********* ��� double n ������ ����� ������� � ������ n �������� � ������� ��˨�

	string modf2str(int n, double d);
	wstring modf2wstr(int n, double d);

	// ������ � ������ ������� ���� �������	�� ����, ��� � ��� (�� ���� ����������)
		
	struct wfilepathnameext
	{
		// ������ 
		std::wstring p, n, e; // ���, ���, ����������

		// ������ �����������
		wfilepathnameext();
		wfilepathnameext(std::wstring fullfname);

		// ������ �� �����������
		wfilepathnameext& operator()(std::wstring fullfname); // �������������� ����� ����� ������ ������ �����
		
		std::wstring operator()(); // �������� � ���������� ������ ���

		std::wstring pn(); // �������� � ���������� ���� � ������� ���
		
		std::wstring ne(); // ���������� ������� ��� � ����������
		
		std::wstring ne(const wfilepathnameext& f1); // ����������� ������ ��� � ���������� (��� ����) �� f1, �������� ���� �������, � ���������� ������� ��� � ����������
		
		std::wstring ne(std::wstring fn1); // �������������� ����� ������ � ����������� (��� ����) �� fn1, �������� ���� �������, � ���������� ������� ��� � ����������
		
		// ������ �����������

		std::vector<std::wstring> static shortNames(const std::vector<std::wstring>& fullname); // �� ������� ������ ���� �������� ������ ������� ���� ��� ����������
		std::vector<std::wstring> static shortNameExts(const std::vector<std::wstring>& fullname); // �� ������� ������ ���� �������� ������ ������� ���� c ������������
		
	};

	struct filepathnameext
	{
		// ������ 
		std::string p, n, e; // ���, ���, ����������

							  // ������ �����������
		filepathnameext();
		filepathnameext(std::string fullfname);

		// ������ �� �����������
		filepathnameext& operator()(std::string fullfname); // �������������� ����� ����� ������ ������ �����

		std::string operator()(); // �������� � ���������� ������ ���

		std::string pn(); // �������� � ���������� ���� � ������� ���

		std::string ne(); // ���������� ������� ��� � ����������

		std::string ne(const filepathnameext& f1); // ����������� ������ ��� � ���������� (��� ����) �� f1, �������� ���� �������, � ���������� ������� ��� � ����������

		std::string ne(std::string fn1); // �������������� ����� ������ � ����������� (��� ����) �� fn1, �������� ���� �������, � ���������� ������� ��� � ����������

										   // ������ �����������

		std::vector<std::string> static shortNames(const std::vector<std::string>& fullname); // �� ������� ������ ���� �������� ������ ������� ���� ��� ����������
		std::vector<std::string> static shortNameExts(const std::vector<std::string>& fullname); // �� ������� ������ ���� �������� ������ ������� ���� c ������������

	};
	
	std::vector<int> invsorttabl(std::vector<int>& s);	

	template<typename ttt1, typename ttt2> bool interSect(pair<ttt1, ttt2> m12, std::vector<pair<ttt2, ttt1>>& v12) // ��������� ���� �� ���� ����������� ������������� el [) � ����� �������������� �� v12 
	{
		bool res = false;

		for (int l = 0; (l < v12.size()) && !res; l++)
		{
			if (m12.first < v12[l].second && v12[l].first < m12.second)  res = true;
		};

		return res;

	};
	


	// ****************** ������ � map<> ************

	template<typename ttt1, typename ttt2> ttt2 fromMap_or(const std::map<ttt1, ttt2>& map1, const ttt1& obj, const ttt2& alt)
	{
		auto it1 = map1.find(obj);
		if (it1 == map1.end()) return alt;
		else return it1->second;
	};
		
	// ������������� � ��������

	struct Perebor // ��������� ��� �������� ��������� ���������
	{
	private:
		// ������ ���������
		std::vector<int> v, s; // v - ����������, s - ������
		int lVar, nVar;
		bool retcode; // True ��� ���������� ���� false ��� ������ 
	
	public:
		// ������ �����������

		Perebor(const std::vector<int>& p1)	:v(p1), s(p1), retcode(true)
		{			
			lVar = 0; nVar = 1;
			for (int l = 0; l < s.size(); l++)
			{
				if (s[l] < 0) s[l] = 0;
				if (s[l] <= 0) v[l] = -1; else v[l] = 0;
				if (s[l] > 1) nVar *= s[l];
			};
		};

		// ������ �� �����������

		inline int operator[](int l) { return v[l]; };
		inline const std::vector<int>& operator()() { return v; };
		inline operator std::vector<int>& () { return v; };
		inline bool rc() { return retcode; };
		inline int get_nVar() { return nVar; };
		inline int get_lVar() { return lVar; };
		

		void first()
		{
			lVar = 0;
			for (int l = 0; l < s.size(); l++)
			{
				if (s[l] <= 0) v[l] = -1; else v[l] = 0;
			};

			retcode = (lVar<nVar);
		};

		bool next()
		{
			if (++lVar >= nVar) return (retcode=false);
			v.back()++;
			int lz = v.size() - 1;
			for (; lz >= 0; lz--)
			{
				if (v[lz] >= s[lz])
				{
					if (s[lz] == 0) v[lz] = -1; else v[lz] = 0;
					if (lz > 0) v[lz - 1]++; else retcode = false;
				};
			};
			return retcode;
		};

	};	

	// ��������� � ������ mbe ������ �� b1---e1 ������� � �� �����������
	template<typename ttt1, typename ttt2> void merge(vector<ttt2>& mbe, ttt1 b1, ttt1 e1)
	{
		for (auto it = b1; it != e1; it++)
		{
			bool isInmbe = false;
			for (auto& v : mbe)
				if (v == *it)
				{
					isInmbe = true;
					break;
				};
			if (!isInmbe) mbe.emplace_back(*it);
		};
	};

	// ��������� � ����� mbe ������ �� b1---e1 ������� � �� �����������
	template<typename ttt1, typename ttt2> void merge(set<ttt2>& mbe, ttt1 b1, ttt1 e1)
	{
		for (auto it = b1; it != e1; it++)
		{
			if(mbe.find(*it)==mbe.end())	 mbe.emplace(*it);
		};
	};

	// ��������� � ������ mbe ������ �� �������� ���������� be1 ������� � �� �����������
	template<typename ttt1, typename ttt2> void merge(vector<ttt2>& mbe, ttt1 be1)
	{
		for (auto it = be1.begin(); it != be1.end(); it++)
		{
			bool isInmbe = false;
			for (auto& v : mbe)
				if (v == *it)
				{
					isInmbe = true;
					break;
				};
			if (!isInmbe) mbe.emplace_back(*it);
		};
	};

	// ��������� � ����� mbe ������ �� �������� ���������� be1 ������� � �� �����������
	template<typename ttt1, typename ttt2> void merge(set<ttt2>& mbe, ttt1 be1)
	{
		for (auto it = be1.begin(); it != be1.end(); it++)
		{
			if (mbe.find(*it) == mbe.end())	 mbe.emplace(*it);
		};
	};

	template<typename ttt1, typename ttt2> void addIntersect(vector<ttt2>& mbe, ttt1 b1, ttt1 e1, ttt1 b2, ttt1 e2)
	{
		int counter = 0;
		for (auto it1 = b1; it1 != e1; it1++)
		{			
			for (auto it2 = b2; it2 != e2; it2++)
				if (*it2 == *it1)
				{
					mbe.emplace_back(*it1);
					counter++;
					break;
				};
			
		};

		return counter;
	};

	template<typename ttt1, typename ttt2> void addIntersect(set<ttt2>& mbe, ttt1 b1, ttt1 e1, ttt1 b2, ttt1 e2)
	{
		int counter = 0;
		for (auto it1 = b1; it1 != e1; it1++)
		{
			for (auto it2 = b2; it2 != e2; it2++)
				if (*it2 == *it1)
				{
					mbe.emplace(*it1);
					counter++;
					break;
				};

		};

		return counter;
	};

	template<typename ttt1, typename ttt2> void addIntersect(vector<ttt2>& mbe, ttt1 be1, ttt1 be2)
	{
		int counter = 0;
		for (auto& v1 : be1)
		{
			for (auto& v2 : be2)
				if (v2 == v1)
				{
					mbe.emplace_back(v1);
					counter++;
					break;
				};

		};

		return counter;
	};

	template<typename ttt1, typename ttt2> void addIntersect(set<ttt2>& mbe, ttt1 be1, ttt1 be2)
	{
		int counter = 0;
		for (auto& v1 : be1)
		{
			for (auto& v2 : be2)
				if (v2 == v1)
				{
					mbe.emplace(v1);
					counter++;
					break;
				};

		};

		return counter;
	};

	// ������ ������������C��: ��������, ���������, �������������

	struct EqClass // ����� ������� ���������������
	{
		// ������
		std::set<std::set<int>> d;

		// ������: �����������
		EqClass() {};
		EqClass(const std::set<std::set<int>>& d1): d(d1) {};

		// ������: �� �����������

		std::set<std::set<int>> static add(const std::set<std::set<int>>& s1, const std::set<std::set<int>>& s2)  // ����������� 2-� ������� ������� ���������������
		{
			std::set<std::set<int>> result = s1;

			for (auto& elem : s2) result.emplace(elem);

			return result;
		};

		EqClass operator+(const EqClass& v1) const   // ����������� 2-� ������� ������� ��������������� 
		{
			EqClass v;
			v.d=add(d, v1.d);
			return v;
		};

		EqClass& operator+=(const EqClass& v1)   // ����������� 2-� ������� ������� ��������������� 
		{
			for (auto& elem : v1.d) d.emplace(elem);			
			return *this;
		};

		void static compress(std::set<std::set<int>>& s) // ����� ���� �������������� ������� ���������� ����� ������� - �� ������������. ������� ������������, ���� ���� �������������� ������
		{
			std::vector<std::set<int>> v;

			for (auto& elem : s) v.emplace_back(elem);

			for (bool needDo = true; needDo;)
			{
				needDo = false;

				for (int l1 = 0; l1 < v.size(); l1++)
				{
					if (v[l1].empty()) continue;

					for (int l2 = l1 + 1; l2 < v.size(); l2++)
					{
						if (v[l2].empty()) continue;

						std::set<int> c3;

						for (auto& v111 : v[l1])
							for (auto& v222 : v[l2])
								if (v111 == v222) c3.emplace(v111);

						if (c3.empty()) continue;

						needDo = true;

						for(auto& v12 : v[l2])	v[l1].emplace(v12);
						v[l2].clear();

					};
				};
			};

			s.clear();

			for (auto& elem : v) if (!elem.empty()) s.emplace(elem);
		};

		EqClass& compress() // ����� ���� �������������� ������� ���������� ����� ������� - �� ������������. ������� ������������, ���� ���� �������������� ������
		{
			compress(d);
			return *this;
		};

		std::vector<std::vector<int>> static toVect(const std::set<std::set<int>>& s, int n = 0) 
		// ����� ������� ��������������� ���������� ��������, ��� ������ ������ - ����� ������ �������, ��������� - ������ ��������, ������������� ���		
		{
			for (auto& el1 : s)
				for (auto& el2 : el1)
					if (n < el2 + 1) n = el2 + 1;

			std::vector<std::vector<int>> res(n);

			for (auto& elem : s)
			{

				for (auto ell : elem) res[*elem.begin()].push_back(ell);

				std::set<int>::iterator it1 = elem.begin();

				it1++;

				for (; it1 != elem.end(); it1++) res[*it1] = res[*elem.begin()];
			};


			for (int l = 0; l < n; l++)
				if (res[l].empty()) res[l].push_back(l);

			return res;
		};

		std::vector<std::vector<int>> toVect(int n = 0) const
		// ����� ������� ��������������� ���������� ��������, ��� ������ ������ - ����� ������ �������, ��������� - ������ ��������, ������������� ���		
		{
			return toVect(d, n);
		};

		std::set<std::set<int>> static addC(const std::set<std::set<int>>& s1, const std::set<std::set<int>>& s2) // ����� ��������������� �� ������ ����� �� ���
		{
			std::vector<std::set<int>> v1, v2;

			for (auto& elem : s1) v1.emplace_back(elem);
			for (auto& elem : s2) v2.emplace_back(elem);

			for (auto& vl1 : v1)
				for (auto& vl2 : v2)
				{
					if (vl2.empty()) continue;

					std::set<int> c3;

					for (auto& v111 : vl1)
						for (auto& v222 : vl2)
							if (v111 == v222) c3.emplace(v111);

					if (c3.empty()) continue;
					{
						for (auto& v12 : vl2)	vl1.emplace(v12);						
						vl2.clear();

					};
				};

			std::set<std::set<int>> result;

			for (auto& elem : v1) result.emplace(elem);

			for (auto& vl2 : v2) if (!(vl2.empty())) result.emplace(vl2);

			return result;
		};
	};

	// ������ � TUPLE

	template<class T> int tupleSize(const T& t) // ���������� ��������� TUPLE
	{
		return tuple_size<T>::value;
	};

	// ��� ������������ ������

	template<class T> struct ring
	{
		// ������
		vector<T> v;
		int cur;
		int realsize;

		// ������: �����������
		ring(int maxsize1, const T& t) : v(maxsize1, t), cur(0), realsize(0) {};

		// ������: �� �����������
		operator T&() { return v[cur]; }; // ������ � �������� �������� ������
		T& back() { return v[cur]; }; // ������ � �������� �������� ������
		T& operator[](int l) { return v[(l + cur+ v.size()) % (v.size())]; }; // ���������� ������ � �������� ������, ���������� �� l ������� �� ��������. l ����� ���� ������������
		T& to(int l = 1) { while (l < 0) l += (v.size());  cur = (l + cur) % (v.size()); return v[cur]; }; // ������� ������� ������� ����� l ������� �� ������� ��������. ���������� ������ � ������ �������� ��������

		vector<T> toVect() // ������ ����� ������ � ������. ������ ������� ������� - ������� ������� ������, ����� � ������ ��� �� ������ �� �����������
		{
			if (realsize < v.size()) return vector<T>();
			vector<T> res(v.begin() + cur, v.end());
			if (cur>0) res.insert(res.end(), v.begin(), v.begin() + cur);
			return res;
		};

		T& addAfterCur(const T& t, int n1 = 1) // �������� ����� �������� �������� n1 ����������� �������� t. 
		{
			v.insert(v.begin() + cur + 1, n1, t);
			return v[cur];
		};

		T& next()
		{
			cur++;
			if (realsize < v.size() && cur < v.size()) realsize = cur;
			else
			{
				cur = 0;
				realsize = v.size();
			};

			return v[cur];
		};

		T& next(const T& t)
		{
			v[cur] = t;

			cur++;
			if (realsize < v.size() && cur < v.size()) realsize = cur;
			else
			{
				cur = 0;
				realsize = v.size();
			};

			return v[cur];
		};

		T& set(int l = 0)
		{
			cur = realsize = 0;
			return v[cur];
		};

		ring<T>& operator=(const T& t)
		{
			v[cur] = t; return *this;
		};

		bool isFull() { return realsize == v.size(); };
	};

	// ����������

	namespace Math
	{
		// ********* ���������� - ��� double n ������ ����� ������� ��� ����� �����

		int modf2int(int n, double d);

		// **** ���������� - ���������� � ������������

		vector<double> calcMean(const vector<double>& indata, int ddim = 1, double ddAct = 0.6);
		// ���������� �������� ��������. ddim - ����������� ������, indata - ������� ������ ������, ����� ����� ������� ������� �� �������, 
		// �� ����� ����� ������ dAct-���� (0<dAct<1) ����� ������� � ����� �������� ��������� ������� � ��������� ������� ��� �� ���� ���������. 
		// ������ ��� ������� ������� ����������.

		// **** ���������� - ������� ������������� ������������ ��������� �����

		template<class T1, class T2> vector < tuple<int, double, T1, T2>> regTimeData(double tMin, double tStep, int num, double dt1max, double dt2max, const vector<pair<double, T1>>& vt1, const vector<pair<double, T2>>& vt2)
			// ��������� ��� ������� ���� ������������ ��������� ����� � ���� ����������
			// ��������������, ��� ������� vt1 � vt2 ������������� �� �����������.
		{
			vector < tuple<int, double, T1, T2>> res;

			int lvt1 = 0, lvt2 = 0;
			double ltd = tMin;
			for (int lc = 0; lc < num; lc++, ltd += tStep)
			{
				while (lvt1<vt1.size() - 1 && abs(ltd - vt1[lvt1].first) >= abs(ltd - vt1[lvt1 + 1].first)) lvt1++;
				while (lvt2<vt2.size() - 1 && abs(ltd - vt2[lvt2].first) >= abs(ltd - vt2[lvt2 + 1].first)) lvt2++;

				if (abs(ltd - vt1[lvt1].first) > dt1max || abs(ltd - vt2[lvt2].first) > dt2max) continue;

				res.emplace_back(lc + 1, ltd, vt1[lvt1].second, vt2[lvt2].second);

			};

			return res;
		};

		template<class T1> vector < tuple<int, double, T1>> regTimeData(double tMin, double tStep, int num, double dt1max, const vector<pair<double, T1>>& vt1)
			// ��������� ��� �������������� ������������� ���������� ���� � ����������
			// ��������������, ��� ������ vt1 ������������ �� �����������.
		{
			vector < tuple<int, double, T1>> res;

			int lvt1 = 0;
			double ltd = tMin;
			for (int lc = 0; lc < num; lc++, ltd += tStep)
			{
				while (lvt1<vt1.size() - 1 && abs(ltd - vt1[lvt1].first) >= abs(ltd - vt1[lvt1 + 1].first)) lvt1++;

				if (abs(ltd - vt1[lvt1].first) > dt1max) continue;

				res.emplace_back(lc + 1, ltd, vt1[lvt1].second);				
			};

			return res;
		};

		// **** ���������� - ���������� ����- � ����������� ������ �� 0--1 ����� �������� � ��������� �� ������ ����������

		bool arrNorm01(vector<double>::iterator dataArr3, int siz3, int subsiz = 3, double minDiff = 1e-8);
		// �� ����� �������� dataArr3 �� ������, ���������� ������������. �������� ������ ����� �������� ���������������.
		// ������ ����� ��������� ���� x y z t x y z t x y z t � ��� �����, ��� x y z t �������� ����� ������ ����������� subsiz (����� 4)
		// ����� ����� ����� (x y z t) � ������ siz3 ���� (����� 3 �����). ������ ���������� x y z t ����������� ����������, �� ���� ��� ��� ������� min max
		// ����� ���� ���� max-min<minDiff, �� ������ ���������� �������������� ����, �������� x=0
		// ���� max-min>=minDiff, �� ������ ���������� ������� ����������������� x:=(x-min)/(max-min)
		// ����� ����� min=0, max=1
		// ������ ���������� �������������� ����������. ���� ��� ���� ��������� max-min<minDiff, ������� ���������� false
		// � ������ ��������� ������� ���������� true

		tuple<bool, vector<double>> mayArrNorm01(const vector<double>::iterator dataArr3, int siz3, int subsiz = 3, double minDiff = 1e-8);
		// �� ����� �������� dataArr3 �� ������, ���������� ������������. ������������ �� �������������.
		// ������ ����� ��������� ���� x y z t x y z t x y z t � ��� �����, ��� x y z t �������� ����� ������ ����������� subsiz (����� 4)
		// ����� ����� ����� (x y z t) � ������ siz3 ���� (����� 3 �����). 
		// ������ ���������� �������������� ����������. ���� ��� ���� ��������� max-min<minDiff, ������� ���������� false
		// � ������ ��������� ������� ���������� true, � ����� ������, � ������� ������� subsiz ���������, ����� subsiz ����������
		// ��� ����������, ��� ������� max-min<minDiff �� ������ min=1 max=0

		void arrNorm01(const vector<double>::iterator dataArr3, int siz3, int subsiz, const vector<double>& minmax1);
		// �� ����� �������� dataArr3 �� ������, ���������� ������������. �������� ������ ����� �������� ���������������.
		// ������ ����� ��������� ���� x y z t x y z t x y z t � ��� �����, ��� x y z t �������� ����� ������ ����������� subsiz (����� 4)
		// ����� ����� ����� (x y z t) � ������ siz3 ���� (����� 3 �����). ������ ���������� x y z t ����������� ����������, ��� ��� ������ min max � ������� minmax1
		// (������ subsiz ���������, ����� subsiz ����������).
		// ����� ����, ���� max<=min, �� ������ ���������� �������������� ����, �������� x=0
		// ����� ������ ���������� ������� ����������������� x:=(x-min)/(max-min)
		// ������ ���������� �������������� ����������. 

		// **** ���������� - ���������� ������ ��� ���������

		struct LearnRecord
			// ����� ��� �������� ��������� ������� - ������ ������ � ��������� ���������� ����  ���� double,double,double,double,...
			// ��� ������������: � ������������ ������ ������� � ������� ������� ����� �����, ������� ���� � �������� � �������� ������ ������ �����
			// ������� ���� ����
			// add(double d) ��� operator()(double d) ��������� � ����� ����� ������� � ����� d
			// clear() ��� operator()() ������� ����
			// write() ��� w() ��������� ���� � ������ �������+����������������������+��������, 
			// ����� ���� ���������������������� ���������� �� �������, � ���� ����� ������ � ����� � ������������ ��������� ������
			// ����� write(fn) ��� w(fn) �������� � ���� � ������ fn
			// ����� ������������ cntWstr() ��� cntWstr(int �����������) - ����������� ������� � ������ wstring � �������� ������
		{
			// ������

			int prec;
			vector<double> vdat; // ���������� ����� �������
			wofstream wof;
			bool wod; // write on delete ���� true �� ���������� ��������� w().			

			// ������: �� �����������

			// ������ � �������

			inline double& operator[](int l) { return vdat[l]; }; // ������ ������ � ���������

			void clear() { vdat.clear(); }; // �������� ������

			// ������ � �������: ���������� �����
			
			void operator()(double d1) { vdat.push_back(d1); };
			void operator()(double d1, double d2) 
			{
				vdat.push_back(d1); 
				vdat.push_back(d2);
			};
			template<class... T> void operator()(double d1, double d2, T... dats)
			{
				vdat.push_back(d1);
				operator()(d2, dats...);
			};			

			// ������ � �������

			bool open(const wstring& fname)
			{
				wof.open(fname);
				return wof.is_open();
			};

			void close()
			{
				wof.close();
			};

			void w()
			{
				for (int l = 0; l < vdat.size(); l++)
				{
					if (l > 0) wof << L',';

					wstringstream ss;
					ss << setprecision(prec) << vdat[l];
					wof << ss.str();
				};

			};

			void w(double dat1)
			{
				wstringstream ss;
				ss << setprecision(prec) << dat1;
				wof << ss.str();
			};

			void w(double dat1, double dat2)
			{
				w(dat1);
				wof << L',';
				w(dat2);
			};

			template<class... T> void w(double dat1, double dat2, T... dats)
			{
				w(dat1);
				wof << L',';
				w(dat2, dats...);
			};

			void w(wofstream& wof1)
			{
				for (int l = 0; l < vdat.size(); l++)
				{
					if (l > 0) wof1 << L',';

					wstringstream ss;
					ss << setprecision(prec) << vdat[l];
					wof1 << ss.str();
				};

			};

			void w(ofstream& wof1)
			{
				for (int l = 0; l < vdat.size(); l++)
				{
					if (l > 0) wof1 << ',';

					stringstream ss;
					ss << setprecision(prec) << vdat[l];
					wof1 << ss.str();
				};

			};

			void w(const wstring& fname)
			{
				wof.open(fname);

				w();

				wof.close();

			};

			// ������ ������ � ������ wof

			template<class T> LearnRecord& operator<<(const T& t) { wof << t; return *this; };

			LearnRecord& operator<<(const string& t) { wof << MySyst::MyUTF8::s2w(t); return *this; };

			static vector<int> ri(const wstring& fname)
			{
				wifstream wif(fname);

				if (!wif) return vector<int>();

				vector<int> res;

				if (MySyst::MyUTF8::one) MySyst::MyUTF8::one->operator()(wif);

				wstring wstr;

				std::getline(wif, wstr); // ������ ������ 

				wif.close();

				vector<vector<wstring>> vw = MySyst::mySplit(wstr, vector<wchar_t>(), vector<wchar_t>(1, L','));

				for (auto& chh : vw)
				{
					int l = 0;
					sscanf(MySyst::MyUTF8::w2s(chh[0]).c_str(), "%d", &l);
					res.push_back(l);
				};

				return res;
			};

			static vector<double> rd(const wstring& fname)
			{
				wifstream wif(fname);

				if (!wif) return vector<double>();

				vector<double> res;

				if (MySyst::MyUTF8::one) MySyst::MyUTF8::one->operator()(wif);

				wstring wstr;

				std::getline(wif, wstr); // ������ ������ 

				wif.close();

				vector<vector<wstring>> vw = MySyst::mySplit(wstr, vector<wchar_t>(), vector<wchar_t>(1, L','));

				for (auto& chh : vw)
				{
					double d = 0;
					sscanf(MySyst::MyUTF8::w2s(chh[0]).c_str(), "%lf", &d);
					res.push_back(d);
				};

				return res;
			};

			static vector<string> rstr(const wstring& fname)
			{
				wifstream wif(fname);

				if (!wif) return vector<string>();

				vector<string> res;

				if (MySyst::MyUTF8::one) MySyst::MyUTF8::one->operator()(wif);

				wstring wstr;

				std::getline(wif, wstr); // ������ ������ 

				wif.close();

				vector<vector<wstring>> vw = MySyst::mySplit(wstr, vector<wchar_t>(), vector<wchar_t>(1, L','));

				for (auto& chh : vw)
					res.push_back(MySyst::MyUTF8::w2s(chh[0]));

				return res;
			};

			// ������: �����������

			LearnRecord(int prec1 = 11)
				: prec(prec1), wod(false)
			{
				if (MySyst::MyUTF8::one) MySyst::MyUTF8::one->operator()(wof);
			};

			LearnRecord(const wstring& fname, bool wod1 = false, int prec1 = 11)
				: prec(prec1), wod(wod1)
			{
				if (MySyst::MyUTF8::one) MySyst::MyUTF8::one->operator()(wof);
				wof.open(fname);
			};

			~LearnRecord()
			{
				if (wof.is_open())
				{
					if (wod) w();
					wof.close();
				};
			};
		};
		
	};
};

// ****************** MyFS ********************

namespace MyFS
{
	// ��������� ������� �����
	struct nullstream
	{
		// ������: �����������
		nullstream() {};
		nullstream(const string& s) {};
		nullstream(const wstring& s) {};

		// ������: �� �����������
		void close() {};

		template<typename t1> nullstream& operator<<(const t1&) { return *this; };
		template<typename t1> nullstream& operator>>(t1&) { return *this; };
	};

	// ����� ��� ���������-����������� ������ �� ������� � � ����, ����� �����������
	struct logstream
	{
		// ������
		int state; // 0-������ ���, 1-������ �� �������, 2-������ � ����, 3-������ � � ���� � �� �������
		ofstream ofs; // ���� ��� ������
		static nullstream nullfs; // ������� �����
		double startTime; // ����� ������ �������
		static logstream* one;

		// ������: �� �����������

		// ������� � ������� �����
		template<typename t1> logstream& open(const t1& s) 
		{
			if ((state / 2) % 2 == 1)
			{
				if (ofs.is_open()) ofs.close();
				ofs.open(s);
			};
				return *this;
		};
		logstream& close() { if (ofs.is_open()) ofs.close(); return *this; };

		template<typename t1> logstream&  operator<<(const t1& s) 
		{
			if (state % 2 == 1) cerr << s;
			if ((state / 2) % 2 == 1) ofs << s;						
			return *this;
		};

		// ������� �������
		double gettime() { return MySyst::time_d() - startTime; };

		void timeMark()
		{
			double dt = MySyst::time_d() - startTime;
			int t = static_cast<int>(dt);
			int h = t / 3600;
			int m = (t % 3600) / 60;
			int s = t % 60;
			char ch[100];
			sprintf(ch, "%0i-%02i-%02i", h, m, s);
			string str = u8"\n����� � ������� ������������� (����-������-�������): "s + string(ch );
			operator<<(str);
		};

		// ���������� �����
		void setMark(const string& lab1)
		{
			string str = u8"\n���������� �����: "s + lab1;
			operator<<(str);
		};
		
		// ������: �����������

		logstream(int state_ = 0) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  if (one == NULL) one = this; };
		logstream(const string& fname, int state_ = 3) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  open(fname);  if (one == NULL) one = this; };
		template<typename t1> logstream(int state_, const t1& s) : logstream(state_) { open(s); };
		~logstream() { close(); };						
	};

	struct wlogstream
	{
		// ������
		int state; // 0-������ ���, 1-������ �� �������, 2-������ � ����, 3-������ � � ���� � �� �������
		wofstream ofs; // ���� ��� ������
		static nullstream nullfs; // ������� �����
		double startTime; // ����� ������ �������
		static wlogstream* one;

		// ������: �� �����������

		// ������� � ������� �����
		template<typename t1> wlogstream& open(const t1& s) 
		{
			if ((state / 2) % 2 == 1)
			{
				if (ofs.is_open()) ofs.close();
				ofs.open(s);
			};
			return *this;
		};

		wlogstream& close(const wstring& wstr=L"") 
		{ 
			if (ofs.is_open())
			{
				*this << L"\n���������� �������� ������. ������������ ���������: " << wstr;
				timeMark();
				ofs.close();
			};
			return *this;
		};

		template<typename t1> wlogstream&  operator<<(const t1& s)
		{
			if (state % 2 == 1) wcerr << s;
			if ((state / 2) % 2 == 1) ofs << s;
			return *this;
		};

		// ������� �������
		double gettime() { return MySyst::time_d() - startTime; };

		void timeMark()
		{
			double dt = MySyst::time_d() - startTime;
			int t = static_cast<int>(dt);
			int h = t / 3600;
			int m = (t % 3600) / 60;
			int s = t % 60;
			char ch[100];
			sprintf(ch, "%0i-%02i-%02i", h, m, s);
			wstring wstr = L"\n����� � ������� ������������� (����-������-�������): " + MySyst::MyUTF8::s2w(string(ch));
			operator<<(wstr);
		};

		// ���������� �����
		void setMark(const wstring& lab1)
		{
			wstring wstr = L"\n���������� �����: " + lab1;
			operator<<(wstr);
		};

		// ������: �����������

		wlogstream(int state_ = 0) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  if (one == NULL) one = this; };
		wlogstream(const wstring& fname, int state_ = 3) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  open(fname);  if (one == NULL) one = this; };
		template<typename t1>  wlogstream(int state1_, const t1& s) : wlogstream(state1_) { open(s); };		
		~wlogstream() 
		{ 
			if (ofs.is_open())
			{
				*this << L"\n���������� ���������� �������.";
				timeMark();
				ofs.close();
			};
		};
	};

	// ��� ����� � ������ �����
	void filesearchadd(std::wstring pathtempl1, std::vector<std::wstring>& shortfnames);  // ���������� ����� ������, ���� �� ���������
	std::vector<std::wstring> filesearch(std::wstring pathtempl1); // ���������� ����� ������, ���� �� ���������
	std::tuple<std::vector<std::wstring>, std::vector<std::wstring>> wfiledirfpsearch(std::wstring pathtempl1, bool needPath=true); // ���������� ����� ������ � �����, ���� ���������/�� ���������
	std::tuple<std::vector<std::string>, std::vector<std::string>> filedirfpsearch(std::string pathtempl1, bool needPath = true); // ���������� ����� ������ � �����, ����  ���������/�� ���������

	// �������� ������� � ����� �����
	bool fileexist(std::wstring filename1);
	int filesize(const std::wstring& filename1); // -1 ���� ���� �����������
	int existFileDir(std::wstring filedirName);  // 0 ���� ��� �����������, 1 ���� ����, 2 ���� ����������

	// ������� ������, �����������, ������� � ������� ���� ��� �����
	string binreadtxt(std::wstring fname);
	inline bool mkdir(std::wstring path1) { return CreateDirectoryW(path1.c_str(), NULL); };
	inline bool rmdir(std::wstring path1) { return RemoveDirectoryW(path1.c_str()); };
	inline bool move(std::wstring path1, std::wstring path2) 
	{
		if(fileexist(path2.c_str())) DeleteFileW(path2.c_str());
		return MoveFileW(path1.c_str(), path2.c_str()); 
	};
	inline bool copy(std::wstring path1, std::wstring path2, bool bFailIfExists = false) { return CopyFileW(path1.c_str(), path2.c_str(), bFailIfExists); };
	inline bool del(std::wstring path1) { return DeleteFileW(path1.c_str()); };
	inline bool dels(std::wstring pathpattern1)
	{
		std::vector<std::wstring> fns = get<0>(wfiledirfpsearch(pathpattern1));
		
		for (auto f : fns)
		{
			DeleteFileW(f.c_str());
		};

		return !fns.empty();
	};

	inline bool mkdirclear(std::wstring path1)
	{
		bool b = mkdir(path1);
		dels(path1 + L"\\*.*");
		return b;
	};

	// ����������� � ��������� ����� ������� � �����

	bool getfiletime(const std::wstring& fname1, double* t1p, double* t2p = 0, double* t3p = 0); // ������� ��������, ���������� �������, ��������� ������ (����� � 1 ��� 1970)
	tuple<double, double, double> getfiletime(const std::wstring& fname1); // ������� ��������, ���������� �������, ��������� ������ (����� � 1 ��� 1970)
	tuple<double, double, double> getfiletime(const std::string& fname1); // ������� ��������, ���������� �������, ��������� ������ (����� � 1 ��� 1970)
	bool setfiletime(std::wstring fname1, double* t1p = 0, double* t2p = 0, double* t3p = 0); // ������� ��������, ���������� �������, ��������� ������ (����� � 1 ��� 1970) 

	// ����������� �������� ����

	wstring getcurdir(); // ���������� ������� ����
	bool setcurdir(const wstring& w1); // ������������� ������� ����

	wstring getExecPathFilename(); // ���������� ������ ��� ���������� ���������

};

