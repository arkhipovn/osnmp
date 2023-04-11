#ifndef COMMUNITYSETTINGS_H
#define COMMUNITYSETTINGS_H

#include "osnmp/oid.h"
#include "osnmp.h"
// Маска сообщества

namespace osnmp {
    // При добавлении { "1.2.3", "1.4", "1.4.5" } исключит "1.4.5". Тк он вложен в "1.4"

    class CommunitySettings
    {
    public:
        CommunitySettings(ObjectAccess access, const OIDVector &includes = {});

        // Add include
        void append(const OID &oid);
        bool contains(const OID &oid) const;

        void setAccess(const ObjectAccess &access);
        inline ObjectAccess access() const { return access_; }
        inline bool isEmpty() const { return includes_.isEmpty(); }

        // Если список пуст, значит есть доступ ко всем данным. Если нет, то там уже будет проверка
        bool checkAccess(const OID &oid) const;

        OIDVector includes() const;
        void setIncludes(const OIDVector &includes);

    private:
        OIDVector includes_;
        ObjectAccess access_;    // RO, RW
    };
}

#endif // COMMUNITYSETTINGS_H
