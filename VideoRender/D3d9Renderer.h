#pragma once




#include <WinSock2.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")       // located in DirectX SDK

#include "video_renderer.h"
#include "webrtc/rtc_base/scoped_ref_ptr.h"
#include "webrtc/typedefs.h"
#include "webrtc/rtc_base/thread.h"

namespace webrtc {

	class D3d9Renderer : public VideoRenderer , public rtc::Thread
	{
	public:


		D3d9Renderer(const void *hwnd, bool fStart = true);
		virtual ~D3d9Renderer();

		virtual void OnFrame(const webrtc::VideoFrame& frame) override;		


		virtual void setRenderAspect(float aspect) override; //���û������	
		virtual void resetRenderDevice(void *winId) override;
		virtual void startRender() override;
		virtual void stopRender() override;

	protected:
		void Run();

	private:
		uint8_t* Data(const uint8_t* y);
		bool Init();
		void renderFrame(const webrtc::VideoFrame *frame);
		void reset();
		
		void saveFrame(const webrtc::VideoFrame& frame, const std::string filename);

		bool						m_fRuning;
		float						m_fAspect;						//Ĭ�� 16��9

		HWND						m_pPlayWnd = NULL;				//��Ⱦ���ĸ�����
		LPDIRECT3DSURFACE9			m_pDirectSurface9 = NULL;		
		LPDIRECT3D9					m_pD3d = NULL;
		LPDIRECT3DDEVICE9			m_pD3dDev = NULL;
		D3DPRESENT_PARAMETERS		m_D3dParam;
		D3DFORMAT					m_D3dFmt;						//surface��ʽ

		RECT						m_rcWin = { 0 };				//��Ⱦ���ڵ�size
		RECT						m_lastRect = { 0 };
		RECT						m_contentRect = { 0 };			//����aspect���������Ⱦ����

	};
}  // namespace webrtc

