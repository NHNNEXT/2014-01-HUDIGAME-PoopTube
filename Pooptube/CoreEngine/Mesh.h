
#pragma once

#include "Node.h"


namespace pooptube {

	//커스텀 버텍스를 설정
	const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE;
	//매쉬에서 사용되는 커스텀 버텍스입니다.
	//이걸 여기둬두 되나? 누가 태클쩜
	struct MESHCUSTOMVERTEX
	{
		D3DXVECTOR3	position;
		D3DXVECTOR3	normal;
		D3DCOLOR	color;
	};

	class Mesh : public Node {
	public:
		Mesh();
		virtual ~Mesh();

		static Mesh* Create();

		virtual bool Init();

		virtual void Render();
		virtual void Update( float dTime );

		int						GetPolygonCount() const { return mPolygonCount; }

	protected:
		//메쉬의 폴리곤 갯수
		//매번 변할때마다 수정해줘야한다. 편하게 자동으로 변하는 방법을 생각해보자.
		int						mPolygonCount;

	private:

	};
}