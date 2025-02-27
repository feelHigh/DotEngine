#include "pch.h"
#include "EditorCameraScript.h"

EditorCameraScript::EditorCameraScript()
	: DScript(-1)
	, m_Speed(500.f)
{

}

EditorCameraScript::~EditorCameraScript()
{

}

void EditorCameraScript::Tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
	{
		OrthoGraphicMove();
	}

	else if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		PerspectiveMove();
	}


	if (KEY_TAP(KEY::P))
	{
		if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}
}

void EditorCameraScript::OrthoGraphicMove()
{
	float Speed = m_Speed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		Speed *= 3.f;
	}

	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos.y += EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.y -= EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos.x -= EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos.x += EngineDT * Speed;
	}

	Transform()->SetRelativePos(vPos);

	float Scale = Camera()->GetScale();
	if (KEY_PRESSED(KEY::_8))
		Scale += EngineDT;

	if (KEY_PRESSED(KEY::_9))
		Scale -= EngineDT;

	Camera()->SetScale(Scale);


}

void EditorCameraScript::PerspectiveMove()
{
	float Speed = m_Speed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		Speed *= 3.f;
	}

	Vec3 vFront = Transform()->GetWorldDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR::RIGHT);

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos += vFront * EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos -= vFront * EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos -= vRight * EngineDT * Speed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += vRight * EngineDT * Speed;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		DKeyMgr::GetInst()->MouseCapture(true);

		// 마우스가 이동하는 방향
		//vDir.x; ==> y축 회전;
		//vDir.y; ==> x축 회전
		Vec2 vDir = DKeyMgr::GetInst()->GetDragDir();

		Vec3 vRot = Transform()->GetRelativeRoatation();
		vRot.y += vDir.x * XM_PI * EngineDT * 10.f;
		vRot.x += vDir.y * XM_PI * EngineDT * 10.f;
		Transform()->SetRelativeRotation(vRot);
	}
	else if (KEY_RELEASED(KEY::RBTN))
	{
		DKeyMgr::GetInst()->MouseCapture(false);
	}
}

void EditorCameraScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void EditorCameraScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
