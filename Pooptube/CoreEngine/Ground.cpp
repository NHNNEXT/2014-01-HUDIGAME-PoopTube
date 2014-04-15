#include "stdafx.h"
#include "Ground.h"

namespace pooptube {
	Ground::Ground() {

	}
	Ground::~Ground(void)
	{
		Release();
	}

	bool Ground::Init()
	{
		m_pd3dDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		return true;
	}
	bool Ground::_InitBuffer(unsigned char *Height)
	{
		m_numVertices = (m_col + 1) * (m_row + 1);
		m_numIndices = m_col * m_row * 6;

		bool IsHeightMap = Height == nullptr ? false : true;
		GROUNDVERTEX *pGroundVertex = new GROUNDVERTEX[m_numVertices];
		int nIndex = 0;
		int imageSize = (m_col + 1) * (m_row + 1) * 3 - 3;
		for (int z = 0; z < m_row + 1; z++)
		{
			for (int x = 0; x < m_col + 1; x++)
			{
				// Z축 반전 구현해야함.

				nIndex = (z * (m_col + 1)) + x;
				pGroundVertex[nIndex].pos.x = m_Size * x;
				pGroundVertex[nIndex].pos.y = IsHeightMap == true ? (float)Height[nIndex * 3] / 200.f : 0.f;
				pGroundVertex[nIndex].pos.z = -1.0f*(m_Size * z);
				pGroundVertex[nIndex].diffuse = D3DCOLOR_RGBA(255, 50, 255, 255);
				//nIndex++;
			}
		}

		if (m_pd3dDevice->CreateVertexBuffer(m_numVertices*sizeof(GROUNDVERTEX), 0, D3DFVF_GROUNDVERTEX, D3DPOOL_DEFAULT, &m_pVB, 0) != D3D_OK)
			return false;

		void *pVertices;
		if (m_pVB->Lock(0, 0, &pVertices, NULL) != D3D_OK) // 2번째 매개변수는 전체를 lock하는 값 0	
			return false;

		memcpy(pVertices, pGroundVertex, m_numVertices*sizeof(GROUNDVERTEX));
		m_pVB->Unlock();

		WORD *pIndex = new WORD[m_numIndices];
		nIndex = 0;
		for (int z = 0; z < m_row; z++)
		{
			for (int x = 0; x < m_col; x++)
			{
				pIndex[nIndex++] = WORD(z * (m_col + 1) + x);
				pIndex[nIndex++] = WORD((z + 1)*(m_col + 1) + x + 1);
				pIndex[nIndex++] = WORD((z + 1)*(m_col + 1) + x);

				pIndex[nIndex++] = WORD(z * (m_col + 1) + x);
				pIndex[nIndex++] = WORD(z * (m_col + 1) + x + 1);
				pIndex[nIndex++] = WORD((z + 1)*(m_col + 1) + x + 1);
			}
		}

		void *pIndices;
		m_pd3dDevice->CreateIndexBuffer(m_numIndices*sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
		m_pIB->Lock(0, 0, (void**)&pIndices, 0);
		memcpy(pIndices, pIndex, sizeof(WORD)*m_numIndices);
		m_pIB->Unlock();

		delete[] pIndex;
		delete[] pGroundVertex;

		return true;
	}
	bool Ground::CreateGround(int row, int col, float size)
	{
		m_col = col;
		m_row = row;
		m_Size = size;

		return _InitBuffer();
	}
	bool Ground::CreateGround(char *HeightmapName)
	{
		FILE* filePtr;
		int error;
		unsigned int count;
		BITMAPFILEHEADER bitmapFileHeader;
		BITMAPINFOHEADER bitmapInfoHeader;
		int imageSize;
		unsigned char* bitmapImage;

		// Open the height map file in binary.
		error = fopen_s(&filePtr, HeightmapName, "rb");
		if (error != 0)
			return false;

		// Read in the file header.
		count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
		if (count != 1)
			return false;

		// Read in the bitmap info header.
		count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
		if (count != 1)
			return false;

		// Save the dimensions of the terrain.
		m_col = bitmapInfoHeader.biWidth - 1;
		m_row = bitmapInfoHeader.biHeight - 1;

		// Calculate the size of the bitmap image data.
		imageSize = (m_col + 1) * (m_row + 1) * 3;

		// Allocate memory for the bitmap image data.
		bitmapImage = new unsigned char[imageSize];
		if (!bitmapImage)
			return false;

		// Move to the beginning of the bitmap data.
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// Read in the bitmap image data.
		count = fread(bitmapImage, 1, imageSize, filePtr);
		if (count != imageSize)
			return false;

		// Close the file.
		error = fclose(filePtr);
		if (error != 0)
			return false;

		bool result = _InitBuffer(bitmapImage);

		delete[]bitmapImage;

		return result;
	}

	void Ground::Render()
	{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(GROUNDVERTEX));
		m_pd3dDevice->SetIndices(m_pIB);
		m_pd3dDevice->SetFVF(D3DFVF_GROUNDVERTEX);
		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_numVertices, 0, m_numIndices / 3);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	void Ground::Release()
	{
		m_pIB->Release();
		m_pVB->Release();
	}

	Ground* Ground::Create()
	{
		Ground* pGround = new Ground();
		if (pGround->Init()) {
			ObjectManager::GetInstance()->AddObject(pGround);
		}
		else {
			delete pGround;
			pGround = nullptr;
		}

		return pGround;
	}

}
