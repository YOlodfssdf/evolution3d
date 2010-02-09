#ifndef _TERRAIN_DEF_H_
#define _TERRAIN_DEF_H_



// ˼·:
// 128 x 128 : 16KB[count] Vertices / patch
// 10000 x 10000 : ��Լ78 x 78 patch��Լ��6084���ύ����
// �༶Patch���Լ����ύ���̣����罫10000x10000����ϸ��Ϊ100��1000x1000�������һ������ϸPatch�ύ��Զ���Ĳ��ü򻯶����ύ��
#define PATCH_SIDETILECOUNT 128
#define PATCH_TILECOUNT (PATCH_SIDETILECOUNT*PATCH_SIDETILECOUNT)
#define PATCH_SIDEVERTCOUNT (PATCH_SIDETILECOUNT+1)
#define PATCH_VERTCOUNT (PATCH_SIDEVERTCOUNT*PATCH_SIDEVERTCOUNT)




#endif