#include "../xStdPch.h"
#include "xShaderName.h"
#include "../BaseLib/xStringHash.h"
#include "../BaseLib/xBaseLexer.h"
#include "../BaseLib/xI18N.h"
#include "../OperationSys/xOperationSys.h"
#include <sstream>
#include <set>
BEGIN_NAMESPACE_XEVOL3D

template <typename _TContainer > void T_AddImportLib( ds_string& _code, _TContainer &vLibImportSet, xShaderCodeNodeMgr* pNodeMgr ) 
{
	_code += "//===============================================\n";
	_code += "//Begin import lib\n";
	for(typename _TContainer::iterator pos = vLibImportSet.begin() ; pos != vLibImportSet.end() ; pos ++ )
	{
		ds_wstring libName = *pos ;
		HShaderCodeNode hLibNode = pNodeMgr->add(libName.c_str() , 0 , true);
		if(hLibNode.getResource())
		{
			std::ds_string libCode;
			_code += "//Import lib name=";
			char _ansiLibName[256] = {0};
			XEvol_UnicodeToLocale(libName.c_str() ,_ansiLibName , 256 );
			_code += _ansiLibName;
			_code += "\n";
			hLibNode->generateCode(libCode);
			_code += libCode;
		}
	}
	_code += "//Finish import lib\n";
	_code += "//============================================\n\n";	
}

void AddImportLib( ds_string& _code, std::set<ds_wstring> &vLibImportSet, xShaderCodeNodeMgr* pNodeMgr ) 
{
	T_AddImportLib(_code , vLibImportSet , pNodeMgr);
}

void AddImportLib( ds_string& _code, ds_wstring_vector &vLibImportSet, xShaderCodeNodeMgr* pNodeMgr ) 
{
	 T_AddImportLib(_code , vLibImportSet , pNodeMgr);
}

void  xGpuProgramName::clear()
{
	m_bLoadFromFile = false;
	m_Shaders.clear();
	m_hash1 = 0;
}
bool xGpuProgramName::hasShader() const
{
	size_t nShaders = m_Shaders.size() ;
	for(size_t  i =0 ; i < nShaders ;  i ++)
	{
		eShaderType _st = m_Shaders[i].second;
		if(_st != eShader_None)
			return true;
	}
	return false;
}
bool xGpuProgramName::toStdName(const wchar_t* shaderName[]) const
{
	size_t nShaders = m_Shaders.size() ;
	for(size_t  i =0 ; i < nShaders ;  i ++)
	{
		eShaderType _shaderType = m_Shaders[i].second;
		if(_shaderType != eShader_None)
		{
			shaderName[_shaderType] = m_Shaders[i].first.c_str();
		}
	}
	return true;
}

bool xGpuProgramName::toName(ds_wstring& name) const
{
	size_t nShaders = m_Shaders.size() ;
	for(size_t  i =0 ; i < nShaders ;  i ++)
	{
		eShaderType _shaderType = m_Shaders[i].second;
		if(_shaderType == eShader_None)
		{
			name = m_Shaders[i].first.c_str();
			return true;
		}
	}
	return false;
}

void xGpuProgramName::set(const wchar_t* vs , const wchar_t* ps  , const wchar_t* gs )
{
	m_bLoadFromFile = false;
	m_Shaders.reserve(3);
	m_hash1 = 0;
	if(vs != NULL && wcslen(vs) > 0 ) 
	{
		m_hash1 +=  xStringHash(vs) ;
		m_Shaders.push_back( ShaderNameValue(vs , eShader_VertexShader) );
		m_bLoadFromFile = true;
	}
	if(ps != NULL && wcslen(ps) > 0) 
	{
		m_hash1 +=  xStringHash(ps) ;
		m_Shaders.push_back( ShaderNameValue(ps , eShader_PixelShader) );
		m_bLoadFromFile = true;

	}
	if(gs != NULL && wcslen(gs) > 0) 
	{
		m_hash1 += xStringHash(gs) ;
		m_Shaders.push_back( ShaderNameValue(gs , eShader_GeometryShader) );
		m_bLoadFromFile = true;
	}
}

void xGpuProgramName::set(const wchar_t* shaderNames[eShader_Max] )
{
	m_bLoadFromFile = false;
	m_Shaders.reserve(eShader_Max);
	m_hash1 = 0;
	for(int i  = 0 ; i < eShader_Max ; i ++)
	{
		eShaderType _type = eShaderType(i);
        if(shaderNames[i] != NULL && wcslen(shaderNames[i]) >0 )
		{
			m_hash1 +=  xStringHash(shaderNames[i]) ;
			m_Shaders.push_back( ShaderNameValue(shaderNames[i] , _type) );
			m_bLoadFromFile = true;
		}
	}
}

