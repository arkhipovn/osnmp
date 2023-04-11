#include "conffunctions.h"

bool osnmp::checkAccess(const OIDVector &vector, const OID &oid, bool exact)
{
    if(oid.isEmpty()) return false;
    if(vector.isEmpty()) return true;
    for(auto it = vector.constBegin(); it != vector.constEnd(); ++it) {
        int compare = it->compare(oid);

        if(exact) {
            if(compare == it->size()) return true;
        }
        else {
            if(compare == it->size() || compare == oid.size()) return true;
        }

        //        if(it->isParent(oid) || (*it) == oid) return true;
    }
    return false;
}
