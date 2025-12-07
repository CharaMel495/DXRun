#pragma once

namespace CaramelEngine
{
	/// <summary>
	/// ゲームで登場するあらゆるクラス(データクラス等例外除く)が
	/// 継承するべき基底クラス
	/// </summary>
	class MonoBehaviour
	{
	public:

		inline MonoBehaviour() noexcept {}

		inline MonoBehaviour(const MonoBehaviour& copyBase) noexcept {}

		virtual inline ~MonoBehaviour() noexcept {}

		/// <summary>
		/// 初期化処理
		/// </summary>
		virtual void initialize() noexcept = 0;

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void update() noexcept = 0;

		/// <summary>
		/// 固定フレーム更新処理
		/// </summary>
		virtual void fixedUpdate() noexcept = 0;

	private:

	};
}