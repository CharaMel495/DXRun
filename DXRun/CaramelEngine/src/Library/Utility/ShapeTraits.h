#pragma once
#include "CVector3.h"
#include "CRectangle.h"
#include "CCircle.h"
#include "CCube.h"
#include "CSphere.h"
#include "CRay.h"

namespace CaramelEngine
{
	template<class T>
    struct ShapeTraits
    {
        static constexpr bool Is2D = false;
        static constexpr bool IsRay = false;

        static constexpr bool HasOBB = false;
        static constexpr bool HasSphere = false;

        static SphereHitJudgeData asBroadSphere(const T&)
        {
            static_assert(sizeof(T) == 0, "ShapeTraits not specialized");
            return {};
        }
    };

	template<>
	struct ShapeTraits<CRectangle>
	{
        static constexpr bool Is2D = true;
        static constexpr bool IsRay = false;

        static constexpr bool HasOBB = true;
        static constexpr bool HasSphere = false;

        static SphereHitJudgeData asBroadSphere(const CRectangle& r)
        {
            auto data = r.getHitJudgeData();
            auto scale = data.Transform->getScale();

            float radius =
                std::sqrt(
                    (scale.getX() * 0.5f) * (scale.getX() * 0.5f) +
                    (scale.getY() * 0.5f) * (scale.getY() * 0.5f)
                );

            return { data.Transform->getPosition(), radius };
        }

        static CubeHitJudgeData asOBB(const CRectangle& r)
        {
            auto data = r.getHitJudgeData();
            auto t = data.Transform;

            CubeHitJudgeData obb{};
            obb.Center = t->getPosition();

            obb.Axis[0] = t->getRight().normalized();
            obb.Axis[1] = t->getUp().normalized();
            obb.Axis[2] = { 0.0f, 0.0f, 1.0f }; // Z固定

            auto scale = t->getScale();
            obb.HalfExtent = 
            {
                scale.getX() * 0.5f,
                scale.getY() * 0.5f,
                0.0f
            };

            return obb;
        }
	};

	template<>
	struct ShapeTraits<CCircle>
	{
        static constexpr bool Is2D = true;
        static constexpr bool IsRay = false;

        static constexpr bool HasOBB = false;
        static constexpr bool HasSphere = true;

        // 下記asSphereとやってることはいっしょだが、便宜上
        // シャローフェーズ用のメソッドとして
        static SphereHitJudgeData asBroadSphere(const CCircle& c)
        {
            auto data = c.getHitJudgeData();
            return { data.Transform->getPosition(), data.Radius };
        }

        static SphereHitJudgeData asSphere(const CCircle& c)
        {
            auto data = c.getHitJudgeData();
            return { data.Transform->getPosition(), data.Radius };
        }
	};

	template<>
	struct ShapeTraits<CCube>
	{
        static constexpr bool Is2D = false;
        static constexpr bool IsRay = false;

        static constexpr bool HasOBB = true;
        static constexpr bool HasSphere = false;

        static SphereHitJudgeData asBroadSphere(const CCube& c)
        {
            auto data = c.getHitJudgeData();
            float r =
                std::sqrt(
                    data.HalfExtent.getX() * data.HalfExtent.getX() +
                    data.HalfExtent.getY() * data.HalfExtent.getY() +
                    data.HalfExtent.getZ() * data.HalfExtent.getZ()
                );

            return { data.Transform->getPosition(), r };
        }

        static const CubeHitJudgeData& asOBB(const CCube& c)
        {
            return c.getHitJudgeData();
        }
	};

	template<>
	struct ShapeTraits<CSphere>
	{
        static constexpr bool Is2D = false;
        static constexpr bool IsRay = false;

        static constexpr bool HasOBB = false;
        static constexpr bool HasSphere = true;

        static SphereHitJudgeData asBroadSphere(const CSphere& s)
        {
            return s.getHitJudgeData();
        }

        static SphereHitJudgeData asSphere(const CSphere& s)
        {
            return s.getHitJudgeData();
        }
	};

	template<>
	struct ShapeTraits<CRay>
	{
        static constexpr bool Is2D = false;
        static constexpr bool IsRay = true;

        static constexpr bool HasOBB = false;
        static constexpr bool HasSphere = false;

        static SphereHitJudgeData asBroadSphere(const CRay& s)
        {
            return {};
        }

        static RayHitJudgeData asRay(const CRay& r)
        {
            return r.getHitJudgeData();
        }
	};
}