
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

	Node* Node::Create() {
		Node* pNode = new Node;
		if (pNode->Init()) {
			ObjectManager::GetInstance()->AddObject(pNode);
		}
		else {
			delete pNode;
			pNode = nullptr;
		}

		return pNode;
	}

	bool Node::Init() {
		mDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
		return true;
	}

	void Node::Render() {
		// TODO: ��� ���
		D3DXMATRIXA16	MatWorld;
		D3DXMATRIXA16	MatTrans;
		D3DXMATRIXA16	MatScale;
		D3DXMATRIXA16	MatRotate;
		
		D3DXMatrixIdentity(&MatWorld);

		//������ ��ǥ��
		//����Ʈ ������ ���� ���� ȸ��
		D3DXMatrixLookAtLH(&MatRotate, &mPosition, &mFrontPoint, &mUpVec);
		//������� ������ �����Ա� ������ �����̼��� ��ó�� ����� ��ȯ�� �ʿ䰡 �ִ�.
		//������� �ڽ��� �����̴� ���� �ƴ� �ڽ��� ������ ��� ��ǥ���� �����̵��� �Ǿ��ִ� ����̴�.
		//������� ������� transpose���� ���¿� �����ϴ�.
		MatRotate._41 = MatRotate._42 = MatRotate._43 = 0.f;
		D3DXMatrixTranspose(&MatRotate, &MatRotate);

		D3DXMatrixTranslation(&MatTrans, mPosition.x, mPosition.y, mPosition.z);
		D3DXMatrixScaling(&MatScale, mScaleVec.x, mScaleVec.y, mScaleVec.z);

		MatWorld = MatScale*MatRotate*MatTrans;

		mDevice->SetTransform(D3DTS_WORLD, &MatWorld);

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

	void Node::Translation(float x, float y, float z) {
		mPosition.x += x; mPosition.y += y; mPosition.z += z;
		mFrontPoint.x += x; mFrontPoint.y += y; mFrontPoint.z += z;
	}

	void Node::SetPosition(const D3DXVECTOR3& newPos) {
		mFrontPoint -= mPosition;
		mPosition = newPos;
		mFrontPoint += mPosition;
	}


}