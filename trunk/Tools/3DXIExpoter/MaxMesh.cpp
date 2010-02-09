/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "stdafx.h"
#include "MaxMesh.h"
#include "SkeletonExp.h"
#include "MaxMeshHelper.h"
void CMaxMesh::_InitData()
{
	m_pGameObject = NULL;
	m_pGameMesh = NULL;
	m_pGameMaterial = NULL;
	m_pNode = NULL;
	m_MatName = L"";
	m_pGameNode = NULL;
	m_pGameMesh = NULL;
	m_pGameObject = NULL;
	m_pMorpModifer = NULL;
	m_pSkinModifer = NULL;
	m_pGameMaterial = NULL;	
	m_CurrentTime = 0;
	m_CurrentFrame= 0;			
	m_pMaxMesh = NULL;
	m_bMirrored = false;
	m_bFaceInCreated = false;
}
CMaxMesh::~CMaxMesh()
{
	//�˳���ʱ�򲻻��Զ�������ȻCMaxMesh����һ��ֵ������ȫû��.
	//Free();
}

CMaxMesh::CMaxMesh()
{
	_InitData();
}

bool CMaxMesh::Free()
{
	if(m_pGameNode) 
	{
		m_pGameNode->ReleaseIGameObject();
	}
	size_t nSubMesh = m_vSubMeshs.size();
	for(size_t i = 0 ; i <  nSubMesh; i ++)
	{
		CMaxSubMesh& subMesh = m_vSubMeshs[i];
		subMesh.Free();
	}
	m_vSubMeshs.clear();
	m_UsedBones.clear();
	m_VertexData.clear();
	m_MatsExport.clear();
	m_BoundBox.clear(10000000.0f);
	
	m_MorphTargets.clear();
	m_vGameMaterials.clear();
	m_MaxVertexIndexs.clear();
	m_ActiveChannels.Resize(0);
	_InitData();
	return true;
}
CMaxMesh::CMaxMesh(INode* pNode , CSkeletonExporter* pSkeleton)
{
	_InitData();
	Init(pNode , pSkeleton);
}

void  CMaxMesh::SetScale(float scale)
{
	m_MeshDataFlag.m_fScale = scale;
}

void  CMaxMesh::SetColorScale(float r_s, float g_s , float b_s , float a_s)
{
	m_MeshDataFlag.m_fColorScale[0] = r_s;
	m_MeshDataFlag.m_fColorScale[1] = g_s;
	m_MeshDataFlag.m_fColorScale[2] = b_s;
	m_MeshDataFlag.m_fColorScale[3] = a_s;
}

bool CMaxMesh::Init(INode* pNode  , CSkeletonExporter* pSkeleton)
{
	Free();
	assert(pNode);
	m_pNode     = pNode;
	if(CreateMesh(0 , pSkeleton) == false)
		return false;

	if(false == CreateFaces() )
		return false;
	return true;
}

bool CMaxMesh::ParseGameMaterial(IGameMaterial* pGameMaterial , int matID)
{
	//���ȱ�֤�в���
	if(pGameMaterial  )
	{
		if(pGameMaterial->IsMultiType())
		{
			for(int i = 0 ; i < pGameMaterial->GetSubMaterialCount() ; i ++)
			{
				int ID = pGameMaterial->GetMaterialID(i);
                ParseGameMaterial(pGameMaterial->GetSubMaterial(i) ,ID );
			}
		}
		else
		{
			m_vGameMaterials[matID] = pGameMaterial;
		}
	}
	return true;

}

IGameMaterial*  CMaxMesh::FindGameMaterialByMatID(int matID)
{
	vGameMaterials::iterator pos = m_vGameMaterials.find( matID );
	if(pos != m_vGameMaterials.end() )
		return pos->second;
	
	if(m_MatsExport.size() == 0)
		return NULL;

	return m_MatsExport[0].m_pGameMaterial;
}

