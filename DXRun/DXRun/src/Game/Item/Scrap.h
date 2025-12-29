#pragma once
#include "CaramelEngine.h"
#include "ItemBase.h"

// プレイヤーが集めることができるアイテム
class Scrap : public ItemBase
{
public:

	Scrap(std::shared_ptr<Transform> transform, CString name) noexcept :
		ItemBase(transform, name)
	{
	}

private:
};