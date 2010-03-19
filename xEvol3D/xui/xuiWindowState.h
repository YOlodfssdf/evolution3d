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

#ifndef __XUI_WINDOW_STATE_H__
#define __XUI_WINDOW_STATE_H__
#include "xuibase.h"
#include "xuiDataType.h"
#include "../font/xFontManager.h"
BEGIN_NAMESPACE_XUI
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
class _XEVOL_BASE_API_ xuiWindowBorder
{
public:
    int       m_size;
    xuiBevel  m_bevel; //�߿�Բ�ǵĴ�С
    xColor_4f m_color;
public:
    bool load(xXmlNode* pNode);
};

class _XEVOL_BASE_API_ xuiWindowTransform
{
public:
    wchar_t                m_name[16];
    xMathLib::xvec4        m_rotCenter;
    float                  m_rotAngle;
    xMathLib::xvec4        m_scale;
    xMathLib::xvec4        m_translate;

public:
    void                   setName(const wchar_t* _name);
    void                   begin();
    void                   end();
    xuiWindowTransform();

    static xuiWindowTransform  XUITRANFORM_IDENTITY;//��λ�任
};



class _XEVOL_BASE_API_ xuiTextureLayer
{
public:
    xuiRect         m_SrcRect;
    HBaseTexture    m_hTexture;
    float           m_fAlpha;
};

typedef ds_vector(xuiTextureLayer) xuiTextureLayers;

class _XEVOL_BASE_API_ xuiWindowState
{
public:
    wchar_t                 m_Name[32];
    xColor_4f               m_color;
    xColor_4f               m_textcolor;
    xuiWindowBorder         m_border;
    xuiRegion               m_Region;
    xuiTextureLayers        m_TextureLayers;
public:
    xuiWindowState(const wchar_t* name=L"");
    void operator = (const xuiWindowState& rhv);

public:
    xuiWindowBorder& border(){ return m_border ; }
    xuiRegion&       region(){ return m_Region ; }
    void             setRegion(const xuiRegion& _reg){ m_Region = _reg ; }
    void             setName(const wchar_t* _name);
    const wchar_t*   name(){ return m_Name ; }

public:
    virtual bool     load(xXmlNode* pAttrNode , xuiWindow* pWindow);
    virtual bool     draw(xuiWindow* pWindow);

protected:
    bool             _drawRegion(xuiWindow* pWindow ,xuiRegion& _region, xColor_4f& cl );
    bool             _drawRect(xuiWindow* pWindow ,xMathLib::xvec4& _rect , xColor_4f& cl);
    bool             _drawRect(xuiWindow* pWindow ,xMathLib::xvec4& _rect , xColor_4f& cl , xuiBevel& bevel);
public:
    static xuiWindowState XUISTATE_HIDE;
};

class _XEVOL_BASE_API_ xuiWindowStateBlender
{
    xuiWindowState    m_Start;
    xuiWindowState    m_End;

public:
    long              m_time;
    long              m_totalTime;
    xuiWindow*        m_pWindow;
public:
    xuiWindowStateBlender(xuiWindow* pWindow);
    virtual ~xuiWindowStateBlender();
    virtual bool update(long passedTime);
    virtual bool isInBlending() const;
    virtual bool stop();
    virtual bool setState(xuiWindowState* pStartState , xuiWindowState* pEndState , long _time);
    virtual bool setState(xuiWindowState* pEndState , long _time);
};

class _XEVOL_BASE_API_  xuiWindowTransBlender
{
protected:
    xuiWindowTransform      m_Start;
    xuiWindowTransform      m_End;
    long                    m_time;
    long                    m_totalTime;
    xuiWindow*              m_pWindow;

public:
    xuiWindowTransBlender(xuiWindow* pWindow);
    virtual ~xuiWindowTransBlender();
    virtual bool update(long passedTime);
    virtual bool isInBlending() const;
    virtual bool stop();
    virtual bool setTrans(xuiWindowTransform* pStartState , xuiWindowTransform* pEndState , long _time);
    virtual bool setTrans(xuiWindowTransform* pEndState , long _time);
    virtual bool getTrans(xuiWindowTransform& tans);
};


#pragma pack(pop)

typedef std::vector<xuiWindowState, dllsafe_alloc<xuiWindowState>> vXuiWindowState;

END_NAMESPACE_XUI

#endif

