#include "TranspData.hpp"

#include "VRPSolver.hpp" 

struct ATISUdata
{
    map<pair<string, string>, double> fromTo2Cost;
    double coeff0, coeff1;

    ATISUdata()
        :fromTo2Cost(), coeff0(0), coeff1(0)
    {};
};

ATISUdata readFromTo2CostCoeff0Coeff1(ifstream& ifs)
{
    ATISUdata result;

    const string strFrom("fromCity=\""s);
    const string strTo("toCity=\""s);
    const string strCost("costATISU(RUR)=\"");
    const string strCost2("costATISU2(RUR)=\"");
    const string strKAV("\""s);
    const string strCoeff0D("coeff0D=\""s);
    const string strCoeff1D("coeff1D=\""s);


    for (string workStr(""); workStr.clear(), getline(ifs, workStr);)
    {
        const int posFrom = workStr.find(strFrom);
        const int posTo = workStr.find(strTo);
        const int posCost = workStr.find(strCost);
        const int posCost2 = workStr.find(strCost2);
        const int posCoeff0D = workStr.find(strCoeff0D);
        const int posCoeff1D = workStr.find(strCoeff1D);

        if (posFrom != string::npos && posTo != string::npos && posCost != string::npos && posCost2 != string::npos)
        {
            const int posFromKav = workStr.find(strKAV, posFrom + strFrom.size());
            const int posToKav = workStr.find(strKAV, posTo + strTo.size());
            const int posCostKav = workStr.find(strKAV, posCost + strCost.size());
            const int posCost2Kav = workStr.find(strKAV, posCost2 + strCost2.size());

            const string fromStr = workStr.substr(posFrom + strFrom.size(), posFromKav - (posFrom + strFrom.size()));
            const string toStr = workStr.substr(posTo + strTo.size(), posToKav - (posTo + strTo.size()));
            const string costStr = workStr.substr(posCost + strCost.size(), posCostKav - (posCost + strCost.size()));
            const string cost2Str = workStr.substr(posCost2 + strCost2.size(), posCost2Kav - (posCost2 + strCost2.size()));

            const double cost2 = stod(cost2Str);

            double cost = stod(costStr);

            if (cost < 0) cost = cost2;

            if (cost > 0)
            {
                const pair<string, string> p(fromStr, toStr);
                result.fromTo2Cost[p] = cost;
            }
        };

        if (posCoeff0D != string::npos)
        {
            const int posCoeff0DKav = workStr.find(strKAV, posCoeff0D + strCoeff0D.size());

            const string coeff0DStr = workStr.substr(posCoeff0D + strCoeff0D.size(), posCoeff0DKav - (posCoeff0D + strCoeff0D.size()));

            result.coeff0 = stod(coeff0DStr);
        };

        if (posCoeff1D != string::npos)
        {
            const int posCoeff1DKav = workStr.find(strKAV, posCoeff1D + strCoeff1D.size());

            const string coeff1DStr = workStr.substr(posCoeff1D + strCoeff1D.size(), posCoeff1DKav - (posCoeff1D + strCoeff1D.size()));

            result.coeff1 = stod(coeff1DStr);
        };


    };

    return result;
};

void transpData2VRPSolver(map<string, TranspData::Terminal>& name2term, VRPSolver& v1)
{
    map<int, string> nomer2code;
    map<string, int> code2nomer;
    set<int> allTransitTerminal;
    map<int, set<int>> graph;
    map<pair<int, int>, double> distMap;

    tie(nomer2code, code2nomer, allTransitTerminal, graph, distMap) = TranspData::Terminal::makeTermTransGraphGeoDist(name2term);

    v1.nomer2code = nomer2code;
    v1.code2nomer = code2nomer;
    v1.allTransitTerminal = allTransitTerminal;
    v1.graph = graph;
    v1.distMap = distMap;

    v1.transitShift = 0;

    for (const auto& ell : v1.nomer2code) if (v1.transitShift < ell.first) v1.transitShift = ell.first;

    v1.transitShift += 1000;
};

