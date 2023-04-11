#ifndef ASNSEQUENCEOF_H
#define ASNSEQUENCEOF_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    template <typename T>
    class SequenceOf {
    public:
        using iterator = ConstructedContent::iterator;
        using const_iterator = ConstructedContent::const_iterator;

        SequenceOf() {}
        SequenceOf(const SequenceOf &other) : data(other.data) {}
        SequenceOf(const SequenceOf &&other) : data(std::move(other.data)) {}
        SequenceOf(const ConstructedContent &value) : data(value) {}
        SequenceOf(std::initializer_list<T> args);
        SequenceOf(const QVector <T> &value);

        inline void append(const RawData &t) { data.append(t); }
        //        inline void append(const T &t) { data.append(t); }    //@N cannot be overloaded в случае Variant

        SequenceOf &operator =(const SequenceOf <T> &v) {
            if(this != &v)
                data = v.data;
            return *this;
        }

        operator ConstructedContent() const { return data; }
        QVector<T> toVector() const;

        inline void clear() { data.clear(); }

        inline bool isEmpty() const { return data.isEmpty(); }
        inline int size() const {  return data.size(); }

        inline T &first() { return data.first(); }
        inline const T &first() const { return data.first(); }

        inline T &last() { return data.last(); }
        inline const T &last() const { return data.last(); }

        inline const T &at(int i) const { return data.at(i); }
        inline T &operator[](int i) { return data[i]; }
        inline const T &operator[](int i) const { return data[i]; }

        inline iterator begin() { return data.begin(); }
        inline const_iterator constBegin() const { return data.constBegin(); }

        inline iterator end() { return data.end(); }
        inline const_iterator constEnd() const { return data.constEnd(); }

    private:
        ConstructedContent data;
    };

    template<typename T>
    SequenceOf <T>::SequenceOf(std::initializer_list<T> args)
    {
        for(auto it = args.begin(); it != args.end(); ++it)
            data.append(*it);
    }

    //@N
    template<typename T>
    SequenceOf<T>::SequenceOf(const QVector<T> &value)
    {
        for(int i = 0; i < value.size(); ++i)
            data.append(value.at(i));
    }

    //@N
    template<typename T>
    QVector<T> SequenceOf<T>::toVector() const
    {
        QVector <T> vec;
        for(int i = 0; i < data.size(); ++i) {
            T t = data.at(i);
            vec.append(t);
        }
        return vec;
    }

    template<typename T>
    bool encode(const SequenceOf <T> &value, RawData &data) {
        auto content = new ConstructedContent(value);
        data = std::move(RawData(Identifier(UniversalTagNumber::Sequence, TagType::Constructor), content));
        return true;
    }

    //@N SequenceOf проверить на утечки!!
    template<typename T>
    bool decode(const RawData &data, SequenceOf <T> &value) {
        value.clear();

        if(data.identifier() != UniversalTagNumber::Sequence) return false;
        if(data.isEmpty() || data.isPrimitive()) return false;

        value = SequenceOf <T>(*static_cast<ConstructedContent*>(data.content()));
        return true;
    }

}




#endif // SEQUENCEOF_H
