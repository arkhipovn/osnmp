#include "constructedcontent.h"
#include "primitivecontent.h"
#include "contentlength.h"
#include "rawdata.h"
#include "osnmp/types/eoc.h"

osnmp::ConstructedContent::ConstructedContent() : Content()
{

}

osnmp::ConstructedContent::ConstructedContent(const ConstructedContent &other) : Content(), data(other.data)
{

}

osnmp::ConstructedContent::ConstructedContent(ConstructedContent &&other) : Content(), data(std::move(other.data))
{

}

osnmp::ConstructedContent::ConstructedContent(const RawDataVector &content) : Content(), data(content)
{

}

osnmp::ConstructedContent &osnmp::ConstructedContent::operator =(const ConstructedContent &content)
{
    if(this != &content)
        data = content.data;
    return *this;
}

osnmp::ConstructedContent &osnmp::ConstructedContent::operator =(const RawDataVector &content)
{
    data = content;
    return *this;
}


// _________________

osnmp::ParseResult osnmp::ConstructedContent::decode(QDataStream &stream)
{
    data.clear();

    ContentLength contentLength;
    auto res = contentLength.decode(stream);
    if(!res.isValid()) return res;

    if(contentLength.isIndefiniteForm()) {
        // Неопределенная форма
        // Размер не задан. Будем искать EOC

        while(!stream.atEnd()) {
            RawData asnData;
            auto decRes = asnData.decodeData(stream);
            res += decRes;
            if(!decRes.isValid())
                return { decRes.error, res.bytes };

            // Посмотрим завершился ли контент
            if(asnData.isEOC()) return res;
            data.append(asnData);
        }
    }
    else {
        // значит задан размер
        if(contentLength == size_t(0)) return res;
        size_t currentLen = 0;

        // считаем данные
        while(!stream.atEnd()) {
            RawData asnData;
            auto decRes = asnData.decodeData(stream);
            currentLen += decRes.bytes;
            if(!decRes.isValid())
                return { decRes.error, res.bytes + currentLen };

            data.append(asnData);

            if(currentLen == contentLength) // все хорошо
                return { ParseError::NoError, res.bytes + currentLen };   // Считали все что нужно
            else if(currentLen > contentLength) {
                res.bytes += currentLen;
                break;  // WrongLen
            }
        }
    }

    return { ParseError::WrongLen, res.bytes };
}

osnmp::ParseError osnmp::ConstructedContent::encode(QDataStream &stream) const
{
    QByteArray content;
    QDataStream contentStream(&content, QIODevice::WriteOnly);
    for(int i = 0; i < data.size(); ++i) {
        auto err = data.at(i).encodeData(contentStream);
        if(err != ParseError::NoError) return err;
    }

    //@N Подумать насчет неопредленного размера
    ContentLength contentLength(content.size());
    auto err = contentLength.encode(stream);
    if(err != ParseError::NoError) return err;

    stream.writeRawData(content.data(), content.size());
    if(contentLength.isIndefiniteForm())
        err = RawData(EOC()).encodeData(stream);
    return err;
}
