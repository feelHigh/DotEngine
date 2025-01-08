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

	// PaperFlipbook 가 있으면 현재 재생중인 Sprite 정보를 Binding 하게 한다.
	if (PaperFlipbook())
		PaperFlipbook()->Binding();
	else
		DPaperFlipbook::Clear();

	// 위치, 크기, 회전 상태정보 바인딩
	Transform()->Binding();

	// 재질 바인딩(재질 상수, 쉐이더 등등)
	GetMaterial()->Binding();

	// 버텍스버퍼, 인덱스버퍼 바인딩 및 렌더링 호출
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