bool CMaxMesh::CreateFaces()
{
	if(m_bFaceInCreated == true)
		return m_vSubMeshs.size() > 0;

	m_pGameMaterial = m_pGameNode->GetNodeMaterial();
	ParseGameMaterial(m_pGameMaterial , 0 );
	BuildMaterialExport();

	//======================���=======================
	//�����ǲ���Ϊ�յ�
	if(m_vGameMaterials.size() == 0)
	{
		wstring errorString = std::wstring(L"Ӧ�õ�[") + std::wstring(INodeName(m_pNode)) + L"]�ϵĲ��ʸ���Ϊ0"; 
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errorString.c_str() );
		MessageBoxW(GetActiveWindow(),errorString.c_str(),L"����",MB_OK);
		return false;
	}

	m_ActiveChannels = m_pGameMesh->GetActiveMapChannelNum();
	int nMapChannel = m_ActiveChannels.Count() ;
	if(nMapChannel > 12)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , "������12��ͨ����Map Channel , meshName = %s" , m_pNode->GetName() );
		return false;
	}
	//����Material������Mesh
	vMaxVertexIndexMap    VertexIndexMap;
	Tab<int> ActiveFace = m_pGameMesh->GetActiveMatIDs();
	m_vSubMeshs.resize( ActiveFace.Count() );
	int nTatolFace = 0;
	for(int i = 0 ; i < ActiveFace.Count() ; i ++)
	{
		CMaxSubMesh& subMesh = m_vSubMeshs[i];

		subMesh.m_pGameMaterial = NULL;
		subMesh.m_MatID = ActiveFace[i];
		Tab<FaceEx *> FaceList = m_pGameMesh->GetFacesFromMatID(subMesh.m_MatID);
		subMesh.m_nFace = FaceList.Count();
		subMesh.m_Faces = new sFace_t[ subMesh.m_nFace ] ;
		nTatolFace += subMesh.m_nFace;
		if(subMesh.m_nFace > 0)
		{
            FaceEx * GameFace = FaceList[0];
		    subMesh.m_pGameMaterial = m_pGameMesh->GetMaterialFromFace(GameFace);			
		}
		if(subMesh.m_pGameMaterial == NULL)
		{
            subMesh.m_pGameMaterial = FindGameMaterialByMatID(subMesh.m_MatID);
		}

		for(int j = 0 ; j < subMesh.m_nFace ; j ++)
		{
			//���Ǹ����յ�Face��������
			sFace_t& MyFace   = subMesh.m_Faces[j];
			FaceEx * GameFace = FaceList[j];
			for(int k = 0 ; k < 3 ; k ++)
			{
				CMaxVertexIndex maxVertIndex;
				maxVertIndex.smGrp = GameFace->smGrp;
				maxVertIndex.vert  = GameFace->vert[k];
				maxVertIndex.color = GameFace->color[k];
				maxVertIndex.alpha = GameFace->alpha[k];
				maxVertIndex.norm  = GameFace->norm[k];
				maxVertIndex.nMapChannel = nMapChannel;
				maxVertIndex.pGameMtl = subMesh.m_pGameMaterial;
				//�ҳ�����ͨ���Ķ�������
				for(int iCh = 0 ; iCh < nMapChannel ; iCh ++)
				{
					int chIdx = m_ActiveChannels[iCh];
					DWORD _index[3];
					m_pGameMesh->GetMapFaceIndex(chIdx , GameFace->meshFaceIndex , _index);
					maxVertIndex.ChVert[iCh] = _index[k];
				}
				//����
				MyFace.vert[k] = AddMaxVertexIndex(maxVertIndex , VertexIndexMap);
			}
		}
	}
    

	int nFace = m_pGameMesh->GetNumberOfFaces();
	if(nTatolFace < nFace)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"��һ��Mesh�Ķ���û�а󶨲���%d name=%s\n", m_pNode->GetName() );
	}
	for(int i = 0 ; i < nFace ; i ++)
	{
		FaceEx* fc = m_pGameMesh->GetFace(i);
		continue;
	}
	//Face����Ϣ�����ú󣬾Ϳ��Կ�ʼ����Mesh��������Ϣ��
	BuildMeshDesc();
	m_bFaceInCreated = true;
	return m_vSubMeshs.size() > 0;
}

bool CMaxMesh::IsMeshMirror()
{
	Matrix3		ObjMatrix;
	Point3		r1, r2, r3, r12;
	float		Mirror;
	ObjMatrix   = m_pNode->GetObjTMAfterWSM(m_CurrentTime);
	r1          = ObjMatrix.GetRow(0);
	r2          = ObjMatrix.GetRow(1);
	r3          = ObjMatrix.GetRow(2);
	r12         = CrossProd(r1, r2);//����������
	Mirror      = DotProd(r12, r3);//�Ƕȼ���
	return Mirror < 0;
}

bool CMaxMesh::CreateMesh(unsigned int iMaxTime , CSkeletonExporter* pSkeleton)
{

	assert(m_pNode);

	m_CurrentTime    = (int)iMaxTime;
	m_CurrentFrame   = G_MaxEnv().MaxTimeToFrame(iMaxTime);
	G_GameScene()->SetStaticFrame(m_CurrentFrame);

	m_pGameNode   = G_GameScene()->GetIGameNode(m_pNode);
	m_pGameObject = m_pGameNode->GetIGameObject();
	if(m_pGameObject->GetIGameType() == IGameObject::IGAME_MESH)
	{
		m_pGameMesh = (IGameMesh*) m_pGameObject;
	}

	if(m_pGameObject == NULL || m_pGameMesh == NULL)
	{
		return false;
	}


	m_bMirrored = IsMeshMirror();
	m_pMaxMesh  = m_pGameMesh->GetMaxMesh(); 

	IGameNode* pIGameNode = G_GameScene()->GetIGameNode(m_pNode);
	m_pGameObject = pIGameNode->GetIGameObject();
	if(m_pGameObject == NULL)
	{
		MessageBox(GetActiveWindow(),"�������ϵ۰���IGameObject��Ȼ����ʧ���ˣ�����","Faint!",MB_OK);
		return false;
	}
	int nIModifier = m_pGameObject->GetNumModifiers();
	m_pMorpModifer = NULL;
	for(int i = 0 ; i < nIModifier ; i ++)
	{
		IGameModifier* pModifier = m_pGameObject->GetIGameModifier(i);
		if(pModifier->GetModifierType() == IGameModifier::IGAME_MORPHER)
		{
			m_pMorpModifer = (IGameMorpher*)pModifier;
		}
		if(pModifier->GetModifierType() == IGameModifier::IGAME_SKINNING)
		{
			m_pSkinModifer = (IGameSkin*)pModifier;
		}
	}
	if(pSkeleton)
	{
		pSkeleton->AddSkinModifier(m_pSkinModifer);
	}

	m_pGameMesh->InitializeData();
	m_pGameMesh->InitializeBinormalData();
	return this->CreateFaces();
}

int CMaxMesh::AddMaxVertexIndex(CMaxVertexIndex& indx , vMaxVertexIndexMap& _map)
{
	vMaxVertexIndexMap::iterator pos = _map.find(indx);
	if(pos != _map.end() )
	{
		//�ҵ��ˡ�
		return pos->second;
	}
	//û�ҵ�,�ӽ�ȥ
	int nVertex = (int)m_MaxVertexIndexs.size() ;
	m_MaxVertexIndexs.push_back(indx);
	_map[indx] = nVertex;
	return nVertex;
}

int  CMaxMesh::MapChannel2UVIndex(int mapChannel)
{
	Tab<int> ActiveMapChannels = m_pGameMesh->GetActiveMapChannelNum();
	for(int i = 0 ;i < ActiveMapChannels.Count() ; i ++)
	{
		if(ActiveMapChannels[i] == mapChannel)
			return i;
	}
	return 0;
}

