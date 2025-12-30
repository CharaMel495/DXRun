#pragma once
#include "ComponentHolder.h"
#include "ICollisionObject.h"

namespace
{
	/// <summary>
	/// ヒットイベント用構造体
	/// </summary>
	struct HitEvent
	{
		int Damage;
		std::any From;
		std::any To;
	};
}


namespace CaramelEngine
{
	/// <summary>
	/// 継承して自由に拡張できてシーン上に配置できるクラス
	/// </summary>
	class Actor : public CaramelEngine::MonoBehaviour, public std::enable_shared_from_this<Actor>
	{
	public:

		/// <summary>
		/// デフォルトコンストラクタは使用禁止
		/// </summary>
		inline Actor() = delete;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline Actor(std::shared_ptr<Transform> transform, const CString name) noexcept :
			MonoBehaviour(), 
			_transform(transform), 
			_actorName(name),
			_destroyFlag(false)
		{}

		/// <summary>
		/// コピーコンストラクタ
		/// </summary>
		/// <param name="copyBase"></param>
		Actor(const Actor& copyBase) noexcept : 
			MonoBehaviour(), 
			_transform(copyBase._transform),
			_actorName(copyBase._actorName),
			_destroyFlag(copyBase._destroyFlag)
		{}

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual inline ~Actor() noexcept override {}

		/// <summary>
		/// 自身を破棄する(破棄を予約する)関数
		/// </summary>
		virtual void destroy() noexcept = 0;

		virtual bool isInScreen() const noexcept;

		/// <summary>
		/// コンポーネントを追加するメソッド
		/// </summary>
		/// <typeparam name="TComponent">追加するコンポーネント</typeparam>
		/// <returns>追加したコンポーネントの参照</returns>
		template <ComponentTraits::ConmponentCreatable TComponent>
		TComponent* addComponent() 
		{
			auto holder = std::make_unique<ComponentHolder<TComponent>>(shared_from_this());
			TComponent* ptr = holder->getIf<TComponent>();
			_myComponents.emplace_back(std::move(holder));
			return ptr;
		}

		/// <summary>
		/// 渡されたクラスのコンポーネントを探すメソッド
		/// </summary>
		/// <typeparam name="TComponent">探したいコンポーネント</typeparam>
		/// <returns>見つかればそのインスタンス、なければnullptr</returns>
		template <ComponentTraits::ConmponentCreatable TComponent>
		TComponent* findComponent() const
		{
			for (auto& holder : _myComponents) 
			{
				if (holder->type() == typeid(TComponent))
					return static_cast<TComponent*>(holder->rawPtr());
			}

			return nullptr;
		}

		std::shared_ptr<Transform>& getTransform() noexcept { return _transform; }
		void setTransform(std::shared_ptr<Transform> newTransform) noexcept { _transform = newTransform; }

		CString getActorName() const noexcept { return _actorName; }
		void setActorName(CString newName) noexcept { _actorName = newName; }

		bool getDestroyFlag() const noexcept { return _destroyFlag; }

		virtual std::shared_ptr<ICollisionObject> getColliderInterface() { return nullptr; }

	protected:

		/// <summary>
		/// 自身のトランスフォーム
		/// 移動やスケーリングはある程度自由にしたいため、mutable指定
		/// </summary>
		mutable std::shared_ptr<Transform> _transform;

		/// <summary>
		/// アクタ名
		/// </summary>
		CString _actorName;

		/// <summary>
		/// 破棄フラグ
		/// </summary>
		bool _destroyFlag;

		/// <summary>
		/// このアクタにアタッチされているコンポーネント
		/// </summary>
		Cvector <std::unique_ptr<IComponentHolder>> _myComponents;
	};
}