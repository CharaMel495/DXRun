#pragma once
#include "ICollider.h"
#include "IColliderVisitor.h"

namespace CaramelEngine
{
    struct RectHitJudgeData
    {
        std::shared_ptr<Transform> Transform;
        Cvector<CVector3> Verts;
    };

    /// <summary>
    /// SIMD最適化された当たり判定矩形クラス (コンポーネント継承版)
    /// </summary>
    class CRectangle : public Component, public ICollider
    {
    public:

        inline CRectangle() noexcept = delete;

        // コンストラクタ
        CRectangle(std::weak_ptr<Actor> owner) noexcept :
            Component(owner, "CRectangle"),
            _transform(),
            _localScale_X(),
            _localScale_Y()
        {
        }

        // コピーコンストラクタ
        CRectangle(const CRectangle& other) noexcept :
            Component(other._owner, other._name),
            _transform(other._transform),
            _localScale_X(other._localScale_X),
            _localScale_Y(other._localScale_Y)

        {
        }

        ~CRectangle() {
            int hoge = 0;
        }

        // 初期化メソッド (Componentからの継承)
        void initialize() noexcept override
        {
            _verts.clear();
            _verts.emplace_back(CVector3());
            _verts.emplace_back(CVector3());
            _verts.emplace_back(CVector3());
            _verts.emplace_back(CVector3());
            updateScale();
            updateVerts();
        }

        /// <summary>
        /// 最新の頂点の座標をSIMDを使って取得する関数
        /// 左上から時計周りになるように取得
        /// </summary>
        void updateVerts()
        {
            auto vectors = GetDirectionVector();
            CVector3 pos = _transform->getPosition();
            _verts[0] = vectors[0] + pos;
            _verts[1] = vectors[1] + pos;
            _verts[2] = vectors[2] + pos;
            _verts[3] = vectors[3] + pos;
        }

        /// <summary>
        /// 最新の大きさを取得する関数
        /// </summary>
        void updateScale()
        {
            _localScale_X = _transform->getScale().getX() * 0.5f;
            _localScale_Y = _transform->getScale().getY() * 0.5f;
        }

        void update() noexcept
        {
            updateVerts();
            updateScale();
        }

        /// <summary>
        /// 矩形同士の衝突判定
        /// </summary>
        bool HitJudge(CRectangle otherRect)
        {
            //頂点情報を更新
            updateVerts();
            otherRect.updateVerts();

            //頂点情報をキャッシュ
            auto vertsA = this->_verts;
            auto vertsB = otherRect._verts;

            //判定に用いる分離軸の候補を用意
            CVector3 sideDirs[4] =
            {
                (vertsA[0] - vertsA[1]).normalized(), (vertsA[1] - vertsA[2]).normalized(),
                (vertsB[0] - vertsB[1]).normalized(), (vertsB[1] - vertsB[2]).normalized()
            };

            //全ての分離軸の可能性を見ていく
            for (auto& u : sideDirs)
            {
                //頂点の内、軸に射影した物の最大と最小を求める
                float minA = vertsA[0].dot(u);
                float maxA = minA;
                float minB = vertsB[0].dot(u);
                float maxB = minB;

                for (int i = 1; i < 4; i++)
                {
                    float projA = vertsA[i].dot(u);
                    minA = (std::min)(minA, projA);
                    maxA = (std::max)(maxA, projA);

                    float projB = vertsB[i].dot(u);
                    minB = (std::min)(minB, projB);
                    maxB = (std::max)(maxB, projB);
                }

                //もし分離軸が存在するなら当たってない
                if (maxB < minA || maxA < minB)
                    return false;
            }
            //もし、分離軸が存在しないなら当たってる
            return true;
        }

        void setTransform(std::shared_ptr<Transform> transform) 
        { 
            _transform = transform; 
            auto scale = _transform->getScale();
            _localScale_X = scale.getX() * 0.5f;
            _localScale_Y = scale.getY() * 0.5f;
        }

        void accept(IColliderVisitor& visitor) override 
        {
            visitor.visit(*this); // 自分を訪問させる
        }

        RectHitJudgeData getHitJudgeData() const noexcept
        {
            return
            {
                _transform,
                _verts
            };
        }
        
    private:
        std::shared_ptr<Transform> _transform;
        Cvector<CVector3> _verts;
        float _localScale_X;
        float _localScale_Y;

        CVector3* GetDirectionVector()
        {
            CVector3 right = _transform->getRight();
            CVector3 up = _transform->getUp();
            CVector3 scaleX = right * _localScale_X;
            CVector3 scaleY = up * _localScale_Y;

            CVector3 upLeft = -scaleX + scaleY;
            CVector3 upRight = scaleX + scaleY;
            CVector3 downRight = scaleX - scaleY;
            CVector3 downLeft = -scaleX - scaleY;

            static CVector3 retruner[4] = { upLeft, upRight, downRight, downLeft };
            return retruner;
        }
    };
}