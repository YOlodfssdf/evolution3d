#ifndef __VIDEO_IMAGE_RENDER_TARGET_H__
#define __VIDEO_IMAGE_RENDER_TARGET_H__
/*
DSHOW������Ƶ��

��Ȩ����
����Ϊ����Դ���롣����ѭ���������ǰ���£������������أ��޸ģ���������Щ���롣
�ô���Ϊ�ο�DShow SDK�Ĵ���д�ɡ��ڱ����������ֵ�ǰ���´�ҿ��������κ���;��
���Ǳ��˲��е��κ����κ�Ǳ�����Ρ�
����    : ������ΪDSHOW������Ƶ���롣

������ 2006/10/10
 */
enum VideoImageFormat
{
	VIF_RGB,
	VIF_RGBA,
	VIF_YV12,
	VIF_YUY2,
};

class IVideoRenderTarget
{
public:


	struct VRT_LockRect
	{
		unsigned char  mBytePerPixel;
		unsigned char* mPixel;
		int            mPicth;

		int            mWidth;
		int            mHeight;

		int            mBufferWidth;
		int            mBufferHeight;
	};


	virtual ~IVideoRenderTarget(){}
	virtual void           release() = 0;
	virtual void           thread_lock() = 0;
	virtual void           thread_unlock() = 0;

	
	//�豸��ɾ����ʱ���յ����֪ͨ
	virtual bool             flipY(){return false; }
	virtual bool             flipRGBA(){return false; }
	virtual bool             onDelete() = 0;
	virtual VideoImageFormat fmt() = 0;
	virtual bool             direct_update() = 0;
	virtual bool             update(void* data , int w , int h , int picth , VideoImageFormat fmt) = 0;
	virtual bool             check_fmt(VideoImageFormat vfmt) = 0;
	//�豸��������ʱ��Ҳ���Ǻ�����ͷ������Ƶ���������ӵ�ʱ�򣬻��յ������Ϣ
	virtual bool             onCreate(int Width,int Height, VideoImageFormat vifmt) = 0;
	//����������������ͷֹͣ��ʱ���յ������Ϣ
	virtual void             onStop() = 0;

	//����������
	virtual void*          lock(VRT_LockRect& lockRect) = 0;
	virtual VRT_LockRect*  lock() = 0;
	virtual void*          unlock() = 0;

	//��ʼ����һ֡ͼ���ʱ���յ������Ϣ
	virtual void           onStartRender() = 0;
	//������ͼ���ʱ���յ������Ϣ�� isRenderedΪtrue��ʱ�򣬱�ʾ���Ƴɹ�������ʧ��
	virtual void           onEndRender(bool isRendered) = 0;


};


#endif
