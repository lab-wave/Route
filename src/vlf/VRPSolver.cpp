#include "VRPSolver.hpp"

VRPSolver::VRPSolver(MyTimerLogger& mtl, int maxPointsInReys_, int minPointsInReys_)
    :maxPointsInReys(maxPointsInReys_), minPointsInReys(minPointsInReys_),transitShift(0)
   
{   
    
    // Сообщение в лог
    {
        mtl.setMainT();
        mtl.sstr << "\nStart of class \"VPSolver\""s;
        mtl.sstr2log();
    }
};

double VRPSolver::distd(int v1, int v2) const
{
    v1 %= transitShift;
    v2 = v2 % transitShift;

    if (v1 == v2) return 0.0;

    pair<int, int> p(v1, v2);   

    auto it = distMap.find(p);

    if (it == distMap.end()) return 10e10;

    return it->second;
}; 

double VRPSolver::calcReysCostd(const list<int>& path) const
{
    double rc = 0;

    if (path.size() < 2) return 0;

    int prev = path.front();

    for (auto it = next(path.begin(), 1); it != path.end(); it++)
    {
        const int cur = *it;

        pair<int, int> p(prev% transitShift, cur% transitShift);

        auto it2 = distMap.find(p);

        if (it2 == distMap.end())
        {
            rc = 10e10;
            break;
        };

        rc += (it2->second);
        prev = cur;
    };

    return rc;
};

void VRPSolver::reysPathOutResult(MyTimerLogger& mtl,  double addedCost, const string& fname1, const list<VRPSolver::Reys> allReys) const
{
    ofstream logfile2(fname1);

    mtl.clear();

    mtl.sstr << "\n\n\nREYS:\n\n";
    mtl.sstr2logOnly(logfile2);
    mtl.clear();

    int cntReys = 0;
    double Psumm = 0;

    for (const auto& ell : allReys)
    {
        cntReys++;

        const double P = calcReysCostd(ell.path);

        string strTrans("NO_TRANSIT");

        if(ell.fromToTransitSign==1) strTrans="FROM_TRANSIT"s;
        if (ell.fromToTransitSign == 2) strTrans = "TO_TRANSIT"s;

        for (const auto& ell2 : transitMagPathList)
        {
            if (ell2 == ell.path)
            {
                strTrans = "MAGISTRAL"s;
                break;
            }
        };

        mtl.sstr << "\nReys_" << cntReys << "(" << strTrans <<",P = " << P;

        for (const auto& ellFrom : ell.perev)
            for(int ellTo : ellFrom.second)
        {
            mtl.sstr << ", " << nomer2code.find(ellFrom.first % transitShift)->second << "->" << nomer2code.find(ellTo % transitShift)->second;
        };

        mtl.sstr << ")=";

        Psumm += P;

        bool isfirst = true;

        for (int ell2 : ell.path)
        {
            if (isfirst) isfirst = false;
            else mtl.sstr << "->";

            mtl.sstr << nomer2code.find(ell2 % transitShift)->second;
        };

        mtl.sstr2logOnly(logfile2);
        mtl.clear();
    }

    mtl.sstr << "\n\nPsumm=" << Psumm << " addedCostTransit=" << addedCost << " FullSum=" << Psumm + addedCost << "\n";
    mtl.sstr << "\nNum of reys: " << cntReys << "\n\n";

    mtl.sstr2log(logfile2);
    mtl.clear();

    logfile2.close();
}

list<VRPSolver::Reys> VRPSolver::toReys(MyTimerLogger& mtl, const list<list<int>>& paths) const
{
    list<VRPSolver::Reys> result;

    int numReysOk = 0, numReysNotOk = 0;

    for (const auto& ell1 : paths)
    {
        bool isOk = true;

        for (int ell2 : ell1)
        {
            if (ell2 < 0)
            {
                isOk = false;
                break;
            }
        }

        if (isOk)
        {
            numReysOk++;

            result.emplace_back();

            for (int ell2 : ell1)
                result.back().path.emplace_back(ell2);
        }
        else
        {
            numReysNotOk++;
        };
    };

    mtl.clear();
    mtl.sstr << "\n\n\ntoReys: Apriori Reys Ok=" << numReysOk << " not Ok=" << numReysNotOk << "\n\n";
    mtl.sstr2logOnly();

    return result;
};

