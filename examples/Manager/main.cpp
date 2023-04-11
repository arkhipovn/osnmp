#include <QCoreApplication>

#include "test.h"
#include "osnmp/rawdata/contentlength.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    using namespace osnmp;

//    ContentLength l = 500;

//    QByteArray array;
//    QDataStream st(&array, QIODevice::ReadWrite);
//    l.encode(st);
//int step = 0;
//    return 0;




//    SnmpV2Message m;
//    m.setCommunity("public");

    //    Pdu p(PduType::GetNext);
    //    p.setRequestId(1010);
    //    p.addNullVar("1.3.10");
    //    p.addNullVar("1.3.6.1.2.1.4.20");






    Pdu p(PduType::GetBulk);
    p.setRequestId(1010);
    p.addNullVar("1");

    p.setMaxRepetitions(100);


    m.setPdu(p);

    TestSocket test;
    test.open();
    qDebug() << test.write(m);



    return a.exec();
}
