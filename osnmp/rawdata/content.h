#ifndef ASNCONTENT_H
#define ASNCONTENT_H

#include <QAtomicInt>
#include "decoderesult.h"

namespace osnmp {
    class Content {
    public:
        Content() : ref(1) {}
        virtual ~Content() = default;
        QAtomicInt ref;

        virtual ParseResult decode(QDataStream &stream) = 0;
        virtual ParseError encode(QDataStream &stream) const = 0;
    };
}

#endif // ASNCONTENT_H
