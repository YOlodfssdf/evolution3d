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

#ifndef   __MEM_POOL_H__
#define   __MEM_POOL_H__

/***************************************************************************************************************
Fixed Alloctor:
DGE project 

�ڴ�ط����������е�С�ڴ����Ļ���.
����һ��û�пռ��˷ѵġ�Fixed Allocator����MemPool��ָ��һ��blockSize��
���MemPool��ֻ�ܷ���blockSize��ô����ڴ�.�������ͨ��is_ptr�ж�һ��ָ���ǲ����ɸ������ġ�

�ܹ���MemPool�ϵ��ڴ��������
SmallAlloc : С�ڴ������������Ϊ���ֲ��ԣ�
SmallAllocFast: �������ٷ��䣮���˷ѣ���Coockie��¼��SmallMem�Ĵ�С��������֪�������
SmallAllocNoCookie�����˷ѵķ�����ԡ���ȫ����MemPool��is_ptr���ж��ڴ������ĸ�����������ģ�
ObjectPool<T>: ����أ�һ��ģ��


�����㷨��Loki��FixedAllocator�ǳ��ӽ�.

����:     MemPool
���ֿռ�: DGE

2004-10-19. ����������
***************************************************************************************************************/

#include <vector>
#include "stl_alloc.h"
#include "memdef.h"

namespace XEvol3D
{ 
	namespace Mem
	{

		//MemChunck��Ĭ�ϵĴ�С����ȡ4096��ԭ����x86��Alphaƽ̨����8k��ҳ�ġ�
#define DEFUALT_CHUNK_SIZE 8192

		class MemPool
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//һ���ڴ森�ڴ��Ĵ�СΪ,blockSize * blocks.
			class MemChunck
			{
				friend class MemPool;

				bool  init(size_t blockSize, unsigned short blocks);
				void  reset(size_t blockSize, unsigned short blocks);
				void  Release();


				void* alloc(size_t blockSize);
				void  dealloc(void* p, size_t blockSize);

				bool  is_ptr(void* p,size_t blockSize, unsigned short blocks);

			private://��Ա
				//�ڴ��ĵ�ַ
				unsigned char* m_pData;
				//��һ�����ÿ飬һ�����ÿ�Ĵ�С����blockSizeָ���ģ�
				//����˵һ��MemChunck�Ĵ�С�ǡ�blockSize * blocks
				unsigned short m_firstAvailableBlock;
				unsigned short m_blocksAvailable;
			};

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		public:
			/*
			����һ��MemPool
			*/
			MemPool(){}
			MemPool(size_t block_size, size_t block_reserved,size_t chunck_size = DEFUALT_CHUNK_SIZE);
			bool   init(size_t block_size, size_t block_reserved,size_t chunck_size = DEFUALT_CHUNK_SIZE);
			void clear();
			void clear_unused();

			//----------------------------------------------------
			//Ϊ���MemPool�������ٸ��ֽڵĿռ�
			//----------------------------------------------------
			void        reserve(size_t block_reserved);
			//----------------------------------------------------
			//�ͷ�û���ù��Ŀռ䣮
			//----------------------------------------------------
			size_t unused_block();

			//----------------------------------------------------
			//�鿴���ж��ٿռ������
			//----------------------------------------------------
			size_t capacity();

			//----------------------------------------------------
			//����һ���ڴ���ͷ�һ���ڴ�
			//----------------------------------------------------
			void*     alloc();
			void      dealloc(void* p);

			//----------------------------------------------------
			//�ж�һ��ָ���ǲ��������Pool����ġ�
			//----------------------------------------------------
			bool      is_ptr(void* p);

			//********************************************************
			//
			//********************************************************
#ifdef _DEBUG
			template<typename T>      void  diagnostic(T& out)
			{

			}
#endif
			void  diagnostic();

			/**
			�ڴ������Ҫ��¼����Ϣ
			1: ÿһ����Ĵ�С����FixedAlloctor�ܷ���Ĵ�С. ��m_blockSize����Ϊ1.
			2: ÿ��MemChunck�У�Block�ĸ���.
			3: �ڴ���У����еĿ�ĸ���
			*/

		private:
			size_t      m_blockSize;       //ÿһ����Ĵ�С����FixedAlloctor�ܷ���Ĵ�С. ��m_blockSize����Ϊ1.
			size_t      m_blocksPerChunk;  //ÿ��MemChunck�У�Block�ĸ���.
			size_t      m_avaliableBlocks; // �ڴ���У����еĿ�ĸ���

			typedef std::vector<MemChunck,std::malloc_allocator<MemChunck> > MemChunks;
			MemChunks        m_Chuncks;

			int              m_lastChunk;



			//-------------------------------------------------------
			//End of the class MemPool
		};

	};//End Name Space Mem
};//End Name Space XEvol3D



#endif

