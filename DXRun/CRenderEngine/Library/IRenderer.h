#pragma once
#include "CaramelEngine.h"

namespace CaramelEngine
{
	/// <summary>
	/// モデルやスプライト等の描画機能を提供する
	/// コンポーネントのインターフェース
	/// </summary>
	class IRenderer : public Component
	{
	public:

		IRenderer() = delete;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline IRenderer(std::weak_ptr<Actor> owner, CString name) noexcept :
			Component(owner, name)
		{}
		virtual ~IRenderer() noexcept = default;

		/// <summary>
		/// RenderQueueに描画依頼を要求するメソッド
		/// </summary>
		virtual void render(class RenderQueue& queue) noexcept = 0;

	private:
	};
}