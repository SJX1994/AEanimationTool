/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This program converts any file in a format supported by the FBX SDK 
// into DAE, FBX, 3DS, OBJ and DXF files. 
//
//�����������壺
//FBX��
	//����� FBX SDK �������� Fbx ��ͷ��ʾ����FbxNode, FbxScene, FbxCamera
//p��
	//���ݸ���Ա�����Ĳ�����Сд��P����ͷ��ʾ����pWriteFileFormat, pScene, pFilename
//l��
	//�ֲ�������Сд��L����ͷ��ʾ����lWriteFileFormat, lScene, lFilename
//g��
	//ȫ�ֱ�����Сд��G����ͷ��ʾ����gStart, gStop, gCurrentTime
//m��
	//��Ա���ݣ���Ա��������Сд��M����ͷ��ʾ����mDescription, mImportname, mSelect
//SJX_step:
//ִ������fbx�ļ���ʽת����ת��Ϊ_fbx7ascii.fbx�ɶ��ı���
		// Steps:
		// 1. Initialize SDK objects.
		// 2. Load a file(fbx, obj,...) to a FBX scene.
		// 3. Create a exporter.
		// 4. Retrieve the writer ID according to the description of file format.
		// 5. Initialize exporter with specified file format
		// 6. Export.
		// 8. Destroy the FBX SDK manager
		// 7. Destroy the exporter
//��ת�����fbx�����²�����
	//��������Ԫ����Ϣ
	//�������Ԫ����Ϣ
//�ϲ�����ĳ���Ԫ�أ�����fbx��_fbx7ascii.fbx�ɶ��ı���
/////////////////////////////////////////////////////////////////////////

#include <fbxsdk.h>
#include <iostream>
#include<stdlib.h>
#include<stdio.h>
#include <vector> 
#include "../Common/Common.h"

using namespace std;
using namespace fbxsdk;

//-----------ȫ�ֱ���----------
#define SAMPLE_FILENAME "SJX_FBX_TEST.fbx"
#define SAMPLE_FILENAME1 "SJX_FBX_FIRST.fbx"
#define SAMPLE_FILENAME2 "SJX_FBX_SECOND.fbx"
const char* lFileTypes[] =
{
    "_dae.dae",            "Collada DAE (*.dae)",
    "_fbx7binary.fbx", "FBX binary (*.fbx)",
    "_fbx7ascii.fbx",  "FBX ascii (*.fbx)",
    "_fbx6binary.fbx", "FBX 6.0 binary (*.fbx)",
    "_fbx6ascii.fbx",  "FBX 6.0 ascii (*.fbx)",
    "_obj.obj",            "Alias OBJ (*.obj)",
    "_dxf.dxf",            "AutoCAD DXF (*.dxf)"
};

