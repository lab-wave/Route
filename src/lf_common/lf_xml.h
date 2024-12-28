#pragma once

#include "lf_types.h"
#include "tinyxml.h"

#include <vector>
#include <string>
#include <ranges>
#include <QString>
#include <QVariant>

namespace lf
{
    bool isArithmetic(QVariant v);
    bool isString(QVariant v);

    struct XmlElementAdvance {
        typedef TiXmlElement XmlElement;
        const XmlElement* operator()(const XmlElement* p) const { return p->NextSiblingElement(); }
    };

    typedef lf::fwd_view<const TiXmlElement*, XmlElementAdvance> CXmlView;

    class CXml {
    public:
        typedef TiXmlElement XmlElement;
        typedef std::shared_ptr<XmlElement> pXmlElement;
        typedef std::vector<pXmlElement> XmlElements;

        static XmlElement* wrap(const std::vector<XmlElement*>& elements, const std::string& rootName);
        static bool save(const QString& fname, XmlElement* root);
        static bool save(const std::string& fname, XmlElement* root);
        static bool load(const QString& fname, pXmlElement& root);
        static bool load(const std::string& fname, pXmlElement& root);
    };
}
