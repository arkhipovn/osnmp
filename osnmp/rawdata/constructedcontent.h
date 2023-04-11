#ifndef CONSTRUCTEDCONTENT_H
#define CONSTRUCTEDCONTENT_H

#include <QVector>
#include "content.h"

namespace osnmp {
    class RawData;
    class ConstructedContent : public Content
    {
    public:
        using RawDataVector = QVector <RawData>;
        using iterator = RawDataVector::iterator;
        using const_iterator = RawDataVector::const_iterator;

        ConstructedContent();
        ConstructedContent(const ConstructedContent &other);
        ConstructedContent(ConstructedContent &&other);
        ConstructedContent(const RawDataVector &content);

        ConstructedContent &operator =(const ConstructedContent &content);
        ConstructedContent &operator =(const RawDataVector &content);

        inline void append(const RawData &data) { this->data.append(data); }

        inline bool isEmpty() const { return data.isEmpty(); }

        inline int size() const { return data.size(); }

        inline void clear() { data.clear(); }

        inline RawData &first() { return data.first(); }
        inline const RawData &first() const { return data.first(); }

        inline RawData &last() { return data.last(); }
        inline const RawData &last() const { return data.last(); }

        inline const RawData &at(int i) const { return data.at(i); }
        inline RawData &operator[](int i) { return data[i]; }
        inline const RawData &operator[](int i) const { return data[i]; }

        inline iterator begin() { return data.begin(); }
        inline const_iterator constBegin() const { return data.constBegin(); }

        inline iterator end() { return data.end(); }
        inline const_iterator constEnd() const { return data.constEnd(); }

        operator RawDataVector() const { return data; }

        ParseResult decode(QDataStream &stream) override;
        ParseError encode(QDataStream &stream) const override;

    private:
        RawDataVector data;
    };
}


#endif // CONSTRUCTEDCONTENT_H
