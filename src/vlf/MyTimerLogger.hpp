#pragma once

#include "MySyst.hpp"

using namespace std;

struct MyTimerLogger
 {
        
        double mainT, startT, mainDT, fullDT;
        const string startTimeStr;
        string name;
        stringstream sstr;
        ofstream logfile;

        MyTimerLogger(const std::string& fname1=""s, const std::string& fname2=""s, const std::string& timerName_ = "");

        MyTimerLogger(MySyst::MyUTF8& m8,  const std::string& fname1, const std::string& fname2, const std::string& timerName_ = "");
           
        ~MyTimerLogger();
       
        void setMainT();
        
        void clear();

        void sstr2log(const string& strFinish=""s);
        
        void sstr2log(ofstream& logfile2, const string& strFinish = ""s);

        void sstr2logOnly(const string& strFinish = ""s);

        void sstr2logOnly(ofstream& logfile2, const string& strFinish = ""s);

        static int utf8len(const string& str);
};

    