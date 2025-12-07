#include "pch.h"

using namespace CaramelEngine;

void ColliderManagerBase::registerObject(uint32_t groupId, std::shared_ptr<ICollisionObject> obj)
{
    _groupMap[groupId].emplace_back(obj);
}

void ColliderManagerBase::unregisterObject(std::shared_ptr<ICollisionObject> obj) noexcept
{
    _pendingRemovals.emplace_back(obj);
}

void ColliderManagerBase::update()
{
    const auto& pairs = getCollisionPairs();

    for (const auto& [groupA, groupB] : pairs)
    {
        auto& listA = _groupMap[groupA];
        auto& listB = _groupMap[groupB];

        for (const auto& objA : listA)
        {
            for (const auto& objB : listB)
            {
                if (objA == objB)
                    continue;

                auto testCollider = objB->getCollider();
                if (!testCollider)
                    continue;

                CollisionTester tester(testCollider);
                objA->getCollider()->accept(tester);

                if (tester.getResult())
                {
                    CollisionKey key{ objA, objB };
                    _currentCollisions.insert(key);

                    bool wasColliding = _prevCollisions.contains(key);

                    if (wasColliding)
                        handleCollisionStayEvent(objA.get(), groupA, objB.get(), groupB);
                    else
                        handleCollisionEnterEvent(objA.get(), groupA, objB.get(), groupB);
                }
            }
        }
    }

    for (auto& obj : _pendingRemovals)
    {
        uint32_t groupId = obj->getCollisionGroupID();

        auto it = _groupMap.find(groupId);
        if (it == _groupMap.end()) continue;

        auto& vec = it->second;
        std::erase_if(vec, [obj](const std::shared_ptr<ICollisionObject>& o) { return o == obj; });
    }

    _pendingRemovals.clear();

    for (const auto& key : _prevCollisions)
    {
        if (!_currentCollisions.contains(key))
        {
            auto objA = key.a.lock();
            auto objB = key.b.lock();

            if (objA && objB)
            {
                handleCollisionExitEvent(objA.get(), objA->getCollisionGroupID(), objB.get(), objB->getCollisionGroupID());
            }
        }
    }

    _prevCollisions = std::move(_currentCollisions);
    _currentCollisions.clear();
}

void ColliderManagerBase::handleCollisionEnterEvent(ICollisionObject* self, uint32_t groupSelf, ICollisionObject* other, uint32_t groupOther)
{
    if (!_dispatcher)
        return;

    CollisionInfo infoA = { self->getOwnerPointer(), other->getOwnerPointer(), groupSelf, groupOther };
    CollisionInfo infoB = { other->getOwnerPointer(), self->getOwnerPointer(), groupOther, groupSelf };

    _dispatcher->dispatchEvent(self->getOnCollisionEnterEventKey(), &infoA);
    _dispatcher->dispatchEvent(other->getOnCollisionEnterEventKey(), &infoB);
}

void ColliderManagerBase::handleCollisionStayEvent(ICollisionObject* self, uint32_t groupSelf, ICollisionObject* other, uint32_t groupOther)
{
    if (!_dispatcher)
        return;

    CollisionInfo infoA = { self->getOwnerPointer(), other->getOwnerPointer(), groupSelf, groupOther };
    CollisionInfo infoB = { other->getOwnerPointer(), self->getOwnerPointer(), groupOther, groupSelf };

    _dispatcher->dispatchEvent(self->getOnCollisionStayEventKey(), &infoA);
    _dispatcher->dispatchEvent(other->getOnCollisionStayEventKey(), &infoB);
}

void ColliderManagerBase::handleCollisionExitEvent(ICollisionObject* self, uint32_t groupSelf, ICollisionObject* other, uint32_t groupOther)
{
    if (!_dispatcher)
        return;

    CollisionInfo infoA = { self->getOwnerPointer(), other->getOwnerPointer(), groupSelf, groupOther };
    CollisionInfo infoB = { other->getOwnerPointer(), self->getOwnerPointer(), groupOther, groupSelf };

    _dispatcher->dispatchEvent(self->getOnCollisionExitEventKey(), &infoA);
    _dispatcher->dispatchEvent(other->getOnCollisionExitEventKey(), &infoB);
}
