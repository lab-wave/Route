#include <QTime>

#include "lf_statistic.h"
#include "tinyxml.h"
#include "lf_types.h"

using namespace lf;

void CMean::add(double x)
{
    m_summ += x;
    m_num += 1;
}

std::vector<lf::CXml::XmlElement*> CStatXml::elements(const CExtStat& ext)
{
    std::vector<XmlElement*> el;

    for (auto& e : ext) {
        TiXmlElement* e_stat = new TiXmlElement(e.first.toStdString());
        el.push_back(e_stat);

        for (auto& stat : *e.second) {
            TiXmlElement* stat_elem = new TiXmlElement("data");
            stat_elem->SetAttribute("name", stat.first.toStdString());
            stat_elem->SetAttribute("value", stat.second.toString().toStdString());
            e_stat->LinkEndChild(stat_elem);
        }
    }

    return el;
}

std::vector<lf::CXml::XmlElement*> CStatXml::elements(const CSimStat& s)
{
    std::vector<XmlElement*> el;

    for (auto& ts : s) {
        TiXmlElement* time_stat = new TiXmlElement("time_stat");
        time_stat->SetDoubleAttribute("time", ts.first);
        el.push_back(time_stat);

        for (auto& stat : *ts.second) {
            TiXmlElement* stat_elem = new TiXmlElement("data");
            stat_elem->SetAttribute("name", stat.first.toStdString());
            stat_elem->SetAttribute("value", stat.second.toString().toStdString());
            time_stat->LinkEndChild(stat_elem);
        }
    }
    return el;
}

bool CStatXml::save(const QString& fname, const CExtStat& ext, const CSimStat& s)
{
    std::vector<XmlElement*> e;

    lf::append(e, elements(ext));
    lf::append(e, elements(s));

    return CXml::save(fname, wrap(e, "stat"));
}

pStat CStatXml::loadStat(TiXmlElement* parentElem)
{
    pStat res = std::make_shared<CStat>();
    TiXmlElement* dataElem = parentElem->FirstChildElement("data");
    while (dataElem) {
        double val = 0;
        if (dataElem->QueryDoubleAttribute("value", &val) == TIXML_SUCCESS) {
            res->insert({ QString(dataElem->Attribute("name")), val });
        }
        dataElem = dataElem->NextSiblingElement("data");
    }
    return res;
}

pStat CStatXml::loadStat(const QString& fname, const QString& tagName)
{
    pStat res;

    TiXmlDocument doc(fname.toStdString().c_str());
    if (!doc.LoadFile()) {
        return res;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {
        return res;
    }

    TiXmlElement* timeElem = root->FirstChildElement(tagName.toStdString().c_str());
    res = loadStat(timeElem);
    return res;
}

CSimStat CStatXml::loadSimStat(const QString& fname)
{
    CSimStat res;

    TiXmlDocument doc(fname.toStdString().c_str());
    if (!doc.LoadFile()) {
        return res;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {
        return res;
    }

    TiXmlElement* timeElem = root->FirstChildElement("time_stat");
    while (timeElem) {
        res[static_cast<TTime>(std::stod(timeElem->Attribute("time")))] = loadStat(timeElem);
        timeElem = timeElem->NextSiblingElement("time_stat");
    }
    return res;
}

CExtStat CStatXml::loadExtStat(const QString& fname)
{
    CExtStat res;

    TiXmlDocument doc(fname.toStdString().c_str());
    if (!doc.LoadFile()) {
        return res;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {
        return res;
    }

    TiXmlElement* keyElem = root->FirstChildElement();
    while (keyElem) {
        res[keyElem->Value()] = loadStat(keyElem);
        keyElem = keyElem->NextSiblingElement();
    }
    return res;
}