Matrix3 CMaxMesh::FindMapChannelTM(int chIdx , IGameMaterial* pMaterial)
{
	size_t nMat = m_MatsExport.size() ;
	for(size_t i = 0 ; i < nMat ; i ++)
	{
		sMaxMatrial_t& mat = m_MatsExport[i];
		if(mat.m_pGameMaterial == pMaterial)
		{
			size_t nTex = mat.m_TexLayers.size();
			for(size_t iTex = 0 ; iTex < nTex ; iTex ++)
			{
				if(mat.m_TexLayers[iTex].m_UVChannel == chIdx)
				{
					return mat.m_TexLayers[iTex].m_UVTransMat;
				}
			}
		}
	}
	Matrix3 idMat;
	idMat.IdentityMatrix();
	return idMat;
}

int  CMaxMesh::MapMatID2MatIndex(IGameMaterial* pMaterial)
{
	size_t nMat = m_MatsExport.size();
	for(size_t i = 0 ;i < nMat ; i ++)
	{
		if(m_MatsExport[i].m_pGameMaterial == pMaterial)
			return (int)i;
	}
	return 0;
}



bool CMaxMesh::BuildMaterialExport()
{
	vGameMaterials::iterator pos = m_vGameMaterials.begin();
	for(;  pos != m_vGameMaterials.end() ; pos ++ )
	{
		IGameMaterial* pGameMaterial = pos->second;
		int            MatID = pos->first;
		Mtl*           pMtl  = pGameMaterial->GetMaxMaterial();

		if(pMtl == NULL)
			continue;
		// ����ǲ���һ����Ч�ĳ����Material
		if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
		{
			StdMat*       pStdMtl = (StdMat*)pMtl;
			sMaxMatrial_t mat;
			std::wstring matName = ToWCHAR( pGameMaterial->GetMaterialName() );
			wcsncpy(mat.m_BaseMat.m_MatName , matName.c_str() , 64);

			if(pMtl->ClassID() != Class_ID(DMTL_CLASS_ID, 0) && pMtl->ClassID() != Class_ID(MULTI_CLASS_ID, 0) )
				return false;

			int nTextureMap = pGameMaterial->GetNumberOfTextureMaps();
			for(int tex_layer = 0 ; tex_layer < nTextureMap/*tex_usage_max*/ ; ++tex_layer)
			{
				IGameTextureMap* pGameTexture = pGameMaterial->GetIGameTextureMap(tex_layer);
				Texmap*          pTextureMap  = pGameTexture->GetMaxTexmap();
				//��ͼ�ǲ��Ǳ�����
				if(pTextureMap != NULL)// && pStdMtl->MapEnabled(tex_layer))
				{
					sMaxTexLayer texLayer;
					std::wstring strFilename;
					strFilename = getTextureName( ToWCHAR(pGameTexture->GetBitmapFileName()) );
					wcsncpy(texLayer.m_TexName,strFilename.c_str(),64);

					int ch = pGameTexture->GetMapChannel();
					texLayer.m_UVChannel = MapChannel2UVIndex(ch);
					pTextureMap->GetUVTransform(texLayer.m_UVTransMat);                            
					mat.m_TexLayers.push_back(texLayer);
				}
			}
			mat.m_nTexLayer = (int)mat.m_TexLayers.size();
			mat.m_pGameMaterial = pGameMaterial;

			ColorConv(mat.m_BaseMat.m_Ambient  , pStdMtl->GetAmbient (0));
			ColorConv(mat.m_BaseMat.m_Diffuse  , pStdMtl->GetDiffuse (0));
			ColorConv(mat.m_BaseMat.m_Speculer , pStdMtl->GetSpecular(0));

			mat.m_BaseMat.m_fOpactiy = pStdMtl->GetOpacity(0);
			mat.m_BaseMat.m_fShiness = pStdMtl->GetShininess(0);

			wcsncpy(mat.m_BaseMat.m_MatName, ToWCHAR(pStdMtl->GetFullName()).c_str() ,64);
			mat.m_BaseMat.m_MatName[63] = 0;
			mat.m_BaseMat.m_bTwoSide = pStdMtl->GetTwoSided() != FALSE;
			m_MatsExport.push_back(mat);
		}
	}
	return true;
}

bool CMaxMesh::BuildMeshDesc()
{

	//����Mesh��
	int nVert = m_pMaxMesh->getNumVerts();
	if(nVert <= 0)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} Node Mesh: [%s]  �������С��0 \n" , INodeName(m_pNode).c_str() );
		return false;
	}

	size_t usedChNumber = (int)m_ActiveChannels.Count();
	for(size_t i = 0 ; i < usedChNumber ; ++i)
	{
		if( m_pGameMesh->GetNumberOfMapVerts(m_ActiveChannels[i]) <=0 )
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} Mesh: [%s]  channel map [index=%d]  nMapVert <= 0 \n" , INodeName(m_pNode).c_str() , m_ActiveChannels[i]);
			return false;
		}
	}

	int nFace    = 0;
	int nSubMesh = (int)this->m_vSubMeshs.size() ;
	for(int iSubMesh = 0 ; iSubMesh < nSubMesh ; iSubMesh ++ )
	{
		CMaxSubMesh& subMesh = m_vSubMeshs[iSubMesh];
		nFace += subMesh.m_nFace;
		subMesh.m_nMatIndex = MapMatID2MatIndex(subMesh.m_pGameMaterial );		
	}

	//sMeshDesc_t���
	wcsncpy(m_MeshDesc.m_Name, INodeName(m_pNode).c_str() , 32);
	m_MeshDesc.m_nSubMesh = (int)nSubMesh;
	m_MeshDesc.m_nUVMaps = (int)usedChNumber;
	m_MeshDesc.m_nFace   = nFace;
	m_MeshDesc.m_FurInfo = m_MeshDataFlag.m_FurInfo;
	m_MeshDesc.m_nVertex = (int)m_MaxVertexIndexs.size();
	return true;
}


bool CMaxMesh::IsExportable()
{
	//�ܴ���
	return m_pGameMesh && m_pMaxMesh;
}

