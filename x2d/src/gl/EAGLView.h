//
//  EAGLView.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "kernel.h"
#import "glm.hpp"
#import "platform.h"

@interface EAGLView : UIView <UIAccelerometerDelegate>
{
    EAGLContext* context_;
    
    glm::mat4 input_transform;
}

- (id) initWithCapabilities:(const device_capabilities&)caps;

@end
