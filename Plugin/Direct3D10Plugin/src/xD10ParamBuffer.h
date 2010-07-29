#ifndef __XEVOL_D10_SHADER_PARAM_BUFFER_H__
#define __XEVOL_D10_SHADER_PARAM_BUFFER_H__
#include <RenderAPI/xBaseShader.h>
#include  <d3d10.h>
#include  <d3d10shader.h>
#include <vector>
#include <map>
#include <string>
#include "xD10InputBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

struct xD10TextureBinding
{
	//HBaseTexture  m_hTexture;
	int             m_iTexUsage;//In the renderer side.
	int             m_iShaderSlot; // ��Shader���Slot
	wchar_t         m_Name[32];//��Shader�������
	int             m_iName;//Shader�������
	IBaseTexture*   m_pTexture;
	int             m_nTexture;
};

struct xD10SamplerBinding
{
	ISamplerState* m_pSampler;
	int            m_iShaderSlot; // ��Shader���Slot
	wchar_t        m_Name[32];
};

struct xD10BufferBinding
{
	IInputBuffer*           m_InputBuffer;
	xD3DInputBufReflection* m_pReflection;
	int                     m_bindPoint;
	std::wstring            m_Name;
	eD3DResourceBindType    m_bindType;
public:
	xD10BufferBinding();
	~xD10BufferBinding();
	void          unload();
	bool          validate();
};


typedef std::vector<xD10SamplerBinding> vD10SamplerBindings;
typedef std::vector<xD10TextureBinding> vD10TextureBindings;
typedef std::vector<xD10BufferBinding>  vD10BufferBindings;


/****************************************
��¼������Ҫ�Զ����ص�Shader
****************************************/
class xD10ShaderParamTable : public IShaderParamTable
{
    IMPL_BASE_OBJECT_INTERFACE(xD10ShaderParamTable);
public:
	xD10ShaderParamTable(IBaseShader*   pD10Shader);
	~xD10ShaderParamTable();	
//public;
public:
    bool                       load(ID3D10ShaderReflection* pShaderReflection );
    bool                       unload();
    bool                       commit();
	size_t                     nConstant();
	IShaderConstantReflection* getConstDesc(size_t idx);
	IBaseShader*               getShader();
	bool                       updateRenderContext();

	bool                       setTexture(const wchar_t* textureName , IBaseTexture* pTexture);
	bool                       setTexture(const int texName          , IBaseTexture* pTexture);
	bool                       setTextureBySlot(const int iSlot      , IBaseTexture* pTexture);

	bool                       setParamater(const wchar_t* name , float  _value);
	bool                       setParamater(const wchar_t* name , int    _value);
	bool                       setParamater(const wchar_t* name , xvec4& _value);
	bool                       setParamater(const wchar_t* name , xmat4& _value);



	bool                       setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex = 0);
	bool                       setParamater(const wchar_t* name , int*   _array , int n , int baseIndex = 0);
	bool                       setParamater(const wchar_t* name , float* _array , int n , int baseIndex = 0);
	bool                       setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex = 0);
   						       
protected:
    bool                       createResourceBindingTable(ID3D10ShaderReflection* pShaderReflection);
    bool                       createConstBufferBinding(ID3D10ShaderReflection* pShaderReflection);
    bool                       createBufferVarList(ID3D10ShaderReflectionConstantBuffer* pCBReflection , D3D10_SHADER_BUFFER_DESC& cbDesc, xD10BufferBinding& cbSlot, int bufIdex);
    bool                       createTextureBuffer(D3D10_SHADER_INPUT_BIND_DESC resDesc);
    bool                       createConstBuffer(ID3D10ShaderReflection* pShaderReflection , D3D10_SHADER_INPUT_BIND_DESC& resDesc);
    bool                       createTBuffer(ID3D10ShaderReflection* pShaderReflection , D3D10_SHADER_INPUT_BIND_DESC& resDesc);
    bool                       installSamplerTextureBinding();
    bool                       installConstBuffer();
    bool                       installBuffer();

protected:
    IBaseShader*                     m_pD10Shader;
    //��Sampler
    vD10SamplerBindings              m_SamplerBindings;
    //������
    vD10TextureBindings              m_TextureBindings;
    //�󶨵�ConstBuffer
    vD10BufferBindings               m_ConstBufferBindings;
    //��ΪShaderResourceView��Buffer;
    vD10BufferBindings               m_BufferBindings;

    //ƥ�䣬һ������˳�������һ�����ڲ���
    vecShaderConstantDescs           m_vecDesc;
    mapShaderConstantDescs           m_mapDesc;
};

END_NAMESPACE_XEVOL3D

#endif
