#include "primitivecontent.h"
#include "contentlength.h"
#include "osnmp/types/eoc.h"

osnmp::PrimitiveContent::PrimitiveContent() : Content()
{

}

osnmp::PrimitiveContent::PrimitiveContent(const char *content, int size) : Content(), data(QByteArray(content, size))
{

}

osnmp::PrimitiveContent::PrimitiveContent(const PrimitiveContent &other) : Content(), data(other.data)
{

}

osnmp::PrimitiveContent::PrimitiveContent(PrimitiveContent &&other) : Content(), data(std::move(other.data))
{

}

osnmp::PrimitiveContent::PrimitiveContent(const QByteArray &content) : Content(), data(content)
{

}

osnmp::PrimitiveContent &osnmp::PrimitiveContent::operator =(const PrimitiveContent &content)
{
    if(this != &content)
        data = content.data;
    return *this;
}

osnmp::PrimitiveContent &osnmp::PrimitiveContent::operator =(const QByteArray &content)
{
    data = content;
    return *this;
}

// _________________

osnmp::ParseResult osnmp::PrimitiveContent::decode(QDataStream &stream)
{
    data.clear();
    ContentLength contentLength;
    auto res = contentLength.decode(stream);
    if(!res.isValid()) return res;

    if(contentLength.isIndefiniteForm()) {
        // Неопределенная форма
        // Размер не задан. Будем искать EOC

        char bytes[2];
        while(!stream.atEnd()) {
            // Cчитаем два бита
            if(stream.readRawData(bytes, EOC::length) != EOC::length)
                break;
            res.bytes += EOC::length;

            if(EOC::isEOC(bytes)) // Нашли. Выходим
                return res;

            // Не нашли. Добавим в данные
            data.append(bytes, EOC::length);
        }

        return { ParseError::WrongLen, res.bytes };
    }
    else {
        // значит задан размер
        if(contentLength == size_t(0))
            return res;

        // считаем данные
        for(int i = 0; i < static_cast<int>(contentLength) && !stream.atEnd(); ++i) {
            quint8 b; stream >> b;
            data.append(b);
        }

        if(data.size() != static_cast <int>(contentLength))
            return { ParseError::WrongLen, 0 };

        //                if(stream.readRawData(value.data(), static_cast<int>(contentLength)) != static_cast <int>(contentLength))
        //                    return { ParseError::WrongLen, 0 };

        res.bytes += contentLength;
    }

    return res;
}

osnmp::ParseError osnmp::PrimitiveContent::encode(QDataStream &stream) const
{
    //@N Подумать насчет неопредленного размера
    ContentLength contentLength(data.size());
    auto err = contentLength.encode(stream);
    if(err != ParseError::NoError) return err;

    if(!data.isEmpty()) {
        stream.writeRawData(data.data(), data.size());
    }

    if(contentLength.isIndefiniteForm())
        err = RawData(EOC()).encodeData(stream);
    return err;
}