Matrix3 CMaxMesh::Transpose(Matrix3& matrix)
{
	float transpose[4][3];

	int row;
	for(row = 0; row < 3; row++)
	{
		int column;
		for(column = 0; column < 3; column++)
		{
			transpose[column][row] = matrix.GetAddr()[row][column];
		}
	}

	int column;
	for(column = 0; column < 3; column++)
	{
		transpose[3][column] = 0;
	}

	Matrix3 transposeMatrix(transpose);
	transposeMatrix.NoTrans();

	return transposeMatrix;
}



bool  CMaxMesh::BuildVertexData(CSkeletonExporter* pSkeleton)
{
	m_VertexData.clear();
	m_BoundBox.clear(100000000.0f);

	sMaxMatrial_t& mat = m_MatsExport[0];

	int nVert              = (int)m_MaxVertexIndexs.size();
	int nActiveChannels    = m_ActiveChannels.Count();

	m_VertexData.reserve(nVert,nActiveChannels);
	m_VertexData.m_nUVChannes = nActiveChannels;

	GMatrix GPosMat = m_pGameNode->GetWorldTM(m_CurrentTime);
    Matrix3 PosMat = GPosMat.ExtractMatrix3();//=    m_pGameNode->GetMaxNode()->GetObjTMAfterWSM(m_CurrentTime) ;
	
	//Matrix3 PosMat = m_pGameNode->GetMaxNode()->GetObjTMAfterWSM(m_CurrentTime) ;

	Matrix3 NrmMat = Inverse(Transpose(PosMat));
	NrmMat.NoTrans();
	bool hasTangent = m_pGameMesh->GetNumberOfTangents() > 0 ;

	for(int i = 0 ; i < nVert ; ++i)
	{
		CMaxVertexIndex& Vi = m_MaxVertexIndexs[i];
		Point3    max_vertex;
		sVector_t v;

		//����Vi������m_nUVӦ����ͬ
		for(int iMapCh = 0 ; iMapCh < nActiveChannels ; ++iMapCh)
		{
			int ch = m_ActiveChannels[iMapCh];

			max_vertex = m_pGameMesh->GetMapVertex(ch , Vi.ChVert[iMapCh]  );

			//m_pGameMaterial->GetMaxMaterial()->Get
			//<!!!!!!!!!!!!!��������>
			//�õ����Channel��TM��Ҫ������ʹ�����Channel��SubMaterial�е�TexMap��������ͬ��TM
			//��Ȼ�������Ķ����᲻����
			//</!!!!!!!!!!!!>
			Matrix3 chMat = FindMapChannelTM(iMapCh , Vi.pGameMtl);
			max_vertex = max_vertex * chMat;            
			sUVCoord_t tv;
			tv.u = max_vertex.x; 
			tv.v = max_vertex.y ;

			//D3D���
			if( G_MaxEnv().m_bInvertTexCoordV )
			{
				tv.v = 1.0f - max_vertex.y ;
			}
			m_VertexData.m_UVChannels[iMapCh].push_back(tv);
		}
		//Position
		_XASSERT( m_pGameMesh->GetNumberOfVerts() >= (int)Vi.vert );
        Point3 vPos = m_pGameMesh->GetVertex(  Vi.vert , true);
		max_vertex = vPos * PosMat;
			//ģ����С�Ŵ�
		v.x = max_vertex.x * m_MeshDataFlag.m_fScale; 
		v.y = max_vertex.y * m_MeshDataFlag.m_fScale; 
		v.z = max_vertex.z * m_MeshDataFlag.m_fScale;

		m_BoundBox.add_vertex(v);
		m_VertexData.m_Positons.push_back(v);

		//Diffuse
		sColor_t color ;  color.a = 255;
		if(m_pGameMesh->GetNumberOfColorVerts() <= 0 ) 
		{
			color.r = 255 * m_MeshDataFlag.m_fColorScale[0];
			color.g = 255 * m_MeshDataFlag.m_fColorScale[1];
			color.b = 255 * m_MeshDataFlag.m_fColorScale[2];    
		}
		else
		{
			m_pGameMesh->GetColorVertex(Vi.color , max_vertex);
			color.r = max_vertex.x * 255 * m_MeshDataFlag.m_fColorScale[0];
			color.g = max_vertex.y * 255 * m_MeshDataFlag.m_fColorScale[1];
			color.b = max_vertex.z * 255 * m_MeshDataFlag.m_fColorScale[2];
		}

		
		if(m_pGameMesh->GetNumberOfAlphaVerts() > 0 )
		{
			float alpha = 1.0f;
			m_pGameMesh->GetAlphaVertex(Vi.alpha, alpha);
			color.a = alpha * 255;
		}
		m_VertexData.m_Diffuses.push_back(color);


		sVector_t nrl;
		sVector_t tangent ;
		_XASSERT(m_pGameMesh->GetNumberOfNormals() >= (int)Vi.norm );
		m_pGameMesh->GetNormal( Vi.norm , max_vertex , true);
		Point3 nrml_pt3 = max_vertex * NrmMat;
		nrml_pt3.Normalize();
		nrl.x = nrml_pt3.x;       nrl.y = nrml_pt3.y;       nrl.z = nrml_pt3.z;
		m_VertexData.m_Normals.push_back(nrl);

		//��û��Tangent;
		if(hasTangent == true)
		{
			m_pGameMesh->GetTangent( Vi.ChVert[0] , max_vertex );
			nrml_pt3 = max_vertex * NrmMat;
			nrml_pt3.Normalize();
			tangent.x = nrml_pt3.x;       tangent.y = nrml_pt3.y;       tangent.z = nrml_pt3.z;
			m_VertexData.m_Tangents.push_back(tangent);
		}
		else
		{			
			tangent.x = 0.0f ;tangent.y = 0.0f;tangent.z = 0.0f;
			m_VertexData.m_Tangents.push_back(tangent);
		}


		//��תYZ
		if( G_MaxEnv().m_bInvertYZCoord )
		{
			float t = m_VertexData.m_Positons[i].y ; 
			m_VertexData.m_Positons[i].y = m_VertexData.m_Positons[i].z ; 
			m_VertexData.m_Positons[i].z = t;

			t = m_VertexData.m_Normals[i].y ; 
			m_VertexData.m_Normals[i].y = m_VertexData.m_Normals[i].z ; 
			m_VertexData.m_Normals[i].z = t;

			t = m_VertexData.m_Tangents[i].y ; 
			m_VertexData.m_Tangents[i].y = m_VertexData.m_Tangents[i].z ; 
			m_VertexData.m_Tangents[i].z = t;
		}


	}
	if(pSkeleton)
	{
		BuildVertexWeights(pSkeleton);
	}
	CalcTangents(pSkeleton != NULL);
	int nVertex = (int)m_VertexData.m_Positons.size();
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   Node Mesh: [%s]  ��[%d]������ \n" , INodeName(m_pNode).c_str() , nVertex );
	return true;
}



