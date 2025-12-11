#include "DXLibModelRender.h"

inline MATRIX XMMATRIXToDXLibMatrix(const DirectX::XMMATRIX& xm)
{
	MATRIX m;

	DirectX::XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, xm); // row-major Ç≈éÊÇËèoÇ∑

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
		MV1SetMatrix(_modelHandle, XMMATRIXToDXLibMatrix(transform->getWorldMatrix()));
		MV1DrawModel(_modelHandle);
	}
	else
		finalize();
}

void DXLibModelRender::finalize() noexcept
{
	MV1DeleteModel(_modelHandle);
	_owner.reset();
}