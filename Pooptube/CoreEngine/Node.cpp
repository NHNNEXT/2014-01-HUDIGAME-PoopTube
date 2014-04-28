
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
		
		D3DXVECTOR3 view = mFrontPoint - mPosition;
		D3DXMATRIXA16 matrix;

		D3DXMatrixRotationY(&matrix, angle);
		D3DXVec3TransformCoord(&view, &view, &matrix);

		mFrontPoint = mPosition + view;
	}

	void Node::Translation( const D3DXVECTOR3& moveVec ) {
		mPosition += moveVec;
		mFrontPoint += moveVec;
	}

	void Node::Translation( float x, float y, float z ) {
		Translation( D3DXVECTOR3( x, y, z ) );
	}

	void Node::SetPosition(const D3DXVECTOR3& newPos) {
		mFrontPoint -= mPosition;
		mPosition = newPos;
		mFrontPoint += mPosition;
	}

	D3DXVECTOR3 Node::GetFrontVector() {
		D3DXVECTOR3 FrontVec = mFrontPoint - mPosition;
		D3DXVec3Normalize(&FrontVec, &FrontVec);
		//y축을 죽여버림
		FrontVec.y = 0.f;
		return FrontVec;
	}

	D3DXVECTOR3 Node::GetRightVector() {
		D3DXVECTOR3 Vec = GetFrontVector();
		D3DXVec3Cross(&Vec, &mUpVec, &Vec);
		return Vec;
	}

	D3DXVECTOR3 Node::GetLeftVector() {
		D3DXVECTOR3 Vec = GetFrontVector();
		D3DXVec3Cross(&Vec, &Vec, &mUpVec);
		return Vec;
	}

	void Node::UpdateMatrix() {
		// TODO: 행렬 계산
		D3DXMATRIXA16	MatWorld;
		D3DXMATRIXA16	MatTrans;
		D3DXMATRIXA16	MatScale;
		D3DXMATRIXA16	MatRotate;

		D3DXMatrixIdentity(&MatWorld);

		//프론트 백터의 값에 따라 회전
		D3DXMatrixLookAtLH(&MatRotate, &mPosition, &mFrontPoint, &mUpVec);
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





}