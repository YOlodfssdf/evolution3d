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

#ifndef __XUI_WINDOW_H__
#define __XUI_WINDOW_H__
#include "xuibase.h"
#include "xuiDataType.h"
#include "../font/xFontManager.h"
BEGIN_NAMESPACE_XUI
class _XEVOL_BASE_API_ xuiWindowManager;
class _XEVOL_BASE_API_ xuiWindow;
enum eXUIWinType
{
    eXUIWT_Control         ,
    eXUIWT_Dialog          ,
    eXUIWT_ControlContainer,

};
enum eXUIWinHitState
{
    eXUIWHS_OutWindow = 0,
    eXUIWHS_InBorder  = 1,
    eXUIWHS_InWindow  = 2,
    eXUIWHS_InNoClient= 3,

};
enum eXUIWinZOrder
{
    eXUIWZOder_BottomMost = 0,
    eXUIWZOder_Normal     = 100000,
    eXUIWZOder_TopMost    = 200000,
};

enum eXUIWinNotifyEvent
{
    eXUIWN_Close  =  0, //Dialog [close] box click
    eXUIWN_Resize ,     //Dialog is resize.
    eXUIWN_Icon   ,     //Dialog is icon.

    eXUIWN_Hide   ,     //�����ڱ�Hide�����յ������Ϣ
    eXUIWN_Show   ,     //������Show��ʱ����յ������Ϣ

    eXUIWN_MouseLeave,  //Mouse leave a control
    eXUIWN_LostFocus,    //Lost Mouse focus
    eXUIWN_LostKeyboard, //Lost Keyboard
    eXUIWN_SetFocus,
    eXUIWN_SetKeyboard,  //

    eXUIWN_ParentLostFocus,
    eXUIWN_ParentLostKeyboard,
    eXUIWN_ParentSetFocus,
    eXUIWN_ParentSetKeyboard,

    //System Notify
    eXUIWN_Sys_Resize = 20000,  //Windows is resize. [Renderer window , not the  xuiWindow]
};

enum eXUIWinCommand
{
    eXUIWCMD_Close  =  0, //Dialog [close] box click
    eXUIWCMD_Resize ,     //Dialog is resize.
    eXUIWCMD_Icon   ,     //Dialog is icon.

    eXUIWCMD_Hide   ,     //�����ڱ�Hide�����յ������Ϣ,

    //----------------------------
    eXUIWCMD_Click  ,     //����
    eXUIWCMD_SelChange ,
};
enum eXUIWinState
{
    eXUIWState_Hide,         //"hide"
    eXUIWState_Normal,       //"normal"
    eXUIWState_LostActive,   //"lostactive"
    eXUIWState_Gray,         //"gray"
    eXUIWState_Press,      //"press";
    eXUIWState_Focus,        //"focus"
    eXUIWState_KeyboardFocus,//"keyboardfocus"
};
#pragma pack(push , 1)
struct xuiWindowZValue
{
    eXUIWinZOrder m_zOrder;
    unsigned int  m_zValue;
    enum {MAX_ZVALUE = 0xffffffff};

    xuiWindowZValue()
    {
        m_zOrder = eXUIWZOder_Normal;
        m_zValue = MAX_ZVALUE;
    }
};
#pragma pack(pop)
bool operator >  (const xuiWindowZValue& lhv , const xuiWindowZValue& rhv);
bool operator == (const xuiWindowZValue& lhv , const xuiWindowZValue& rhv);
bool operator <  (const xuiWindowZValue& lhv , const xuiWindowZValue& rhv);
//=====================================================
//  xuiWindow��һЩ��������
//����������ԣ�
//     1��Region. ��ɫ��
//     2���߿�[�Ƿ��б߿򣬱߿���ɫ���߿���]
//��Ϊ����
//     1���Ƿ��ǽ��㣬�Ƿ�������Ƿ�
//
//
//======================================================