void xGpuProgramName::set(const wchar_t* _shaderName, bool bLoadFromeFile)
{
	m_Shaders.reserve(5);
	if(_shaderName != NULL && wcslen(_shaderName) > 0) 
	{
		m_hash1 = 0 +  xStringHash(_shaderName) ;
		m_Shaders.push_back( ShaderNameValue(_shaderName , eShader_None) );
	}
	m_bLoadFromFile = bLoadFromeFile;
}

xGpuProgramName::xGpuProgramName(const wchar_t* vs , const wchar_t* ps , const wchar_t* gs )
{
	set(vs,ps,gs);
}

xGpuProgramName::xGpuProgramName(const wchar_t* shaderNames[eShader_Max])
{
	set(shaderNames);
}

xGpuProgramName::xGpuProgramName(const wchar_t* _shaderName, bool bLoadFromeFile)
{
	set(_shaderName,bLoadFromeFile);
}


#ifdef _MSC_VER
size_t xGpuProgramName::hash_value() const
{
	size_t hv = 0;
	size_t nShaders = m_Shaders.size() ;
	for(size_t  i =0 ; i < nShaders ;  i ++)
	{
		const std::ds_wstring& shaderName = m_Shaders[i].first;
		hv += xStringHash(shaderName.c_str());
	}
	return hv;
}

#endif


//=============GpuProgramNameParser===================
class xGpuProgNameParserLexer : public xBaseLexer<wchar_t , ds_wstring>
{
public:
	int getFileName(const wchar_t * p, ds_wstring& text)
	{
		const wchar_t * pBase = p;
		text.clear();
		assert( p );
		while( p && *p &&	( *p != '(' && *p !=' ' && *p !='\t' && *p != 0x0a  && *p != 0x0d)  )
		{
			p += GetCStyleChar(p , text);
		}
		return (int)(p - pBase);
	}

	int   getNodeSocketName(	const wchar_t* p, ds_wstring& text)
	{
		const wchar_t* pBase = p;
		text.clear();
		assert( p );
		//if (    p && *p && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
		{
			while( p && *p &&	( IsAlphaNum( *p) || *p == '_' || *p == '.' ) )
			{
				text += *p;
				++p;
			}
		}
		return (int)(p - pBase);
	}
};

static xGpuProgNameParserLexer gs_Lexer;

//=================================
xShaderSocketInfo* xShaderName::findSocket(const wchar_t* socketName)
{
	size_t nSockets = m_Sokects.size() ;
	for(size_t  i =0 ; i < nSockets ;  i ++)
	{
		if(m_Sokects[i].m_name == socketName)
			return &m_Sokects[i];
	}
	return NULL;
}

xShaderSocketInfo* xShaderName::findSocket(const size_t   socketIdx)
{
	size_t nSockets = m_Sokects.size() ;
	for(size_t  i =0 ; i < nSockets ;  i ++)
	{
		if(m_Sokects[i].m_idx == socketIdx)
			return &m_Sokects[i];
	}
	return NULL;
}
bool xShaderName::removeNode(const wchar_t* nodeName)
{
    size_t nSockets = m_Sokects.size() ;
    for(size_t  i =0 ; i < nSockets ;  i ++)
    {
        xShaderSocketInfo& socketInfo = m_Sokects[i];
        std::ds_wstring_vector::iterator pos = socketInfo.m_attachNodes.begin();
        for(; pos != socketInfo.m_attachNodes.end() ; pos ++)
        {
            if(*pos == nodeName)
            {
                socketInfo.m_attachNodes.erase( pos );
                return true;
            }
        }
    }
    return false;
}
void xShaderName::toName(ds_wstring& _strName)
{
	_strName = m_strMainNodeName;
	if(m_Sokects.size() == 0)
		return ;
    
    ds_wstring _postFix = L"(";
	size_t nSockets = m_Sokects.size() ;
	for(size_t  i =0 ; i < nSockets ;  i ++)
	{
		xShaderSocketInfo& Socket = m_Sokects[i];
		if(Socket.m_attachNodes.size() == 0)
			continue;
		if(Socket.m_idx == -1)
			_postFix += Socket.m_name;
		else
		{
			std::wostringstream oss;
			oss<<Socket.m_idx;
			_postFix += oss.str().c_str();
		}
		_postFix += ':';
		size_t nAttachedNodes = Socket.m_attachNodes.size();
		for(size_t j = 0 ; j <  nAttachedNodes; j ++)
		{
			_postFix += Socket.m_attachNodes[j];
			if(j != Socket.m_attachNodes.size() - 1)
				_postFix += ',';
		}
		_postFix += ';';
	}
	_postFix += ')';

    if(_postFix !=L"()")
    {
        _strName += _postFix;
    }
	return ;
}

