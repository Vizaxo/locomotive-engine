#pragma once

#include "rhi/RHI.h" 

struct SpriteSheetCB {
	v2f tileSize;
	v2f sheetSize;
};

struct SpriteSheet {
	RefPtr<RHI::Texture2D> texture;
	SpriteSheetCB cbData;
};

struct Sprite {
	RefPtr<SpriteSheet> spriteSheet;
};
