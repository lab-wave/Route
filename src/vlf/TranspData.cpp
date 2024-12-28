#include "TranspData.hpp"

#include "MyCSVDataStrReader.hpp"

TranspData::Coord TranspData::Coord::longlat2Coord(double longitude1, double latitude1)
{
    const double pi = 3.1415926536;
    const double R = 6371000;
    const double long1 = longitude1 * pi / 180.0;
    const double lat1 = latitude1 * pi / 180.0;

    const double x = R * sin(lat1); // Ось направлена из центра Земли на север. Широта болььше нуля, если северная и меньше, если южная
    const double y = R * cos(lat1) * cos(long1); // Ось направлена из центра Земли на точку нулевой широты и нулевой долготы
    const double z = R * sin(lat1) * sin(long1); // Ось, образующая правую тройку. Восточная долгота положительна, западная - отрицательная

    return Coord(x, y, z);
};

void TranspData::Terminal::readTranspTerminalData(ifstream& ifsGeoCoord, ifstream& ifsTermCode, map<string, TranspData::Terminal>& name2term)
{

    // Читаю геопозиции терминалов

    readTranspTerminalGeoData(ifsGeoCoord, name2term);
    
    // Читаем коды терминалов   

    readTranspTerminalCode(ifsTermCode, name2term);

};

void TranspData::Terminal::readTranspTerminalGeoData(ifstream& ifsGeoCoord, map<string, TranspData::Terminal>& name2term)
{

    // Читаю геопозиции терминалов

    {
        const string strYes = u8"да"s;

        MyCSVDataStrReader geoPos(ifsGeoCoord, ","s);

        // , city, addr, Транзит, lat, lon, full_addr

        const int field0Pos = geoPos.getPos("field0");
        const int cityPos = geoPos.getPos("city");
        const int addrPos = geoPos.getPos("addr");
        const int transitPos = geoPos.getPos(u8"Транзит");
        const int latPos = geoPos.getPos("lat");
        const int lonPos = geoPos.getPos("lon");
        const int full_addrPos = geoPos.getPos("full_addr");



        for (const auto& ellGeoPos : geoPos.dataStr)
        {
            const int geonomer = stoi(ellGeoPos[field0Pos]);
            const string name = ellGeoPos[cityPos];
            const string address = ellGeoPos[addrPos];
            const bool isTransit = (ellGeoPos[transitPos] == strYes);
            const double latitude = stod(geoPos.tochka2zpt(ellGeoPos[latPos]));
            const double longitude = stod(geoPos.tochka2zpt(ellGeoPos[lonPos]));
            const string fullAddress = ellGeoPos[full_addrPos];

            name2term[name].geonomer = geonomer;
            name2term[name].name = name;
            name2term[name].address = address;
            name2term[name].isTransit = isTransit;
            name2term[name].latitude = latitude;
            name2term[name].longitude = longitude;
            name2term[name].fullAddress = fullAddress;
            name2term[name].coord = Coord::longlat2Coord(longitude, latitude);
            name2term[name].isApplyGeoCoord = true;
        }
    }

};

void TranspData::Terminal::readTranspTerminalCode(ifstream& ifsTermCode, map<string, TranspData::Terminal>& name2term)
{
    // Читаем коды терминалов   

    {
        const string strYes = u8"да"s;

        MyCSVDataStrReader termCode(ifsTermCode, ";"s);

        // Код терминала;Название терминала;Адрес;Транзит

        const int codePos = termCode.getPos(u8"Код терминала");
        const int namePos = termCode.getPos(u8"Название терминала");
        const int addressPos = termCode.getPos(u8"Адрес");
        const int transitPos = termCode.getPos(u8"Транзит");


        for (const auto& elltermCode : termCode.dataStr)
        {
            const string code = elltermCode[codePos];
            const string name = elltermCode[namePos];
            const string address = elltermCode[addressPos];
            const bool isTransit = (elltermCode[transitPos] == strYes);

            name2term[name].code = code;
            name2term[name].name = name;
            name2term[name].address = address;
            name2term[name].isTransit = isTransit;
            name2term[name].isApplyCode = true;
        }

    }
}

