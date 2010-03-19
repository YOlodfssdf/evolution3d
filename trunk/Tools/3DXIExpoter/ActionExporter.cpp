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
#include "ActionExporter.h"
#include "xXmlDocument.h"
#include <decomp.h> 

void     makBoneTrans2(INode* pNode , sBoneTrans_t& boneTrans , Matrix3& mat)
{
	Point3   Trans = mat.GetTrans();
	Matrix3  RotMat = mat;
	Matrix3  ScaleMat = mat;

    RotMat.NoScale();
    ScaleMat = mat * Inverse(RotMat);
    //���Scale Matrix;

	RotMat.NoTrans();	
	Quat RotQuat(RotMat);

	//ScaleMat.NoRot();
	//ScaleMat.NoTrans();

	boneTrans.m_Rotate    = conv_type<sQuat_t , Quat>(RotQuat);
	boneTrans.m_Trans     = conv_type<sVector_t   ,Point3 >(Trans);
	boneTrans.m_Scale.x   = ScaleMat.GetRow(0).x;
	boneTrans.m_Scale.y   = ScaleMat.GetRow(1).y;
	boneTrans.m_Scale.z   = ScaleMat.GetRow(2).z;
	if( abs(boneTrans.m_Scale.x - boneTrans.m_Scale.y) > 0.000001 ||
		abs(boneTrans.m_Scale.y - boneTrans.m_Scale.z) > 0.000001 ||
		abs(boneTrans.m_Scale.z - boneTrans.m_Scale.x) > 0.000001 )
	{
            std::wstring _NodeName = INodeName(pNode);
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {����} :  ��ͷ[ %s ] ������NonUniformScale\r\n", _NodeName.c_str() );
	}
}

void     makBoneTrans(INode* pNode , sBoneTrans_t& boneTrans , Matrix3& mat)
{

    Point3   Scale ;
	Quat     Rot;
	Point3   Trans = mat.GetTrans();
	SpectralDecomp(mat,Scale ,Rot );

	makBoneTrans2(pNode , boneTrans,mat);
	//boneTrans.m_Rotate    = conv_type<sQuat_t , Quat>(Rot);
	//boneTrans.m_Trans     = conv_type<sVector_t   ,Point3 >(Trans);
	//boneTrans.m_Scale     = conv_type<sVector_t   ,Point3 >(Scale);

}

Matrix3  CActionExporter::get_locale_tm(INode* pNode , bool hasParent,unsigned int iMaxTime)
{
     if(hasParent && pNode->GetParentNode())
     {
         Matrix3 parentTM = pNode->GetParentNode()->GetNodeTM(iMaxTime);
         return pNode->GetNodeTM(iMaxTime) * Inverse(parentTM);
     }
     return pNode->GetNodeTM(iMaxTime);
}


Matrix3 CActionExporter::get_bone_tm(CSkeletonExporter* pSkeleton,int boneIndex,unsigned int iMaxTime)
{
         sMaxBoneNode_t bone = pSkeleton->m_MaxBones[boneIndex];
         Matrix3        boneInitMTInv;
         if(G_MaxEnv().m_bUseBeforeSkeletonPose)
             boneInitMTInv = bone.m_SkinInitMT;
         else
             boneInitMTInv = bone.m_InitNodeTM0;

         boneInitMTInv.Invert();
         INode* pNode = bone.m_pNode;

         Matrix3 BoneTM = boneInitMTInv * pNode->GetNodeTM(iMaxTime); //>GetNodeTM(iMaxTime);
         Point3 Trans = BoneTM.GetTrans();
         BoneTM.NoTrans();
         //�Թ�����������
         Trans.x *= pSkeleton->m_fScale;
         Trans.y *= pSkeleton->m_fScale;
         Trans.z *= pSkeleton->m_fScale;
         BoneTM.SetTrans(Trans);

         return BoneTM;

}

