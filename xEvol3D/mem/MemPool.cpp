/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) .
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "../xStdPch.h"
#include "MemPool.h"
#include <cassert>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <iostream>
using namespace std;
/******
�ڴ�ط�������
Ҳ����һ���̶���С���ڴ������.
�����㷨Ϊ�Դ����֣����Loki�����������������㷨����ΪLoki�е�FixedAlloctor

******/

BEGIN_XEVOL3D_MEM

/*
m_pData--->        [X X X X X ...]
��һ�����ÿ�	-->  [3 | _____ ...]
[X X X X X ...]
[  | _____ ...]
[X X X X X ...]
*/
bool  MemPool::MemChunck::init(size_t blockSize, unsigned short blocks)
{
	assert(blockSize > 0);
	assert(blocks > 0);
	// Overflow check
	assert((blockSize * blocks) / blockSize == blocks);

	m_pData = (unsigned char*) malloc(blockSize * blocks);
	if(m_pData == NULL)
		return false;
	reset(blockSize, blocks);
	return true;
}

void* MemPool::MemChunck::alloc(size_t blockSize)
{
	if (!m_blocksAvailable) return 0;

	//����ڴ��ǲ�����Ȼ�����
	assert((m_firstAvailableBlock * blockSize) / blockSize == m_firstAvailableBlock);

	//�ѵ�һ�����÷��ظ�ӵ��
	unsigned char* pResult = m_pData + (m_firstAvailableBlock * blockSize);
	m_firstAvailableBlock = *((unsigned short*)pResult);
	-- m_blocksAvailable;

	return pResult;
}

void  MemPool::MemChunck::dealloc(void* p, size_t blockSize)
{
	assert(p >= m_pData);

	unsigned char* toRelease = static_cast<unsigned char*>(p);
	//�ڴ������
	assert((toRelease - m_pData) % blockSize == 0);

	//���ͷŵ��Ŀ���뵽��ͷ��½�һ����ͷ,��ͷ��һ����ָ��ԭ���ĵ�һ�����ÿ�
	* ((unsigned short*)toRelease) = m_firstAvailableBlock;
	//��һ�����ÿ�ָ���ͷ
	m_firstAvailableBlock = static_cast<unsigned short>( (toRelease - m_pData) / blockSize   )  ;
	//�������
	assert(m_firstAvailableBlock == (toRelease - m_pData) / blockSize);

	++ m_blocksAvailable;
}

void  MemPool::MemChunck::reset(size_t blockSize, unsigned short blocks)
{
	assert(blockSize > 0);
	assert(blocks > 0);
	//������
	assert((blockSize * blocks) / blockSize == blocks);

	m_firstAvailableBlock = 0;
	m_blocksAvailable = blocks;

	//����ڴ�����
	unsigned short i = 0;
	unsigned char* p = m_pData;
	for (; i != blocks; p += blockSize)
	{
		unsigned short * pNext =(unsigned short*) p;
		*pNext = ++i;
	}

}

void  MemPool::MemChunck::Release()
{
	free((void*)m_pData);
}

bool  MemPool::MemChunck::is_ptr(void* p,size_t blockSize, unsigned short blocks)
{
	if( p < m_pData) return false;
	//�ڴ治��������档Ҳ����������ġ�
	if( p > m_pData + blockSize * blocks) return false;

	//ָ��û��blockSize�߽��϶��룮�϶����������MemChunck�����
	if( ((unsigned char*)p - m_pData)%blockSize != 0) return false;
	return true;
}


//======================================================================================
//������ΪMemPool���࣮
//  ������ʵ�֣�
//======================================================================================

MemPool::MemPool(size_t block_size, size_t block_reserved,size_t chunck_size)
{
	init(block_size,block_reserved,chunck_size);
}

bool MemPool::init(size_t block_size, size_t block_reserved,size_t chunck_size)
{
	if(block_size < sizeof(unsigned short) )
		block_size = sizeof(unsigned short) ;
	m_blockSize = block_size;
	assert(chunck_size > block_size);
	m_blocksPerChunk  = chunck_size / block_size;

	m_avaliableBlocks = 0;
	size_t nChuncks = (block_reserved + 1 ) / m_blocksPerChunk;

	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		MemChunck chunck;
		if( chunck.init(m_blockSize,(unsigned short)m_blocksPerChunk) == false)
			return false;
		m_Chuncks.push_back(chunck);
		m_avaliableBlocks += m_blocksPerChunk;
	}

	m_lastChunk = 0;

	return true;
}

void MemPool::clear()
{
	size_t nChuncks = m_Chuncks.size();
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		m_Chuncks[i].Release();
	}
	m_avaliableBlocks = 0;
}

