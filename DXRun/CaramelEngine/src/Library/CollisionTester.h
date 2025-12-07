#pragma once

namespace CaramelEngine
{
//キャストして判定を取って結果を記録するマクロ
#define COLLISION_CASE(TargetType, FuncName)			 \
	if (auto* casted = dynamic_cast<TargetType*>(_other))\
	{													 \
		_result = FuncName(lhs, *casted);				 \
		return;											 \
	}													 \

#define COLLISION_RECT_CASE(Func) COLLISION_CASE(CRectangle, Func)
#define COLLISION_CIRC_CASE(Func) COLLISION_CASE(CCircle, Func)

	/// <summary>
	/// 当たり判定に責任を持つクラス
	/// </summary>
	class CollisionTester final : public IColliderVisitor
	{
	public:

		CollisionTester() = delete;

		CollisionTester(ICollider* other) : 
			_other(other),
			_result(false)
		{}

		void visit(CRectangle& lhs) noexcept
		{
			COLLISION_RECT_CASE(testRectangleVsRectangle);
			COLLISION_CIRC_CASE(testRectangleVsCircle);
		}

		void visit(CCircle& lhs) noexcept
		{
			COLLISION_RECT_CASE(testCircleVsRectangle);
			COLLISION_CIRC_CASE(testCircleVsCircle);
		}

		bool getResult() const noexcept { return _result; }

	private:

		ICollider* _other;
		bool _result;

		// 判定関数（仮実装）
		bool testRectangleVsRectangle(const CRectangle& a, const CRectangle& b) noexcept;
		bool testRectangleVsCircle(const CRectangle& a, const CCircle& b) noexcept;
		bool testCircleVsCircle(const CCircle& a, const CCircle& b) noexcept;
		//マクロを統一するため、対称性の利用をした関数
		bool testCircleVsRectangle(const CCircle& a, const CRectangle& b) noexcept { return testRectangleVsCircle(b, a); }
	};
}