/*
xuiWindow��һЩ�������״̬:
1.  �ǲ��Ǹ���? 
2.  �������ǲ����н���
3.  �ǲ����м��̽���
4.  ��xuiWindow���ڵ�Dialog�ǲ����м��̽���
5.  �ǲ����б��״̬(check)

xuiButton��һЩ���״̬:
1.  xuiWindow��״̬
2.  ����ǲ����ڴ�������
3.  ����ǲ��ǰ���?
4.  
*/
#pragma pack(push , 1)
struct xuiWindowBorder_t
{
    int       m_size;
    xuiBevel  m_bevel; //�߿�Բ�ǵĴ�С
    xColor_4f m_color;
};

struct _XEVOL_BASE_API_ xuiWindowTransform_t
{
    wchar_t                m_name[16];
    xMathLib::xvec4        m_rotCenter;
    float                  m_rotAngle;
    xMathLib::xvec4        m_scale;
    xMathLib::xvec4        m_translate;

public:
    void                   setName(const wchar_t* _name);
    void                   begin();
    void                   end();
    xuiWindowTransform_t();

    static xuiWindowTransform_t  XUITRANFORM_IDENTITY;//��λ�任
};

class _XEVOL_BASE_API_  xuiWindowTransBlender
{
    xuiWindowTransform_t    m_Start;
    xuiWindowTransform_t    m_End;
    long                    m_time;
    long                    m_totalTime;
    xuiWindow*              m_pWindow;

public:
    xuiWindowTransBlender(xuiWindow* pWindow);
    virtual ~xuiWindowTransBlender();
    virtual bool update(long passedTime);
    virtual bool isInBlending() const;
    virtual bool stop();
    virtual bool setTrans(xuiWindowTransform_t* pStartState , xuiWindowTransform_t* pEndState , long _time);
    virtual bool setTrans(xuiWindowTransform_t* pEndState , long _time);
    virtual bool getTrans(xuiWindowTransform_t& tans);
};


struct _XEVOL_BASE_API_ xuiWindowState_t
{
    wchar_t                 m_Name[32];
    xColor_4f               m_color;
    xColor_4f               m_textcolor;
    xuiWindowBorder_t       m_border;
    xuiRegion               m_Region;
    void            setName(const wchar_t* _name);
    xuiWindowState_t(const wchar_t* name=L"");
    void operator = (const xuiWindowState_t& rhv);
    static xuiWindowState_t XUISTATE_HIDE;
};




class _XEVOL_BASE_API_ xuiWindowStateBlender
{
    xuiWindowState_t  m_Start;
    xuiWindowState_t  m_End;
    long              m_time;
    long              m_totalTime;
    xuiWindow*        m_pWindow;

public:
    xuiWindowStateBlender(xuiWindow* pWindow);
    virtual ~xuiWindowStateBlender();
    virtual bool update(long passedTime);
    virtual bool isInBlending() const;
    virtual bool stop();
    virtual bool setState(xuiWindowState_t* pStartState , xuiWindowState_t* pEndState , long _time);
    virtual bool setState(xuiWindowState_t* pEndState , long _time);
};
#pragma pack(pop)

typedef std::vector<xuiWindowState_t> xuiWindowStates_t;
typedef std::vector<xuiWindow*>       xuiWindows;
class _XEVOL_BASE_API_ xuiWindow : public IRefCountObject
{
    IMPL_REFCOUNT_OBJECT_INTERFACE(xuiWindow);
public:
    virtual   ~xuiWindow();
    xuiWindow(xuiWindow* parent , xuiWindowManager* pMgr);
    friend class xuiWindowManager;
    friend class xuiWindowStateBlender;
    friend class xuiDialog;

    bool                    addChild(xuiWindow* pWindow);
    bool                    removeChild(xuiWindow* pWindow);

    xuiWindowManager*       getWindowMgr(){return m_pWindowMgr;}
private://����ĺ���ֻ�� xuiWindowManager ����
    bool                    drawWindow();
    virtual bool            updateFrame(int passedTime , unsigned int thisTime);

public:
    virtual bool            load(xXmlNode* pCfgNode) = 0; // virtual ;
public:
    //����״̬��ѯ����
    virtual const wchar_t*  name();
    bool                    nameEq(const wchar_t* _name);

