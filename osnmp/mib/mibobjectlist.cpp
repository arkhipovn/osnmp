#include "mibobjectlist.h"
#include "osnmp/conf/conffunctions.h"

osnmp::MibObjectList::MibObjectList()
{

}

osnmp::MibObjectList::MibObjectList(const osnmp::MibObjectList::Data &data)
{
    append(data);
}

osnmp::MibObjectList::~MibObjectList()
{
    qDeleteAll(data);
}

void osnmp::MibObjectList::append(osnmp::Mib *var)
{
    if(var == nullptr) return;
    if(var->oid().isEmpty()) return;

    //    if(var->)
    auto it = std::lower_bound(data.begin(), data.end(), var, [](Mib *v1, Mib *v2) { return *v1 < *v2; });
    if(it != data.end() && !(*var < (**it)))  // проверить есть ли такой уже..
        return;
    data.insert(it, var);
}

void osnmp::MibObjectList::append(const osnmp::MibObjectList::Data &data)
{
    for(int i = 0; i < data.size(); ++i)
        append(data.at(i));
}

void osnmp::MibObjectList::clear()
{
    qDeleteAll(data);
    data.clear();
}

osnmp::MibSearchResult osnmp::MibObjectList::find(const OID &oid, bool exact, const OIDVector &includes) const
{
    if(data.isEmpty()) return MibSearchResult();
    return find(oid, exact, 0, data.size() - 1, includes);
}

osnmp::MibSearchResult osnmp::MibObjectList::find(const OID &oid, bool exact) const
{
    return find(oid, exact, {});
}

osnmp::MibSearchResult osnmp::MibObjectList::find(const OID &oid, const OIDVector &includes) const
{
    return find(oid, false, includes);
}

osnmp::MibSearchResult osnmp::MibObjectList::firstObject(const OIDVector &includes) const
{
    for(int i = 0; i < data.size(); ++i) {  // Будем проходится с начала пока не найдем нужный нам элемент
        auto res = data.at(i)->firstObject(includes);
        if(!res.isEmpty()) return res;  // Нашли что-то
    }
    return MibSearchResult();
}

osnmp::MibSearchResult osnmp::MibObjectList::lastObject(const OIDVector &includes) const
{
    for(int i = data.size() - 1; i >= 0; --i) {  // Будем проходится с конца пока не найдем нужный нам элемент
        auto res = data.at(i)->lastObject(includes);
        if(!res.isEmpty()) return res;  // Нашли что-то
    }
    return MibSearchResult();
}

osnmp::MibSearchResult osnmp::MibObjectList::nextObject(osnmp::Mib *object, const OIDVector &includes) const
{
    if(object == nullptr) return MibSearchResult();
    auto it = std::upper_bound(data.begin(), data.end(), object, [](Mib *v1, Mib *v2) { return *v1 < *v2; });
    if(it != data.end() && !(*object > (**it))) {
        // Что-то нашли. Теперь нужно бы пройтись по всем оставшимся пока не найдем нужный нам элемент
        while(it != data.end()) {
            auto res = static_cast<Mib*>(*it)->firstObject(includes);
            if(!res.isEmpty()) return res;  // Нашли что-то
            ++it;
        }
    }
    return MibSearchResult(nullptr, true);
}

osnmp::MibSearchResult osnmp::MibObjectList::find(const OID &oid, bool exact, int left, int right, const OIDVector &includes) const
{
    if(right < left) {
        return exact ? MibSearchResult()
                     : data.at(left)->firstObject(includes);
    }

    int mid = (left + right) / 2;

    auto currentMib = data.at(mid);
    auto searchResult = currentMib->findObject(oid, exact, includes);

    if(!searchResult.isEmpty()) {
        auto mib = searchResult.mib;
        if(exact) {
            return mib->isVariable() ? searchResult
                                     : MibSearchResult();
        }
        return searchResult;
    }
    else if(searchResult.isCurrentPath) {  // Ничего не нашли, а модуль правильный..
        if(!exact && !currentMib->isVariable()) // Тк для переменной уже нашли бы следующий элемент..
            return currentMib->nextObject(nullptr, includes);    // Найдем следующий возможный вообще
        return searchResult;
    }

    if(right == left) {
        if(exact) return MibSearchResult();
        if(*currentMib > oid)
            return currentMib->firstObject(includes);
        return currentMib->nextObject(nullptr, includes);
    }

    if(*currentMib > oid)
        return find(oid, exact, left, mid - 1, includes);
    else
        return find(oid, exact, mid + 1, right, includes);
}