void reysPathOutResult2Excel(ofstream& ofs, VRPSolver& vrps,  map<int, TranspData::Terminal> nomer2term, const list<VRPSolver::Reys> allReys)
{

    ofs << u8"Рейс;Этап;ИзКодТерм;ИзИмяТерм;ИзКодГорода;ИзГород;ВКодТерм;ВИмяТерм;ВКодГорода;ВГород;Транзит;ЦенаЭтапа";

    int cntReys = 0;
    double Psumm = 0;

    for (const auto& ell : allReys)
    {
        double PPPsumm = 0;

        cntReys++;

        int cntEtap = 0;

        int prev = -1;

        for (int ellPoint : ell.path)
        {
            if (prev < 0)
            {
                prev = ellPoint;
                continue;
            };

            cntEtap++;

            double Petap = vrps.distd(prev, ellPoint);
            Psumm += Petap;
            PPPsumm += Petap;

            ofs << "\n";
            ofs << cntReys << ";";
            ofs << cntEtap << ";";
            ofs << "\"Code " << nomer2term[prev].code << "\"" << ";";
            ofs << "\"" << nomer2term[prev].name << "\"" << ";";
            ofs << "\"Code " << nomer2term[prev].cityCode << "\"" << ";";
            ofs << "\"" << nomer2term[prev].city << "\"" << ";";


            ofs << "\"Code " << nomer2term[ellPoint].code << "\"" << ";";
            ofs << "\"" << nomer2term[ellPoint].name << "\"" << ";";
            ofs << "\"Code " << nomer2term[ellPoint].cityCode << "\"" << ";";
            ofs << "\"" << nomer2term[ellPoint].city << "\"" << ";";

            if (ell.fromToTransitSign == 0) ofs << u8"\"Без транзита\";"s;
            if (ell.fromToTransitSign == 1) ofs << u8"\"До транзита\";"s;
            if (ell.fromToTransitSign == 2) ofs << u8"\"После транзита\";"s;

            ofs << to_string(Petap) << ";";
           

            prev = ellPoint;
        };

        ofs << u8"\n;;;;;;;;;;\"Цена рейса\";" << to_string(PPPsumm);


    };

    ofs << u8"\n;;;;;;;;;;\"Итого\";" << to_string(Psumm);
};


int main()
{
    string in_dir = "../../data/vlf_in/"s;
    string out_dir = "../../data/vlf_out/"s;


    MySyst::MyUTF8 m8;

    const string needDate = "03.08.2022"s;

    std::filesystem::create_directory(in_dir);
    std::filesystem::create_directory(out_dir);

    MyTimerLogger mtl(out_dir, "_log.txt"s);
    m8(mtl.logfile);

    map<string, TranspData::Terminal> name2term;

    // Прочитаем геоданные терминалов

    ifstream ifsGeoCoord(in_dir + "geocoding.csv");
    m8(ifsGeoCoord);
    TranspData::Terminal::readTranspTerminalGeoData(ifsGeoCoord, name2term);
    ifsGeoCoord.close();

    // Прочитаем код и админ. данные терминалов

    ifstream ifsTermCode(in_dir + "terminals.csv");
    m8(ifsTermCode);
    TranspData::Terminal::readTranspTerminalCode(ifsTermCode, name2term);
    ifsTermCode.close();   

    // Прочитаем все требуемые перевозки

    ifstream ifsPerev(in_dir + "Gruzoperevozki.csv");
    m8(ifsPerev);
    list<TranspData::Perevozka> perev=TranspData::Perevozka::readPerevozka(mtl, ifsPerev);
    ifsPerev.close();

    // Откорректируем город и код города терминалов по данным перевозок

    TranspData::perevCity2term(perev, name2term);

    // Распечатаем данные по терминалам

    ofstream ofsTerminals(out_dir + mtl.startTimeStr + "_terminals.txt");
    m8(ofsTerminals);
    TranspData::Terminal::printTranspTerminalData(ofsTerminals, name2term);
    ofsTerminals.close();

    ofstream ofsTerminalNames(out_dir + mtl.startTimeStr + "_terminalNames.txt");
    m8(ofsTerminalNames);
    TranspData::Terminal::printTranspTerminalNameOnly(ofsTerminalNames, name2term);
    ofsTerminalNames.close();

    // Отфильтруем все перевозки по дате, заодно получим номера торговых документов

    set<string> torgDocs;

    for (list<TranspData::Perevozka>::const_iterator it = perev.cbegin(); it != perev.end(); )
    {
        if (it->dateStr == needDate)
        {
            torgDocs.emplace(it->torgDoc);
            it++;
        }
        else
        {
            it = perev.erase(it);
        };
    };
        
    // Найдем транспортные документы, соответствующие торговым документам:

    int cnt = 0;    
       
    VRPSolver solver3(mtl, 6, 2);

    transpData2VRPSolver(name2term, solver3);

    map<int, TranspData::Terminal> nomer2term;

    for (const pair<string, TranspData::Terminal>& ell : name2term)
    {
        const int nomer = ell.second.geonomer;

        if (nomer >= 0)
            nomer2term[nomer] = ell.second;
    };
        
    // Переведем априорные рейсы в термины VRPSolver

    mtl.clear();

    mtl.sstr << "\n\nAPRIORI DATA OF REYS:\n";
    mtl.sstr2logOnly();
    mtl.clear();

    

    mtl.clear();
    mtl.sstr << "\n\nAPRIORI DATA OF REYS IS OK\n";
    mtl.sstr2logOnly();
    mtl.clear();

   
    map<int, set<int>> allPerev;

    map<pair<string, string>, list<TranspData::Perevozka>> fromTo2perev;

    for (const TranspData::Perevozka& ellPerev : perev)
    {
        auto itFrom = solver3.code2nomer.find(ellPerev.fromCode);
        auto itTo = solver3.code2nomer.find(ellPerev.toCode);

        if ((itFrom != solver3.code2nomer.end()) && (itTo != solver3.code2nomer.end()))
        {
            allPerev[itFrom->second].emplace(itTo->second);

            fromTo2perev[make_pair(ellPerev.fromCode, ellPerev.toCode)].emplace_back(ellPerev);

            if(itFrom->second == itTo->second)
            {
                mtl.clear();
                mtl.sstr << "\n\nERROR 198: itFrom->second == itTo->second ==" << itFrom->second << "\n\n";
                mtl.sstr2logOnly();

            }
        }
    };


    vector<vector<int>> transitScheme;
    map<pair<string, string>, pair<string, string>> transitMap;
    list<VRPSolver::TransitMag> transitMags;

    int numPerevOrig = 0;

    for (const auto& ell1 : allPerev)
        for (int ell2 : ell1.second)
            numPerevOrig++;


    auto ff = [](const VRPSolver::TransitMag& v1, const VRPSolver::TransitMag& v2)->bool
    {
        return ((v1.dCostP + v1.dCostM) < (v2.dCostP + v2.dCostM));
    };

  
    for (int ellUseTransit = 0; ellUseTransit < 4; ellUseTransit++)
    {
        mtl.setMainT();

        mtl.sstr << "\n\n\n ellUseTransit=" << ellUseTransit << "\n\n";

        mtl.sstr2log();

        solver3.useTransit(mtl, 25, allPerev, transitMags, transitScheme, solver3.transitShift);

        transitMags.sort(ff);

        while (transitMags.size() > 12)  transitMags.pop_back();
    }
    
    

    mtl.setMainT();

    mtl.sstr << "\n\n\n ellUseTransit Ok\n\n";

    mtl.sstr2log();

    // Отладка
    {


        set<int> ff;

        for (const auto& ell1 : allPerev)
            for (int ell2 : ell1.second)
                if (ell1.first == ell2)
                    ff.emplace(ell2);

        if (!ff.empty())
        {
            mtl.clear();
            mtl.sstr << "\n\nERROR usetrans0 : from=to=";

            for (int ll : ff) mtl.sstr << "  " << ll;

            mtl.sstr2logOnly();
        };
    };



   tie(allPerev, solver3.currTransitTerminal) = solver3.useTransit(mtl, 10, allPerev, transitMags, transitScheme, solver3.transitShift);

   // tie(allPerev, solver3.currTransitTerminal) = solver3.useTransit(mtl, 5, allPerev, transitMags, transitScheme, solver3.transitShift);

    // Отладка
    {


        set<int> ff;

        for (const auto& ell1 : allPerev)
            for (int ell2 : ell1.second)
                if (ell1.first == ell2)
                    ff.emplace(ell2);

        if (!ff.empty())
        {
            mtl.clear();
            mtl.sstr << "\n\nERROR usetrans1 : from=to=";

            for (int ll : ff) mtl.sstr << "  " << ll;

            mtl.sstr2logOnly();
        };
    };

    // Калькуляция объем-масса по перевозкам

    map<pair<int, int>, MassaObjem> per2MO; // Вес-объем каждой перевозки

    map<pair<int, int>, MassaObjem> transit2MO; // Вес-объем по транзитной магистрали

    for (const TranspData::Perevozka& ell : perev)
    {
        auto itFrom = solver3.code2nomer.find(ell.fromCode);
        auto itTo = solver3.code2nomer.find(ell.toCode);

        if ((itFrom != solver3.code2nomer.end()) && (itTo != solver3.code2nomer.end()))
        {
            pair<int, int> p(itFrom->second, itTo->second);
            per2MO[p] += ell.mV;
        };
    };

    // Распечатка транзитного транспорта

    double addedCost = 0;

    {
        ofstream transitOutFile(out_dir + "VRP_"s + mtl.startTimeStr + "_Transit.txt"s);

        mtl.clear();
        mtl.sstr << "\n\nTRANSIT MAGISTRALS\n";
        mtl.sstr2logOnly(transitOutFile);

        int notTransitPerev = numPerevOrig;

        for (const auto& ellTM : transitMags)
        {
            mtl.clear();
            mtl.sstr << "\nfrom=" << solver3.nomer2code[ellTM.from] << " to=" << solver3.nomer2code[ellTM.to] << " costRight=" << ellTM.costP << " costBack=" << ellTM.costM
                    << " dCostRight=" << ellTM.dCostP << " dCostBack=" << ellTM.dCostM
                    << "  forw.transp.=" << ellTM.numP << "  backw.transp.=" << ellTM.numM << " of full transp.=" << numPerevOrig;
                mtl.sstr2logOnly(transitOutFile);
                notTransitPerev -= (ellTM.numP + ellTM.numM);
        };

            mtl.clear();
            mtl.sstr << "\nNO TRANSIT transp.=" << notTransitPerev << " of full transp.=" << numPerevOrig;
            mtl.sstr2logOnly(transitOutFile);

            mtl.clear();
            mtl.sstr << "\n\nTRANSIT SCHEME\n";
            mtl.sstr2logOnly(transitOutFile);

            map<string, set<string>> schemeTransitPrint;

           

            for (const auto& ellTM : transitScheme)
            {
                if (ellTM.size() == 2)
                    schemeTransitPrint["NO TRANSIT"s].emplace(solver3.nomer2code[ellTM[0]] + "->"s + solver3.nomer2code[ellTM[1]]);
                else if (ellTM.size() == 4)
                {
                    transitMap[make_pair(solver3.nomer2code[ellTM[0]], solver3.nomer2code[ellTM[3]])] = make_pair(solver3.nomer2code[ellTM[1]], solver3.nomer2code[ellTM[2]]);

                    schemeTransitPrint["TRANSIT "s + solver3.nomer2code[ellTM[1]] + "->"s + solver3.nomer2code[ellTM[2]]].emplace(solver3.nomer2code[ellTM[0]]
                        + "->"s + solver3.nomer2code[ellTM[1]] + "->"s + solver3.nomer2code[ellTM[2]] + "->"s + solver3.nomer2code[ellTM[3]]);

                    const pair<int, int> onePer(ellTM[0], ellTM[3]);
                    const pair<int, int> perTransit(ellTM[1], ellTM[2]);

                    transit2MO[perTransit] += per2MO[onePer];
                }
            };

            for (const auto& ellTS : schemeTransitPrint)
            {
                mtl.clear();
                mtl.sstr << "\n\n" + ellTS.first + "   (" << ellTS.second.size() << " transp.):\n";
                mtl.sstr2logOnly(transitOutFile);

                for (const auto& ellTS2 : ellTS.second)
                {
                    mtl.clear();
                    mtl.sstr << "\n" + ellTS2;
                    mtl.sstr2logOnly(transitOutFile);
                };
            };

            mtl.clear();
            mtl.sstr << "\n\nTRANSIT COST, MASS, VOLUME:\n";
            mtl.sstr2logOnly(transitOutFile);

            map<pair<int, int>, int> numFur;

            for (const auto& ell : transit2MO)
            {
                MassaObjem mo1 = ell.second;

                int numf = 0;

                MassaObjem moFura(20000, 90);

                while (mo1.m > 0 && mo1.V > 0)
                {
                    numf++;
                    mo1 -= moFura;
                };

                numFur[ell.first] = numf;
            }

            map<pair<int, int>, int> numFurSimm;

            for (const auto& ell : numFur)
            {
                int numf = ell.second;

                if (numFurSimm[ell.first] < numf) numFurSimm[ell.first] = numf;
                if (numFurSimm[make_pair(ell.first.second, ell.first.first)] < numf) numFurSimm[make_pair(ell.first.second, ell.first.first)] = numf;
            }

            for (const auto& ell : transit2MO)
            {
                mtl.clear();
                mtl.sstr << "\nTRANSIT: " << solver3.nomer2code[ell.first.first] + "->"s + solver3.nomer2code[ell.first.second]
                    << " ONE_REYS_DIST=" << solver3.distd(ell.first.first, ell.first.second) << " m=" << ell.second.m << " V=" << ell.second.V;
                mtl.sstr2logOnly(transitOutFile);
            };

            mtl.clear();
            mtl.sstr << "\n\n";
            mtl.sstr2logOnly(transitOutFile);



            for (const auto& ell : numFurSimm)
            {
                
                if (ell.first.first > ell.first.second) continue;
                int addNum = 0;
                if (addNum < ell.second - 1) addNum = ell.second - 1;

                double addCost = (solver3.distd(ell.first.first, ell.first.second) + solver3.distd(ell.first.second, ell.first.first)) * double(addNum);
                addedCost += addCost;


                mtl.clear();
                mtl.sstr << "\nON TRANSIT: " << solver3.nomer2code[ell.first.first] + "->"s + solver3.nomer2code[ell.first.second] + "->"s + solver3.nomer2code[ell.first.first]
                    << " need fur=" << ell.second << " addCost=" << addCost;
                mtl.sstr2logOnly(transitOutFile);
            };

            mtl.clear();
            mtl.sstr << "\n\nON TRANSIT: addedCost on added furs=" << addedCost ;
            mtl.sstr2logOnly(transitOutFile);
            transitOutFile.close();
    };

    // Она же, но для Excel
    {
        ofstream transitOutFile(out_dir + "VRP_"s + mtl.startTimeStr + "_Transit.csv"s);
        m8(transitOutFile);

        transitOutFile << u8"Номер;ИзКодТерм;ИзИмяТерм;ИзКодГорода;ИзГород;ВКодТерм;ВИмяТерм;ВКодГорода;ВГород;ЦенаТудаИОбратно;ЭкономияНаПрямых;ЭкономияНаОбратных;ПрямыхТранзитныхПеревозок;ОбратныхТранзитныхПеревозок;ВсегоПеревозок;"s;
        transitOutFile << u8"МассаПрямо;ОбъемПрямо;МассаОбратно;ОбъемОбратно;НужноФур;ДопСтоимость";

        int cntMagistral = 0;

        double PPsum = 0;
        int NNFur = 0;


        for (const auto& ellTM : transitMags)
        {
            int nFur = 1;

            int nn = 1 + (transit2MO[make_pair(ellTM.from, ellTM.to)].m - 1) / 20000;
            if (nFur < nn) nFur = nn;

            nn = 1 + (transit2MO[make_pair(ellTM.to, ellTM.from)].m - 1) / 20000;
            if (nFur < nn) nFur = nn;

            nn = 1 + (transit2MO[make_pair(ellTM.from, ellTM.to)].V - 1) / 90;
            if (nFur < nn) nFur = nn;

            nn = 1 + (transit2MO[make_pair(ellTM.to, ellTM.from)].V - 1) / 90;
            if (nFur < nn) nFur = nn;

            cntMagistral++;

            transitOutFile << "\n" << cntMagistral << ";";

            transitOutFile << "\"Code " << nomer2term[ellTM.from].code << "\"" << ";";
            transitOutFile << "\"" << nomer2term[ellTM.from].name << "\"" << ";";
            transitOutFile << "\"Code " << nomer2term[ellTM.from].cityCode << "\"" << ";";
            transitOutFile << "\"" << nomer2term[ellTM.from].city << "\"" << ";";

            transitOutFile << "\"Code " << nomer2term[ellTM.to].code << "\"" << ";";
            transitOutFile << "\"" << nomer2term[ellTM.to].name << "\"" << ";";
            transitOutFile << "\"Code " << string(nomer2term[ellTM.to].cityCode) << "\"" << ";";
            transitOutFile << "\"" << nomer2term[ellTM.to].city << "\"" << ";";

            transitOutFile << double(ellTM.costP + ellTM.costM) << ";";
            transitOutFile << double(ellTM.dCostP) << ";";
            transitOutFile << double(ellTM.dCostM) << ";";
            transitOutFile << ellTM.numP << ";";
            transitOutFile << ellTM.numM << ";";
            transitOutFile << numPerevOrig << ";";
            transitOutFile << transit2MO[make_pair(ellTM.from, ellTM.to)].m << ";";
            transitOutFile << transit2MO[make_pair(ellTM.from, ellTM.to)].V << ";";
            transitOutFile << transit2MO[make_pair(ellTM.to, ellTM.from)].m << ";";
            transitOutFile << transit2MO[make_pair(ellTM.to, ellTM.from)].V << ";";
            transitOutFile << nFur << ";";
            transitOutFile << to_string(double(ellTM.costP + ellTM.costM) * double(nFur - 1)) << ";";

            PPsum += double(ellTM.costP + ellTM.costM) * double(nFur - 1);
            NNFur += nFur;

        };

        transitOutFile << u8"\n;;;;;;;;;;;;;;;;;;\"Итого\";" << NNFur << ";" << to_string(PPsum);

        transitOutFile.close();

        ofstream transitSchemeOutFile(out_dir + "VRP_"s + mtl.startTimeStr + "_TransitScheme.csv"s);
        m8(transitSchemeOutFile);

        transitSchemeOutFile << u8"ИзКодТерм;ИзИмяТерм;ИзКодГорода;ИзГород;ВКодТерм;ВИмяТерм;ВКодГорода;ВГород;";
        transitSchemeOutFile << u8"TpВхКодТерм;TpВхИмяТерм;TpВхКодГорода;TpВхГород;TpВыхКодТерм;TpВыхИмяТерм;TpВыхКодГорода;TpВыхГород";

        for (const auto& ell : transitScheme)
        {
            if (ell.size() != 4) continue;

            transitSchemeOutFile << "\n" ;

           
            transitSchemeOutFile << "\"Code " << nomer2term[ell[0]].code << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[0]].name << "\"" << ";";
            transitSchemeOutFile << "\"Code " << nomer2term[ell[0]].cityCode << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[0]].city << "\"" << ";";
          

            transitSchemeOutFile << "\"Code " << nomer2term[ell[3]].code << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[3]].name << "\"" << ";";
            transitSchemeOutFile << "\"Code " << nomer2term[ell[3]].cityCode << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[3]].city << "\"" << ";";

            transitSchemeOutFile << "\"Code " << nomer2term[ell[1]].code << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[1]].name << "\"" << ";";
            transitSchemeOutFile << "\"Code " << nomer2term[ell[1]].cityCode << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[1]].city << "\"" << ";";


            transitSchemeOutFile << "\"Code " << nomer2term[ell[2]].code << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[2]].name << "\"" << ";";
            transitSchemeOutFile << "\"Code " << nomer2term[ell[2]].cityCode << "\"" << ";";
            transitSchemeOutFile << "\"" << nomer2term[ell[2]].city << "\"";

        }

        transitOutFile.close();

    }

        return 0;   

};


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