xShaderSocketInfo* xShaderName::addSocket(int idx , const wchar_t* socketName)
{
    xShaderSocketInfo info;
    info.m_name = idx >= 0 ? L"" : socketName;
    info.m_idx = idx;
    m_Sokects.push_back(info);
    xShaderSocketInfo* pSocketInfo = & m_Sokects[ m_Sokects.size() - 1 ];
    return pSocketInfo;
}

void xShaderName::addSocket(const xShaderSocketInfo& Socket)
{
    m_Sokects.push_back(Socket);
}

const wchar_t* xShaderName::mainNodeName() 
{ 
    return m_strMainNodeName.c_str() ; 
}

bool xShaderName::isNull()
{ 
    return m_strMainNodeName.length() == 0 ; 
}

xShaderName::xShaderName()
{
    m_strMainNodeName = L"";
}

static int StringToIdx(const wchar_t* idx)
{
    for(size_t i = 0 ; i < wcslen(idx) ; i ++)
    {
        if( (idx[i]<'0')||(idx[i]>'9') )
            return -1;
    }
    return _wtoi(idx);
}


bool xShaderName::parse(const wchar_t* _shaderName)
{
    m_Sokects.clear();
    ds_wstring _shaderFile;
    const wchar_t* _code = _shaderName;

    //����Shader�����ļ���
    _code += gs_Lexer.getFileName(_code , _shaderFile);
    if(_shaderFile.length() == 0 )
        return false;

    //�������ڵ������
    m_strMainNodeName = _shaderFile;
    //��ʼ��������������
    _code += gs_Lexer.skipWhiteSpace(_code);
    if(*_code != '(' )//û�м�Node
        return true;

    _code += 1;

    while(*_code != 0 && *_code !=')')
    {
        //��ʼ��ȡsocket����
        _code += gs_Lexer.skipWhiteSpace(_code);
        ds_wstring socketName;
        _code += gs_Lexer.getNodeSocketName(_code  , socketName);
        if(socketName.length() == 0 )
            return true;

        //Socket���ֺ���Ӧ�ý�:�ַ�
        _code += gs_Lexer.skipWhiteSpace(_code);
        if(*_code != ':' )//û�м�Node
        {
            XEVOL_LOG(eXL_DEBUG_NORMAL,"Shader��׼Name�е�socket=%s��δ��:����\n",socketName.c_str() );
            return true;
        }
        _code ++ ;
        _code += gs_Lexer.skipWhiteSpace(_code);

        //����ShaderSokect;

        int idx = StringToIdx(socketName.c_str() );
        xShaderSocketInfo* pSocketInfo = NULL;
        if(idx == -1)
        {
            pSocketInfo = findSocket(socketName.c_str() );
        }
        else
        {
            pSocketInfo = findSocket( idx );
        }
        if(pSocketInfo == NULL)
        {
            pSocketInfo = addSocket(idx , socketName.c_str() );
        }

        //Socket��Ϣ��ȡ���ˡ����뵽Socket�б����ˡ�
        //��������ȡ��һ��Node����
        while(*_code != 0 && *_code != ';' &&  *_code != ')')
        {
            ds_wstring _nodeName;
            _code += gs_Lexer.getNodeSocketName(_code  , _nodeName);
            if(socketName.length() == 0 )
                continue;
            _code += gs_Lexer.skipWhiteSpace(_code );
            pSocketInfo->m_attachNodes.push_back(_nodeName.c_str() );


            _code += gs_Lexer.skipWhiteSpace(_code);

            if(*_code == ',' )
            {
                _code ++ ;
                _code += gs_Lexer.skipWhiteSpace(_code);
                continue;
            }
            else if(*_code == ')')
            {
                return true;
            }
            else if(*_code == ';')
            {
                _code ++ ;
                _code += gs_Lexer.skipWhiteSpace(_code);
                break;
            }
            else
            {
                XEVOL_LOG(eXL_DEBUG_NORMAL,"Shader��׼Name�е�Node=%s����ַ�λ��\n",_nodeName.c_str() );
                return true;
            }
        }

    }
    return true;

}
//===================


