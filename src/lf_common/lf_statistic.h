#pragma once

#include "lf_types.h"
#include "lf_xml.h"

#include <map>
#include <QVariant>
#include <QCoreApplication>

namespace lf
{
    class CMean {
    public:
        void add(double x);
        double result() const { return m_num > 0 ? m_summ / m_num : 0; }
        int num() const { return m_num; }

    protected:
        double m_summ = 0;
        int m_num = 0;
    };

    typedef std::map<QString, QVariant> CStat;
    typedef std::shared_ptr<CStat> pStat;

    typedef std::map<TTime, pStat> CSimStat;
    typedef std::shared_ptr<CSimStat> pSimStat;
    typedef std::vector<double> CHeatMap;
    typedef std::shared_ptr<CHeatMap> pHeatMap;
    typedef std::map<QString, pStat> CExtStat;
    typedef std::map<QString, pHeatMap> CHeatMaps;
    typedef std::shared_ptr<CHeatMaps> pHeatMaps;

    class CStatXml : public CXml {
    public:
        static bool save(const QString& fname, const CExtStat& ext, const CSimStat& s);
        static std::vector<lf::CXml::XmlElement*> elements(const CExtStat& ext);
        static std::vector<lf::CXml::XmlElement*> elements(const CSimStat& ext);

        static CSimStat loadSimStat(const QString& fname);
        static CExtStat loadExtStat(const QString& fname);
        static pStat loadStat(XmlElement* parentElem);
        static pStat loadStat(const QString& fname, const QString& tagName);
    };
}
