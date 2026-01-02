#pragma once
#include "DXLib.h"
#include "CaramelEngine.h"

using namespace CaramelEngine;
using namespace DxLib;

class DXLibModelRender : public Component
{
public:

	DXLibModelRender(std::weak_ptr<Actor> owner) :
		Component(owner, "DXLibModelRender"),
		_filePath(""),
		_modelHandle(-1),
		_animHandle(-1)
	{}

	void initialize() noexcept override;

	void update() noexcept;
	
	void renderModel() noexcept;

	void finalize() noexcept;

	void setFilePath(CString filePath) noexcept { _filePath = filePath; }

    struct AABB3
    {
        DxLib::VECTOR minV;
        DxLib::VECTOR maxV;
    };

    static AABB3 CalcModelLocalAABB(int modelHandle)
    {
        const int meshNum = DxLib::MV1GetMeshNum(modelHandle);

        AABB3 aabb{};
        aabb.minV = DxLib::VGet(+FLT_MAX, +FLT_MAX, +FLT_MAX);
        aabb.maxV = DxLib::VGet(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        MV1_REF_POLYGONLIST RefPoly;

        for (int i = 0; i < meshNum; ++i)
        {
            // モデルの全フレームのポリゴンの情報を取得
            RefPoly = MV1GetReferenceMesh(modelHandle, i, TRUE);

            const DxLib::VECTOR mx = RefPoly.MaxPosition;
            const DxLib::VECTOR mn = RefPoly.MinPosition;

            aabb.minV.x = (std::min)(aabb.minV.x, mn.x);
            aabb.minV.y = (std::min)(aabb.minV.y, mn.y);
            aabb.minV.z = (std::min)(aabb.minV.z, mn.z);

            aabb.maxV.x = (std::max)(aabb.maxV.x, mx.x);
            aabb.maxV.y = (std::max)(aabb.maxV.y, mx.y);
            aabb.maxV.z = (std::max)(aabb.maxV.z, mx.z);
        }
        return aabb;
    }

    static DxLib::VECTOR SizeOf(const AABB3& aabb)
    {
        return DxLib::VSub(aabb.maxV, aabb.minV);
    }

    void startBlink(int frameCount) noexcept { _blinkFrame = frameCount; }

private:

	CString _filePath;
	int _modelHandle;
	int _animHandle;
    CVector3 _modelLocalSize{ 1.f, 1.f, 1.f }; // ローカルAABBサイズ
    float _importScale = 1.0f;
    bool _visible = true;              // 表示/非表示
    int _blinkFrame = 0;               // 残り点滅フレーム数
    int _blinkInterval = 5;            // 点滅切り替え間隔（フレーム）
};