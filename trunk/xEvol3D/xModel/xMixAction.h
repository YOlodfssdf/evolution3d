#ifndef __XMIX_ACTION_H__
#define __XMIX_ACTION_H__

#include "xBaseAction.h"

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
class _XEVOL_BASE_API_ xMixedAction : public xBaseAction
{
	IMPL_BASE_OBJECT_INTERFACE(xMixedAction);
public:

	struct xActionSlot
	{
		std::ds_wstring       m_strName;
		int                   m_hashName;
		xCoreActionAttr*      m_pAttribute;
    public:
        xActionSlot()
        {
            m_hAction.setNULL();
        }
        xBaseAction*          getAction() const
        {
            return m_hAction;
        }
        void                  setAction(HCoreAction hAction)
        {
            m_hAction = hAction;
        }
        void                  setAction(xBaseAction* pAction)
        {
            m_hAction = HCoreAction(pAction);
        }

    protected:
        HCoreAction           m_hAction;
	};

public:
	xMixedAction(xCoreSkeleton* pSkeleton , int param);
	~xMixedAction();
public:
    bool                 setMainAction(xBaseAction* pAction);
	bool                 setAction(const wchar_t* slotName , HCoreAction pAction);
	bool                 setAction(int iSlot , HCoreAction pAction);
    bool                 setAction(const wchar_t* slotName , xBaseAction* pAction);
    bool                 setAction(int iSlot , xBaseAction* pAction);

	xActionSlot&         actionSlot(int iSlot){ return m_ActionSlots[iSlot] ; }
    int                  nActionSlot() const { return (int)m_ActionSlots.size() ; }
    bool                 getBoneTrans(int boneIndex, xBaseActionState* _pActionState , xBoneTrans& boneTrans);
public:
	bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	bool                 load(const wchar_t * _name , xcomdocstream* stream);
	bool                 load(const wchar_t * _name , istream& stream);
	bool                 load(xXmlNode* pCfgNode );

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

	xActionSlots      m_ActionSlots;
	xBoneActionMap    m_BoneActionMap;
	xCoreSkeleton*    m_pSkeleton;
	xBaseActionState* m_pActionSlotState;
};

END_NAMESPACE_XEVOL3D
#endif