xGpuProgNameParser::xGpuProgNameParser(xGpuProgramName& _name)
{
    for(size_t i = 0 ; i < eShader_Max ; i ++)
    {
        m_ShaderNames[i].setMainNode( L"" ); 
    }

	parse(_name);
}

xGpuProgNameParser::xGpuProgNameParser()
{
    for(size_t i = 0 ; i < eShader_Max ; i ++)
    {
        m_ShaderNames[i].setMainNode(L""); 
    }
}

bool xGpuProgNameParser::parse(const xGpuProgramName& _name)
{
	size_t nShaders = _name.m_Shaders.size();
	for(size_t i = 0 ; i <  nShaders; i ++)
	{
		eShaderType _shaderType = _name.m_Shaders[i].second;
		if(_shaderType != eShader_None)
		{
			setShaderName(_shaderType , _name.m_Shaders[i].first.c_str() );
		}
	}
	return true;
}

bool xGpuProgNameParser::parse(const char* _shaderNameString)
{
	return false;
}

void xGpuProgNameParser::toName(xGpuProgramName& _name)
{
	_name.clear();
	_name.m_bLoadFromFile = false;
	ds_wstring _strShaderName[eShader_Max]  ;
    const wchar_t* shaderName[eShader_Max] = {NULL};
    for(int i = 0 ; i  < eShader_Max  ; i ++)
    {
        shaderName[i] = NULL;
        if( !m_ShaderNames[i].isNull() )
        {
           m_ShaderNames[i].toName(_strShaderName[i]);
           shaderName[i] = _strShaderName[i].c_str();
        }
    }
    _name.set(shaderName);
	return ;
}

bool xGpuProgNameParser::setShaderName(eShaderType _shaderType , const wchar_t* _shaderName)
{
	xShaderName* pShaderName = _getShader(_shaderType);
    if(pShaderName == NULL) return false;
   return pShaderName->parse(_shaderName);
}

void xGpuProgNameParser::addShaderNode(eShaderType _shaderType , const wchar_t* _nodeName, const wchar_t* _socketName )
{
	xShaderName* pShaderName = _getShader(_shaderType);
	int idx = StringToIdx( _socketName );
	xShaderSocketInfo* pSocketInfo = NULL;

	if(idx == -1)
	{
		pSocketInfo = pShaderName->findSocket( _socketName );
	}
	else
	{
		pSocketInfo = pShaderName->findSocket( idx );
	}
	if(pSocketInfo == NULL)
	{
		pSocketInfo = pShaderName->addSocket( idx , _socketName);
    }
	pSocketInfo->m_attachNodes.push_back( _nodeName );
	return ;
}

xShaderName* xGpuProgNameParser::_getShader(eShaderType _shaderType)
{
	return &m_ShaderNames[_shaderType];
}

void xGpuProgNameParser::addShaderNode(eShaderType _shaderType , const wchar_t* _nodeName , size_t _socketIdx )
{
	xShaderName* pShaderName = _getShader(_shaderType);
	int idx = (int)_socketIdx;
	xShaderSocketInfo* pSocketInfo = pShaderName->findSocket( idx );
	if(pSocketInfo == NULL)
	{
		pSocketInfo = pShaderName->addSocket( idx , L"");
	}
	pSocketInfo->m_attachNodes.push_back( _nodeName );
	return ;
}

void InsertNodes(vHShaderCodeNodes& _nodes , HShaderCodeNode hCode)
{
	size_t nNodes =  _nodes.size() ;
	for(size_t i = 0 ; i < nNodes ; i ++)
	{
		if(_nodes[i] == hCode)
			return ;
	}
	_nodes.push_back(hCode);
}

void printNodeName( HShaderCodeNode hCodeNode , ds_string& _code)
{
	char node_name[1024] = {0};
	XEvol_UnicodeToLocale(hCodeNode->name() , node_name , 1024 );
	_code += node_name ; 
}

