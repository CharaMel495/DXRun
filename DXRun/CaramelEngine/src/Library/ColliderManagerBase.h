#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

namespace CaramelEngine
{
    class ICollisionObject;
    class EventDispatcher;

    struct CollisionInfo;

    // unordered_set で pair を使うためのハッシュ構造体
    struct PairHash
    {
        std::size_t operator()(const std::pair<uint32_t, uint32_t>& p) const
        {
            return std::hash<uint32_t>()(p.first) ^ (std::hash<uint32_t>()(p.second) << 1);
        }
    };

    // 衝突ペアを weak_ptr で安全に保持する構造体
    struct CollisionKey
    {
        std::weak_ptr<ICollisionObject> a;
        std::weak_ptr<ICollisionObject> b;

        bool operator==(const CollisionKey& other) const
        {
            auto la = a.lock();
            auto lb = b.lock();
            auto ra = other.a.lock();
            auto rb = other.b.lock();
            return la == ra && lb == rb;
        }
    };

    struct CollisionKeyHash
    {
        std::size_t operator()(const CollisionKey& key) const
        {
            auto la = key.a.lock();
            auto lb = key.b.lock();
            std::size_t h1 = std::hash<void*>{}(la.get());
            std::size_t h2 = std::hash<void*>{}(lb.get());
            return h1 ^ (h2 << 1);
        }
    };

    class ColliderManagerBase
    {
    public:
        inline ColliderManagerBase() noexcept = delete;

        inline ColliderManagerBase(EventDispatcher* dispatcher) noexcept :
            _dispatcher(dispatcher) {
        }

        virtual ~ColliderManagerBase() = default;

        virtual void initialize() noexcept = 0;
        virtual void update();

        void registerObject(uint32_t groupId, std::shared_ptr<ICollisionObject> obj);
        void unregisterObject(std::shared_ptr<ICollisionObject> obj) noexcept;

    protected:
        virtual const std::unordered_set<std::pair<uint32_t, uint32_t>, PairHash>& getCollisionPairs() const = 0;

        virtual void handleCollisionEnterEvent(ICollisionObject* self, uint32_t groupSelf, ICollisionObject* other, uint32_t groupOther);
        virtual void handleCollisionStayEvent(ICollisionObject* self, uint32_t groupSelf, ICollisionObject* other, uint32_t groupOther);
        virtual void handleCollisionExitEvent(ICollisionObject* self, uint32_t groupSelf, ICollisionObject* other, uint32_t groupOther);

        EventDispatcher* _dispatcher = nullptr;

        std::unordered_map<uint32_t, std::vector<std::shared_ptr<ICollisionObject>>> _groupMap;

        std::unordered_set<CollisionKey, CollisionKeyHash> _prevCollisions;
        std::unordered_set<CollisionKey, CollisionKeyHash> _currentCollisions;

        std::vector<std::shared_ptr<ICollisionObject>> _pendingRemovals;
    };
}
