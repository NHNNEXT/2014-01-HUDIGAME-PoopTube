
#include "stdafx.h"
#include "Node.h"

#include "ObjectManager.h"

namespace pooptube {
	Node::Node() {
	}
	Node::~Node() {
		for (auto& iter = mChildList.begin(); iter != mChildList.end(); iter++) {
			ObjectManager::GetInstance()->RemoveObject((*iter));
		}
		mChildList.clear();
	}

	std::shared_ptr<Node> Node::Create() {
		std::shared_ptr<Node> pNode(new Node);
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
			child->Render();
		}
	}
	void Node::Update(float dTime) {
		for (auto child : mChildList) {
			child->Update(dTime);
		}
	}

	void Node::AddChild(Node* pChild) {
		mChildList.push_back(pChild);
	}
	void Node::RemoveChild(Node* pChild) {
		for (auto& iter = mChildList.begin(); iter != mChildList.end(); iter++) {
			if ((*iter) == pChild) {
				//(*iter).reset();
				ObjectManager::GetInstance()->RemoveObject((*iter));
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

	void Node::SetPosition(const D3DXVECTOR3& newPos) {
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

}