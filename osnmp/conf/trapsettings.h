#ifndef TRAPSETTINGS_H
#define TRAPSETTINGS_H

#include <QHostAddress>
#include "communitysettings.h"

namespace osnmp {
    using TrapTarget = QHostAddress;

    class TrapSettings {
    public:
        TrapSettings();

        void appendTrapTarget(const TrapTarget &target);
        void eraseTrapTarget(const TrapTarget &target);
        void clearTrapTargets();

        QVector<TrapTarget> trapTargets() const;
        void setTrapTargets(const QVector<TrapTarget> &TrapTargets);

        bool enable() const;
        void setEnable(bool enable);

        QString community() const;
        void setCommunity(const QString &community);

        quint16 port() const;
        void setPort(const quint16 &port);

    private:
        bool enable_;
        quint16 port_;   // по умолчанию 162;
        QString community_;
        QVector <QHostAddress> trapTargets_;
    };
}

#endif // TRAPSETTINGS_H