bool CMaxMesh::BuildVertexWeights(CSkeletonExporter* pSkeleton)
{
	if(fabs(pSkeleton->m_fScale - m_MeshDataFlag.m_fScale) > 0.000001)
	{
		MessageBoxA(GetActiveWindow(),"������ͬ־û�е���Mesh::set_scale(),Mesh��Scale��Skeleton��Scale����ͬ","����",MB_OK);
		assert(0);
	}

	m_BoundBox.clear(100000000.0f);
	m_VertexData.m_VertexWeights.clear();
	int nVert  = (int)m_MaxVertexIndexs.size();

	m_VertexWeights.reserve(nVert);
	m_VertexData.m_VertexWeights.reserve(nVert);

	if(ExtractVertexWeights(pSkeleton) == false)
		return false;

	for(int i = 0 ; i < nVert ; ++i)
	{
		int wi = m_MaxVertexIndexs[i].vert;
		sWeightedVertex_t wv = m_VertexWeights[wi];

		//���Ҫ�����Ǽܳ�ʼλ�ã���ôҪʹ��InitPos�����InitPos��__extract_vertex_weight_fromXXXX����Ѿ�������ˡ�
		//���������Physique���ǿ���ֱ��ȡ������vtxBlend->GetOffsetVector(j). ����Skin�Ļ���Ҫ�Լ�����.
		//ͬʱ����������InitPos�Ļ����ǵû�Ҫ��InitNormalҲ�������

		//���ʹ�õ��ǵ�һ֡����ô��˵��ֱ��ʹ�ø�Static�õ������ݾͺ��ˡ�Pos��Normal��ֱ����
		//�������ʱ��InitTM����TM(0)�����CSkeletonExporter::push_bone(INode* pBone)��������
		if(G_MaxEnv().m_bUseBeforeSkeletonPose)
		{
			wv.m_InitPos.x *= pSkeleton->m_fScale;
			wv.m_InitPos.y *= pSkeleton->m_fScale;
			wv.m_InitPos.z *= pSkeleton->m_fScale;

			//===============================================
			//�����Ƥ�ĳ�ʼ������
			//���Լ�ȥ�㶥��ĳ�ʼλ��һ���ġ���
			//ֻ�Ƿ��ߵı任���������任����ʽInvert Transpose�Ĺ�ϵ���������ת�õĹ�ϵ
			//��������__extract_vertex_weight_fromskin��__extract_vertex_weight_fromphysique
			Point3 nrm0;
			nrm0.x = m_VertexData.m_Normals[i].x;
			nrm0.y = m_VertexData.m_Normals[i].y;
			nrm0.z = m_VertexData.m_Normals[i].z;
			Point3 nrmInit;
			nrmInit.Set(0.0f,0.0f,0.0f);
			for(int iBone = 0 ; iBone < wv.m_nEffBone ; iBone++)
			{
				Matrix3 matMeshInit;
				int boneIndex = wv.m_BoneWeights[iBone].m_BoneIndex ;
				float fWeight = wv.m_BoneWeights[iBone].m_fWeight;
				sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[boneIndex];
				assert(wv.m_BoneWeights[iBone].m_BoneID == boneNode.m_Bone.m_BoneID);

				//Normal �ĳ�ʼ����
				Matrix3 normlMat_InitTM = boneNode.m_SkinInitMT;
				normlMat_InitTM.Invert();
				normlMat_InitTM = Transpose(normlMat_InitTM);

				//Normal��0֡�任�������
				Matrix3 normlMat_TM0 = boneNode.m_InitNodeTM0;
				normlMat_TM0 = Transpose(normlMat_TM0);


				//matMeshInit = Inverse(boneNode.m_InitNodeTM0) * boneNode.m_SkinInitMT;
				//matMeshInit = Inverse( Inverse(normlMat_TM0) ) * normlMat_InitTM;

				matMeshInit = normlMat_TM0 * normlMat_InitTM;

				matMeshInit.NoTrans();
				Point3 pt = nrm0 * matMeshInit;
				nrmInit.x = nrmInit.x + fWeight * pt.x;
				nrmInit.y = nrmInit.y + fWeight * pt.y;
				nrmInit.z = nrmInit.z + fWeight * pt.z;
			}
			nrmInit.Normalize();
			m_VertexData.m_Normals[i].x = nrmInit.x;
			m_VertexData.m_Normals[i].y = nrmInit.y;
			m_VertexData.m_Normals[i].z = nrmInit.z;
			//================================================

		}
		else
		{
			wv.m_InitPos.x = m_VertexData.m_Positons[i].x;
			wv.m_InitPos.y = m_VertexData.m_Positons[i].y;
			wv.m_InitPos.z = m_VertexData.m_Positons[i].z;
		}


		//��תYZ
		if( G_MaxEnv().m_bInvertYZCoord )
		{
			float t = wv.m_InitPos.y ; wv.m_InitPos.y = wv.m_InitPos.z ; wv.m_InitPos.z = t;

			t = m_VertexData.m_Normals[i].y ; 
			m_VertexData.m_Normals[i].y = m_VertexData.m_Normals[i].z ; 
			m_VertexData.m_Normals[i].z = t;

			t = m_VertexData.m_Tangents[i].y ; 
			m_VertexData.m_Tangents[i].y = m_VertexData.m_Tangents[i].z ; 
			m_VertexData.m_Tangents[i].z = t;
		}

		m_BoundBox.add_vertex(wv.m_InitPos);
		m_VertexData.m_VertexWeights.push_back(wv);
	}
	return  true;
}