void TranspData::perevCity2term(const list<TranspData::Perevozka>& allPerev1, map<string, TranspData::Terminal>& name2term)
{
    struct CityCodeCity
    {
        string cityCode, city;

        CityCodeCity(const string& cityCode_ = ""s, const string& city_ = ""s)
            :cityCode(cityCode_), city(city_)
        {};
    };

    map<string, CityCodeCity> code2CityCodeCity;

    for (const TranspData::Perevozka& ellPerev : allPerev1)        
    {
        if (code2CityCodeCity.count(ellPerev.fromCode) == 0 && !ellPerev.fromCityCode.empty() && !ellPerev.fromCity.empty())
            code2CityCodeCity[ellPerev.fromCode] = CityCodeCity(ellPerev.fromCityCode, ellPerev.fromCity);

        if (code2CityCodeCity.count(ellPerev.toCode) == 0 && !ellPerev.toCityCode.empty() && !ellPerev.toCity.empty())
            code2CityCodeCity[ellPerev.toCode] = CityCodeCity(ellPerev.toCityCode, ellPerev.toCity);
    }

    for (auto& ellTerm : name2term)
    {
        auto it = code2CityCodeCity.find(ellTerm.second.code);
        if (it == code2CityCodeCity.end()) continue;

        const CityCodeCity& c = it->second;

        ellTerm.second.cityCode = c.cityCode;
        ellTerm.second.city = c.city;       
    }
}

void TranspData::Terminal::printTranspTerminalData(ofstream& ofs, const map<string, TranspData::Terminal>& name2term)
{
    // Распечатаем данные терминалов:

    for (const pair<string, Terminal>& ell : name2term)
    {
        ofs << "\n" << ell.first << ": ";
        ofs << " geonomer=" << ell.second.geonomer;
        ofs << " code=" << ell.second.code;
        ofs << " isTransit=" << ((ell.second.isTransit) ? "true"s : "false"s);
        ofs << " name=" << ell.second.name;
        ofs << " cityCode=" << ell.second.cityCode;
        ofs << " city=" << ell.second.city;
        ofs << " address=" << ell.second.address;
        ofs << " longitude=" << ell.second.longitude;
        ofs << " latitude=" << ell.second.latitude;
        ofs << " coordX=" << ell.second.coord.x;
        ofs << " coordY=" << ell.second.coord.y;
        ofs << " coordZ=" << ell.second.coord.z;
        ofs << " isGeoData=" << ((ell.second.isApplyGeoCoord) ? "true"s : "false"s);
        ofs << " isTermCode=" << ((ell.second.isApplyCode) ? "true"s : "false"s);
    };            
}

void TranspData::Terminal::printTranspTerminalNameOnly(ofstream& ofs, const map<string, TranspData::Terminal>& name2term)
{
    // Распечатаем данные терминалов:

    int nameMaxLen = 0, codeMaxLen = 0, cityMaxLen = 0, cityCodeMaxLen = 0, fullAddrMaxLen = 0;

    for (const pair<string, Terminal>& ell : name2term)
    {
        const int nameLen = MyTimerLogger::utf8len(ell.first);
        const int codeLen = MyTimerLogger::utf8len(ell.second.code);
        const int cityLen = MyTimerLogger::utf8len(ell.second.city);
        const int cityCodeLen = MyTimerLogger::utf8len(ell.second.cityCode);
        const int fullAddrLen = MyTimerLogger::utf8len(ell.second.fullAddress);

        if (nameMaxLen < nameLen) nameMaxLen = nameLen;
        if (codeMaxLen < codeLen) codeMaxLen = codeLen;
        if (cityMaxLen < cityLen) cityMaxLen = cityLen;
        if (cityCodeMaxLen < cityCodeLen) cityCodeMaxLen = cityCodeLen;
        if (fullAddrMaxLen < fullAddrLen) fullAddrMaxLen = fullAddrLen;

    };

    for (const pair<string, Terminal>& ell : name2term)
    {
        const int nameLen = MyTimerLogger::utf8len(ell.first);
        const int codeLen = MyTimerLogger::utf8len(ell.second.code);
        const int cityLen = MyTimerLogger::utf8len(ell.second.city);
        const int cityCodeLen = MyTimerLogger::utf8len(ell.second.cityCode);
        const int fullAddrLen = MyTimerLogger::utf8len(ell.second.fullAddress);

        ofs << "\n";

        if (ell.second.isApplyGeoCoord) ofs << "Geo=\"True\" ";
        else ofs << "Geo=\"False\"";

        ofs << "   "s;
        ofs << "name=\"" << ell.first << "\"" << setw(nameMaxLen - nameLen + 3) << " ";
        ofs << "code=\"" << ell.second.code << "\"" << setw(codeMaxLen - codeLen + 3) << " ";
        ofs << "cityCode=\"" << ell.second.cityCode << "\"" << setw(cityCodeMaxLen - cityCodeLen + 3) << " ";
        ofs << "city=\"" << ell.second.city << "\"" << setw(cityMaxLen - cityLen + 3) << " ";
        ofs << "fullAddr=\"" << ell.second.fullAddress << "\"";

    };

};

