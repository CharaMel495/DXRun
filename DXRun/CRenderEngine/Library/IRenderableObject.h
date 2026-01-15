#pragma once

namespace CaramelEngine
{
	class IRenderableObject
	{
	public:

		int GetRenderPriority() { return _renderPriority; }

	private:

		int _renderPriority;
		
	};
}