bool CMaxMesh::ExtractVertexWeights(CSkeletonExporter* pSkeleton)
{
      if(m_pSkinModifer)
		  return ExtractVertexWeightsFromModifier(m_pSkinModifer , pSkeleton);
	  INode* pParent = m_pNode->GetParentNode();
	  if(pParent && pParent != G_MaxEnv().m_pInterace->GetRootNode())
		  return ExtractVertexWeightsFromLink(m_pNode->GetParentNode(),pSkeleton);
	  else
		  return ExtractVertexWeightsFromLink(m_pNode , pSkeleton);
}

bool CMaxMesh::ExtractVertexWeightsFromModifier(IGameSkin* pGameSkin ,CSkeletonExporter* pSkeleton)
{
	int nNumVerts = pGameSkin->GetNumOfSkinnedVerts();

	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] ��ʼ��IGameSkin�޸�������Vertex Weights<--------\n" , INodeName(m_pNode).c_str()  );
	if(m_pGameMesh->GetNumberOfVerts() !=nNumVerts )
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]  �õ��Ķ�����Ŀ��ISkin�õ��Ķ�����Ŀ�����\n" , INodeName(m_pNode).c_str()  );
		//return false;
	}
	// �õ����������������
	sWeightedVertex_t exportVertex;
	m_VertexWeights.clear();
	m_VertexWeights.resize(nNumVerts);
	for(int ivert = 0 ; ivert  < nNumVerts ; ++ivert)
	{
		sTempWeight_t formerVertex;
		int nBones =  pGameSkin->GetNumberOfBones(ivert);// ->GetNumAssignedBones(ivert);
		if(nBones <= 0 )
		{
			wchar_t buf[128]={0};
			swprintf(buf, L"˭����ģ�Ͱ�����Ȼ�и���û����� iVertex = %d , MeshNodeName = %s �޸���ΪSkin", ivert,INodeName(m_pNode));
			MessageBoxW(GetActiveWindow(),buf,L"����",MB_OK);
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Warning} [%s] \n" , buf  );
		}
		for(int jBone = 0; jBone < nBones; jBone ++)
		{            
			INode* pBone = pGameSkin->GetBone(  ivert , jBone );
			int iBoneIndex = pSkeleton->push_bone(pBone);
			if( iBoneIndex<0 )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]   Skeleton�Ҳ���ָ������ Bone=[%s]\n" , INodeName(m_pNode).c_str() , INodeName(pBone).c_str() );
				return false;
			}

			//�õ���������
			xMeshBone_t usedBone;
			usedBone.m_BoneIndex = iBoneIndex;
			usedBone.m_pBone     = pBone;
			addUsedBone(usedBone);

			sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[iBoneIndex];
			float fWeight = pGameSkin->GetWeight(ivert,jBone);
			bool bRepeat = false;
			for( size_t indexB = 0; indexB < formerVertex.m_Weights.size(); ++indexB )
			{
				if( formerVertex.m_Weights[indexB].m_BoneIndex == (WORD)iBoneIndex )
				{
					formerVertex.m_Weights[indexB].m_fWeight += fWeight;
					bRepeat = true;
					break;
				}
			}
			// ���û���ظ��Ĺ�ͷ
			if( bRepeat == false )
			{
				sVertWeight_t boneWeight;
				boneWeight.m_BoneID = boneNode.m_Bone.m_BoneID;
				boneWeight.m_BoneIndex = iBoneIndex;
				boneWeight.m_fWeight = fWeight;
				//if( fabs( boneWeight.m_fWeight ) > 1.0e-3 )
				{
					formerVertex.m_Weights.push_back(boneWeight);
				}
			}

		}


		// sort
		formerVertex._sort();
		//ȥ������4�������Ӱ��
		for(int i = MAX_INFLUENCE_BONE ; i < (int)formerVertex.m_Weights.size() ; i ++)
		{
			formerVertex.m_Weights[i].m_fWeight = 0.0f;
		}

		//���Ȩ��

		float fTWeight = 0.0f;
		for(size_t iW = 0 ; iW < formerVertex.m_Weights.size(); ++iW)
		{
			fTWeight += formerVertex.m_Weights[iW].m_fWeight;
		}
		if(fTWeight != 0)
		{
			float scale = 1.0f/fTWeight;
			for(size_t iW = 0 ; iW < formerVertex.m_Weights.size(); ++iW)
			{
				formerVertex.m_Weights[iW].m_fWeight *= scale;
			}
		}


		// ��formerVertex ��ֵ���� exportVertex
		if( formerVertex.m_Weights.size() > MAX_INFLUENCE_BONE )
		{
			exportVertex.m_nEffBone = MAX_INFLUENCE_BONE;
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {waring} Mesh: [%s]   �ж����ܵ�����4���ͷӰ�죬���Զ���Ĺ���: iVert= [%d]\n" , INodeName(m_pNode).c_str() , ivert );
		}
		else
		{
			exportVertex.m_nEffBone = (int)formerVertex.m_Weights.size();
		}

		//�������ݵ������Ķ����в����Ȩ��֮��
		float fWeigtAll = 0.0f;
		for( int jBone = 0; jBone < exportVertex.m_nEffBone; ++jBone )
		{
			exportVertex.m_BoneWeights[jBone].m_BoneID    = formerVertex.m_Weights[jBone].m_BoneID;
			exportVertex.m_BoneWeights[jBone].m_BoneIndex = formerVertex.m_Weights[jBone].m_BoneIndex;
			exportVertex.m_BoneWeights[jBone].m_fWeight   = formerVertex.m_Weights[jBone].m_fWeight;
			fWeigtAll += formerVertex.m_Weights[jBone].m_fWeight;
		}
		//Ȩ��֮��Ϊ0�Ļ������Ǿ�ǿ��ƽ��
		if( fabs(fWeigtAll-1.f)>0.01f )
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {waring} Mesh: [%s]  �ж����weight ��Ϊ 0 (iVert = %d) ǿ��ƽ��weight :" , INodeName(m_pNode).c_str() , ivert );
			float weight = 1.0f/float(exportVertex.m_nEffBone);
			for( int jBone = 0; jBone < exportVertex.m_nEffBone; ++jBone )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH , L"<Wight iBone=%d, weight=%f>",jBone ,exportVertex.m_BoneWeights[jBone].m_fWeight);
				exportVertex.m_BoneWeights[jBone].m_fWeight   = weight;
			}
			XEVOL_LOG(eXL_DEBUG_HIGH , L"\n");
		}

		//�����Ƥ�ĳ�ʼλ��-----���ǽ�ɫ��ƽ���ŵ��Ǹ�����
		{
			//��ȡ��0֡������. �ӵ�0֡��ص���ƽ���ŵ�ʱ���ģ�ͳ�ʼλ�ã���i֡����
			//���裬��0֡��ʱ��λ����v0, �����任������TM(0,k)
			//��ƽ���ŵ�ʱ��,    λ����vi, �����任����ʽTM(i,k). ---�������Bone InitTM
			//�ĸ�Ȩ�ض�Ӧλ��Ϊ W(k);

			//�ڵ�0֡��ʱ�򣬶�������ĸ������ĵ�λ��Ӧ��Ϊ v0 * inv[TM(0,k)]. 
			//�����ֵ��Physique��ȡ�����ľ���vtxBlend->GetOffsetVector(j), ����Skinֻ���Լ�����
			//���ԡ���ʵ���ǲ�����Ҫ���Offset���ӵ�0֡������i֡��Bone InitTM�Ϳ���������offset��
			//��ô�ӵ�0֡���㵽��i֡����ʼ֡��, ���ݹ���������ʽ��
			//vi = SIGMA< v0 * inv[TM(0,k)] * TM(i,k) > (k = 0, 4).
			Point3 v0 = m_pGameMesh->GetVertex( ivert  , true ) * m_pNode->GetObjTMAfterWSM(0);
			Point3 vInit;
			vInit.Set(0.0f,0.0f,0.0f);
			for(int i = 0 ; i < exportVertex.m_nEffBone ; i++)
			{
				Matrix3 matMeshInit;
				int boneIndex = exportVertex.m_BoneWeights[i].m_BoneIndex ;
				float fWeight = exportVertex.m_BoneWeights[i].m_fWeight;
				sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[boneIndex];
				assert(exportVertex.m_BoneWeights[i].m_BoneID == boneNode.m_Bone.m_BoneID);
				matMeshInit = Inverse(boneNode.m_InitNodeTM0) * boneNode.m_SkinInitMT;
				Point3 pt = v0 * matMeshInit;
				vInit.x = vInit.x + fWeight * pt.x;
				vInit.y = vInit.y + fWeight * pt.y;
				vInit.z = vInit.z + fWeight * pt.z;
			}
			exportVertex.m_InitPos.x = vInit.x;
			exportVertex.m_InitPos.y = vInit.y;
			exportVertex.m_InitPos.z = vInit.z;
		}

		m_VertexWeights[ivert] = exportVertex;
	}
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] ��Vertex Weight�Ѿ��ɹ���ISkin�ﹹ�����<--------\n" , INodeName(m_pNode).c_str()  );
	return true;
}

