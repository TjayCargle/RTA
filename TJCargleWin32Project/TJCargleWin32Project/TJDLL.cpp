#include "TJDLL.h"

#include <fbxsdk.h>
struct my_fbx_joint
{
	FbxNode* node;
	int parent_index;
};
struct joint
{
	Point pos;
	TJMatrix myMatrix;
	int parent_index;
};
namespace TJDEV5LIB
{
	FbxManager * myFbxManager = nullptr;
	FbxIOSettings * myFbxIOsetter = nullptr;
	FbxImporter * myFbxImporter = nullptr;
	FbxScene * myFbxScene = nullptr;

	double Functions::Add(double a, double b)
	{

		return a + b;
	}
	void Functions::SetupFbxManager()
	{
		myFbxManager = FbxManager::Create();
	}
	void Functions::setFbxIORoot()
	{
		if (myFbxManager == nullptr)
			SetupFbxManager();
		myFbxIOsetter = FbxIOSettings::Create(myFbxManager, IOSROOT);
		myFbxManager->SetIOSettings(myFbxIOsetter);
	}
	bool Functions::setupFbxImporter(const char * filename)
	{
		myFbxImporter = FbxImporter::Create(myFbxManager, "");
		if (!myFbxImporter->Initialize(filename), -1, myFbxManager->GetIOSettings())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	void Functions::importFbxscene(const char * sceneName)
	{
		myFbxScene = FbxScene::Create(myFbxManager, sceneName);
		myFbxImporter->Import(myFbxScene);
		myFbxImporter->Destroy();
		myFbxImporter = nullptr;
	}

	FbxNode * DepthFirtstTraversal(std::vector<FbxNode *> & nodeVector, FbxNode * node, int childCount)
	{
		for (int i = 0; i < childCount; i++)
		{
			if (node->GetChild(i) != nullptr)
			{

				nodeVector.push_back(DepthFirtstTraversal(nodeVector, node->GetChild(i), node->GetChild(i)->GetChildCount()));
			}
		}
		return node;
	}

	void FillVector_DepthFirtstTraversal(std::vector<my_fbx_joint> & theVector, FbxNode * node, int childCount)
	{
		std::vector<FbxNode *> tempVector;
		tempVector.push_back(DepthFirtstTraversal(tempVector, node, node->GetChildCount()));
		for (int i = 0; i < tempVector.size(); i++)
		{
			my_fbx_joint somejoint;
			somejoint.node = tempVector[i];
			theVector.push_back(somejoint);
		}
	}
	Mesh Functions::getMeshFromFbx()
	{
		Mesh returnMesh;
		int scenePoseCount = myFbxScene->GetPoseCount();
		FbxPose * thePose = nullptr;
		for (int i = 0; i < scenePoseCount; i++)
		{
			if (myFbxScene->GetPose(i)->IsBindPose())
			{
				thePose = myFbxScene->GetPose(i);
				break;
			}
		}
		if (thePose == nullptr)
		{
			return returnMesh;
		}

		int itemCount = thePose->GetCount();
		if (thePose->GetNode(0))
		{
			FbxNode * firstNode = thePose->GetNode(0);
			if (firstNode->GetMesh())
			{
				FbxMesh * firstMesh = firstNode->GetMesh();
				int vertextCount = firstMesh->GetControlPointsCount();

				int polyCount = firstMesh->GetPolygonCount();
				FbxGeometryElement::EMappingMode normalMappingMode = FbxGeometryElement::eNone;
				FbxGeometryElement::EMappingMode uvMappingMode = FbxGeometryElement::eNone;
				bool mAllbyControlPoints = true;
				bool mAllByPolygonVertex = false;
				bool meshHasNormal = firstMesh->GetElementNormalCount() > 0 ? true : false;
				bool meshHasUV = firstMesh->GetElementUVCount() > 0 ? true : false;
				if (meshHasNormal)
				{
					normalMappingMode = firstMesh->GetElementNormal(0)->GetMappingMode();
					if (normalMappingMode == FbxGeometryElement::eNone)
						meshHasNormal = false;
					if (meshHasNormal && normalMappingMode != FbxGeometryElement::eByControlPoint)
						mAllbyControlPoints = false;
					if (meshHasNormal && normalMappingMode == FbxGeometryElement::eByPolygonVertex)
						mAllByPolygonVertex = true;
				}
				if (meshHasUV)
				{
					uvMappingMode = firstMesh->GetElementUV(0)->GetMappingMode();
					if (uvMappingMode == FbxGeometryElement::eNone)
						meshHasUV = false;
					if (meshHasUV && uvMappingMode != FbxGeometryElement::eByControlPoint)
						mAllbyControlPoints = false;
					if (meshHasUV && uvMappingMode == FbxGeometryElement::eByPolygonVertex)
						mAllByPolygonVertex = true;
				}
				if (!mAllbyControlPoints)
				{
					vertextCount = vertextCount * 3; //3 because triangles
				}

				const FbxVector4 * ControlPoints = firstMesh->GetControlPoints();

				FbxVector4 currentVertex;
				FbxVector4 currentNormal;
				FbxVector2 currentUV;

				const FbxGeometryElementNormal * normalElement = NULL;
				const FbxGeometryElementUV * uvElement = NULL;
				if (meshHasNormal)
				{
					normalElement = firstMesh->GetElementNormal(0);
				}
				if (meshHasUV)
				{
					uvElement = firstMesh->GetElementUV(0);
				}
				std::vector<TJVertex> tjVerts;
				std::vector<VTriangle> tjTriangles;
				returnMesh.vertexCount = firstMesh->GetPolygonVertexCount();

				for (int ind = 0; ind < vertextCount; ind++)
				{


					// Save the vertex position.
					//if (mAllbyControlPoints)
					{
						TJVertex tjCurrentVert;
						currentVertex = firstMesh->GetControlPointAt(ind);

						tjCurrentVert.pos.x = currentVertex.mData[0];
						tjCurrentVert.pos.y = currentVertex.mData[1];
						tjCurrentVert.pos.z = currentVertex.mData[2];
						tjCurrentVert.pos.w = 1;//currentVertex.mData[3];

						if (meshHasNormal)
						{
							int normalIndex = ind;
							if (normalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
							{
								normalIndex = normalElement->GetIndexArray().GetAt(ind);
							}
							currentNormal = normalElement->GetDirectArray().GetAt(normalIndex);


							tjCurrentVert.normal.x = static_cast<float>(currentNormal[0]);
							tjCurrentVert.normal.y = static_cast<float>(currentNormal[1]);
							tjCurrentVert.normal.z = static_cast<float>(currentNormal[2]);
						}
						if (meshHasUV)
						{
							int UVIndex = ind;
							if (uvElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
							{
								UVIndex = uvElement->GetIndexArray().GetAt(ind);
							}
							currentUV = uvElement->GetDirectArray().GetAt(UVIndex);

							tjCurrentVert.u = static_cast<float>(currentUV[0]);
							tjCurrentVert.v = static_cast<float>(currentUV[1]);
						}
						tjVerts.push_back(tjCurrentVert);
					}

				}


				for (int i = 0; i < tjVerts.size(); i += 3)
				{
					VTriangle aTriangle;
					aTriangle.a = tjVerts[i];
					aTriangle.b = tjVerts[i + 1];
					aTriangle.c = tjVerts[i + 2];
					tjTriangles.push_back(aTriangle);
				}
				returnMesh.myTriangles = tjTriangles;
				uint32_t indexCount = firstMesh->GetPolygonVertexCount();
				uint32_t polygonCount = firstMesh->GetPolygonCount();
				for (int i = 0; i < polygonCount; i++)
				{
					returnMesh.indexBuffer.push_back(firstMesh->GetPolygonVertex(i, 0));
					returnMesh.indexBuffer.push_back(firstMesh->GetPolygonVertex(i, 1));
					returnMesh.indexBuffer.push_back(firstMesh->GetPolygonVertex(i, 2));
				}
			}
			FbxNode * rootNode = nullptr;
			for (int i = 0; i < itemCount; i++)
			{
				FbxNode * tempNode = thePose->GetNode(i);
				if (tempNode->GetSkeleton())
				{
					if (tempNode->GetSkeleton()->IsSkeletonRoot())
					{
						rootNode = tempNode;
						break;
					}
				}
			}
			if (rootNode == nullptr)
			{
				return returnMesh;
			}
			std::vector<my_fbx_joint> myFbxJoints;
			FillVector_DepthFirtstTraversal(myFbxJoints, rootNode, rootNode->GetChildCount());
			FbxVector4 currVertex;

			std::vector<joint> gloabalTransforms;
			for (int i = 0; i < myFbxJoints.size(); i++)
			{
				joint someJoint;
				FbxMatrix aMatrix = myFbxJoints[i].node->EvaluateGlobalTransform();
				someJoint.myMatrix.e11 = aMatrix.mData[0][0];
				someJoint.myMatrix.e12 = aMatrix.mData[0][1];
				someJoint.myMatrix.e13 = aMatrix.mData[0][2];
				someJoint.myMatrix.e14 = aMatrix.mData[0][3];

				someJoint.myMatrix.e21 = aMatrix.mData[1][0];
				someJoint.myMatrix.e22 = aMatrix.mData[1][1];
				someJoint.myMatrix.e23 = aMatrix.mData[1][2];
				someJoint.myMatrix.e24 = aMatrix.mData[1][3];

				someJoint.myMatrix.e31 = aMatrix.mData[2][0];
				someJoint.myMatrix.e32 = aMatrix.mData[2][1];
				someJoint.myMatrix.e33 = aMatrix.mData[2][2];
				someJoint.myMatrix.e34 = aMatrix.mData[2][3];

				someJoint.myMatrix.e41 = aMatrix.mData[3][0];
				someJoint.myMatrix.e42 = aMatrix.mData[3][1];
				someJoint.myMatrix.e43 = aMatrix.mData[3][2];
				someJoint.myMatrix.e44 = aMatrix.mData[3][3];

				someJoint.pos.x = aMatrix.mData[3][0];
				someJoint.pos.y = aMatrix.mData[3][1];
				someJoint.pos.z = aMatrix.mData[3][2];
				someJoint.pos.w = 1;
					//gloabalTransforms
				returnMesh.bones.push_back(someJoint.pos);
			}
		
			return returnMesh;
		}


	}
	void Functions::ReleaseFBXPointers()
	{
		if (myFbxScene != nullptr)
			myFbxScene->Destroy();
		if (myFbxImporter != nullptr)
			myFbxImporter->Destroy();
		if (myFbxIOsetter != nullptr)
			myFbxIOsetter->Destroy();
		if (myFbxManager != nullptr)
			myFbxManager->Destroy();
	}
}