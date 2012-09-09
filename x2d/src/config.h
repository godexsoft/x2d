//
//  config.h
//  x2d
//
//  Created by Alex Kremer on 2/13/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#ifndef __X2D_CONFIG_H__
#define __X2D_CONFIG_H__

// auto selection of driver
#ifdef ANDROID
#	define X2D_NULL_SOUND 1
#elif defined(__APPLE__)
#	define X2D_IOS_SOUND 1
#else
#	warning "Couldn't auto-select audio driver. Will use NULL sound."
#	define X2D_NULL_SOUND 1
#endif

#if defined X2D_NULL_SOUND
#	define X2D_SND_DRIVER null_audio_driver
#elif defined X2D_IOS_SOUND
#	define X2D_SND_DRIVER audio_queue_driver
#endif

//
// you can override sound driver here
//

#endif // __X2D_CONFIG_H__
