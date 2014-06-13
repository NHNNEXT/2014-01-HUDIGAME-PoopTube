#include "stdafx.h"
#include "Node.h"

#include "ObjectManager.h"


namespace pooptube {
	unsigned int Node::ObjectNum = 0; // 생성된 Object의 갯수를 카운팅

	Node::Node() {
	}
	Node::~Node() {
// 		for (auto& iter = mChildList.begin(); iter != mChildList.end(); iter++) {
// 			ObjectManager::GetInstance()->RemoveObject((*iter).get());
// 		}
		mChildList.clear();
	}

	Node *Node::Create() {
		Node *pNode(new Node);
		if (pNode->Init()) 
			return pNode;
		else 
			return nullptr;
	}

	bool Node::Init() {
		mDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
		UpdateMatrix();

		mObjectName = "Node" + std::to_string(Node::ObjectNum++);
		mClassName = "Node";

		return true;
	}

	void Node::Render() {

		UpdateMatrix();

		for (auto& child : mChildList) {
			if (child->GetVisible())
				child->Render();
		}
	}
	void Node::Update(float dTime) {
		for (auto& child : mChildList) {
			if (child->GetUpdatable())
				child->Update(dTime);
		}
	}

	void Node::AddChild(Node* pChild) {
		mChildList.push_back(std::shared_ptr<Node>(pChild));
	}
	void Node::RemoveChild(Node* pChild) {
		for (auto& iter = mChildList.begin(); iter != mChildList.end(); iter++) {
			if ((*iter).get() == pChild) {
				//(*iter).reset();
				ObjectManager::GetInstance()->RemoveObject((*iter).get());
				mChildList.erase(iter);
				break;
			}
		}
	}

	void Node::RotationY(float Angle) {
		RotateFrontVectorY(Angle);
		RotateUpVectorY(Angle);

		for (auto& child : mChildList) {
			child->RotationY(Angle);
		}
	}


	void Node::RotationZ(float Angle) {
		RotateFrontVectorZ(Angle);
		RotateUpVectorZ(Angle);

		for (auto& child : mChildList) {
			child->RotationZ(Angle);
		}
	}

	void Node::RotationX(float Angle) {
		RotateFrontVectorX(Angle);
		RotateUpVectorX(Angle);

		for (auto& child : mChildList) {
			child->RotationX(Angle);
		}
	}

	void Node::RotateFrontVectorY(float angle) {
// 		float vx = mFrontVec.x*cosf(angle) + mFrontVec.z*sinf(angle);
// 		float vz = -mFrontVec.x*sinf(angle) + mFrontVec.z*cosf(angle);
// 		mFrontVec.x = vx;
// 		mFrontVec.z = vz;
// 		D3DXVec3Normalize(&mFrontVec, &mFrontVec);

		mFrontVector.x = mFrontVector.x*cosf(angle) + mFrontVector.z*sinf(angle);
		mFrontVector.z = -mFrontVector.x*sinf(angle) + mFrontVector.z*cosf(angle);
		D3DXVec3Normalize(&mFrontVector, &mFrontVector);
	}


	void Node::RotateFrontVectorX(float angle) {
		mFrontVector.z = mFrontVector.z*cosf(angle) + mFrontVector.y*sinf(angle);
		mFrontVector.y = -mFrontVector.z*sinf(angle) + mFrontVector.y*cosf(angle);
		D3DXVec3Normalize(&mFrontVector, &mFrontVector);
	}

	void Node::RotateFrontVectorZ(float angle) {
		mUpVec.y = mUpVec.y*cosf(angle) + mUpVec.x*sinf(angle);
		mUpVec.x = -mUpVec.y*sinf(angle) + mUpVec.x*cosf(angle);
		D3DXVec3Normalize(&mUpVec, &mUpVec);
	}

	void Node::RotateUpVectorX(float angle) {
		mUpVec.z = mUpVec.z*cosf(angle) + mUpVec.y*sinf(angle);
		mUpVec.y = -mUpVec.z*sinf(angle) + mUpVec.y*cosf(angle);
		D3DXVec3Normalize(&mUpVec, &mUpVec);
	}

	void Node::RotateUpVectorY(float angle) {
		mUpVec.x = mUpVec.x*cosf(angle) + mUpVec.z*sinf(angle);
		mUpVec.z = -mUpVec.x*sinf(angle) + mUpVec.z*cosf(angle);
		D3DXVec3Normalize(&mUpVec, &mUpVec);
	}

	void Node::RotateUpVectorZ(float angle) {

		mFrontVector.y = mFrontVector.y*cosf(angle) + mFrontVector.x*sinf(angle);
		mFrontVector.x = -mFrontVector.y*sinf(angle) + mFrontVector.x*cosf(angle);
		D3DXVec3Normalize(&mFrontVector, &mFrontVector);
	}

