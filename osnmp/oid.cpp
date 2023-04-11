#include "oid.h"

OID::OID() {}

OID::OID(const OID &o) : oidVector(o.oidVector) {}
OID::OID(OID &&o) : oidVector(std::move(o.oidVector)) {}

OID::OID(const QString &o)
{
    operator +=(o);
}

OID::OID(const QVector<sid> &oid) : oidVector(oid) {}

OID::OID(const char *o)
{
    operator +=(o);
}

OID::OID(std::initializer_list<OID::sid> args)
{
    auto it = args.begin();
    while(it != args.end()) {
        oidVector.append(*(it++));
    }
}

OID::OID(const sid *oid, const int length)
{
    set(oid, length);
}

OID::OID(OID::Special oid)
{
    operator =(oid);
}

void OID::clear()
{
    oidVector.clear();
}

OID &OID::set(const OID &o)
{
    if(this == &o) return *this;
    oidVector = o.oidVector;
    return *this;
}

OID &OID::set(const sid *o, int length)
{
    oidVector = QVector <sid>(length);
    for(int i = 0; i < length; ++i)
        oidVector[i] = o[i];
    return *this;
}

OID &OID::set(const QString &o)
{
    clear();
    return operator+=(o);
}

OID &OID::set(const char *o)
{
    clear();
    return operator+=(o);
}

void OID::append(const QString &o)
{
    operator +=(o);
}

void OID::append(const char *o)
{
    operator +=(o);
}

void OID::append(const OID &other)
{
    operator +=(other);
}

void OID::append(sid o)
{
    operator +=(o);
}


int OID::compare(const OID &oid) const
{
    return OID::compare(*this, oid);
}

int OID::compare(const OID &oid1, const OID &oid2)
{
    int min = qMin(oid1.size(), oid2.size());
    int count = 0;
    for(int i = 0; i < min; ++i) {
        if(oid1.at(i) != oid2.at(i)) break;
        ++count;
    }
    return count;
}

OID OID::operator-(const OID &other) const
{
    if(other.isEmpty()) return *this;
    int index = this->compare(other);
    return oidVector.mid(index);
}

OID OID::operator+(sid o) const
{
    OID temp(*this);
    temp.oidVector.append(o);
    return temp;
}

OID OID::operator+(const OID &other) const
{
    OID temp(*this);
    temp.oidVector.append(other.oidVector);
    return temp;
}

OID OID::operator+(const char * o) const
{
    OID temp(*this);
    temp +=(o);
    return temp;
}

OID OID::operator+(const QVector<OID::sid> &oid) const
{
    OID temp(*this);
    temp +=(oid);
    return temp;
}

OID &OID::operator+=(const char * o)
{
    quint8 pos = 0;
    sid value = 0;
    while(*o != '\0') {
        if(*o == '.') {
            oidVector.append(value);
            value = 0; pos = 0;
        }
        else if(std::isdigit(*o)) {
            value *= 10; value += *o - '0';
            ++pos;
        }
        else break;
        ++o;
    }
    if(pos != 0) oidVector.append(value);
    return *this;
}

OID &OID::operator+=(const QVector<OID::sid> &oid)
{
    if(oid.isEmpty()) return *this;
    this->oidVector.append(oid);
    return *this;
}

OID OID::operator+(const QString &o) const
{
    OID temp(*this);
    temp +=(o);
    return temp;
}

OID &OID::operator +=(const QString &o)
{
    if(o.isEmpty()) return *this;

    quint8 pos = 0;
    sid value = 0;
    for(auto it = o.constBegin(); it != o.constEnd(); ++it) {
        if(*it == '.') {
            oidVector.append(value);
            value = 0; pos = 0;
        }
        else {
            int v = it->digitValue();
            if(v == -1) break;
            value *= 10; value += v;
            ++pos;
        }
    }

    if(pos != 0) oidVector.append(value);
    return *this;
}

OID &OID::operator+=(const OID &other)
{
    if(other.isEmpty()) return *this;
    this->oidVector.append(other.oidVector);
    return *this;
}

