#pragma once

namespace CaramelEngine
{
	/// <summary>
	/// 方向を表す(8方向)
	/// </summary>
	enum struct Direction
	{
		Up,
		UpRight,
		Right,
		DownRight,
		Down,
		DownLeft,
		Left,
		UpLeft,
		None
	};

	/// <summary>
	/// 方向を表す(4方向)
	/// </summary>
	enum struct Direction4
	{
		Up,
		Right,
		Down,
		Left,
		None
	};
}

namespace
{
	enum struct EnemyType : uint32_t
	{
		Normal,
		Shooter,
	};

	enum struct ShootType
	{
		OneWayStraight,
	};

	enum struct BulletType
	{
		PlayerBullet,
		EnemyBullet,
	};

	/// <summary>
	/// 弾の所有者タイプ
	/// </summary>
	enum class OwnerType
	{
		Player,
		Enemy
	};

	enum class ColliderType
	{
		Rectangle,
		Circle
	};
}