bool CActionExporter::load_action_list(sActionInfos_t& actions,xcomdoc& comdoc , std::wstring action_list_name)
{

	xcomdocstream* pactionsstreamTXT = comdoc.open_stream((action_list_name).c_str());
	xXmlDocument  xmlDoc;
	if(pactionsstreamTXT->data_len())
	{
		int dataLen = (int)pactionsstreamTXT->data_len()  ;
		char* buf = new char[dataLen];
		pactionsstreamTXT->stream_seekr(xcdsd_beg,0);
		pactionsstreamTXT->read(buf,dataLen);
		xmlDoc.load(buf,dataLen,true);
		delete buf;
	}

	comdoc.close_stream(pactionsstreamTXT);
	xXmlNode* rootNode = xmlDoc.root();
	if(rootNode == NULL)
		return true;

    for(int i = 0 ;  i < rootNode->countNode() ; i ++)
	{
		xXmlNode* actNode = rootNode->findNode(i);
		if( actNode )
		{

			sActionInfo_t action_info;
			wcsncpy(action_info.m_Name,actNode->name(),32);
			action_info.m_ActionType  = actNode->int_value(L"ActionType");
			action_info.m_iFirstFrame = actNode->int_value(L"FirstFrame");
			action_info.m_iLastFrame  = actNode->int_value(L"LastFrame");
			action_info.m_nFrame      = actNode->int_value(L"nFrame");
			action_info.m_lTime       = actNode->int_value(L"DurTime");
			actions.push_back(action_info);
		}
	}
	return true;
}

void CActionExporter::writeActionList(sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_list_name)
{
	xcomdocstream* pactionsstream = comdoc.open_stream(action_list_name.c_str(),false);
	pactionsstream->stream_seekr(xcdsd_beg,0);
	xXmlDocument xmlDoc;

	if(pactionsstream->data_len())
	{
		int dataLen = (int)pactionsstream->data_len();
		char* buf = new char[dataLen];
		pactionsstream->stream_seekr(xcdsd_beg,0);
		pactionsstream->read(buf,dataLen);
		xmlDoc.load(buf,dataLen,true);
		delete buf;
	}
	if(xmlDoc.countNode() == 0)
	{
		xmlDoc.insertNode(L"actions");
	}
    xXmlNode* rootNode = xmlDoc.root();

	for(size_t i = 0 ;  i < actions.size(); i ++)
	{
		actions[i].m_nFrame = GetActNumFrame(actions[i]);
		if(rootNode->findNode(actions[i].m_Name))
		{
             XEVOL_LOG(eXL_DEBUG_HIGH , L"{����} :  ����Ϊ <  %s > �Ķ����Ѿ�����\r\n" , actions[i].m_Name );
			continue;
		}
		else
		{
			xXmlNode* actionNode = rootNode->insertNode( actions[i].m_Name );
			actionNode->setValue(L"Name" , actions[i].m_Name);
            actionNode->setValue(L"ActionType",  actions[i].m_ActionType);
			actionNode->setValue(L"FirstFrame",  actions[i].m_iFirstFrame);
			actionNode->setValue(L"LastFrame" ,  actions[i].m_iLastFrame);
			actionNode->setValue(L"nFrame"    ,  actions[i].m_nFrame);
			actionNode->setValue(L"DurTime"   ,  actions[i].m_lTime);
		}
	}
	pactionsstream->stream_seekw(xcdsd_beg,0);

	//write xml file to the stringstream
	xcdstream out(pactionsstream);
	xmlDoc.setXMLStyle(true);
	xmlDoc.save(out);
	out.close();
	comdoc.close_stream(pactionsstream);
	xmlDoc.unload();
	
}



