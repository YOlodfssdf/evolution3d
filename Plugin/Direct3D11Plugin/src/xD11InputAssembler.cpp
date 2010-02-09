#include "stdafx.h"
#include "xDirect3D11API.h"
#include "xD11InputAssembler.h"
#include "xD11InputBuffer.h"
#include "xD3DInputBuffer.h"
#include <string>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11InputAssembler , IInputAssembler);


static wchar_t * dummyShader = L"\nfloat4 main( VS_INPUT input ) : SV_POSITION  \n{\n    return input.Pos; \n}\n ";

xD11InputAssembler::xD11InputAssembler(xD3D11RenderApi* pRenderer , const wchar_t* name ,xInputLayoutDesc& desc)
:m_pRenderApi(pRenderer) , IInputAssembler(pRenderer) , m_Name(name)
{
    m_RefCount = 1;
	m_HashName = xStringHash(name);
	m_Desc = desc;
	m_D11InputLayout = NULL;
	m_pRenderApi->objectManager().inputAssemblerMgr().add( name , this);
	m_pDummyVertexShader = NULL;
}

xD11InputAssembler::~xD11InputAssembler()
{
	m_pRenderApi->objectManager().inputAssemblerMgr().remove( this );
	
	if( m_D11InputLayout ) m_D11InputLayout->Release();
}

const xInputLayoutDesc& xD11InputAssembler::desc() const
{
	return m_Desc;
}

const wchar_t* xD11InputAssembler::name() const
{
	return m_Name.c_str();
}

const size_t xD11InputAssembler::hash_name() const
{
	return m_HashName;
}

static wstring conv(const xInputElement& from , D3D11_INPUT_ELEMENT_DESC& to)
{
	wstring decl = L"float4";
    to.AlignedByteOffset = (UINT)from.m_offset;
	WideCharToMultiByte(CP_ACP , 0 , from.m_Semantic, (INT)wcslen(from.m_Semantic) , (LPSTR)to.SemanticName , 16 , NULL , NULL);
	switch(from.m_Type)
	{
	case SHADERVARTYPE_FLOAT:
		to.Format = DXGI_FORMAT_R32_FLOAT ;
		decl = L"float";
		break;
	case SHADERVARTYPE_FLOAT2:
		to.Format = DXGI_FORMAT_R32G32_FLOAT ;
		decl = L"float2";
		break;
	case SHADERVARTYPE_FLOAT3:
		to.Format = DXGI_FORMAT_R32G32B32_FLOAT ;
		decl = L"float4";
		break;
	case SHADERVARTYPE_FLOAT4:
		to.Format = DXGI_FORMAT_R32G32B32A32_FLOAT  ;
        decl = L"float4";
		break;

	case SHADERVARTYPE_HALF:
		to.Format = DXGI_FORMAT_R16_FLOAT;
		decl = L"half";
		break;
	case SHADERVARTYPE_HALF2:
		to.Format = DXGI_FORMAT_R16G16_FLOAT;
		decl = L"half2";
		break;
	case SHADERVARTYPE_HALF4:
		to.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		decl = L"half4";
		break;

	case SHADERVARTYPE_INT:
		to.Format = DXGI_FORMAT_R32_SINT ;
		decl = L"int";
		break;		
	case SHADERVARTYPE_INT2:	
		to.Format = DXGI_FORMAT_R32G32_SINT ;
		decl = L"int2";
		break;		
	case SHADERVARTYPE_INT3:	
		to.Format = DXGI_FORMAT_R32G32B32_SINT ;
		decl = L"int3";
		break;		
	case SHADERVARTYPE_INT4:	
		to.Format = DXGI_FORMAT_R32G32B32A32_SINT  ;
		decl = L"int4";
		break;

	case SHADERVARTYPE_UINT:
		to.Format = DXGI_FORMAT_R32_UINT ;
		decl = L"int";
		break;		
	case SHADERVARTYPE_UINT2:	
		to.Format = DXGI_FORMAT_R32G32_UINT ;
		decl = L"int2";
		break;		
	case SHADERVARTYPE_UINT3:	
		to.Format = DXGI_FORMAT_R32G32B32_UINT ;
		decl = L"int3";
		break;		
	case SHADERVARTYPE_UINT4:	
		to.Format = DXGI_FORMAT_R32G32B32A32_UINT  ;
		decl = L"int4";
		break;

	case SHADERVARTYPE_SHORT:
		to.Format = DXGI_FORMAT_R16_SINT;
		decl = L"int";
		break;
	case SHADERVARTYPE_SHORT2:
		to.Format = DXGI_FORMAT_R16G16_SINT;
		decl = L"int2";
		break;
	case SHADERVARTYPE_SHORT4:
		to.Format = DXGI_FORMAT_R16G16B16A16_SINT;
		decl = L"int4";
		break;

	case SHADERVARTYPE_USHORT:
		to.Format = DXGI_FORMAT_R16_UINT;
		decl = L"int";
		break;		
	case SHADERVARTYPE_USHORT2:
		to.Format = DXGI_FORMAT_R16G16_UINT;
		decl = L"int2";
		break;		
	case SHADERVARTYPE_USHORT4:
		to.Format = DXGI_FORMAT_R16G16B16A16_UINT;
		decl = L"int4";
		break;

	case SHADERVARTYPE_BYTE:
		to.Format = DXGI_FORMAT_R8_SINT;
		decl = L"int";
		break;
	case SHADERVARTYPE_BYTE2:
		to.Format = DXGI_FORMAT_R8G8_SINT;
		decl = L"int2";
		break;
	case SHADERVARTYPE_BYTE4:
		to.Format = DXGI_FORMAT_R8G8B8A8_SINT;
		decl = L"int4";
		break;

	case SHADERVARTYPE_UBYTE:
		to.Format = DXGI_FORMAT_R8_UINT;
		decl = L"int";
		break;
	case SHADERVARTYPE_UBYTE2:
		to.Format = DXGI_FORMAT_R8G8_UINT;
		decl = L"int2";
		break;
	case SHADERVARTYPE_UBYTE4:
		to.Format = DXGI_FORMAT_R8G8B8A8_UINT;
		decl = L"int4";
		break;
	}

	to.InputSlot = (UINT)from.m_BufferIdx;
	switch(from.m_InputUsage)
	{
	case INPUT_USAGE_PER_INSTANCE_DATA:
		to.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		break;
	case INPUT_USAGE_PER_VERTEX_DATA:
		to.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		break;
	}

	to.InstanceDataStepRate = (UINT)from.m_InstanceDataStepRate;
	to.SemanticIndex        = (UINT)from.m_SemanticIdx;
	return decl;
}

