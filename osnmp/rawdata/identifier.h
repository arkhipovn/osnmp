#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QDebug>
#include <QDataStream>
#include "tag.h"
#include "decoderesult.h"

namespace osnmp {
    // Состоит из TagClass | TagType | TagNumber
    class Identifier
    {
    public:
        constexpr Identifier(TagClass tagClass, TagType type, TagNumber number) :
            c(tagClass), t(type), n(number) {}

        constexpr Identifier(TagClass tagClass, TagNumber number) :
            c(tagClass), t(TagType::Primitive), n(number) {}

        constexpr Identifier(quint8 id) :
            c(static_cast<TagClass>((id & kTagClassMask) >> kTagClassOffset)),
            t(static_cast<TagType>((id & kTagTypeMask) >> kTagTypeOffset)),
            n(static_cast<TagNumber>(id & 0x1F)) {}

        constexpr Identifier() :  c(TagClass::UniversalClass),
            t(TagType::Primitive),
            n(0) {}

        constexpr Identifier(const Identifier &id) : c(id.c), t(id.t), n(id.n) {}

        constexpr Identifier(const Identifier &&id) : c(id.c), t(id.t), n(id.n) {}

        constexpr Identifier(UniversalTagNumber tag, TagType type = TagType::Primitive) : //EncodingStructureComponent(),
            c(TagClass::UniversalClass),
            t(type),
            n(static_cast<TagNumber>(tag)) {}

        Identifier &operator =(const Identifier &id);
        Identifier &operator =(const UniversalTagNumber &tag);

        constexpr bool operator ==(const Identifier &other) const { return c == other.c && n == other.n; }
        constexpr bool operator !=(const Identifier &other) const { return !(*this == other); }
        constexpr bool operator ==(const UniversalTagNumber &tag) const { return c == TagClass::UniversalClass && this->n == static_cast<TagNumber>(tag); }
        constexpr bool operator !=(const UniversalTagNumber &tag) const { return !(*this == tag); }
        constexpr bool operator ==(const Tag &tag) const { return static_cast<Tag>(*this) == tag; }

        constexpr TagClass tagClass() const { return c; }
        constexpr TagType type() const { return t; }
        constexpr TagNumber number() const { return n; }

        constexpr Tag tag() const { return *this; }

        constexpr bool isPrimitive() const { return t == TagType::Primitive; }
        constexpr bool isConstructor() const { return t == TagType::Constructor; }

        void swap(Identifier &other);

        //! Это число нужно чисто для сравнения. Это не тоже самое число, что будет закодировано
        constexpr operator Tag() const { return (static_cast<Tag>(c) << (sizeof(Tag) * 8 - 8 + kTagClassOffset)) | static_cast<Tag>(n); }

        ParseResult decode(QDataStream &stream);
        ParseError encode(QDataStream &stream) const;

    protected:
        TagClass c;
        TagType t;
        TagNumber n;
    };
}

QDebug operator << (QDebug stream, const osnmp::Identifier &id);

#endif // IDENTIFIER_H

