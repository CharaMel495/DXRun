#pragma once

namespace CaramelEngine
{
	/// <summary>
	/// 当たり判定に責任を持つクラス
	/// </summary>
	class CollisionTester final : public IColliderVisitor
	{
	private:

		template <class L>
		bool testCollision(const L& lhs)
		{
			switch (_other->getColliderType())
			{
			case eColliderType::Rectangle:
				return testCollisionImpl(lhs, *static_cast<CRectangle*>(_other));
			case eColliderType::Circle:
				return testCollisionImpl(lhs, *static_cast<CCircle*>(_other));
			case eColliderType::Cube:
				return testCollisionImpl(lhs, *static_cast<CCube*>(_other));
			case eColliderType::Sphere:
				return testCollisionImpl(lhs, *static_cast<CSphere*>(_other));
			case eColliderType::Ray:
				return testCollisionImpl(lhs, *static_cast<CRay*>(_other));
			default:
				return false;
			}
		}

		template <class L, class R>
		bool testCollisionImpl(const L& lhs, const R& rhs)
		{
			// 2D専用ルート
			if constexpr (ShapeTraits<L>::Is2D && ShapeTraits<R>::Is2D)
				return test2D(lhs, rhs);

			// Ray 特例
			if constexpr (ShapeTraits<L>::IsRay)
				return testRay(lhs, rhs);
			if constexpr (ShapeTraits<R>::IsRay)
				return testRay(rhs, lhs);

			// BroadPhase
			auto aBroad = ShapeTraits<L>::asBroadSphere(lhs);
			auto bBroad = ShapeTraits<R>::asBroadSphere(rhs);

			if (!testSphereVsSphere(aBroad, bBroad))
				return false;

			// NarrowPhase
			return test3D(lhs, rhs);
		}

	public:

		CollisionTester() = delete;

		CollisionTester(ICollider* other) : 
			_other(other),
			_result(false)
		{}

		void visit(CRectangle& lhs) noexcept override
		{
			_result = testCollision(lhs);
		}

		void visit(CCircle& lhs) noexcept override
		{
			_result = testCollision(lhs);
		}

		void visit(CCube& lhs) noexcept override
		{
			_result = testCollision(lhs);
		}

		void visit(CSphere& lhs) noexcept override
		{
			_result = testCollision(lhs);
		}

		void visit(CRay& lhs) noexcept override
		{
			_result = testCollision(lhs);
		}

		bool getResult() const noexcept { return _result; }

	private:

		// ヘルパー構造体メソッド
		static CubeHitJudgeData MakeOBBFromRectangle(const CRectangle& rect)
		{
			auto data = rect.getHitJudgeData();
			auto transform = data.Transform;

			CubeHitJudgeData obb{};
			obb.Center = transform->getPosition();

			obb.Axis[0] = transform->getRight().normalized();
			obb.Axis[1] = transform->getUp().normalized();
			obb.Axis[2] = { 0.0f, 0.0f, 1.0f }; // Z固定

			CVector3 scale = transform->getScale();
			obb.HalfExtent = CVector3(
				scale.getX() * 0.5f,
				scale.getY() * 0.5f,
				0.0f   // 厚みなし
			);

			return obb;
		}

		ICollider* _other;
		bool _result;

		// 判定関数
		// 2D判定
		bool testRectangleVsRectangle(const CRectangle& a, const CRectangle& b) noexcept;
		bool testRectangleVsCircle(const CRectangle& a, const CCircle& b) noexcept;
		bool testCircleVsCircle(const CCircle& a, const CCircle& b) noexcept;
		// 3D判定
		bool testOBBVsOBB(const CubeHitJudgeData& a, const CubeHitJudgeData& b) noexcept;
		bool testOBBVsSphere(const CubeHitJudgeData& cube, const SphereHitJudgeData& sphere) noexcept;
		bool testRayVsOBB(const RayHitJudgeData& ray, const CubeHitJudgeData& cube) noexcept;
		bool testSphereVsSphere(const SphereHitJudgeData& a, const SphereHitJudgeData& b) noexcept;
		bool testRayVsSphere(const RayHitJudgeData& ray, const SphereHitJudgeData& sphere) noexcept;
		
		// 各次元で判定を行う本体

		template <class L, class R>
		bool test2D(const L& lhs, const R& rhs)
		{
			if constexpr (std::is_same_v<L, CRectangle> && std::is_same_v<R, CRectangle>)
				return testRectangleVsRectangle(lhs, rhs);

			if constexpr (std::is_same_v<L, CRectangle> && std::is_same_v<R, CCircle>)
				return testRectangleVsCircle(lhs, rhs);

			if constexpr (std::is_same_v<L, CCircle> && std::is_same_v<R, CRectangle>)
				return testRectangleVsCircle(rhs, lhs);

			if constexpr (std::is_same_v<L, CCircle> && std::is_same_v<R, CCircle>)
				return testCircleVsCircle(lhs, rhs);

			return false;
		}

		template <class L, class R>
		bool test3D(const L& lhs, const R& rhs)
		{
			// OBB vs OBB
			if constexpr (ShapeTraits<L>::HasOBB && ShapeTraits<R>::HasOBB)
			{
				return testOBBVsOBB(
					ShapeTraits<L>::asOBB(lhs),
					ShapeTraits<R>::asOBB(rhs)
				);
			}

			// OBB vs Sphere
			if constexpr (ShapeTraits<L>::HasOBB && ShapeTraits<R>::HasSphere)
			{
				return testOBBVsSphere(
					ShapeTraits<L>::asOBB(lhs),
					ShapeTraits<R>::asSphere(rhs)
				);
			}

			if constexpr (ShapeTraits<L>::HasSphere && ShapeTraits<R>::HasOBB)
			{
				return testOBBVsSphere(
					ShapeTraits<R>::asOBB(rhs),
					ShapeTraits<L>::asSphere(lhs)
				);
			}

			// Sphere vs Sphere
			if constexpr (ShapeTraits<L>::HasSphere && ShapeTraits<R>::HasSphere)
			{
				return testSphereVsSphere(
					ShapeTraits<L>::asSphere(lhs),
					ShapeTraits<R>::asSphere(rhs)
				);
			}

			return false;
		}

		template <class L, class R>
		bool testRay(const L& lhs, const R& rhs)
		{
			// lhs が Ray の場合だけ通る想定
			// rhs が Ray の場合は対称にして突っ込む
			if constexpr (!ShapeTraits<L>::IsRay)
				return false;

			if constexpr (ShapeTraits<R>::HasOBB)
			{
				return testRayVsOBB(
					ShapeTraits<L>::asRay(lhs),
					ShapeTraits<R>::asOBB(rhs)
				);
			}

			if constexpr (ShapeTraits<R>::HasSphere)
			{
				return testRayVsSphere(
					ShapeTraits<L>::asRay(lhs),
					ShapeTraits<R>::asSphere(rhs)
				);
			}

			return false;
		}
	};
}