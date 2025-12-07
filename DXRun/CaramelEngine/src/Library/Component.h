#pragma once

namespace CaramelEngine
{
	class Actor;

	/// <summary>
	/// 全てのコンポーネントの基底クラス
	/// </summary>
	class Component
	{
	public:

		inline Component() noexcept = delete;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		inline Component(std::weak_ptr<Actor> owner, CString name) noexcept :
			_owner(owner),
			_name(name),
			_enabled(true)
		{}

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Component() = default;

		virtual void initialize() noexcept = 0;

		void setOwner(std::weak_ptr<Actor> owner) { _owner = owner; }
		std::weak_ptr<Actor> getOwner() const { return _owner; }

		void setEnabled(bool enabled) { _enabled = enabled; }
		bool isEnabled() const { return _enabled; }

		void setName(const CString& name) { _name = name; }
		const CString& getName() const { return _name; }

	protected:

		/// <summary>
		/// 親アクタ参照
		/// </summary>
		std::weak_ptr<Actor> _owner;

		/// <summary>
		/// 有効/無効フラグ
		/// </summary>
		bool _enabled = true;    

		/// <summary>
		/// 任意の識別名
		/// </summary>
		CString _name = "Unnamed";    
	};
}

namespace CaramelEngine
{
	namespace ComponentTraits
	{

		template<typename TComponent>
		concept ConmponentCreatable =
			std::is_base_of_v<Component, TComponent>&&
			std::constructible_from<TComponent, std::weak_ptr<Actor>>;

		// 更新処理を呼び出せるconcept
		template<typename T>
		concept Updatable = requires(T t)
		{
			{ t.update() } -> std::same_as<void>;
		};

		// 固定長更新処理を呼び出せるconcept
		template<typename T>
		concept FixedUpdatable = requires(T t)
		{
			{ t.fixedUpdate() } -> std::same_as<void>;
		};

		// --- 呼び出しユーティリティ ---

		template<typename T>
		void callUpdateIfUpdatable(T& obj)
		{
			if constexpr (Updatable<T>)
				obj.update();
		}

		template<typename T>
		void callFixedUpdateIfFixedUpdatable(T& obj)
		{
			if constexpr (FixedUpdatable<T>)
				obj.fixedUpdate();
		}
	}
}