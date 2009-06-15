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

#ifndef __WIN32__PLATFORM_H__
#define __WIN32__PLATFORM_H__
#include <windows.h>
#include <map>
#include <vector>
using namespace std;

#include <Application/xPlatform.h>
#include <Application/xApplication.h>
#include "xWindowImp.h"
BEGIN_NAMESPACE_XEVOL3D

#define MAPVKEY(x) (x)


//Platform �ﱣ��������Ϊ��Platform�����Ĵ����࣮
class PlatformWin32 : public  IPlatform
{
	IMPL_BASE_OBJECT_INTERFACE(PlatformWin32);
protected:
	int                  m_FrameTime;
	int                  m_startTickt;
public:
	IWindow*             m_pWindowInCreating;
public:
	PlatformWin32();
	~PlatformWin32();
	const wchar_t*  name();
	//��ʼ��ƽ̨
	bool            open(int argc , wchar_t** argv);

	//�ر�ƽ̨
	bool            close() ;
	//Ӧ�ó����˳�
	bool            quit() ;

	void            getMousePosition(x2DPoint& point);

	bool            setDisplayMode(int w, int h, int cl_bits,int display_fre);
	//����һ������

	IWindow*        createWindow(WIN_CREATE_STRUCT& createStruct , const xXmlNode* params = NULL) ;

	IRenderApi*     createRenderApi(int iWnd , const wchar_t* RenderApi , const xXmlNode* params);
	IWindow*        createRenderWindow(IRenderApi** pOutRenderApi ,  WIN_CREATE_STRUCT& createStruct, const wchar_t* RenderApi , const xXmlNode* params = NULL);

	//�ر�һ������
	bool            closeWindow(IWindow* pWnd) ;
	//ץȡ������Ϣ
	bool            grabWindowEvent(IWindow* pWnd);
	bool            releaseWindowEvent(IWindow* pWnd);


	//���ڹ���
	void            managerWindow(IWindow* pWindow);
	void            unmanagerWindow(IWindow* pWindow);


	void            __translateKbMsg(xWindowMsg& msg);
	void            __processMouseMsg(xWindowMsg& msg,WPARAM wParam,LPARAM lParam);
	//�õ���ͨ���ȵ�ʱ������
	int             getTicktCount() ;
	int             getFrameTime();
	int             getElapsTime();
	//������Ϣѭ��
	bool            messageLoop() ;

	bool            dealMessage();

	//������Ϣ
	bool            dispatchMsg(xWindowMsg & msg);
	bool            setMessageTranslator(ISysEventTranslator* pTranslator);

private:
	bool            insertWindow(IWindow*);
	bool            insertWindow(IWindow* , HWND hWnd);
public:
	HINSTANCE       getInstance()
	{
		return m_hInstance;
	}


	bool      _registerWindow(WIN_CREATE_STRUCT& createStruct);
public:
	static    LONG CALLBACK windowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam); 
private:

	static bool     m_bKeyPressed[256];

	typedef         map<HWND,IWindow*> WindowMap;


	WindowMap       m_Windows;

	HINSTANCE       m_hInstance;

	static Win32WindowImp* m_inCreatingWnd;
};

END_NAMESPACE_XEVOL3D


#endif
