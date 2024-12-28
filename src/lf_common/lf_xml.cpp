#include "lf_xml.h"

bool lf::isArithmetic(QVariant v)
{
    return v.typeId() == QMetaType::Type::Int ||
        v.typeId() == QMetaType::Type::UInt ||
        v.typeId() == QMetaType::Type::ULongLong ||
        v.typeId() == QMetaType::Type::LongLong ||
        v.typeId() == QMetaType::Type::Double;
}

bool lf::isString(QVariant v)
{
    return v.typeId() == QMetaType::Type::QString;
}

lf::CXml::XmlElement* lf::CXml::wrap(const std::vector<XmlElement*>& elements, const std::string& rootName)
{
    XmlElement* root = new XmlElement(rootName);

    for (auto& e : elements)
        root->LinkEndChild(e);

    return root;
}

bool lf::CXml::save(const QString& fname, XmlElement* root)
{
    return save(fname.toStdString(), root);
}

bool lf::CXml::save(const std::string& fname, XmlElement* root)
{
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
    doc.LinkEndChild(decl);
    doc.LinkEndChild(root);

    return doc.SaveFile(fname);
}

bool lf::CXml::load(const QString& fname, pXmlElement& root)
{
    return load(fname.toStdString(), root);
}

bool lf::CXml::load(const std::string& fname, pXmlElement& root)
{
    TiXmlDocument doc(fname);
    if (!doc.LoadFile()) {
        return false;
    }

    XmlElement* rootE = doc.RootElement();
    if (!rootE) {
        return false;
    }

    root = pXmlElement(dynamic_cast<TiXmlElement*>(rootE->Clone()));

    return true;

}
