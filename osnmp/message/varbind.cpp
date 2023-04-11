#include "varbind.h"

osnmp::Varbind::Varbind() : oid_(OID::Null)
{

}

osnmp::Varbind::Varbind(const OID &oid) : oid_(oid)
{

}

osnmp::Varbind::Varbind(const OID &oid, const Variant &data) : oid_(oid), var_(data)
{

}

osnmp::Varbind::Varbind(const Varbind &other) : oid_(other.oid_), var_(other.var_)
{

}

osnmp::Varbind::Varbind(Varbind &&other) : oid_(std::move(other.oid_)), var_(std::move(other.var_))
{

}

osnmp::Varbind &osnmp::Varbind::operator =(const Varbind &other)
{
    if(this != &other) {
        oid_ = other.oid_;
        var_ = other.var_;
    }
    return *this;
}

osnmp::Varbind &osnmp::Varbind::operator =(const Variant &data)
{
    var_ = data;
    return *this;
}

bool osnmp::Varbind::isNull() const
{
    return var_.isNull();
}

bool osnmp::Varbind::isExceptionData() const
{
    return var_.isException();
}

void osnmp::Varbind::clear()
{
    oid_ = OID::Null;
    var_.clear();
}

bool osnmp::decode(const RawData &data, Varbind &value)
{
    value.clear();

    if(data.identifier() != osnmp::UniversalTagNumber::Sequence) return false;
    if(data.isEmpty() || data.isPrimitive()) return false;

    auto content = static_cast<ConstructedContent*>(data.content());
    if(content->size() != 2) return false;

    OID oid;
    if(!decode(content->at(0), oid)) return false;

    value = Varbind(oid, content->at(1));
    return true;
}

bool osnmp::encode(const Varbind &value, RawData &data)
{
    auto content = new ConstructedContent({ value.oid(), value.data() });
    data = std::move(RawData(Identifier(osnmp::UniversalTagNumber::SequenceOf, osnmp::TagType::Constructor), content));
    return true;
    //    Sequence seq = { value.oid(), value.data() };
    //    return encode(seq, data);
}

