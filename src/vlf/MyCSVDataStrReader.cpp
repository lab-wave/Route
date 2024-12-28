#include "MyCSVDataStrReader.hpp"

MyCSVDataStrReader::MyCSVDataStrReader(ifstream& ifs, const string& strRAZD)
{     
    dataStr = csvFileDecompose(ifs, strRAZD);
    
    if (dataStr.empty()) return;

    for (int l = 0; l < dataStr.front().size(); l++)
    {
        const string keyStr = (dataStr.front()[l].empty()) ? ("field"s + to_string(l)) : dataStr.front()[l];

        keyTabl[keyStr] = l;
        keyVect.emplace_back(keyStr);
    }

    dataStr.pop_front();
};

MyCSVDataStrReader::MyCSVDataStrReader()
    :keyTabl(), keyVect(), dataStr()
{};


void MyCSVDataStrReader::readCSVData(ifstream& ifs, const string& strRAZD)
{
    keyTabl.clear();

    keyVect.clear();

    dataStr.clear();

    dataStr = csvFileDecompose(ifs, strRAZD);

    if (dataStr.empty()) return;

    for (int l = 0; l < dataStr.front().size(); l++)
    {
        const string keyStr = (dataStr.front()[l].empty()) ? ("field"s + to_string(l)) : dataStr.front()[l];

        keyTabl[keyStr] = l;
        keyVect.emplace_back(keyStr);
    }

    dataStr.pop_front();
};



int MyCSVDataStrReader::getPos(const string& posName) const
{
    auto it = keyTabl.find(posName);

    if (it == keyTabl.end()) return -1;

    return it->second;
};

string MyCSVDataStrReader::tochka2zpt(const string& str1)
{
    const string strTochka("."s), strZPT(","s);

    int pos1 = 0;

    string str2 = str1;

    for (;;)
    {
        const int cnt = str2.find(strTochka, pos1);
        if (cnt == string::npos) break;

        str2.replace(cnt, strTochka.size(), strZPT);
        pos1 = cnt + strZPT.size();
        if (pos1 >= str2.size()) break;
    }

    return str2;
};

void MyCSVDataStrReader::str2str(string& str1, const string& from1, const string& to1)
{

    for (int pos1 = 0; pos1 < str1.size();)
    {
        const int cnt = str1.find(from1, pos1);
        if (cnt == string::npos) break;

        str1.replace(cnt, from1.size(), to1);
        pos1 = cnt + to1.size();
    }

};

string MyCSVDataStrReader::str2strrc(const string& str1, const string& from1, const string& to1)
{
    string result = str1;

    str2str(result, from1, to1);

    return result;
};

vector<string> MyCSVDataStrReader::csvStrDecompose(string& workStr, const string& strRAZD)
{
    const string strKAV(u8"\""s);
    const string strKAV2(u8"\"\""s);
    const string strKAVRAZD = strKAV + strRAZD;
    const string strRAZDKAV = strRAZD + strKAV;

    vector<string> fullStr;

    while (!workStr.empty()) // Каждое выполнение цикла - одна позиция таблицы (одно поле)
    {
            string smallStr; // Здесь строится одно поле таблицы

            bool inKAV = false;
            bool firstPosW = true;

            for (;;)
            {

                const int posRAZD = workStr.find(strRAZD);
                const int posKAV = workStr.find(strKAV);
                const int posKAV2 = workStr.find(strKAV2);
                const int posRAZDKAV = workStr.find(strRAZDKAV);
                const int posKAVRAZD = workStr.find(strKAVRAZD);

                if (firstPosW)
                {
                    firstPosW = false;
                    if (posKAV == 0) // Должно стоять вначале
                    {
                        inKAV = true;
                        workStr = string(workStr.begin() + 1, workStr.end());

                    }
                    else inKAV = false;

                    continue;
                }

                else
                {
                    bool b = !inKAV && posKAV == string::npos && posRAZD == string::npos;
                    if (b)
                    {
                        smallStr = workStr;
                        workStr.clear();
                        break;
                    };

                    b = !inKAV && posKAV == string::npos && posRAZD != string::npos;
                    if (b)
                    {
                        smallStr = string(workStr.begin(), workStr.begin() + posRAZD);
                        workStr = string(workStr.begin() + posRAZD + strRAZD.size(), workStr.end());
                        break;
                    };

                    b = !inKAV && posKAV != string::npos && posRAZD != string::npos && (posKAV > posRAZD);
                    if (b)
                    {
                        smallStr = string(workStr.begin(), workStr.begin() + posRAZD);
                        workStr = string(workStr.begin() + posRAZD + strRAZD.size(), workStr.end());
                        break;
                    };

                    b = !inKAV && posKAV2 != string::npos && posRAZD == string::npos;
                    if (b)
                    {
                        smallStr = string(workStr.begin(), workStr.begin() + posKAV2) + strKAV;
                        workStr = string(workStr.begin() + posKAV2 + strKAV2.size(), workStr.end());
                        continue;
                    };

                    b = !inKAV && posKAV2 != string::npos && posRAZD != string::npos && (posKAV2 < posRAZD);
                    if (b)
                    {
                        smallStr = string(workStr.begin(), workStr.begin() + posKAV2) + strKAV;
                        workStr = string(workStr.begin() + posKAV2 + strKAV2.size(), workStr.end());
                        continue;
                    };

                    b = inKAV && posKAV != string::npos && posKAV2 == string::npos && posKAVRAZD == string::npos;
                    if (b)
                    {
                        smallStr = string(workStr.begin(), workStr.begin() + posKAV);
                        workStr.clear();
                        inKAV = false;
                        break;
                    };

                    b = inKAV && (posKAV2 != string::npos) && ((posKAVRAZD == string::npos) || (posKAVRAZD > posKAV2));
                    if (b)
                    {
                        smallStr = string(workStr.begin(), workStr.begin() + posKAV2) + strKAV;
                        workStr = string(workStr.begin() + posKAV2 + strKAV2.size(), workStr.end());
                        continue;
                    };

                    b = inKAV && (posKAVRAZD != string::npos) && ((posKAV2 == string::npos) || (posKAVRAZD < posKAV2));
                    if (b)
                    {
                        smallStr = string(workStr.begin(), workStr.begin() + posKAVRAZD);
                        workStr = string(workStr.begin() + posKAVRAZD + strKAVRAZD.size(), workStr.end());
                        break;
                    };
                }
            };

            fullStr.emplace_back(smallStr);
    };    

    return fullStr;
};

