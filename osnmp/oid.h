#ifndef OID_H
#define OID_H

#include <QVector>
#include <QString>
#include <QDebug>

class OID
{
public:
    using sid = quint32;

    OID();
    OID(const OID &o);
    OID(OID &&o);
    OID(const QString &o);
    OID(const QVector <sid> &oid);
    OID(const char *o);
    OID(std::initializer_list<sid> args);

    OID(const sid *oid, const int length);

    enum Special {
        Null,
        Internet,               // 1.3.6.1
        Private,                // 1.3.6.1.4
        Enterprises,            // 1.3.6.1.4.1
        SysUpTime,              // 1.3.6.1.2.1.1.3.0
        Trap,                   // 1.3.6.1.6.3.1.1.4.1.0

        ColdStart,              // 1.3.6.1.6.3.1.1.5.1
        WarmStart,              // 1.3.6.1.6.3.1.1.5.2
        LinkDown,               // 1.3.6.1.6.3.1.1.5.3
        LinkUp,                 // 1.3.6.1.6.3.1.1.5.4
        AuthenticationFailure,  // 1.3.6.1.6.3.1.1.5.5
        EGPneighborLoss         // 1.3.6.1.6.3.1.1.5.6
    };

    OID(Special oid);

    void clear();

    OID &set(const OID &o);
    OID &set(const sid *o, int length);
    OID &set(const QString &o);
    OID &set(const char *o);

    inline const sid &at(int i) const { return oidVector.at(i); }
    inline sid &operator[](int i) { return oidVector[i]; }
    inline const sid &operator[](int i) const { return oidVector.at(i); }

    inline int length() const { return static_cast<int>(oidVector.size()); }
    inline int size() const { return length(); }

    inline bool isNull() const { return oidVector.isEmpty(); }
    inline bool isEmpty() const { return oidVector.isEmpty(); }
    QString toString() const;
    QByteArray toByteArray() const;

    operator QByteArray() const;

    void append(sid o);
    void append(const QString &o);
    void append(const char *o);
    void append(const OID& other);

    // Находит разницу. Если 1.2.3.4.5 - 1.2.3.10.5.6 = 4.5
    OID operator- (const OID &other) const;

    OID operator+ (sid o) const;
    OID operator+ (const OID &other) const;
    OID operator+ (const QString &o) const;
    OID operator+ (const char *o) const;
    OID operator+ (const QVector <sid> &oid) const;


    OID &operator+= (sid o);
    OID &operator+= (const OID& o);
    OID &operator+= (const QString &o);
    OID &operator+= (const char * o);
    OID &operator+= (const QVector <sid> &oid);


    OID& operator= (OID&& other);

    OID& operator= (const OID& other);
    OID& operator= (const QString &o);

    OID& operator= (Special oid);

    bool operator ==(Special oid) const;

    inline bool operator== (const OID &other) const { return oidVector == other.oidVector; }
    inline bool operator!= (const OID &other) const { return oidVector != other.oidVector; }

    // Операторы сравнения исходят из принципа
    // 1.2.3 < 1.3.3;   1.3.3.4 < 1.3.3.4.1
    // Тоесть сравниваются последовательно все элементы
    bool operator< (const OID &other) const;
    bool operator> (const OID &other) const;

    bool operator<= (const OID &other) const;
    bool operator>= (const OID &other) const;

    // Проверяет является ли OID ребенком у @parent
    // OID oid("1.2.3.4.5"); oid.isChild("1.2.3") == true
    bool isChild(const OID &parent) const;

    // Проверяет является ли OID родителем у @child
    // OID oid("1.2.3"); oid.isParent("1.2.3.4.5") == true
    bool isParent(const OID &child) const;


    OID left(int n) const;
    OID right(int n) const;  //@N не проверял еще
    OID mid(int position, int n = -1) const;



    // Возвращает количество равных элементов (до первого не правильно)
    static int compare(const OID &oid1, const OID &oid2);
    int compare(const OID &oid) const;

private:
    QVector <sid> oidVector;
};


QDebug operator <<(QDebug stream, const OID &o);

using OIDVector = QVector <OID>;


#endif // OID_H
