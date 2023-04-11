#include "communitysettings.h"
#include "conffunctions.h"

osnmp::CommunitySettings::CommunitySettings(osnmp::ObjectAccess access, const OIDVector &includes) : access_(access)
{
    if(!includes.isEmpty()) {
        auto array = includes;
        std::sort(array.begin(), array.end());
        for(int i = 0; i < array.size(); ++i) append(array.at(i));
    }
}

void osnmp::CommunitySettings::append(const OID &oid)
{
    if(oid.isEmpty()) return;
    if(includes_.isEmpty()) {
        includes_.append(oid);
        return;
    }
    if(checkAccess(oid)) return;    // Если есть доступ, значит есть вложенность
    includes_.append(oid);
}

bool osnmp::CommunitySettings::contains(const OID &oid) const
{
    return includes_.contains(oid);
}

bool osnmp::CommunitySettings::checkAccess(const OID &oid) const
{
    return osnmp::checkAccess(includes_, oid);
}

OIDVector osnmp::CommunitySettings::includes() const
{
    return includes_;
}

void osnmp::CommunitySettings::setIncludes(const OIDVector &includes)
{
    includes_ = includes;
}

void osnmp::CommunitySettings::setAccess(const ObjectAccess &access)
{
    access_ = access;
}

