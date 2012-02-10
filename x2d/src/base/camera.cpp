//
//  camera.cpp
//  x2d
//
//  Created by Alex Kremer on 2/7/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "camera.h"
#include "object.h"

namespace x2d {
namespace base {

    void camera::calculate_in_world(const boost::shared_ptr<object>& o)
    {
        // position is 0.0,0.0 == left-bottom corner; 1.0,1.0 == right-top corner. 0.5,0.5 = center
        vector_2d vp( o->camera_space_position().x()*frustum_.width, o->camera_space_position().y()*frustum_.height );
        
        // revert all camera stuff
        affine_matrix m = affine_matrix::translation(-frustum_.width/2.0f, -frustum_.height/2.0f);
        m              *= affine_matrix::rotate( math::to_rad(360.0f-rotation_) );
        m              *= affine_matrix::scale(1.0f/zoom_, 1.0f/zoom_);
        m              *= affine_matrix::translation(-position_.x(), -position_.y());
        vp = m.apply(vp);
        
        float rot = 360.0f-rotation_;
        rot += o->camera_space_rotation();
        
        float zm = 1.0f/zoom_;
        zm *= o->camera_space_scale();
        
        // set new properties in world-space
        o->position(vp);
        o->rotation(rot);
        o->scale(zm);
    }

} // namespace base
} // namespace x2d