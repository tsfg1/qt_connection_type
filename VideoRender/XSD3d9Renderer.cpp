#include "XSD3d9Renderer.h"
#include <windows.h>
#include "webrtc/api/video/video_frame.h"
#include "webrtc/rtc_base/bind.h"
#include "third_party/libyuv/include/libyuv.h"
#include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"
#include "webrtc/common_video/include/video_frame_buffer.h"
#include "webrtc/api/video/i420_buffer.h"
#include <iostream>
#include <functional>
#include <fstream>

namespace webrtc {

	XSD3d9Renderer::XSD3d9Renderer(const void *hwnd, bool fStart):
		m_pDirectSurface9(NULL),
		m_pD3d(NULL),
		m_pD3dDev(NULL),
		m_rcWin{ 0 },				
		m_lastRect{ 0 },
		m_contentRect{ 0 },
		m_fRuning(false),
		m_fAspect(16.0 / 9.0),
		m_pPlayWnd((HWND)hwnd)
	{		
		if (hwnd == NULL)
			return;
		reset();
		//Init();
		m_fRuning = Start(); //开启渲染线程
		//this->Invoke<bool>(RTC_FROM_HERE, rtc::Bind(&D3d9Renderer::Init, this));//在线程内初始化 防止阻塞其他线程
	}

	XSD3d9Renderer::~XSD3d9Renderer()
	{
		this->Stop();
		m_fRuning = false;
		reset();
	}


	void XSD3d9Renderer::OnFrame(const webrtc::VideoFrame & frame)
	{	
		//测试:保存一帧数据到文件 是为了用QtAV渲染
		//saveFrame(frame, "f:\\frame.yuv");
		
#if 1
		if (!m_fRuning)
		{
			return;
		}
		else {
			this->Invoke<void>(RTC_FROM_HERE, rtc::Bind(&XSD3d9Renderer::renderFrame, this, &frame));
		}
#else
			renderFrame(&frame); //非线程渲染模式
#endif	
	}
	void XSD3d9Renderer::resetRenderDevice(void * winId)
	{
			if (winId == NULL || winId == m_pPlayWnd)
				return;
			m_pPlayWnd = HWND(winId);
			this->Stop();
			m_fRuning = false;
			reset();
			//Init();
			m_fRuning = this->Start();
	}

	void XSD3d9Renderer::startRender()
	{
		if (!m_fRuning)
		{
			m_fRuning = this->Start();
		}
	}

	void XSD3d9Renderer::stopRender()
	{
		if (m_fRuning)
		{
			this->Stop();
			m_fRuning = false;
		}
	}

	void XSD3d9Renderer::Run()
	{
		Init();
		rtc::Thread::Run();
	}


	void XSD3d9Renderer::setRenderAspect(float aspect)
	{
		this->Invoke<void>(RTC_FROM_HERE, std::bind([this](float as)
		{
			m_fAspect = as;
		}, aspect));
	}

	void XSD3d9Renderer::reset()
	{
		if (m_pDirectSurface9)
		{
			m_pDirectSurface9->Release();
			m_pDirectSurface9 = NULL;
		}
		if (m_pDirectSurface9)
		{
			m_pD3d->Release();
			m_pD3d = NULL;
		}
		if (m_pD3dDev)
		{
			m_pD3dDev->Release();
			m_pD3dDev = NULL;
		}

		ZeroMemory(&m_D3dParam, sizeof(D3DPRESENT_PARAMETERS));
		m_rcWin = { 0 };
		m_lastRect = { 0 };
		m_contentRect = { 0 };
	}

