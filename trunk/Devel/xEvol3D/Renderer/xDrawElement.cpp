#include "xDrawElement.h"
#include "xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(IDrawableObject ,  IRenderObject);
IMPL_BASE_OBJECT_DLLSAFE(IDrawElement    ,  IDrawableObject);
IMPL_BASE_OBJECT_DLLSAFE(xRenderPass     ,  IRenderObject);

IDrawableObject::IDrawableObject(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}
IDrawElement::IDrawElement(IRenderApi* pRdr)
:IDrawableObject(pRdr)
{
}

xRenderPass::xRenderPass(IRenderApi* pRenderApi)
:IRenderObject(pRenderApi)
{

}

xRenderPass::~xRenderPass()
{

}

bool  xRenderPass::render(unsigned long passedTime)
{
   if(m_Drawable == NULL)
	   return false;
  
   //�в���
   if(m_Material)
   {
	    //Ӧ�ò���
	   m_Material->begin(m_ConstTable);

	   //��������
	   m_Material->render(m_Drawable,passedTime);

	   //����ʹ�ý���
	   m_Material->end(m_ConstTable);
   }
   else
   {
	   //��������
	   m_Drawable->render(passedTime);
   }
   return true;
}

END_NAMESPACE_XEVOL3D
