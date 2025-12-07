#include "pch.h"

namespace CaramelEngine
{
	namespace Renderer2D
	{
		void drawRectangle(const CVector3& center, const CVector3& size, const Color& color)
		{
			float x = center.getX() - size.getX() * 0.5f;
			float y = center.getX() - size.getY() * 0.5f;
			float w = size.getX();
			float h = size.getY();

			auto d2dColor = MakeColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
			auto rect = MakeRect(x, y, w, h);

			DXRenderer2D::get()->drawRect(rect, d2dColor);
		}

		void drawSprite(int textureId, const CVector3& center, const CVector3& size, const Color& color)
		{
		//	ID2D1Bitmap* bitmap = TextureManager::getInstance().getBitmap(textureId);  // ©‚ ‚Æ‚Åà–¾‚·‚é
		//	if (!bitmap) return;

		//	float x = center.getX() - size.getX() * 0.5f;
		//	float y = center.getY() - size.getY() * 0.5f;
		//	float w = size.getX();
		//	float h = size.getY();

		//	D2D1_RECT_F dst = MakeRect(x, y, w, h);
		//	DXRenderer2D::get()->drawSprite(bitmap, dst);
		}
	}
}
