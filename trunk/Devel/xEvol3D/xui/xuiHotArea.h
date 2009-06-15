/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
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

#include "xuiWindow.h"
BEGIN_NAMESPACE_XUI

class _XEVOL_BASE_API_ xuiHotArea
{
public:
    enum eHotAreaActive
    {
        eHOTAREA_ACTIVE_SHOW , //�����ڴ�����ʾ��ʱ����Ч��
        eHOTAREA_ACTIVE_HIDE , //�����ڴ������ص�ʱ����Ч��
        eHOTAREA_ACTIVE_ANY  , //�κ�ʱ����Ч��
    };

    enum eHotAreaAction
    {
        eHOTAREA_ACTION_HIDE = 0, //�����ڴ�����ʱ�����ش���
        eHOTAREA_ACTION_SHOW = 1, //�����ڴ�����ʱ����ʾ����
        eHOTAREA_ACTION_USER = 2, //�����ڳ�����ʱ��ִ���û��ص���
    };
protected:
    eHotAreaAction    m_action;
    eHotAreaActive    m_active;
    bool              m_bEnable;
    xuiRegion         m_Region;
    wchar_t           m_name[32];
    xuiWindow*        m_pOwnder;
    xuiWindowManager* m_pWindowMgr;
    wstring           m_event;
    bool              __checkEvent(eMsgID msgID);
    bool              __checkActive();
public:
    xuiHotArea(xuiWindow* pWindow, xuiWindowManager* pWindowMgr);
    ~xuiHotArea();
    bool           hitTest(int x , int y);
    void           enable();//���ø�������
    void           disable();//�رո�����.
    bool           isEnable();

    eHotAreaAction getAction();
    eHotAreaActive getActiveType();
    void           fire(eMsgID msgID);//����
    xuiRegion&     getRegion();
    bool           load(xXmlNode* pCfgNode);

};

END_NAMESPACE_XUI
