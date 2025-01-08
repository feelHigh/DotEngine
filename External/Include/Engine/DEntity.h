#ifndef ENGINE_DENTITY_H
#define ENGINE_DENTITY_H

class DEntity
{
public:
	CLONE_DISABLE(DEntity);
	DEntity();
	DEntity(const DEntity& _Other);
	virtual ~DEntity();

public:
	void SetName(const wstring& _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }
	UINT GetID() { return m_ID; }

private:
	static UINT g_ID;

private:
	wstring		m_Name;
	const UINT	m_ID;

};

#endif // !ENGINE_DENTITY_H