	void XSD3d9Renderer::saveFrame(const webrtc::VideoFrame& frame, const std::string filename)
	{
		std::ofstream of(filename, std::ios::binary | std::ios::trunc);
		if (of.is_open())
		{
			//of.write()
			int w = frame.width();
			int h = frame.height();
			int sy = frame.video_frame_buffer()->GetI420()->StrideY();
			int su = frame.video_frame_buffer()->GetI420()->StrideU();
			int sv = frame.video_frame_buffer()->GetI420()->StrideV();

			of.write((char *)&w, sizeof(int));
			of.write((char *)&h, sizeof(int));
			of.write((char *)&sy, sizeof(int));
			of.write((char *)&su, sizeof(int));
			of.write((char *)&sv, sizeof(int));

			for (int i = 0; i < frame.height(); i++)
			{
				of.write((char *)frame.video_frame_buffer()->GetI420()->DataY() + i * frame.video_frame_buffer()->GetI420()->StrideY(),
					frame.video_frame_buffer()->GetI420()->StrideY());
			}
			for (int i = 0; i < frame.height() / 2; i++)
			{
				of.write((char *)frame.video_frame_buffer()->GetI420()->DataU() + i * frame.video_frame_buffer()->GetI420()->StrideU(),
					frame.video_frame_buffer()->GetI420()->StrideU());

			}
			for (int i = 0; i < frame.height() / 2; i++)
			{
				of.write((char *)frame.video_frame_buffer()->GetI420()->DataV() + i * frame.video_frame_buffer()->GetI420()->StrideV(),
					frame.video_frame_buffer()->GetI420()->StrideV());
			}

			of.close();
		}
		else
		{

		}
	}

	
	bool XSD3d9Renderer::Init()
	{
		//Direct3d COM接口
		m_pDirectSurface9 = (LPDIRECT3DSURFACE9)calloc(1, sizeof(IDirect3DSurface9));
		//创建Direct3D9对象
		m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);

