#pragma once
#include "stdafx.h"

namespace pooptube {

	class FBXManager
	{
	public:
		FBXManager(void);
		~FBXManager(void);

		bool InitSdkObjects();
		void DestroySdkObjects();

		void LoadFBXFile(const char* pFileName);

		// 		void LoadNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer, 
		// 			FbxAMatrix& pParentGlobalPosition,
		// 			FbxPose* pPose, ShadingMode pShadingMode);

	protected:


	private:

		FbxManager*	m_pManager;
		FbxScene*	m_pScene;


	};
}