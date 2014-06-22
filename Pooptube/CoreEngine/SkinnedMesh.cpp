#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
#include "resourcemanager.h"
#include "Application.h"

namespace pooptube {

	//문자열 변환 함수 임시로 여기에 둠
	HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName) {
		UINT cbLength;

		if (Name != NULL)
		{
			cbLength = (UINT)strlen(Name) + 1;
			*pNewName = new CHAR[cbLength];
			if (*pNewName == NULL)
				return E_OUTOFMEMORY;
			memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
		}
		else
		{
			*pNewName = NULL;
		}

		return S_OK;
	}

	HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame) {
		HRESULT hr = S_OK;
		D3DXFRAME_DERIVED* pFrame;

		*ppNewFrame = NULL;

		pFrame = new D3DXFRAME_DERIVED;
		if (pFrame == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = AllocateName(Name, &pFrame->Name);
		if (FAILED(hr))
			goto e_Exit;

		// initialize other data members of the frame
		D3DXMatrixIdentity(&pFrame->TransformationMatrix);
		D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

		pFrame->pMeshContainer = NULL;
		pFrame->pFrameSibling = NULL;
		pFrame->pFrameFirstChild = NULL;

		*ppNewFrame = pFrame;
		pFrame = NULL;

	e_Exit:
		delete pFrame;
		return hr;
	}

	HRESULT CAllocateHierarchy::CreateMeshContainer(
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) {

		HRESULT hr;
		D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
		UINT NumFaces;
		UINT iMaterial;
		UINT iBone, cBones;
		LPDIRECT3DDEVICE9 pd3dDevice = NULL;

		LPD3DXMESH pMesh = NULL;

		*ppNewMeshContainer = NULL;

		// this sample does not handle patch meshes, so fail when one is found
		if (pMeshData->Type != D3DXMESHTYPE_MESH)
		{
			hr = E_FAIL;
			goto e_Exit;
		}

		// get the pMesh interface pointer out of the mesh data structure
		pMesh = pMeshData->pMesh;

		// this sample does not FVF compatible meshes, so fail when one is found
		DWORD temp = pMesh->GetFVF();
		if (pMesh->GetFVF() == 0) {
			hr = E_FAIL;
			goto e_Exit;
		}

		// allocate the overloaded structure to return as a D3DXMESHCONTAINER
		pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
		if (pMeshContainer == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}
		memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

		// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
		hr = AllocateName(Name, &pMeshContainer->Name);
		if (FAILED(hr))
			goto e_Exit;

		pMesh->GetDevice(&pd3dDevice);
		NumFaces = pMesh->GetNumFaces();

		//노멀추가 정상작동 확인
		if (!(pMesh->GetFVF() & D3DFVF_NORMAL)) {
			pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

			hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
				pMesh->GetFVF() | D3DFVF_NORMAL,
				pd3dDevice, &pMeshContainer->MeshData.pMesh);
			if (FAILED(hr))
				goto e_Exit;

			pMesh = pMeshContainer->MeshData.pMesh;

			D3DXComputeNormals(pMesh, NULL);
		}
		else {
			pMeshContainer->MeshData.pMesh = pMesh;
			pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

			pMesh->AddRef();
		}

		// allocate memory to contain the material information.  This sample uses
		//   the D3D9 materials and texture names instead of the EffectInstance style materials
		pMeshContainer->NumMaterials = max(1, NumMaterials);
		pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
		pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
		pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
		if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)* NumFaces * 3);
		memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)* pMeshContainer->NumMaterials);

		// if materials provided, copy them
		if (NumMaterials > 0) {
			memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)* NumMaterials);

			for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++) {
				if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL) {
					WCHAR wszBuf[MAX_PATH];
					MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[iMaterial].pTextureFilename, -1, wszBuf, MAX_PATH);
					wszBuf[MAX_PATH - 1] = L'\0';

					//작업폴더 설정 추가
					//코드로 박아넣음 변경 필요
					std::wstring strBuf = L"Model\\";
					strBuf += wszBuf;

					//텍스쳐 로딩
					if (FAILED(D3DXCreateTextureFromFile(pd3dDevice, strBuf.c_str(),
						&pMeshContainer->ppTextures[iMaterial])))
						pMeshContainer->ppTextures[iMaterial] = NULL;

					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
				}
			}
		}
		else {
			pMeshContainer->pMaterials[0].pTextureFilename = NULL;
			memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}

		// if there is skinning information, save off the required data and then setup for HW skinning
		if (pSkinInfo != NULL) {
			// first save off the SkinInfo and original mesh data
			pMeshContainer->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();

			pMeshContainer->pOrigMesh = pMesh;
			pMesh->AddRef();

			// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
			cBones = pSkinInfo->GetNumBones();
			pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
			if (pMeshContainer->pBoneOffsetMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}

			// get each of the bone offset matrices so that we don't need to get them later
			for (iBone = 0; iBone < cBones; iBone++)
			{
				pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
			}

			// GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
			hr = mMA->GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
			if (FAILED(hr))
				goto e_Exit;

		}

		*ppNewMeshContainer = pMeshContainer;
		pMeshContainer = NULL;

	e_Exit:
		SAFE_RELEASE(pd3dDevice);

		// call Destroy function to properly clean up the memory allocated 
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}

		return hr;
	}

	HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
	{
		SAFE_DELETE_ARRAY(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);
		return S_OK;
	}

	HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		UINT iMaterial;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

		SAFE_DELETE_ARRAY(pMeshContainer->Name);
		SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

		// release all the allocated textures
		if (pMeshContainer->ppTextures != NULL) {
			for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++) {
				SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
			}
		}

		SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
		SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
		SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainer->pSkinInfo);
		SAFE_RELEASE(pMeshContainer->pOrigMesh);
		SAFE_DELETE( pMeshContainer->pEffects );
		if( pMeshContainer->pNextMeshContainer != NULL )
			DestroyMeshContainer( pMeshContainer->pNextMeshContainer );
		SAFE_DELETE(pMeshContainer);
		return S_OK;
	}

	STDMETHODIMP CAllocateHierarchy::SetMA(THIS_ MeshData *pMA) {
		mMA = pMA;
		return S_OK;
	}

	MeshData::MeshData() {
	}

	MeshData::~MeshData() {
		ReleaseAttributeTable(mFrameRoot);
		delete[] mBoneMatrices;

		D3DXFrameDestroy(mFrameRoot, &mAlloc);
		//SAFE_RELEASE(mEffect);
		SAFE_RELEASE(mAnimController);
	}

	MeshData * MeshData::Create(const std::wstring& XMeshPath) {
		MeshData *pMesh = new MeshData;
		if (pMesh->Init(XMeshPath))
			return pMesh;
		else
			return nullptr;
	}

	bool MeshData::Init(const std::wstring& XMeshPath) {
		mDevice = Application::GetInstance()->GetSceneManager()->GetRenderer()->GetDevice();

		mAlloc.SetMA(this);

		DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG;

		//여기서 fail이 뜨면 .fx파일의 컴파일 실패일 가능성이 있음
		//fx파일 경로를 박아둠 나중에 수정 필요
// 		if (FAILED(D3DXCreateEffectFromFile(mDevice, L"Shader\\SkinnedMesh.fx", NULL, NULL, dwShaderFlags,
// 			NULL, &mEffect, NULL))) {
// 
// 			MessageBox(NULL, L"Could not HLSL file", L"ERROR", MB_OK);
// 			assert(false);
// 			return false;
// 		}

		mEffect = ResourceManager::GetInstance()->LoadHLSL(L"Shader\\SkinnedMesh.fx");
		if (!mEffect) {
			MessageBox(NULL, L"Could not HLSL file", L"ERROR", MB_OK);
			assert(false);
			return false;
		}

		if (FAILED(D3DXLoadMeshHierarchyFromX(XMeshPath.c_str(), D3DXMESH_MANAGED, mDevice,
			&mAlloc, NULL, &mFrameRoot, &mAnimController))) {

			MessageBox(NULL, L"Could not find Xmesh file", L"ERROR", MB_OK);
			assert(false);
			return false;
		}

		if (FAILED(SetupBoneMatrixPointers(mFrameRoot))) {
			MessageBox(NULL, L"SetupBoneMatrixPointers Error!", L"ERROR", MB_OK);
			assert(false);
			return false;
		}

		// Obtain the behavior flags
		D3DDEVICE_CREATION_PARAMETERS cp;
		mDevice->GetCreationParameters(&cp);
		mBehaviorFlags = cp.BehaviorFlags;

		return true;
	}

	HRESULT MeshData::GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer) {
		HRESULT hr = S_OK;
		D3DCAPS9 d3dCaps;
		pd3dDevice->GetDeviceCaps(&d3dCaps);

		if (pMeshContainer->pSkinInfo == NULL)
			return hr;

		mUseSoftwareVP = false;

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

		//SoftWare***************************************
// 		hr = pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
// 			pd3dDevice, &pMeshContainer->MeshData.pMesh);
// 		if (FAILED(hr))
// 			goto e_Exit;
// 
// 		hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
// 		if (FAILED(hr))
// 			goto e_Exit;
// 
// 		delete[] pMeshContainer->pAttributeTable;
// 		pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
// 		if (pMeshContainer->pAttributeTable == NULL)
// 		{
// 			hr = E_OUTOFMEMORY;
// 			goto e_Exit;
// 		}
// 
// 		hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
// 		if (FAILED(hr))
// 			goto e_Exit;
// 
// 		// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
// 		if (mNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
// 		{
// 			mNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();
// 
// 			// Allocate space for blend matrices
// 			delete[] mBoneMatrices;
// 			mBoneMatrices = new D3DXMATRIXA16[mNumBoneMatricesMax];
// 			if (mBoneMatrices == NULL)
// 			{
// 				hr = E_OUTOFMEMORY;
// 				goto e_Exit;
// 			}
// 		}

//HLSL**********************************************************
// Get palette size
// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
// (96 - 9) /3 i.e. Maximum constant count - used constants 
		UINT MaxMatrices = 26;
		pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
		if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
			pMeshContainer->UseSoftwareVP = false;
			Flags |= D3DXMESH_MANAGED;
		}
		else
		{
			pMeshContainer->UseSoftwareVP = true;
			mUseSoftwareVP = true;
			Flags |= D3DXMESH_SYSTEMMEM;
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
			(
			pMeshContainer->pOrigMesh,
			Flags,
			pMeshContainer->NumPaletteEntries,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;


		// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
		DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
			D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
		{
			LPD3DXMESH pMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF,
				pd3dDevice, &pMesh);
			if (!FAILED(hr))
			{
				pMeshContainer->MeshData.pMesh->Release();
				pMeshContainer->MeshData.pMesh = pMesh;
				pMesh = NULL;
			}
		}

		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		LPD3DVERTEXELEMENT9 pDeclCur;
		hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
		//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
		//          this is more of a "cast" operation
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
		if (mNumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones()) {
			mNumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

			// Allocate space for blend matrices
			delete[] mBoneMatrices;
			mBoneMatrices = new D3DXMATRIXA16[mNumBoneMatricesMax];
			if (mBoneMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}
		}

	e_Exit:
		return hr;
	}

	HRESULT MeshData::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase) {
		UINT iBone, cBones;
		D3DXFRAME_DERIVED* pFrame;

		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

		// if there is a skinmesh, then setup the bone matrices
		if (pMeshContainer->pSkinInfo != NULL) {
			cBones = pMeshContainer->pSkinInfo->GetNumBones();

			pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
			if (pMeshContainer->ppBoneMatrixPtrs == NULL)
				return E_OUTOFMEMORY;

			for (iBone = 0; iBone < cBones; iBone++)
			{
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(mFrameRoot,
					pMeshContainer->pSkinInfo->GetBoneName(iBone));
				if (pFrame == NULL)
					return E_FAIL;

				pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}

		return S_OK;
	}

	HRESULT MeshData::SetupBoneMatrixPointers(LPD3DXFRAME pFrame) {
		HRESULT hr;

		if (pFrame->pMeshContainer != NULL)
		{
			hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
			if (FAILED(hr))
				return hr;
		}

		if (pFrame->pFrameSibling != NULL)
		{
			hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
			if (FAILED(hr))
				return hr;
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
			if (FAILED(hr))
				return hr;
		}

		return S_OK;
	}

	void MeshData::ReleaseAttributeTable(LPD3DXFRAME pFrameBase) {
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer;

		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		while (pMeshContainer != NULL)
		{
			delete[] pMeshContainer->pAttributeTable;

			pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainer->pNextMeshContainer;
		}

		if (pFrame->pFrameSibling != NULL)
		{
			ReleaseAttributeTable(pFrame->pFrameSibling);
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			ReleaseAttributeTable(pFrame->pFrameFirstChild);
		}
	}

	ID3DXAnimationController* MeshData::CloneAnimationController() {
		ID3DXAnimationController* newController = nullptr;

		if (mAnimController == nullptr)
			return nullptr;

		mAnimController->AddRef();

		if(FAILED(mAnimController->CloneAnimationController(
			mAnimController->GetMaxNumAnimationOutputs(),
			mAnimController->GetMaxNumAnimationSets(),
			mAnimController->GetMaxNumTracks(),
			mAnimController->GetMaxNumEvents(),
			&newController))) {
			
			MessageBox(NULL, L"Clone Animation Controller Error!", L"ERROR", MB_OK);
			assert(false);
			return NULL;
		}

		return newController;
	}

	SkinnedMesh::SkinnedMesh() {
	}

	SkinnedMesh::~SkinnedMesh() {
		SAFE_RELEASE(mAnimController);
/*		delete[] mBoneMatrices;*/
	}

	SkinnedMesh * SkinnedMesh::Create(const std::wstring& XMeshPath) {
		SkinnedMesh *pMesh = new SkinnedMesh;
		if (pMesh->_Init(XMeshPath))
			return pMesh;
		else
			return nullptr;
	}

	void SkinnedMesh::Render() {
		if( _CheckFrustum() == false ) return; // 절두체 컬링
		Application::GetInstance()->GetSceneManager()->GetRenderer()->mRenderedMeshNum++;

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

		//mDevice->SetTransform(D3DTS_WORLD, &MatWorld);
		mMeshData->mEffect->SetMatrix("mWorld", &MatWorld);

		D3DXMATRIX matProj;
		D3DXMATRIX matView;
		D3DLIGHT9 mLight;

		mDevice->GetTransform(D3DTS_VIEW, &matView);
		mDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		mDevice->GetLight(0, &mLight);

		D3DXMatrixMultiply(&matView, &matView, &matProj);
		D3DXVECTOR4 vLightDir(mLight.Direction.x, mLight.Direction.y, mLight.Direction.z, 0.f);

		//쉐이더 추가시 추가해야함
		mMeshData->mEffect->SetMatrix("mViewProj", &matView);
		
		DrawFrame(mMeshData->mFrameRoot);
		UpdateFrameMatrices(mMeshData->mFrameRoot, &MatWorld);
	}

	void SkinnedMesh::Update(float dTime) {

		if (mAnimController)
			mAnimController->AdvanceTime(dTime, NULL);
	}

	bool SkinnedMesh::_Init(const std::wstring& XMeshPath) {

		Node::Init();

		mMeshData = ResourceManager::GetInstance()->LoadSkinnedMesh(XMeshPath);

		mAnimController = mMeshData->CloneAnimationController();

		//맵툴용 함수 클라에서는 꺼야함
		//InitFrame(mMeshData->mFrameRoot);

		// 경계구 작성
		//_MakeBoundingSphere( mBoundingSphereCenter, mBoundingSphereRadius );

		return true;
	}

	void SkinnedMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix) {
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

		if (pParentMatrix != NULL)
			D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
		else
			pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

		if (pFrame->pFrameSibling != NULL)
			UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);

		if (pFrame->pFrameFirstChild != NULL)
			UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}

	void SkinnedMesh::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase) {
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
		UINT iMaterial;
		UINT iAttrib;
		UINT iMatrixIndex;
		UINT iPaletteEntry;
		LPD3DXBONECOMBINATION pBoneComb;
		D3DXMATRIXA16 matTemp;
		D3DCAPS9 d3dCaps;
		mDevice->GetDeviceCaps(&d3dCaps);

		// first check for skinning
		if (pMeshContainer->pSkinInfo != NULL) {

			//SoftWare**************************************
			D3DXMATRIX Identity;
			DWORD cBones = pMeshContainer->pSkinInfo->GetNumBones();
			DWORD iBone;
			PBYTE pbVerticesSrc;
			PBYTE pbVerticesDest;
// 
// 			// set up bone transforms
// 			for (iBone = 0; iBone < cBones; ++iBone)
// 			{
// 				D3DXMatrixMultiply (
// 					&mMeshData->mBoneMatrices[iBone],                 // output
// 					&pMeshContainer->pBoneOffsetMatrices[iBone],
// 					pMeshContainer->ppBoneMatrixPtrs[iBone]
// 					);
// 			}
// 
// 			// set world transform
// 			D3DXMatrixIdentity(&Identity);
// 			mDevice->SetTransform(D3DTS_WORLD, &Identity);
// 
// 			pMeshContainer->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);
// 			pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);
// 
// 			// generate skinned mesh
// 			pMeshContainer->pSkinInfo->UpdateSkinnedMesh(mMeshData->mBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest);
// 
// 			pMeshContainer->pOrigMesh->UnlockVertexBuffer();
// 			pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
// 
// 			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++) {
// 
// 				mDevice->SetMaterial(&(
// 					pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D));
// 				mDevice->SetTexture(0,
// 					pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
// 
// 				//DWORD temp = pMeshContainer->pAttributeTable[iAttrib].AttribId;
// 				pMeshContainer->MeshData.pMesh->DrawSubset(pMeshContainer->pAttributeTable[iAttrib].AttribId);
// 
// 				Application::GetInstance()->UpdateDPCall();
// 			}
// 			mDevice->SetTexture(0, NULL);

			//HLSL**********************************
			if (pMeshContainer->UseSoftwareVP)
			{
				// If hw or pure hw vertex processing is forced, we can't render the
				// mesh, so just exit out.  Typical applications should create
				// a device with appropriate vertex processing capability for this
				// skinning method.
// 				if (mBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
// 					return;

				mDevice->SetSoftwareVertexProcessing(TRUE);
			}

			mMeshData->mEffect->SetTechnique("t0");

			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer
				());
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				// first calculate all the world matrices
				for (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
					if (iMatrixIndex != UINT_MAX) {

						D3DXMATRIX g_matView;
						mDevice->GetTransform(D3DTS_VIEW, &g_matView);

						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
						D3DXMatrixMultiply(&mMeshData->mBoneMatrices[iPaletteEntry], &matTemp, &g_matView);

						D3DXMatrixMultiply(&mMeshData->mBoneMatrices[iPaletteEntry], &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
					}
				}
				mMeshData->mEffect->SetMatrixArray("mWorldMatrixArray", mMeshData->mBoneMatrices,
					pMeshContainer->NumPaletteEntries);

				// Sum of all ambient and emissive contribution
				D3DXCOLOR color1(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient);
				D3DXCOLOR color2(.25, .25, .25, 1.0);
				D3DXCOLOR ambEmm;
				D3DXColorModulate(&ambEmm, &color1, &color2);
				ambEmm += D3DXCOLOR(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Emissive);

				// set material color properties 
				/*
				mMeshData->mEffect->SetVector("MaterialDiffuse",
					(D3DXVECTOR4*)&(
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse));
				mMeshData->mEffect->SetVector("MaterialAmbient", (D3DXVECTOR4*)&ambEmm);
				*/

				// setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
				//mDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
				mMeshData->mEffect->SetTexture("mTexture", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
				
				// Set CurNumBones to select the correct vertex shader for the number of bones
				mMeshData->mEffect->SetInt("CurNumBones", pMeshContainer->NumInfl - 1);

				// Start the effect now all parameters have been updated
				UINT numPasses;
				mMeshData->mEffect->Begin(&numPasses, D3DXFX_DONOTSAVESTATE);
				for (UINT iPass = 0; iPass < numPasses; iPass++) {
					mMeshData->mEffect->BeginPass(iPass);

					// draw the subset with the current world matrix palette and material state
					pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
					Application::GetInstance()->UpdateDPCall();

					mMeshData->mEffect->EndPass();
				}

				mMeshData->mEffect->End();

				mDevice->SetVertexShader(NULL);
			}

			// remember to reset back to hw vertex processing if software was required
			if (pMeshContainer->UseSoftwareVP)
				mDevice->SetSoftwareVertexProcessing(FALSE);
		}
		else  // standard mesh, just draw it after setting material properties
		{
			mDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

			for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++) {
				mDevice->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
				mDevice->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
				pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
			}
		}
	}

	void SkinnedMesh::DrawFrame(LPD3DXFRAME pFrame) {
		LPD3DXMESHCONTAINER pMeshContainer;

		pMeshContainer = pFrame->pMeshContainer;
		while (pMeshContainer != NULL)
		{
			DrawMeshContainer(pMeshContainer, pFrame);

			pMeshContainer = pMeshContainer->pNextMeshContainer;
		}

		if (pFrame->pFrameSibling != NULL)
		{
			DrawFrame(pFrame->pFrameSibling);
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			DrawFrame(pFrame->pFrameFirstChild);
		}
	}

	void SkinnedMesh::SetAnimationTrack(DWORD num) {
		ID3DXAnimationSet*  Wave = NULL;

		mAnimController->GetAnimationSet(num, &Wave);
		mAnimController->SetTrackAnimationSet(0, Wave);
		mAnimController->SetTrackEnable(0, TRUE);
		mAnimController->ResetTime();
	}

	void SkinnedMesh::InitFrame(LPD3DXFRAME pFrame) {
		LPD3DXMESHCONTAINER pMeshContainer;

		pMeshContainer = pFrame->pMeshContainer;
		while (pMeshContainer != NULL)
		{
			InitMeshContainer(pMeshContainer, pFrame);

			pMeshContainer = pMeshContainer->pNextMeshContainer;
		}

		if (pFrame->pFrameSibling != NULL)
		{
			InitFrame(pFrame->pFrameSibling);
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			InitFrame(pFrame->pFrameFirstChild);
		}
	}

	void SkinnedMesh::InitMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase) {
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

		int VerticesNum = pMeshContainer->MeshData.pMesh->GetNumVertices();
		BYTE* vertexBuffer;
		DWORD numBytesPerVertex = pMeshContainer->MeshData.pMesh->GetNumBytesPerVertex();
		unsigned int offset = D3DXGetFVFVertexSize(pMeshContainer->MeshData.pMesh->GetFVF());

		pMeshContainer->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertexBuffer);
		for (WORD i = 0; i < VerticesNum; i++)
			mVertices.push_back(*((D3DXVECTOR3*)(vertexBuffer + i * offset)));
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();


		void *pIB;
		int IndicesNum = pMeshContainer->MeshData.pMesh->GetNumFaces();
		WORD *indexBuffer = new WORD[IndicesNum * 3];

		pMeshContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIB);
		memcpy(indexBuffer, pIB, sizeof(WORD)*IndicesNum * 3);

		for (int i = 0; i < IndicesNum; ++i)
			mIndices.push_back(D3DXVECTOR3(indexBuffer[i * 3], indexBuffer[i * 3 + 1], indexBuffer[i * 3 + 2]));

		pMeshContainer->MeshData.pMesh->UnlockIndexBuffer();
		delete[]indexBuffer;
	}
	void SkinnedMesh::_MakeBoundingSphere( D3DXVECTOR3& outSphereCenter, float& outSphereRadius )
	{
		std::vector<DirectX::XMFLOAT3> vertices;

		// 버텍스 정보 가져오기
		D3DXVECTOR3 tempVec;
		for( auto& vec : mVertices )
			vertices.push_back( DirectX::XMFLOAT3( vec.x*mScaleVec.x, vec.y*mScaleVec.y, vec.z*mScaleVec.z ) );

		// 경계구 작성
		DirectX::BoundingSphere sphere;
		DirectX::BoundingSphere::CreateFromPoints( sphere, mVertices.size(), &vertices[0], sizeof( DirectX::XMFLOAT3 ) );
		outSphereCenter = D3DXVECTOR3( sphere.Center.x, sphere.Center.y, sphere.Center.z );
		outSphereRadius = sphere.Radius;
	}
	bool SkinnedMesh::_CheckFrustum()
	{
		D3DXVECTOR3 boundingSpherePos = mBoundingSphereCenter + GetPosition();
		for( auto& plane : Application::GetInstance()->GetSceneManager()->GetRenderer()->GetFrustumPlane() ){
			if( plane.a * boundingSpherePos.x + plane.b * boundingSpherePos.y + plane.c * boundingSpherePos.z + plane.d >= mBoundingSphereRadius )
				return false;
		}
		return true;
	}
}