D3D11_INPUT_ELEMENT_DESC* xD11InputAssembler::_createInputElementDesc(xInputLayoutDesc  &   desc , wstring& inputDecl)
{
	ds_vector(std::wstring) vInputDecl;
	const xInputLayoutDesc::xInputElements_t& elementDesc = desc.inputElements();
    D3D11_INPUT_ELEMENT_DESC* D10ElementDesc = new D3D11_INPUT_ELEMENT_DESC[ elementDesc.size() ];
	inputDecl = L"struct VS_INPUT\n {\n     float4 Pos : POSITION ; \n";
	int numElements = (int)elementDesc.size();
	for(int i = 0 ; i < numElements ; i ++)
	{
		D10ElementDesc[i].SemanticName = new char[ 16 ];
		memset( (void *)D10ElementDesc[i].SemanticName , 0 , 16 );
		vInputDecl.push_back( conv(elementDesc[i], D10ElementDesc[i]) );
		//����λ��,��һ����ֱ���Թ�.
		if(elementDesc[i].m_SemanticIdx == 0 && wstring(elementDesc[i].m_Semantic) == SHADER_SEMANTIC_POSITION)
		{
			continue;
		}
		wchar_t postfix[4]={0};
		if(elementDesc[i].m_SemanticIdx != 0) swprintf(postfix , L"%d", elementDesc[i].m_SemanticIdx );
		inputDecl += (wstring(L"     ") +  vInputDecl[i] +  wstring(L"  _") +  elementDesc[i].m_Semantic + postfix + L" : " +elementDesc[i].m_Semantic + postfix + L" ; \n" );
	}

	inputDecl += L"}; ";

	return D10ElementDesc;
}

void xD11InputAssembler::_releaseInputElementDesc(D3D11_INPUT_ELEMENT_DESC* pIED , int nElement)
{
	for(int i = 0 ; i < nElement  ; i ++)
	{
		delete [] pIED[i].SemanticName;
	}
	delete [] pIED;
}

bool xD11InputAssembler::create()
{
	 const xInputLayoutDesc::xInputElements_t& elementDesc = m_Desc.inputElements();
	 wstring inputDecl = L"";
	 int numElements = (int)elementDesc.size();
	 D3D11_INPUT_ELEMENT_DESC* D10ElementDesc = NULL;//new D3D11_INPUT_ELEMENT_DESC[ elementDesc.size() ];
	 D10ElementDesc = _createInputElementDesc(m_Desc , inputDecl );
	 wstring strDummyShader = wstring(inputDecl) + dummyShader;


	 ID3D10Effect*           pEffect = NULL;
	 ID3D10EffectTechnique*  pTechnique = NULL;
	 DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	 //dwShaderFlags |= D3D11_SHADER_DEBUG;
#endif

	 char* srcBuf = new char[strDummyShader.length() * 2];
	 memset(srcBuf , 0  , strDummyShader.length() * 2 );
	 WideCharToMultiByte(CP_ACP , 0 , strDummyShader.c_str(), (int)strDummyShader.length() , srcBuf , (int)strDummyShader.length() * 2 , NULL , NULL);

	 ID3D10Blob* pShaderBlob = NULL;
	 ID3D10Blob* pErroMessageBlob = NULL;
	 const char* profileName = m_pRenderApi->getShaderProfile(eShader_VertexShader);
	 HRESULT hr = D3DX11CompileFromMemory(srcBuf, strlen(srcBuf),  "DummyShader.hlsl", NULL, NULL, "main" , profileName,
		                                  dwShaderFlags, 0, NULL, &pShaderBlob, &pErroMessageBlob , NULL);
	 delete [] srcBuf;
	 
	 if( FAILED( hr ) )
	 {
		 char* strAnsiBuffer = (char*)pErroMessageBlob->GetBufferPointer();
		 
		 XEVOL_LOG(eXL_ERROR_FALT,L"Compile Dummy shader (create Vertex Layout ) failed , error info : %s " , strAnsiBuffer);
		 pErroMessageBlob->Release();
		 _releaseInputElementDesc(D10ElementDesc , numElements);
		 return false;
	 }

	 hr = m_pRenderApi->d11Device()->CreateInputLayout( D10ElementDesc, numElements, 
		 pShaderBlob->GetBufferPointer(),
		 pShaderBlob->GetBufferSize(), &m_D11InputLayout );
	 _releaseInputElementDesc(D10ElementDesc , numElements);
	 pShaderBlob->Release();
     XSAFE_RELEASE(pErroMessageBlob);

	 if( FAILED( hr ) )
		 return false;

	 return true;
}


IVertexStream* xD11InputAssembler::createVertexStream()
{
	return new xD3DVertexStream(m_pRenderApi , this);
}

END_NAMESPACE_XEVOL3D