bool xGpuProgNameParser::generateCode(eShaderType _shaderType  , xShaderCodeNodeMgr* pNodeMgr , ds_string& _code)
{
	xShaderName* pShaderName = _getShader(_shaderType);

	//���Main Code û���ҵ����򷵻ش���
	HShaderCodeNode hMainCode = pNodeMgr->add(pShaderName->mainNodeName() , 0 , true);
	if(hMainCode.getResource() == NULL)
		return false;


	//��Node�������
	std::set<ds_wstring> vLibImportSet;	
	ds_wstring_vector&  MainImpLibs  = hMainCode->importLibs() ;
	size_t nMainImpLibs = MainImpLibs.size() ;
	for(size_t j = 0 ; j < nMainImpLibs; j ++)
	{
		vLibImportSet.insert( MainImpLibs[j] );
	}

	//���û�и����κ�Node���������Node��������Ĭ�ϵ� code��
	if(pShaderName->nSokects() == 0)
	{
		AddImportLib(_code, vLibImportSet, pNodeMgr);
		hMainCode->generateCode(_code);
		return true;
	}
	//��������Node������Ϊ�Ǽ�Node�������Ĭ�ϵ�code������������
	if(hMainCode->isSkeletonNode() == false)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"Shader Node ������Ϊ���ڵ�ʹ��node name=");
		XEVOL_LOG(eXL_DEBUG_HIGH,pShaderName->mainNodeName() );
		XEVOL_LOG(eXL_DEBUG_HIGH,"\n");
		AddImportLib(_code, vLibImportSet, pNodeMgr);
		hMainCode->generateCode(_code);
		return true;
	}

	//������Ϊÿ��Socket�ҵ���Ӧ��xShaderNodeSocket,ÿһ��xShaderNodeSocket*��Ӧ�ô�����hMainCode��
	//������Щ�����ڵ�Socket������Ҫȥ������������ʾ

	//�Ӵ����ShaderName����һ���µĹ淶��ShaderName
	xShaderName newShaderName ;
	newShaderName.setMainNode ( pShaderName->mainNodeName() ) ;
	size_t _nSockets = pShaderName->nSokects() ; // m_Sokects.size() ;
	for(size_t i = 0 ; i < _nSockets;  i ++)
	{
        xShaderSocketInfo* pSocketInfo = pShaderName->findSocket(i);

		ds_wstring& socketName = pSocketInfo->m_name ; // pShaderName->m_Sokects[i].m_name;
		int         socketIdx  = pSocketInfo->m_idx  ; // pShaderName->m_Sokects[i].m_idx;
		xShaderNodeSocket* pSocket = hMainCode->findSocket(socketIdx , socketName.c_str() );
		if(pSocket == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH,"socket name �����ڵ���ʹ�ò����� socket name=");
			XEVOL_LOG(eXL_DEBUG_HIGH,socketName.c_str() );
			XEVOL_LOG(eXL_DEBUG_HIGH,"\n");
		}
		else
		{
			newShaderName.addSocket( *pSocketInfo ) ;//.push_back( pShaderName->m_Sokects[i]) ;
		}

	}

	//���������Code������Ҫ���ҳ����е�Node, ���ң����Nodeһ��Ҫ��Slot���͵�
	vHShaderCodeNodes _nodes;
    _nSockets = newShaderName.nSokects();
	for(size_t i = 0 ; i < _nSockets;  i ++)
	{
        xShaderSocketInfo* pSocketInfo = newShaderName.findSocket(i);
		size_t nAttachNodes = pSocketInfo->m_attachNodes.size();
		for(size_t j = 0 ; j < nAttachNodes ; j ++ )
		{
			ds_wstring& nodeName = pSocketInfo->m_attachNodes[j];
			HShaderCodeNode hCodeNode = pNodeMgr->add(nodeName.c_str() , 0 , true);
			if(hCodeNode.getResource() == NULL)
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"Shader Node not found name=");
				XEVOL_LOG(eXL_DEBUG_HIGH,nodeName.c_str());
				XEVOL_LOG(eXL_DEBUG_HIGH,"\n");
			}
			else if(!hCodeNode->isSlotNode() )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"Shader Node is not a slot node name=");
				XEVOL_LOG(eXL_DEBUG_HIGH,nodeName.c_str());
				XEVOL_LOG(eXL_DEBUG_HIGH,"\n");	
			}
			else
			{
				InsertNodes(_nodes , hCodeNode);
			}
		}
	}

	//���е�Node���Ѿ����أ����ҿ���ʹ�õ�SocketҲ���Ѿ�׼�����.

	//========================================================
	//�������������
	//�����ڵ�������
	size_t nNodes = _nodes.size() ;
	for(size_t i = 0 ; i < nNodes; i ++)
	{
		HShaderCodeNode     hCodeNode   = _nodes[i];
		ds_wstring_vector&  importLibs  = hCodeNode->importLibs() ;
		for(size_t j = 0 ; j < importLibs.size() ; j ++)
		{
			vLibImportSet.insert( importLibs[j] );
		}
	}

	AddImportLib(_code, vLibImportSet, pNodeMgr);

	//���е�slot Node Ӧ�ö�Ҫ�Ž���


	//������������Ĭ��shader code ��declaration.
	_generateDecl(hMainCode , _code);
	nNodes = _nodes.size() ;
	for(size_t i = 0 ; i < nNodes; i ++)
	{
		HShaderCodeNode hCodeNode = _nodes[i];
		_generateDecl(hCodeNode , _code);
	}
	_code += "//==================================================\n\n";




	_code += "//============================================\n";
	nNodes = _nodes.size() ;
	for(size_t i = 0 ; i < nNodes; i ++)
	{
		HShaderCodeNode hCodeNode = _nodes[i];
		_generateSlotCode(hCodeNode , _code);
	}
	_code += "//============================================\n";


	_code += "\n\n";
	_code += "//============================================\n";
	//���ڸ��Ǳ�������hMainCode��section�����ж��ǲ���Sokect.
	_code += "//Begin Main Node'code \n";
	xShaderCodeSections& sections = hMainCode->sections();
	int idxSocket = 0 ; //��ǰ��Socket idx
	size_t nSection = sections.size() ;
	for(size_t i = 0 ; i < nSection; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		//�����Source Code����ֱ����ӽ���
		if(pSection->typeName() == std::ds_wstring(CODE_SECTION) )
		{
			_code += pSection->code();
		}
		else if(pSection->typeName() == std::ds_wstring(DEFAULT_SECTION) )
		{
			//�����Ĭ�ϴ��룬��Ҫ�ж����sectionָ����sockt����û�а�Node
			const wchar_t* socketName = pSection->name();
			const int      socketIdx  = hMainCode->findSocketIdx( socketName );
			xShaderSocketInfo* pSocketInfo = newShaderName.findSocket(socketName);
			if(pSocketInfo == NULL) pSocketInfo = newShaderName.findSocket( socketIdx );
			//���ڵ�Socket��û�а��κ�Node
			if(pSocketInfo == NULL || pSocketInfo->m_attachNodes.size() == 0)
			{
				_code += pSection->code();
			}
		}
		else if( pSection->typeName() == std::ds_wstring(SOCKET_SECTION) )
		{
			//�����Ĭ�ϴ��룬��Ҫ�ж����sectionָ����sockt����û�а�Node
			const wchar_t* socketName = pSection->name();
			const int      socketIdx  = hMainCode->findSocketIdx( socketName );
			xShaderSocketInfo* pSocketInfo = newShaderName.findSocket(socketName);
			if(pSocketInfo == NULL) pSocketInfo = newShaderName.findSocket( socketIdx );
			//���ڵ�Socket�ϰ���Node
			if(pSocketInfo && pSocketInfo->m_attachNodes.size() != 0)
			{
				xShaderNodeSocket* pSocket = hMainCode->findSocket( socketName );
				_generateSocketCode(pNodeMgr, pSocket,pSocketInfo , _code);
			}
		}
		//���ˣ������������
	}
	return false;
}