list<vector<string>> MyCSVDataStrReader::csvFileDecompose(ifstream& ifs, const string& strRAZD)
{
    list<vector<string>> result;

    for (string workStr(""); workStr.clear(), getline(ifs, workStr);)
    {
        const vector<string> fullStr = csvStrDecompose(workStr, strRAZD);

        if (!fullStr.empty()) result.emplace_back(fullStr);
    };

    return result;
};

string filterProbel(MyTimerLogger& mtl, const string& workStr)
{
    string result;

    const string strKAV1(u8"\'"s);
    const string strKAV2(u8"\""s);
    const string strProbel(u8" "s);
    const vector<string> strPattern{ strProbel,  strKAV1,  strKAV2 };    
    const int workStrSize = workStr.size();

    int curPos = 0;
    vector<int> pos3(3, MYINTMAX);
    int inKAV = 0;
    
    while (curPos < workStrSize)
    {
        int minPos = MYINTMAX;
        int minPosPattern = -1;

        for (int l = 0; l < 3; l++)
        {
            const int pos = workStr.find(strPattern[l], curPos);
            pos3[l] = (pos == string::npos) ? MYINTMAX : pos;
            if (minPos > pos3[l])
            {
                minPos = pos3[l];
                minPosPattern = l;
            };
        };

        if (inKAV == 0 && minPosPattern >0) // Было незакавычено, встретилась кавычка
        {
            result+= workStr.substr(curPos, minPos - curPos+ strPattern[minPosPattern].size());
            inKAV = minPosPattern;
            curPos = minPos + strPattern[minPosPattern].size();
            continue;
        };

        if (inKAV > 0 && pos3[inKAV]==MYINTMAX)
        {
            mtl.setMainT();
            mtl.sstr << "\n\nfilterProbel: ERROR: NO CLOSED QUOTE";
            break;
        };

        if (inKAV > 0 && pos3[inKAV] < MYINTMAX)
        {
            result += workStr.substr(curPos, pos3[inKAV] - curPos + strPattern[inKAV].size());            
            curPos = pos3[inKAV] + strPattern[inKAV].size();
            inKAV = 0;
            continue;
        };

        if (inKAV == 0 && minPosPattern == 0) // Недалеко пробел, кавычек нет. Если закавычено, пробел игнорируем.
        {
            if (minPos > curPos) result += workStr.substr(curPos, minPos - curPos);
            curPos = minPos + strPattern[0].size();
            continue;
        };

        if (inKAV == 0 && minPosPattern == 0) // Недалеко пробел, кавычек нет. Если закавычено, пробел игнорируем.
        {
            if (minPos > curPos) result += workStr.substr(curPos, minPos - curPos);
            curPos = minPos + strPattern[0].size();
            continue;
        };

        if (inKAV == 0 && minPosPattern == -1) 
        {
            result += workStr.substr(curPos);
            curPos = workStr.size();
            break;
        };

    };

    return result;
}
