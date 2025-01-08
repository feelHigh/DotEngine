#pragma once

// �����Ҵ�
// 1. ��Ÿ�ӿ� ���� �� ������ ����
// 2. ���α׷� ����� �ݵ�� ��ü �޸� ���� �����־�� ��

// ��������
// 1. ��ü �޸� ������ �Ű澲�� �ʾƵ� ��
// 2. ��Ÿ�� ���� ���������ν� ������ų �� ����

template<typename T>
class DSingleton
{
public:
	DSingleton()
	{
		atexit((EXIT)&DSingleton<T>::Destroy);
	}

	DSingleton(const DSingleton& _Other) = delete;
	virtual ~DSingleton() {}

public:
	static T* GetInst()
	{
		if (nullptr == g_Inst)
		{
			g_Inst = new T;
		}

		return g_Inst;
	}

	static void Destroy()
	{
		if (g_Inst)
		{
			delete g_Inst;
			g_Inst = nullptr;
		}
	}

private:
	static T* g_Inst;

	typedef void (*EXIT)(void);

};

template<typename T>
T* DSingleton<T>::g_Inst = nullptr;
