#ifndef HEADERDATA_H
#define HEADERDATA_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    enum MsgFlag : quint8 {
        AuthenticationFlag = 1,
        ReportableFlag = 2,
        PrivacyFlag = 4
    };

    class HeaderData
    {
    public:
        HeaderData();

        int id() const;
        void setId(int id);

        int maxSize() const;
        void setMaxSize(int maxSize);

        QByteArray flags() const;
        void setFlags(quint8 flags);
        void setFlags(const QByteArray &flags);

        int securityModel() const;
        void setSecurityModel(int securityModel);

        bool isPrivacy() const;
        bool isReportable() const;
        bool isAuthentication() const;

    private:
        int id_;
        int maxSize_;
        QByteArray flags_;      // реально тут 1 байт
        int securityModel_;     // Целочисленное значение, указывающее, какая модель безопасности использовалась для этого сообщения
    };

    bool decode(const RawData &data, HeaderData &value);
    bool encode(const HeaderData &value, RawData &data);
}


#endif // HEADERDATA_H
