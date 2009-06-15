#ifndef __XEVOL3D_DRAWELEMENT_H__
#define __XEVOL3D_DRAWELEMENT_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../XMathLib/XGeomLib.h"
#include "../XMathLib/XGVolume.h"
#include "xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D

//-------------------
//�����еĽڵ�
// ISceneNode
//      ----- xRenderEffect : ��¼�ýڵ���ɶEffect����������
//      ----- vector<xRenderPassArg> : ��¼ÿһ��Pass�Ĳ�����
//      ----- load() ����load Effect. Ȼ��loadÿһ��xRenderPassArg. ���xRenderPassArg��û�в�������ʹ��Ĭ�ϵġ�

//�ǳ����Ľڵ㣬��ȫdemo���Ի�
//Object
//        ----xRenderEffect
//        ----create: ��ȡÿ��RenderPass,Ȼ��Ϊÿ��RenderPass���ò�������������ã���ʹ��Ĭ�ϵĲ�����
//-------------------

//���еĿɻ��Ƶ�ԭɫ����IDrawableObject������
struct sDrawElementInfo_t
{
      char m_queueIndex;
      int  m_queueName;
};

class _XEVOL_BASE_API_  IDrawableObject : public IRenderObject
{
protected:
	IDrawableObject(IRenderApi* pRenderApi);
	virtual ~IDrawableObject(){};   
public:
      DECL_BASE_OBJECT_DLLSAFE(IDrawableObject);
      //���ζ�λ�Ͳü��á�
      virtual void           center(XMathLib::xvec3&   _center) = 0;
      virtual void           aabb(xGeomLib::xaabb&     _aabb) = 0;
      virtual void           shpere(xGeomLib::xshpere& _shpere) = 0;
      virtual IRenderApi*    renderApi() = 0;
      virtual bool           render(unsigned long passedTime) = 0;
      virtual bool           update(unsigned long passedTime) = 0 ;

	  //--------------------------------------------------------
};


class _XEVOL_BASE_API_ IDrawElement : public IDrawableObject
{
public:
      DECL_BASE_OBJECT_DLLSAFE(IDrawElement);
      IDrawElement(IRenderApi* pRenderApi);
      virtual ~IDrawElement() = 0{};    
};



//��������Ͷ��RenderQueue��Ķ���
class _XEVOL_BASE_API_ xRenderPass  : IRenderObject
{
public:
   DECL_BASE_OBJECT_DLLSAFE(IRenderObject);   
   xRenderPass(IRenderApi* pRenderApi);
   virtual ~xRenderPass();
public:
   XEVOL_DEFINE_PROPERTY_P(xMaterial             , Material);
   XEVOL_DEFINE_PROPERTY_P(IDrawElement          , Drawable);
   XEVOL_DEFINE_PROPERTY_P(IGpuProgramParamTable , ConstTable);

public:
	virtual bool  render(unsigned long passedTime);
};

END_NAMESPACE_XEVOL3D
#endif
