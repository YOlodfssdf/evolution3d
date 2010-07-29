#ifndef __XEVOL3D_DRAWELEMENT_H__
#define __XEVOL3D_DRAWELEMENT_H__
#include "../RenderAPI/xRenderAPI.h"
#include "xRenderObject.h"
#include "../XMathLib/xGeomLib.h"
#include "../XMathLib/xvolume.h"
#include "xMaterial.h"
#include "../BaseLib/xObjectFactory.h"
BEGIN_NAMESPACE_XEVOL3D

//-------------------
//�����еĽڵ�
// ISceneNode
//      ----- IRenderEffect : ��¼�ýڵ���ɶEffect����������
//      ----- vector<xRenderPassArg> : ��¼ÿһ��Pass�Ĳ�����
//      ----- load() ����load Effect. Ȼ��loadÿһ��xRenderPassArg. ���xRenderPassArg��û�в�������ʹ��Ĭ�ϵġ�

//�ǳ����Ľڵ㣬��ȫdemo���Ի�
//Object
//        ----IRenderEffect
//        ----create: ��ȡÿ��RenderPass,Ȼ��Ϊÿ��RenderPass���ò�������������ã���ʹ��Ĭ�ϵĲ�����
//-------------------

//���еĿɻ��Ƶ�ԭɫ����IDrawElement������
struct sDrawElementInfo_t
{
      char m_queueIndex;
      int  m_queueName;
};

class _XEVOL_BASE_API_  IDrawElement : public IRenderObject
{
protected:
	IDrawElement(IBaseRenderer* pRenderer);
	virtual ~IDrawElement(){};   
public:
      DECL_BASE_OBJECT_DLLSAFE(IDrawElement);
      //���ζ�λ�Ͳü��á�
      virtual void                     center(xMathLib::xvec3&   _center) = 0;
      virtual void                     aabb(xGeomLib::xaabb&     _aabb) = 0;
      virtual void                     shpere(xGeomLib::xshpere& _shpere) = 0;
      virtual IRenderApi*              renderApi();
      virtual bool                     render(unsigned long passedTime) = 0;
      virtual bool                     update(unsigned long passedTime) = 0 ;
	  virtual bool                     isVisible(xGeomLib::xCamera* pCamera ,  const xMathLib::xmat4& _mat);
	  //--------------------------------------------------------
};



class _XEVOL_BASE_API_  xSerializeDrawElement : public IDrawElement
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xSerializeDrawElement);
	xSerializeDrawElement(IBaseRenderer* pRenderer, int arg);
	virtual ~xSerializeDrawElement();
	virtual bool   load(xXmlNode* pXml) = 0;
	virtual bool   save(xXmlNode* pNode) = 0;
};

class _XEVOL_BASE_API_ IRenderPassArg : public IBaseObject
{
public:
	DECL_BASE_OBJECT_DLLSAFE(IRenderPassArg); 
	IRenderPassArg();
	virtual ~IRenderPassArg();
	virtual bool                     setMatrix(const xMathLib::xmat4& _mat);
	virtual const xMathLib::xmat4&   getMatrix();
protected:
	xMathLib::xmat4                  m_trans;
};

//��������Ͷ��RenderQueue��Ķ���
class _XEVOL_BASE_API_ xRenderPass  : public  IRenderObject
{
public:
   DECL_BASE_OBJECT_DLLSAFE(xRenderPass);   
   xRenderPass(IBaseRenderer* pRenderer);
   virtual ~xRenderPass();
public:
   XEVOL_DEFINE_PROPERTY_P(xMaterial             , Material);
   XEVOL_DEFINE_PROPERTY_P(IDrawElement          , Drawable);
   XEVOL_DEFINE_PROPERTY_P(IGpuProgramParamTable , ConstTable);
   XEVOL_DEFINE_PROPERTY_P(IRenderPassArg        , RenderPassArg);
public:
	virtual bool  render(unsigned long passedTime);
};



class  _XEVOL_BASE_API_ IDrawElementCreator;
class  _XEVOL_BASE_API_ xDrawElementFactoryMgr;
class  _XEVOL_BASE_API_ xRenderPassCreator;
class  _XEVOL_BASE_API_ xRenderPassFactoryMgr;
DECL_OBJECT_FACTORY_AND_MGR(IDrawElement ,IDrawElementCreator , xDrawElementFactoryMgr , IBaseRenderer* , int);
DECL_OBJECT_FACTORY_AND_MGR(xRenderPass  ,xRenderPassCreator  , xRenderPassFactoryMgr  , IBaseRenderer* , int);
END_NAMESPACE_XEVOL3D
#endif
