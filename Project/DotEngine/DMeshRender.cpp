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

	// PaperFlipbook �� ������ ���� ������� Sprite ������ Binding �ϰ� �Ѵ�.
	if (PaperFlipbook())
		PaperFlipbook()->Binding();
	else
		DPaperFlipbook::Clear();

	// ��ġ, ũ��, ȸ�� �������� ���ε�
	Transform()->Binding();

	// ���� ���ε�(���� ���, ���̴� ���)
	GetMaterial()->Binding();

	// ���ؽ�����, �ε������� ���ε� �� ������ ȣ��
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
