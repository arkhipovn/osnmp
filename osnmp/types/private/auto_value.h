#ifndef AUTO_VALUE_H
#define AUTO_VALUE_H

template <typename T, T i = 0>
class auto_value
{
public:
    using self_t = auto_value&;

    constexpr auto_value() : d(i) {}

    // конструктор с 1 параметром (в том числе - конструктор копирования)
    template <class V>
    constexpr auto_value(V v) : d(v) {}

    // доступ к данным
    inline const T& data() const { return d; }
    inline T& data() { return d; }

    // считается, что исходный тип - простой
    inline operator T() const { return d; }
    inline operator T&() { return d; }

    // операторы присваивания
    template <class V> inline self_t operator =   (V v) { d =   v; return *this; }
    template <class V> inline self_t operator +=  (V v) { d +=  v; return *this; }
    template <class V> inline self_t operator -=  (V v) { d -=  v; return *this; }
    template <class V> inline self_t operator *=  (V v) { d *=  v; return *this; }
    template <class V> inline self_t operator /=  (V v) { d /=  v; return *this; }
    template <class V> inline self_t operator %=  (V v) { d %=  v; return *this; }
    template <class V> inline self_t operator &=  (V v) { d &=  v; return *this; }
    template <class V> inline self_t operator |=  (V v) { d |=  v; return *this; }
    template <class V> inline self_t operator ^=  (V v) { d ^=  v; return *this; }
    template <class V> inline self_t operator <<= (V v) { d <<= v; return *this; }
    template <class V> inline self_t operator >>= (V v) { d >>= v; return *this; }

private:
    T d;
};

#endif // AUTO_VALUE_H
