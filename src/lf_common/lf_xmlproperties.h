#pragma once

#include "lf_types.h"
#include "tinyxml.h"
#include "lf_xml.h"
#include "lf_properties.h"

#include <map>
#include <QString>

namespace lf
{
    class CXmlProperty : public CXml {
    public:
        using CXml::XmlElements;
        static std::vector<std::string> s_flags_names;


        virtual XmlElement* saveProperty(const CProperty& p);
        virtual XmlElement* saveConstraint(const pConstrains& pc);
        virtual std::string saveFlags(uint32_t f);

        virtual lf::pConstrainsListBase makeConstrintBase(const std::string& type);
        virtual CProperty loadProperty(const XmlElement* e);
        virtual pConstrains loadConstraint(const XmlElement* e);
        virtual bool loadConstraintElement(const XmlElement* e, lf::pConstrainsListBase constr);
        virtual bool loadFlags(const std::string& s, uint32_t& res);
        virtual bool loadDefaultAndType(const std::string& t, const std::string& d, QVariant& tp);
    };
}