bool CMaxMesh::ExtractVertexWeightsFromLink(INode* pFakeBone, CSkeletonExporter* pSkeleton)
{
	int nNumVerts = m_pGameMesh->GetNumberOfVerts();;

	std::wstring BoneName = INodeName( pFakeBone );
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] ��ʼ��Linked Node��Vertex Weights<--------\n" , INodeName(m_pNode).c_str()  );

	// �õ����������������
	sWeightedVertex_t exportVertex;
	m_VertexWeights.clear();
	m_VertexWeights.resize(nNumVerts);

	int boneIndex  = pSkeleton->push_bone(pFakeBone);

	if(boneIndex < 0)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]  α��ͷ �Ҳ���BondeIndex���Ǽܴ��� FakeBone=[%s] \n" , INodeName(m_pNode).c_str() , INodeName(pFakeBone).c_str() );
		return false;
	}

	//���Meshֻ����һ��mesh�ġ�
	xMeshBone_t usedBone;
	usedBone.m_BoneIndex = boneIndex;
	usedBone.m_pBone     = pFakeBone;
	addUsedBone(usedBone);

	sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[boneIndex];
	for(int ivert = 0 ; ivert < nNumVerts ; ++ivert)
	{

		exportVertex.m_nEffBone = 1.0f;
		exportVertex.m_BoneWeights[0].m_BoneID = boneNode.m_Bone.m_BoneID;
		exportVertex.m_BoneWeights[0].m_BoneIndex = boneIndex;
		exportVertex.m_BoneWeights[0].m_fWeight = 1.0f;
		//�����Ƥ�ĳ�ʼλ��
		{
			Point3 v0 = m_pGameMesh->GetVertex(ivert , true ) * m_pNode->GetObjTMAfterWSM(0);
			Matrix3 matMeshInit = Inverse(boneNode.m_InitNodeTM0) * boneNode.m_SkinInitMT;
			Point3 vInit = v0 * matMeshInit;
			exportVertex.m_InitPos.x = vInit.x;
			exportVertex.m_InitPos.y = vInit.y;
			exportVertex.m_InitPos.z = vInit.z;
		}

		m_VertexWeights[ivert] = exportVertex;
	}

	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] ��Linked Node�ﹹ��Vertex Weights���\n" , INodeName(m_pNode).c_str()  );
	return true;
}

