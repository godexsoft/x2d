//
//  EAGLView.h
//  x2d
//
//  Created by Alex Kremer on 1/22/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "kernel.h"
#import "math_util.h"

@interface EAGLView : UIView 
{
    EAGLContext* context_;
    kernel*      _k;
    
    affine_matrix input_transform;
}

- (id) initWithFrame:(const CGRect&)frame kernel:(kernel*)k;

@end
