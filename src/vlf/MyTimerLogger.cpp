#include "MyTimerLogger.hpp"


MyTimerLogger::MyTimerLogger(const std::string& fname1, const std::string& fname2, const std::string& name_ )
        :name(name_), mainT(MySyst::time_d()), startTimeStr(MySyst::time2str(mainT))
        , startT(0), mainDT(0), fullDT(0), logfile(fname1+ startTimeStr +fname2)
{
    startT = mainT;
    sstr << "\n\n" << name << "Started.";

    sstr2log("\n"s);
};

MyTimerLogger::MyTimerLogger(MySyst::MyUTF8& m8, const std::string& fname1, const std::string& fname2, const std::string& name_)
    :name(name_), mainT(MySyst::time_d()), startTimeStr(MySyst::time2str(mainT))
    , startT(0), mainDT(0), fullDT(0), logfile(fname1 + startTimeStr + fname2)
{
    m8(logfile);
    startT = mainT;
    sstr << "\n\n" << name << "Started.";

    sstr2log("\n"s);
};

MyTimerLogger::~MyTimerLogger()
{
    setMainT();

    sstr << "\n\n" << name << "Finished.";

    sstr2log("\n\n"s);

    logfile.close();
};

void MyTimerLogger::setMainT()
{
    const double curT = MySyst::time_d();
    mainDT = curT - mainT;
    fullDT = curT - startT;
    mainT = curT;
    sstr.str(string());
};

void MyTimerLogger::clear()
{    
    sstr.str(string());
};

void MyTimerLogger::sstr2log(const string& strFinish)
{
    sstr << " CurrTime=" << MySyst::time2str(mainT) << "  DT=" << mainDT << "  fullDT=" << fullDT << strFinish;
    logfile << sstr.str();
    std::cout << sstr.str();
    logfile.flush();

};

void MyTimerLogger::sstr2log(ofstream& logfile2, const string& strFinish)
{
    sstr << " CurrTime=" << MySyst::time2str(mainT) << "  DT=" << mainDT << "  fullDT=" << fullDT << strFinish;
    logfile << sstr.str();
    logfile2 << sstr.str();
    std::cout << sstr.str();
    logfile.flush();
    logfile2.flush();
};

void MyTimerLogger::sstr2logOnly(const string& strFinish)
{
    sstr << strFinish;
    logfile << sstr.str();
    std::cout << sstr.str();
    logfile.flush();

};

void MyTimerLogger::sstr2logOnly(ofstream& logfile2, const string& strFinish)
{
    sstr << strFinish;
    logfile << sstr.str();
    logfile2 << sstr.str();
    std::cout << sstr.str();
    logfile.flush();
    logfile2.flush();
};


int MyTimerLogger::utf8len(const string& str)
{
    int len = 0;

    for (const char c : str)
    {
        if ((c & 0xc0) != 0x80) len++;
    };

    return len;


};