//-----------����-------------
class SJX_Debug 
{
	public:
		void debug(const char* logString)
		{
			std::cout << "\n" << logString << std::endl;
			int flag;
			flag = std::cin.get();
		}
		void debug(int argc, char** argv)
		{
			std::cout << "�� " << argc << " ����:" << std::endl;
			for (int i = 0; i < argc; ++i) {
				std::cout << argv[i] << std::endl;
			}
			int flag;
			flag = std::cin.get();
		}
		void debug(int logInt)
		{
			std::cout << "\n" << logInt << std::endl;
			int flag;
			flag = std::cin.get();
		}
};
//-------����--------
class FBX_SJX_Process
{
	public:
		const char* lFileAsciiTypeName = "_fbx7ascii.fbx";
		const char* lFileAsciiType = "FBX ascii(*.fbx)";
		fbxsdk::FbxString lFileName;
		//����
		FBX_SJX_Process(fbxsdk::FbxString GFileName)
		{
			lFileName = GFileName;
		}
		//ת��
		bool pFBX2Ascii()
		{
			fbxsdk::FbxString lFilePath("");
			lFilePath = lFileName;
			lResult = FBX_SDK_Init_Checker(lFilePath);
			if (lResult)
			{
				printf("SDK��ʼ����%s\n", mSuccessed);

				lResult = FBX_SDK_Export_Checker();

				if (lResult)
				{
					printf("SDK������%s\n", mSuccessed);
					lExporter->Destroy();
					DestroySdkObjects(lSdkManager, lResult);
					return true;
				}
				else {
					printf("SDK������%s\n", mFailed);
					return false;
				}

				
			}
			else
			{
				printf("SDK��ʼ����%s\n", mFailed);
				return false;
			}
			

		}
		//����
		bool pFBXParse()
		{
			fbxsdk::FbxString lFilePath("");
			lFilePath = lFileName;
			lResult = FBX_SDK_Init_Checker(lFilePath);
			if (lResult)
			{
				printf("SDK��ʼ����%s\n", mSuccessed);
				FBX_SDK_Parse();
				return true;
			}
			else 
			{
				printf("SDK��ʼ����%s\n", mFailed);
				return false;
			}
		}
		//mat & Texture
		bool pFBXmatTex()
		{
			
			//����fbxSdk document
			bool lResult = false;
			 fbxsdk::FbxDocument* lDocument = NULL;
			 fbxsdk::FbxString lFilePath("");
			 lFilePath = lFileName;
			 
			 lResult = FBX_SDK_Init_Checker(lFilePath);

			 if(lResult)
			 {
				 printf("SDK��ʼ����%s\n", mSuccessed);
				 
				 lDocument = fbxsdk::FbxDocument::Create(lSdkManager, "RootDoc");

				 //д��document��fbx
				 printf("CreatDoc��ʼ����%s\n", mSuccessed);
				 CreateDocument(lSdkManager, lDocument);
				 //��������fbx��document
				//��ȡdocument
				//����sdk
				 DestroySdkObjects(lSdkManager, lResult);
			 }
			 else 
			 {
				 printf("SDK��ʼ����%s\n", mFailed);
				 
			 }
			

			
			return true;
		}
	private:
		fbxsdk::FbxManager* lSdkManager = NULL;
		fbxsdk::FbxScene* lScene = NULL;
		fbxsdk::FbxExporter* lExporter = NULL;
		const char mSuccessed[5] = "�ɹ�";
		const char mFailed[5] = "ʧ��";
		bool lResult = true;
		int numTabs = 0;

		bool FBX_SDK_Init_Checker(FbxManager*& pManager)
		{
			//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
			pManager = FbxManager::Create();
			if (!pManager)
			{
				FBXSDK_printf("Error: Unable to create FBX Manager!\n");
				exit(1);
			}
			else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

			//Create an IOSettings object. This object holds all import/export settings.
			FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
			pManager->SetIOSettings(ios);

			return lResult;
		}

		bool FBX_SDK_Init_Checker(fbxsdk::FbxString lFilePath)
		{
			
			InitializeSdkObjects(lSdkManager, lScene);
			lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());

			return lResult;
		}
		
	
		//����
#pragma region
		bool FBX_SDK_Export_Checker()
		{
			
			FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");// ����һ����������
			int lFormat = lSdkManager->GetIOPluginRegistry()->FindWriterIDByDescription(lFileAsciiType);//  �����ļ���ʽ����������д��ID��
			fbxsdk::FbxString mMove = ".fbx";
			lFileName.ReplaceAll(mMove, "");
			fbxsdk::FbxString lName = lFileName + lFileAsciiTypeName;
			lResult = lExporter->Initialize(lName, 1, lSdkManager->GetIOSettings());// ��ʼ����������
			return lExporter->Export(lScene);// Export the scene. ����������
		}
#pragma endregion
		//����
