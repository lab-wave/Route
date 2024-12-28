
#include "MySyst.hpp"

using namespace std;

#pragma warning (disable : 4996)

// ****************** MySyst ********************
// ****************** MySyst ********************
// ****************** MySyst ********************

// ������� ����������� � UTF-8 � ������� � ���������� ��������� � ��� ��� ������ � �����������

MySyst::MyUTF8* MySyst::MyUTF8::one = NULL;

// ������ �� ����������� - ������ � ������ �������� ������� � ������

bool MySyst::MyUTF8::readTxt(const std::wstring& ifname1, std::string& target1, int stepcount, int* alreadySize, double* relSize1)
{
	int fsize = MyFS::filesize(ifname1);

	double relsize=0;

	if (fsize<0)
	{
		std::cerr << "\nMySyst::MyUTF8::readTxt: no input file\n";
		return false;
	};

	std::ifstream ifs(ifname1); // ������ ������ �� ����.  

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

	(*MySyst::MyUTF8::one)(ifs); // � ������� ������� UTF-8

	std::string strGraph; // ���� ����������� ���� �� �������� ������

	int siz0 = target1.size();

	int lstrcount = 0;

	static int alrSize = 0;

	for (; ifs; lstrcount++) // ���� ���� �� �������� ����
	{		
		int pc1 = int(relsize * 100);

		if (stepcount > 0 && (lstrcount % stepcount == 0)) std::cerr << "\nreadTxt reading: " << lstrcount
			<< " strs  " << target1.size() - siz0 << " of " << fsize << " bytes ( " << pc1 << "% )";

		std::string ws; // ����� ����� ���� ������� ������ �������� �����
		std::getline(ifs, ws); // ������ ������              

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

// ��������������� ����� ���� � ������������� �����������

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

// ��������������� ����� ���� � ������������� �����������
// �������� ��� ���������: 1251 - ��� cp1251, CP_UTF8 - ��� UTF-8

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

// ������ � ����������� ��������� ������

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

// ������ � ���������� �����������

// ������ � ����� � ��������

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
// ��������� ������ �������. t-��� �����, fmt-��������� ������ � ���� sprintf, n0--n6 ���������, ��� �� ������� fmt ���� �������: 
// n=0 ���, n=1 �����(01--12), n=2 ����, n=3 ���(00--24), n=4 ������, n=5 �������, n=6 ������������
{
	vector<int> pars{ n0,n1,n2,n3,n4,n5,n6 };
	vector<int> time1 = time2vector7(t);

	char ch[5000];

	sprintf(ch, fmt.c_str(), time1[n0], time1[n1], time1[n2], time1[n3], time1[n4], time1[n5], time1[n6]);
	return string(ch);
};

std::tuple<int, int, int, int, int, int>  MySyst::time2tuple(unsigned int t) // ���� ��� 2020,04,12,22,04,15  (���,�����,����, ���, ������, �������)
{
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);
	tuple<int, int, int, int, int, int> res(ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

	return res;
};

std::vector<int>  MySyst::time2vector(unsigned int t) // ���� ��� 2020,04,12,22,04,15  (���,�����,����, ���, ������, �������)
{
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);
	vector<int> res{ ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec };

	return res;
};

std::vector<int>  MySyst::time2vector7(double t) // ���� ��� 2020,04,12,22,04,15,452  (���,�����,����, ���, ������, �������, ������������)
{
	time_t ttt = t;
	struct tm* ltm = localtime(&ttt);

	double i1;
	int l3 = min(999, static_cast<int>(modf(t, &i1)* 1000.0));

	vector<int> res{ ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec, l3 };

	return res;
};

std::string MySyst::sec2hhmmssStr(long int t) // ������� � ��:��:�� ��� ���:��:��:��
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

tuple<int,int,int,int> MySyst::sec2dhms(int dt) // ����������� �������� dt ������ � (���, ����, ������, �������)
{
	int days = dt / (24 * 3600);  dt -= days*(24 * 3600);
	int hhh = dt / 3600; dt -= hhh*3600;
	int mmm = dt / 60;   dt -= mmm * 60;
	int sss = dt;
	
	return make_tuple(days,hhh,mmm,sss);
};

tuple<int, int, int, int, double> MySyst::sec2dhms(double dt) // ����������� �������� dt ������ � (���, ����, ������, �������, ������� ����� ������)
{
	int dti = dt; dt -= double(dti);

	int days = dti / (24 * 3600);  dti -= days*(24 * 3600);
	int hhh = dti / 3600; dti -= hhh * 3600;
	int mmm = dti / 60;   dti -= mmm * 60;
	int sss = dti;

	return make_tuple(days, hhh, mmm, sss, dt);
};

vector<int> MySyst::sec2dhms7(double dt) // ����������� �������� dt ������ � (���, ����, ������, �������, ������������)
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
// ����������� �������� dt ������ � (���, ����, ������, �������, ������������)
// ��������� ������ �������. t-��� �����, fmt-��������� ������ � ���� sprintf, n0--n4 ���������, ��� �� ������� fmt ���� �������: 
// n=0 ����, n=1 ���(00--24), n=2 ������, n=3 �������, n=4 ������������
{
	vector<int> pars{ n0,n1,n2,n3,n4};
	vector<int> time1 = sec2dhms7(t);

	char ch[5000];

	sprintf(ch, fmt.c_str(), time1[n0], time1[n1], time1[n2], time1[n3], time1[n4]);
	return string(ch);	
};

int MySyst::date2timei(int year1, int month1, int day1, int h1, int m1, int s1)
// �� ����������� ������� ����, ������, �����, ����, ������, ������� �������� ����� ������ � 1 ������ 1070�.
{
	struct tm tmm { s1, m1, h1, day1, month1 - 1, year1 - 1900 };

	time_t t1 = mktime(&tmm);

	return t1;
};

int MySyst::date2timei(vector<int>::iterator it1)
// �� ����������� ������� *it1, ������ *(it1+1), ����� *(it1+2), ���� *(it1+3), ������ *(it1+5), �������  *(it1+5) �������� ����� ������ � 1 ������ 1070�.
{
	return date2timei(*it1, *(it1 + 1), *(it1 + 2), *(it1 + 3), *(it1 + 4), *(it1 + 5));
};

double MySyst::wstrRest62time(std::wstring s) // � ������ ������ ������� 7 ����� � �������������� ��� ��� ����� ���� ��� ������ ������� ������������. ������������� ����� ���������� ������
{
	vector<int> vtime1 = MySyst::wstr2vectint(s.begin(),s.end());

	if (vtime1.size() < 7) vtime1.resize(7, 0);

	return double(MySyst::date2timei(vtime1.end()-7)) + 0.001*double(vtime1.back());
};

double MySyst::wstrRest52time(std::wstring s) // � ������ ������ ������� 6 ����� � �������������� ��� ��� ����� ���� ��� ������ �������. ������������� ����� ���������� ������ 
{
	vector<int> vtime1 = MySyst::wstr2vectint(s.begin(), s.end());

	if (vtime1.size() < 6) vtime1.resize(6, 0);

	return double(MySyst::date2timei(vtime1.end() - 6));
};

double MySyst::time_d(FILETIME ft) // �� ������ � 1601 � ������� � 1970
{
	ULONGLONG qwResult = (((ULONGLONG)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
	return static_cast<double>(qwResult) / 10000000.0 - 11644473600.0;   // epoch is Jan. 1, 1601: 134774 days to Jan. 1, 1970
};

FILETIME MySyst::time_d(double ftd) // �� ������ � 1970 � ������� � 1601
{
	union
	{
		ULONGLONG u64;
		FILETIME ftout;
	} u;

	u.u64 = static_cast<ULONGLONG>((ftd + 11644473600.0) * 10000000.0);

	return u.ftout;
};

double MySyst::time_d() // ��������� ����� ������� � 1970
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return time_d(ft);
};

// *********** ������ �����, ������� � �.�.*************** 

vector<vector<wstring>> MySyst::mySplit(const wstring& wstr, const vector<wchar_t>& wrazdelv, const vector<wchar_t>& wsbrosv)
// ������ ������ �����. 
// ��� ������� ����� ��������� �� ������� wsbrosv ���������� ���������� ������� ������� ����������, ���� ��������� � ��������� �� �����������, ������� ��������� ������ ������������� � ����
// ��� ������� ��������� ��������� (�� ���� ������ ������� ����� �������� ��������� ������) �� ������� wrazdelv ���������� ���������� �� �������
// ������� ������� ���������� � �������� ��������� ������, ���� ��������� � ��������� �� �����������
// ��� ��������� ������� ����������� ���������� � ������� ������� ����������
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
// ������ ������ �����. 
// ��� ������� ����� ��������� �� ������� wsbrosv ���������� ���������� ������� ������� ����������, ���� ��������� � ��������� �� �����������, ������� ��������� ������ ������������� � ����
// ��� ������� ��������� ��������� (�� ���� ������ ������� ����� �������� ��������� ������) �� ������� wrazdelv ���������� ���������� �� �������
// ������� ������� ���������� � �������� ��������� ������, ���� ��������� � ��������� �� �����������
// ��� ��������� ������� ����������� ���������� � ������� ������� ����������
{
	vector<wstring> res0(1, L"");

	vector<vector<wstring>> res(1, res0);

	int lwc = 0; // ����� ���������� �����������

	for (int lwstr = 0; lwstr < wstr.size(); lwstr++) // ������� ������ ������� ������
	{
		wchar_t wc1 = wstr[lwstr];

		bool isSbros = false;

		for (int lsbr = 0; (lsbr < wsbrosv.size()) && !isSbros; lsbr++) // ������� �����-�����
		{
			if (wsbrosv[lsbr].size() > wstr.size() - lwstr) continue; // ���� �����-������ �� ��������� � ������� �������� ������
			bool isEq = (wsbrosv[lsbr][0] == wc1);
			for (int lc = 1; (lc < wsbrosv[lsbr].size()) && isEq; lc++) // ������������ ���������
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

		// ���� ��� ����� - �� ���������

		if ((lwc<wrazdelv.size()) && (wrazdelv[lwc].size() <= wstr.size() - lwstr)) // ���� ����������� ����� ���������� � ������� ������
		{
			bool isEq = (wrazdelv[lwc][0] == wc1);
			for (int lc = 1; (lc < wrazdelv[lwc].size()) && isEq; lc++) // ������������ ���������
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

// ********** �������������� ������ � ��������� ������������ � ������ ����� ******************

vector<int> MySyst::wstr2vectint(wstring::iterator wsbegin, wstring::iterator wsend)
{
	vector<int> res;
	bool isCurrNum = false; // ���� ���������� �����

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

// ********* ��� double n ������ ����� ������� � ������ n �������� � ������� ��˨�

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


// ������ � ������ ������� ���� �������	�� ����, ��� � ��� (�� ���� ����������)

// ������ �����������
MySyst::wfilepathnameext::wfilepathnameext() : p(L""), n(L""), e(L"") {};
MySyst::wfilepathnameext::wfilepathnameext(std::wstring fullfname) { operator()(fullfname); };

MySyst::filepathnameext::filepathnameext() : p(u8""), n(u8""), e(u8"") {};
MySyst::filepathnameext::filepathnameext(std::string fullfname) { operator()(fullfname); };

MySyst::wfilepathnameext& MySyst::wfilepathnameext::operator()(std::wstring fullfname) // �������������� ����� ����� ������ ������ �����
{
	p = n = e = std::wstring(L"");

	int len = fullfname.length() - 1;

	if (len >= 0)
	{

		while (len >= 0 && fullfname[len] != '/' && fullfname[len] != '\\') len--; // �� ������ ����� len ���� ������� \\ ���� -1

		// ������ � ������� ����
		if (len >= 0)
		{
			p = fullfname.substr(0, len);
			fullfname = fullfname.substr(len + 1);
		};

		// �������� ��� �������� �� ��� � ����������
		len = fullfname.length() - 1;
		while (len >= 0 && fullfname[len] != '.') len--; // �� ������ ����� len ���� ������� ����� ���� -1;
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

MySyst::filepathnameext& MySyst::filepathnameext::operator()(std::string fullfname) // �������������� ����� ����� ������ ������ �����
{
	int len = fullfname.length() - 1;
	p = n = e = std::string(u8"");
	while (len >= 0 && fullfname[len] != '/' && fullfname[len] != '\\') len--; // �� ������ ����� len ���� ������� \\ ���� -1

																			   // ������ � ������� ����
	if (len >= 0)
	{
		p = fullfname.substr(0, len);
		fullfname = fullfname.substr(len + 1);
	};

	// �������� ��� �������� �� ��� � ����������
	len = fullfname.length() - 1;
	while (len >= 0 && fullfname[len] != '.') len--; // �� ������ ����� len ���� ������� ����� ���� -1;
	if (len < 0)
		n = fullfname;
	else
	{
		e = fullfname.substr(len + 1);
		n = fullfname.substr(0, len);
	};

	return *this;
};

std::wstring MySyst::wfilepathnameext::operator()() // �������� � ���������� ������ ���
{
	std::wstring fullfname = n;
	if (e.length() > 0) fullfname = fullfname + L"." + e;
	if (p.length() > 0) fullfname = p + L"\\" + fullfname;
	return fullfname;
};

std::string MySyst::filepathnameext::operator()() // �������� � ���������� ������ ���
{
	std::string fullfname = n;
	if (e.length() > 0) fullfname = fullfname + u8"." + e;
	if (p.length() > 0) fullfname = p + u8"\\" + fullfname;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::pn() // �������� � ���������� ���� � ������� ���
{
	std::wstring fullfname = n;
	if (p.length() > 0) fullfname = p + L"\\"s + fullfname;
	return fullfname;
};

std::string MySyst::filepathnameext::pn() // �������� � ���������� ���� � ������� ���
{
	std::string fullfname = n;
	if (p.length() > 0) fullfname = p + u8"\\"s + fullfname;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::ne() // ���������� ������� ��� � ����������
{
	std::wstring fullfname = n;
	if (e.length() > 0) fullfname = fullfname + L"." + e;
	return fullfname;
};

std::string MySyst::filepathnameext::ne() // ���������� ������� ��� � ����������
{
	std::string fullfname = n;
	if (e.length() > 0) fullfname = fullfname + u8"." + e;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::ne(const MySyst::wfilepathnameext& f1) // ����������� ������ ��� � ���������� (��� ����) �� f1, �������� ���� �������, � ���������� ������� ��� � ����������
{
	std::wstring fullfname = n = f1.n;
	if ((e = f1.e).length() > 0) fullfname = fullfname + L"." + e;
	return fullfname;
};

std::string MySyst::filepathnameext::ne(const MySyst::filepathnameext& f1) // ����������� ������ ��� � ���������� (��� ����) �� f1, �������� ���� �������, � ���������� ������� ��� � ����������
{
	std::string fullfname = n = f1.n;
	if ((e = f1.e).length() > 0) fullfname = fullfname + u8"." + e;
	return fullfname;
};

std::wstring MySyst::wfilepathnameext::ne(std::wstring fn1) // ����������� ������ ��� � ���������� (��� ����) �� fn1, �������� ���� �������, � ���������� ������� ��� � ����������
{
	wfilepathnameext f1(fn1);
	return ne(f1);
};

std::string MySyst::filepathnameext::ne(std::string fn1) // ����������� ������ ��� � ���������� (��� ����) �� fn1, �������� ���� �������, � ���������� ������� ��� � ����������
{
	filepathnameext f1(fn1);
	return ne(f1);
};
// ������ �����������

std::vector<std::wstring>  MySyst::wfilepathnameext::shortNames(const std::vector<std::wstring>& fullname) // �� ������� ������ ���� �������� ������ ������� ���� ��� ����������
{
	std::vector<std::wstring> n1;

	for (auto& fulln : fullname)
		n1.push_back(wfilepathnameext(fulln).n);

	return n1;
};

std::vector<std::string>  MySyst::filepathnameext::shortNames(const std::vector<std::string>& fullname) // �� ������� ������ ���� �������� ������ ������� ���� ��� ����������
{
	std::vector<std::string> n1;

	for (auto& fulln : fullname)
		n1.push_back(filepathnameext(fulln).n);

	return n1;
};

std::vector<std::wstring>  MySyst::wfilepathnameext::shortNameExts(const std::vector<std::wstring>& fullname) // �� ������� ������ ���� �������� ������ ������� ���� c ������������
{
	std::vector<std::wstring> n1;

	for (auto& fulln : fullname)
		n1.push_back(wfilepathnameext(fulln).ne());

	return n1;
};

std::vector<std::string>  MySyst::filepathnameext::shortNameExts(const std::vector<std::string>& fullname) // �� ������� ������ ���� �������� ������ ������� ���� c ������������
{
	std::vector<std::string> n1;

	for (auto& fulln : fullname)
		n1.push_back(filepathnameext(fulln).ne());

	return n1;
};

// *********** ������������� � ���������� ****************

std::vector<int> MySyst::invsorttabl(std::vector<int>& s)
{
	std::vector<int> invs(s.size(), 0);

	for (int l = 0; l < invs.size(); l++) invs[s[l]] = l;

	return invs;
};

// *********** ���������� ******************************

// ********* ���������� - ��� double n ������ ����� ������� ��� ����� �����

int MySyst::Math::modf2int(int n, double d)
{
	double i1;
	int p10 = 1;
	for (int l = 0; l < n; l++) p10 = p10 * 10;

	return min(p10 - 1, int(modf(d, &i1)* double(p10)));
};

// **** ���������� - ���������� � ������������

vector<double> MySyst::Math::calcMean(const vector<double>& indata, int ddim, double ddAct)
// ���������� �������� ��������. ddim - ����������� ������, indata - ������� ������ ������, ����� ����� ������� ������� �� �������, 
// �� ����� ����� ������ dAct-���� (0<dAct<1) ����� ������� � ����� �������� ��������� ������� � ��������� ������� ��� �� ���� ���������. 
// ������ ��� ������� ������� ����������.
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

	sort(sdist.begin(), sdist.end()); // ���������� �� ����������� ���������� �� ��������

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

// **** ���������� - ���������� ����- � ����������� ������ �� 0--1 ����� �������� � ��������� �� ������ ����������

bool MySyst::Math::arrNorm01(vector<double>::iterator dataArr3, int siz3, int subsiz, double minDiff)
// �� ����� �������� dataArr3 �� ������, ���������� ������������. �������� ������ ����� �������� ���������������.
// ������ ����� ��������� ���� x y z t x y z t x y z t � ��� �����, ��� x y z t �������� ����� ������ ����������� subsiz (����� 4)
// ����� ����� ����� (x y z t) � ������ siz3 ���� (����� 3 �����). ������ ���������� x y z t ����������� ����������, �� ���� ��� ��� ������� min max
// ����� ���� ���� max-min<minDiff, �� ������ ���������� �������������� ����, �������� x=0
// ���� max-min>=minDiff, �� ������ ���������� ������� ����������������� x:=(x-min)/(max-min)
// ����� ����� min=0, max=1
// ������ ���������� �������������� ����������. ���� ��� ���� ��������� max-min<minDiff, ������� ���������� false
// � ������ ��������� ������� ���������� true
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
// �� ����� �������� dataArr3 �� ������, ���������� ������������. ������������ �� �������������.
// ������ ����� ��������� ���� x y z t x y z t x y z t � ��� �����, ��� x y z t �������� ����� ������ ����������� subsiz (����� 4)
// ����� ����� ����� (x y z t) � ������ siz3 ���� (����� 3 �����). 
// ������ ���������� �������������� ����������. ���� ��� ���� ��������� max-min<minDiff, ������� ���������� false
// � ������ ��������� ������� ���������� true, � ����� ������, � ������� ������� subsiz ���������, ����� subsiz ����������
// ��� ����������, ��� ������� max-min<minDiff �� ������ min=1 max=0
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
// �� ����� �������� dataArr3 �� ������, ���������� ������������. �������� ������ ����� �������� ���������������.
// ������ ����� ��������� ���� x y z t x y z t x y z t � ��� �����, ��� x y z t �������� ����� ������ ����������� subsiz (����� 4)
// ����� ����� ����� (x y z t) � ������ siz3 ���� (����� 3 �����). ������ ���������� x y z t ����������� ����������, ��� ��� ������ min max � ������� minmax1
// (������ subsiz ���������, ����� subsiz ����������).
// ����� ����, ���� max<=min, �� ������ ���������� �������������� ����, �������� x=0
// ����� ������ ���������� ������� ����������������� x:=(x-min)/(max-min)
// ������ ���������� �������������� ����������. 
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

// ��� ����� � ������ �����

void MyFS::filesearchadd(std::wstring pathtempl, std::vector<std::wstring>& shortfnames) // ���������� ����� ������, ���� �� ���������
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

std::vector<std::wstring> MyFS::filesearch(std::wstring pathtempl)  // ���������� ����� ������, ���� �� ���������
{
	std::vector<std::wstring> w;
	filesearchadd(pathtempl, w);
	return w;
};

std::tuple<std::vector<std::wstring>, std::vector<std::wstring>> MyFS::wfiledirfpsearch(std::wstring pathtempl1, bool needPath) // ���������� ����� ������ � �����, ����  ���������/�� ���������
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

std::tuple<std::vector<std::string>, std::vector<std::string>> MyFS::filedirfpsearch(std::string pathtempl1, bool needPath) // ���������� ����� ������ � �����, ����  ���������/�� ���������
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

// �������� ������� � ����� �����

bool MyFS::fileexist(std::wstring filename1)
{
	wifstream f(filename1);
	if (!f) return false;
	f.close();
	return true;
};

int MyFS::filesize(const std::wstring& filename1)  // -1 ���� ���� �����������
{
	std::ifstream f(filename1, std::ios::binary | std::ios::ate);
	if (!f) return -1;
	int s = f.tellg();
	f.close();
	return s;
};

int MyFS::existFileDir(std::wstring filedirName) // 0 ���� ��� �����������, 1 ���� ����, 2 ���� ����������
{
	DWORD ftyp = GetFileAttributesW(filedirName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return 0;  // ���������� ������ �����������!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return 2;   // ����������!

	return 1;    // �� ����������, �� ���� ����!
};

// ����������� � ��������� ����� ������� � �����

bool MyFS::setfiletime(std::wstring fname1, double* t1p, double* t2p, double* t3p) // ������� ��������, ���������� �������, ��������� ������ 
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

bool MyFS::getfiletime(const std::wstring& fname1, double* t1p, double* t2p, double* t3p) // ������� ��������, ���������� �������, ��������� ������ 
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

tuple<double,double,double> MyFS::getfiletime(const std::wstring& fname1) // ������� ��������, ���������� �������, ��������� ������ (����� � 1 ��� 1970) 
{
	double t1 = -1, t2 = -1, t3 = -1;
	if(getfiletime(fname1, &t1, &t2, &t3))	return make_tuple(t1, t2, t3);
	else return make_tuple(-1, -1, -1);
};

tuple<double, double, double> MyFS::getfiletime(const std::string& fname1) // ������� ��������, ���������� �������, ��������� ������ (����� � 1 ��� 1970) 
{
	double t1 = -1, t2 = -1, t3 = -1;
	if (getfiletime(MySyst::MyUTF8::s2w(fname1), &t1, &t2, &t3)) return make_tuple(t1, t2, t3);
	else return make_tuple(-1, -1, -1);
};

// ����������� �������� ���� (������� ����������)

wstring MyFS::getcurdir() // ���������� ������� ����
{
	int size1 = GetCurrentDirectory(0, NULL);
		
	vector<wchar_t> buff(size1);

	GetCurrentDirectory(size1, &buff[0]);

	return wstring(&buff[0]);
};

bool MyFS::setcurdir(const wstring& w1) // ������������� ������� ����
{
	return SetCurrentDirectory(w1.c_str());
};

wstring MyFS::getExecPathFilename() // ���������� ������ ��� ���������� ���������
{
	const int size1 = 10000;
	
	vector<wchar_t> buff(size1);

	int size2=GetModuleFileName(0, &buff[0], size1);

	buff.erase(buff.begin() + size2, buff.end());
	
	return wstring(&buff[0]);
};