#ifndef __XEVOL_BASE_SHADER_H__
#define __XEVOL_BASE_SHADER_H__
#include "xRenderAPI.h"
#include "xShaderSemantic.h"
#include "xShaderNode.h"
#include "xShaderDefine.h"
#include "../XMathLib/XMathLib.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
/*
��һ���֣�
  shaderϵͳ������shader��ɲ��֣�
  һ: ������SHader���룬������Shader���������ִ�С�����Ҫshader�޸���һ��ġ�
  ��: shader�������롣������shader������������޸�������װĬ���޸��������ɶ���ִ�С�
  ��: shader�޸������롣������һ��������
  ��: shader������ģ�顣������shaderģ�飬��ʾһ��ģ�����ʲô�������������
    ������ϵͳ�������������ݽ��л�������
    Vertex Shaderͨ����һ����Ӧ��Input Assembler��ϵͳ
    Pixel Shader���Ӧ���ץȡ�������ݺͽ���GS/VS�������������ݡ�
    ����˵��һ����Ƶ�����ps������ģ�������һ��ת��YUVΪRGB�����������ȣ��Աȶȵ��ڵ�����ģ�顣
    ����ģ������ģ�飬�޸���ϵͳһ�����һ��������shader.
    
    shader����ϵͳ����Ҫ��������������⣬����Ҫ�����ϵͳ��������ݽ��ж��塣�����Ľṹ������������
    ����shader��������һ������������

  ��:shader���ģ��:
    ������shaderģ�飬��ʾһ��shader������������ʲô�������ݡ�
    VS�����ģ��Ҫ��gs/ps������ϵͳƥ�䡣
    ps�����ģ������Զ���������ٸ�RenderTarget�ȡ�
    ���ģ������������Ҫ�����������Ա����Ӧ������ģ�����ƥ�䡣
    ����shader��������һ�����������
    
  ��:shader�Ŀ����ļ���
    ��������������������5�������е���Щ���������һ���������õ�shader��
    

�ڶ����֣�
  shader�޸���ϵͳ�����
  (���¸����־�Ϊ�;����shader api�йأ�����)

  shader��������:
  shader�������������ʹ�����޸�����������Ĭ�ϵ����̣�
  �������#ifdef�����ơ�Ҳ������Shaderϵͳ��Ĭ�ϵĲ�����롣
  shader���޸�����������
  ����shader�޸��������ͣ������ȣ������ܶ����Ĭ�ϵĲ������
  shader���޸�������:
  ����޸����������һ���������޸�����
  һ���޸���������뵽shader�޸���֮ǰ��Ҫƥ�������
  shader�޸�����ջ:
  һ����������������޸�����������޸����������뵽shader�������γ���shader�Ľṹ��

*/

/*
�ﵽ��Ŀ�ģ�
    1:  ����õ�IGpuProgram��Object�������ύ�Լ��� IGpuConstantTable
	2:  ÿ��IGpuProgram�������Լ���IGpuConstantTable��Ҳ����û�С�
	3:  IGpuConstantTable������һ��IGpuProgram���е�constant table , VertexShader , Pixel Shader , 
	4:  IGpuConstantTable�������Լ�������Щ��Ⱦ���󶨵�Shader����.
*/


/*
����������Щ��Ҫ�Զ����µı���
����任����ʲô��.
*/
class _XEVOL_BASE_API_  IShaderConstBinder : public IRefCountObject
{
public:
	virtual ~IShaderConstBinder(){};
	IShaderConstBinder(){}
	virtual void                  setDirty() = 0;
	//����true����ʾ������������
	virtual bool                  updateConstant(IShaderConstantReflection* pConst) = 0;
	virtual IShaderConstBinder*   createInstance() = 0 ;
};

struct xShaderConstantDesc
{
public:
	unsigned  int           m_bufOffset;//���Constant��Const Buffer���ƫ��λ��.
	eShaderVarType          m_varType;//ʲô��;����������ɫ�����Ǿ���
	eShaderParamSemantic    m_semantic;//���壿�����������Param�Ǹ�ʲô��
	unsigned int            m_element;//��������飬���ж೤
	std::ds_wstring         m_strName;//����������
	int                     m_hashName;//������Hash����
};

class _XEVOL_BASE_API_  IShaderConstantReflection : public xShaderConstantDesc
{
public:
	IShaderConstBinder*     m_pBinder;//�󶨺����Զ����²���.
public:
	PRIVATE virtual  bool setData(void* _data , int dataLen) = 0;
	PRIVATE virtual  bool updateData();
};

class _XEVOL_BASE_API_ IInputBufferReflection : public IRefCountObject
{
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(IInputBufferReflection);
public:
	IInputBufferReflection(){};
	virtual ~IInputBufferReflection(){}

	virtual size_t                 lenght() = 0;
	virtual size_t                 nConstant() = 0;
	virtual IShaderConstantReflection*   constantDesc(size_t idx) = 0;
	virtual bool                   setData(int _off , void* _data , int dataLen) = 0;
};

