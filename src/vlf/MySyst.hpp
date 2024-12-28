#pragma once

#include <iostream>

#include <fstream>

#include <sstream>

#include <iomanip>

#include <thread>

#include <mutex>

// Необходимые контейнерные типы:
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

//***************** МАКРОСЫ MySyst ****************

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
	// СИСТЕМНОЕ

	struct MyUTF8 // РУССКАЯ ЛОКАЛИЗАЦИЯ И UTF-8 В ПОТОКАХ С ПОДДЕРЖКОЙ КИРИЛЛИЦЫ И ВСЕ ДЛЯ РАБОТЫ С КОДИРОВКАМИ
	{
		struct my_numpunct : std::numpunct<char>
		{
			std::string do_grouping() const { return ""; };
		};

		// Данные 
		const std::locale utf8_locale; // Локаль
		const std::locale utf8_locale0; // Локаль
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>  cX; // Перекодировщик UTF-8 		
		std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t>  cX16; // Перекодировщик UTF-16 	
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>  cX816; // Перекодировщик UTF-16 	

		// Данные статические
		static MySyst::MyUTF8* one; // Указатель на последнюю локализацию
		
		// Методы конструктор
		MyUTF8()
		:utf8_locale(std::locale("Russian"), new std::codecvt_utf8<wchar_t>), // UTF-8 для ввода-вывода
		utf8_locale0(utf8_locale, new my_numpunct)
		{
			setlocale(LC_ALL, "Russian");		
			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);
			one = this;
		}; // Локализация текста и консоли    
		
		// Методы не конструктор - локализация файловых потоков
		inline void operator()(std::ifstream& s) { s.imbue(utf8_locale); };
		inline void operator()(std::ofstream& s) { s.imbue(utf8_locale); };
		inline void operator()(std::wifstream& s) { s.imbue(utf8_locale); };
		inline void operator()(std::wofstream& s) { s.imbue(utf8_locale); };

		inline void loc0(std::ifstream& s) { s.imbue(utf8_locale0); };
		inline void loc0(std::ofstream& s) { s.imbue(utf8_locale0); };
		inline void loc0(std::wifstream& s) { s.imbue(utf8_locale0); };
		inline void loc0(std::wofstream& s) { s.imbue(utf8_locale0); };

		// Методы не конструктор - перекодировщик UTF-8 в/из двухбайтную кодировку
		inline std::wstring w(const std::string& s1)  { return cX.from_bytes(s1); };
		inline std::string s(const std::wstring& s1)  { return cX.to_bytes(s1); };
		inline std::wstring operator()(const std::string& s1)  { return cX.from_bytes(s1); };
		inline std::string  operator()(const std::wstring& s1)  { return cX.to_bytes(s1); };

		std::vector<std::wstring> w(const std::vector<std::string>& s1) ;
		std::vector<std::string>  s(const std::vector<std::wstring>& s1) ;
		std::vector<std::wstring> operator()(const std::vector<std::string>& s1) ;
		std::vector<std::string>  operator()(const std::vector<std::wstring>& s1) ;
		
		
		// Перекодировщики между двухбайтными и одно-многобайтными кодировками
		// Значения для кодировок: 1251 - это cp1251, CP_UTF8 - это UTF-8

		std::wstring static s2w(const std::string& str, int codePage1= CP_UTF8);
				
		std::string static w2s(const std::wstring& wstr, int codePage1= CP_UTF8);
		
		std::string static recode(const std::string& str, int fromCod, int toCod);
		
		// Ввод параметров командной строки и перекодировка в UTF-8

		std::vector<std::string> static cmdArgs(int argc, char* argv[]);
		std::vector<std::wstring> static wcmdArgs(int argc, char* argv[]);

		// Методы не конструктор - чтение и запись файловых потоков в строки

		bool static readTxt(const std::wstring& ifname1, std::string& target1, int stepcount = 0, int* alreadySize = NULL, double* relSize1=NULL);

		std::string static readTxt(const std::wstring& ifname1, int stepcount = 0, int* alreadySize = NULL, double* relSize1=NULL);
	};
		
	// РАБОТА С ДАТОЙ И ВРЕМЕНЕМ
		
	std::string time2str(unsigned int t); // Формат даты 2020-04-12--22-04-15

	std::string time33str(unsigned int t); // Формат даты 2020-04-12 22-04-15

	bool istime33str(std::string s); // Проверяет, имеет ли строка s вид выдачи программы std::string time33str(unsigned int t)

	int istime33strvlast(std::string s); // Проверяет, имеет ли хвост строки s вид даты 2020-04-12--22-04-15 v123  Если да, возвращает длину хвоста, если нет, возвращает 0.

	string time2str7(double t, const string& fmt = string("%04i-%02i-%02i--%02i-%02i-%02i-%03i"), int n0 = 0, int n1 = 1, int n2 = 2, int n3 = 3, int n4 = 4, int n5 = 5, int n6 = 6);
	// Форматная выдача времени. t-это время, fmt-форматная строка в духе sprintf, n0--n6 указывает, что на позиции fmt надо ставить: 
	// n=0 год, n=1 месяц(01--12), n=2 день, n=3 час(00--24), n=4 минута, n=5 секунда, n=6 миллисекунда

	inline wstring time2wstr7(double t, const string& fmt = string("%04i-%02i-%02i--%02i-%02i-%02i-%03i"), int n0 = 0, int n1 = 1, int n2 = 2, int n3 = 3, int n4 = 4, int n5 = 5, int n6 = 6)
		// Форматная выдача времени. t-это время, fmt-форматная строка в духе sprintf, n0--n6 указывает, что на позиции fmt надо ставить: 
		// n=0 год, n=1 месяц(01--12), n=2 день, n=3 час(00--24), n=4 минута, n=5 секунда, n=6 миллисекунда
	{
		return MyUTF8::s2w(time2str7(t, fmt, n0, n1, n2, n3, n4, n5, n6));
	};

	std::tuple<int,int,int,int,int,int> time2tuple(unsigned int t); // Дата как 2020,04,12,22,04,15  (год,месяц,день, час, минута, секунда)

	std::vector<int> time2vector(unsigned int t); // Дата как 2020,04,12,22,04,15  (год,месяц,день, час, минута, секунда)

	std::vector<int> time2vector7(double t); // Дата как 2020,04,12,22,04,15,452  (год,месяц,день, час, минута, секунда, миллисекунда)	

	int date2timei(int year1, int month1 = 1, int day1 = 1, int h1 = 0, int m1 = 0, int s1 = 0);
		// Из стандартных номеров года, месяца, числа, часа, минуты, секунды получает число секунд с 1 января 1070г.

	int date2timei(vector<int>::iterator it1);
		// Из стандартных номеров *it1, месяца *(it1+1), числа *(it1+2), часа *(it1+3), минуты *(it1+5), секунды  *(it1+5) получает число секунд с 1 января 1070г.

	std::string sec2hhmmssStr(long int t); // Секунды в ЧЧ:ММ:СС или ДДД:ЧЧ:ММ:СС

	tuple<int, int, int, int> sec2dhms(int dt); // Преобразует интервал dt секунд в (дни, часы, минуты, секунды)

	tuple<int, int, int, int, double> sec2dhms(double dt); // Преобразует интервал dt секунд в (дни, часы, минуты, секунды, дробная часть секунд)

	vector<int> sec2dhms7(double dt); // Преобразует интервал dt секунд в (дни, часы, минуты, секунды, миллисекунды)

	string sec2str7(double t, const string& fmt = string("%02i:%02i:%02i"), int n0 = 1, int n1 = 2, int n2 = 3, int n3 = 4, int n4 = 4);
	// Преобразует интервал dt секунд в (дни, часы, минуты, секунды, миллисекунды)
	// Форматная выдача времени. t-это время, fmt-форматная строка в духе sprintf, n0--n4 указывает, что на позиции fmt надо ставить: 
	// n=0 день, n=1 час(00--24), n=2 минута, n=3 секунда, n=4 миллисекунда

	inline wstring sec2wstr7(double t, const string& fmt = string("%02i:%02i:%02i"), int n0 = 1, int n1 = 2, int n2 = 3, int n3 = 4, int n4 = 4)
	// Преобразует интервал dt секунд в (дни, часы, минуты, секунды, миллисекунды)
	// Форматная выдача времени. t-это время, fmt-форматная строка в духе sprintf, n0--n4 указывает, что на позиции fmt надо ставить: 
	// n=0 день, n=1 час(00--24), n=2 минута, n=3 секунда, n=4 миллисекунда
	{
		return MyUTF8::s2w(sec2str7(t, fmt, n0,  n1,  n2,  n3,  n4 ));
	};

	double wstrRest62time(std::wstring s); //  В хвосте строки находит 7 чисел и интерпретирует как ГОД МЕСЯЦ ДЕНЬ ЧАС МИНУТА СЕКУНДА МИЛЛИСЕКУНДА. Отсутствующие числа заменяются нулями

	double wstrRest52time(std::wstring s); //  В хвосте строки находит 6 чисел и интерпретирует как ГОД МЕСЯЦ ДЕНЬ ЧАС МИНУТА СЕКУНДА. Отсутствующие числа заменяются нулями 

	double time_d(FILETIME ft); // Из секунд с 1601 в секунды с 1970

	FILETIME time_d(double ftd); // Из секунд с 1970 в секунды с 1601

	double time_d(); // Системное время секунды с 1970

	tuple<string, int, int, int, int, int, int> inline time_of_compilation() // Время компиляции текущего модуля
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

	// *********** РАЗБОР СТРОК, ПАРСИНГ И Т.Д.*************** 

	vector<vector<wstring>> mySplit(const wstring& wstr, const vector<wchar_t>& wrazdelv, const vector<wchar_t>& wsbrosv = {});
	// Разбор строки файла. 
	// При встрече ЛЮБОЙ подстроки из вектора wsbrosv происходит увеличение первого индекса результата, сама подстрока в результат не вставляется, счетчик ожидаемой строки сбрасывавется в нуль
	// При встрече ОЖИДАЕМОЙ подстроки (то есть индекс которой равен счетчику ожидаемой строки) из вектора wrazdelv происходит увеличение на единицу
	// второго индекса результата и счетчика ожидаемой строки, сама подстрока в результат не вставляется
	// все остальные символы посимвольно копируются в текущий элемент результата

	vector<vector<wstring>> mySplit(const wstring& wstr, const vector<wstring>& wrazdelv, const vector<wstring>& wsbrosv = {});
	// Разбор строки файла. 
	// При встрече ЛЮБОЙ подстроки из вектора wsbrosv происходит увеличение первого индекса результата, сама подстрока в результат не вставляется, счетчик ожидаемой строки сбрасывавется в нуль
	// При встрече ОЖИДАЕМОЙ подстроки (то есть индекс которой равен счетчику ожидаемой строки) из вектора wrazdelv происходит увеличение на единицу
	// второго индекса результата и счетчика ожидаемой строки, сама подстрока в результат не вставляется
	// все остальные символы посимвольно копируются в текущий элемент результата
	
	// ПРЕОБРАЗОВАНИЕ ТИПОВ ЧИСЛО В СТРОКУ И ОБРАТНО

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

	// ************ ОБЕРТКИ ДЛЯ SPRINTF

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


	// ********** ПРЕОБРАЗОВАНИЕ СТРОКИ С ЧИСЛОВЫМИ ВКРАПЛЕНИЯМИ В ВЕКТОР ЧИСЕЛ ******************

	vector<int> wstr2vectint(wstring::iterator wsbegin, wstring::iterator wsend);

	// ********* ДЛЯ double n ЗНАКОВ ПОСЛЕ ЗАПЯТОЙ В СТРОКУ n СИМВОЛОВ С ВЕДУЩИМ НУЛЁМ

	string modf2str(int n, double d);
	wstring modf2wstr(int n, double d);

	// РАЗБОР И СБОРКА ПОЛНОГО ПУТИ ВЕКТОРА	НА ПУТЬ, ИМЯ И ТИП (ТО ЕСТЬ РАСШИРЕНИЕ)
		
	struct wfilepathnameext
	{
		// Данные 
		std::wstring p, n, e; // Имя, тип, расширение

		// Методы конструктор
		wfilepathnameext();
		wfilepathnameext(std::wstring fullfname);

		// Методы не конструктор
		wfilepathnameext& operator()(std::wstring fullfname); // Инициализирует класс новыи полным именем файла
		
		std::wstring operator()(); // Собирает и возвращает полное имя

		std::wstring pn(); // Собирает и возвращает путь и краткое имя
		
		std::wstring ne(); // Возвращает краткое имя и расширение
		
		std::wstring ne(const wfilepathnameext& f1); // Присваивает классу имя и расширение (без пути) от f1, сохраняя путь прежним, и возвращает краткое имя и расширение
		
		std::wstring ne(std::wstring fn1); // Инициализирует класс именем и расширением (без пути) от fn1, сохраняя путь прежним, и возвращает краткое имя и расширение
		
		// Методы статические

		std::vector<std::wstring> static shortNames(const std::vector<std::wstring>& fullname); // Из вектора полных имен получаем вектор кратких имен без расширений
		std::vector<std::wstring> static shortNameExts(const std::vector<std::wstring>& fullname); // Из вектора полных имен получаем вектор кратких имен c расширениями
		
	};

	struct filepathnameext
	{
		// Данные 
		std::string p, n, e; // Имя, тип, расширение

							  // Методы конструктор
		filepathnameext();
		filepathnameext(std::string fullfname);

		// Методы не конструктор
		filepathnameext& operator()(std::string fullfname); // Инициализирует класс новыи полным именем файла

		std::string operator()(); // Собирает и возвращает полное имя

		std::string pn(); // Собирает и возвращает путь и краткое имя

		std::string ne(); // Возвращает краткое имя и расширение

		std::string ne(const filepathnameext& f1); // Присваивает классу имя и расширение (без пути) от f1, сохраняя путь прежним, и возвращает краткое имя и расширение

		std::string ne(std::string fn1); // Инициализирует класс именем и расширением (без пути) от fn1, сохраняя путь прежним, и возвращает краткое имя и расширение

										   // Методы статические

		std::vector<std::string> static shortNames(const std::vector<std::string>& fullname); // Из вектора полных имен получаем вектор кратких имен без расширений
		std::vector<std::string> static shortNameExts(const std::vector<std::string>& fullname); // Из вектора полных имен получаем вектор кратких имен c расширениями

	};
	
	std::vector<int> invsorttabl(std::vector<int>& s);	

	template<typename ttt1, typename ttt2> bool interSect(pair<ttt1, ttt2> m12, std::vector<pair<ttt2, ttt1>>& v12) // Фиксирует хотя бы одно пересечение полуинтервала el [) с любым полуинтервалом из v12 
	{
		bool res = false;

		for (int l = 0; (l < v12.size()) && !res; l++)
		{
			if (m12.first < v12[l].second && v12[l].first < m12.second)  res = true;
		};

		return res;

	};
	


	// ****************** Работа с map<> ************

	template<typename ttt1, typename ttt2> ttt2 fromMap_or(const std::map<ttt1, ttt2>& map1, const ttt1& obj, const ttt2& alt)
	{
		auto it1 = map1.find(obj);
		if (it1 == map1.end()) return alt;
		else return it1->second;
	};
		
	// Комбинаторика и переборы

	struct Perebor // Структура для перебора векторных вариантов
	{
	private:
		// Данные приватные
		std::vector<int> v, s; // v - переменная, s - размер
		int lVar, nVar;
		bool retcode; // True при нормальном шаге false при выходе 
	
	public:
		// Методы конструктор

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

		// Методы не конструктор

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

	// Вставляет в вектор mbe только те b1---e1 которые в нём отсутствуют
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

	// Вставляет в набор mbe только те b1---e1 которые в нём отсутствуют
	template<typename ttt1, typename ttt2> void merge(set<ttt2>& mbe, ttt1 b1, ttt1 e1)
	{
		for (auto it = b1; it != e1; it++)
		{
			if(mbe.find(*it)==mbe.end())	 mbe.emplace(*it);
		};
	};

	// Вставляет в вектор mbe только те элементы контейнера be1 которые в нём отсутствуют
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

	// Вставляет в набор mbe только те элементы контейнера be1 которые в нём отсутствуют
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

	// КЛАССЫ ЭКВИВАЛЕНТНОCТИ: СОЗДАНИЕ, ОБРАБОТКА, ИСПОЛЬЗОВАНИЕ

	struct EqClass // Набор классов эквивалентности
	{
		// Данные
		std::set<std::set<int>> d;

		// Методы: конструктор
		EqClass() {};
		EqClass(const std::set<std::set<int>>& d1): d(d1) {};

		// Методы: не конструктор

		std::set<std::set<int>> static add(const std::set<std::set<int>>& s1, const std::set<std::set<int>>& s2)  // Объединение 2-х наборов классов эквивалентности
		{
			std::set<std::set<int>> result = s1;

			for (auto& elem : s2) result.emplace(elem);

			return result;
		};

		EqClass operator+(const EqClass& v1) const   // Объединение 2-х наборов классов эквивалентности 
		{
			EqClass v;
			v.d=add(d, v1.d);
			return v;
		};

		EqClass& operator+=(const EqClass& v1)   // Объединение 2-х наборов классов эквивалентности 
		{
			for (auto& elem : v1.d) d.emplace(elem);			
			return *this;
		};

		void static compress(std::set<std::set<int>>& s) // Любая пара пересекающихся классов заменяется одним классом - их объединением. Процесс продолжается, пока есть пересекающиеся классы
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

		EqClass& compress() // Любая пара пересекающихся классов заменяется одним классом - их объединением. Процесс продолжается, пока есть пересекающиеся классы
		{
			compress(d);
			return *this;
		};

		std::vector<std::vector<int>> static toVect(const std::set<std::set<int>>& s, int n = 0) 
		// Набор классов эквивалентности заменяется вектором, где первый индекс - номер любого объекта, результат - вектор объектов, эквивалентных ему		
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
		// Набор классов эквивалентности заменяется вектором, где первый индекс - номер любого объекта, результат - вектор объектов, эквивалентных ему		
		{
			return toVect(d, n);
		};

		std::set<std::set<int>> static addC(const std::set<std::set<int>>& s1, const std::set<std::set<int>>& s2) // Карта эквивалентности на основе графа из ГИД
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

	// РАБОТА С TUPLE

	template<class T> int tupleSize(const T& t) // Количество элементов TUPLE
	{
		return tuple_size<T>::value;
	};

	// МОИ КОНТЕЙНЕРНЫЕ КЛАССЫ

	template<class T> struct ring
	{
		// Данные
		vector<T> v;
		int cur;
		int realsize;

		// Методы: конструктор
		ring(int maxsize1, const T& t) : v(maxsize1, t), cur(0), realsize(0) {};

		// Методы: не конструктор
		operator T&() { return v[cur]; }; // Доступ к текущему элементу кольца
		T& back() { return v[cur]; }; // Доступ к текущему элементу кольца
		T& operator[](int l) { return v[(l + cur+ v.size()) % (v.size())]; }; // Возвращает доступ к элементу кольца, сдвинутого на l позиций от текущего. l может быть отрицательно
		T& to(int l = 1) { while (l < 0) l += (v.size());  cur = (l + cur) % (v.size()); return v[cur]; }; // Считать текущим элемент через l позиций от данного текущего. Возвращает доступ к новому текущему элементу

		vector<T> toVect() // Выдача всего кольца в вектор. Первый элемент вектора - текущий элемент кольца, далее в вектор идёт всё кольцо по возрастанию
		{
			if (realsize < v.size()) return vector<T>();
			vector<T> res(v.begin() + cur, v.end());
			if (cur>0) res.insert(res.end(), v.begin(), v.begin() + cur);
			return res;
		};

		T& addAfterCur(const T& t, int n1 = 1) // Вставить после текущего элемента n1 экземпляров элемента t. 
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

	// МАТЕМАТИКА

	namespace Math
	{
		// ********* МАТЕМАТИКА - ДЛЯ double n ЗНАКОВ ПОСЛЕ ЗАПЯТОЙ КАК ЦЕЛОЕ ЧИСЛО

		int modf2int(int n, double d);

		// **** МАТЕМАТИКА - УСРЕДНЕНИЯ И СТАБИЛИЗАЦИЯ

		vector<double> calcMean(const vector<double>& indata, int ddim = 1, double ddAct = 0.6);
		// Вычисление среднего значения. ddim - размерность данных, indata - выборка данных подряд, когда будет найдено среднее по выборке, 
		// то будет взята только dAct-доля (0<dAct<1) самых близких к этому среднему элементов выборки и вычислено среднее уже от этих элементов. 
		// Именно это среднее функция возвращает.

		// **** МАТЕМАТИКА - СЛИЯНИЕ РЕГУЛЯРИЗАЦИЯ НЕРЕГУЛЯРНЫХ ВРЕМЕННЫХ РЯДОВ

		template<class T1, class T2> vector < tuple<int, double, T1, T2>> regTimeData(double tMin, double tStep, int num, double dt1max, double dt2max, const vector<pair<double, T1>>& vt1, const vector<pair<double, T2>>& vt2)
			// Программа для слияния двух нерегулярных временнЫх рядов в один регулярный
			// Предполагается, что вектора vt1 и vt2 отсортированы по возрастанию.
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
			// Программа для преобразования нерегулярного временнОго ряда в регулярный
			// Предполагается, что вектор vt1 отсортирован по возрастанию.
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

		// **** МАТЕМАТИКА - НОРМИРОВКА ОДНО- и МНОГОМЕРНЫХ ДАННЫХ НА 0--1 ПОИСК МИНИМУМА И МАКСИМУМА ПО КАЖДОЙ КООРДИНАТЕ

		bool arrNorm01(vector<double>::iterator dataArr3, int siz3, int subsiz = 3, double minDiff = 1e-8);
		// На входе итератор dataArr3 на запись, подлежащую нормализации. Исходная запись будет заменена нормализованной.
		// Запись имеет структуру вида x y z t x y z t x y z t и так далее, где x y z t является одной точкой размерности subsiz (здесь 4)
		// Всего таких точек (x y z t) в записи siz3 штук (здесь 3 штуки). Каждая координата x y z t нормируется независимо, то есть для нее находим min max
		// после чего если max-min<minDiff, то данная координата приравнивается нулю, например x=0
		// Если max-min>=minDiff, то данная координата линейно преобразовывается x:=(x-min)/(max-min)
		// чтобы стало min=0, max=1
		// Каждая координата обрабатывается независимо. Если для всех координат max-min<minDiff, функция возвращает false
		// В других ситуациях функция возвращает true

		tuple<bool, vector<double>> mayArrNorm01(const vector<double>::iterator dataArr3, int siz3, int subsiz = 3, double minDiff = 1e-8);
		// На входе итератор dataArr3 на запись, подлежащую нормализации. Нормализация не ппроизводится.
		// Запись имеет структуру вида x y z t x y z t x y z t и так далее, где x y z t является одной точкой размерности subsiz (здесь 4)
		// Всего таких точек (x y z t) в записи siz3 штук (здесь 3 штуки). 
		// Каждая координата обрабатывается независимо. Если для всех координат max-min<minDiff, функция возвращает false
		// В других ситуациях функция возвращает true, а также вектор, в котором вначале subsiz минимумов, потом subsiz максимумов
		// Для координаты, для которой max-min<minDiff на выходе min=1 max=0

		void arrNorm01(const vector<double>::iterator dataArr3, int siz3, int subsiz, const vector<double>& minmax1);
		// На входе итератор dataArr3 на запись, подлежащую нормализации. Исходная запись будет заменена нормализованной.
		// Запись имеет структуру вида x y z t x y z t x y z t и так далее, где x y z t является одной точкой размерности subsiz (здесь 4)
		// Всего таких точек (x y z t) в записи siz3 штук (здесь 3 штуки). Каждая координата x y z t нормируется независимо, для нее дается min max в векторе minmax1
		// (сперва subsiz минимумов, потом subsiz максимумов).
		// после чего, если max<=min, то данная координата приравнивается нулю, например x=0
		// Иначе данная координата линейно преобразовывается x:=(x-min)/(max-min)
		// Каждая координата обрабатывается независимо. 

		// **** МАТЕМАТИКА - ПОДГОТОВКА ДАННЫХ ДЛЯ НЕЙРОСЕТИ

		struct LearnRecord
			// Класс для создания обучающей выборки - набора файлов с текстовым содержимым вида  вида double,double,double,double,...
			// Как использовать: в конструкторе задать префикс и постфик полного имени файла, сколько цифр в счетчике и точность данных внутри файла
			// Вначале файл пуст
			// add(double d) или operator()(double d) добавляет в хвост файла запятую и число d
			// clear() или operator()() очищают файл
			// write() или w() сохраняют файл с именем префикс+ЗначениеСчетчикаФайлов+постфикс, 
			// после чего ЗначениеСчетчикаФайлов увеличится на единицу, а файл будет очищен и готов к формированию следующей записи
			// Также write(fn) или w(fn) сохранит в файл с именем fn
			// Можно использовать cntWstr() или cntWstr(int длинаСтроки) - преобразует счетчик в строку wstring с ведущими нулями
		{
			// Данные

			int prec;
			vector<double> vdat; // Содержимое файла выборки
			wofstream wof;
			bool wod; // write on delete Если true то деструктор выполняет w().			

			// Методы: не конструктор

			// Работа с данными

			inline double& operator[](int l) { return vdat[l]; }; // Прямой доступ к элементам

			void clear() { vdat.clear(); }; // Очистить запись

			// Работа с данными: добавление чисел
			
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

			// Работа с файлами

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

			// Прямой доступ к потоку wof

			template<class T> LearnRecord& operator<<(const T& t) { wof << t; return *this; };

			LearnRecord& operator<<(const string& t) { wof << MySyst::MyUTF8::s2w(t); return *this; };

			static vector<int> ri(const wstring& fname)
			{
				wifstream wif(fname);

				if (!wif) return vector<int>();

				vector<int> res;

				if (MySyst::MyUTF8::one) MySyst::MyUTF8::one->operator()(wif);

				wstring wstr;

				std::getline(wif, wstr); // Читаем строку 

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

				std::getline(wif, wstr); // Читаем строку 

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

				std::getline(wif, wstr); // Читаем строку 

				wif.close();

				vector<vector<wstring>> vw = MySyst::mySplit(wstr, vector<wchar_t>(), vector<wchar_t>(1, L','));

				for (auto& chh : vw)
					res.push_back(MySyst::MyUTF8::w2s(chh[0]));

				return res;
			};

			// Методы: конструктор

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
	// Фиктивный нулевой поток
	struct nullstream
	{
		// Методы: конструктор
		nullstream() {};
		nullstream(const string& s) {};
		nullstream(const wstring& s) {};

		// Методы: не конструктор
		void close() {};

		template<typename t1> nullstream& operator<<(const t1&) { return *this; };
		template<typename t1> nullstream& operator>>(t1&) { return *this; };
	};

	// Поток для отладочно-контрольной выдачи на консоль и в файл, легко отключаемый
	struct logstream
	{
		// Данные
		int state; // 0-выдачи нет, 1-выдача на консоль, 2-выдача в файл, 3-выдача и в файл и на консоль
		ofstream ofs; // Файл для выдачи
		static nullstream nullfs; // Нулевой поток
		double startTime; // Время начала отсчета
		static logstream* one;

		// Методы: не конструктор

		// Открыть и закрыть поток
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

		// Отметка времени
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
			string str = u8"\nВремя с момента инициализации (часы-минуты-секунды): "s + string(ch );
			operator<<(str);
		};

		// Отладочная метка
		void setMark(const string& lab1)
		{
			string str = u8"\nОтладочная метка: "s + lab1;
			operator<<(str);
		};
		
		// Методы: конструктор

		logstream(int state_ = 0) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  if (one == NULL) one = this; };
		logstream(const string& fname, int state_ = 3) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  open(fname);  if (one == NULL) one = this; };
		template<typename t1> logstream(int state_, const t1& s) : logstream(state_) { open(s); };
		~logstream() { close(); };						
	};

	struct wlogstream
	{
		// Данные
		int state; // 0-выдачи нет, 1-выдача на консоль, 2-выдача в файл, 3-выдача и в файл и на консоль
		wofstream ofs; // Файл для выдачи
		static nullstream nullfs; // Нулевой поток
		double startTime; // Время начала отсчета
		static wlogstream* one;

		// Методы: не конструктор

		// Открыть и закрыть поток
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
				*this << L"\nНормальное закрытие потока. Опциональное сообщение: " << wstr;
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

		// Отметка времени
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
			wstring wstr = L"\nВремя с момента инициализации (часы-минуты-секунды): " + MySyst::MyUTF8::s2w(string(ch));
			operator<<(wstr);
		};

		// Отладочная метка
		void setMark(const wstring& lab1)
		{
			wstring wstr = L"\nОтладочная метка: " + lab1;
			operator<<(wstr);
		};

		// Методы: конструктор

		wlogstream(int state_ = 0) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  if (one == NULL) one = this; };
		wlogstream(const wstring& fname, int state_ = 3) : state(state_) { MySyst::MyUTF8::one->operator()(ofs); startTime = MySyst::time_d();  open(fname);  if (one == NULL) one = this; };
		template<typename t1>  wlogstream(int state1_, const t1& s) : wlogstream(state1_) { open(s); };		
		~wlogstream() 
		{ 
			if (ofs.is_open())
			{
				*this << L"\nНормальное завершение объекта.";
				timeMark();
				ofs.close();
			};
		};
	};

	// Все файлы в данной папке
	void filesearchadd(std::wstring pathtempl1, std::vector<std::wstring>& shortfnames);  // Возвращает имена файлов, путь не указывает
	std::vector<std::wstring> filesearch(std::wstring pathtempl1); // Возвращает имена файлов, путь не указывает
	std::tuple<std::vector<std::wstring>, std::vector<std::wstring>> wfiledirfpsearch(std::wstring pathtempl1, bool needPath=true); // Возвращает имена файлов и папок, путь указывает/не указывает
	std::tuple<std::vector<std::string>, std::vector<std::string>> filedirfpsearch(std::string pathtempl1, bool needPath = true); // Возвращает имена файлов и папок, путь  указывает/не указывает

	// Проверка наличия и длины файла
	bool fileexist(std::wstring filename1);
	int filesize(const std::wstring& filename1); // -1 если файл отсутствует
	int existFileDir(std::wstring filedirName);  // 0 если имя отсутствует, 1 если файл, 2 если директория

	// Бинарно читать, скопировать, создать и удалить файл или папку
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

	// Определение и установка времён доступа к файлу

	bool getfiletime(const std::wstring& fname1, double* t1p, double* t2p = 0, double* t3p = 0); // Времена создания, последнего доступа, последней записи (время с 1 янв 1970)
	tuple<double, double, double> getfiletime(const std::wstring& fname1); // Времена создания, последнего доступа, последней записи (время с 1 янв 1970)
	tuple<double, double, double> getfiletime(const std::string& fname1); // Времена создания, последнего доступа, последней записи (время с 1 янв 1970)
	bool setfiletime(std::wstring fname1, double* t1p = 0, double* t2p = 0, double* t3p = 0); // Времена создания, последнего доступа, последней записи (время с 1 янв 1970) 

	// Определение текущего пути

	wstring getcurdir(); // Возвращает текущий путь
	bool setcurdir(const wstring& w1); // Устанавливает текущий путь

	wstring getExecPathFilename(); // Возвращает полное имя запущенной программы

};

