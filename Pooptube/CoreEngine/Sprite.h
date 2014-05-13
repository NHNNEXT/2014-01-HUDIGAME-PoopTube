#pragma once
#include "Node.h"

namespace pooptube {
	class Sprite : public Node
	{
	public:
		Sprite();
		virtual ~Sprite();

		// ������ �ؽ������ϸ��� �ؽ��ķ� ��������Ʈ ����.
		static Sprite *Create(std::wstring path);
		bool _Init(std::wstring path);

		// color������ D3DCOLOR_ARGB���� ���İ��� �־� �������� �ؽ��ĸ� ��°���
		// prect_textureregion�� �ؽ����� �Ϻο����� �׸��� ���. ��ü�ؽ��ĸ� �׸��� NULL�� ��.
		void Draw(RECT *prect_textureregion, D3DXVECTOR3 *pCenter, D3DCOLOR color);

		// ���Լ��� ȭ��ǥ�ÿ����� ���Ҽ� �ִ� ��(�̹����� �ø��� ����)�� �ٸ�.
		void Draw(RECT *prect_screenregion, RECT *prect_textureregion, D3DXVECTOR3 *pCenter, D3DCOLOR color);

		// ȭ���(client area)�� tx, ty�ȼ� ������ �ؽ��ĸ� ǥ��.
		void Translate(float tx, float ty);

		// 1.0�� ����ũ��.
		void Scale(float scalex, float scaley);

		// ������ŭ ȸ�� (+���� �ð����ȸ��)
		void Rotate(float angle_rad);

		// �� 3����ȯ���� ����.
		void ApplyTransform();

		// �ε��� �ؽ����� �ʺ�, ����.
		int m_width_texture, m_height_texture;

		void Render();

	private:
		// ȸ��, ������, �̵� ���
		D3DXMATRIXA16   m_matRot, m_matScale, m_matTranslation;

		// �� 3����� ��� �����(�� ������� ������) ���
		D3DXMATRIXA16   m_matTransform;

		// ����� �ؽ���
		LPDIRECT3DTEXTURE9  m_ptexture;

		// d3d ��������Ʈ
		LPD3DXSPRITE   m_psprite;
	};
}