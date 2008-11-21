#ifndef __XEVOL_BASE_TEXTURE_H__
#define __XEVOL_BASE_TEXTURE_H__
#include "xRenderObject.h"
#include "../BaseLib/stl_dllalloc.h"
#include "../BaseLib/xStringHash.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
/*
�����Sampler��ʹ�ù���
1. ����֪��ʹ��ɶShader�Ķ���ֱ��ʹ��setSampler(L"samplerName")��setTexture(L"TextureName")�����á�
   ����ʹ��Ԥ�����iSamplerSlot��iTexSlot���Ż�

2. ���ڲ�֪��ʹ��ɶShader�Ķ��󣬲��ð����Ļ��ƣ���Shader�󶨵�������ȥ.��ΪShader֪���Լ���Ҫʲô��
   Shader���Texture/Sampler���ֶ�ָ��ʹ���ĸ�slot����shader��bind��ʱ���Զ���RenderApi��Ӧslot�еĶ���
   Ӧ�õ�Shader�
   RenderApi�ᶨ��һȺԤ�Ƶ�Sampler����ЩSampler�Ĳ���д�ڽű�������Զ����أ�Shader��Sampler����ָ��ʹ����ЩSampler.
   RenderApi�ᶨ��һȺԤ�Ƶ�Texture Slot. ��Slotʹ��hash ID��Ϊ��ǡ�Shader�л�ʹ�ø�hashID��ָ��ʹ���ĸ�Slot��
  
�󶨷���1. DrawalbeObject->m_pGpuProgram->bindTexture(L"DiffuseTexture" , pTexture);
�󶨷���2. RenderApi->setTexture(L"TextureSlot",pTexture);�������
  

*/

struct xTextureLockArea
{
     char*  m_pixels;
	 int    m_width;
	 int    m_height;
	 int    m_depth;
	 int    m_picth;
	 int    m_slice_pitch;
	 long   m_lockResource;
};

struct xTextureDesc
{
	int           m_width;
	int           m_height;
	int           m_depth;
	ePIXEL_FORMAT m_fmt;
	int           m_nArraySize;
};



class _XEVOL_BASE_API_   IBaseTexture : public IRenderResource
{
public:
	IBaseTexture(IRenderApi* pRenderApi);
	virtual ~IBaseTexture();
public:
	virtual ePIXEL_FORMAT  format() = 0;
	virtual bool           validate() = 0;
	virtual bool           desc(xTextureDesc& desc) = 0;
	virtual bool           update(void* data  , int dateLen ,int mipmapLevel = 0 , int arraySlice = 0) = 0 ;
	virtual bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0) = 0 ;
	virtual bool           unlock(xTextureLockArea& lockInfo) = 0 ;
     
	virtual bool           create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) = 0;
	virtual bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) = 0;
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0) = 0;
};


class _XEVOL_BASE_API_  ITextureLoader : public IBaseObject
{
public:
	virtual IBaseTexture*  createTexture(IRenderApi* pApi) = 0;
	virtual IBaseTexture*  loadTexture(IRenderApi* pApi , const wchar_t* textFile, void* _buf , int bufLen , const xTextureName& _texName ) = 0;
	virtual size_t         nExt() = 0;
	virtual const wchar_t* ext(size_t idx) = 0;
};

class _XEVOL_BASE_API_ xTextureLdrMgr
{
	xTextureLdrMgr();
	~xTextureLdrMgr();
public:
	//�Զ������������
	ITextureLoader*        findTextureLoader(const wchar_t* texExt);
	void                   addTextureLoader(ITextureLoader* pTextureLoader);
	void                   removeTextureLoader(ITextureLoader* pTextureLoader);
	static xTextureLdrMgr* singleton();
};


END_NAMESPACE_XEVOL3D
#endif
