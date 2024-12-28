#include "lf_xmlproperties.h"

using namespace lf;


lf::CProperty& CProperty::operator=(const QVariant& v)
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

    return QVariant::typeName();
}

QVariant CProperty::convert(const QVariant& v) const
{
    if (//v.typeId() == QMetaType::Int ||
        v.typeId() == QMetaType::UInt ||
        v.typeId() == QMetaType::LongLong ||
        v.typeId() == QMetaType::ULongLong ||
        v.typeId() == QMetaType::Long ||
        v.typeId() == QMetaType::Short ||
        v.typeId() == QMetaType::Char ||
        v.typeId() == QMetaType::Char16 ||
        v.typeId() == QMetaType::Char32 ||
        v.typeId() == QMetaType::ULong ||
        v.typeId() == QMetaType::UShort ||
        v.typeId() == QMetaType::UChar ||
        v.typeId() == QMetaType::SChar)
        return QVariant(v.toInt());

    if (//v.typeId() == QMetaType::Double ||
        v.typeId() == QMetaType::Float)
        return QVariant(v.toDouble());

    if (v.typeId() == QMetaType::QByteArray)
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
