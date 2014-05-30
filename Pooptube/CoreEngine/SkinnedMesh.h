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
	class MeshData;

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

		STDMETHOD(SetMA)(THIS_ MeshData *pMA);

	private:

		MeshData* mMA;
	};

	class MeshData {
		friend class SkinnedMesh;
	
	public:
		MeshData();
		virtual ~MeshData();

		static MeshData *Create(const std::wstring& XMeshPath);
		bool Init(const std::wstring& XMeshPath);

		ID3DXAnimationController* CloneAnimationController();
		HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, D3DXMESHCONTAINER_DERIVED* pMeshContainer);

		HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase);
		HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame);

		void ReleaseAttributeTable(LPD3DXFRAME pFrameBase);

	private:
		LPDIRECT3DDEVICE9			mDevice;

		CAllocateHierarchy			mAlloc;
		ID3DXEffect*				mEffect = nullptr;
		LPD3DXFRAME					mFrameRoot = nullptr;
		ID3DXAnimationController*   mAnimController = nullptr;

		DWORD			            mBehaviorFlags;

		D3DXMATRIXA16*				mBoneMatrices;
		UINT						mNumBoneMatricesMax;
		bool						mUseSoftwareVP;

	};

	class SkinnedMesh : public Node {
		friend class MeshData;

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

		virtual void		SetScale( const D3DXVECTOR3& newScale ) {
			mScaleVec = newScale;
			_MakeBoundingSphere( mBoundingSphereCenter, mBoundingSphereRadius );
		}
		virtual void		SetScale( float x, float y, float z ) { SetScale( D3DXVECTOR3( x, y, z ) ); }

	protected:

		bool _Init(const std::wstring& XMeshPath);
		//아래 두 함수는 픽킹, 맵툴용 클라에서는 빼야함
		void InitFrame(LPD3DXFRAME pFrame);
		void InitMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);

		void _MakeBoundingSphere( D3DXVECTOR3& outSphereCenter, float& outSphereRadius );
		bool _CheckFrustum();

	private:

		MeshData*					mMeshData = nullptr;
		ID3DXAnimationController*   mAnimController = nullptr;

		DWORD						mCurrentTrack;

		D3DXVECTOR3						mBoundingSphereCenter; // 절두체 컬링용 Bounding Sphere
		float							mBoundingSphereRadius; // 절두체 컬링용 Bounding Sphere
	};

}