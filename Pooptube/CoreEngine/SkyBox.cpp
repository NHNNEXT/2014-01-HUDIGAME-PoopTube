#include "stdafx.h"
#include "ResourceManager.h"
#include "SkyBox.h"

namespace pooptube{

	SkyBox::SkyBox() {
	}


	SkyBox::~SkyBox() {
	}


	SkyBox *SkyBox::Create(const std::wstring& top,
		const std::wstring& bottom,
		const std::wstring& front,
		const std::wstring& back,
		const std::wstring& left,
		const std::wstring& right)
	{
		SkyBox *pSkyBox(new SkyBox);
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

		mEffect = ResourceManager::GetInstance()->LoadHLSL(L"Shader\\SkinnedMesh.fx");
		if (!mEffect) {
			MessageBox(NULL, L"Could not HLSL file", L"ERROR", MB_OK);
			assert(false);
			return false;
		}

		mObjectName = "SkyBox" + std::to_string(Node::ObjectNum-1);
		mClassName = "SkyBox";

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

		// TODO: 행렬 계산
		D3DXMATRIXA16	MatWorld;
		D3DXMATRIXA16	MatTrans;
		D3DXMATRIXA16	MatScale;
		D3DXMATRIXA16	MatRotate;
		D3DXVECTOR3		LookPt = mPosition + mFrontVector;

		D3DXMatrixIdentity(&MatWorld);

		//프론트 백터의 값에 따라 회전
		D3DXMatrixLookAtLH(&MatRotate, &mPosition, &LookPt, &mUpVec);
		//뷰행렬을 가져왔기 때문에 로테이션한 것처럼 행렬을 변환할 필요가 있다.
		//뷰행렬은 자신이 움직이는 것이 아닌 자신을 제외한 모든 좌표들이 움직이도록 되어있는 행렬이다.
		//(카메라의 좌표계에 맞춰져있다)
		//뷰행렬의 역행렬은 transpose해준 형태와 동일하다.
		MatRotate._41 = MatRotate._42 = MatRotate._43 = 0.f;
		D3DXMatrixTranspose(&MatRotate, &MatRotate);

		D3DXMatrixTranslation(&MatTrans, mPosition.x, mPosition.y, mPosition.z);
		D3DXMatrixScaling(&MatScale, mScaleVec.x, mScaleVec.y, mScaleVec.z);

		MatWorld = MatScale*MatRotate*MatTrans;

		mDevice->SetTransform(D3DTS_WORLD, &MatWorld);

		mEffect->SetTechnique("t3");
		mEffect->SetMatrix("mWorld", &MatWorld);

		//빛을끈다.
		//GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		GetDevice()->SetFVF(D3DFVF_CUSTOMVERTEX_SKYBOX);

		GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);

		//디바이스에 버텍스버퍼를 전달
		GetDevice()->SetStreamSource(0, mVertexBuffer, 0, sizeof(SKYBOX_CUSTOM_VERTEX));

		//인덱스 설정
		GetDevice()->SetIndices(mIndexBuffer);

		//GetDevice()->SetTexture(0, mBackTexture);
		mEffect->SetTexture("mTexture", mBackTexture);
		UINT cPasses;
		mEffect->Begin(&cPasses, 0);
		for (UINT p = 0; p < cPasses; ++p) {
			mEffect->BeginPass(p);

			Application::GetInstance()->UpdateDPCall();
			GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 4);

			mEffect->EndPass();
		}
		mEffect->End();
		
		//GetDevice()->SetTexture(0, mFrontTexture);
		mEffect->SetTexture("mTexture", mFrontTexture);
		mEffect->Begin(&cPasses, 0);
		for (UINT p = 0; p < cPasses; ++p) {
			mEffect->BeginPass(p);

			Application::GetInstance()->UpdateDPCall();
			GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 6, 4);

			mEffect->EndPass();
		}
		mEffect->End();
		
		//GetDevice()->SetTexture(0, mBotTexture);
		mEffect->SetTexture("mTexture", mBotTexture);
		mEffect->Begin(&cPasses, 0);
		for (UINT p = 0; p < cPasses; ++p) {
			mEffect->BeginPass(p);

			Application::GetInstance()->UpdateDPCall();
			GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 12, 4);

			mEffect->EndPass();
		}
		mEffect->End();
		
		//GetDevice()->SetTexture(0, mTopTexture);
		mEffect->SetTexture("mTexture", mTopTexture);
		mEffect->Begin(&cPasses, 0);
		for (UINT p = 0; p < cPasses; ++p) {
			mEffect->BeginPass(p);

			Application::GetInstance()->UpdateDPCall();
			GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 18, 4);

			mEffect->EndPass();
		}
		mEffect->End();
		

		//GetDevice()->SetTexture(0, mLeftTexture);
		mEffect->SetTexture("mTexture", mLeftTexture);
		mEffect->Begin(&cPasses, 0);
		for (UINT p = 0; p < cPasses; ++p) {
			mEffect->BeginPass(p);

			Application::GetInstance()->UpdateDPCall();
			GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 24, 4);

			mEffect->EndPass();
		}
		mEffect->End();
		

		//GetDevice()->SetTexture(0, mRightTexture);
		mEffect->SetTexture("mTexture", mRightTexture);
		mEffect->Begin(&cPasses, 0);
		for (UINT p = 0; p < cPasses; ++p) {
			mEffect->BeginPass(p);

			Application::GetInstance()->UpdateDPCall();
			GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 30, 4);

			mEffect->EndPass();
		}
		mEffect->End();
		
		GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		GetDevice()->SetTexture(0, 0);
		//GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	}

	void SkyBox::Update(float dTime) {

		if (mTarget)
			SetPosition(mTarget->GetPosition());

		Vibrater(dTime);
	}

	void SkyBox::Vibrater(float dTime) {

		if (mSwitchVibe)
			mVibeCurrentTime += dTime;
		else
			mVibeCurrentTime -= dTime;

		if (mVibeCurrentTime > 10.f)
			mSwitchVibe = false;
		else if (mVibeCurrentTime < 0.f)
			mSwitchVibe = true;

		float temp = mVibeSpeed*mVibeCurrentTime;

		Translation(D3DXVECTOR3(temp, temp, temp));
	}
}