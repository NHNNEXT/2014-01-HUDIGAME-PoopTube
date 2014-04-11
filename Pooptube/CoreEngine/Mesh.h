
#pragma once

#include "Node.h"


namespace pooptube {

	//Ŀ���� ���ؽ��� ����
	const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE;
	//�Ž����� ���Ǵ� Ŀ���� ���ؽ��Դϴ�.
	//�̰� ����ֵ� �ǳ�? ���� ��Ŭ��
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
		//�޽��� ������ ����
		//�Ź� ���Ҷ����� ����������Ѵ�. ���ϰ� �ڵ����� ���ϴ� ����� �����غ���.
		int						mPolygonCount;

	private:

	};
}