tuple<map<int, string>, map<string, int>, set<int>, map<int, set<int>>, map<pair<int, int>, double>>
     TranspData::Terminal::makeTermTransGraphGeoDist(map<string, TranspData::Terminal>& name2term)
{
    // Выдает как результат в том же порядке
    map<int, string> nomer2code;
    map<string, int> code2nomer;
    set<int> transitTerminals;
    map<int, set<int>> graph;
    map<pair<int, int>, double> dist;
    // Описание результата закончено

    map<int, Coord> nomer2coord;

    set<int> uniqGeonomer;
    set<string> uniqCode;

    for (map<string, Terminal>::const_iterator it=name2term.cbegin(); it != name2term.cend(); )
    {
        if (it->second.isApplyCode && it->second.isApplyGeoCoord && uniqGeonomer.count(it->second.geonomer)==0 && uniqCode.count(it->second.code) == 0)
        {
            nomer2coord[it->second.geonomer] = it->second.coord;
            nomer2code[it->second.geonomer] = it->second.code;
            code2nomer[it->second.code] = it->second.geonomer;
            uniqGeonomer.emplace(it->second.geonomer);
            uniqCode.emplace(it->second.code);
            if (it->second.isTransit) transitTerminals.emplace(it->second.geonomer);

            it++;
        }
        else
        {
            it = name2term.erase(it);
        };
    };

    for (const pair<int, Coord>& ell1 : nomer2coord)
        for (const pair<int, Coord>& ell2 : nomer2coord)
        {
            pair<int, int> p(ell1.first, ell2.first);

            if (ell1.first == ell2.first) dist[p] = 0;
            else
            {
                dist[p] = ell1.second.rm(ell2.second);
                graph[ell1.first].emplace(ell2.first);
            }
        };

    return make_tuple(nomer2code, code2nomer, transitTerminals, graph, dist);
};

list<TranspData::Perevozka> TranspData::Perevozka::readPerevozka(MyTimerLogger& mtl, ifstream& ifs)
{
    list<TranspData::Perevozka> result;

    MyCSVDataStrReader perev(ifs, ";"s);

    // ТоргДокумент;Завод отправитель;ЗаводПолучатель;"Дата ""Груз принят""";ГородИЗ;ГородВ;Вес нетто;Единица веса;Объем нетто;Единица объема;НаимГородИЗ;НаимГородВ

    for (const auto& ellPerev : perev.dataStr)
    {
        const string torgDoc = ellPerev[0];
        const string fromCode = ellPerev[1];
        const string toCode = ellPerev[2];
        const string dateStr = ellPerev[3];
        const string fromCityCode = ellPerev[4];
        const string toCityCode = ellPerev[5];
        const double m = stod(ellPerev[6]);
        const string mUnit = ellPerev[7];
        const double V = stod(ellPerev[8]);
        const string VUnit = ellPerev[9];
        const string fromCity = ellPerev[10];
        const string toCity = ellPerev[11];

        if (fromCode == toCode)
        {
            /*
            mtl.clear();
            mtl.sstr << "\n\nERROR readPerevozka: fromCode==toCode==" << fromCode << "\n\n";
            mtl.sstr2logOnly();
            */
        }
        else
        {
            result.emplace_back();

            result.back().torgDoc = torgDoc;
            result.back().fromCode = fromCode;
            result.back().toCode = toCode;
            result.back().dateStr = dateStr;
            result.back().fromCityCode = fromCityCode;
            result.back().toCityCode = toCityCode;
            result.back().mV.m = m;
            result.back().mUnit = mUnit;
            result.back().mV.V = V;
            result.back().VUnit = VUnit;
            result.back().fromCity = fromCity;
            result.back().toCity = toCity;
        };
    };


    return result;
};

