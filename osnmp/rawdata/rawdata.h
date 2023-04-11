#ifndef RAWDATA_H
#define RAWDATA_H

#include "osnmp.h"
#include "constructedcontent.h"
#include "primitivecontent.h"

// Нужно определить функции для своего класса, чтобы он работал с RawData
// Пример:
// bool decode(const RawData &data, QString &value);
// bool encode(const QString &value, RawData &data);

namespace osnmp {
    class RawData
    {
    public:
        RawData();
        RawData(const RawData &other);
        RawData(RawData &&other);
        RawData(const Identifier &id);
        RawData(const Identifier &id, Content *content);
        template <typename T> RawData(const T &t);

        ~RawData();

        template <typename T>
        RawData &operator = (const T &t);

        RawData &operator = (const RawData &other);
        RawData &operator = (RawData &&other);

        inline bool isEOC() const { return id_ == UniversalTagNumber::EOC; }
        inline bool isNull() const { return id_ == UniversalTagNumber::Null; }
        inline Identifier identifier() const { return id_; }
        inline bool isPrimitive() const { return id_.isPrimitive(); }
        inline bool isConstructor() const { return id_.isConstructor(); }
        inline bool isException() const;

        bool isEmpty() const;
        void clear();

        void swap(RawData &data);

        Content *content() const { return content_; }

        template<typename T> T value() const;
        template<typename T> operator T() const;

        friend class ConstructedContent;
        friend class PrimitiveContent;

        static RawData fromByteArray(const QByteArray &array, ParseError *error = nullptr);
        QByteArray toByteArray(ParseError *error = nullptr) const;

    private:
        Identifier id_;
        Content *content_;

    private:
        void setContent(Content *content);

        ParseResult decodeData(QDataStream &stream);
        ParseError encodeData(QDataStream &stream) const;
    };

    template <typename T>
    bool decode(const RawData &data, T &value);

    template <typename T>
    bool encode(const T &value, RawData &data);


    template<typename T> T raw_cast(const RawData &data) {
        T value;
        decode(data, value);    //@N isok
        return value;
    }

    template<typename T>
    T RawData::value() const
    {
        return raw_cast<T>(*this);
    }

    template<typename T>
    RawData::operator T() const
    {
        return raw_cast<T>(*this);
    }

    template<typename T>
    RawData &RawData::operator =(const T &t)
    {
        RawData d(t);
        swap(d);
        return *this;
    }

    bool RawData::isException() const
    {
        switch (static_cast<ExceptionType>(static_cast<Tag>(id_))) {
            case ExceptionType::NoSuchObject:
            case ExceptionType::NoSuchInstance:
            case ExceptionType::EndOfMibView:
                return true;
            default:
                break;
        }
        return false;
    }

    template<typename T>
    RawData::RawData(const T &t) : content_(nullptr) {
        encode(t, *this);   //@N isok?
    }
}

Q_DECLARE_METATYPE(osnmp::RawData)

#endif // RAWDATA_H
