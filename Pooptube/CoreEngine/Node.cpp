
#include "stdafx.h"
#include "Node.h"

#include "ObjectManager.h"

namespace pooptube {
	Node::Node() {
	}
	Node::~Node() {
		for (auto& iter = mChildList.begin(); iter != mChildList.end(); iter++) {
			ObjectManager::GetInstance()->RemoveObject((*iter).get());
		}
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

		return true;
	}

	void Node::Render() {

		UpdateMatrix();

		for (auto child : mChildList) {
			if (child->DoRender())
				child->Render();
		}
	}
	void Node::Update(float dTime) {
		for (auto child : mChildList) {
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

	void Node::DisableKeyEvent() {
		if (mIsKeyEventEnabled == true) {
			//this->_RegistrationToKeyEventDispatcher();
			Application::GetInstance()->GetKeyEventDispatcher()->RemoveEventDelegate(this);
			mIsKeyEventEnabled = false;
		}
	}
	void Node::DisableMouseEvent() {
		if (mIsMouseEventEnabled == true) {
			Application::GetInstance()->GetMouseEventDispatcher()->RemoveEventDelegate(this);
			mIsMouseEventEnabled = false;
		}
	}

	void Node::EnableKeyEvent() {
		if (mIsKeyEventEnabled == false) {
			this->_RegistrationToKeyEventDispatcher();
			mIsKeyEventEnabled = true;
		}
	}
	void Node::EnableMouseEvent() {
		if (mIsMouseEventEnabled == false) {
			this->_RegistrationToMouseEventDispatcher();
			mIsMouseEventEnabled = true;
		}
	}

	void Node::_RegistrationToKeyEventDispatcher() {
		Application::GetInstance()->GetKeyEventDispatcher()->AddEventDelegate(this);
	}
	void Node::_RegistrationToMouseEventDispatcher() {
		Application::GetInstance()->GetMouseEventDispatcher()->AddEventDelegate(this);
	}

	void Node::RotationY(float Angle) {
		RotateFrontVectorY(Angle);
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

	void Node::Translation( const D3DXVECTOR3& moveVec ) {
		mPosition += moveVec;
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
	}


	void Node::SetPosition(const D3DXVECTOR3& newPos) {
		mPosition = newPos;
	}

	// agebreak : 아래 함수들은 자주 호출되는 함수들이다. 외적 계산은 비싼 연산이다. 호출할때마다 계산하지 말고, FrontVector가 업데이트 될때, 한번만 연산하고 저장해 두도록 수정할것.
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

	float Node::GetTurnAngle( D3DXVECTOR3 dst)
	{
// 		src.y = dst.y = 0.f;
// 
// 		D3DXVECTOR3 dir = dst - src;
// 		D3DXVECTOR3 Look = GetFrontVector() + src;
// 		float dot = D3DXVec3Dot(&dir, &mFrontVector);
// 		float dis = abs((D3DXVec3Length(&dir) * D3DXVec3Length(&mFrontVector)));
// 		float angle = 0.f;
// 		float det = ((dst.x - src.x) * (Look.z - src.z) - (dst.z - src.z) * (Look.x - src.x));
// 
// 		if (det > 0.f)	det = -1.f; // CW
// 		else			det = 1.f;	// CCW
// 
//  		if (dis != 0.f && ((dot / dis) < 1.f))
//  			angle = det * acos(dot / dis);

		// agebreak : 위의 이상한 공식은 아래처럼 내적와 외적을 사용해서, 깔끔하게 구현할 수 있음. 				
		D3DXVECTOR3 dir = dst - GetPosition();
		D3DXVECTOR3 front = GetFrontVector();
		dir.y = 0.0f;
		D3DXVec3Normalize(&dir, &dir);

		float angle = acos(D3DXVec3Dot(&dir, &front));
		
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &dir, &front);

		return (cross.y > 0) ? angle : -angle;				
	}

	// agebreak : 타겟을 바라보겠다면, 현재 위치와 frontVector가 필요하다.. 그럼 아예 둘다 인자로 받던가, 아님 둘다 인자로 안받아야 한다. 왜 현재 위치는 받으면서 FrontVector는 있는걸 쓰남?
	//bool Node::Turn(D3DXVECTOR3 src, D3DXVECTOR3 dst, float speed)
	bool Node::Turn(D3DXVECTOR3 dst, float speed)
	{
		float precision = 0.05f;
		float angle = GetTurnAngle(dst);

		if (angle > 0.f)
			speed *= -1.f;

		if (angle < precision && angle > -precision)	return false;
		else if (abs(angle) > abs(speed))				RotationY(speed);
		else											RotationY(angle);

		return true;
	}

}