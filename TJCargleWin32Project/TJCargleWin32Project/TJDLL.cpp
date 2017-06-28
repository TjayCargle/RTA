#include "TJDLL.h"

#include <fbxsdk.h>
struct my_fbx_joint
{
	int parent_index = -1;
	FbxNode* node = nullptr;

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
	Mesh * Functions::getMeshFromFbx()
	{
		Mesh * returnMesh = new Mesh();
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
				uint32_t indexCount = firstMesh->GetPolygonVertexCount();
				uint32_t polygonCount = firstMesh->GetPolygonCount();
				int polyVertCount = polygonCount * 3;
				//int polyCount = firstMesh->GetPolygonCount();
				FbxGeometryElement::EMappingMode normalMappingMode = FbxGeometryElement::eNone;
				FbxGeometryElement::EMappingMode uvMappingMode = FbxGeometryElement::eNone;
				bool mAllbyControlPoints = true;
				bool mAllByPolygonVertex = false;
				bool meshHasNormal = firstMesh->GetElementNormalCount() > 0 ? true : false;
				bool meshHasUV = firstMesh->GetElementUVCount() > 0 ? true : false;
				if (meshHasNormal)
				{
					int someCOunt = firstMesh->GetElementNormalCount();
					for (int l = 0; l < someCOunt; l++)
					{
						if (firstMesh->GetElementNormal(l)->GetMappingMode() == FbxGeometryElement::eByControlPoint)
						{
							int yo = 0;
						}
					}
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
				float * lUVs = NULL;
				FbxStringList lUVNames;
				firstMesh->GetUVSetNames(lUVNames);
				const char * lUVName = NULL;
				if (meshHasUV && lUVNames.GetCount())
				{
					lUVs = new float[polyVertCount * 2];
					lUVName = lUVNames[0];
				}
				std::vector<TJVertex> tjVerts;
				std::vector<VTriangle> tjTriangles;

				//	std::vector<TJVertex> polyTjVerts;
				//	std::vector<VTriangle> polyTjTriangles;

				int ployIndexCounter = 0;
				//if (mAllbyControlPoints)
				{
					returnMesh->vertexCount = firstMesh->GetPolygonVertexCount();
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
							tjCurrentVert.u = 1;
							tjCurrentVert.v = 1;
							tjVerts.push_back(tjCurrentVert);
						}

					}

					for (int polyInd = 0; polyInd < polygonCount; polyInd++)
					{
						returnMesh->indexBuffer.push_back(firstMesh->GetPolygonVertex(polyInd, 0));
						returnMesh->indexBuffer.push_back(firstMesh->GetPolygonVertex(polyInd, 1));
						returnMesh->indexBuffer.push_back(firstMesh->GetPolygonVertex(polyInd, 2));
					}
				}
				int amountHit = 0;
			 if (mAllByPolygonVertex)
				{
					returnMesh->vertexCount = polyVertCount;

					for (int lPolygonIndex = 0; lPolygonIndex < polygonCount; ++lPolygonIndex)
					{
						for (int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex)
						{
					
							if (meshHasUV)
							{
								int UVIndex = firstMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
								bool mappedUV;
								firstMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, currentUV, mappedUV);
								
								tjVerts[UVIndex].u =  static_cast<float>(currentUV[0]);
								tjVerts[UVIndex].v = 1- static_cast<float>(currentUV[1]);
								amountHit++;
							}
						
						}
					}
					returnMesh->myVerts = tjVerts;
					for (int i = 0; i < tjVerts.size(); i += 3)
					{
						VTriangle aTriangle;
						aTriangle.a = tjVerts[i];
						aTriangle.b = tjVerts[i + 1];
						aTriangle.c = tjVerts[i + 2];
						tjTriangles.push_back(aTriangle);
					}
					returnMesh->myTriangles.push_back(tjTriangles);