bool xGpuProgNameParser::_generateDecl(HShaderCodeNode hCodeNode , ds_string& _code)
{
	xShaderCodeSections& sections = hCodeNode->sections();

	int nDecl = 0;
	size_t _nSection = sections.size();
	for(size_t i = 0  ; i < _nSection ; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		if(pSection->typeName() == std::ds_wstring(DECL_SECTION) )
			nDecl ++ ;
	}
	if(nDecl == 0)
		return true;

	_code += "\n//Insert Node Declaration Node= ";
	printNodeName(hCodeNode,_code);
	_code += "\n";
	_nSection = sections.size();
	for(size_t i = 0  ; i < _nSection ; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		if(pSection->typeName() == std::ds_wstring(DECL_SECTION) )
		{
			_code += pSection->code();
		}
	}

	return true;
}

bool xGpuProgNameParser::_generateSlotCode(HShaderCodeNode hCodeNode , ds_string& _code)
{
	_code += "\n//Insert Node Code Node= ";
	printNodeName(hCodeNode,_code);
	_code += "\n";
	xShaderCodeSections& sections = hCodeNode->sections();
	size_t nSection = sections.size();
	for(size_t i = 0  ; i < nSection ; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		if(pSection->typeName() == std::ds_wstring(CODE_SECTION ) )
		{
			_code += pSection->code();
		}
	}
	return true;
}

struct xRetVal
{
	ds_string               m_RetVal;
	std::ds_string          m_TypeName;//��������
	xShaderVarInfo          m_info;
	size_t                  m_nElement;
};

