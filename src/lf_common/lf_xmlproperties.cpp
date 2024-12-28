#include "lf_xmlproperties.h"

#include <ranges>
#include <string_view>

#include <QFont>

using namespace lf;

std::vector<std::string> CXmlProperty::s_flags_names{ "none", "virtual", "fixed", "readonly", "typebased", "mapped"};

lf::CXml::XmlElement* CXmlProperty::saveProperty(const CProperty& p)
{
    CXml::XmlElement* e = new CXml::XmlElement("property");

    e->SetAttribute("id", p.m_id.toStdString());
    e->SetAttribute("name", p.m_name.toStdString());
    e->SetAttribute("type", p.typeName());
    e->SetAttribute("default", p.defaultValue().toString().toStdString());
    e->SetAttribute("flags", saveFlags(p.m_flags));

    auto epc = saveConstraint(p.m_constraint);
    if (epc)
        e->LinkEndChild(epc);

    return e;
}

lf::CXml::XmlElement* CXmlProperty::saveConstraint(const pConstrains& pc)
{
    if (!pc)
        return nullptr;

    if (auto ipc = std::dynamic_pointer_cast<CIntegerConstrains>(pc)) {
        auto res = new XmlElement("constraint");
        res->SetAttribute("type", "int");
        res->SetAttribute("min", ipc->minValue);
        res->SetAttribute("max", ipc->maxValue);
        return res;
    }
    else if (auto dpc = std::dynamic_pointer_cast<CDoubleConstrains>(pc)) {
        auto res = new XmlElement("constraint");
        res->SetAttribute("type", "double");
        res->SetAttribute("min", dpc->minValue);
        res->SetAttribute("max", dpc->maxValue);
        return res;
    }
    else if (auto lpc = std::dynamic_pointer_cast<CConstrainsList>(pc)) {
        auto res = new XmlElement("constraint");
        res->SetAttribute("type", "list");
        for (auto i : std::ranges::iota_view(0, lpc->names().size())) {
            auto elem = new XmlElement("element");
            elem->SetAttribute("id", lpc->ids()[i]);
            elem->SetAttribute("name", lpc->names()[i].toStdString());
            res->LinkEndChild(elem);
        }
        return res;
    }
    else if (auto epc = std::dynamic_pointer_cast<CConstrainsEnum>(pc)) {
        auto res = new XmlElement("constraint");
        res->SetAttribute("type", "enum");
        for (auto i : std::ranges::iota_view(0, epc->names().size())) {
            auto elem = new XmlElement("element");
            elem->SetAttribute("id", epc->ids()[i]);
            elem->SetAttribute("name", epc->names()[i].toStdString());
            res->LinkEndChild(elem);
        }
        return res;
    }

    return nullptr;
}

std::string CXmlProperty::saveFlags(uint32_t f)
{
    std::string res;

    Q_ASSERT(s_flags_names.size() < 32);

    if (f == 0)
        return s_flags_names[0];

    for (uint32_t i : std::ranges::views::iota(1u, s_flags_names.size())) {
        if (f & (1 << (i - 1))) {
            res += (res.empty() ? "" : "|") + s_flags_names[i];
        }
    }

    return res;
}

lf::CProperty CXmlProperty::loadProperty(const CXml::XmlElement* e)
{
    std::string id, name, type, def, flags;
    QVariant mt;
    uint32_t f;

    if (e->QueryStringAttribute("id", &id) != TIXML_SUCCESS ||
        e->QueryStringAttribute("name", &name) != TIXML_SUCCESS || 
        e->QueryStringAttribute("type", &type) != TIXML_SUCCESS ||
        e->QueryStringAttribute("default", &def) != TIXML_SUCCESS || !loadDefaultAndType(type, def, mt) ||
        e->QueryStringAttribute("flags", &flags) != TIXML_SUCCESS || !loadFlags(flags, f)) {
        return CProperty();
    }

    CProperty res;
    res.m_id = QByteArray::fromStdString(id);
    res.m_name = QString::fromStdString(name);
    res.m_flags = f;
    res.defaultValueAndType(mt);

    auto c = e->FirstChildElement("constraint");
    if (c) {
        res.m_constraint = loadConstraint(c);
    }

    return res;
}

lf::pConstrainsListBase CXmlProperty::makeConstrintBase(const std::string& type)
{
    using namespace std::literals;
    if (std::ranges::equal(type, "enum"sv)) {
        return  std::make_shared<CConstrainsEnum>();
    }
    else if (std::ranges::equal(type, "list"sv)) {
        return std::make_shared<CConstrainsList>();
    }

    return pConstrainsListBase();
}

