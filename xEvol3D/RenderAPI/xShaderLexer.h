#ifndef _XSHADER_LEXER_H_
#define _XSHADER_LEXER_H_
#include "xRenderAPI.h"
#include "xShaderSemantic.h"
#include "xShaderDefine.h"
#include "../BaseLib/xResPackageMgr.h"

//=====================================
#define SECTION_BEGIN     "///{{"
#define SECTION_END       "///}}"
#define COMMENT_ACTION   "///>>"
#define SLOT_SECTION      L"Slot"
#define SOCKET_SECTION    L"Socket"
#define DECL_SECTION      L"Declaration"
#define CODE_SECTION      L"SourceCode"
#define DEFAULT_SECTION   L"DefaultCode"
#define PIN_SECTION       L"Description"
//=======================================

BEGIN_NAMESPACE_XEVOL3D

struct xShaderVarInfo
{
	eShaderVarType         m_Type;
	eShaderVarDataType     m_dataType;
	int                    m_nCol;
	int                    m_nRow;
	int                    m_bytePerComponent;
	int                    nComponent(){return m_nCol * m_nRow ;}

};

bool _XEVOL_BASE_API_  XEvol_IsShaderVarTypeChar(unsigned int ch);
bool _XEVOL_BASE_API_  XEvol_IsShaderVarNameChar(unsigned int ch);
class _XEVOL_BASE_API_  xShaderVarParser
{
public:
	typedef ds_map(int , eShaderVarType) mapShaderVarTypes;
	mapShaderVarTypes                    m_mapTypes;
	
public:
	xShaderVarParser();
	~xShaderVarParser();

public:
	bool                     shaderVarInfo(const char* varType , xShaderVarInfo& info);
	bool                     shaderVarInfo(eShaderVarType varType , xShaderVarInfo& info);
	eShaderVarSemantic       shaderVarSemantic(const wchar_t* semanticName);
	const wchar_t*           shaderVarSemantic(eShaderVarSemantic semantic);
    static xShaderVarParser* singleton();
protected:
	void _insert(const char* typeName , eShaderVarType _et);
   
};

#define SHADER_VAR_PARSER() xShaderVarParser::singleton()

class _XEVOL_BASE_API_  xShaderVariable
{
public:
	xShaderVariable();
	~xShaderVariable();
public:
	std::ds_string          m_TypeName;//��������
	std::ds_string          m_Name;
	std::ds_string          m_Semantic;

    xShaderVarInfo          m_info;
	size_t                  m_nElement;//������Ļ���������ĳ��ȣ����������Ϊ0
};

/*
Shader Node ���ӵ����Ϣ
*/
class _XEVOL_BASE_API_  xShaderPin
{
public:
	typedef ds_vector(xShaderVariable) vPinArgs;
	std::ds_wstring m_name;	
	std::ds_string  m_structName;
	std::ds_string  m_varName;
	vPinArgs        m_Args;
public:
	xShaderPin();
	void            set_name(const wchar_t* _name);
	int             load(const char* _code);
	bool            unload();
	const wchar_t*  name();
	bool            insertArg(const char* _datatype , const char* _argName , const char* _argSemantic);
};

/*
���һ������һ��Socket��������ɶ�Ӧ��NodeSocket����
������ʱ��裬����Socket�ϲ��������Nodes������Slot��Code

ֻ��һ��Node����Socket�ڣ��ſ�����Ϊ�Ǽܽڵ�

Socket����һ�������Pin,��Ϊ���
����һ�����Pin�ġ���Ϊ�յ㡣
*/
class _XEVOL_BASE_API_  xShaderNodeSocket
{
public:
	std::ds_wstring m_name;
	xShaderPin     m_InputPin;
	xShaderPin     m_OutPin;
public:
	xShaderNodeSocket(const wchar_t* _name);
	const wchar_t*    name();
	bool              load(const char* _code);
	bool              unload();
    const std::ds_string ansiName()
    {
        char _buf[64] = {0};
        XEvol_UnicodeToLocale(m_name.c_str() , _buf , 64);
        return _buf;
    }
};

/*
���һ���ڵ㲻�ǹǼܽڵ㣬��������һ��������Node�Ļ����������һ��Slot
ÿ��Section���ֻ����һ��Slot��
*/
class _XEVOL_BASE_API_  xShaderNodeSlot : public xShaderNodeSocket
{
public:
   std::ds_string m_funcName;
public:
	xShaderNodeSlot(const wchar_t* _name);
};
/*
��ʾShader Code ��һ�ڡ�
Sectionֻ�洢ԭʼ��Ϣ���������κζ���
*/
class _XEVOL_BASE_API_  xShaderCodeSection
{
	std::ds_string  m_code;
	std::ds_wstring m_typename;
	std::ds_wstring m_name;
public:
	xShaderCodeSection(const wchar_t* _typeName , const wchar_t* _name);
	const wchar_t* typeName(){return m_typename.c_str(); }
	const wchar_t* name(){return m_name.c_str(); }
	const char*    code(){return m_code.c_str(); }
	int            load(const char* _code);
	bool           unload();
};

END_NAMESPACE_XEVOL3D
#endif