class xRetValManager
{
	typedef std::map<ds_string , xRetVal> vRetVals;
	vRetVals m_vRetVals;
public:
	bool find(const char* _semantic , ds_string& strRetVal , xShaderVariable& arg)
	{
		vRetVals::iterator pos  = m_vRetVals.find(_semantic);
		if(pos == m_vRetVals.end() )
			return false;

		xRetVal retVal = pos->second;
		if(retVal.m_TypeName == arg.m_TypeName )
		{
			strRetVal = retVal.m_RetVal;
			return true;
		}
		else if(retVal.m_info.nComponent() > arg.m_info.nComponent() )
		{
			//����ת����Ŀǰֻ֧��floatn֮�������ת��
			switch(arg.m_info.nComponent())
			{
			case 1:
				strRetVal =  arg.m_TypeName + "(" + retVal.m_RetVal + ".x)"; 
				return true;
			case 2:
				strRetVal =  arg.m_TypeName + "(" + retVal.m_RetVal + ".xy)"; 
				return true;
			case 3:
				strRetVal =  arg.m_TypeName + "(" + retVal.m_RetVal + ".xyz)"; 
				return true;
			}

		}

		XEVOL_LOG(eXL_DEBUG_HIGH,"���������ƥ�� �����������Ϊ%s,ʵ�ʲ�������Ϊ%s\n",arg.m_TypeName.c_str() , retVal.m_TypeName.c_str());

		return false;
	}

	bool insert(const char* _semantic , const char* strRetVal , xShaderVariable& arg)
	{
		xRetVal retVal ;
		retVal.m_TypeName   =arg.m_TypeName   ;//��������
		retVal.m_info       =arg.m_info       ;//��float4����matrix4x4?
		retVal.m_RetVal     =strRetVal;
		retVal.m_nElement   =arg.m_nElement;
		m_vRetVals[_semantic] = retVal;
		return true;
	}
};


