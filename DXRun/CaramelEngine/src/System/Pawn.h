#pragma once

namespace CaramelEngine
{
	/// <summary>
	/// プレイヤーやAIによって制御可能なアクタ
	/// </summary>
	class Pawn : public Actor
	{
	public:

		inline Pawn(std::shared_ptr<Transform> transform, CString name) noexcept :
			Actor(transform, name)
		{}

		virtual inline ~Pawn() {}

	private:
	};
}