#pragma region
		bool FBX_SDK_Parse()
		{
			// Create the IO settings object.
			FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
			lSdkManager->SetIOSettings(ios);
			// Create an importer using the SDK manager.
			FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
			FbxNode* lRootNode = lScene->GetRootNode();
			if (lRootNode) {
				for (int i = 0; i < lRootNode->GetChildCount(); i++)
					PrintNode(lRootNode->GetChild(i));
			}
			lSdkManager->Destroy();
			return true;
		}
		void PrintNode(FbxNode* pNode) {
			PrintTabs();
			const char* nodeName = pNode->GetName();
			FbxDouble3 translation = pNode->LclTranslation.Get();
			FbxDouble3 rotation = pNode->LclRotation.Get();
			FbxDouble3 scaling = pNode->LclScaling.Get();

			// Print the contents of the node.
			printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
				nodeName,
				translation[0], translation[1], translation[2],
				rotation[0], rotation[1], rotation[2],
				scaling[0], scaling[1], scaling[2]
			);
			numTabs++;

			// Print the node's attributes.
			for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
				PrintAttribute(pNode->GetNodeAttributeByIndex(i));

			// Recursively print the children.
			for (int j = 0; j < pNode->GetChildCount(); j++)
				PrintNode(pNode->GetChild(j));

			numTabs--;
			PrintTabs();
			printf("</node>\n");
		}
		string PrintAttribute(FbxNodeAttribute* pAttribute) {
			if (!pAttribute) return "none";

			fbxsdk::FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
			fbxsdk::FbxString attrName = pAttribute->GetName();
			PrintTabs();
			//std::string mTypeName =typeName.Buffer();
			std::string mTypeNameS = typeName.Buffer();
			// Note: to retrieve the character array of a FbxString, use its Buffer() method.
			printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
			
			return mTypeNameS;
		}
		fbxsdk::FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
			switch (type) {
			case FbxNodeAttribute::eUnknown: return "unidentified";
			case FbxNodeAttribute::eNull: return "null";
			case FbxNodeAttribute::eMarker: return "marker";
			case FbxNodeAttribute::eSkeleton: return "skeleton";
			case FbxNodeAttribute::eMesh: return "mesh";
			case FbxNodeAttribute::eNurbs: return "nurbs";
			case FbxNodeAttribute::ePatch: return "patch";
			case FbxNodeAttribute::eCamera: return "camera";
			case FbxNodeAttribute::eCameraStereo: return "stereo";
			case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
			case FbxNodeAttribute::eLight: return "light";
			case FbxNodeAttribute::eOpticalReference: return "optical reference";
			case FbxNodeAttribute::eOpticalMarker: return "marker";
			case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
			case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
			case FbxNodeAttribute::eBoundary: return "boundary";
			case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
			case FbxNodeAttribute::eShape: return "shape";
			case FbxNodeAttribute::eLODGroup: return "lodgroup";
			case FbxNodeAttribute::eSubDiv: return "subdiv";
			default: return "unknown";
			}
		}
		void PrintTabs() {
			for (int i = 0; i < numTabs; i++)
				printf("\t");
		}
#pragma endregion
		//�ϲ�
		//���ʽ���
		bool CreateDocument(FbxManager* pManager, fbxsdk::FbxDocument* pDocument)
		{
			
			int lCount = 0;
			
			// create document info
			fbxsdk::FbxDocumentInfo* lDocInfo = fbxsdk::FbxDocumentInfo::Create(pManager, "DocInfo");
			lDocInfo->mTitle = "SJX_Example document";
			lDocInfo->mSubject = "SJX_materials and texture.";
			lDocInfo->mAuthor = "SJX_meme";
			lDocInfo->mRevision = "SJX_rev. 1.0";
			lDocInfo->mKeywords = "SJX_Fbx document";
			lDocInfo->mComment = "SJX_no particular comments required.";

			// add the documentInfo
			pDocument->SetDocumentInfo(lDocInfo);

			// NOTE: Objects created directly in the SDK Manager are not visible
			// to the disk save routines unless they are manually connected to the
			// documents (see below). Ideally, one would directly use the FbxScene/FbxDocument
			// during the creation of objects so they are automatically connected and become visible
			// to the disk save routines.
			 lEachNode(pDocument);
			 
			//// add the geometry to the main document.
			 lCount = pDocument->GetRootMemberCount();
			 printf("totle node:   %i", lCount);
			//FbxMesh belongs to lPlane; Material that connect to lPlane

			//// Create sub document to contain materials.
			FbxDocument* lMatDocument = FbxDocument::Create(pManager, "Material");

			
			//// Connect the light sub document to main document
			//pDocument->AddMember(lMatDocument);

			//// Create sub document to contain lights
			//FbxDocument* lLightDocument = FbxDocument::Create(pManager, "Light");
			//CreateLightDocument(pManager, lLightDocument);
			//// Connect the light sub document to main document
			//pDocument->AddMember(lLightDocument);

			//lCount = pDocument->GetMemberCount();       // lCount = 5 : 3 add two sub document

			//// document can contain animation. Please refer to other sample about how to set animation
			//pDocument->CreateAnimStack("PlanAnim");

			//lCount = pDocument->GetRootMemberCount();  // lCount = 1: only the lPlane
			//lCount = pDocument->GetMemberCount();      // lCount = 7: 5 add AnimStack and AnimLayer
			//lCount = pDocument->GetMemberCount<FbxDocument>();    // lCount = 2

			return true;
		}
		void lEachNode(fbxsdk::FbxDocument* pDocument)
		{
			
			
			FbxNode* mTmp = lScene->GetRootNode();
			// add the geometry to the main document.
			pDocument->AddRootMember(mTmp);
			// add material object to the sub document
			 // get material to document
			/*const int lMaterialCount = mTmp->GetMaterialCount();
			for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
			{
				FbxSurfaceMaterial * lMaterial = mTmp->GetMaterial(lMaterialIndex);
				pDocument->AddMember(lMaterial);
			}*/
			
			/*const char* nodeName = mTmp->GetName();
			printf("nnn:: %s", nodeName);*/
		
			for (int i = 0; i < mTmp->GetChildCount(); i++)
			{
				
				string mType = PrintAttribute(mTmp->GetNodeAttributeByIndex(i));
				GetChildNode(mTmp->GetChild(i), pDocument);
				printf("���� %s \n", mType);
				
				
			}
				
			
			
		}
		void GetChildNode(FbxNode* pNode, fbxsdk::FbxDocument* pDocument) {
			PrintTabs();
			string mType = PrintAttribute(pNode->GetNodeAttributeByIndex(0));
			// add the geometry to the main document.
			pDocument->AddRootMember(pNode);
			const char* nodeName = pNode->GetName();
			char ch[20];
			strcpy(ch, mType.c_str());
			printf("���� %s \n", ch);
			if (mType == "mesh")
			{
				printf("abc::");
			}
			// Print the contents of the node.
			printf("<node name='%s' >\n",
				nodeName
			);
			numTabs++;

			// Recursively print the children.
			for (int j = 0; j < pNode->GetChildCount(); j++)
			{
			
				string mType = PrintAttribute(pNode->GetNodeAttributeByIndex(j));
				GetChildNode(pNode->GetChild(j),pDocument);

				printf("���� %s \n", mType);
				if (mType == "mesh")
				{
					printf("����mesh");

					// get material to document
					/*const int lMaterialCount = mTmp->GetMaterialCount();
					for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
					{
						FbxSurfaceMaterial * lMaterial = mTmp->GetMaterial(lMaterialIndex);
						pDocument->AddMember(lMaterial);
						fbxsdk::FbxString mMatName = lMaterial->GetName();

						printf("����mat��%s", mMatName.Buffer());
					}*/

				}
			}

			numTabs--;
			PrintTabs();
			printf("</node>\n");
		}
		

};

