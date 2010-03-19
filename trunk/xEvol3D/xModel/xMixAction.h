#ifndef __XMIX_ACTION_H__
#define __XMIX_ACTION_H__

#include "xCoreAction.h"

BEGIN_NAMESPACE_XEVOL3D
//��ϵĶ������������������ϵĶ����������϶����ȵ�
/***********************************************
������ϵ�ԭ��
1. ��ϵĶ�������Ҫ����һ�����������ļ�����չ��Ϊ.mix
   ����ļ�һ��������Ķ����ļ�����һ���ն���������
   �Ĺ�����������Ҫ��xActionNameָ��TypeΪ"Mix";

2. ���������ļ���ָ�����飬�����ΪxActionSlot��ÿ��
   Slot�����ֺ����Ա�

3.xActionSlot����һ��Attribute���顣����.mix�ļ�����ʵ
  ����������ЩSlot�е�Attribute���飬ͬʱ����xMixedAction
  �е�m_BoneActionMap��Map����ָ��ĳ����������ĸ�Slot���ơ�
  ��Slot��Attribute�У�ֻ�����ڸ�Slot��Bone.��Attributeֵ
  �Ż���0.

5. ��.mix�ļ��У����ĳ��slotûָ����Щ���������ʹ������
   ǰ�����ʣ�µĹ����� ���ָ���˹����������ѡ���ǲ���
   �ݹ�������е��ӹ�����һ���������true)��Ҳ����������
   ֻ�������ӹ�����

6.��϶�����ʱ��getBoneData�����Bone�����ĸ�Slot������
  getBoneData�ĵ���ת����Slot�Ϲҽӵ�xCoreAction��ȥ��

7.��϶�����MixedAction���и�ר�ú���������Ϊÿ��Slot�ϵ�
  ����ָ��һ��ʱ���ᡣע��time_in_ms�Ǹ����顣


***********************************************/
class _XEVOL_BASE_API_ xMixedAction : public xCoreAction
{
	IMPL_BASE_OBJECT_INTERFACE(xMixedAction);
public:

	struct xActionSlot
	{
		xCoreAction*          m_pAction;
		std::ds_wstring       m_strName;
		int                   m_hashName;
		xCoreActionAttr*      m_pAttribute;
		int                   m_TimeShift;
	};

public:
	xMixedAction(xCoreSkeleton* pSkeleton = NULL);
	~xMixedAction();
public:
    bool                 setMainAction(xCoreAction* pAction);
	bool                 setAction(const wchar_t* slotName , xCoreAction* pAction);
	bool                 setAction(int iSlot , xCoreAction* pAction);
	xActionSlot&         actionSlot(int iSlot){ return m_ActionSlots[iSlot] ; }
    int                  nActionSlot() const { return (int)m_ActionSlots.size() ; }

public:
	bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	bool                 load(const wchar_t * _name , xcomdocstream* stream);
	bool                 load(const wchar_t * _name , istream& stream);
	bool                 load(xXmlNode* pCfgNode );
	const wchar_t*       typeID() const;


	xBoneData*           getBoneData(int boneIndex, int frame );
    xBaseActionState*    getActionState(int boneIndex, xBaseActionState* pActionState);
	xCoreActionAttr*     attribute();
	
	bool                 blend(long time_in_ms  , xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
    bool                 blend(long time_in_ms[], xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton);
    bool                 blend(xBaseActionState* pActionSlotState , xCoreActionFrame* skeletonFrame, xCoreSkeleton* pSkeleton );
	void                 unload();
	int                  memUsage();
	bool                 setTime(float _time);
    float                getDurTime();
	const xActionInfo*   info() const;
	const xSkeletonID&   skeletonID() const;
	const wchar_t*       name() const;
	const int            nBone() const;
	

protected:
	bool    setBoneToActionSlot(int boneIdx , int iSlot , bool bRecursive , bool onlyChild);
    bool    excludeBone(int boneIdx , int iSlotOwnner);
protected:
	typedef ds_vector(xActionSlot)  xActionSlots;
	typedef std::vector<int>        xBoneActionMap;

	xActionSlots   m_ActionSlots;
	xBoneActionMap m_BoneActionMap;
	xCoreSkeleton* m_pSkeleton;
};

END_NAMESPACE_XEVOL3D
#endif
