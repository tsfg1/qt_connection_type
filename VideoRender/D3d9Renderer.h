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


		virtual void setRenderAspect(float aspect) override; //设置画面比例	
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
		float						m_fAspect;						//默认 16比9

		HWND						m_pPlayWnd = NULL;				//渲染在哪个窗口
		LPDIRECT3DSURFACE9			m_pDirectSurface9 = NULL;		
		LPDIRECT3D9					m_pD3d = NULL;
		LPDIRECT3DDEVICE9			m_pD3dDev = NULL;
		D3DPRESENT_PARAMETERS		m_D3dParam;
		D3DFORMAT					m_D3dFmt;						//surface格式

		RECT						m_rcWin = { 0 };				//渲染窗口的size
		RECT						m_lastRect = { 0 };
		RECT						m_contentRect = { 0 };			//根据aspect计算出的渲染区域

	};
}  // namespace webrtc