		//描述显示模式的结构体参数
		D3DDISPLAYMODE d3dmode = { 0 };
		ZeroMemory(&m_D3dParam, sizeof(D3DPRESENT_PARAMETERS));
		m_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dmode);
		
		m_D3dParam.BackBufferFormat = D3DFMT_UNKNOWN;       //缓冲区格式
		m_D3dParam.BackBufferCount = 1;
		m_D3dParam.BackBufferWidth = GetSystemMetrics(SM_CXFULLSCREEN);;	//桌面大小
		m_D3dParam.BackBufferHeight = GetSystemMetrics(SM_CYFULLSCREEN);

		m_D3dParam.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_D3dParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_D3dParam.hDeviceWindow = m_pPlayWnd;
		m_D3dParam.Windowed = TRUE;
		m_D3dParam.EnableAutoDepthStencil = FALSE;
		m_D3dParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		//创建一个设置去呈现显示适配器
		if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT
			, D3DDEVTYPE_HAL, m_pPlayWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING
			, &m_D3dParam, &m_pD3dDev)))
		{
			return false;
		}

		//创建一个屏幕之外的平面
		if (SUCCEEDED(m_pD3dDev->CreateOffscreenPlainSurface(1, 1, D3DFMT_YUY2,
			D3DPOOL_DEFAULT, &m_pDirectSurface9, NULL)))
		{
			m_D3dFmt = D3DFMT_YUY2;
		}
		else if (SUCCEEDED(m_pD3dDev->CreateOffscreenPlainSurface(1, 1, D3DFMT_UYVY,
			D3DPOOL_DEFAULT, &m_pDirectSurface9, NULL)))
		{
			m_D3dFmt = D3DFMT_UYVY;
			
		}
		else
		{
			m_D3dFmt = D3DFMT_X8R8G8B8;
		}

		if (m_pDirectSurface9)
		{
			m_pDirectSurface9->Release();
			m_pDirectSurface9 = NULL;
		}
		return true;
	}

	uint8_t* XSD3d9Renderer::Data(const uint8_t* y)
	{
		return const_cast<uint8_t*>(y);
	}

	void XSD3d9Renderer::renderFrame(const webrtc::VideoFrame * frame)
	{
		RECT rcClient;
		GetClientRect(m_pPlayWnd, &rcClient);
		//判断渲染窗口是否改变了size
		if (rcClient.left == rcClient.bottom == rcClient.right == rcClient.top == 0
			|| rcClient.left != m_rcWin.left
			|| rcClient.top != m_rcWin.top
			|| rcClient.right != m_rcWin.right
			|| rcClient.bottom != m_rcWin.bottom)
		{
			InvalidateRect(m_pPlayWnd, &rcClient, FALSE);
			m_rcWin = rcClient;
#if 1
			m_contentRect = rcClient;
#else

			//计算渲染区
			long W = m_rcWin.right - m_rcWin.left;
			long H = m_rcWin.bottom - m_rcWin.top;

			if ((float)W / H > m_fAspect)
			{
				m_contentRect.top = m_rcWin.top;
				m_contentRect.bottom = m_rcWin.bottom;
				float w = m_fAspect * H;
				m_contentRect.left = (W - w) / 2;
				m_contentRect.right = m_contentRect.left + w;
			}
			else
			{
				m_contentRect.left = m_rcWin.left;
				m_contentRect.right = m_rcWin.right;
				float h = W / m_fAspect;
				m_contentRect.top = (H - h) / 2;
				m_contentRect.bottom = m_contentRect.top + h;
			}
			//计算渲染区 end
#endif

			//释放原来的surface
			if (m_pDirectSurface9)
			{
				m_pDirectSurface9->Release();
				m_pDirectSurface9 = NULL;
			}

			//创建一个新的surface
			if (FAILED(m_pD3dDev->CreateOffscreenPlainSurface(m_contentRect.right - m_contentRect.left
				, m_contentRect.bottom - m_contentRect.top, (D3DFORMAT)m_D3dFmt,
				D3DPOOL_DEFAULT, &m_pDirectSurface9, NULL)))
			{
				return ;
			}

			
		}

		D3DLOCKED_RECT d3d_rect;
		if (m_pDirectSurface9 == nullptr)
			return;

			m_pDirectSurface9->LockRect(&d3d_rect, NULL, D3DLOCK_DISCARD);


		//webrtc::VideoFrame scaledFrame;	//用来保存 缩放后的videoframe
		rtc::scoped_refptr<webrtc::I420Buffer> i420_buffer_;
		if (!i420_buffer_.get()) {
			i420_buffer_ = webrtc::I420Buffer::Create(m_contentRect.right - m_contentRect.left, m_contentRect.bottom - m_contentRect.top);
		};
		webrtc::VideoFrame  scaledFrame = webrtc::VideoFrame::Builder()
			.set_video_frame_buffer(i420_buffer_->GetI420())
			.set_rotation(webrtc::kVideoRotation_0)
			.build();
		//scaledFrame.InitToEmptyBuffer(m_contentRect.right - m_contentRect.left,
			//m_contentRect.bottom - m_contentRect.top);

		//缩放到设置比例的区域中
		libyuv::I420Scale(frame->video_frame_buffer()->GetI420()->DataY(), frame->video_frame_buffer()->GetI420()->StrideY(),
			frame->video_frame_buffer()->GetI420()->DataU(), frame->video_frame_buffer()->GetI420()->StrideU(),
			frame->video_frame_buffer()->GetI420()->DataV(), frame->video_frame_buffer()->GetI420()->StrideV(),
			frame->width(), frame->height(),
			Data(scaledFrame.video_frame_buffer()->GetI420()->DataY()), scaledFrame.video_frame_buffer()->GetI420()->StrideY(),
			Data(scaledFrame.video_frame_buffer()->GetI420()->DataU()), scaledFrame.video_frame_buffer()->GetI420()->StrideU(),
			Data(scaledFrame.video_frame_buffer()->GetI420()->DataV()), scaledFrame.video_frame_buffer()->GetI420()->StrideV(),
			m_contentRect.right - m_contentRect.left, m_contentRect.bottom - m_contentRect.top, libyuv::kFilterBox);

		//if (m_D3dFmt == D3DFMT_YUY2)
		{
			//把I420转为YUV422 并保存至surface中
			libyuv::ConvertFromI420(scaledFrame.video_frame_buffer()->GetI420()->DataY(), scaledFrame.video_frame_buffer()->GetI420()->StrideY(),
				scaledFrame.video_frame_buffer()->GetI420()->DataU(), scaledFrame.video_frame_buffer()->GetI420()->StrideU(),
				scaledFrame.video_frame_buffer()->GetI420()->DataV(), scaledFrame.video_frame_buffer()->GetI420()->StrideV(),
				(uint8_t *)d3d_rect.pBits, d3d_rect.Pitch,
				m_contentRect.right-m_contentRect.left, m_contentRect.bottom-m_contentRect.top, libyuv::FOURCC_YUY2);
		}
		/*else if (m_D3dFmt == D3DFMT_UYVY)
		{
			m_pDirectSurface9->UnlockRect();
			return;
		}
		else
		{
			m_pDirectSurface9->UnlockRect();
			return;
		}
*/
	
		m_pDirectSurface9->UnlockRect();


		IDirect3DSurface9 *pBackBuffer = NULL;

		if (SUCCEEDED(m_pD3dDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
		{
			if (pBackBuffer)
			{
				if (SUCCEEDED(m_pD3dDev->StretchRect(m_pDirectSurface9, NULL, pBackBuffer, NULL, D3DTEXF_LINEAR)))
				{
					m_pD3dDev->Present(NULL, &m_contentRect, NULL, NULL);
					//m_pD3dDev->Present(NULL, &m_rcWin, NULL, NULL);
				}
				pBackBuffer->Release();
			}

		}
	}

}
