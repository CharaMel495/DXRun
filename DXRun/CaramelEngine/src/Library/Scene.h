#pragma once
#include "MonoBehaviour.h"
#include "Actor.h"


//class Actor;

namespace CaramelEngine
{
	/// <summary>
	/// このゲームにおけるシーンクラスの基底
	/// </summary>
	class Scene : public MonoBehaviour
	{
	public:

		inline Scene() noexcept = default;

		virtual inline ~Scene() = default;

		//inline Scene(const Scene& scene) noexcept {}

		/// <summary>
		/// シーンにアクタを追加するメソッド
		/// </summary>
		/// <param name="newActor">追加したいアクタ</param>
		virtual void addActor(std::shared_ptr<Actor> newActor) noexcept;

		/// <summary>
		/// シーンからアクタを排除するメソッド
		/// </summary>
		/// <param name="actor"></param>
		virtual void removeActor(const std::shared_ptr<Actor>& actor) noexcept;

		virtual void shutdown() noexcept = 0;

		std::shared_ptr<CameraBase> getMainCamera() noexcept { return _mainCam; }

	protected:

		std::shared_ptr<CameraBase> _mainCam;

		/// <summary>
		/// このシーンに登場するアクター
		/// </summary>
		Cvector<std::shared_ptr<Actor>> _actors;

		/// <summary>
		/// 配列に登録待ちのアクター
		/// </summary>
		Cvector<std::shared_ptr<Actor>> _pendingAdditions;

		Cvector<std::shared_ptr<Actor>> _pendingRemovals;

		/// <summary>
		/// 登録待ちのアクタを取り出して本配列に登録する
		/// </summary>
		void flushPendingActors() noexcept;

		/// <summary>
		/// 削除待ちのアクタを取り出して本配列から取り除く
		/// </summary>
		void flushPendingRemovals() noexcept;
	};
}