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

@interface EAGLView : UIView <UIAccelerometerDelegate>
{
    EAGLContext* context_;
    kernel*      _k;
    
    glm::mat4 input_transform;
}

- (id) initWithFrame:(const CGRect&)frame kernel:(kernel*)k;

@end
