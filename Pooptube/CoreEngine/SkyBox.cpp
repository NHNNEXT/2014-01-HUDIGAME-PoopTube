#include "stdafx.h"
#include "ResourceManager.h"
#include "SkyBox.h"
#include "Mesh.h"

namespace pooptube{

	SkyBox::SkyBox() {
	}


	SkyBox::~SkyBox() {
	}


	std::shared_ptr<SkyBox> SkyBox::Create(const std::wstring& top,
		const std::wstring& bottom,
		const std::wstring& front,
		const std::wstring& back,
		const std::wstring& left,
		const std::wstring& right)
	{
		std::shared_ptr<SkyBox> pSkyBox(new SkyBox);
		if (pSkyBox->_Init(top, bottom, front, back, left, right))
			return pSkyBox;
		else
			return nullptr;
	}


	bool SkyBox::_Init(const std::wstring& top,
		const std::wstring& bottom,
		const std::wstring& front,
		const std::wstring& back,
		const std::wstring& left,
		const std::wstring& right)
	{
		Node::Init();

		mTopTexture = ResourceManager::GetInstance()->LoadTexture(top);
		mBotTexture = ResourceManager::GetInstance()->LoadTexture(bottom);
		mFrontTexture = ResourceManager::GetInstance()->LoadTexture(front);
		mBackTexture = ResourceManager::GetInstance()->LoadTexture(back);
		mLeftTexture = ResourceManager::GetInstance()->LoadTexture(left);
		mRightTexture = ResourceManager::GetInstance()->LoadTexture(right);

		float w = mWidth;
		float h = mHeight;
		float d = mDepth;

		float x = GetPosition().x;
		float y = GetPosition().y;
		float z = GetPosition().z;

		/// 상자(cube)를 렌더링하기위해 8개의 정점을 선언
		SKYBOX_CUSTOM_VERTEX vertices[] =
		{
			//back
			{ D3DXVECTOR3(x + w, y - h, z - d), 1.0f, 1.0f },
			{ D3DXVECTOR3(x + w, y + h, z - d), 1.0f, 0.0f },
			{ D3DXVECTOR3(x - w, y + h, z - d), 0.0f, 0.0f },
			{ D3DXVECTOR3(x - w, y - h, z - d), 0.0f, 1.0f },

			//front
			{ D3DXVECTOR3(x - w, y - h, z + d), 1.0f, 1.0f },
			{ D3DXVECTOR3(x - w, y + h, z + d), 1.0f, 0.0f },
			{ D3DXVECTOR3(x + w, y + h, z + d), 0.0f, 0.0f },
			{ D3DXVECTOR3(x + w, y - h, z + d), 0.0f, 1.0f },

			//bottom
			{ D3DXVECTOR3(x - w, y - h, z - d), 1.0f, 1.0f },
			{ D3DXVECTOR3(x - w, y - h, z + d), 1.0f, 0.0f },
			{ D3DXVECTOR3(x + w, y - h, z + d), 0.0f, 0.0f },
			{ D3DXVECTOR3(x + w, y - h, z - d), 0.0f, 1.0f },

			//top
			{ D3DXVECTOR3(x + w, y + h, z - d), 0.0f, 0.0f },
			{ D3DXVECTOR3(x + w, y + h, z + d), 0.0f, 1.0f },
			{ D3DXVECTOR3(x - w, y + h, z + d), 1.0f, 1.0f },
			{ D3DXVECTOR3(x - w, y + h, z - d), 1.0f, 0.0f },

			//left
			{ D3DXVECTOR3(x - w, y + h, z - d), 1.0f, 0.0f },
			{ D3DXVECTOR3(x - w, y + h, z + d), 0.0f, 0.0f },
			{ D3DXVECTOR3(x - w, y - h, z + d), 0.0f, 1.0f },
			{ D3DXVECTOR3(x - w, y - h, z - d), 1.0f, 1.0f },

			//right
			{ D3DXVECTOR3(x + w, y - h, z - d), 0.0f, 1.0f },
			{ D3DXVECTOR3(x + w, y - h, z + d), 1.0f, 1.0f },
			{ D3DXVECTOR3(x + w, y + h, z + d), 1.0f, 0.0f },
			{ D3DXVECTOR3(x + w, y + h, z - d), 0.0f, 0.0f },
		};

		/// 정점버퍼 생성
		/// 8개의 사용자정점을 보관할 메모리를 할당한다.
		/// FVF를 지정하여 보관할 데이터의 형식을 지정한다.
		if (FAILED(GetDevice()->CreateVertexBuffer(24 * sizeof(SKYBOX_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX_SKYBOX,
			D3DPOOL_DEFAULT, &mVertexBuffer, NULL)))
		{
			return false;
		}

		/// 정점버퍼를 값으로 채운다. 
		/// 정점버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pVertices;
		if (FAILED(mVertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return false;
		memcpy(pVertices, vertices, sizeof(vertices));
		mVertexBuffer->Unlock();


		/// 상자(cube)를 렌더링하기위해 12개의 면을 선언
		MESH_CUSTOM_INDEX indices[] = { 2, 1, 0,
			0, 3, 2,
			6, 5, 4,
			4, 7, 6,
			10, 9, 8,
			8, 11, 10,
			14, 13, 12,
			12, 15, 14,
			18, 17, 16,
			16, 19, 18,
			22, 21, 20,
			20, 23, 22 };

		/// 인덱스버퍼 생성
		/// D3DFMT_INDEX16은 인덱스의 단위가 16비트 라는 것이다.
		/// 우리는 MYINDEX 구조체에서 WORD형으로 선언했으므로 D3DFMT_INDEX16을 사용한다.
		if (FAILED(GetDevice()->CreateIndexBuffer(36 * sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &mIndexBuffer, NULL)))
		{
			return false;
		}

		/// 인덱스버퍼를 값으로 채운다. 
		/// 인덱스버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
		VOID* pIndices;
		if (FAILED(mIndexBuffer->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
			return false;
		memcpy(pIndices, indices, sizeof(indices));
		mIndexBuffer->Unlock();

		return true;
	}

	void SkyBox::Render() {

		//행렬의 연산은 node에서 상속받는다.
		Node::Render();

		//빛을끈다.
		GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX_SKYBOX);

		//디바이스에 버텍스버퍼를 전달
		GetDevice()->SetStreamSource(0, mVertexBuffer, 0, sizeof(SKYBOX_CUSTOM_VERTEX));

		//인덱스 설정
		GetDevice()->SetIndices(mIndexBuffer);

		GetDevice()->SetTexture(0, mBackTexture);
		// 		GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		// 		GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		// 		GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		// 		GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 4);

		GetDevice()->SetTexture(0, mFrontTexture);
		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 6, 4);

		GetDevice()->SetTexture(0, mBotTexture);
		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 12, 4);

		GetDevice()->SetTexture(0, mTopTexture);
		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 18, 4);

		GetDevice()->SetTexture(0, mLeftTexture);
		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 24, 4);

		GetDevice()->SetTexture(0, mRightTexture);
		GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 30, 4);
		
		GetDevice()->SetTexture(0, 0);
		GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	}

	void SkyBox::Update(float dTime) {

	}


}