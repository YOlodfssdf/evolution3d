#ifndef __XEVOL3D_RENDERER_H__
#define __XEVOL3D_RENDERER_H__
#include "../RenderAPI/xRenderAPI.h"
/*********************************************

xRenderer����ƣ�

��Ⱦ�Ĳ��ϣ�
  ����Meshһ��Ķ����� �����xDrawableObject

��Ⱦ�ĵ�λ��
   xDrawElement. xDrawableObjectֻ��һ�����塣
   ������һ��֪�������Ⱦ�Լ������塣������
   һ�����߶��xDrawElement.

��Ⱦ�Ĳ�����
   xSurfaceMaterial: �������
   xGPUPrograme: ����Shader�����Լ�shader����.
   xTextureLayer: һ����߶���texture.
   
��Ⱦ�Ĺ��ߣ�
  RenderScript . RenderScript������������ݣ�
  ��xDrawableObject ������һ����xDrawElement.
  xDrawableObject�������xDrawElement����һЩ
  �������������ҪRenderScript��ı���Щ������
  ���߲�����Щ�������Ĳ�����

��Ⱦ�ĳ���:
  Renderer: 
  Renderer����һ����Ⱦ���е��б���ά�������
  ��Ⱦ��Щ���е�RendererScript.



Renderer �� RenderScript���߼����롣ͨ����������
��ͬ��Ϊ��Render��ʽ��

xSurfaceMaterial xTextureLayer xDrawableObject
��Щ�������ṩ���ݶ��ѡ�

*********************************************/

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xRendererQueue;
class _XEVOL_BASE_API_ IDrawElement;
class _XEVOL_BASE_API_ xMaterial;

class _XEVOL_BASE_API_ IRenderer
{
public:
	virtual bool              loadConfig(const wchar_t* xmlName , const int8* xmlData , int len) = 0;
	virtual int               nRenderQueue() = 0;
	virtual xRendererQueue*   RenderQueueByName(int QueueName) = 0;
	virtual xRendererQueue*   RenderQueueByName(const wchar_t* QueueName) = 0;
	virtual xRendererQueue*   RenderQueue(int iQueue) = 0;
	virtual bool              drawObject(IDrawElement* pObject , bool drawImm = false) = 0;
    virtual bool              setRenderTech(const wchar_t* techName) = 0;
	virtual const char*       getRenderTech() = 0;
	virtual bool              begin() = 0;
	virtual bool              end() = 0;
	virtual xMaterial*        createMaterial(const wchar_t* matName) = 0 ;
	virtual void              deleteMaterial(xMaterial* pMaterial) = 0 ;
	virtual xMaterial*        loadMaterial(const wchar_t* matFileName) = 0;
};

class _XEVOL_BASE_API_ IRendererCreator
{
public:
	virtual const wchar_t* name() const = 0;
	virtual IRenderer*     createRenderer() const = 0;
};

class _XEVOL_BASE_API_ xRendererManager
{
	xRendererManager();
	IRenderer*     m_pRenderer ;
public:
	static xRendererManager* singleton();
    IRenderer*               getRenderer();
	bool                     registeRenderer(IRendererCreator* pRendererCreator);
	bool                     unregisteRenderer(IRendererCreator* pRendererCreator);
	IRenderer*               createRenderer(const wchar_t* name);
};

END_NAMESPACE_XEVOL3D

#endif