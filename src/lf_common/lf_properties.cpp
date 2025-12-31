#include "lf_xmlproperties.h"

Q_DECLARE_METATYPE(lf::CClassName);

using namespace lf;

QDataStream& operator>>(QDataStream& s, CClassName& p) {
    QByteArray a;
    s >> a;
    p = CClassName(a);
    return s;
}

QDataStream& operator<<(QDataStream& s, const lf::CClassName& p) {
    return (s << p.toByteArray());
}

bool CProperty::isIntType(int tp) {
    return
        tp == QMetaType::Int ||
        tp == QMetaType::UInt ||
        tp == QMetaType::LongLong ||
        tp == QMetaType::ULongLong ||
        tp == QMetaType::Long ||
        tp == QMetaType::Short ||
        tp == QMetaType::Char ||
        tp == QMetaType::Char16 ||
        tp == QMetaType::Char32 ||
        tp == QMetaType::ULong ||
        tp == QMetaType::UShort ||
        tp == QMetaType::UChar ||
        tp == QMetaType::SChar;
}

bool CProperty::isDoubleType(int tp)
{
    return
        tp == QMetaType::Double ||
        tp == QMetaType::Float;
}

bool CProperty::isStringType(int tp)
{
    return
        tp == QMetaType::QString ||
        tp == QMetaType::QByteArray;
}

bool CProperty::isClassType(int tp)
{
    return tp == QMetaTypeId<CClassName>::qt_metatype_id();
}

CProperty& CProperty::operator=(const QVariant& v)
{
    if (typeId() != QMetaType::UnknownType)
        QVariant::operator=(convertToBase(v));
    else
        QVariant::operator=(convert(v));
    return *this;
}

const char* CProperty::typeName() const
{
    if (typeId() == QMetaType::QColor)
        return "color";
    if (typeId() == QMetaType::QString)
        return "string";
    if (typeId() == QMetaType::QFont)
        return "font";
    if (typeId() == QMetaTypeId<CClassName>::qt_metatype_id())
        return "class";

    return QVariant::typeName();
}

QVariant CProperty::convert(const QVariant& v) const
{
    if (v.typeId() != QMetaType::Int && isIntType(v.typeId()))
        return QVariant(v.toInt());

    if (v.typeId() != QMetaType::Double && isDoubleType(v.typeId()))
        return QVariant(v.toDouble());

    if (v.typeId() != QMetaType::QString && isStringType(v.typeId()))
        return QVariant(v.toString());

    return v;
}

QVariant CProperty::convertToBase(const QVariant& v) const
{
    if (v.typeId() == typeId())
        return v;

    if (typeId() == QMetaType::Int)
        return QVariant(v.toInt());

    if (typeId() == QMetaType::Double)
        return QVariant(v.toDouble());

    if (typeId() == QMetaType::QColor)
        return QVariant(QColor(v.toString()));

    if (typeId() == QMetaType::QString)
        return QVariant(v.toString());

    if (typeId() == QMetaTypeId<CClassName>::qt_metatype_id())
        return QVariant::fromValue(CClassName(v.toByteArray()));

    return v;
}

QVariant CProperty::value(const QVariant& src)
{
    QVariant base = convertToBase(src);
    if (!base.isValid())
        return defaultValue();

    if (m_constraint)
        return m_constraint->value(base, defaultValue());
    return base;
}

CIntegerConstrains::CIntegerConstrains(TBaseInt minV /*= std::numeric_limits<TBaseInt>::min()*/, TBaseInt maxV /*= std::numeric_limits<TBaseInt>::max()*/)
    : minValue(minV)
    , maxValue(maxV)
{
    if (maxValue < minValue)
        std::swap(maxValue, minValue);
}

CIntegerConstrains::CIntegerConstrains(IConstrains* ptr /*= nullptr*/)
{
    CIntegerConstrains* dptr = dynamic_cast<CIntegerConstrains*>(ptr);
    if (dptr)
    {
        minValue = dptr->minValue;
        maxValue = dptr->maxValue;
        if (maxValue < minValue)
            std::swap(maxValue, minValue);
    }
}

QVariant CIntegerConstrains::value(const QVariant& src, const QVariant& def)
{
    if (!src.isValid())
        return def;
    int i = src.toInt();
    return std::clamp(i, minValue, maxValue);
}

bool CIntegerConstrains::isValid(const QVariant& src) const {
    if (!src.isValid())
        return false;

    if (!CProperty::isIntType(src.typeId()))
        return false;

    if (src.toInt() < minValue || src.toInt() > maxValue)
        return false;

    return true;
}

CDoubleConstrains::CDoubleConstrains(TBaseDouble minV, TBaseDouble maxV, int decs /*= 4*/)
{
    minValue = minV;
    maxValue = maxV;
    decPoints = decs;
    if (maxValue < minValue)
        std::swap(maxValue, minValue);
}

CDoubleConstrains::CDoubleConstrains(IConstrains* ptr /*= NULL*/)
{
    CDoubleConstrains* dptr = dynamic_cast<CDoubleConstrains*>(ptr);
    if (dptr)
    {
        minValue = dptr->minValue;
        maxValue = dptr->maxValue;
        decPoints = dptr->decPoints;
        if (maxValue < minValue)
            std::swap(maxValue, minValue);
    }
}

QVariant CDoubleConstrains::value(const QVariant& src, const QVariant& def)
{
    if (!src.isValid())
        return def;
    TBaseDouble i = src.toDouble();
    return std::clamp(i, minValue, maxValue);
}

bool CDoubleConstrains::isValid(const QVariant& src) const
{
    if (!src.isValid())
        return false;

    if (!CProperty::isDoubleType(src.typeId()))
        return false;

    if (src.toDouble() < minValue || src.toDouble() > maxValue)
        return false;

    return true;
}

void CConstrainsList::append(const QString& name, const QByteArray& id)
{
    IConstrainsListBase::m_names << name;
    m_ids << id;
}

QVariant CConstrainsList::value(const QVariant& src, const QVariant& def)
{
    if (!src.isValid())
        return def;
    int i = index(src);
    if (i < 0)
        return def;
    return src;
}

bool CConstrainsList::isValid(const QVariant& src) const
{
    if (!src.isValid())
        return false;

    if (!m_ids.contains(src.toByteArray()))
        return false;

    return true;
}

void CConstrainsEnum::append(const QString& name, int id)
{
    IConstrainsListBase::m_names << name;
    m_ids << id;
}

QVariant CConstrainsEnum::value(const QVariant& src, const QVariant& def)
{
    if (!src.isValid())
        return def;
    int i = index(src);
    if (i < 0)
        return def;
    return src;
}

bool CConstrainsEnum::isValid(const QVariant& src) const
{
    if (!src.isValid())
        return false;

    if (!CProperty::isIntType(src.typeId()))
        return false;

    if (!m_ids.contains(src.toInt()))
        return false;

    return true;
}

