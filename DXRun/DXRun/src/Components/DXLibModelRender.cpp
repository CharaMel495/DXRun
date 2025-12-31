#include "DXLibModelRender.h"

using namespace DxLib;

inline MATRIX XMMATRIXToDXLibMatrix(const DirectX::XMMATRIX& xm)
{
	MATRIX m;

	DirectX::XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, xm); // row-major で取り出す

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			m.m[r][c] = temp.m[r][c];
		}
	}

	return m;
}


void DXLibModelRender::initialize() noexcept
{
	_modelHandle = MV1LoadModel(_filePath.c_str());

    if (_modelHandle < 0)
    {
        _modelLocalSize = CVector3{ 1.f, 1.f, 1.f };
        _importScale = 1.0f;
        return;
    }

    const AABB3 aabb = CalcModelLocalAABB(_modelHandle);
    const DxLib::VECTOR sizeV = SizeOf(aabb);

    _modelLocalSize = CVector3{ sizeV.x, sizeV.y, sizeV.z };

    // 長辺（絶対値）を基準に「縮小だけ」する
    const float ax = std::abs(sizeV.x);
    const float ay = std::abs(sizeV.y);
    const float az = std::abs(sizeV.z);
    const float longest = (std::max)({ ax, ay, az });

    constexpr float kTargetLongest = 1.0f;
    constexpr float kEps = 1e-6f;
    constexpr float kMaxUpScale = 10.0f; // 拡大は最大10倍まで

    if (longest > kEps)
    {
        _importScale = kTargetLongest / longest;
        _importScale = (std::min)(_importScale, kMaxUpScale);
    }
    else
    {
        _importScale = 1.0f;
    }

}

void DXLibModelRender::update() noexcept
{
	renderModel();
}

void DXLibModelRender::renderModel() noexcept
{
    if (!_enabled)
        return;

    if (auto owner = _owner.lock())
    {
        auto transform = owner->getTransform();

        DirectX::XMMATRIX world = transform->getWorldMatrix();
        const DirectX::XMMATRIX importS =
            DirectX::XMMatrixScaling(_importScale, _importScale, _importScale);

        // 「ローカル補正」→「Transformの世界変換」
        world = importS * world;

        DxLib::MV1SetMatrix(_modelHandle, XMMATRIXToDXLibMatrix(world));
        DxLib::MV1DrawModel(_modelHandle);
    }
    else
    {
        finalize();
    }
}

void DXLibModelRender::finalize() noexcept
{
	MV1DeleteModel(_modelHandle);
	_owner.reset();
}
