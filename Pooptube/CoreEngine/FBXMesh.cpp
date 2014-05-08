#include "stdafx.h"
#include "FBXMesh.h"
#include "ResourceManager.h"

namespace pooptube{

	FBXMesh::FBXMesh() {
	}


	FBXMesh::~FBXMesh(){
	}

	FBXMesh *FBXMesh::Create(const std::string& FilePath) {
		FBXMesh *pMesh(new FBXMesh);
		if (pMesh->Init(FilePath))
			return pMesh;
		else
			return nullptr;
	}

	bool FBXMesh::Init(const std::string& FilePath) {
		//mFbxScene = ResourceManager::GetInstance()->LoadFBX(FilePath);

		return true;
	}

	void FBXMesh::_RenderFBXRecursive(FbxNode* pNode) {
		if (pNode->GetNodeAttribute())
			_RenderFBX(pNode);

		const int lChildCount = pNode->GetChildCount();
		for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
			_RenderFBXRecursive(pNode->GetChild(lChildIndex));
		}
	}

	void FBXMesh::_RenderFBX(FbxNode* pNode) {
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

		if (lNodeAttribute) {
			// All lights has been processed before the whole scene because they influence every geometry.
			if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMarker) {
				//DrawMarker(pGlobalPosition);
			}
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton) {
				//DrawSkeleton(pNode, pParentGlobalPosition, pGlobalPosition);
			}
			// NURBS and patch have been converted into triangluation meshes.
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
				//DrawMesh(pNode, pTime, pAnimLayer, pGlobalPosition, pPose, pShadingMode);
			}
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eCamera) {
				//DrawCamera(pNode, pTime, pAnimLayer, pGlobalPosition);
			}
			else if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNull) {
				//DrawNull(pGlobalPosition);
			}
		}
		else {
			// Draw a Null for nodes without attribute.
			//DrawNull(pGlobalPosition);
		}
	}

	void FBXMesh::_RenderMesh(FbxNode* pNode) {
// 		FbxMesh* lMesh = pNode->GetMesh();
// 		const int lVertexCount = lMesh->GetControlPointsCount();
// 
// 		// No vertex to draw.
// 		if (lVertexCount == 0)
// 			return;
// 
// 		const VBOMesh * lMeshCache = static_cast<const VBOMesh *>(lMesh->GetUserDataPtr());
// 
// 		// If it has some defomer connection, update the vertices position
// 		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
// 			(static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->IsActive();
// 		const bool lHasShape = lMesh->GetShapeCount() > 0;
// 		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
// 		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;
// 
// 		FbxVector4* lVertexArray = NULL;
// 		if (!lMeshCache || lHasDeformation)
// 		{
// 			lVertexArray = new FbxVector4[lVertexCount];
// 			memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
// 		}
// 
// 		if (lHasDeformation)
// 		{
// 			// Active vertex cache deformer will overwrite any other deformer
// 			if (lHasVertexCache)
// 			{
// 				ReadVertexCacheData(lMesh, pTime, lVertexArray);
// 			}
// 			else
// 			{
// 				if (lHasShape)
// 				{
// 					// Deform the vertex array with the shapes.
// 					ComputeShapeDeformation(lMesh, pTime, pAnimLayer, lVertexArray);
// 				}
// 
// 				//we need to get the number of clusters
// 				const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
// 				int lClusterCount = 0;
// 				for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
// 				{
// 					lClusterCount += ((FbxSkin *)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
// 				}
// 				if (lClusterCount)
// 				{
// 					// Deform the vertex array with the skin deformer.
// 					ComputeSkinDeformation(pGlobalPosition, lMesh, pTime, lVertexArray, pPose);
// 				}
// 			}
// 
// 			if (lMeshCache)
// 				lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);
// 		}
// 
// 		glPushMatrix();
// 		glMultMatrixd((const double*)pGlobalPosition);
// 
// 		if (lMeshCache)
// 		{
// 			lMeshCache->BeginDraw(pShadingMode);
// 			const int lSubMeshCount = lMeshCache->GetSubMeshCount();
// 			for (int lIndex = 0; lIndex < lSubMeshCount; ++lIndex)
// 			{
// 				if (pShadingMode == SHADING_MODE_SHADED)
// 				{
// 					const FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lIndex);
// 					if (lMaterial)
// 					{
// 						const MaterialCache * lMaterialCache = static_cast<const MaterialCache *>(lMaterial->GetUserDataPtr());
// 						if (lMaterialCache)
// 						{
// 							lMaterialCache->SetCurrentMaterial();
// 						}
// 					}
// 					else
// 					{
// 						// Draw green for faces without material
// 						MaterialCache::SetDefaultMaterial();
// 					}
// 				}
// 
// 				lMeshCache->Draw(lIndex, pShadingMode);
// 			}
// 			lMeshCache->EndDraw();
// 		}
// 		else
// 		{
// 			// OpenGL driver is too lower and use Immediate Mode
// 			glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
// 			const int lPolygonCount = lMesh->GetPolygonCount();
// 			for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++)
// 			{
// 				const int lVerticeCount = lMesh->GetPolygonSize(lPolygonIndex);
// 				glBegin(GL_LINE_LOOP);
// 				for (int lVerticeIndex = 0; lVerticeIndex < lVerticeCount; lVerticeIndex++)
// 				{
// 					glVertex3dv((GLdouble *)lVertexArray[lMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex)]);
// 				}
// 				glEnd();
// 			}
// 		}
// 
// 		glPopMatrix();
// 
// 		delete[] lVertexArray;
	}



}