	void Node::Translation( const D3DXVECTOR3& moveVec ) {
		mPosition += moveVec;

		for( auto& child : mChildList ) {
			child->Translation( moveVec );
		}
	}

	void Node::Translation( float x, float y, float z ) {
		D3DXVECTOR3 temp(x, y, z);
		Translation( temp );
	}


	void Node::Move(float dForward, float dSide)
	{
		D3DXVECTOR3 pos = mPosition;

		pos += mFrontVector * dForward;
		pos += GetLeftVector() * dSide;

		mPosition = pos;

		for( auto& child : mChildList ) {
			child->Move( dForward, dSide );
		}
	}


	void Node::SetPosition(const D3DXVECTOR3& newPos) {
		for( auto& child : mChildList ){
			D3DXVECTOR3 dPos = child->mPosition - mPosition;
			child->SetPosition( newPos + dPos );
		}
		mPosition = newPos;
	}

	D3DXVECTOR3 Node::GetRightVector() {
		D3DXVECTOR3 Vec = mFrontVector;
		D3DXVec3Cross(&Vec, &mUpVec, &Vec);
		return Vec;
	}

	D3DXVECTOR3 Node::GetLeftVector() {
		D3DXVECTOR3 Vec = mFrontVector;
		D3DXVec3Cross(&Vec, &Vec, &mUpVec);
		return Vec;
	}

	void Node::SetScale( const D3DXVECTOR3& newScale ) {
		for( auto& child : mChildList ){
			D3DXVECTOR3 dVec;
			dVec.x = newScale.x * child->mScaleVec.x / mScaleVec.x;
			dVec.y = newScale.y * child->mScaleVec.y / mScaleVec.y;
			dVec.z = newScale.z * child->mScaleVec.z / mScaleVec.z;
			child->SetScale( dVec );
		}
		mScaleVec = newScale;
	}

	void Node::UpdateMatrix() {
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
	}

	float Node::GetTurnAngle(D3DXVECTOR3 src, D3DXVECTOR3 dst)
	{
		src.y = dst.y = 0.f;

		D3DXVECTOR3 dir = dst - src;
		D3DXVECTOR3 Look = GetFrontVector() + src;
		float dot = D3DXVec3Dot(&dir, &mFrontVector);
		float dis = abs((D3DXVec3Length(&dir) * D3DXVec3Length(&mFrontVector)));
		float angle = 0.f;
		float det = ((dst.x - src.x) * (Look.z - src.z) - (dst.z - src.z) * (Look.x - src.x));

		if (det > 0.f)	det = -1.f; // CW
		else			det = 1.f;	// CCW

 		if (dis != 0.f && ((dot / dis) < 1.f))
 			angle = det * acos(dot / dis);

		return angle;
	}

	bool Node::Turn(D3DXVECTOR3 src, D3DXVECTOR3 dst, float speed)
	{
		float precision = 0.05f;
		float angle = GetTurnAngle(src, dst);

		if (angle > 0.f)
			speed *= -1.f;

		if (angle < precision && angle > -precision)	return false;
		else if (abs(angle) > abs(speed))				RotationY(speed);
		else											RotationY(angle);

		return true;
	}

	void Node::GetRay(float x, float y, D3DXVECTOR3 *Origin, D3DXVECTOR3 *Direction)
	{
		D3DVIEWPORT9 view;
		GetDevice()->GetViewport(&view);

		//getting projection matrix
		D3DXMATRIX projMat;
		GetDevice()->GetTransform(D3DTS_PROJECTION, &projMat);

		//calculating.. mouse ray
		//float vx = (((((x - view.X) * 2.0f / (float)view.Width) - 1.0f)) - projMat(2, 0)) / projMat(0, 0);
		//float vy = ((-(((y - view.Y) * 2.0f / (float)view.Height) - 1.0f)) - projMat(2, 1)) / projMat(1, 1);

 		float vx = (+2.0f*x / view.Width - 1.0f) / projMat._11;
 		float vy = (-2.0f*y / view.Height + 1.0f) / projMat._22;

		//Vector is D3DVECTOR
		Origin->x = 0.0f;
		Origin->y = 0.0f;
		Origin->z = 0.0f;

		// I used Z as my UP VECTOR, not sure how it will work for you
		Direction->x = vx;
		Direction->y = vy;
		Direction->z = 1.0f;

		//getting projection matrix
		D3DXMATRIX viewMat;
		GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);

		//inversing projection matrix
		D3DXMATRIX iviewMat;
		D3DXMatrixInverse(&iviewMat, 0, &viewMat);
		D3DXVec3TransformCoord(Origin, Origin, &iviewMat);
		D3DXVec3TransformNormal(Direction, Direction, &iviewMat);

		//printf("INVERSING VIEW MATRIX ============\n");
		//printf("(%f %f %f)\n", Origin->x, Origin->y, Origin->z);
		//printf("DIRECT (%f %f %f)\n\n", Direction->x, Direction->y, Direction->z);

