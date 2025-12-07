#pragma once

namespace CaramelEngine
{
	class IColliderVisitor;

	class ICollider 
	{
	public:
		virtual void accept(IColliderVisitor& visitor) = 0;
	};
}