lf::pConstrains CXmlProperty::loadConstraint(const XmlElement* elem)
{
    using namespace std::literals;
    std::string type;
    if (elem->QueryStringAttribute("type", &type) != TIXML_SUCCESS) {
        return pConstrains();
    }

    if (std::ranges::equal(type, "int"sv)) {
        int _min, _max;
        if (elem->QueryIntAttribute("min", &_min) != TIXML_SUCCESS ||
            elem->QueryIntAttribute("max", &_max) != TIXML_SUCCESS) {
            return pConstrains();
        }
        return std::make_shared<CIntegerConstrains>(_min, _max);
    }
    else if (std::ranges::equal(type, "double"sv)) {
        double _min, _max;
        int _decs = 0;
        if (elem->QueryDoubleAttribute("min", &_min) != TIXML_SUCCESS ||
            elem->QueryDoubleAttribute("max", &_max) != TIXML_SUCCESS) {
            return pConstrains();
        }
        elem->QueryIntAttribute("decs", &_decs);
        return std::make_shared<CDoubleConstrains>(_min, _max, _decs);
    }
    else if (std::ranges::equal(type, "enum"sv) || std::ranges::equal(type, "list"sv)) {
        pConstrainsListBase res = makeConstrintBase(type);

        auto filter = [](const XmlElement* e) -> bool { return e->ValueStr().compare("element") == 0; };

//        static_assert(std::input_iterator < SimpleInputIterator>);

        static_assert(std::indirectly_readable<lf::fwd_iterator<const XmlElement*, XmlElementAdvance>>);
        static_assert(std::input_iterator<lf::fwd_iterator<const XmlElement*, XmlElementAdvance>>);
        static_assert(std::input_or_output_iterator<lf::fwd_iterator<const XmlElement*, XmlElementAdvance>>);

        static_assert(std::indirectly_readable<lf::fwd_iterator<XmlElement*, XmlElementAdvance>>);
        static_assert(std::input_iterator<lf::fwd_iterator<std::shared_ptr<XmlElement>, XmlElementAdvance>>);
        static_assert(std::input_or_output_iterator<lf::fwd_iterator<XmlElement*, XmlElementAdvance>>);

        auto v = lf::CXmlView(elem->FirstChildElement(), nullptr) | std::views::filter(filter);

        for (auto& c : v) {
            loadConstraintElement(c, res);
        }
        if (res->empty())
            return pConstrains();

        return res;
    }

    return pConstrains();
}

bool CXmlProperty::loadConstraintElement(const XmlElement* e, lf::pConstrainsListBase constr)
{
    if (!constr)
        return false;

    std::string name, icon;
    if (e->QueryStringAttribute("name", &name) != TIXML_SUCCESS) {
        return false;
    }

    if (auto lst = std::dynamic_pointer_cast<CConstrainsList>(constr)) {
        std::string id;
        if (e->QueryStringAttribute("id", &id) != TIXML_SUCCESS) {
            return false;
        }
        lst->append(QString::fromStdString(name), QByteArray::fromStdString(id));
        return true;
    }
    else if (auto enm = std::dynamic_pointer_cast<CConstrainsEnum>(constr)) {
        int id;
        if (e->QueryIntAttribute("id", &id) != TIXML_SUCCESS) {
            return false;
        }
        enm->append(QString::fromStdString(name), id);
        return true;
    }

    return false;
}

bool CXmlProperty::loadFlags(const std::string& s, uint32_t& res)
{
    res = 0;
    for (const auto word : std::views::split(s, '|')) {
        if(word.empty())
            continue;

        auto iw = std::ranges::find_if(s_flags_names, [&word](const std::string& s) {
                return std::ranges::equal(word, s);
            });
        if (iw == s_flags_names.end())
            return false;

        uint32_t subres = std::distance(s_flags_names.begin(), iw);
        if (subres > 0) {
            res |= (1 << (subres - 1));
        }
    }
    return true;
}

bool CXmlProperty::loadDefaultAndType(const std::string& t, const std::string& d, QVariant& tp)
{
    bool ok = true;
    if (t.compare("double") == 0) {
        double tmp = QString::fromStdString(d).toDouble(&ok);
        if (ok) {
            tp = QVariant(tmp);
            return true;
        }
    }
    else if(t.compare("int") == 0)
    {
        int tmp = QString::fromStdString(d).toInt(&ok);
        if (ok) {
            tp = QVariant(tmp);
            return true;
        }
    }
    else if (t.compare("bool") == 0)
    {
        if (d.compare("true") == 0) {
            tp = QVariant(true);
            return true;
        }
        else if (d.compare("false") == 0) {
            tp = QVariant(false);
            return true;
        }
    }
    else if (t.compare("color") == 0) {
        QColor tmp = QColor::fromString(QString::fromStdString(d));
        if (tmp.isValid()) {
            tp = tmp;
            return true;
        }
    }
    else if (t.compare("font") == 0) {
        QFont tmp;
        if (tmp.fromString(QString::fromStdString(d))) {
            tp = tmp;
            return true;
        }
    }
    else if (t.compare("string") == 0) {
        tp = QString::fromStdString(d);
        return true;
    }

    return false;
}
