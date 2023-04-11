#ifndef CONFFUNCTIONS_H
#define CONFFUNCTIONS_H

#include "osnmp/oid.h"

namespace osnmp {
    // Проверка входит есть ли хоть один родитель oid в vector. Если список пуст return true;
    bool checkAccess(const OIDVector &vector, const OID &oid, bool exact = true);
}

#endif // CONFFUNCTIONS_H