    const xuiWindowZValue&  zValue() const;
    void                    setZValue(unsigned int zValue);

    void                    setPosition(xuiPoint& pos);
    xuiPoint                getAbsPosition(xuiPoint& pos);
    xuiRect                 getAbsRect(xuiRect&   rect);

    xuiPoint                getWndAbsPos();
    xuiRect                 getWndAbsRect();
    xuiRegion&              getRegion();

    xuiWindowStateBlender*  getStateBlender();
    xuiWindowTransBlender*  getTransBlender();

    virtual void            setState(const wchar_t* _stateName, bool bBlend = true);
    virtual void            setState(eXUIWinState _state, bool bBlend = true);
    virtual void            setState(xuiWindowState_t* pState, bool bBlend = true);
    xuiWindowState_t*       findState(const wchar_t* _stateName);


    virtual bool              hasKbInputFocus();
    virtual bool              hasMouseFocus();

public:
    //�ⲿ������Ϊ����
    virtual bool            hide();
    virtual bool            isVisable();
    virtual bool            show();

    virtual eXUIWinHitState hitTest(int x , int y);
    virtual bool            hitTest(xMathLib::xvec4& rect , int x , int y);



protected:

    virtual bool            hasSeperatedKBFocus();
    //===========�ڲ���Ϊ����
    virtual bool            draw() = 0;
    virtual bool            drawBorder();


    virtual void            startHideBlend();
    virtual void            startShowBlend();

    virtual bool            setMouseFocus(bool bFocus);

    //==========��Ӧ������
    virtual void            onLostActive();
    virtual void            onActive();

    virtual void            onMouseFocus();
    virtual void            onLostMouseFocus();

    virtual bool            onShow();
    virtual bool            onHide();

    virtual bool            onNotify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2) = 0;

    virtual bool            command(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2);
    virtual bool            notify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2);

protected:
    bool                    _drawRegion(xuiRegion& _region, xColor_4f& cl );
    bool                    _drawRect(xMathLib::xvec4& _rect , xColor_4f& cl);
    bool                    _drawRect(xMathLib::xvec4& _rect , xColor_4f& cl , xuiBevel& bevel);
    bool                    __loadBorder(xuiWindowBorder_t& border , xXmlNode* pNode);
    bool                    __loadAllStates(xXmlNode* pNode);
    bool                    __loadState(xuiWindowState_t& _state, xXmlNode* pNode);
    void                    __setState(xuiWindowState_t* pState);
    bool                    __parseStyle(const wchar_t* _style);

    bool                    __fireCommand(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2);  //�������ڷ�����

    bool                    __notifyParent(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2); //�������ڷ�֪ͨ
    bool                    __notifyChildren(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2);//���Ӵ��ڷ�֪ͨ

protected:


protected:
    wchar_t              m_name[32];
    HFontRender          m_hFont;
    bool                 m_bVisable;
    bool                 m_bMouseWindow;
    bool                 m_bKbWindow;
    xuiWindowZValue      m_zValue;
    xuiWindowManager*    m_pWindowMgr;
    xuiWindowStates_t          m_vStates;
    xuiWindowState_t           m_state;
    xuiWindowStateBlender*     m_stateBlend;
    xuiWindowTransBlender*     m_transBlend;

protected:
    xuiWindow*          m_wndParent;
    xuiWindows          m_Children;
};


class _XEVOL_BASE_API_ xuiWindowCreator
{
public:
    static bool                 registeWindowCreate(xuiWindowCreator* pXUIWindowCreator);
    static xuiWindowCreator*    findWindowCreater(const wchar_t* _windowName);

public:
    virtual xuiWindow*          createInstance(xuiWindow* parent , xuiWindowManager* pMgr) = 0;
    virtual const wchar_t*      scriptFile() = 0;
    virtual const wchar_t*      type() = 0;
    virtual eXUIWinType         winType() = 0;
};

END_NAMESPACE_XUI

#endif