		//inversing world matrix
// 		D3DXMATRIXA16 worldMat;
// 		GetDevice()->GetTransform(D3DTS_WORLD, &worldMat);
// 		D3DXMatrixInverse(&worldMat, 0, &worldMat);
// 
// 		D3DXVec3TransformCoord(Origin, Origin, &worldMat);
// 		D3DXVec3TransformNormal(Direction, Direction, &worldMat
		//printf("INVERSING WORLD MATRIX ============\n");
		//printf("(%f %f %f)\n\n", Origin->x, Origin->y, Origin->z);
		//printf("DIRECT (%f %f %f)\n\n", Direction->x, Direction->y, Direction->z);
	}

	Node * Node::Pick(float x, float y, D3DXVECTOR3 *intersectPos)
	{
		//result->clear();
		Node *result = nullptr;
		float minDistance = 9999.f;


		D3DXVECTOR3 Origin, Direction;
		GetRay(x, y, &Origin, &Direction);

		for (auto &iter : mChildList)
		{
			std::vector<D3DXVECTOR3> VB = *(iter->GetVertices());
			std::vector<D3DXVECTOR3> IB = *(iter->GetIndices());
			D3DXVECTOR3 pos = iter->GetPosition();
			DWORD dwFace;
			FLOAT fBary1, fBary2, fDist;
			BOOL picked = false;
			//float mU, mV;
			//int mIdx;

			for (UINT i = 0; i < IB.size(); ++i)
			{
				// Vertex 정보는 Translate 하기 전인 원점을 기준으로 좌표 설정이 되어있기 때문에
				// 현재 Object의 Position과 더해준 값이 Vertex의 좌표이다.
				// 근데 Picking 로직이 상당히 퍼포먼스가 저질인 것 같은데; 임시로 만들어 놓은거라..
				// 지금은 위치 이동만 반영했고 회전에 따른 Vertex 위치 변화도 만들어야 하는데 나중에 하겠음.

				D3DXVECTOR3 a, b, c;
				a = VB[(UINT)IB[i].x] + pos;
				b = VB[(UINT)IB[i].y] + pos;
				c = VB[(UINT)IB[i].z] + pos;
				//picked = D3DXIntersectTri(&VB[(UINT)IB[i].x], &VB[(UINT)IB[i].y], &VB[(UINT)IB[i].z], &Origin, &Direction, &fBary1, &fBary2, &fDist);
				picked = D3DXIntersectTri(&a, &b, &c, &Origin, &Direction, &fBary1, &fBary2, &fDist);

				if (picked)
				{
					if (minDistance > fDist)
					{
// 						printf("[%s] ", iter->GetObjectName().c_str());
// 						D3DXVECTOR3 pos = Origin + Direction * fDist;
// 						printf("%f %f %f \n", pos.x, pos.y, pos.z);
						minDistance = fDist;
						result = &*iter;
					}
					//printf("%s\n", iter->GetObjectName().c_str());
					//result->push_back(&*iter);
					break;
				}
			}
		}
		if (intersectPos != nullptr)
			*intersectPos = Origin + Direction * minDistance;

		return result;
	}

	bool Node::CheckIntersectThis(float x, float y, D3DXVECTOR3 *intersectPos)
	{
		float minDistance = 9999.f;

		D3DXVECTOR3 Origin, Direction;
		GetRay(x, y, &Origin, &Direction);

		D3DXVECTOR3 pos = mPosition;
		DWORD dwFace;
		FLOAT fBary1, fBary2, fDist;
		BOOL picked = false;

		for (UINT i = 0; i < mIndices.size(); ++i)
		{
			// Vertex 정보는 Translate 하기 전인 원점을 기준으로 좌표 설정이 되어있기 때문에
			// 현재 Object의 Position과 더해준 값이 Vertex의 좌표이다.
			// 근데 Picking 로직이 상당히 퍼포먼스가 저질인 것 같은데; 임시로 만들어 놓은거라..
			// 지금은 위치 이동만 반영했고 회전에 따른 Vertex 위치 변화도 만들어야 하는데 나중에 하겠음.

			D3DXVECTOR3 a, b, c;
			a = mVertices[(UINT)mIndices[i].x] + pos;
			b = mVertices[(UINT)mIndices[i].y] + pos;
			c = mVertices[(UINT)mIndices[i].z] + pos;
			//picked = D3DXIntersectTri(&VB[(UINT)IB[i].x], &VB[(UINT)IB[i].y], &VB[(UINT)IB[i].z], &Origin, &Direction, &fBary1, &fBary2, &fDist);
			picked = D3DXIntersectTri(&a, &b, &c, &Origin, &Direction, &fBary1, &fBary2, &fDist);

			if (picked)
			{
				if (minDistance > fDist)
					minDistance = fDist;
			}
		}
		if (intersectPos != nullptr)
			*intersectPos = Origin + Direction * minDistance;

		return minDistance != 9999.f ? true : false;
	}

}