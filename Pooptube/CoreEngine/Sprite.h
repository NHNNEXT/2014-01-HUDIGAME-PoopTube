#pragma once
#include "Node.h"

namespace pooptube {
	class Sprite : public Node
	{
	public:
		Sprite();
		virtual ~Sprite();

		// 지정된 텍스쳐파일명의 텍스쳐로 스프라이트 생성.
		static Sprite *Create(std::wstring path);
		bool _Init(std::wstring path);

		// color값에서 D3DCOLOR_ARGB값중 알파값을 주어 반투명한 텍스쳐를 출력가능
		// prect_textureregion은 텍스쳐의 일부영역을 그릴때 사용. 전체텍스쳐를 그릴땐 NULL을 줌.
		void Draw(RECT *prect_textureregion, D3DXVECTOR3 *pCenter, D3DCOLOR color);

		// 위함수와 화면표시영역을 정할수 있는 것(이미지를 늘리고 줄임)만 다름.
		void Draw(RECT *prect_screenregion, RECT *prect_textureregion, D3DXVECTOR3 *pCenter, D3DCOLOR color);

		// 화면상(client area)의 tx, ty픽셀 지점에 텍스쳐를 표시.
		void Translate(float tx, float ty);

		// 1.0이 원래크기.
		void Scale(float scalex, float scaley);

		// 각도만큼 회전 (+값이 시계방향회전)
		void Rotate(float angle_rad);

		// 위 3개변환들을 적용.
		void ApplyTransform();

		// 로드한 텍스쳐의 너비, 높이.
		int m_width_texture, m_height_texture;

		void Render();

	private:
		// 회전, 스케일, 이동 행렬
		D3DXMATRIXA16   m_matRot, m_matScale, m_matTranslation;

		// 위 3행렬이 모두 적용된(위 순서대로 곱해진) 행렬
		D3DXMATRIXA16   m_matTransform;

		// 출력할 텍스쳐
		LPDIRECT3DTEXTURE9  m_ptexture;

		// d3d 스프라이트
		LPD3DXSPRITE   m_psprite;
	};
}