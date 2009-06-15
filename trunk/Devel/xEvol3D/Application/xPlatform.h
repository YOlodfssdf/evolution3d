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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#include "xWindow.h"
#include "xApplication.h"
#include "xVirtualKey.h"
#include "../BaseLib/stl_dllalloc.h"
#include "../BaseLib/xXmlDocument.h"
#include "../BaseLib/logstream.h"
using namespace std;
BEGIN_NAMESPACE_XEVOL3D
//----------------------------------------------------------------

//----------------------------------------------------------------


template<typename T> class TScopeLocker
{
	T* m_pLocker;
public:
	TScopeLocker(T* pT)
	{
		m_pLocker=pT;
		if(m_pLocker) m_pLocker->lock();
	};
	~TScopeLocker()
	{
		if(m_pLocker)	m_pLocker->unlock();
	};
};

class _XEVOL_BASE_API_  IRenderApi;
class _XEVOL_BASE_API_  ISysEventTranslator
{
public:
	virtual bool translator(xWindowMsg& _winMsg) = 0;
};

class _XEVOL_BASE_API_  IPlatform : public IBaseObject
{
protected:
	ISysEventTranslator* m_pEventTranslator;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IPlatform)
public:

	IPlatform();
    virtual ~IPlatform();
	static  IPlatform*      singleton();
	virtual const wchar_t*  name() = 0;
	//��ʼ��ƽ̨
	virtual bool            open(int argc , wchar_t** argv) = 0;
	//�ر�ƽ̨
	virtual bool            close() = 0;
	//Ӧ�ó����˳�
	virtual bool            quit() = 0;
	
	virtual void            getMousePosition(x2DPoint& point) = 0;

	virtual bool            setDisplayMode(int w, int h, int cl_bits,int display_fre) = 0;
	//����һ������
	virtual IWindow*        createWindow(WIN_CREATE_STRUCT& createStruct,  const xXmlNode* params = NULL) = 0;

	//����һ����Ⱦ����
	//Win32 + OpenGL < Win32Platform Window -> OpenGL Win32 RenderInitor ->
	//Win32 + D3D    < Win32Platform Window -> D3D Win32 RenderInitor ->
	//SDL + OpenGL   < SDL Platform (OpenGL) -> OpenGL Renderer
	//XLib + OpenGL  < Unix/Linux Platform  ->  GLX OpenGL RendererInitor->
	virtual IWindow*        createRenderWindow(IRenderApi** pOutRenderApi ,  WIN_CREATE_STRUCT& createStruct, const wchar_t* RenderApi , const xXmlNode* params = NULL) = 0;
    virtual IRenderApi*     createRenderApi(int iWnd , const wchar_t* RenderApi , const xXmlNode* params = NULL) = 0;

	//�ر�һ������
	virtual bool            closeWindow(IWindow* pWnd) = 0;

	//ץסһ�����ڵ���Ϣ
	virtual bool            grabWindowEvent(IWindow* pWnd) = 0;
	virtual bool            releaseWindowEvent(IWindow* pWnd) = 0;

	//���ڹ���
	virtual void            managerWindow(IWindow* pWindow) = 0;
	virtual void            unmanagerWindow(IWindow* pWindow) = 0;

	//�õ���ͨ���ȵ�ʱ������
    virtual int             getFrameTime()  = 0;
	virtual int             getElapsTime()  = 0;
	//������Ϣѭ��
	virtual bool            messageLoop() = 0;

	//������Ϣ
	virtual bool            dispatchMsg(xWindowMsg & msg) = 0;
	virtual bool            setMessageTranslator(ISysEventTranslator* pTranslator) = 0;

	//���õ�ǰ��Ч����Ӧ�ã�
	bool                    setApplication(IApplication* pApp);
	//�õ���ǰ��Ч��Ӧ��
	IApplication*           getApplication();
protected:
	IApplication*           m_pCurrentApplication;
};

class _XEVOL_BASE_API_  IPlatformFactory
{
public:	
	virtual const wchar_t*    name() = 0 ;
	virtual IPlatform*        createInstance() = 0;

public:
	static  IPlatform*        create(const wchar_t*  name);
	static  bool              registe(IPlatformFactory* creater);
	static  int               numOfPlatform();
	static  IPlatformFactory* getByIndex(int i) ;
};

inline bool xR_CreatePlatform(const wchar_t* name)
{
	return IPlatformFactory::create(name) != NULL;
}
END_NAMESPACE_XEVOL3D
#endif

