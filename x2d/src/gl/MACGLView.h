//
//  MACGLView.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "platform.h"

@interface MACGLView : NSOpenGLView
{
    NSOpenGLContext* context_;
    
    glm::mat4 input_transform;
}

- (id) initWithCapabilities:(const device_capabilities&)caps;

@end
