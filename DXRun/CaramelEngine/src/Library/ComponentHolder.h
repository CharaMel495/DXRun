#pragma once
#include <typeindex>

namespace CaramelEngine
{
    /// <summary>
    /// ComponentHolderがテンプレートクラスで配列などで一括で持てないため
    /// 一度IComponentHandlerを継承してという形にしている
    /// </summary>
    class IComponentHolder
    {
    public:
        virtual ~IComponentHolder() = default;
        virtual void callUpdate() noexcept = 0;
        virtual void callFixedUpdate() noexcept = 0;

        // 型IDを返す純粋仮想関数を定義する！
        virtual std::type_index type() const = 0;
        virtual void* rawPtr() = 0;
    };

    /// <summary>
    /// コンポーネントをラップするクラス
    /// </summary>
    /// <typeparam name="TComponent"></typeparam>
    template <ComponentTraits::ConmponentCreatable TComponent>
    class ComponentHolder final : public IComponentHolder
    {
    public:

        explicit ComponentHolder(std::shared_ptr<Actor> owner) :
            _component(std::make_unique<TComponent>(owner))
        {
        }

        void callUpdate() noexcept override
        {
            ComponentTraits::callUpdateIfUpdatable(*_component);
        }

        void callFixedUpdate() noexcept override
        {
            ComponentTraits::callFixedUpdateIfFixedUpdatable(*_component);
        }

        template <typename T>
        T* getIf()
        {
            if constexpr (std::is_same_v<T, TComponent>)
                return _component.get();
            else
                return nullptr;
        }

        std::type_index type() const override 
        {
            return typeid(TComponent);
        }

        void* rawPtr() override 
        {
            return _component.get();
        }

    private:
        std::unique_ptr<TComponent> _component;
    };
}