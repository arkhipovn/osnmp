#include "mibobject.h"
#include "osnmp/agent/strategy/abstractagenttablestrategy.h"
#include "osnmp/conf/conffunctions.h"

osnmp::MibObject::MibObject(quint16 valueId, Type type, ObjectAccess access, const OID &oid, AbstractAgentStrategy *strategy) : Mib(oid, strategy),
    valueId_(valueId), type_(type), access_(access)
{

}

osnmp::MibObject::~MibObject()
{
    if(parent_ == nullptr && strategy_ != nullptr) // Выше уже никого нет.. Пройдемся и очистим список стратегий
        delete strategy_;
}

quint16 osnmp::MibObject::valueId() const
{
    return valueId_;
}

osnmp::Type osnmp::MibObject::type() const
{
    return type_;
}

osnmp::ObjectAccess osnmp::MibObject::access() const
{
    return access_;
}

osnmp::MibSearchResult osnmp::MibObject::firstObject(const OIDVector &includes) const
{    
    if(!includes.isEmpty()) {
        if(!osnmp::checkAccess(includes, absoluteOid(), false))    // Нет доступа дальше для текущего списка includes..
            return MibSearchResult(nullptr, true);
    }

    if(parent_ != nullptr) {
        if(parent_->isTable()) {
            // Текущий объект является колонкой в таблице
            auto st = static_cast<AbstractAgentTableStrategy*>(strategy());
            if(st == nullptr) return MibSearchResult();
            auto index = st->firstTableRow(valueId_);
            if(index.isNull()) return MibSearchResult();
            return MibSearchResult(const_cast<MibObject*>(this), true, index);
        }
    }

    return MibSearchResult(const_cast<MibObject*>(this), true);
}

osnmp::MibSearchResult osnmp::MibObject::lastObject(const OIDVector &includes) const
{
    if(!includes.isEmpty()) {
        if(!osnmp::checkAccess(includes, absoluteOid(), false))    // Нет доступа дальше для текущего списка includes..
            return MibSearchResult(nullptr, true);
    }

    if(parent_ != nullptr) {
        if(parent_->isTable()) {
            // Текущий объект является колонкой в таблице
            auto st = static_cast<AbstractAgentTableStrategy*>(strategy());
            if(st == nullptr) return MibSearchResult();
            auto index = st->lastTableRow(valueId_);
            if(index.isNull()) return MibSearchResult();
            return MibSearchResult(const_cast<MibObject*>(this), true, index);
        }
    }

    return MibSearchResult(const_cast<MibObject*>(this), true);
}

osnmp::MibSearchResult osnmp::MibObject::nextObject(Mib *object, const OIDVector &includes) const
{
    if(object == nullptr || object == this) {
        if(parent_ != nullptr) {
            if(parent_->isTable()) {
                // В этом случае нужно сначала проверить можем ли мы брать данные из этой таблицы..
                if(includes.isEmpty() || osnmp::checkAccess(includes, absoluteOid(), false)) { // Есть доступ дальше для текущего списка includes..
                    // Текущий объект является колонкой в таблице
                    auto st = static_cast<AbstractAgentTableStrategy*>(strategy());
                    if(st == nullptr) return MibSearchResult();
                    auto index = st->firstTableRow(valueId_);
                    if(!index.isNull())
                        MibSearchResult(const_cast<MibObject*>(this), true, index);
                }
            }

            // Нужно вернуться назад.. И найти следующий элемент
            return parent_->nextObject(const_cast<MibObject*>(this), includes);
        }
    }
    return MibSearchResult(nullptr, true);
}

osnmp::MibSearchResult osnmp::MibObject::findObject(const OID &oid, bool exact, const OIDVector &includes) const
{
    int compare = this->oid_.compare(oid);
    if(compare == this->oid_.size()) {
        // Полностью совпал поиск. Проверим найденый элемент является таблицей?
        if(parent_ != nullptr) {
            if(parent_->isTable()) {
                // Текущий объект является колонкой в таблице
                auto st = static_cast<AbstractAgentTableStrategy*>(strategy());
                if(st == nullptr) {
                    return exact ? MibSearchResult(nullptr, true)
                                 : this->nextObject(nullptr, includes);
                }

                // Нужно найти в колонках
                auto index = oid.mid(compare);

                if(exact && index.isEmpty()) {  // Нашли точное совпадение и это указывают на таблицу. А в таблице еще должны быть индексы..
                    return exact ? MibSearchResult(nullptr, true)
                                 : this->nextObject(nullptr, includes);
                }

                // Не точное совпадение
                index = st->findTableRow(valueId_, index, exact);
                if(index.isNull()) {
                    return exact ? MibSearchResult(nullptr, true)
                                 : this->nextObject(nullptr, includes);
                }

                return { const_cast<MibObject*>(this), true, index };
            }
        }

        // Похоже не является таблицей
        if(this->oid_.size() == oid.size()) {
            if(!exact) return this->nextObject(nullptr, includes);  // Нужен следующий элемент
            return { const_cast<MibObject*>(this), true };          // Нужен текущий
        }
    }
    else if(!exact) {
        if(compare == oid.size())  // Полностью совпал размер. Явно что-то не так..
            return firstObject(includes);
        else if(parent_ == nullptr) { // Это только для случае, если выше ничего нет, то нужно самый первый вернуть
            if(oid < this->oid_) // Нужно вернуть самый первый элемент получается
                return firstObject(includes);
        }
    }

    return MibSearchResult(nullptr, false);
}