bool xGpuProgNameParser::_generateSocketCode(xShaderCodeNodeMgr* pNodeMgr ,xShaderNodeSocket* pSocket , xShaderSocketInfo* info , ds_string& _code)
{
    std::ds_string _socketName = pSocket->ansiName();
	xRetValManager retValMgr;
	size_t nInputPinArgs = pSocket->m_InputPin.m_Args.size() ;
	for(size_t i = 0 ; i < nInputPinArgs; i ++)
	{
		xShaderVariable& arg = pSocket->m_InputPin.m_Args[i];
		ds_string retArgName = arg.m_Name;
		if(pSocket->m_InputPin.m_structName.length() != 0 )
		{
			retArgName = pSocket->m_InputPin.m_structName + "." + arg.m_Name;
		}
		retValMgr.insert(arg.m_Semantic.c_str() , retArgName.c_str() , arg);
	}

	//��ÿһ��Node,
	//1. ���ȹ���������� ---(1). �ṹ������ (2). �ǽṹ������
	//2. ���ɷ���ֵ�����֣������ǽṹ�壬Ҳ�����ǻ������ݽṹ�����ֻ�ǻ����������ͣ��򷵻�ֵӦ��ֻ��һ����
	//3. ���뺯�����ã������ĵ��õĲ������ɵ�1�����ɣ�����ֵ�ɵڶ������ɡ�
	//4. ��������ֵ���������ɵķ���ֵѹ�뵽����ֵ�б��
	xShaderPin* pLastOutPin = NULL;
	ds_string   strLastVal = "";
	ds_string   strLastValType = "";
	size_t nAttachNodes = info->m_attachNodes.size() ;
	for(  size_t i = 0 ; i <  nAttachNodes; i ++ )
	{
		char preInfo [256];
		char nodeName[64]={0};
		XEvol_UnicodeToLocale(info->m_attachNodes[i].c_str() , nodeName , 64);
		sprintf(preInfo,"\n   //Ӧ���޸��� name=%s \n" , nodeName );
		_code += preInfo;

		HShaderCodeNode hNode = pNodeMgr->add(info->m_attachNodes[i].c_str() , 0 , true);
		
		if(hNode.getResource() == NULL)
			continue;

		xShaderNodeSlot* pSlot = hNode->getSlotInfo();
		xShaderPin& InputPin = pSlot->m_InputPin;
		xShaderPin& OutPin   = pSlot->m_OutPin;
		pLastOutPin = &OutPin;
		//�������������
		ds_string inputArgList = "";
		nInputPinArgs = InputPin.m_Args.size() ;
		for(size_t iArg = 0 ; iArg < nInputPinArgs ; iArg ++)
		{
			xShaderVariable& arg = InputPin.m_Args[iArg];
			ds_string retVal = "";
			//�ҵ���Ӧ�Ĳ���
			if(false == retValMgr.find(arg.m_Semantic.c_str() , retVal , arg) )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"���������ƥ�� semantic =: %s \n" , arg.m_Semantic.c_str());
				return false;
			}
			inputArgList += retVal;
			if(iArg != InputPin.m_Args.size() - 1) inputArgList += " , ";
		}
		//��������������

		//���췵��ֵ
		ds_string outRetClass = "";
		if( OutPin.m_structName.length() != 0 )
		{

			outRetClass = OutPin.m_structName ;
		}
		else
		{
			outRetClass = OutPin.m_Args[0].m_TypeName ;
		}

		char nodeRetArgName[125];

		sprintf(nodeRetArgName , "Ret_%s_Node%d", _socketName.c_str()  ,  i);
		strLastVal = nodeRetArgName;
		strLastValType = outRetClass;
		outRetClass += " ";
		outRetClass += nodeRetArgName ;
		outRetClass += " ";

		//���ɺ�������
		ds_string funcall = "   " + outRetClass + "= " + pSlot->m_funcName + "(" + inputArgList + ");\n";
		_code += funcall ;

		//�����������. �ѽ��ѹ�뵽����ֵ�б���
		size_t nOutPinArgs =  OutPin.m_Args.size() ;
		for(size_t iArg = 0 ; iArg < nOutPinArgs; iArg ++)
		{
			xShaderVariable& arg = OutPin.m_Args[iArg];
			ds_string outRetArg = "";
			if( OutPin.m_structName.length() != 0 )
			{
				outRetArg = ds_string(nodeRetArgName) + "." + arg.m_Name;
			}
			else
			{
				outRetArg = ds_string(nodeRetArgName);
			}

			retValMgr.insert(arg.m_Semantic.c_str() , outRetArg.c_str() , arg);
			continue;
		}		
	}

	//���еĺ������ö��Ѿ�������ϣ����ڸ���Socket��OutputPin��ʼ����Socket�Ľ��
	xShaderPin& socketOutpin = pSocket->m_OutPin;
	ds_string   retVarPre = "";
	ds_string   retDecl = "";
	if(socketOutpin.m_varName.length() != 0)
	{
		retVarPre = socketOutpin.m_varName + "." ;
	}
	else if(socketOutpin.m_structName.length() != 0)
	{
		retDecl += "\n";;
		retDecl += "//��������ֵ;\n";
		retDecl += "   " + socketOutpin.m_structName + "  " + pSocket->ansiName()  + "_RetFinal;\n";
		retVarPre = pSocket->ansiName() + "_RetFinal." ;
	}

	_code += "\n";
	if(socketOutpin.m_Args.size() > 0 )//�����������б��С��͸�����������б���
	{
		_code += retDecl;
		size_t nSocketOutPinArgs = socketOutpin.m_Args.size() ;
		for(size_t i = 0 ; i < nSocketOutPinArgs ; i ++ )
		{
			xShaderVariable& arg = socketOutpin.m_Args[i];
			ds_string assSent =  "   " + retVarPre + arg.m_Name + " = ";
			ds_string retVal = "";
			//�ҵ���Ӧ�Ĳ���
			if(false == retValMgr.find(arg.m_Semantic.c_str() , retVal , arg) )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"���������ƥ�� semantic =: %s \n" , arg.m_Semantic.c_str());
				return false;
			}
			assSent = assSent + retVal + "; \n";

            if( std::ds_string(retVarPre) + arg.m_Name != retVal )
			   _code += assSent;
            else
            {
                continue;
            }
		}
	}
	else if(socketOutpin.m_structName.length() > 0 ) 
	{
		//������Pin������Ϊ�գ���ô��ʾ��Ҫ�Լ�����Shader�ķ���ֵ��
		//���ض��ٲ������������Լ���������һ��Node������
		//OpenGL����򵥣�ֻҪ���һ��Node������varying�Ϳ����ˡ�����Ҫ�õ��˹��ܡ�
		//����һ������ֵ
		ds_string def_ = "#define " + socketOutpin.m_structName + " " + strLastValType + " \n";
		_code = def_ + _code;
		_code += "\n";
		_code += "   return " + strLastVal  + "; \n";
		_code += "}\n";
	}

	return true;
}

END_NAMESPACE_XEVOL3D