void MemPool::clear_unused()
{
	MemChunks::iterator i = m_Chuncks.end();
	size_t  n = m_Chuncks.size();
	while( (n--)!=0)
	{
		--i;
		if(i->m_blocksAvailable == m_blocksPerChunk)
		{
			i->Release();
			m_Chuncks.erase(i);
		}
	}
	m_lastChunk = 0;

}

//----------------------------------------------------
//Ϊ���MemPool�������ٸ��ֽڵĿռ�
//----------------------------------------------------
void      MemPool::reserve(size_t block_reserved)
{
	if(m_avaliableBlocks >= block_reserved)
		return ;

	size_t nChuncks = (block_reserved - m_avaliableBlocks + 1) / m_blocksPerChunk;
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		MemChunck chunck;
		if( chunck.init(m_blockSize,(unsigned short)m_blocksPerChunk) == false)
			return ;
		m_Chuncks.push_back(chunck);
		m_avaliableBlocks += m_blocksPerChunk;
	}
}

//----------------------------------------------------
//�ͷ�û���ù��Ŀռ䣮
//----------------------------------------------------
size_t MemPool::unused_block()
{
	return m_avaliableBlocks;
}

//----------------------------------------------------
//�鿴���ж��ٿռ������
//----------------------------------------------------
size_t MemPool::capacity()
{
	return m_Chuncks.size() * m_blocksPerChunk * m_blockSize;
}

//-------------------------------------------- --------
//����һ���ڴ���ͷ�һ���ڴ�
//----------------------------------------------------
void*     MemPool::alloc()
{
	//�������һ�η����MemChunck�����䣮
	//������һ���黹�У��ͷ��أ�
	void * ret =  m_Chuncks[m_lastChunk].alloc(m_blockSize);
	if(ret) {m_avaliableBlocks --; return ret;}



	size_t nChuncks = m_Chuncks.size();

	//��һ���пտ��MemChunck
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		if(m_Chuncks[i].m_blocksAvailable > 0)
		{
			//�����һ���������α�ָ���п��еĿ飮
			//���ÿ�ļ������٣�������һ�����ȥ
			m_lastChunk = (int)i;
			m_avaliableBlocks --;
			return m_Chuncks[m_lastChunk].alloc(m_blockSize);
		}
	}
	//����һ���µ�Block���ŵ����森
	m_Chuncks.push_back( MemChunck() );
	m_lastChunk = (int)nChuncks;
	//������ʼ��ʧ�ܣ����ʾû���ڴ��ˡ����ؿգ���ʾʧ��
	if( false == m_Chuncks[nChuncks].init(m_blockSize,(unsigned short)m_blocksPerChunk))
		return NULL;

	//�½�����һ���飮�ŵ����
	m_avaliableBlocks += (m_blocksPerChunk - 1);
	return m_Chuncks[m_lastChunk].alloc(m_blockSize);

}

void      MemPool::dealloc(void* p)
{
	if(m_Chuncks[m_lastChunk].is_ptr(p,m_blockSize,(unsigned short)m_blocksPerChunk) == true)
	{
		m_Chuncks[m_lastChunk].dealloc(p,m_blockSize);
		m_avaliableBlocks ++;
		return ;
	}
	size_t nChuncks = m_Chuncks.size();

	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		if(m_Chuncks[i].is_ptr(p,m_blockSize,(unsigned short)m_blocksPerChunk) == true)
		{
			//��ǰ�ͷ���һ���ڴ森��ô���Chunck�϶������ڴ�����õġ�
			m_lastChunk =(int) i;
			m_Chuncks[m_lastChunk].dealloc(p,m_blockSize);
			m_avaliableBlocks ++;
			return ;
		}
	}  
}

//----------------------------------------------------
//�ж�һ��ָ���ǲ��������Pool����ġ�
//----------------------------------------------------
bool      MemPool::is_ptr(void* p)
{
	size_t nChuncks = m_Chuncks.size();
	for(size_t i = 0 ; i < nChuncks ; i ++ )
	{
		if(m_Chuncks[i].is_ptr(p,m_blockSize,(unsigned short)m_blocksPerChunk) == true)
		{
			return true;
		}
	} 
	return false;
}
void  MemPool::diagnostic()
{
	size_t  nChunck = m_Chuncks.size();
	size_t totalPoolSize = capacity();
	size_t freeSize = 0;
	for(size_t i = 0 ; i < nChunck ; i ++)
	{
		freeSize += m_Chuncks[i].m_blocksAvailable * m_blockSize;
	}

	wchar_t buf[512];
	swprintf(buf,L"Mempool:Capacity=%d(int8) unused=%d(int8) blockSize=%d nChunk = %d\n ",totalPoolSize,freeSize , m_blockSize, nChunck  );
#ifdef _WIN32
		OutputDebugStringW(buf);
#else
    XEVOL_LOG(eXL_DEBUG_NORMAL,buf);
#endif
}
END_XEVOL3D_MEM
