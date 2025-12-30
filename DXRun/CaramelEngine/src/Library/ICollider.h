#pragma once

namespace CaramelEngine
{
	class IColliderVisitor;

	enum class eColliderType
	{
		Rectangle,
		Circle,
		Cube,
		Sphere,
		Ray
	};

	class ICollider 
	{
	public:
		virtual void accept(IColliderVisitor& visitor) = 0;
		virtual eColliderType getColliderType() = 0;
	};
}