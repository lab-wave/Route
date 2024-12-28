
#include "MySyst.hpp"

using namespace std;

#pragma warning (disable : 4996)

// ****************** MySyst ********************
// ****************** MySyst ********************
// ****************** MySyst ********************

// РУССКАЯ ЛОКАЛИЗАЦИЯ И UTF-8 В ПОТОКАХ С ПОДДЕРЖКОЙ КИРИЛЛИЦЫ И ВСЕ ДЛЯ РАБОТЫ С КОДИРОВКАМИ

MySyst::MyUTF8* MySyst::MyUTF8::one = NULL;

// Методы не конструктор - чтение и запись файловых потоков в строки

bool MySyst::MyUTF8::readTxt(const std::wstring& ifname1, std::string& target1, int stepcount, int* alreadySize, double* relSize1)
{
	int fsize = MyFS::filesize(ifname1);

	double relsize=0;

	if (fsize<0)
	{
		std::cerr << "\nMySyst::MyUTF8::readTxt: no input file\n";
		return false;
	};

	std::ifstream ifs(ifname1); // Отсюда читаем ЖД граф.  

	if (!ifs)
	{
		std::cerr << "\nMySyst::MyUTF8::readTxt: no input file\n";
		return false;
	};

	if (!MySyst::MyUTF8::one)
	{
		ifs.close();
		std::cerr << "\nMySyst::MyUTF8::readTxt: not init UTF-8\n";
		return false;
	};

	(*MySyst::MyUTF8::one)(ifs); // В кодовой таблице UTF-8

	std::string strGraph; // Сюда накапливаем граф из входного потока

	int siz0 = target1.size();

	int lstrcount = 0;

	static int alrSize = 0;

	for (; ifs; lstrcount++) // Цикл пока не кончился файл
	{		
		int pc1 = int(relsize * 100);

		if (stepcount > 0 && (lstrcount % stepcount == 0)) std::cerr << "\nreadTxt reading: " << lstrcount
			<< " strs  " << target1.size() - siz0 << " of " << fsize << " bytes ( " << pc1 << "% )";

		std::string ws; // Здесь будет одна текущая строка входного файла
		std::getline(ifs, ws); // Читаем строку              

		target1 = target1 + "\n " + ws;		

		relsize = double(target1.size() - siz0) / double(fsize);
		if (alreadySize) *alreadySize = alrSize = target1.size() - siz0;
		if (relSize1) *relSize1 = relsize;
	};

	if (stepcount > 0)
	{
		relsize = double(target1.size() - siz0) / double(fsize);
		if (relSize1) *relSize1 = relsize;
		int pc1 = int(relsize * 100);

		std::cerr << "\nreadTxt reading: " << lstrcount
			<< " strs  " << target1.size() - siz0 << " of " << fsize << " bytes ( " << pc1 << "% )";
		std::cerr << "\nreadTxt reading: Ok";
	};

	ifs.close();

	return true;
};

std::string MySyst::MyUTF8::readTxt(const std::wstring& ifname1, int stepcount, int* alreadySize, double* relSize1)
{
	std::string s1;
	if (readTxt(ifname1, s1, stepcount, alreadySize, relSize1)) return s1;
	return std::string();
};

// Перекодировщики между одно и многобайтными кодировками

std::vector<std::wstring> MySyst::MyUTF8::w(const std::vector<std::string>& s1)
{
	std::vector<std::wstring> res;
	for (auto& ell : s1) res.push_back(w(ell));
	return res;
};

std::vector<std::string>  MySyst::MyUTF8::s(const std::vector<std::wstring>& s1)
{
	std::vector<std::string> res;
	for (auto& ell : s1) res.push_back(s(ell));
	return res;
};

std::vector<std::wstring> MySyst::MyUTF8::operator()(const std::vector<std::string>& s1)
{
	std::vector<std::wstring> res;
	for (auto& ell : s1) res.push_back(w(ell));
	return res;
};

std::vector<std::string>  MySyst::MyUTF8::operator()(const std::vector<std::wstring>& s1)
{
	std::vector<std::string> res;
	for (auto& ell : s1) res.push_back(s(ell));
	return res;
};

// Перекодировщики между одно и многобайтными кодировками
// Значения для кодировок: 1251 - это cp1251, CP_UTF8 - это UTF-8

std::wstring MySyst::MyUTF8::s2w(const std::string& str, int codePage1)
{
	int size_w = MultiByteToWideChar(codePage1, 0, &str[0], -1, 0, 0);

	if (!size_w) return L"";

	std::vector<wchar_t> ures(size_w);

	if (MultiByteToWideChar(codePage1, 0, &str[0], -1, &ures[0], size_w))
		return std::wstring(&ures[0]);
	else return L"";
};

std::string MySyst::MyUTF8::w2s(const std::wstring& wstr, int codePage1)
{
	int size_c = WideCharToMultiByte(codePage1, 0, &wstr[0], -1, 0, 0, 0, 0);

	if (!size_c) return "";

	std::vector<char> cres(size_c);

	if (WideCharToMultiByte(codePage1, 0, &wstr[0], -1, &cres[0], size_c, 0, 0))
		return std::string(&cres[0]);
	else return "";

};

