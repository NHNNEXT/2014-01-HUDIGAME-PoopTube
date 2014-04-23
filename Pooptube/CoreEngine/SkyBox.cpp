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

		/// ����(cube)�� �������ϱ����� 8���� ������ ����
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

		/// �������� ����
		/// 8���� ����������� ������ �޸𸮸� �Ҵ��Ѵ�.
		/// FVF�� �����Ͽ� ������ �������� ������ �����Ѵ�.
		if (FAILED(GetDevice()->CreateVertexBuffer(24 * sizeof(SKYBOX_CUSTOM_VERTEX),
			0, D3DFVF_CUSTOMVERTEX_SKYBOX,
			D3DPOOL_DEFAULT, &mVertexBuffer, NULL)))
		{
			return false;
		}

		/// �������۸� ������ ä���. 
		/// ���������� Lock()�Լ��� ȣ���Ͽ� �����͸� ���´�.
		VOID* pVertices;
		if (FAILED(mVertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
			return false;
		memcpy(pVertices, vertices, sizeof(vertices));
		mVertexBuffer->Unlock();


		/// ����(cube)�� �������ϱ����� 12���� ���� ����
		MESH_CUSTOM_INDEX indices[] = { 0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20 };

		/// �ε������� ����
		/// D3DFMT_INDEX16�� �ε����� ������ 16��Ʈ ��� ���̴�.
		/// �츮�� MYINDEX ����ü���� WORD������ ���������Ƿ� D3DFMT_INDEX16�� ����Ѵ�.
		if (FAILED(GetDevice()->CreateIndexBuffer(36 * sizeof(MESH_CUSTOM_INDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &mIndexBuffer, NULL)))
		{
			return false;
		}

		/// �ε������۸� ������ ä���. 
		/// �ε��������� Lock()�Լ��� ȣ���Ͽ� �����͸� ���´�.
		VOID* pIndices;
		if (FAILED(mIndexBuffer->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
			return false;
		memcpy(pIndices, indices, sizeof(indices));
		mIndexBuffer->Unlock();

		return true;
	}

	void SkyBox::Render() {

		//����� ������ node���� ��ӹ޴´�.
		Node::Render();

		GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX_SKYBOX);

		//����̽��� ���ؽ����۸� ����
		GetDevice()->SetStreamSource(0, mVertexBuffer, 0, sizeof(SKYBOX_CUSTOM_VERTEX));

		//�ε��� ����
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


		GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	}

	void SkyBox::Update(float dTime) {

	}


}