/**
 * Load a scene given an FbxManager, a FbxScene, and a valid filename.
 */
int LoadSceneTest(FbxManager* pSdkManager, FbxScene* pScene, char* filename) {
	// Create the io settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(pSdkManager, IOSROOT);
	pSdkManager->SetIOSettings(ios);

	// Create an importer using our sdk manager.
	FbxImporter* lImporter = FbxImporter::Create(pSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(filename, -1, pSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		lImporter->Destroy();
		return -1;
	}

	// Import the contents of the file into the scene.
	lImporter->Import(pScene);

	// The file has been imported; we can get rid of the importer.
	lImporter->Destroy();
	return 0;
}
int main(int argc, char** argv)
{
	//argc: count ����Ĳ���
	//argv: vector ����ľ���ֵ
	
	//����ģʽ
#pragma region
	int i;
	fbxsdk::FbxString lFilePath("");
	for ( i = 1; i < argc; ++i) { 	//��0�����Լ����Դ�1��ʼ
		
		
		if (fbxsdk::FbxString(argv[i]) == "-h" || fbxsdk::FbxString(argv[i]) == "-help")
		{
			char help[71] = "������� -s ���� -single ��ʼ������Ϊ��SJX_FBX_TEST����ʵ��fbx";
			char help2[82] = "������� -s ���� -single �� �ո� �� �ļ���.fbx ��ʼ������Ϊ���ļ������ĵ���fbx";
			char help3[82] = "������� -v ���� -view �� �ո� �� �ļ���.fbx ��ʼ�۲�ڵ�";
			char help4[92] = "������� -vm ���� -viewMat �� �ո� �� �ļ���.fbx ��ʼ�۲�ڵ�, ���������Լ���������ͼ��";

			printf("\n    ������\n %s \n", help);
			printf(" %s \n", help2);
			printf(" %s \n", help3);
			printf(" %s \n", help4);
		}
		else if (argc ==3)
		{
			//���������Լ���������ͼ��,����unity�Զ�������
			if (fbxsdk::FbxString(argv[i]) == "-vm" || fbxsdk::FbxString(argv[i]) == "-viewMat")
			{
				printf("��ʼ�����������Լ���ͼ \n");
				
			
				
				lFilePath = fbxsdk::FbxString(argv[0]);
				fbxsdk::FbxString mMove = "ConvertScene.exe";
				fbxsdk::FbxString mCustomizeName("");
				mCustomizeName = fbxsdk::FbxString(argv[2]);
				lFilePath.ReplaceAll(mMove, mCustomizeName);

				FBX_SJX_Process mSJX(lFilePath);
				mSJX.pFBXmatTex();
				

			}
			if (fbxsdk::FbxString(argv[i]) == "-s" || fbxsdk::FbxString(argv[i]) == "-single")
			{
				printf("��ʼת���Զ��嵥��fbx \n");
				lFilePath = fbxsdk::FbxString(argv[0]);
				fbxsdk::FbxString mMove = "ConvertScene.exe";
				fbxsdk::FbxString mCustomizeName("");
				mCustomizeName = fbxsdk::FbxString(argv[2]);
				if (!mCustomizeName.IsEmpty())
				{
					if (lFilePath.ReplaceAll(mMove, mCustomizeName))
					{
						FBX_SJX_Process mSJX(lFilePath);
						mSJX.pFBX2Ascii();
					}
				}
				
			}
			if (fbxsdk::FbxString(argv[i]) == "-v" || fbxsdk::FbxString(argv[i]) == "-view")
			{
				printf("��ʼ�����Զ��嵥��fbx \n");

				lFilePath = fbxsdk::FbxString(argv[0]);
				
			
					fbxsdk::FbxString mMove = "ConvertScene.exe";
					fbxsdk::FbxString mCustomizeName("");
					mCustomizeName = fbxsdk::FbxString(argv[2]);
					lFilePath.ReplaceAll(mMove, mCustomizeName);
					printf("·��Ϊ %s \n", lFilePath);
					FBX_SJX_Process mSJX(lFilePath);
					mSJX.pFBXParse();
				
			}
		}
		
		else if (fbxsdk::FbxString(argv[i]) == "-s" || fbxsdk::FbxString(argv[i]) == "-single")
		{
			printf("��ʼת�뵥��fbx \n");

			lFilePath = fbxsdk::FbxString(argv[0]);
			fbxsdk::FbxString mMove = "ConvertScene.exe";
			if (lFilePath.ReplaceAll(mMove, SAMPLE_FILENAME))
			{
				printf("·��Ϊ %s \n", lFilePath);
				FBX_SJX_Process mSJX(lFilePath);
				mSJX.pFBX2Ascii();
			}


		}
		else if (fbxsdk::FbxString(argv[i]) == "-v" || fbxsdk::FbxString(argv[i]) == "-view")
		{
			printf("��ʼ��������fbx \n");

			lFilePath = fbxsdk::FbxString(argv[0]);
			fbxsdk::FbxString mMove = "ConvertScene.exe";
			if (lFilePath.ReplaceAll(mMove, SAMPLE_FILENAME))
			{
				printf("·��Ϊ %s \n", lFilePath);
				FBX_SJX_Process mSJX(lFilePath);
				mSJX.pFBXParse();
			}


		}
		
	}
	


 	SJX_Debug mDebug;
 	mDebug.debug("�������debug��");
 	mDebug.debug(argc, argv);
	if (argc == 1)
	{
		printf("�𾴵����������ã����� -h ��ȡ����");
	}
#pragma endregion

//test zone
#pragma region
	//lFilePath = fbxsdk::FbxString(argv[0]);
	//fbxsdk::FbxString mMove = "ConvertScene.exe";
	//lFilePath.ReplaceAll(mMove, SAMPLE_FILENAME);
	//

	//
	//// Create an SDK manager.
	//FbxManager* lSdkManager = FbxManager::Create();

	////// Create a new scene so it can be populated by the imported file.
	//FbxScene* lCurrentScene = FbxScene::Create(lSdkManager, "My Scene");

	////// Load the scene.
	//LoadSceneTest(lSdkManager, lCurrentScene, lFilePath.Buffer());
	//
	////// Modify the scene. In this example, only one node name is changed.
	//lCurrentScene->GetRootNode()->GetChild(0)->SetName("Test Name q");
	//const char* mName;
	//mName= lCurrentScene->GetRootNode()->GetChild(0)->GetName();
	//printf("mName %s",mName);

	//FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");// ����һ����������

	//lExporter->Initialize(lFilePath.Buffer(), 1, lSdkManager->GetIOSettings());// ��ʼ����������
	//lExporter->Export(lCurrentScene);// Export the scene. ����������

	//lSdkManager->Destroy();
#pragma endregion

    return 0;
}