std::string MySyst::MyUTF8::recode(const std::string& str, int fromCod, int toCod)
{
	std::wstring wstr = s2w(str, fromCod);

	return w2s(wstr, toCod);
};

// РАБОТА С ПАРАМЕТРАМИ КОМАНДНОЙ СТРОКИ

std::vector<std::string> MySyst::MyUTF8::cmdArgs(int argc, char* argv[])
{
	std::vector<std::string> argstr;

	for (int l = 0; l < argc; l++) argstr.emplace_back(MySyst::MyUTF8::recode(argv[l], 1251, CP_UTF8));

	return argstr;
};

std::vector<std::wstring> MySyst::MyUTF8::wcmdArgs(int argc, char* argv[])
{
	std::vector<std::wstring> wargstr;

	for (int l = 0; l < argc; l++) wargstr.emplace_back(MySyst::MyUTF8::s2w(argv[l], 1251));

	return wargstr;
};

// РАБОТА С СИСТЕМНЫМИ ПАРАМЕТРАМИ

// РАБОТА С ДАТОЙ И ВРЕМЕНЕМ

std::string MySyst::time2str(unsigned int t)
{
	std::string calctime;
	std::stringstream ss;
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);
	ss << std::dec << std::setfill('0');
	ss << std::setw(4) << ltm->tm_year + 1900 << "-" << std::setw(2) << ltm->tm_mon + 1 << "-" << std::setw(2) << ltm->tm_mday << "--" << std::setw(2) <<
		ltm->tm_hour << "-" << std::setw(2) << ltm->tm_min << "-" << std::setw(2) << ltm->tm_sec;
	ss >> calctime;
	return calctime;
};

std::string MySyst::time33str(unsigned int t)
{
	std::stringstream ss;
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);
	ss << std::dec << std::setfill('0');
	ss << std::setw(4) << ltm->tm_year + 1900 << "-" << std::setw(2) << ltm->tm_mon + 1 << "-" << std::setw(2) << ltm->tm_mday << " " << std::setw(2) <<
		ltm->tm_hour << "-" << std::setw(2) << ltm->tm_min << "-" << std::setw(2) << ltm->tm_sec;
	return ss.str();
};

bool MySyst::istime33str(std::string s)
{
	bool istime = true;
	if (s.length() != 19) return false;
	if (s[4] != '-' || s[7] != '-' || s[10] != ' ' || s[13] != '-' || s[16] != '-') return false;
	for (int l : {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18})
	{
		if (isdigit(s[l]) == 0) { istime = false; break; };
	};

	return istime;
};

int MySyst::istime33strvlast(std::string s)
{
	
	if (s.length() < 19) return 0;

	if (s.length() == 19 && istime33str(s)) return 19;

	int slen = s.size();

	int lback = slen - 1;

	while(lback >= 0 && isdigit(s[lback]) != 0) lback--;

	if (s[lback] == '-' && slen - lback == 3 && lback >= 16 && istime33str(string(s.begin() + lback - 16, s.begin() + lback + 3)))
		return 19;

	if (s[lback] == 'v' && lback >= 20 && istime33str(string(s.begin() + lback - 20, s.begin() + lback - 1)))
		return slen-lback+20;

	return 0;
};







string MySyst::time2str7(double t, const string& fmt, int n0, int n1, int n2, int n3, int n4, int n5, int n6)
// Форматная выдача времени. t-это время, fmt-форматная строка в духе sprintf, n0--n6 указывает, что на позиции fmt надо ставить: 
// n=0 год, n=1 месяц(01--12), n=2 день, n=3 час(00--24), n=4 минута, n=5 секунда, n=6 миллисекунда
{
	vector<int> pars{ n0,n1,n2,n3,n4,n5,n6 };
	vector<int> time1 = time2vector7(t);

	char ch[5000];

	sprintf(ch, fmt.c_str(), time1[n0], time1[n1], time1[n2], time1[n3], time1[n4], time1[n5], time1[n6]);
	return string(ch);
};

std::tuple<int, int, int, int, int, int>  MySyst::time2tuple(unsigned int t) // Дата как 2020,04,12,22,04,15  (год,месяц,день, час, минута, секунда)
{
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);
	tuple<int, int, int, int, int, int> res(ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

	return res;
};

std::vector<int>  MySyst::time2vector(unsigned int t) // Дата как 2020,04,12,22,04,15  (год,месяц,день, час, минута, секунда)
{
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);
	vector<int> res{ ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec };

	return res;
};

std::vector<int>  MySyst::time2vector7(double t) // Дата как 2020,04,12,22,04,15,452  (год,месяц,день, час, минута, секунда, миллисекунда)
{
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);

	double i1;
	int l3 = min(999, static_cast<int>(modf(t, &i1)* 1000.0));

	vector<int> res{ ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec, l3 };

	return res;
};

std::string MySyst::sec2hhmmssStr(long int t) // Секунды в ЧЧ:ММ:СС или ДДД:ЧЧ:ММ:СС
{
	long int hhh = t / 3600;
	long int mmm = t / 60 - hhh * 60;
	long int sss = t - mmm * 60 - hhh * 3600;
	long int days = hhh / 24;
	hhh -= days * 24;
	std::stringstream ss;
	ss << std::dec << std::setfill('0');
	if (days > 0) ss << std::setw(3) << days << ":";
	ss << std::setw(2) << hhh << ":" << std::setw(2) << mmm << ":" << std::setw(2) << sss;

	return ss.str();
};