void CActionExporter::export(CSkeletonExporter* pSkeleton, sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_dir)
{

    std::wstring action_list_name = action_dir + L"actions.xml";
	writeActionList(actions,comdoc,action_list_name);

	Matrix3 FlipYZMat;
	FlipYZMat.IdentityMatrix();
	FlipYZMat.SetRow(0 , Point3(1.0f,0.0f,0.0f) );
	FlipYZMat.SetRow(1 , Point3(0.0f,0.0f,1.0f) );
	FlipYZMat.SetRow(2 , Point3(0.0f,1.0f,0.0f) );
	//
	int nBone  = (int)pSkeleton->m_MaxBones.size();


	//�ֶ����������е�Bone��Dataд�뵽�ļ���
    for(size_t iAction = 0 ; iAction < actions.size() ; ++iAction)
    {
	    sActionInfo_t action = actions[iAction];
		xcomdocstream* pactiondatastream = comdoc.create_stream((action_dir + action.m_Name + L".xra").c_str(),xcddt_common,XCOMDOC_COMPRESS_BLEND);


		if(pactiondatastream == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"{error}: Action�Ѿ����ڻ��߷Ƿ�: < %s > \n" , (action_dir + action.m_Name).c_str() );
			continue;
		}

		//д��ID;
		pSkeleton->makeID();
		sSkeletonID_t sid = pSkeleton->getID();
		pactiondatastream->write(sid);

        int nBone = (int)pSkeleton->m_MaxBones.size();
		pactiondatastream->write(nBone);

        int nFrame   = GetActNumFrame(action);
		int firstFrame = action.m_iFirstFrame;

		action.m_iLastFrame  -= firstFrame;
		action.m_iFirstFrame -= firstFrame;
		action.m_nFrame      = nFrame;
        pactiondatastream->write( (const char*)&action.m_lTime , sizeof(sActionInfo_t) - sizeof(wchar_t) * 32 );


		XEVOL_LOG(eXL_DEBUG_HIGH , L"  ��������=%s ����ʱ��=%d ����֡��=%d \n",action.m_Name,action.m_lTime,nFrame );

		for(int i = 0 ; i < nFrame ; ++i)
		{
			int iFrame = firstFrame + i ;
			for(size_t iBone = 0 ; iBone < pSkeleton->m_MaxBones.size() ; ++iBone)
			{
				sMaxBoneNode_t bone = pSkeleton->m_MaxBones[iBone];
				bool           hasParentBone = (bone.m_Bone.m_ParentIndex != -1);
				INode* pNode = bone.m_pNode;

				unsigned int iMaxTime = iFrame * GetTicksPerFrame();
				//��Ϊ��������WeightedVertx��ʱ�����InitMT��
				//������������Ҫ������˻�ȥ���õ�����
				Matrix3 BoneTM = get_bone_tm(pSkeleton,(int)iBone,iMaxTime);

				//���ڿ�ʼ���������ڸ��ڵ����Ծ���
				Matrix3 localTM = get_locale_tm(pNode,hasParentBone,iMaxTime);//Inverse(parentBoneTM) * BoneTM;
				if(hasParentBone == false)
					localTM = pNode->GetNodeTM(iMaxTime);
				Point3 lTrans = localTM.GetTrans();
				localTM.NoTrans();
				//�Թ�����������
				lTrans.x *= pSkeleton->m_fScale;
				lTrans.y *= pSkeleton->m_fScale;
				lTrans.z *= pSkeleton->m_fScale;
				localTM.SetTrans(lTrans);


				sBoneData_t BoneData;
				//����ռ��еı任����
				BoneData.m_Matrix    = conv_type<sMatrix4x3_t,Matrix3>(BoneTM);
				//�ֲ��ռ��еı任����
				BoneData.m_LocaleTM  = conv_type<sMatrix4x3_t,Matrix3>(localTM);

				if(G_MaxEnv().m_bInvertYZCoord )
				{
					Matrix3 fMat ;
					fMat = FlipYZMat * BoneTM * FlipYZMat;
					//����ռ��еı任����
					BoneData.m_Matrix    = conv_type<sMatrix4x3_t,Matrix3>(fMat);
					fMat =  FlipYZMat * localTM * FlipYZMat;
					BoneData.m_LocaleTM  = conv_type<sMatrix4x3_t,Matrix3>(fMat);
				}
				//��LocalTM�ﹹ�����ת��ƽ�ƺ����ž���
				makBoneTrans2(pNode , BoneData.m_lTranform , localTM );
				pactiondatastream->write(BoneData);
			}
		}
	}

}

