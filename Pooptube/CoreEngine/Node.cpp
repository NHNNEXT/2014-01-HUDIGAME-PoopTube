
#include "stdafx.h"
#include "Node.h"

#include "ObjectManager.h"
#include "Application.h"

namespace pooptube {
	Node::Node() :
		mIsKeyEventEnabled(false),
		mIsMouseEventEnabled(false) {
	}
	Node::~Node() {
		for (auto& iter = mChildList.begin(); iter != mChildList.end(); iter++) {
			ObjectManager::GetInstance()->RemoveObject((*iter));
		}
		mChildList.clear();
	}

	Node* Node::Create() {
		Node* pNode = new Node();
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
		D3DXMatrixIdentity(&mMatWorld);

		return true;
	}

	void Node::Render() {
		// TODO: 행렬 계산
		LPDIRECT3DDEVICE9 pDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();
		pDevice->SetTransform(D3DTS_WORLD, &mMatWorld);
		//D3DXMatrixIdentity(&mMatWorld);

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

	void Node::Translation(float xTrans, float yTrans, float zTrans) {
		D3DXMATRIXA16 MatTrans;
		D3DXMatrixTranslation(&MatTrans, xTrans, yTrans, zTrans);
		D3DXMatrixMultiply(&mMatWorld, &MatTrans, &mMatWorld);
	}

	void Node::RotationX(float Angle) {
		D3DXMATRIXA16 MatRotate;
		D3DXMatrixRotationX(&MatRotate, Angle);
		D3DXMatrixMultiply(&mMatWorld, &MatRotate, &mMatWorld);
	}

	void Node::RotationY(float Angle) {
		D3DXMATRIXA16 MatRotate;
		D3DXMatrixRotationY(&MatRotate, Angle);
		D3DXMatrixMultiply(&mMatWorld, &MatRotate, &mMatWorld);
	}

	void Node::RotationZ(float Angle) {
		D3DXMATRIXA16 MatRotate;
		D3DXMatrixRotationZ(&MatRotate, Angle);
		D3DXMatrixMultiply(&mMatWorld, &MatRotate, &mMatWorld);
	}







}