#ifndef TIMETICKS_H
#define TIMETICKS_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    class TimeTicks {
    public:
        using self_t = TimeTicks&;

        constexpr TimeTicks() : d(0) {}

        // конструктор с 1 параметром (в том числе - конструктор копирования)
        template< class V >
        constexpr TimeTicks(V v) : d(v) {}

        // доступ к данным
        inline const quint64& data() const { return d; }
        inline quint64& data() { return d; }

        // считается, что исходный тип - простой
        inline operator quint64() const { return d; }
        inline operator quint64&() { return d; }

        static TimeTicks upTime();

    private:
        quint64 d;
    };

    bool decode(const RawData &data, TimeTicks &value);
    bool encode(TimeTicks value, RawData &data);
}

#endif // TIMETICKS_H
