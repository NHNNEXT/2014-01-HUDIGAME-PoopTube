/**
* MacroSet.h
* �ۼ���: �幮��
* �ۼ���: 2014. 4. 22
* ���������� ������ ���: �幮��
* ������: 2014. 4. 22
* ���� ���ֵ� �ǳ� �𸣰ڳ�...?
*/

#pragma once

#define WM_SOCKET 104
#define ALIGNMENT_SIZE 16

template <class T>
class Singleton
{
protected:
	Singleton(void) {}
	virtual ~Singleton(void) {}
public:
	static T* GetInstance(void)
	{
		if (!m_Instance)
		{
			m_Instance = (T*)_aligned_malloc(sizeof(T), ALIGNMENT_SIZE);
			new (m_Instance)T();
		}
		return m_Instance;
	}

	static void Release(void)
	{
		if (m_Instance)
		{
			m_Instance->~T();
			_aligned_free(m_Instance);
		}
	}

private:
	static T*	m_Instance;
};

template <typename T>
inline void SafeDelete(T* &p)
{
	if (p != nullptr)
	{
		delete p;
		p = nullptr;
	}
}

template <typename T>
inline void SafeRelease(T* &p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}