bool CMaxMesh::CalcTBN(sFace_t& face,bool isSkin)
{
	Point3  normal[3];
	Point3  Tangent; 
	Point3  p[3];

	assign(normal[0],m_VertexData.m_Normals[face.vert[0]]);
	assign(normal[1],m_VertexData.m_Normals[face.vert[1]]);
	assign(normal[2],m_VertexData.m_Normals[face.vert[2]]);

	if(isSkin == false)
	{
		assign(p[0],m_VertexData.m_Positons[face.vert[0]]);
		assign(p[1],m_VertexData.m_Positons[face.vert[1]]);
		assign(p[2],m_VertexData.m_Positons[face.vert[2]]);
	}
	else
	{
		assign(p[0],m_VertexData.m_VertexWeights[face.vert[0]].m_InitPos);
		assign(p[1],m_VertexData.m_VertexWeights[face.vert[1]].m_InitPos);
		assign(p[2],m_VertexData.m_VertexWeights[face.vert[2]].m_InitPos);
	}


	sUVCoord_t uv[3];
	uv[0] = m_VertexData.m_UVChannels[0][face.vert[0]];
	uv[1] = m_VertexData.m_UVChannels[0][face.vert[1]];
	uv[2] = m_VertexData.m_UVChannels[0][face.vert[2]];

	Point3  e1 = p[1] - p[0];
	Point3  e2 = p[2] - p[0];
	sUVCoord_t u1 = { uv[1].u - uv[0].u , uv[1].v - uv[0].v};
	sUVCoord_t u2 = { uv[2].u - uv[0].u , uv[2].v - uv[0].v};

	float det =  ( u1.u * u2.v - u2.u * u1.v);
	if(det == 0.0f)
	{
		Tangent = e1;
	}
	else
	{
		Tangent = u2.v * e1 - u1.v * e2;
	}

	//��Normal �� Tangent�����¼����Tangent,��Ϊ���Tangent�Ͷ����Normal���ܲ���ֱ
	Point3 final_tangent;
	for(int i = 0 ;i < 3 ; ++i)
	{
		Point3 binormal  = CrossProd(normal[i],Tangent);
		final_tangent = CrossProd(binormal,normal[i]);
		final_tangent.Normalize();
		m_VertexData.m_Tangents[face.vert[i]].x += final_tangent.x;
		m_VertexData.m_Tangents[face.vert[i]].y += final_tangent.y;
		m_VertexData.m_Tangents[face.vert[i]].z += final_tangent.z;

	}
	return true;
}

void CMaxMesh::CalcTangents(bool isSkin)
{
	size_t nSubMesh = m_vSubMeshs.size();
	for(size_t iMesh = 0 ; iMesh < nSubMesh ; iMesh ++)
	{
		CMaxSubMesh& subMesh = m_vSubMeshs[iMesh];
		size_t nFace = subMesh.m_nFace;
		for(size_t iFace = 0 ; iFace < nFace ; iFace ++)
		{
			sFace_t& _face = subMesh.m_Faces[iFace];
			CalcTBN(_face,isSkin);
		}
	}
	//������Ժ󣬰�tangent���µ�λ��
	for(size_t i = 0 ; i < m_VertexData.m_Tangents.size() ; ++i)
	{
		m_VertexData.m_Tangents[i].normalize();
	}
}

bool CMaxMesh::BuildMorphTarget()
{

	if(m_pMorpModifer==NULL)
		return false;

	int nMorphTarget = m_pMorpModifer->GetNumberOfMorphTargets();
	for(int i = 0 ; i < nMorphTarget ; ++i)
	{
		sMaxMorphTarget_t morphTarget;
		IGameNode* pGameNode =  m_pMorpModifer->GetMorphTarget(i);
		//morphTarget.m_MorphName = pGameNode;
	}

	//int nMorphChannel = m_pMaxMophModifier->chanNum;
	//for(int i = 0 ; i < nMorphChannel ; i ++)
	//{
	//	morphChannel& morphCh = m_pMaxMophModifier->chanBank[i];
	//	wstring channelName = morphCh.mName;
	//}

	return true;

}
void  CMaxMesh::InitSkeleton(CSkeletonExporter* pSkeleton)
{
    pSkeleton->AddSkinModifier(m_pSkinModifer);
	return FindAllUsedBone(pSkeleton);
}

void CMaxMesh::FindAllUsedBone(CSkeletonExporter* pSkeleton)
{
	if(m_pSkinModifer != NULL)
	{
		int nNumVerts = m_pSkinModifer->GetNumOfSkinnedVerts() ;
		for( int iVert =0 ; iVert<nNumVerts ; iVert++ )
		{
			int nBone = m_pSkinModifer->GetNumberOfBones(iVert);
			for( int j=0 ; j< nBone ; j++ )
			{
				INode* pBone = m_pSkinModifer->GetBone(iVert ,  j);
				if(pBone == NULL)
				{
					XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] Mesh�Ҳ�������\n" , INodeName(m_pNode).c_str() );
					return ;
				}
				int iBone = pSkeleton->push_bone(pBone);
				if( iBone<0 )
				{
					XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] Skeleton�Ҳ���ָ������ Bone=[]\n" , INodeName(m_pNode).c_str() , INodeName(pBone).c_str() );
					return ;
				}
			}
		}
	}

	INode* pParent = m_pNode->GetParentNode();
	if(pParent && pParent != G_MaxEnv().m_pInterace->GetRootNode())
		pSkeleton->push_bone(m_pNode->GetParentNode());
	else
		pSkeleton->push_bone(m_pNode);
}
