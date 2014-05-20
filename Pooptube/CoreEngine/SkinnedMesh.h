/**
* @author 양현찬
* @brief
* 버텍스, 에니메이션, 텍스쳐 등의 정보를 읽어와 렌더하는 객체
* 팩토리 구조. Mesh를 생성한후 objmanager에서 관리
* 사용자가 직접 접근하는 함수
* @date 2014/04/17
* @file SkinnedMesh.h
*/
#pragma once
#include "Node.h"

namespace pooptube {

	class SkinnedMesh;

	HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName);

	struct D3DXFRAME_DERIVED : public D3DXFRAME {
		D3DXMATRIXA16 CombinedTransformationMatrix;
	};

	struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
		LPDIRECT3DTEXTURE9* ppTextures;

		// SkinMesh info             
		LPD3DXMESH pOrigMesh;
		LPD3DXATTRIBUTERANGE pAttributeTable;
		DWORD NumAttributeGroups;
		DWORD NumInfl;
		LPD3DXBUFFER pBoneCombinationBuf;
		D3DXMATRIX** ppBoneMatrixPtrs;
		D3DXMATRIX* pBoneOffsetMatrices;
		DWORD NumPaletteEntries;
		bool UseSoftwareVP;
		DWORD iAttributeSW;
	};

	class CAllocateHierarchy : public ID3DXAllocateHierarchy {
	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
		STDMETHOD(CreateMeshContainer)(THIS_
			LPCSTR Name,
			CONST D3DXMESHDATA *pMeshData,
			CONST D3DXMATERIAL *pMaterials,
			CONST D3DXEFFECTINSTANCE *pEffectInstances,
			DWORD NumMaterials,
			CONST DWORD *pAdjacency,
			LPD3DXSKININFO pSkinInfo,
			LPD3DXMESHCONTAINER *ppNewMeshContainer);
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

		STDMETHOD(SetMA)(THIS_ SkinnedMesh *pMA);

	private:

		SkinnedMesh* mMA;
	};

	class SkinnedMesh : public Node {
	public:
		SkinnedMesh();
		virtual ~SkinnedMesh();

		static SkinnedMesh *Create(const std::wstring& XMeshPath);

		virtual void Render();
		void SetAnimationTrack(DWORD num);
		void DrawFrame(LPD3DXFRAME pFrame);
		void DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);

		virtual void Update(float dTime);
		void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);

		HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
		HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame);

		void ReleaseAttributeTable(LPD3DXFRAME pFrameBase);

		HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer);

	protected:

		bool _Init(const std::wstring& XMeshPath);
		//아래 두 함수는 픽킹, 맵툴용 클라에서는 빼야함
		void InitFrame(LPD3DXFRAME pFrame);
		void InitMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);


	private:

		CAllocateHierarchy			mAlloc;
		ID3DXEffect*				mEffect = nullptr;
		LPD3DXFRAME					mFrameRoot = nullptr;
		ID3DXAnimationController*   mAnimController = nullptr;

		DWORD			            mBehaviorFlags;

		D3DXMATRIXA16*       mBoneMatrices;
		UINT                 mNumBoneMatricesMax;
		bool                 mUseSoftwareVP;
	};


//	class Mesh;
// 
// 	class SkinnedMesh : public Node {
// 	public:
// 		SkinnedMesh();
// 		virtual ~SkinnedMesh();
// 
// 		//@param VertexCount 
// 		//@param PolygonCount
// 		//@param ResourceType
// 		//@return 생성한 Mesh를 반환
// 		//@exception 초기화에 실패하거나 리소스 타입이 null일 경우 nullptr반환
// 		static SkinnedMesh *Create(const std::string& MeshFilePath);
// 		virtual bool Init(const std::string& MeshFilePath);
// 
// 		virtual void Render();
// 		virtual void Update(float dTime);
// 		
// 		Mesh *GetMeshData() const { return mMesh; }
// 
// 	protected:
// 		virtual bool CheckFrustum(); //절두체 컬링 체크
// 		bool _InitFBX( const std::string& MeshFilePath );
// 
// 		std::vector<D3DMATERIAL9>		mMaterial;
// 		std::vector<LPDIRECT3DTEXTURE9>	mTexture;
// 
// 	private:
// 		void _MakeBoundingSphere( D3DXVECTOR3& outSphereCenter, float& outSphereRadius ); // 절두체 컬링용 Bounding Sphere 작성
// 
// 		LPDIRECT3DVERTEXBUFFER9			mMeshVertexBuffer = nullptr;
// 		LPDIRECT3DINDEXBUFFER9			mMeshIndexBuffer = nullptr;
// 
// 		D3DXVECTOR3						mBoundingSphereCenter; // 절두체 컬링용 Bounding Sphere
// 		float							mBoundingSphereRadius; // 절두체 컬링용 Bounding Sphere
// 
// 		Mesh			*mMesh = nullptr;
// 	};

}