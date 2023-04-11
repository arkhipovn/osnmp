#include "mibmodule.h"
#include "mibobjectlist.h"
#include "osnmp/conf/conffunctions.h"

osnmp::MibModule::MibModule(const OID &oid, AbstractAgentStrategy *strategy) : Mib(oid, strategy), tree_(new MibObjectList)
{

}

osnmp::MibModule::~MibModule()
{
    if(parent_ == nullptr) {
        // Выше уже никого нет.. Пройдемся и очистим список стратегий
        auto set = getStrategySet();
        auto it = set.begin();
        while(it != set.end()) {
            delete (*it);
            ++it;
        }
    }

    delete tree_;
}

void osnmp::MibModule::append(osnmp::Mib *object)
{
    if(object == this) return;
    object->setParent(this);
    if(object->isVariable()) object->oid() += "0";  //@N где именно это лучше сделать.. Не знаю..
    tree_->append(object);
}

void osnmp::MibModule::append(QVector<osnmp::Mib *> list)
{
    for(int i = 0; i < list.size(); ++i)
        append(list.at(i));
}

void osnmp::MibModule::clear()
{
    tree_->clear();
}

osnmp::MibSearchResult osnmp::MibModule::firstObject(const OIDVector &includes) const
{
    if(!includes.isEmpty()) {
        if(!osnmp::checkAccess(includes, absoluteOid(), false))    // Нет доступа дальше для текущего списка includes..
            return MibSearchResult(nullptr, true);
    }
    auto res = tree_->firstObject(includes);
    res.isCurrentPath = true;
    return res;
}

osnmp::MibSearchResult osnmp::MibModule::lastObject(const OIDVector &includes) const
{
    if(!includes.isEmpty()) {
        if(!osnmp::checkAccess(includes, absoluteOid(), false))    // Нет доступа дальше для текущего списка includes..
            return MibSearchResult(nullptr, true);
    }
    auto res = tree_->lastObject(includes);
    res.isCurrentPath = true;
    return res;
}

osnmp::MibSearchResult osnmp::MibModule::nextObject(Mib *object, const OIDVector &includes) const
{
    if(object == nullptr || object->parent() == this) {
        if(includes.isEmpty() || osnmp::checkAccess(includes, absoluteOid(), false)) { // Есть доступ дальше для текущего списка includes..
            auto newobject = tree_->nextObject(object, includes);
            if(!newobject.isEmpty())
                return newobject;
        }

        if(parent_ != nullptr)
            return parent_->nextObject(const_cast<MibModule*>(this), includes);
    }
    return MibSearchResult(nullptr, true);
}

QSet<osnmp::AbstractAgentStrategy *> osnmp::MibModule::getStrategySet() const
{
    QSet<AbstractAgentStrategy *> set = Mib::getStrategySet();
    for(int i = 0; i < tree_->size(); ++i) {
        auto setAt = tree_->at(i)->getStrategySet();
        auto it = setAt.begin();
        while(it != setAt.end()) {
            set.insert(*it);
            ++it;
        }
    }
    return set;
}

osnmp::MibSearchResult osnmp::MibModule::findObject(const OID &oid, bool exact, const OIDVector &includes) const
{
    int compare = this->oid_.compare(oid);
    if(compare == this->oid_.size()) {
        // является родителем
        if(compare == oid.size()) {
            // Если размеры совпадают
            if(exact) return MibSearchResult(nullptr, true);     // Точно ничего не найдем.. Это же не объект конечный, а модуль
            return this->firstObject(includes);
        }

        // Проверим, а доступна ли текущая ветка нам?
        if(!includes.isEmpty()) {
            if(!osnmp::checkAccess(includes, absoluteOid(), false))    // Нет доступа дальше для текущего списка includes..
                return MibSearchResult(nullptr, true);
        }

        return tree_->find(oid.mid(compare), exact, includes);
    }
    else if(!exact) {
        if(compare == oid.size())  // Полностью совпал размер. Явно что-то не так..
            return firstObject(includes);
        else if(parent_ == nullptr) { // Это только для случае, если выше ничего нет, то нужно самый первый вернуть
            if(oid < this->oid_) // Нужно вернуть самый первый элемент получается
                return firstObject(includes);
        }
    }

    return MibSearchResult();   // Ветка отличная от oid
}
