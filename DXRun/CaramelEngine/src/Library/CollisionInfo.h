#pragma once
#include <cstdint>

namespace CaramelEngine
{
    /// <summary>
    /// オブジェクト同士が衝突した時用の情報をパッキングしたクラス
    /// </summary>
    struct CollisionInfo
    {
        void* self = nullptr;
        void* other = nullptr;
        uint32_t groupSelf = 0;
        uint32_t groupOther = 0;

        // 追加したい場合：
        // float damage = 0.0f;
        // CVector3 contactPoint;
        // bool wasTrigger = false;
    };
}