/*

void CActionExporter::export_xml(CSkeletonExporter* pSkeleton, sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_dir)
{

	wstring action_list_name = action_dir + L"actions.xml";
	writeActionList(actions,comdoc,action_list_name);

	//

	int nBone  = (int)pSkeleton->m_MaxBones.size();


	xcomdocstream* pboneinfostream = comdoc.create_stream((action_dir + L"boneinfo").c_str(),xcddt_common,XCOMDOC_COMPRESS_BEST);
	if(pboneinfostream)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"=====��ʼд��action data==========\r\n");
		XEVOL_LOG(eXL_DEBUG_HIGH , L"        һ����%d������\r\n",nBone );
		pboneinfostream->write(nBone);
		for(size_t iBone = 0 ; iBone < pSkeleton->m_MaxBones.size() ; ++iBone)
		{
			sMaxBoneNode_t bone = pSkeleton->m_MaxBones[iBone];
			Matrix3 InitTM,InitTMInv;
			if(G_MaxEnv().m_bUseBeforeSkeletonPose)
				InitTM = bone.m_SkinInitMT;
			else
				InitTM = bone.m_InitNodeTM0 ;
			Point3 Trans = InitTM.GetTrans();
			InitTM.NoTrans();
			Trans.x *= pSkeleton->m_fScale;
			Trans.y *= pSkeleton->m_fScale;
			Trans.z *= pSkeleton->m_fScale;
			InitTM.SetTrans(Trans);
			InitTMInv = Inverse(InitTM);
			bone.m_Bone.m_InitMTInv = conv_type<sMatrix4x3_t,Matrix3>(InitTMInv);
			bone.m_Bone.m_InitMT    = conv_type<sMatrix4x3_t,Matrix3>(InitTM);
			pboneinfostream->write(bone.m_Bone);//��д��Bone�Ļ�����Ϣ
		}
	}

	//�ֶ����������е�Bone��Dataд�뵽�ļ���
	for(size_t iAction = 0 ; iAction < actions.size() ; ++iAction)
	{
		sActionInfo_t& action = actions[iAction];
		xcomdocstream* pactiondatastream = comdoc.create_stream( (action_dir + action.m_Name).c_str(),xcddt_common,XCOMDOC_COMPRESS_BEST);

		int nFrame   = action.m_iLastFrame - action.m_iFirstFrame + 1;
		int DataSize = nFrame *  nBone * sizeof(sBoneData_t);
		for(size_t iBone = 0 ; iBone < pSkeleton->m_MaxBones.size() ; ++iBone)
		{
			sMaxBoneNode_t bone = pSkeleton->m_MaxBones[iBone];
			bool           hasParentBone = (bone.m_Bone.m_ParentIndex != -1);
			INode* pNode = bone.m_pNode;
			for(int iFrame = action.m_iFirstFrame ; iFrame <= action.m_iLastFrame ; ++iFrame)
			{
				unsigned int iMaxTime = iFrame * GetTicksPerFrame();

				//��Ϊ��������WeightedVertx��ʱ�����InitMT��
				//������������Ҫ������˻�ȥ���õ�����
				Matrix3 BoneTM = get_bone_tm(pSkeleton,(int)iBone,iMaxTime);
				Matrix3 RotMat = BoneTM;
				Point3 Trans = BoneTM.GetTrans();
				RotMat.NoTrans();
				Quat RotQuat(RotMat);

				//���ڿ�ʼ���������ڸ��ڵ����Ծ���
				Matrix3 localTM = get_locale_tm(pNode,hasParentBone,iMaxTime);//Inverse(parentBoneTM) * BoneTM;
				if(hasParentBone == false)
					localTM = pNode->GetNodeTM(iMaxTime);
				Point3 lTrans = localTM.GetTrans();
				localTM.NoTrans();
				//�Թ�����������
				lTrans.x *= pSkeleton->m_fScale;
				lTrans.y *= pSkeleton->m_fScale;
				lTrans.z *= pSkeleton->m_fScale;
				localTM.SetTrans(lTrans);


				sBoneData_t BoneData;
				BoneData.m_Matrix    = conv_type<sMatrix4x3_t,Matrix3>(BoneTM);
				BoneData.m_LocaleTM  = conv_type<sMatrix4x3_t,Matrix3>(localTM);
				makBoneTrans(pNode, BoneData.m_lTranform , localTM);
				
				pactiondatastream->write(BoneData);

			}
		}
	}

}
*/