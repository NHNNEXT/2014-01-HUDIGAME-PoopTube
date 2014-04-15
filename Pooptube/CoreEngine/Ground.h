#pragma once
#include "Node.h"
#include "Application.h"
#define D3DFVF_GROUNDVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

namespace pooptube {

	struct GROUNDVERTEX
	{
		D3DXVECTOR3 pos;
		DWORD		diffuse;
	};

	class Ground : public Node
	{
	private:
		DWORD m_numVertices, m_numIndices;
		LPDIRECT3DINDEXBUFFER9	m_pIB;
		LPDIRECT3DVERTEXBUFFER9	m_pVB;
		LPDIRECT3DDEVICE9		m_pd3dDevice;
		D3DXVECTOR3 m_pos;
		int m_col, m_row;
		float m_Size = 0.5f;

	public:
		Ground();
		~Ground(void);

		static Ground* Create();

		virtual bool Init();
		virtual bool CreateGround(int row, int col, float size = 0.5f);
		virtual bool CreateGround(char* HeightmapName);
		virtual void Render();
		virtual void Release();

	protected:
		bool _InitBuffer(unsigned char *Height = nullptr);
	};
}