bool VRPSolver::isTransit(int x) const
{
    return (allTransitTerminal.count(x) > 0);    
};

pair<map<int, set<int>>, set<int>> VRPSolver::useTransit(MyTimerLogger& mtl, int numMags, const map<int, set<int>>& allPerev, 
    list<VRPSolver::TransitMag>& transitMags, vector<vector<int>>& scheme, int transitShift) const
{
    {
        mtl.setMainT();
        mtl.sstr << "\nuseTransit: Start";
        mtl.sstr2log();
    }

    
    scheme.clear();

    for (auto& ell : transitMags)
    {
        ell.dCostM = ell.dCostP = 0;
        ell.numM = ell.numP = 0;
    };

    list<pair<int, int>> perev;

    for (const auto& ell1 : allPerev)
        for (const int ell2 : ell1.second)
            perev.emplace_back(ell1.first, ell2);

    if (perev.size() < 2) return make_pair(map<int, set<int>>(), set<int>());

    // Строим транзитные магистрали

    vector<int> transPoints; // Транзитные терминалы

    for (int ell : allTransitTerminal) transPoints.emplace_back(ell);

    for (int ellW = 0; ellW < numMags; ellW++) // Цикл создает по одной транзитной магистрали
    {
        // Создадим очередную транзитную магистраль

        double optimalDCostAllPerev = 0;

        {
            transitMags.emplace_back();

            TransitMag optimalNewTransit;
            
            for (int lTranFrom = 0, supTranFrom = transPoints.size() - 1; lTranFrom < supTranFrom; lTranFrom++)
                for (int lTranTo = lTranFrom + 1, supTranTo = transPoints.size(); lTranTo < supTranTo; lTranTo++)
                {
                    transitMags.back() = TransitMag(lTranFrom, lTranTo);
                    transitMags.back().costP = distd(transitMags.back().from, transitMags.back().to);
                    transitMags.back().costM = distd(transitMags.back().to, transitMags.back().from);

                    double dCostAllPerev = 0;

                    for (const auto& ellTM : transitMags) dCostAllPerev += (ellTM.costP + ellTM.costM);
                                                           
                    for (const pair<int, int>& ellPerev : perev)
                    {
                        const double costOrig = distd(ellPerev);

                        double dcostAllTransits = 0;
                        
                        for (const auto& ellAllTransit : transitMags)
                        {                            
                            const double costTuda = distd(ellPerev.first, ellAllTransit.from) + distd(ellAllTransit.to, ellPerev.second);
                            const double costObratno = distd(ellPerev.first, ellAllTransit.to) + distd(ellAllTransit.from, ellPerev.second);

                            
                            if (dcostAllTransits > costTuda - costOrig)
                            {
                                dcostAllTransits = costTuda - costOrig;
                            }

                            if (dcostAllTransits > costObratno - costOrig)
                            {
                                dcostAllTransits = costObratno - costOrig;
                            }
                        }

                        dCostAllPerev += dcostAllTransits;
                    }

                    if (optimalDCostAllPerev > dCostAllPerev)
                    {
                        optimalDCostAllPerev = dCostAllPerev;
                        optimalNewTransit = transitMags.back();
                    };
                };

            transitMags.back() = optimalNewTransit;
        };

        if(transitMags.back().from > -1)
        {
            // Сообщение в лог
            {
                 mtl.setMainT();
                 mtl.sstr << "\nuseTransit: trans magistrals=" << transitMags.size() << " new newAllCost-origAllCost=" << optimalDCostAllPerev
                        << " newMagistral=(" << transitMags.back().from << ", " << transitMags.back().to
                        << ")  +P=" << distd(transitMags.back().from, transitMags.back().to)
                        << "  -P=" << distd(transitMags.back().from, transitMags.back().to);
                    mtl.sstr2log();
                }
            }
            else
            {
                transitMags.pop_back();
                // Сообщение в лог
                {
                    mtl.setMainT();
                    mtl.sstr << "\nuseTransit: cnt=" << ellW+1 << "  trans magistrals = " << transitMags.size() << " not make new transit";
                    mtl.sstr2log();
                }
                break;
            }
    };

    if (transitMags.empty()) map<int, set<int>>();

    list<pair<int, int>> newPerev;

    
    for (const auto& ell : perev)
    {
        const double origCost = distd(ell);
        double mainCost = distd(ell);
        pair<int, int> mainTransit(-1, -1);
        
        for (const auto& ellTrans : transitMags)
        {
                const double cost1 = distd(ell.first, ellTrans.from) + distd(ellTrans.to, ell.second);
                const double cost2 = distd(ell.first, ellTrans.to) + distd(ellTrans.from, ell.second);

                if (mainCost > cost1)
                {
                    mainCost = cost1;
                    mainTransit = make_pair(ellTrans.from, ellTrans.to);
                };

                if (mainCost > cost2)
                {
                    mainCost = cost2;
                    mainTransit = make_pair(ellTrans.to, ellTrans.from);
                };
        };
       

        if (mainTransit.first == -1)
        {
            newPerev.emplace_back(ell);

            mtl.clear();
            mtl.sstr << "\nNO TRANSIT: " << ell.first << "->" << ell.second;
            mtl.sstr2logOnly();

            vector<int> transitSchemeVect = { ell.first, ell.second };
            scheme.emplace_back(transitSchemeVect);
        }
        else
        {
            if(ell.first != mainTransit.first)   newPerev.emplace_back(ell.first, mainTransit.first+transitShift);
            if(mainTransit.second != ell.second)   newPerev.emplace_back(mainTransit.second+transitShift, ell.second);

            mtl.clear();
            mtl.sstr << "\nTRANSIT: FROM " << ell.first << "->"  << ell.second <<
                
             "  TO  "  << ell.first << "->" << mainTransit.first << "->" << mainTransit.second << "->" << ell.second;
            mtl.sstr2logOnly();

            vector<int> transitSchemeVect = { ell.first, mainTransit.first, mainTransit.second, ell.second };
            scheme.emplace_back(transitSchemeVect);

            for (auto& ellTransit : transitMags)
            {
                if (mainTransit.first == ellTransit.from && mainTransit.second == ellTransit.to)
                {
                    ellTransit.dCostP += (mainCost - origCost);
                    ellTransit.numP++;
                    break;
                }

                if (mainTransit.first == ellTransit.to && mainTransit.second == ellTransit.from)
                {
                    ellTransit.dCostM += (mainCost - origCost);
                    ellTransit.numM++;
                    break;
                }
            }
        }
    };

    mtl.clear();
    mtl.sstr << "\n\n";
    mtl.sstr2logOnly();

    map<int, set<int>> newAllPerev;

    for (const auto& ell : newPerev) newAllPerev[ell.first].emplace(ell.second);

    set<int> currTransitTerm;
    
    mtl.clear();
    int cntTransit = 1;
    for (const auto& ell : transitMags)
    {
        mtl.sstr << "\nuseTransit: transit magistral " << cntTransit << " from " << ell.from << " to " << ell.to
            << " costP=" << ell.costP << " costM=" << ell.costM
            << " dcostP=" << ell.dCostP << " dcostM=" << ell.dCostM;

        currTransitTerm.emplace(ell.from);
        currTransitTerm.emplace(ell.to);

        cntTransit++;
    }

    mtl.sstr2logOnly();

    mtl.setMainT();
    mtl.sstr << "\n\nuseTransit: finished. Transit magistral num=" << transitMags.size();
    mtl.sstr2log();

    return make_pair(newAllPerev, currTransitTerm);
};

