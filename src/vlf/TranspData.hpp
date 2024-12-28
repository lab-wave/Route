#pragma once

#include "MassaObjem.hpp"

#include "MyTimerLogger.hpp"

struct TranspData
{    
    struct Perevozka
    {
        string torgDoc, fromCode, toCode, fromCityCode, toCityCode, fromCity, toCity, dateStr, mUnit, VUnit;
        
        MassaObjem mV;

        Perevozka()
            :torgDoc(""s), fromCode(""s), toCode(""s), fromCityCode(""s), toCityCode(""s), fromCity(""s), toCity(""s)
            ,dateStr(""s), mV(), mUnit(""s), VUnit(""s)
        {};

        static list<Perevozka> readPerevozka(MyTimerLogger& mtl, ifstream& ifs);
    };

    struct ReysEtap
    {
        string transpDoc, fromCode, toCode;
        int etap;

        ReysEtap(string transpDoc_, int etap_, string fromCode_, string toCode_)
            :transpDoc(transpDoc_), etap(etap_), fromCode(fromCode_), toCode(toCode_)
        {};

        bool operator<(const ReysEtap& m1) const
        {
            return (tie(transpDoc, etap, fromCode, toCode) < tie(m1.transpDoc, m1.etap, m1.fromCode, m1.toCode));
        }
    };

    struct Reys
    {
        string transpDoc;

        list<string> codes;        

        Reys(string transpDoc_)
            :transpDoc(transpDoc_), codes()
        {};

        static list<TranspData::Reys> readReys(ifstream& ifs);
    };

    struct Coord
    {
        double x, y, z;

        Coord()
            :x(0)
            , y(0)
            , z(0)
        {};

        Coord(double x_, double y_, double z_)
            :x(x_)
            , y(y_)
            , z(z_)
        {};

        const double re(const Coord& c1) const
        {
            const double dx = x - c1.x, dy = y - c1.y, dz = z - c1.z;
            return sqrt(dx * dx + dy * dy + dz * dz);
        };

        const double rm(const Coord& c1) const
        {
            return abs(x - c1.x) + abs(y - c1.y) + abs(z - c1.z);
        };

        static Coord longlat2Coord(double longitude1, double latitude1);
    };

    struct Terminal
    {
        int geonomer;
        string code;
        string cityCode;
        string city;
        bool isTransit;
        string name;
        string address, fullAddress;
        double longitude, latitude;
        Coord coord;
        bool isApplyCode;
        bool isApplyGeoCoord;

        Terminal()
            :geonomer(-1), code(""), cityCode(""), city(""), isTransit(false), name(""), address(""), fullAddress(""), longitude(0), latitude(0), coord(0, 0, 0)
            ,isApplyCode(false),isApplyGeoCoord(false)
        {};

        static void readTranspTerminalGeoData(ifstream& ifsGeoCoord, map<string, TranspData::Terminal>& name2term);

        static void readTranspTerminalCode(ifstream& ifsTermCode, map<string, TranspData::Terminal>& name2term);

        static void readTranspTerminalData(ifstream& ifsGeoCoord, ifstream& ifsTermCode, map<string, TranspData::Terminal>& name2term);

        static void printTranspTerminalData(ofstream& ofs, const map<string, TranspData::Terminal>& name2term);
        static void printTranspTerminalNameOnly(ofstream& ofs, const map<string, TranspData::Terminal>& name2term);

        static  tuple<map<int, string>, map<string, int>, set<int>, map<int, set<int>>, map<pair<int, int>, double>>
            makeTermTransGraphGeoDist(map<string, TranspData::Terminal>& name2term);
    };

    static void perevCity2term(const list<TranspData::Perevozka>& allPerev1, map<string, TranspData::Terminal>& name2term);

};