					for (int polyInd = 0; polyInd < polyVertCount; polyInd++)
					{
						returnMesh->indexBuffer.push_back(firstMesh->GetPolygonVertex(polyInd, 0));
						returnMesh->indexBuffer.push_back(firstMesh->GetPolygonVertex(polyInd, 1));
						returnMesh->indexBuffer.push_back(firstMesh->GetPolygonVertex(polyInd, 2));
					}
				}
			}


			//Getting Bone Data
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


			for (int i = 0; i < myFbxJoints.size(); i++)
			{
				FbxNode * parent = myFbxJoints[i].node->GetParent();
				for (int k = 0; k < myFbxJoints.size(); k++)
				{
					if (parent == myFbxJoints[k].node)
					{
						myFbxJoints[i].parent_index = k;
						break;
					}
				}

			}


			std::vector<Point> tempBones;
			for (int i = 0; i < myFbxJoints.size(); i++)
			{
				joint someJoint;
				FbxMatrix aMatrix = myFbxJoints[i].node->EvaluateGlobalTransform();
				//someJoint.myMatrix.e11 = aMatrix.mData[0][0];
				//someJoint.myMatrix.e12 = aMatrix.mData[0][1];
				//someJoint.myMatrix.e13 = aMatrix.mData[0][2];
				//someJoint.myMatrix.e14 = aMatrix.mData[0][3];
				//
				//someJoint.myMatrix.e21 = aMatrix.mData[1][0];
				//someJoint.myMatrix.e22 = aMatrix.mData[1][1];
				//someJoint.myMatrix.e23 = aMatrix.mData[1][2];
				//someJoint.myMatrix.e24 = aMatrix.mData[1][3];
				//
				//someJoint.myMatrix.e31 = aMatrix.mData[2][0];
				//someJoint.myMatrix.e32 = aMatrix.mData[2][1];
				//someJoint.myMatrix.e33 = aMatrix.mData[2][2];
				//someJoint.myMatrix.e34 = aMatrix.mData[2][3];
				//
				//someJoint.myMatrix.e41 = aMatrix.mData[3][0];
				//someJoint.myMatrix.e42 = aMatrix.mData[3][1];
				//someJoint.myMatrix.e43 = aMatrix.mData[3][2];
				//someJoint.myMatrix.e44 = aMatrix.mData[3][3];

				someJoint.pos.x = aMatrix.mData[3][0];
				someJoint.pos.y = aMatrix.mData[3][1];
				someJoint.pos.z = aMatrix.mData[3][2];
				someJoint.pos.w = 1;
				someJoint.parent_index = myFbxJoints[i].parent_index;
				someJoint.pos.parentIndex = myFbxJoints[i].parent_index;
				//gloabalTransforms
				tempBones.push_back(someJoint.pos);
				my_fbx_joint * aJoint = &myFbxJoints[i];
				returnMesh->fbxJoints.push_back(aJoint);
			}
			returnMesh->bones.push_back(tempBones);
			returnMesh->boneVectorSize++;


			return LoadMeshAnimationData(returnMesh);
		}


	}
	Mesh * Functions::LoadMeshAnimationData(Mesh * targetMesh)
	{
		FbxAnimStack * currentAnimStack = myFbxScene->GetCurrentAnimationStack();
		FbxTimeSpan localTimeSpan = currentAnimStack->GetLocalTimeSpan();
		FbxTime theDuration = localTimeSpan.GetDuration();
		FbxLong frameCount = theDuration.GetFrameCount(FbxTime::EMode::eFrames24);
		AnimationClip aCLip;
		aCLip.duration = theDuration.GetSecondDouble();

		int nbAnimLayers = currentAnimStack->GetMemberCount<FbxAnimLayer>();



		for (int i = 1; i < frameCount; i++)
		{
			keyFrame aFrame;
			//aFrame.frameMatrix[j]
			FbxTime keyTime;
			keyTime.SetFrame(i, theDuration.eFrames24);
			aFrame.time = keyTime.GetSecondDouble();

			std::vector<TJVertex> tjVerts;
			std::vector<VTriangle> tjTriangles;
			std::vector<Point> tempBones;
			for (int k = 0; k < targetMesh->fbxJoints.size(); k++)
			{
				TJMatrix myMatrix;
				FbxMatrix aMatrix = targetMesh->fbxJoints[k]->node->EvaluateGlobalTransform(keyTime);
				myMatrix.e11 = aMatrix.mData[0][0];
				myMatrix.e12 = aMatrix.mData[0][1];
				myMatrix.e13 = aMatrix.mData[0][2];
				myMatrix.e14 = aMatrix.mData[0][3];

				myMatrix.e21 = aMatrix.mData[1][0];
				myMatrix.e22 = aMatrix.mData[1][1];
				myMatrix.e23 = aMatrix.mData[1][2];
				myMatrix.e24 = aMatrix.mData[1][3];

				myMatrix.e31 = aMatrix.mData[2][0];
				myMatrix.e32 = aMatrix.mData[2][1];
				myMatrix.e33 = aMatrix.mData[2][2];
				myMatrix.e34 = aMatrix.mData[2][3];

				myMatrix.e41 = aMatrix.mData[3][0];
				myMatrix.e42 = aMatrix.mData[3][1];
				myMatrix.e43 = aMatrix.mData[3][2];
				myMatrix.e44 = aMatrix.mData[3][3];

				aFrame.frameMatrix.push_back(myMatrix);
				Point aPos;
				aPos.x = aMatrix.mData[3][0];
				aPos.y = aMatrix.mData[3][1];
				aPos.z = aMatrix.mData[3][2];
				aPos.w = 1;
				aPos.parentIndex = targetMesh->fbxJoints[k]->parent_index;
				tempBones.push_back(aPos);
				TJVertex aVert;
				aVert.pos = aPos;
				tjVerts.push_back(aVert);


			}




			targetMesh->bones.push_back(tempBones);
			targetMesh->boneVectorSize++;



			aCLip.frames.push_back(aFrame);
			targetMesh->myClip = &aCLip;
		}
		return LoadSkinAnimationData(targetMesh);
	}

	Mesh * Functions::LoadSkinAnimationData(Mesh * targetMesh)
	{
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
			return targetMesh;
		}

		int itemCount = thePose->GetCount();
		std::vector<Point> weightPoints;
		if (thePose->GetNode(0))
		{
			FbxNode * firstNode = thePose->GetNode(0);
			if (firstNode->GetMesh())
			{
				FbxMesh * firstMesh = firstNode->GetMesh();
				int deformerCount = firstMesh->GetDeformerCount();
				if (deformerCount > 0)
				{
					FbxDeformer * firstDeformer = nullptr;
					for (int i = 0; i < deformerCount; i++)
					{
						if (firstMesh->GetDeformer(i))
						{
							FbxDeformer * testDeformer = firstMesh->GetDeformer(i);
							if (testDeformer->GetDeformerType() == FbxDeformer::eSkin)
							{
								firstDeformer = testDeformer;
								break;
							}

						}
					}
					if (firstDeformer)
					{
						FbxSkin * firstSkin = reinterpret_cast<FbxSkin*>(firstDeformer);
						FbxCluster * firstCluster = nullptr;
						int clusterCount = firstSkin->GetClusterCount();
						int mycount = 0;
						for (int i = 0; i < clusterCount; i++)
						{
							std::vector<TJVertex> tempVerts = targetMesh->myVerts;
							std::vector<VTriangle> tempTriangles;
							FbxCluster * aCluster = firstSkin->GetCluster(i);
							FbxNode * linkedNode = aCluster->GetLink();
							int linkedControlCount = aCluster->GetControlPointIndicesCount();
							int * linkedpointArray = aCluster->GetControlPointIndices();
							double * weightsArray = aCluster->GetControlPointWeights();
							FbxNode * selectedNode = nullptr;

							int index = -1;

							for (int j = 0; j < targetMesh->fbxJoints.size(); j++)
							{
								if (linkedNode == targetMesh->fbxJoints[j]->node)
								{
									selectedNode = targetMesh->fbxJoints[j]->node;
									index = j;
									break;
								}
							}
							if (selectedNode)
							{
								
									for (int j = 0; j < linkedControlCount; j++)
									{
										
										tempVerts[linkedpointArray[j]].pos.x += weightsArray[j];
										tempVerts[linkedpointArray[j]].pos.y += weightsArray[j];
										tempVerts[linkedpointArray[j]].pos.z += weightsArray[j];
										//tempVerts[linkedpointArray[j]].pos.id2Affect = mycount;
										mycount++;
									}
								


							}
							for (int i = 0; i < tempVerts.size(); i += 3)
							{
								VTriangle aTriangle;
								aTriangle.a = tempVerts[i];
								aTriangle.b = tempVerts[i + 1];
								aTriangle.c = tempVerts[i + 2];
								tempTriangles.push_back(aTriangle);
							}
							targetMesh->myTriangles.push_back(tempTriangles);


						}
					


					}
				}
			}
		}
		return targetMesh;
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