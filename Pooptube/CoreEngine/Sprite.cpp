#include "stdafx.h"
#include "Sprite.h"

namespace pooptube {
	Sprite::Sprite()
	{
		D3DXMatrixIdentity(&m_matRot);
		D3DXMatrixIdentity(&m_matScale);
		D3DXMatrixIdentity(&m_matTranslation);
	}
	Sprite::~Sprite()
	{
		m_psprite->Release();
		m_ptexture->Release();
	}
	Sprite *Sprite::Create(std::wstring path)
	{
		Sprite *pSprite(new Sprite);
		if (pSprite->_Init(path))
			return pSprite;
		else
			return nullptr;
	}
	bool Sprite::_Init(std::wstring path)
	{
		LPDIRECT3DDEVICE9 pd3dDevice = pooptube::Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
		D3DXCreateSprite(pd3dDevice, &m_psprite);

		D3DXCreateTextureFromFile(pd3dDevice, path.c_str(), &m_ptexture);
		D3DSURFACE_DESC desc;
		m_ptexture->GetLevelDesc(0, &desc);
		m_width_texture = desc.Width;
		m_height_texture = desc.Height;
		D3DXMatrixIdentity(&m_matTransform);

		return true;
	}
	void Sprite::Draw(RECT *prect_textureregion, D3DXVECTOR3 *pCenter, D3DCOLOR color)
	{
		if (Sprite::GetVisible()) {
			m_psprite->Begin(D3DXSPRITE_ALPHABLEND);
			m_psprite->Draw(m_ptexture, prect_textureregion, pCenter, NULL, color);
			m_psprite->End();
		}
	}
	void Sprite::Draw(RECT *prect_screenregion, RECT *prect_textureregion, D3DXVECTOR3 *pCenter, D3DCOLOR color)
	{
		if (Sprite::GetVisible()) {
			RECT *rect = prect_screenregion;
			Translate((float)rect->left, (float)rect->top);
			Scale((float)(rect->right - rect->left) / (float)m_width_texture,
				(float)(rect->bottom - rect->top) / (float)m_height_texture);
			ApplyTransform();
			Draw(prect_textureregion, pCenter, color);
		}
	}
	void Sprite::Translate(float tx, float ty)
	{
		D3DXMatrixTranslation(&m_matTranslation, tx, ty, 0.0f);
	}
	void Sprite::Scale(float scalex, float scaley)
	{
		D3DXMatrixScaling(&m_matScale, scalex, scaley, 1.0f);
	}
	void Sprite::Rotate(float angle_rad)
	{
		D3DXMatrixRotationZ(&m_matRot, angle_rad);
	}
	void Sprite::ApplyTransform()
	{
		D3DXMatrixMultiply(&m_matTransform, &m_matRot, &m_matScale);
		D3DXMatrixMultiply(&m_matTransform, &m_matTransform, &m_matTranslation);
		m_psprite->SetTransform(&m_matTransform);
	}
	void Sprite::Render()
	{
		//Node::Render();
	}
}