#include "pch.h"
#include "DMeshRender.h"

// Asset Headers
#include "DSprite.h"

// Component Headers
#include "DTransform.h"
#include "DPaperFlipbook.h"

DMeshRender::DMeshRender()
	: DRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

DMeshRender::~DMeshRender()
{
}

void DMeshRender::FinalTick()
{
}

void DMeshRender::Render()
{
	if (!GetMesh() || !GetMaterial() || !GetMaterial()->GetShader())
		return;

	// Whether if Paper Flipbook, Binds the Sprite information that is currently playing 
	if (PaperFlipbook())
		PaperFlipbook()->Binding();
	else
		DPaperFlipbook::Clear();

	// Bind position, size, rotation status information
	Transform()->Binding();

	// Material binding (material constant, shader, etc.)
	GetMaterial()->Binding();

	// Vertex buffer, index buffer binding, and rendering calls
	GetMesh()->Render();
}

void DMeshRender::SaveToFile(FILE* _File)
{
	SaveDataToFile(_File);
}

void DMeshRender::LoadFromFile(FILE* _File)
{
	LoadDataFromFile(_File);
}
