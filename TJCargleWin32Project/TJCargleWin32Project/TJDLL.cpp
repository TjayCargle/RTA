#include "TJDLL.h"

#include <fbxsdk.h>

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
		FbxNode * rootNode = nullptr;
		for (int i = 0; i < itemCount; i++)
		{
			FbxNode * tempNode = thePose->GetNode(i);
		}
		
	}

	
}