list<TranspData::Reys> TranspData::Reys::readReys(ifstream& ifs)
{
    list<TranspData::Reys> result;

    list<TranspData::ReysEtap> reysEtapList;

    MyCSVDataStrReader reysEtaps(ifs, ";"s);

    // № транспортир.; ОчередЭтапa; АктНачалоТрансп; АктОкончТранспр; Завод отправки; ГородИЗ; НаимГородИЗ; МестоНазн:завод; ГородВ; НаимГородВ; Вес; Единица веса; Объем; Единица объема

    for (const auto& ellReysEtap : reysEtaps.dataStr)
    {
        const string transpDoc = ellReysEtap[0];
        const int etap = stoi(ellReysEtap[1]);
        
        const string fromCode = ellReysEtap[4];
        const string toCode = ellReysEtap[7];
        
        reysEtapList.emplace_back(transpDoc, etap, fromCode, toCode);        
    };

    reysEtapList.sort();

    string curTranspDoc(""s);

    for (const TranspData::ReysEtap& ell : reysEtapList)
    {
        if (curTranspDoc != ell.transpDoc)
        {
            curTranspDoc = ell.transpDoc;
            result.emplace_back(ell.transpDoc);
            result.back().codes.emplace_back(ell.fromCode);
        };

        result.back().codes.emplace_back(ell.toCode);
    };

    for (TranspData::Reys& ell : result)
    {
        if (ell.codes.front() != ell.codes.back()) ell.codes.emplace_back(ell.codes.front());        
    };

    return result;
};

/*






list<list<int>> TranspTerminal2::readReys(MySyst::MyUTF8& m8, const string& fname, const vector<TranspTerminal2>& terminals)
{   

    map<string, int> code2num;

    for (const TranspTerminal2& ell : terminals) code2num[ell.cod] = ell.geonomer;
    
    const string strTZAP(u8";"s);

    struct OnePerev
    {
        string transpName, etapName, fromName, toName;

        int nreys, netap, from, to;

        OnePerev()
            :transpName(""s), etapName(""s), fromName(""s), toName(""s)
            , nreys(0), netap(0), from(0), to(0)
        {};

        bool operator<(const OnePerev& m1) const
        {
            return (tie(nreys, netap, from, to) < tie(m1.nreys, m1.netap, m1.from, m1.to));
        };        

    };
    
    list<OnePerev> oneperevlist;
    
    // Читаю 

    {
        ifstream fileTerminalStages(fname);

        m8(fileTerminalStages);

        string workStr;

        getline(fileTerminalStages, workStr);
        workStr.clear();

        while (getline(fileTerminalStages, workStr))
        {
            vector<string> fullStr;

            while (!workStr.empty())
            {
                const int posTZAP = workStr.find(strTZAP);                

                if (posTZAP == string::npos)
                {
                    fullStr.emplace_back(workStr);
                    workStr.clear();
                }

                else 
                {
                    fullStr.emplace_back(workStr.begin(), workStr.begin() + posTZAP);
                    workStr = string(workStr.begin() + posTZAP + strTZAP.size(), workStr.end());
                }                
            }

            if (fullStr.size() < 8) continue;

            oneperevlist.emplace_back();

            OnePerev& oneper = oneperevlist.back();

            oneper.transpName = fullStr[0];
            oneper.etapName = fullStr[1];
            oneper.fromName = fullStr[4];
            oneper.toName = fullStr[7];            

        }

        fileTerminalStages.close();
    }

    // Прочел.

    // Сделаем таблицу рейсов

    set<string> reysNames;

    for (const OnePerev& ell : oneperevlist)
        reysNames.emplace(ell.transpName);

    map<string, int> reysName2NumMap;
    vector<string> reysNum2NameVect;

    for (const string& ell : reysNames)
    {
        const int l = reysNum2NameVect.size();

        reysName2NumMap[ell] = l;
        reysNum2NameVect.emplace_back(ell);
    };

    for (OnePerev& ell : oneperevlist)
    {
        ell.nreys = reysName2NumMap.find(ell.transpName)->second;
        ell.netap = stoi(ell.etapName);
        ell.from = ell.to = -1;

        auto itfrom = code2num.find(ell.fromName);
        auto itto = code2num.find(ell.toName);

        if (itfrom != code2num.end()) ell.from = itfrom->second;
        if (itto != code2num.end()) ell.to = itto->second;
    };
    
    oneperevlist.sort();

    list<list<int>> result;

    int nPrevReys = -20;

    for (const OnePerev& ell : oneperevlist)
    {
        if (ell.nreys != nPrevReys)
        {
            nPrevReys = ell.nreys;
            result.emplace_back();
            result.back().emplace_back(ell.from);
            result.back().emplace_back(ell.to);
        }
        else
        {
            result.back().emplace_back(ell.to);
        }
    };

    return result;

}
*/