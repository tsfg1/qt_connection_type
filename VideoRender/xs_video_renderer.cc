/*
 *  Copyright (c) 2013 The WebRTC@AnyRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include "xs_video_renderer.h"

// TODO(pbos): Android renderer

#include "webrtc/typedefs.h"
#include "XSD3d9Renderer.h"
#include "d3d_renderer.h"

namespace webrtc {

class NullRenderer : public VideoRenderer 
{
	void OnFrame(const webrtc::VideoFrame& video_frame) override {}
};


XSVideoRenderer* XSVideoRenderer::Create(const void* hwnd,size_t width,size_t height)
{
#if 1
	return new XSD3d9Renderer(hwnd);
#else
	auto *r = new D3dRenderer(hwnd, width, height);
	r->Init(hwnd);
	return r;
#endif
}
}  // namespace webrtc