class _XEVOL_BASE_API_  IShaderParamTable  : public IRefCountObject
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(IShaderParamTable)
public:
	virtual ~IShaderParamTable();
	IShaderParamTable();
	PUBLIC  virtual size_t                 nConstant() = 0;
	PUBLIC  virtual IShaderConstantReflection*   getConstDesc(size_t idx) = 0;
	PUBLIC  virtual IBaseShader*           getShader() = 0;
	
	/*
	�Զ�������ȾAPI��״̬��
	����: 
	�任����:          World , View , WorldView , Normal , MVP , InvWorld , InvView , InvWorldView .
	�ƹ�: 
	fog:               Arg , Color.
	TextureStage Arg:  �����С
	*/

	PRIVATE virtual bool                   updateRenderContext() = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float  _value) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int    _value) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4& _value) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4& _value) = 0;

	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex = 0) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int*   _array , int n , int baseIndex = 0) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float* _array , int n , int baseIndex = 0) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex = 0) = 0;
};

class _XEVOL_BASE_API_  IBaseShader : public IRenderResource
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IBaseShader)
public:
    IBaseShader(IRenderApi* pOwner);
	virtual ~IBaseShader();
	PRIVATE virtual IShaderParamTable*       createShaderParamTable() = 0;
	PUBLIC  virtual bool                   load(const wchar_t* fileName , unsigned long  arg) = 0;
	PUBLIC  virtual bool                   load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	PUBLIC  virtual bool                   isLoaded()  = 0;
	PUBLIC  virtual bool                   unload() = 0;
	PUBLIC  virtual unsigned long          memUsage() = 0;
	//PUBLIC  virtual IShaderNodePin*      inputPin() = 0;
	//PUBLIC  virtual IShaderNodePin*      outPin() = 0;
	PUBLIC  virtual eResourceType          res_type(){ return RESOURCE_SHADER ; }

public:
	PUBLIC  virtual eShaderType            type() = 0;
};


class _XEVOL_BASE_API_  IGpuProgramParamTable  : public IRefCountObject
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD10GpuProgram)
public:
	virtual ~IGpuProgramParamTable();
	IGpuProgramParamTable();
	PUBLIC  virtual IGpuProgram*           getGpuProgram() = 0;
	PUBLIC  virtual IShaderParamTable*       getShaderParamTab(eShaderType shType) = 0;

	/*
	�Զ�������ȾAPI��״̬��
	����: 
	      �任����:          World , View , WorldView , Normal , MVP , InvWorld , InvView , InvWorldView .
	      �ƹ�: 
		  fog:               Arg , Color.
		  TextureStage Arg:  �����С

	*/
	//����Ⱦ��ʹ��

	PRIVATE virtual bool                   updateRenderContext() = 0;
	PRIVATE virtual bool                   commit() = 0;

	PUBLIC  virtual bool                   setTexture(const wchar_t* texName     , IBaseTexture* pTexture , eShaderType _shader =  eShader_None) = 0;
	PUBLIC  virtual bool                   setTexture(const int texName          , IBaseTexture* pTexture , eShaderType _shader =  eShader_None) = 0;
	PUBLIC  virtual bool                   setTextureBySlot(const int iSlot      , IBaseTexture* pTexture , eShaderType _shader =  eShader_None) = 0;

    PUBLIC  virtual bool                   load(xCfgNode* pXMLNode) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float  _value , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int    _value , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4& _value , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4& _value , eShaderType shType = eShader_None) = 0;

	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xvec4* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , int*   _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , float* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
	PUBLIC  virtual bool                   setParamater(const wchar_t* name , xmat4* _array , int n , int baseIndex = 0 , eShaderType shType = eShader_None) = 0;
};


class _XEVOL_BASE_API_  IGpuProgram : public IRenderResource
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IGpuProgram)
public:
	IGpuProgram(IRenderApi* pOwner);
	virtual ~IGpuProgram();
	PUBLIC virtual bool                    load(const wchar_t* fileName , unsigned long  arg) = 0;
	PUBLIC virtual bool                    load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	PUBLIC virtual bool                    load(const wchar_t* vsName , const wchar_t* psName, const wchar_t* gsName , unsigned long arg) = 0;
	PUBLIC virtual bool                    isLoaded()  = 0;
	PUBLIC virtual bool                    unload() = 0;
	PUBLIC virtual unsigned long           memUsage() = 0;
	PUBLIC virtual eResourceType           res_type(){ return RESOURCE_SHADER ; }
public:
	/*
	GpuProgramConstTable�Ĺ���
	*/
	PUBLIC  virtual IGpuProgramParamTable*   createParamTable(bool bLoadDef) = 0;
	PUBLIC  virtual IGpuProgramParamTable*   getParamTable() = 0;
	PUBLIC  virtual void                   setParamTable(IGpuProgramParamTable* pTable) = 0;

	//GpuProgram ������Ĺ���
	PUBLIC  virtual bool                   setTexture(const wchar_t* textureName , IBaseTexture* pTexture, eShaderType _shader =  eShader_None) = 0;
    PUBLIC  virtual bool                   setTexture(const int texName          , IBaseTexture* pTexture, eShaderType _shader =  eShader_None) = 0;
	PUBLIC  virtual bool                   setTextureBySlot(const int iSlot      , IBaseTexture* pTexture, eShaderType _shader =  eShader_None) = 0;
	//����Ⱦ��ʹ��
	PRIVATE virtual IGpuProgramParamTable*  getDefaultParamTable() = 0;
	PRIVATE virtual void                  commit() = 0 ;
	
};

END_NAMESPACE_XEVOL3D
#endif
