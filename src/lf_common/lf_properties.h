#pragma once

#include "lf_types.h"

#include <QString>
#include <QVariant>
#include <QColor>
#include <QMap>
#include <QIcon>

namespace lf
{

    // attribute constrains
    class IConstrains
    {
    public:
        virtual ~IConstrains() {}
        virtual QVariant value(const QVariant& src, const QVariant& def) = 0;
    };

    typedef std::shared_ptr<IConstrains> pConstrains;


    class CProperty : protected QVariant {
    public:
        enum AttrFlags : uint32_t
        {
            ATTR_NONE = 0,
            ATTR_VIRTUAL = 1,       // read only, not to be stored & read
            ATTR_FIXED = 2,         // non-user defined
            ATTR_READONLY = 4,      // read only
            ATTR_TYPE_BASED = 8,
            ATTR_MAPPED = 16,       // internal: mapped to some system value, i.e. coordinate or size
        };

        CProperty() : QVariant(), m_flags(ATTR_NONE) {}
        explicit CProperty(const QVariant& val, const QByteArray& _id = QByteArray(), const QString& _name = QString(), const uint32_t _flags = ATTR_NONE)
            : QVariant(convert(val)), m_id(_id), m_name(_name.isEmpty() ? _id : _name), m_flags(_flags) {}
        explicit CProperty(QMetaType type, const QByteArray& _id = QByteArray(), const QString& _name = QString(), const uint32_t _flags = ATTR_NONE)
            : QVariant(convert(QVariant(type))), m_id(_id), m_name(_name.isEmpty() ? _id : _name), m_flags(_flags) {}
        virtual ~CProperty() {}

        CProperty& operator=(const QVariant& v);

        //operator QVariant() const { return *this; }

        int typeId() const { return QVariant::typeId(); }
        const char* typeName() const;
        inline bool isValid() const { return QVariant::isValid(); }
        bool isNull() const { return QVariant::isNull(); }

        QVariant convert(const QVariant& v) const;
        QVariant convertToBase(const QVariant& v) const;

        const QVariant& defaultValue() const { return *this; }
        void defaultValue(const QVariant& val) { *this = convertToBase(val); }
        void defaultValueAndType(const QVariant& val) { *this = convert(val); }

        virtual QVariant value(const QVariant& src);

        QByteArray m_id;
        QString m_name;
        uint32_t m_flags;
        pConstrains m_constraint;
    };

    class CPropertyMap : public std::unordered_map<QByteArray, QVariant> {
    public:
        typedef QByteArray TKey;
        typedef QVariant TValue;

        template<class T>
        T cast(const TValue& v, bool& ok) const { ok = false; return T(); }

        template<>
        double cast(const TValue& v, bool& ok) const { return v.toDouble(&ok); }

        template<>
        int cast(const TValue& v, bool& ok) const { return v.toInt(&ok); }

        template<>
        unsigned int cast(const TValue& v, bool& ok) const { return v.toUInt(&ok); }

        template<>
        QString cast(const TValue& v, bool& ok) const { ok = true; return v.toString(); }

        template<>
        std::string cast(const TValue& v, bool& ok) const { ok = true; return v.toString().toStdString(); }

        template<>
        QByteArray cast(const TValue& v, bool& ok) const { ok = true; return v.toByteArray(); }

        template<>
        QStringList cast(const TValue& v, bool& ok) const { ok = true; return v.toStringList(); }

        template<class T>
        bool queryValue(const TKey& name, T& val, pErrors err = pErrors()) const {
            bool ok = false;
            TValue v = lf::value(*this, name, TValue());
            if (v.isValid()) {
                auto cv = cast<T>(v, ok);
                if (ok) {
                    val = cv;
                }
                else {
                    err->push_back(CError{ CError::E_ERROR, QString("Attribute \"%1\" cast error \"%2\"").arg(name).arg(v.toString()) });
                }
            }
            else {
                err->push_back(CError{ CError::E_ERROR, QString("Attribute \"%1\" not found").arg(name) });
            }

            return ok;
        }

        template<class T>
        T value(const TKey& name, const T& defVal = T()) const {
            bool ok = false;
            TValue v = lf::value(*this, name, TValue());
            if (v.isValid()) {
                auto cv = cast<T>(v, ok);
                if (ok) {
                    return cv;
                }
            }

            return defVal;
        }

        void remove(const TKey& name) {
            auto it = find(name);
            if (it != end())
                erase(it);
        }
    };

    class CIntegerConstrains : public IConstrains
    {
    public:
        typedef int TBaseInt;

        CIntegerConstrains(TBaseInt minV = std::numeric_limits<TBaseInt>::min(), TBaseInt maxV = std::numeric_limits<TBaseInt>::max());
        CIntegerConstrains(IConstrains* ptr = nullptr);

        QVariant value(const QVariant& src, const QVariant& def) override;

        TBaseInt minValue = std::numeric_limits<TBaseInt>::min();
        TBaseInt maxValue = std::numeric_limits<TBaseInt>::max();
    };

    class CDoubleConstrains : public IConstrains
    {
    public:
        typedef double TBaseDouble;

        CDoubleConstrains(TBaseDouble minV, TBaseDouble maxV, int decs = 4);
        CDoubleConstrains(IConstrains* ptr = NULL);

        QVariant value(const QVariant& src, const QVariant& def) override;

        TBaseDouble minValue = std::numeric_limits<TBaseDouble>::lowest();
        TBaseDouble maxValue = std::numeric_limits<TBaseDouble>::max();
        int decPoints = 4;
    };

    class IConstrainsListBase : public IConstrains
    {
    public:
        virtual std::size_t size() const { return m_names.size(); }
        virtual bool empty() const { return m_names.empty(); }
        virtual int index(const QVariant& val) const = 0;
        virtual const QStringList& names() const { return m_names; }
    protected:
        QStringList m_names;
    };

    typedef std::shared_ptr<IConstrainsListBase> pConstrainsListBase;

    class CConstrainsList : virtual public IConstrainsListBase
    {
    public:
        int index(const QVariant& val) const { return m_ids.indexOf(val.toByteArray()); }
        const QByteArrayList& ids() const { return m_ids; }
        void append(const QString& name, const QByteArray& id);
        QVariant value(const QVariant& src, const QVariant& def) override;
    protected:
        QByteArrayList m_ids;
    };

    class CConstrainsEnum : virtual public IConstrainsListBase
    {
    public:
        int index(const QVariant& val) const { return m_ids.indexOf(val.toInt()); }
        const QList<int>& ids() const { return m_ids; }
        void append(const QString& name, int id);
        QVariant value(const QVariant& src, const QVariant& def) override;
    protected:
        QList<int> m_ids;
    };
}
