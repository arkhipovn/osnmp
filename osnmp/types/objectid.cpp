#include "objectid.h"

//@N переделать по нормальному

bool osnmp::decode(const RawData &data, ObjectId &value)
{
    value = OID::Null;

    if(data.identifier() != UniversalTagNumber::ObjectId) return false;
    if(data.isEmpty() || !data.isPrimitive()) return false;

    auto content = static_cast<PrimitiveContent*>(data.content());
    if(content->size() == 0) return false;

    QVector <OID::sid> vector;

    vector.append(content->at(0) / 40);
    vector.append(content->at(0) % 40);
    for(int i = 1, v = 0; i < content->size(); ++i) {
        v += kLowBits & content->at(i);
        if((content->at(i) & kBit8) == 0) {
            vector.append(v);
            v = 0;
        }
        else
            v <<= 7;
    }

    value = OID(vector);
    return true;
}

bool osnmp::encode(const ObjectId &value, RawData &data)
{
    if(value.size() == 0) return false;

    QByteArray content;
    if(value.size() == 1)
        content.append(static_cast<quint8>(value.at(0) * 40));
    else
        content.append(static_cast<quint8>(value.at(0) * 40 + value.at(1)));

    for(int i = 2; i < value.size(); ++i) {
        QByteArray result;
        QByteArray tmp;
        OID::sid sid = value.at(i);

        if(sid < kBit8)
            result.append(sid);

        else {
            while(sid != 0) {
                qint8 bval = (qint8)(sid & 0xFF);
                if ((bval & kBit8) != 0) {
                    bval = (qint8)(bval & kLowBits);
                }

                sid >>= 7;
                tmp.append(bval);
            }

            for(int i = tmp.length() - 1; i >= 0; i--) {
                if (i > 0)
                    result.append(tmp[i] | kBit8);
                else
                    result.append(tmp[i]);
            }
        }

        content.append(result);
    }

    data = std::move(RawData(UniversalTagNumber::ObjectId, new PrimitiveContent(content)));
    return true;
}