tuple<int,int,int,int> MySyst::sec2dhms(int dt) // Преобразует интервал dt секунд в (дни, часы, минуты, секунды)
{
	int days = dt / (24 * 3600);  dt -= days*(24 * 3600);
	int hhh = dt / 3600; dt -= hhh*3600;
	int mmm = dt / 60;   dt -= mmm * 60;
	int sss = dt;
	
	return make_tuple(days,hhh,mmm,sss);
};

tuple<int, int, int, int, double> MySyst::sec2dhms(double dt) // Преобразует интервал dt секунд в (дни, часы, минуты, секунды, дробная часть секунд)
{
	int dti = dt; dt -= double(dti);

	int days = dti / (24 * 3600);  dti -= days*(24 * 3600);
	int hhh = dti / 3600; dti -= hhh * 3600;
	int mmm = dti / 60;   dti -= mmm * 60;
	int sss = dti;

	return make_tuple(days, hhh, mmm, sss, dt);
};

vector<int> MySyst::sec2dhms7(double dt) // Преобразует интервал dt секунд в (дни, часы, минуты, секунды, миллисекунды)
{
	int dti = dt; dt -= double(dti);

	int days = dti / (24 * 3600);  dti -= days*(24 * 3600);
	int hhh = dti / 3600; dti -= hhh * 3600;
	int mmm = dti / 60;   dti -= mmm * 60;
	int sss = dti;
	int ms = min( int(dt*1000.0), 999); 

	vector<int> res{ days,hhh,mmm,sss,ms };

	return res;
};

string MySyst::sec2str7(double t, const string& fmt, int n0, int n1, int n2, int n3, int n4)
// Преобразует интервал dt секунд в (дни, часы, минуты, секунды, миллисекунды)
// Форматная выдача времени. t-это время, fmt-форматная строка в духе sprintf, n0--n4 указывает, что на позиции fmt надо ставить: 
// n=0 день, n=1 час(00--24), n=2 минута, n=3 секунда, n=4 миллисекунда
{
	vector<int> pars{ n0,n1,n2,n3,n4};
	vector<int> time1 = sec2dhms7(t);

	char ch[5000];

	sprintf(ch, fmt.c_str(), time1[n0], time1[n1], time1[n2], time1[n3], time1[n4]);
	return string(ch);	
};

int MySyst::date2timei(int year1, int month1, int day1, int h1, int m1, int s1)
// Из стандартных номеров года, месяца, числа, часа, минуты, секунды получает число секунд с 1 января 1070г.
{
	struct tm tmm { s1, m1, h1, day1, month1 - 1, year1 - 1900 };

	time_t t1 = mktime(&tmm);

	return t1;
};

int MySyst::date2timei(vector<int>::iterator it1)
// Из стандартных номеров *it1, месяца *(it1+1), числа *(it1+2), часа *(it1+3), минуты *(it1+5), секунды  *(it1+5) получает число секунд с 1 января 1070г.
{
	return date2timei(*it1, *(it1 + 1), *(it1 + 2), *(it1 + 3), *(it1 + 4), *(it1 + 5));
};

double MySyst::wstrRest62time(std::wstring s) // В хвосте строки находит 7 чисел и интерпретирует как ГОД МЕСЯЦ ДЕНЬ ЧАС МИНУТА СЕКУНДА МИЛЛИСЕКУНДА. Отсутствующие числа заменяются нулями
{
	vector<int> vtime1 = MySyst::wstr2vectint(s.begin(),s.end());

	if (vtime1.size() < 7) vtime1.resize(7, 0);

	return double(MySyst::date2timei(vtime1.end()-7)) + 0.001*double(vtime1.back());
};

double MySyst::wstrRest52time(std::wstring s) // В хвосте строки находит 6 чисел и интерпретирует как ГОД МЕСЯЦ ДЕНЬ ЧАС МИНУТА СЕКУНДА. Отсутствующие числа заменяются нулями 
{
	vector<int> vtime1 = MySyst::wstr2vectint(s.begin(), s.end());

	if (vtime1.size() < 6) vtime1.resize(6, 0);

	return double(MySyst::date2timei(vtime1.end() - 6));
};

