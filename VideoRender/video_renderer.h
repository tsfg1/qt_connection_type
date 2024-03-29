/*
 *  Copyright (c) 2013 The WebRTC@AnyRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef WEBRTC_TEST_VIDEO_RENDERER_H_
#define WEBRTC_TEST_VIDEO_RENDERER_H_

#include <stddef.h>

#include "webrtc/api/video/video_sink_interface.h"
#include "webrtc/api/video/video_frame.h"

namespace webrtc {

class VideoRenderer : public rtc::VideoSinkInterface<webrtc::VideoFrame> 
{
 public:
	//static VideoRenderer* Create(const void* hwnd, size_t width, size_t height);    // Creates a platform-specific renderer if possible, or a null implementation if failing.
	static VideoRenderer* Create(const void* hwnd, size_t width, size_t height);    // Creates a platform-specific renderer if possible, or a null implementation if failing.
	static VideoRenderer* Create(const void* hwnd, float aspect);    // Creates a platform-specific renderer if possible, or a null implementation if failing.


	virtual void resetRenderDevice(void *winId) {};
	virtual void startRender() {};
	virtual void stopRender() {};
	virtual void setRenderAspect(float aspect) {};

	 virtual ~VideoRenderer() {}
 protected:
	VideoRenderer() {}
};
}  // namespace webrtc

#endif  // WEBRTC_TEST_VIDEO_RENDERER_H_
