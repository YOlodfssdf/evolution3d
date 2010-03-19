#ifndef __XEVOL_BASE_TEXTURE_H__
#define __XEVOL_BASE_TEXTURE_H__
#include "xRenderApiObject.h"
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

enum
{
    eTextureArray_CubeMap = -6,
};

struct xTextureDesc
{
	int           m_width;
	int           m_height;
	int           m_depth;
	ePIXEL_FORMAT m_fmt;
	int           m_nArraySize; //ArraySize =  -6 Means CubeMap
};

enum   
{
    eCUBESIDE_POSITIVE_X = 0, 
    eCUBESIDE_NEGATIVE_X,

    eCUBESIDE_POSITIVE_Y,
    eCUBESIDE_NEGATIVE_Y,

    eCUBESIDE_POSITIVE_Z,
    eCUBESIDE_NEGATIVE_Z,

};

struct xTextureInitData
{
    void* m_pData;
    int   m_Pitch;
    int   m_DepthPitch;
    int   m_DataLen;
};

struct xTextureInitDesc
{
    xTextureDesc         m_TextureDesc;
    int                  m_nMipmap;
    eResourceUsage       m_usage ;//= RESOURCE_USAGE_DYNAMIC , 
    eResourceAccessFlage m_access ;//= RESOURCE_ACCESS_WRITE , 
    eResourceBindType    m_bindType ;//= BIND_AS_SHADER_RESOURCE
    bool                 m_bReadable;
public:
    xTextureInitDesc(int w = 0, int h = 0, ePIXEL_FORMAT fmt = PIXELFORMAT_R8G8B8A8 , int depth = 1  )
    {
        m_TextureDesc.m_width = w;
        m_TextureDesc.m_height= h;
        m_TextureDesc.m_depth = depth;
        m_TextureDesc.m_fmt   = fmt;
        m_TextureDesc.m_nArraySize = 1;
        m_nMipmap = 1;
        m_usage   = RESOURCE_USAGE_DYNAMIC;
        m_access  = RESOURCE_ACCESS_WRITE;
        m_bindType= BIND_AS_SHADER_RESOURCE ;
        m_bReadable = false;
    }
};

class _XEVOL_BASE_API_   IBaseTexture : public IRenderApiResource
{
public:
	IBaseTexture(IRenderApi* pRenderApi);
	virtual ~IBaseTexture();
public:
	virtual ePIXEL_FORMAT  format() = 0;
	virtual bool           validate() = 0;
	virtual bool           desc(xTextureDesc& desc) = 0;
	virtual bool           update(void* data  , int dateLen , int rowPitch , int depthPicth =0 , int mipmapLevel = 0 , int arraySlice = 0) = 0 ;
	virtual bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0) = 0 ;
	virtual bool           unlock(xTextureLockArea& lockInfo) = 0 ;
     
	virtual bool           create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0) = 0;
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0) = 0;
	virtual bool           saveToFile(const wchar_t* fileName) = 0;
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