double MySyst::time_d(FILETIME ft) // Из секунд с 1601 в секунды с 1970
{
	ULONGLONG qwResult = (((ULONGLONG)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
	return static_cast<double>(qwResult) / 10000000.0 - 11644473600.0;   // epoch is Jan. 1, 1601: 134774 days to Jan. 1, 1970
};

FILETIME MySyst::time_d(double ftd) // Из секунд с 1970 в секунды с 1601
{
	union
	{
		ULONGLONG u64;
		FILETIME ftout;
	} u;

	u.u64 = static_cast<ULONGLONG>((ftd + 11644473600.0) * 10000000.0);

	return u.ftout;
};

double MySyst::time_d() // Системное время секунды с 1970
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return time_d(ft);
};

// *********** РАЗБОР СТРОК, ПАРСИНГ И Т.Д.*************** 

vector<vector<wstring>> MySyst::mySplit(const wstring& wstr, const vector<wchar_t>& wrazdelv, const vector<wchar_t>& wsbrosv)
// Разбор строки файла. 
// При встрече ЛЮБОЙ подстроки из вектора wsbrosv происходит увеличение первого индекса результата, сама подстрока в результат не вставляется, счетчик ожидаемой строки сбрасывавется в нуль
// При встрече ОЖИДАЕМОЙ подстроки (то есть индекс которой равен счетчику ожидаемой строки) из вектора wrazdelv происходит увеличение на единицу
// второго индекса результата и счетчика ожидаемой строки, сама подстрока в результат не вставляется
// все остальные символы посимвольно копируются в текущий элемент результата
{
	vector<wstring> res0(1, L"");

	vector<vector<wstring>> res(1, res0);

	int lwc = 0;

	for (wchar_t wc1 : wstr)
	{
		bool isSbros = false;

		for (wchar_t wsbr : wsbrosv)
		{
			if (wc1 == wsbr)
			{
				res.push_back(res0);
				lwc = 0;
				isSbros = true;
				break;
			};
		};

		if (isSbros) continue;

		if (lwc<wrazdelv.size() && wc1 == wrazdelv[lwc])
		{
			lwc++;
			res.back().emplace_back(L"");
			continue;
		};

		res.back().back().push_back(wc1);
	};

	return res;
};

vector<vector<wstring>> MySyst::mySplit(const wstring& wstr, const vector<wstring>& wrazdelv, const vector<wstring>& wsbrosv)
// Разбор строки файла. 
// При встрече ЛЮБОЙ подстроки из вектора wsbrosv происходит увеличение первого индекса результата, сама подстрока в результат не вставляется, счетчик ожидаемой строки сбрасывавется в нуль
// При встрече ОЖИДАЕМОЙ подстроки (то есть индекс которой равен счетчику ожидаемой строки) из вектора wrazdelv происходит увеличение на единицу
// второго индекса результата и счетчика ожидаемой строки, сама подстрока в результат не вставляется
// все остальные символы посимвольно копируются в текущий элемент результата
{
	vector<wstring> res0(1, L"");

	vector<vector<wstring>> res(1, res0);

	int lwc = 0; // Номер ожидаемого разделителя

	for (int lwstr = 0; lwstr < wstr.size(); lwstr++) // Текущий символ входной строки
	{
		wchar_t wc1 = wstr[lwstr];

		bool isSbros = false;

		for (int lsbr = 0; (lsbr < wsbrosv.size()) && !isSbros; lsbr++) // Перебор сброс-строк
		{
			if (wsbrosv[lsbr].size() > wstr.size() - lwstr) continue; // Если сброс-строка не умещается в остатке исходной строки
			bool isEq = (wsbrosv[lsbr][0] == wc1);
			for (int lc = 1; (lc < wsbrosv[lsbr].size()) && isEq; lc++) // Посимвольное сравнение
				if (wsbrosv[lsbr][lc] != wstr[lwstr + lc])
					isEq = false;

			if (isEq)
			{
				isSbros = true; lwstr += (wsbrosv[lsbr].size() - 1);
			};
		};

		if (isSbros)
		{
			res.push_back(res0);
			lwc = 0;
			continue;
		};

		// Если был сброс - он произошел

		if ((lwc<wrazdelv.size()) && (wrazdelv[lwc].size() <= wstr.size() - lwstr)) // Если разделитель может уместиться в остатке строки
		{
			bool isEq = (wrazdelv[lwc][0] == wc1);
			for (int lc = 1; (lc < wrazdelv[lwc].size()) && isEq; lc++) // Посимвольное сравнение
				if (wrazdelv[lwc][lc] != wstr[lwstr + lc])
					isEq = false;

			if (isEq)
			{
				lwstr += (wrazdelv[lwc].size() - 1);
				lwc++;
				res.back().emplace_back(L"");
				continue;
			};
		};

		res.back().back().push_back(wc1);
	};

	return res;
};

// ********** ПРЕОБРАЗОВАНИЕ СТРОКИ С ЧИСЛОВЫМИ ВКРАПЛЕНИЯМИ В ВЕКТОР ЧИСЕЛ ******************

vector<int> MySyst::wstr2vectint(wstring::iterator wsbegin, wstring::iterator wsend)
{
	vector<int> res;
	bool isCurrNum = false; // Идет считывание числа

	vector<wchar_t> wc{ L'0',L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9' };

	for (wstring::iterator it = wsbegin; it != wsend; it++)
	{
		int ln = 0;
		while (ln < 10 && *it != wc[ln]) ln++;

		if (ln >= 10) isCurrNum = false;
		else if (isCurrNum) res.back() = res.back() * 10 + ln;
		else
		{
			res.push_back(ln);
			isCurrNum = true;
		};
	};

	return res;
};

// ********* ДЛЯ double n ЗНАКОВ ПОСЛЕ ЗАПЯТОЙ В СТРОКУ n СИМВОЛОВ С ВЕДУЩИМ НУЛЁМ

string MySyst::modf2str(int n, double d)
{
	double i1;
	int p10 = 1;
	for (int l = 0; l < n; l++) p10 = p10 * 10;
		
	int l3 = min(p10-1, int(modf(d, &i1)* double(p10)));

	string ss = MySyst::toStr(l3);

	while (ss.size() < n) ss = "0" + ss;

	return ss;
};

wstring MySyst::modf2wstr(int n, double d)
{
	double i1;
	int p10 = 1;
	for (int l = 0; l < n; l++) p10 = p10 * 10;

	int l3 = min(p10 - 1, int(modf(d, &i1)* double(p10)));

	wstring ss = MySyst::toWstr(l3);

	while (ss.size() < n) ss = L"0" + ss;

	return ss;
};


// РАЗБОР И СБОРКА ПОЛНОГО ПУТИ ВЕКТОРА	НА ПУТЬ, ИМЯ И ТИП (ТО ЕСТЬ РАСШИРЕНИЕ)

// Методы конструктор
MySyst::wfilepathnameext::wfilepathnameext() : p(L""), n(L""), e(L"") {};
MySyst::wfilepathnameext::wfilepathnameext(std::wstring fullfname) { operator()(fullfname); };

MySyst::filepathnameext::filepathnameext() : p(u8""), n(u8""), e(u8"") {};
MySyst::filepathnameext::filepathnameext(std::string fullfname) { operator()(fullfname); };

MySyst::wfilepathnameext& MySyst::wfilepathnameext::operator()(std::wstring fullfname) // Инициализирует класс новыи полным именем файла
{
	p = n = e = std::wstring(L"");

	int len = fullfname.length() - 1;

	if (len >= 0)
	{

		while (len >= 0 && fullfname[len] != '/' && fullfname[len] != '\\') len--; // На выходе цикла len либо позиция \\ либо -1

		// Найдем и отделим путь
		if (len >= 0)
		{
			p = fullfname.substr(0, len);
			fullfname = fullfname.substr(len + 1);
		};

		// Короткое имя разделим на имя и расширение
		len = fullfname.length() - 1;
		while (len >= 0 && fullfname[len] != '.') len--; // На выходе цикла len либо позиция точки либо -1;
		if (len < 0)
			n = fullfname;
		else
		{
			e = fullfname.substr(len + 1);
			n = fullfname.substr(0, len);
		};
	};

	return *this;
};

MySyst::filepathnameext& MySyst::filepathnameext::operator()(std::string fullfname) // Инициализирует класс новыи полным именем файла
{
	int len = fullfname.length() - 1;
	p = n = e = std::string(u8"");
	while (len >= 0 && fullfname[len] != '/' && fullfname[len] != '\\') len--; // На выходе цикла len либо позиция \\ либо -1

																			   // Найдем и отделим путь
	if (len >= 0)
	{
		p = fullfname.substr(0, len);
		fullfname = fullfname.substr(len + 1);
	};

	// Короткое имя разделим на имя и расширение
	len = fullfname.length() - 1;
	while (len >= 0 && fullfname[len] != '.') len--; // На выходе цикла len либо позиция точки либо -1;
	if (len < 0)
		n = fullfname;
	else
	{
		e = fullfname.substr(len + 1);
		n = fullfname.substr(0, len);
	};

	return *this;
};

std::wstring MySyst::wfilepathnameext::operator()() // Собирает и возвращает полное имя
{
	std::wstring fullfname = n;
	if (e.length() > 0) fullfname = fullfname + L"." + e;
	if (p.length() > 0) fullfname = p + L"\\" + fullfname;
	return fullfname;
};

std::string MySyst::filepathnameext::operator()() // Собирает и возвращает полное имя
{
	std::string fullfname = n;
	if (e.length() > 0) fullfname = fullfname + u8"." + e;
	if (p.length() > 0) fullfname = p + u8"\\" + fullfname;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::pn() // Собирает и возвращает путь и краткое имя
{
	std::wstring fullfname = n;
	if (p.length() > 0) fullfname = p + L"\\"s + fullfname;
	return fullfname;
};

std::string MySyst::filepathnameext::pn() // Собирает и возвращает путь и краткое имя
{
	std::string fullfname = n;
	if (p.length() > 0) fullfname = p + u8"\\"s + fullfname;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::ne() // Возвращает краткое имя и расширение
{
	std::wstring fullfname = n;
	if (e.length() > 0) fullfname = fullfname + L"." + e;
	return fullfname;
};

std::string MySyst::filepathnameext::ne() // Возвращает краткое имя и расширение
{
	std::string fullfname = n;
	if (e.length() > 0) fullfname = fullfname + u8"." + e;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::ne(const MySyst::wfilepathnameext& f1) // Присваивает классу имя и расширение (без пути) от f1, сохраняя путь прежним, и возвращает краткое имя и расширение
{
	std::wstring fullfname = n = f1.n;
	if ((e = f1.e).length() > 0) fullfname = fullfname + L"." + e;
	return fullfname;
};

std::string MySyst::filepathnameext::ne(const MySyst::filepathnameext& f1) // Присваивает классу имя и расширение (без пути) от f1, сохраняя путь прежним, и возвращает краткое имя и расширение
{
	std::string fullfname = n = f1.n;
	if ((e = f1.e).length() > 0) fullfname = fullfname + u8"." + e;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::ne(std::wstring fn1) // Присваивает классу имя и расширение (без пути) от fn1, сохраняя путь прежним, и возвращает краткое имя и расширение
{
	wfilepathnameext f1(fn1);
	return ne(f1);
};

std::string MySyst::filepathnameext::ne(std::string fn1) // Присваивает классу имя и расширение (без пути) от fn1, сохраняя путь прежним, и возвращает краткое имя и расширение
{
	filepathnameext f1(fn1);
	return ne(f1);
};
// Методы статические

std::vector<std::wstring>  MySyst::wfilepathnameext::shortNames(const std::vector<std::wstring>& fullname) // Из вектора полных имен получаем вектор кратких имен без расширений
{
	std::vector<std::wstring> n1;

	for (auto& fulln : fullname)
		n1.push_back(wfilepathnameext(fulln).n);

	return n1;
};

std::vector<std::string>  MySyst::filepathnameext::shortNames(const std::vector<std::string>& fullname) // Из вектора полных имен получаем вектор кратких имен без расширений
{
	std::vector<std::string> n1;

	for (auto& fulln : fullname)
		n1.push_back(filepathnameext(fulln).n);

	return n1;
};

std::vector<std::wstring>  MySyst::wfilepathnameext::shortNameExts(const std::vector<std::wstring>& fullname) // Из вектора полных имен получаем вектор кратких имен c расширениями
{
	std::vector<std::wstring> n1;

	for (auto& fulln : fullname)
		n1.push_back(wfilepathnameext(fulln).ne());

	return n1;
};

std::vector<std::string>  MySyst::filepathnameext::shortNameExts(const std::vector<std::string>& fullname) // Из вектора полных имен получаем вектор кратких имен c расширениями
{
	std::vector<std::string> n1;

	for (auto& fulln : fullname)
		n1.push_back(filepathnameext(fulln).ne());

	return n1;
};

// *********** КОМБИНАТОРИКА И СОРТИРОВКА ****************

std::vector<int> MySyst::invsorttabl(std::vector<int>& s)
{
	std::vector<int> invs(s.size(), 0);

	for (int l = 0; l < invs.size(); l++) invs[s[l]] = l;

	return invs;
};

// *********** МАТЕМАТИКА ******************************

// ********* МАТЕМАТИКА - ДЛЯ double n ЗНАКОВ ПОСЛЕ ЗАПЯТОЙ КАК ЦЕЛОЕ ЧИСЛО

int MySyst::Math::modf2int(int n, double d)
{
	double i1;
	int p10 = 1;
	for (int l = 0; l < n; l++) p10 = p10 * 10;

	return min(p10 - 1, int(modf(d, &i1)* double(p10)));
};

// **** МАТЕМАТИКА - УСРЕДНЕНИЯ И СТАБИЛИЗАЦИЯ

vector<double> MySyst::Math::calcMean(const vector<double>& indata, int ddim, double ddAct)
// Вычисление среднего значения. ddim - размерность данных, indata - выборка данных подряд, когда будет найдено среднее по выборке, 
// то будет взята только dAct-доля (0<dAct<1) самых близких к этому среднему элементов выборки и вычислено среднее уже от этих элементов. 
// Именно это среднее функция возвращает.
{
	vector<double> res1(ddim, 0);
	int nPoints = indata.size() / ddim;
	double dPoints = nPoints;

	for (int ld0 = 0; ld0 < ddim; ld0++)
	{
		for (int ld1 = 0; ld1 < nPoints; ld1++)
			res1[ld0] += indata[ld1*ddim + ld0];

		res1[ld0] /= dPoints;
	};

	vector<pair<double, int>> sdist(nPoints, make_pair(0, 0));

	for (int ld1 = 0; ld1 < nPoints; ld1++)
	{
		sdist[ld1].second = ld1;

		for (int ld0 = 0; ld0 < ddim; ld0++)
			sdist[ld1].first += abs(indata[ld1*ddim + ld0] - res1[ld0]);
	};

	sort(sdist.begin(), sdist.end()); // Сортировка по возрастанию расстояния до среднего

	int nActs = ddAct*dPoints;
	double dActs = nActs;

	if (nActs < nPoints)
	{
		for (int ld0 = 0; ld0 < ddim; ld0++)
		{
			res1[ld0] = 0;

			for (int ld11 = 0; ld11 < nActs; ld11++)
			{
				const int ld1 = sdist[ld11].second;
				res1[ld0] += indata[ld1*ddim + ld0];
			};

			res1[ld0] /= dActs;

		};
	};

	return res1;
};

// **** МАТЕМАТИКА - НОРМИРОВКА ОДНО- и МНОГОМЕРНЫХ ДАННЫХ НА 0--1 ПОИСК МИНИМУМА И МАКСИМУМА ПО КАЖДОЙ КООРДИНАТЕ

bool MySyst::Math::arrNorm01(vector<double>::iterator dataArr3, int siz3, int subsiz, double minDiff)
// На входе итератор dataArr3 на запись, подлежащую нормализации. Исходная запись будет заменена нормализованной.
// Запись имеет структуру вида x y z t x y z t x y z t и так далее, где x y z t является одной точкой размерности subsiz (здесь 4)
// Всего таких точек (x y z t) в записи siz3 штук (здесь 3 штуки). Каждая координата x y z t нормируется независимо, то есть для нее находим min max
// после чего если max-min<minDiff, то данная координата приравнивается нулю, например x=0
// Если max-min>=minDiff, то данная координата линейно преобразовывается x:=(x-min)/(max-min)
// чтобы стало min=0, max=1
// Каждая координата обрабатывается независимо. Если для всех координат max-min<minDiff, функция возвращает false
// В других ситуациях функция возвращает true
{
	bool notAllConst = false;

	for (int l3 = 0; l3 < subsiz; l3++)
	{
		double min3 = *(dataArr3 + l3);
		double max3 = min3;

		for (int lv = 1; lv < siz3; lv++)
		{
			tie(min3, max3) = minmax({ min3, max3, *(dataArr3 + subsiz * lv + l3) });
		};

		if (max3 - min3 >= minDiff)
		{
			notAllConst = true;

			for (int lv = 0; lv < siz3; lv++)
				*(dataArr3 + subsiz * lv + l3) = (*(dataArr3 + subsiz * lv + l3) - min3) / (max3 - min3);
		}
		else
			for (int lv = 0; lv < siz3; lv++)
				*(dataArr3 + subsiz * lv + l3) = 0;
	};

	return notAllConst;
};

tuple<bool, vector<double>> MySyst::Math::mayArrNorm01(const vector<double>::iterator dataArr3, int siz3, int subsiz, double minDiff)
// На входе итератор dataArr3 на запись, подлежащую нормализации. Нормализация не ппроизводится.
// Запись имеет структуру вида x y z t x y z t x y z t и так далее, где x y z t является одной точкой размерности subsiz (здесь 4)
// Всего таких точек (x y z t) в записи siz3 штук (здесь 3 штуки). 
// Каждая координата обрабатывается независимо. Если для всех координат max-min<minDiff, функция возвращает false
// В других ситуациях функция возвращает true, а также вектор, в котором вначале subsiz минимумов, потом subsiz максимумов
// Для координаты, для которой max-min<minDiff на выходе min=1 max=0
{
	bool notAllConst = false;

	vector<double> res(2 * subsiz);

	for (int l3 = 0; l3 < subsiz; l3++)
	{
		res[l3] = res[l3 + subsiz] = *(dataArr3 + l3);

		for (int lv = 1; lv < siz3; lv++)
		{
			tie(res[l3], res[l3 + subsiz]) = minmax({ res[l3], res[l3 + subsiz], *(dataArr3 + subsiz * lv + l3) });
		};

		if (res[l3 + subsiz] - res[l3] >= minDiff) notAllConst = true;
		else
		{
			res[l3 + subsiz] = 0;
			res[l3] = 1;
		};

	};

	return std::tuple<bool, vector<double>>(notAllConst, res);
};

void MySyst::Math::arrNorm01(const vector<double>::iterator dataArr3, int siz3, int subsiz, const vector<double>& minmax1)
// На входе итератор dataArr3 на запись, подлежащую нормализации. Исходная запись будет заменена нормализованной.
// Запись имеет структуру вида x y z t x y z t x y z t и так далее, где x y z t является одной точкой размерности subsiz (здесь 4)
// Всего таких точек (x y z t) в записи siz3 штук (здесь 3 штуки). Каждая координата x y z t нормируется независимо, для нее дается min max в векторе minmax1
// (сперва subsiz минимумов, потом subsiz максимумов).
// после чего, если max<=min, то данная координата приравнивается нулю, например x=0
// Иначе данная координата линейно преобразовывается x:=(x-min)/(max-min)
// Каждая координата обрабатывается независимо. 
{
	for (int l3 = 0; l3 < subsiz; l3++)
	{
		if (minmax1[l3 + subsiz] - minmax1[l3] >0)
		{
			for (int lv = 0; lv < siz3; lv++)
				*(dataArr3 + subsiz * lv + l3) = (*(dataArr3 + subsiz * lv + l3) - minmax1[l3]) / (minmax1[l3 + subsiz] - minmax1[l3 + subsiz]);
		}
		else
			for (int lv = 0; lv < siz3; lv++)
				*(dataArr3 + subsiz * lv + l3) = 0;
	};

};

// ****************** MyFS ********************
// ****************** MyFS ********************
// ****************** MyFS ********************

string MyFS::binreadtxt(std::wstring fname)
{
	int len1 = filesize(fname);
	if (len1 <= 0) return string("");

	ifstream wif(fname, std::fstream::in | std::fstream::binary);
	if (!wif) return string("");

	string chBuffer(len1, ' ');

	wif.read(&chBuffer[0], len1);

	wif.close();

	return chBuffer;
};

MyFS::logstream*  MyFS::logstream::one = NULL;
MyFS::wlogstream* MyFS::wlogstream::one = NULL;

// Все файлы в данной папке

void MyFS::filesearchadd(std::wstring pathtempl, std::vector<std::wstring>& shortfnames) // Возвращает имена файлов, путь не указывает
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	hf = FindFirstFile(pathtempl.c_str(), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			shortfnames.emplace_back(FindFileData.cFileName);
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	};
};

std::vector<std::wstring> MyFS::filesearch(std::wstring pathtempl)  // Возвращает имена файлов, путь не указывает
{
	std::vector<std::wstring> w;
	filesearchadd(pathtempl, w);
	return w;
};

std::tuple<std::vector<std::wstring>, std::vector<std::wstring>> MyFS::wfiledirfpsearch(std::wstring pathtempl1, bool needPath) // Возвращает имена файлов и папок, путь  указывает/не указывает
{
	std::vector<std::wstring> w, wdirs, wfiles;
	filesearchadd(pathtempl1, w);
	MySyst::wfilepathnameext fp1(pathtempl1);

	for (auto& ell : w)
	{
		if (ell == L"." || ell == L"..") continue;

		wstring fullFname= fp1.p + L"\\" + ell;

		if(needPath) ell = fullFname;

		switch (existFileDir(fullFname))
		{
		case 1: wfiles.push_back(ell); break;
		case 2: wdirs.push_back(ell); break;
		};
	};

	return make_tuple(wfiles, wdirs);
};

std::tuple<std::vector<std::string>, std::vector<std::string>> MyFS::filedirfpsearch(std::string pathtempl1, bool needPath) // Возвращает имена файлов и папок, путь  указывает/не указывает
{
	std::vector<std::wstring> w;
	std::vector<std::string> dirs, files;
	filesearchadd(MySyst::MyUTF8::s2w(pathtempl1), w);
	MySyst::wfilepathnameext fp1(MySyst::MyUTF8::s2w(pathtempl1));

	for (auto& ell : w)
	{
		if (ell == L"." || ell == L"..") continue;

		wstring fullFname = fp1.p + L"\\" + ell;

		if (needPath) ell = fullFname;

		switch (existFileDir(fullFname))
		{
		case 1: files.push_back(MySyst::MyUTF8::w2s(ell)); break;
		case 2: dirs.push_back(MySyst::MyUTF8::w2s(ell)); break;
		};
	};

	return make_tuple(files, dirs);
};

// Проверка наличия и длины файла

bool MyFS::fileexist(std::wstring filename1)
{
	wifstream f(filename1);
	if (!f) return false;
	f.close();
	return true;
};

int MyFS::filesize(const std::wstring& filename1)  // -1 если файл отсутствует
{
	std::ifstream f(filename1, std::ios::binary | std::ios::ate);
	if (!f) return -1;
	int s = f.tellg();
	f.close();
	return s;
};

int MyFS::existFileDir(std::wstring filedirName) // 0 если имя отсутствует, 1 если файл, 2 если директория
{
	DWORD ftyp = GetFileAttributesW(filedirName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return 0;  // Нормальный объект отсутствует!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return 2;   // Директория!

	return 1;    // Не директория, то есть файл!
};

// Определение и установка времён доступа к файлу

bool MyFS::setfiletime(std::wstring fname1, double* t1p, double* t2p, double* t3p) // Времена создания, последнего доступа, последней записи 
{
	FILETIME t1, t2, t3, * pt1 = 0, * pt2 = 0, * pt3 = 0;
	if (t1p != 0) pt1 = &(t1 = MySyst::time_d(*t1p));
	if (t2p != 0) pt2 = &(t2 = MySyst::time_d(*t2p));
	if (t3p != 0) pt3 = &(t3 = MySyst::time_d(*t3p));

	HANDLE hFile = CreateFileW(fname1.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!SetFileTime(hFile, pt1, pt2, pt3)) return false;
	CloseHandle(hFile);

	return true;
};

bool MyFS::getfiletime(const std::wstring& fname1, double* t1p, double* t2p, double* t3p) // Времена создания, последнего доступа, последней записи 
{
	FILETIME t1, t2, t3;
	HANDLE hFile = CreateFileW(fname1.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!GetFileTime(hFile, &t1, &t2, &t3)) return false;
	CloseHandle(hFile);

	if (t1p != 0) *t1p = MySyst::time_d(t1);
	if (t2p != 0) *t2p = MySyst::time_d(t2);
	if (t3p != 0) *t3p = MySyst::time_d(t3);

	return true;
};

tuple<double,double,double> MyFS::getfiletime(const std::wstring& fname1) // Времена создания, последнего доступа, последней записи (время с 1 янв 1970) 
{
	double t1 = -1, t2 = -1, t3 = -1;
	if(getfiletime(fname1, &t1, &t2, &t3))	return make_tuple(t1, t2, t3);
	else return make_tuple(-1, -1, -1);
};

tuple<double, double, double> MyFS::getfiletime(const std::string& fname1) // Времена создания, последнего доступа, последней записи (время с 1 янв 1970) 
{
	double t1 = -1, t2 = -1, t3 = -1;
	if (getfiletime(MySyst::MyUTF8::s2w(fname1), &t1, &t2, &t3)) return make_tuple(t1, t2, t3);
	else return make_tuple(-1, -1, -1);
};

// Определение текущего пути (текущей директории)

wstring MyFS::getcurdir() // Возвращает текущий путь
{
	int size1 = GetCurrentDirectory(0, NULL);
		
	vector<wchar_t> buff(size1);

	GetCurrentDirectory(size1, &buff[0]);

	return wstring(&buff[0]);
};

bool MyFS::setcurdir(const wstring& w1) // Устанавливает текущий путь
{
	return SetCurrentDirectory(w1.c_str());
};

wstring MyFS::getExecPathFilename() // Возвращает полное имя запущенной программы
{
	const int size1 = 10000;
	
	vector<wchar_t> buff(size1);

	int size2=GetModuleFileName(0, &buff[0], size1);

	buff.erase(buff.begin() + size2, buff.end());
	
	return wstring(&buff[0]);
};