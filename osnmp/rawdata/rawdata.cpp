#include "rawdata.h"
#include "contentlength.h"

osnmp::RawData::RawData() : id_(UniversalTagNumber::Null), content_(nullptr)
{

}

osnmp::RawData::RawData(const RawData &other) : id_(other.id_), content_(other.content_)
{
    if(content_ != nullptr) content_->ref.ref();
}

osnmp::RawData::RawData(RawData &&other) : id_(std::move(other.id_)), content_(other.content_)
{
    other.content_ = nullptr;
}

osnmp::RawData::RawData(const Identifier &id) : id_(id), content_(nullptr)
{

}

osnmp::RawData::RawData(const Identifier &id, Content *content_) : id_(id), content_(nullptr)
{
    setContent(content_);
}

osnmp::RawData::~RawData()
{
    if(content_ && !content_->ref.deref()) {
        delete content_;
    }
}

osnmp::RawData &osnmp::RawData::operator =(const RawData &other)
{
    if(this == &other)
        return *this;

    id_ = other.id_;

    if(content_ == nullptr) {
        if(other.content_ != nullptr) {
            other.content_->ref.ref();
            content_ = other.content_;
        }
    }
    else if(other.content_ == nullptr)
        clear();
    else {
        // other.content_ != nullptr
        // нужно скопировать
        qAtomicAssign(content_, other.content_);
    }

    return *this;
}

osnmp::RawData &osnmp::RawData::operator =(RawData &&other)
{
    id_ = std::move(other.id_);
    if(content_ != other.content_) {
        clear();
        content_ = other.content_;
        other.content_ = nullptr;
    }
    return *this;
}

bool osnmp::RawData::isEmpty() const
{
    return content_ == nullptr;
}

void osnmp::RawData::clear()
{
    if(content_ && !content_->ref.deref())
        delete content_;
    content_ = nullptr;
}

void osnmp::RawData::swap(RawData &data)
{
    std::swap(id_, data.id_);
    std::swap(content_, data.content_);
}

osnmp::ParseResult osnmp::RawData::decodeData(QDataStream &stream)
{
    clear();

    // Считаем идентификатор
    auto res = id_.decode(stream);
    if(!res.isValid())
        return res;

    // Все нормально. Теперь считаем данные
    if(id_.isPrimitive()) content_ = new PrimitiveContent;
    else content_ = new ConstructedContent;

    res += content_->decode(stream);
    return res;
}

osnmp::ParseError osnmp::RawData::encodeData(QDataStream &stream) const
{
    auto err = id_.encode(stream);
    if(err != ParseError::NoError)
        return err;
    if(content_ == nullptr) {
        ContentLength contentLength(0);
        auto err = contentLength.encode(stream);
        if(err != ParseError::NoError) return err;
    }
    else
        err = content_->encode(stream);
    return err;
}

void osnmp::RawData::setContent(Content *content)
{
    if(content_ == content) return;
    if(content_ == nullptr)
        content_ = content;
    else {
        if(!content_->ref.deref())
            delete content_;
        content_ = content;
    }
}

osnmp::RawData osnmp::RawData::fromByteArray(const QByteArray &array, osnmp::ParseError *error)
{
    QDataStream stream(const_cast<QByteArray*>(&array), QIODevice::ReadOnly);
    RawData data;
    auto res = data.decodeData(stream);
    if(error != nullptr) *error = res.error;
    return data;
}

QByteArray osnmp::RawData::toByteArray(osnmp::ParseError *error) const
{
    QByteArray array;
    QDataStream stream(const_cast<QByteArray*>(&array), QIODevice::ReadWrite);
    auto err = this->encodeData(stream);
    if(error != nullptr) *error = err;
    return array;
}





