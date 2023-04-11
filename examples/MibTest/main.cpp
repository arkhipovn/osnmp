#include <QCoreApplication>

#include <QDebug>

#include "osnmp/mib/mibmodule.h"
#include "osnmp/mib/mibtableobject.h"

#include "osnmp/agent/strategy/abstractagentcolumnstrategy.h"
#include "osnmp/agent/udpagent.h"

void test(const osnmp::MibModule &mib, const OID &oid, bool exact) {
    auto v = mib.find(oid, exact);
    if(v.isEmpty()) qDebug() << oid << ": Null";
    else qDebug() << oid << ":" << v.mib->absoluteOid() + v.tableIndex;
}


class Temp : public osnmp::AbstractAgentColumnStrategy {
public:
    Temp() : osnmp::AbstractAgentColumnStrategy(5) {}

    osnmp::Variant getValue(int valueId, const OID &index, osnmp::ErrorCodes &errorCode) {}
    osnmp::Variant setValue(int valueId, const OID &index, const osnmp::Variant &var, osnmp::ErrorCodes &errorCode)  {}
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    using namespace osnmp;
    MibModule module("1");

    module.append(new MibObject(1, Type::Integer, ObjectAccess::ReadWrite, "1" ));

    auto mibmodule2 = new MibModule("2");
    module.append(mibmodule2);
    mibmodule2->append({
                           new MibObject(2, Type::Integer, ObjectAccess::ReadWrite, "1" ),
                           new MibObject(3, Type::Integer, ObjectAccess::ReadWrite, "30" )
                           //                           new MibModule("1"),
                           //                           new MibModule("30")
                       });


    //    auto mibmodule2 = new MibTableObject(ObjectAccess::ReadOnly, "2", new Temp());// MibModule("2");
    //    module.append(mibmodule2);
    //    mibmodule2->append({
    //                           new MibObject(2, Type::Integer, ObjectAccess::ReadWrite, "1" ),
    //                           new MibObject(3, Type::Integer, ObjectAccess::ReadWrite, "30" )
    //                           //                           new MibModule("1"),
    //                           //                           new MibModule("30")
    //                       });

    module.append(new MibObject(4, Type::Integer, ObjectAccess::ReadWrite, "5" ));
    module.append(new MibObject(5, Type::Integer, ObjectAccess::ReadWrite, "7" ));


    bool exact = false;
    test(module, "1", exact);
    test(module, "2", exact);

    test(module, "1.1", exact);
    test(module, "1.1.2", exact);
    test(module, "1.1.0", exact);

    test(module, "1.2", exact);
    test(module, "1.2.1", exact);
    test(module, "1.2.1.0", exact);
    test(module, "1.2.1.2", exact);
    test(module, "1.2.1.5", exact);
    test(module, "1.2.1.8", exact);
    test(module, "1.2.3", exact);
    test(module, "1.2.1.5", exact);
    test(module, "1.2.3.3", exact);
    test(module, "1.2.30.5", exact);

    test(module, "1.5", exact);
    test(module, "1.6", exact);
    test(module, "1.7", exact);
    test(module, "1.7.0", exact);
    test(module, "1.10", exact);



    return a.exec();
}
