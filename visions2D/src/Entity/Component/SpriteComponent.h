#pragma once
#include "Component.h"

namespace visions2D {
	class Texture;

	class SpriteComponent : public Component {
	public:
		Texture* tex;
		int DrawOrder;
		Color SpriteColor;

		SpriteComponent() : tex(nullptr) {}
		SpriteComponent(Texture* _Texture, int _DrawOrder) : tex(_Texture), DrawOrder(_DrawOrder) {}

		void Render(Renderer* RendererReference) override {
			{
				TransformComponent* t = Owner->GetComponentOfType<TransformComponent>();
				RenderData rd;

				rd.Texture = this->tex;
				rd.TextureScale = glm::vec2(this->tex->GetWidth(), this->tex->GetHeight());
				// TODO: Sprite Component should have a way to know the tex coords necessary (rect? float[]?)
				rd.TexCoords = nullptr; // tex coords being nullptr will use the default one

				rd.WorldPosition = t->Position;
				rd.WorldRotation = t->Rotation;
				rd.WorldScale = t->Scale;
				rd.tint = SpriteColor;
				RendererReference->SpriteRenderData.push_back(rd);
			}
		}
	};
}