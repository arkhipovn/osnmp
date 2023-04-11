#ifndef PRIMITIVECONTENT_H
#define PRIMITIVECONTENT_H

#include <QByteArray>
#include "content.h"

namespace osnmp {
    class PrimitiveContent : public Content
    {
    public:
        PrimitiveContent();
        PrimitiveContent(const char *content, int size = -1);
        PrimitiveContent(const PrimitiveContent &other);
        PrimitiveContent(PrimitiveContent &&other);
        PrimitiveContent(const QByteArray &content);

        PrimitiveContent &operator =(const PrimitiveContent &content);
        PrimitiveContent &operator =(const QByteArray &content);

        inline void append(const char *s, int len) { data.append(s, len); }
        inline void append(const QByteArray &a) { data.append(a); }
        inline bool isEmpty() const { return data.isEmpty(); }

        inline int size() const {  return data.size(); }

        inline char at(int i) const { return data.at(i); }
        inline char operator[](int i) const { return data[i]; }
        inline QByteRef operator[](int i) { return data[i]; }

        inline QByteArray::iterator begin() { return data.begin(); }
        inline QByteArray::const_iterator constBegin() const { return data.constBegin(); }

        inline QByteArray::iterator end() { return data.end(); }
        inline QByteArray::const_iterator constEnd() const { return data.constEnd(); }

        inline void clear() { data.clear(); }
        operator QByteArray() const { return data; }

        ParseResult decode(QDataStream &stream) override;
        ParseError encode(QDataStream &stream) const override;

    private:
        QByteArray data;
    };
}

#endif // PRIMITIVECONTENT_H
