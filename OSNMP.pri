QT += network
CONFIG += c++11
INCLUDEPATH += $$PWD

DEFINES += QOBJECT_AGENT_STRATEGY
DEFINES += ASYNC_AGENT_SERVER

HEADERS += \
    $$PWD/osnmp/agent/abstractagent.h \
    $$PWD/osnmp/agent/strategy/abstractagentcolumnstrategy.h \
    $$PWD/osnmp/agent/strategy/abstractagentstrategy.h \
    $$PWD/osnmp/agent/strategy/abstractagenttablestrategy.h \
    $$PWD/osnmp/agent/udpagent.h \
    $$PWD/osnmp/conf/communitysettings.h \
    $$PWD/osnmp/conf/conffunctions.h \
    $$PWD/osnmp/conf/snmpconf.h \
    $$PWD/osnmp/conf/trapsettings.h \
    $$PWD/osnmp/message/messagefactory.h \
    $$PWD/osnmp/message/trapv2.h \
    $$PWD/osnmp/mib/mib.h \
    $$PWD/osnmp/mib/mibmodule.h \
    $$PWD/osnmp/mib/mibobject.h \
    $$PWD/osnmp/mib/mibobjectlist.h \
    $$PWD/osnmp/mib/mibsearchresult.h \
    $$PWD/osnmp/mib/mibtableobject.h \
    $$PWD/osnmp.h \
    $$PWD/osnmp/message/headerdata.h \
    $$PWD/osnmp/message/scopedpdu.h \
    $$PWD/osnmp/message/securityparameters.h \
    $$PWD/osnmp/message/snmpmessage.h \
    $$PWD/osnmp/message/snmpv1message.h \
    $$PWD/osnmp/message/snmpv2message.h \
    $$PWD/osnmp/message/snmpv3message.h \
    $$PWD/osnmp/message/pdu.h \
    $$PWD/osnmp/message/varbind.h \
    $$PWD/osnmp/oid.h \
    $$PWD/osnmp/rawdata/constants.h \
    $$PWD/osnmp/rawdata/constructedcontent.h \
    $$PWD/osnmp/rawdata/content.h \
    $$PWD/osnmp/rawdata/contentlength.h \
    $$PWD/osnmp/rawdata/decoderesult.h \
    $$PWD/osnmp/rawdata/identifier.h \
    $$PWD/osnmp/rawdata/primitivecontent.h \
    $$PWD/osnmp/rawdata/rawdata.h \
    $$PWD/osnmp/rawdata/tag.h \
    $$PWD/osnmp/types/all.h \
    $$PWD/osnmp/types/bitstring.h \
    $$PWD/osnmp/types/counter.h \
    $$PWD/osnmp/types/counter64.h \
    $$PWD/osnmp/types/eoc.h \
    $$PWD/osnmp/types/exeption/endofmibview.h \
    $$PWD/osnmp/types/exeption/nosuchinstance.h \
    $$PWD/osnmp/types/exeption/nosuchobject.h \
    $$PWD/osnmp/types/integer.h \
    $$PWD/osnmp/types/ipaddress.h \
    $$PWD/osnmp/types/null.h \
    $$PWD/osnmp/types/objectid.h \
    $$PWD/osnmp/types/octetstring.h \
    $$PWD/osnmp/types/private/auto_value.h \
    $$PWD/osnmp/types/private/decoder.h \
    $$PWD/osnmp/types/sequence.h \
    $$PWD/osnmp/types/sequenceof.h \
    $$PWD/osnmp/types/timeticks.h \
    $$PWD/osnmp/types/uinteger.h \
    $$PWD/osnmp/variant.h
SOURCES += \
    $$PWD/osnmp/agent/abstractagent.cpp \
    $$PWD/osnmp/agent/strategy/abstractagentcolumnstrategy.cpp \
    $$PWD/osnmp/agent/udpagent.cpp \
    $$PWD/osnmp/conf/communitysettings.cpp \
    $$PWD/osnmp/conf/conffunctions.cpp \
    $$PWD/osnmp/conf/snmpconf.cpp \
    $$PWD/osnmp/conf/trapsettings.cpp \
    $$PWD/osnmp/message/messagefactory.cpp \
    $$PWD/osnmp/message/snmpv2message.cpp \
    $$PWD/osnmp/message/trapv2.cpp \
    $$PWD/osnmp/mib/mib.cpp \
    $$PWD/osnmp/mib/mibmodule.cpp \
    $$PWD/osnmp/mib/mibobject.cpp \
    $$PWD/osnmp/mib/mibobjectlist.cpp \
    $$PWD/osnmp/mib/mibtableobject.cpp \
    $$PWD/osnmp/message/headerdata.cpp \
    $$PWD/osnmp/message/scopedpdu.cpp \
    $$PWD/osnmp/message/pdu.cpp \
    $$PWD/osnmp/message/securityparameters.cpp \
    $$PWD/osnmp/message/snmpv1message.cpp \
    $$PWD/osnmp/message/snmpv3message.cpp \
    $$PWD/osnmp/message/varbind.cpp \
    $$PWD/osnmp/oid.cpp \
    $$PWD/osnmp/rawdata/constructedcontent.cpp \
    $$PWD/osnmp/rawdata/contentlength.cpp \
    $$PWD/osnmp/rawdata/identifier.cpp \
    $$PWD/osnmp/rawdata/primitivecontent.cpp \
    $$PWD/osnmp/rawdata/rawdata.cpp \
    $$PWD/osnmp/types/bitstring.cpp \
    $$PWD/osnmp/types/counter.cpp \
    $$PWD/osnmp/types/counter64.cpp \
    $$PWD/osnmp/types/eoc.cpp \
    $$PWD/osnmp/types/exeption/endofmibview.cpp \
    $$PWD/osnmp/types/exeption/nosuchinstance.cpp \
    $$PWD/osnmp/types/exeption/nosuchobject.cpp \
    $$PWD/osnmp/types/integer.cpp \
    $$PWD/osnmp/types/ipaddress.cpp \
    $$PWD/osnmp/types/null.cpp \
    $$PWD/osnmp/types/objectid.cpp \
    $$PWD/osnmp/types/octetstring.cpp \
    $$PWD/osnmp/types/timeticks.cpp \
    $$PWD/osnmp/types/uinteger.cpp \
    $$PWD/osnmp/variant.cpp \
    $$PWD/osnmp/agent/strategy/abstractagentstrategy.cpp \
    $$PWD/osnmp/agent/strategy/abstractagenttablestrategy.cpp