OID &OID::operator+=(sid o)
{
    this->oidVector.append(o);
    return *this;
}

OID &OID::operator=(OID &&other)
{
    oidVector = std::move(other.oidVector);
    return *this;
}

OID &OID::operator =(const OID &other)
{
    return set(other);
}

OID &OID::operator =(const QString &other)
{
    return set(other);
}

OID &OID::operator=(OID::Special oid)
{
    switch (oid) {
        case Internet:
            this->oidVector = { 1, 3, 6, 1 };
            break;
        case Private:
            this->oidVector = { 1, 3, 6, 1, 4 };
            break;
        case Enterprises:
            this->oidVector = { 1, 3, 6, 1, 4, 1 };
            break;
        case SysUpTime:
            this->oidVector = { 1, 3, 6, 1, 2, 1, 1, 3, 0 };
            break;
        case Trap:
            this->oidVector = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 };
            break;
        case ColdStart:
            this->oidVector = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 1 };
            break;
        case WarmStart:
            this->oidVector = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 2 };
            break;
        case LinkDown:
            this->oidVector = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 3 };
            break;
        case LinkUp:
            this->oidVector = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 4 };
            break;
        case AuthenticationFailure:
            this->oidVector = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 5 };
            break;
        case EGPneighborLoss:
            this->oidVector = { 1, 3, 6, 1, 6, 3, 1, 1, 5, 6 };
            break;
        case Null:
        default:
            this->oidVector.clear();
            break;
    }
    return *this;
}

bool OID::operator ==(OID::Special oid) const
{
    return *this == OID(oid);
}

bool OID::operator<(const OID &other) const
{
    auto minLen = qMin(oidVector.size(), other.size());
    for(int i = 0; i < minLen; ++i) {
        if(oidVector.at(i) == other.oidVector.at(i)) continue;
        return oidVector.at(i) < other.oidVector.at(i);
    }

    // Прошлись до конца одного из массивов. Все данные равны
    // Значит решится все по размерам самого oid
    return oidVector.size() < other.size();
}

bool OID::operator>(const OID &other) const
{
    return other < *this;
}

bool OID::operator<=(const OID &other) const
{
    return !(*this > other);
}

bool OID::operator>=(const OID &other) const
{
    return !(*this < other);
}

bool OID::isChild(const OID &parent) const
{
    if(isEmpty() || parent.isEmpty()) return false;
    if(parent.size() >= this->size()) return false;
    for(int i = 0; i < static_cast<int>(parent.size()); ++i) {
        if(this->at(i) != parent.at(i)) return false;
    }
    return true;
}

bool OID::isParent(const OID &child) const
{
    if(isEmpty() || child.isEmpty()) return false;
    if(this->size() >= child.size()) return false;
    for(int i = 0; i < static_cast<int>(this->size()); ++i) {
        if(this->at(i) != child.at(i)) return false;
    }
    return true;
}

OID OID::left(int n) const
{
    return OID(oidVector.mid(0, n));
}

OID OID::right(int n) const
{
    return oidVector.size() > n ? OID(oidVector.mid(oidVector.size() - n))
                                : OID(oidVector);
}

OID OID::mid(int position, int n) const
{
    //    OID o(*this);
    //    o.oidVector = oidVector.mid(position, n);
    //    return o;
    return oidVector.mid(position, n);
}

QString OID::toString() const
{
    if(oidVector.isEmpty()) return "";
    QString str;
    for(int i = 0; i < oidVector.size() - 1; ++i)
        str.append(QString::number(oidVector.at(i)) + ".");
    str.append(QString::number(oidVector.last()));
    return str;
}

QByteArray OID::toByteArray() const
{
    if(oidVector.isEmpty()) return "";
    QByteArray str;
    for(int i = 0; i < oidVector.size() - 1; ++i)
        str.append(QByteArray::number(oidVector.at(i)) + ".");
    str.append(QByteArray::number(oidVector.last()));
    return str;
}

OID::operator QByteArray() const
{
    return toByteArray();
}

QDebug operator <<(QDebug stream, const OID &o)
{
    return stream << o.toString();
}
