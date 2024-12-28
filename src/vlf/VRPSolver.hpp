#pragma once

#include "MySyst.hpp"

#include "MyTimerLogger.hpp"

#include <numbers>

#include <iostream>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <fstream>
#include <filesystem>


using namespace std;

#define MYINTMAX 2000000000

struct VRPSolver
{
    // typedef set<pair<int, int>> Perevozki; // —тартова€ точка - и набор точек получени€
    // typedef map<int, map<int, int>> Graph; // ¬ершина и набор исход€щих ребер (вершина-вход€ща€, вес ребра)
    
    struct Reys
    {
        list<int> path;

        map<int, set<int>> perev;

        double P;

        int fromToTransitSign; 
        // 0 если мимо транзитных терминалов
        // 1 если забирает с транзитных терминалов
        // 2 если грузит на транзитные терминалы

        Reys()
            :path(), perev(), P(0), fromToTransitSign(0)
        {};
    };

    struct TransitMag
    {
        int from, to;
        int numP, numM;
        double costP, costM, dCostP, dCostM;

        TransitMag(int from_=-1, int to_=-1)
            :from(from_), to(to_), costP(0), costM(0), dCostP(0), dCostM(0), numP(0), numM(0)
        {};
    };
    
    list<TransitMag> transitMagList;
    list<list<int>> transitMagPathList;

    // Ёти величины надо выставить вручную перед использованием. »наче не будет работать.
    int maxPointsInReys, minPointsInReys;
    // maxPointsInReys - максимальное число терминалов в рейсе.  онечный совпадающий с начальным считаетс€ за один
    // minPointsInReys - минимальное число терминалов в рейсе, чтобы получить разрешение замкнуть маршрут.  онечный совпадающий с начальным считаетс€ за один
    map<int, string> nomer2code;
    map<string, int> code2nomer;
    set<int> allTransitTerminal;  // Ќомера транзитных терминалов
    set<int> currTransitTerminal;  // Ќомера транзитных терминалов
    int transitShift;
    map<int, set<int>> graph; // “опологи€ графа, то есть дл€ каждой вершины множество вершин, куда из нее исход€т ребра графа и вес каждого ребра
    map<pair<int, int>, double> distMap;

    // ¬от до этого момента.

    VRPSolver(MyTimerLogger& mtl, int maxPointsInReys_=6, int minPointsInReys_=3);
        // maxPointsInReys - максимальное число терминалов в рейсе.  онечный совпадающий с начальным считаетс€ за один
        // minPointsInReys - минимальное число терминалов в рейсе, чтобы получить разрешение замкнуть маршрут.  онечный совпадающий с начальным считаетс€ за один
        
    
    list<VRPSolver::Reys> makeReys(MyTimerLogger& mtl, map<int, set<int>>& allPerev, bool needLogged=false) const;
   
        // maxPointsInReys - максимальное число терминалов в рейсе.  онечный совпадающий с начальным считаетс€ за один
        // minPointsInReys - минимальное число терминалов в рейсе, чтобы получить разрешение замкнуть маршрут.  онечный совпадающий с начальным считаетс€ за один
        // cntMax - сколько перевозок подр€д рассчитываем при выборе самого дешевого начала рейса
        // currAllActualPerev3 - дл€ каких перевозок создаем рейсы.
        // из данных класса функци€ использует только логгеры

    
    int minCostStartPoint(MyTimerLogger& mtl, const map<int, set<int>>& allPerev, bool logged=false) const;

    
           
    Reys makeOneReys(MyTimerLogger& mtl, int startPoint, const map<int, set<int>>& allPerev, bool needLogged=false) const;
        

   
    double distd(int v1, int v2) const;

    inline double distd(pair<int, int> v12) const
    {
        return distd(v12.first, v12.second);
    };

    void reysPathOutResult(MyTimerLogger& mtl, double addedCost, const string& fname1, const list<VRPSolver::Reys> allReys) const;
        
    list<VRPSolver::Reys> toReys(MyTimerLogger& mtl, const list<list<int>>& paths) const;

    double calcReysCostd(const list<int>& path) const;
    
    void point2perev(int v1, const map<int, set<int>>& allPerev, map<int, set<int>>& currNeed_kuda2otkuda, bool putCurGruz, bool putTransitGruz,
        bool getCurGruz, bool getTransitGruz, map<int, set<int>>& okPerev, int& fromToTransitSign) const;
    

    bool povorotReys(VRPSolver::Reys& reys, const map<int, set<int>>& allPerev) const;

    pair<bool, double> restructReys(MyTimerLogger& mtl, list<VRPSolver::Reys>& allReys) const;
        
    pair<map<int, set<int>>, set<int>> 
        useTransit(MyTimerLogger& mtl, int numMags, const map<int, set<int>>& allPerev, list<VRPSolver::TransitMag>& transitMags, vector<vector<int>>& scheme, int transitShift) const;

    bool isTransit(int x) const;
};
