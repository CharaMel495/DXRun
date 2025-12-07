#pragma once

namespace CaramelEngine
{
    class ICollisionObject 
    {
    public:

        ICollisionObject(CString eventKey) :
            _eventKeyEnter(eventKey + "OnCollisionEnter"),
            _eventKeyStay(eventKey + "OnCollisionStay"),
            _eventKeyExit(eventKey + "OnCollisionExit")
        {}

        virtual ~ICollisionObject() = default;

        //衝突判定に使うコライダー
        virtual ICollider* getCollider() const = 0;

        //CollisionGroupを返す（どのグループに属するか）
        virtual std::uint32_t getCollisionGroupID() const = 0;

        //衝突通知先などのための汎用ポインタ（Actor*など）
        virtual void* getOwnerPointer() const = 0;

        //イベント通知用に名前を受け取る
        CString getOnCollisionEnterEventKey() const noexcept { return _eventKeyEnter; }
        CString getOnCollisionStayEventKey() const noexcept { return _eventKeyStay; }
        CString getOnCollisionExitEventKey() const noexcept { return _eventKeyExit; }

        //衝突イベント３種
        virtual void onCollisionEnter(void* data) noexcept = 0;
        virtual void onCollisionStay(void* data) noexcept = 0;
        virtual void onCollisionExit(void* data) noexcept = 0;

    private:

        //イベントの発火に使うオブジェクト名
        CString _eventKeyEnter;
        CString _eventKeyStay;
        CString _eventKeyExit;
    };
}