/**
* MacroSet.h
* 작성자: 장문익
* 작성일: 2014. 4. 22
* 마지막으로 수정한 사람: 장문익
* 수정일: 2014. 4. 22
* 여기 나둬도 되나 모르겠네...?
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

#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }