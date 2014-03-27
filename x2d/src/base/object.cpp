//
//  object.cpp
//  x2d
//
//  Created by Alex Kremer on 2/5/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "object.h"
#include "configuration.h"

namespace x2d {

    object_traits::object_traits()
    : name("_no_name_") // can't really happen if you use x2d config system
    , path("_no_path_") // can't really happen if you use x2d config system
    , scale(1.0f)
    , has_bgcolor(false)
    , bgcolor(color_info(0.0f, 0.0f, 0.0f, 0.0f))
    , lifetime(0.0f)
    , want_screen_touch_input(false)
    , want_world_touch_input(false)
    , want_accelerometer_input(false)
    , enabled(true)
    , visible(true)
    , has_animation(false)
    , has_sprite(false)
    , has_spawner(false)
    , has_emitter(false)
    , has_zone(false)
    , has_text(false)
    , align(LEFT_ALIGN)
    , has_body(false)
    , camera("camera") // FIXME
    , obj_space(WORLD_SPACE)
    , par_space(PARENT_SPACE_NONE) // all in world space by default
    , has_parent(false)
    {            
    }
    
    void object::set_lifetime(const float& ttl)
    {
        if(ttl < 0.0f)
        {
            lifetime_timer_.cancel();
        }
        else
        {
            lifetime_timer_.set(ttl);
        }
    }
            
    void object::on_lifetime_timer(const clock_info& clock)
    {
        LOG("[%s] Lifetime timer handler...", name_.c_str());
        
        // nuke all children first
        for_each(children_.begin(), children_.end(),
            boost::bind(&object::on_lifetime_timer, _1, clock));
        
        lifetime_timer_.cancel();        
        config_.deregister_object(shared_from_this());
    }

    object::object(configuration& c, const object_traits& t, spawner* spwn)
    : base_object(c.get_kernel())
    , config_(c)
    , lifetime_timer_(c.get_kernel())
    , name_(t.name)
    , path_(t.path)
    , camera_(config_.get_object<camera>(t.camera))
    , space_(t.obj_space)
    , parent_space_(t.par_space)
    , position_(*t.position)
    , camera_space_position_(*t.position)
    , scale_(*t.scale)
    , rotation_(*t.rotation)
    , box_(*t.box)
    , camera_space_box_(box_) // copy from originally generated value
    , pivot_(*t.pivot)
    , camera_space_pivot_(pivot_)
    , bgcolor_(t.bgcolor)
    , align_(t.align)
    , is_enabled_(*t.enabled)
    , is_visible_(*t.visible)
    , parent_(NULL)
    , has_parent_(t.has_parent)
    , on_create_(config_)
    , on_destroy_(config_)
    , on_input_began_(config_)
    , on_input_moved_(config_)
    , on_input_ended_(config_)
    {
        if(!t.on_create.empty())
        {
            on_create_.set_script(config_.create_sys_object<script>(t.on_create));
        }
        
        if(!t.on_destroy.empty())
        {
            on_destroy_.set_script(config_.create_sys_object<script>(t.on_destroy));
        }
        
        if(!t.on_input_began.empty())
        {
            if(! on_input_began_ && ! on_input_moved_ && ! on_input_ended_)
            {
                LOG("[%s] Register for input (input began script)", name_.c_str());
                config_.get_input_manager().register_object(this);
            }

            on_input_began_.set_script(config_.create_sys_object<script>(t.on_input_began));
        }
        
        if(!t.on_input_moved.empty())
        {
            if(! on_input_began_ && ! on_input_moved_ && ! on_input_ended_)
            {
                LOG("[%s] Register for input (input moved script)", name_.c_str());
                config_.get_input_manager().register_object(this);
            }

            on_input_moved_.set_script(config_.create_sys_object<script>(t.on_input_moved));
        }

        if(!t.on_input_ended.empty())
        {
            if(! on_input_began_ && ! on_input_moved_ && ! on_input_ended_)
            {
                LOG("[%s] Register for input (input ended script)", name_.c_str());
                config_.get_input_manager().register_object(this);
            }

            on_input_ended_.set_script(config_.create_sys_object<script>(t.on_input_ended));
        }
        
        if(spwn)
        {
            LOG("Creating object relative to a spawner...");
            position_ += spwn->world_position();
        }
        
        lifetime_timer_.handler( boost::bind(&object::on_lifetime_timer, this, _1) );
        
        float lt = *t.lifetime;
        if(lt > 0.0f)
        {
            LOG("Setting lifetime!");
            set_lifetime(lt);
        }
        
        if(*t.want_screen_touch_input)
        {
            connect_touch_input(SCREEN_SPACE);
        }
        
        if(*t.want_world_touch_input)
        {
            connect_touch_input(WORLD_SPACE);
        }
        
        if(*t.want_accelerometer_input)
        {
            // TODO accel connect
        }
        
        if(t.has_animation || t.has_sprite || t.has_bgcolor || t.has_text || t.has_emitter)
        {
            if(!has_parent_)
            {
                LOG("[%s] Connecting render: %f [%s]", name_.c_str(), position_.z,
                    space_ == CAMERA_SPACE? "CAMERA":"WORLD");

                connect_render(position_.z, space_ == CAMERA_SPACE);
            }
        }
        
        if(t.has_animation)
        {
            cur_animation_ = config_.create_sys_object<animation>(t.animation);
            
            if(!has_parent_)
            {
                connect_update(); // animation needs updating
            }
        }
        
        if(t.has_sprite)
        {
            cur_sprite_ = config_.get_object<sprite>(t.sprite);
        }
        
        if(t.has_spawner)
        {
            spawner_ = config_.create_sys_object<spawner>(t.spawner);
            spawner_->set_parent(this); // the spawner will live while this object lives
        }
        
        if(t.has_emitter)
        {
            emitter_ = config_.create_sys_object<emitter>(t.emitter);
            
            if(!has_parent_)
            {
                connect_update(); // emitter needs updating
            }
        }
        
        if(t.has_zone)
        {
            zone_ = config_.get_object<zone>(t.zone);
            zone_->set_parent(this); // moves along the object
        }
        
        if(t.has_text)
        {
            cur_font_ = config_.get_object<font>(t.font);
            text_ = boost::shared_ptr<std::string>( new std::string(t.text) );
        }
    
        // check space and add object to list of camera space objects
        if(space_ == CAMERA_SPACE && !has_parent_)
        {
            LOG("[%s] Add camera space object", name_.c_str());
            kernel_.add_camera_space_object(this);
        }
        
        // add physics body if available
        if(t.has_body) 
        {
            body_ = config_.create_sys_object_1<body>(t.body, *this);
            
            if(!has_parent_)
            {
                connect_update(); // physics require updating
            }
        }
        
        // add children
        for(std::vector<std::string>::const_iterator it = t.children.begin(); it != t.children.end(); ++it)
        {
            LOG("[%s] adding child: %s", name_.c_str(), (*it).c_str());
            add_child( config_.create_object(*it) );
        }
        
        // populate contexts and register with them
        for(std::vector<std::string>::const_iterator it = t.contexts.begin(); it != t.contexts.end(); ++it)
        {
            LOG("Getting context with key '%s'", (*it).c_str());
            boost::shared_ptr<context> ctx = config_.get_object<context>( *it );

            ctx->reg_object(this);
            contexts_.push_back( ctx );
        }        
    }
    
    object::~object()
    {
        LOG("[%s] Removing object from everywhere", name_.c_str());
        
        // unregister object from all contexts it's in atm.
        for(std::vector<boost::shared_ptr<context> >::const_iterator it = contexts_.begin(); it != contexts_.end(); ++it)
        {
            (*it)->unreg_object(this);            
        }
        
        kernel_.remove_camera_space_object(this);
    }
    
    void object::destroy_self()
    {
        LOG("destroy_self called on object 0x%X", this);
        set_lifetime(0.0f);
    }
    
    void object::set_parent(object* o)
    {
        parent_ = o;
    }
        
    void object::update(const clock_info& clock) 
    {
        if(!is_enabled_)
        {
            return;
        }

        //
        // TODO: this stuff should be recalculated only if the values got dirty!!
        //
        if(parent_ != NULL && parent_space_ != PARENT_SPACE_NONE)
        {
            reposition_in_parent_space(parent_->box());
        }
        else if(space_ == CAMERA_SPACE && !has_parent_)
        {
            reposition_in_parent_space(camera_->frustum());
        }
        
        // physics
        if(body_) 
        {
            position(body_->position() + pivot_);
            rotation( glm::degrees(body_->angle()) );
        }
        
        if(cur_animation_)
        {
            cur_animation_->update(clock);
        }
        
        if(emitter_)
        {
            emitter_->update(clock);
        }
        
        // update all children
        std::for_each(children_.begin(), children_.end(), 
            boost::bind(&object::update, _1, clock));
    }
    
    void object::render(const clock_info& clock)
    {
        if(!is_enabled_ || !is_visible_)
        {
            return;
        }

        glPushMatrix();
        glTranslatef(position_.x, position_.y, 0.0f);
        
        if(rotation_ != 0.0f)
        {
            glRotatef(rotation_, 0, 0, 1);
        }
        
        if(scale_ != 0.0f)
        {
            glScalef(scale_, scale_, 1.0f);
        }
        
        // draw all children which are behind the parent
        // in current space (relative to this object)
        std::for_each(std::find_if(children_.begin(), children_.end(), 
            boost::bind(&glm::vec3::z, boost::bind(&object::position_, _1))
                > -std::numeric_limits<float>::epsilon() ), children_.end(),
                boost::bind(&object::render, _1, clock) );
        
        // draw the rect with bgcolor if required
        if(bgcolor_.a > 0.0f)
        {
            glPushMatrix();
            glTranslatef(-pivot_.x, -pivot_.y, 0.0f);

            glDisable(GL_TEXTURE_2D);
            glColor4f(bgcolor_.r, bgcolor_.g, bgcolor_.b, bgcolor_.a);

            size b = box();

            float vertices[] =
            {      
                -b.width / 2,     -b.height / 2,    0.0,
                b.width / 2,      -b.height / 2,    0.0,
                -b.width / 2,     b.height / 2,     0.0,
                b.width / 2,      b.height / 2,     0.0
            };
            
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glVertexPointer(3, GL_FLOAT, 0, vertices);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisable(GL_BLEND);        
            glEnable(GL_TEXTURE_2D);
            
            glPopMatrix();
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        
        if(cur_animation_)
        {
            cur_animation_->draw();
        }
        
        if(cur_sprite_)
        {
            cur_sprite_->draw();
        }
        
        if(text_)
        {
            cur_font_->print(*text_, align_);
        }

        glColor4f(1, 1, 1, 1);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);   
        glDisable(GL_BLEND);
        
        if(emitter_)
        {
            emitter_->draw();
        }
        
        // draw all children which are in front of the parent
        // in current space (relative to this object)
        std::for_each(std::find_if(children_.begin(), children_.end(), 
            boost::bind(&glm::vec3::z, boost::bind(&object::position_, _1))
                <= -std::numeric_limits<float>::epsilon() ), children_.end(),
                boost::bind(&object::render, _1, clock) );
        
        glPopMatrix();        
    }

    void object::reposition_in_parent_space(const size& b)
    {
        // only need to calculate box if working in parent space
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            box_ = size( camera_space_box_.width * b.width, 
                         camera_space_box_.height * b.height);
        }

        // position is 0.0,0.0 == left-bottom corner; 1.0,1.0 == right-top corner. 0.5,0.5 = center
        if(parent_ && 
           (parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_POSITION))
        {            
            position( glm::vec2(-(b.width/2) + camera_space_position_.x * b.width, 
                                -(b.height/2) + camera_space_position_.y * b.height) );
        } 
        else
        {
            position( glm::vec2(camera_space_position_.x * b.width, 
                                camera_space_position_.y * b.height) );            
        }
        
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            glm::vec2 pi( camera_space_pivot_.x * box_.width, camera_space_pivot_.y * box_.height );
            pivot_ = glm::vec2(-(box_.width/2)+pi.x, -(box_.height/2)+pi.y);
        }
    }
    
    boost::shared_ptr<object> object::child_by_name(const std::string& n)
    {
        for(std::vector< boost::shared_ptr<object> >::iterator it = children_.begin(); 
            it != children_.end(); ++it)
        {
            if((*it)->name_ == n)
                return *it;                        
        }
        
        LOG("Couldn't find child by name: '%s'", n.c_str());
        throw sys_exception("Couldn't find child by name: '" + n + "'");
    }
    
    const std::string object::name() const
    {
        return name_;
    }

    const object::object_path object::path() const
    {
        return path_;
    }

    void object::add_child(const boost::shared_ptr<object>& child)
    {
        child->set_parent(this);
        children_.push_back(child);
        
        if(!has_parent_)
        {
            // Now connect update and render if we did not before.
            // Need these connections to update children.
            connect_update();

            LOG("[%s] Connecting render: %f [%s]", name_.c_str(), position_.z,
                space_ == CAMERA_SPACE? "CAMERA":"WORLD");
            connect_render(position_.z, space_ == CAMERA_SPACE);
        }
    }
    
    void object::release(object& obj)
    {
        if(spawner_)
        {
            spawner_->release(obj);
        }
    }
    
    const glm::vec3 object::position() const
    {
        return position_;
    }
    
    void object::camera_space_position(const glm::vec3& p)
    {
        camera_space_position_ = p;
    }
    
    void object::position(const glm::vec3& p)        
    {
        position_ = p;

        if(body_)
        {
            body_->position(glm::vec2(position_.x - pivot_.x, position_.y - pivot_.y));
        }
    }
    
    void object::camera_space_position(const glm::vec2& p)
    {
        camera_space_position_.x = p.x;
        camera_space_position_.y = p.y;
        // retain z value
    }
    
    void object::position(const glm::vec2& p)        
    {
        position_.x = p.x;
        position_.y = p.y;
        // retain z value
        
        if(body_) 
        {
            body_->position(glm::vec2(p.x - pivot_.x, p.y - pivot_.y));
        }
    }        
    
    const glm::mat4 object::final_transform() const
    {
        // TODO: make this a member and cache until invalidated by position/scale/rotation etc.
        glm::mat4 transform_(1.0f);
        
        if(position_.x != 0.0f || position_.y != 0.0f)
            transform_ = glm::translate(transform_, glm::vec3(position_.x, position_.y, 0.0f));

        if(rotation_ != 0.0f)
            transform_ = glm::rotate(transform_, rotation_, glm::vec3(0,0,1));

        if(scale_ != 1.0f)
            transform_ = glm::scale(transform_, glm::vec3(scale_, scale_, 1.0f));
   
        if(pivot_.x != 0.0f || pivot_.y != 0.0f)
            transform_ = glm::translate(transform_, glm::vec3(-pivot_.x, -pivot_.y, 0.0f));
        
        if(parent_)
            return parent_->final_transform() * transform_;
        
        return transform_;
    }

    const glm::vec3 object::world_position() const
    {
        glm::vec4 pp(0, 0, 0, 1.0f);
        pp = final_transform() * pp;
        return glm::vec3(pp.x, pp.y, position_.z);
    }

    bool object::is_camera_space() const
    {
        if(space_ == CAMERA_SPACE)
            return true;
        
        if(parent_ != NULL)
            return parent_->is_camera_space();
        
        return false;
    }
    
    const bbox object::screen_bbox() const
    {
        size b = box();
        glm::vec4 bl(-b.width/2, -b.height/2, position_.z, 1.0f);
        glm::vec4 tl(-b.width/2, b.height/2, position_.z, 1.0f);
        glm::vec4 tr(b.width/2, b.height/2, position_.z, 1.0f);
        glm::vec4 br(b.width/2, -b.height/2, position_.z, 1.0f);
        
        glm::mat4 m = is_camera_space() ?
            final_transform() // just the object transformation
                :
            (camera_->transform() * final_transform()); // with camera
        bl = m * bl;
        tl = m * tl;
        tr = m * tr;
        br = m * br;
        
        return bbox(glm::vec3(bl.x, bl.y, position_.z),
                    glm::vec3(tl.x, tl.y, position_.z),
                    glm::vec3(tr.x, tr.y, position_.z),
                    glm::vec3(br.x, br.y, position_.z));
    }
    
    const float object::rotation() const
    {
        return rotation_;
    }
    
    void object::rotation(float a)        
    {
        rotation_ = a;
    }
    
    void object::scale(float s)
    {
        scale_ = s;
    }

    float object::scale() const
    {
        return scale_;
    }

    void object::pivot(const glm::vec2& p)
    {
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            camera_space_pivot_ = p;
        }
        else
        {
            pivot_ = p;
        }
    }
    
    void object::box(const size& s)
    {
        if(parent_space_ == PARENT_SPACE_BOTH || parent_space_ == PARENT_SPACE_BOX)
        {
            camera_space_box_ = s;
        }
        else
        {
            box_ = s;
        }
    }
    
    const std::string object::text() const
    {
        return *text_;
    }
    
    void object::text(const std::string& t)
    {
        text_ = boost::shared_ptr<std::string>(new std::string(t) );
    }
    
    const glm::vec3& object::camera_space_position() const
    {
        return camera_space_position_;
    }
    
    const size object::box() const
    {        
        if(box_.width != 0 && box_.height != 0) 
        {
            return box_;
        }
        else if(cur_sprite_)
        {
            return cur_sprite_->box();
        }
        else if(cur_animation_)
        {
            return cur_animation_->box();
        }
        
        return size(); // return default
    }
    
    const size object::camera_space_box() const
    {
        return camera_space_box_;
    }
    
    const glm::vec2 object::pivot() const
    {
        return pivot_;
    }
    
    const glm::vec2 object::camera_space_pivot() const
    {
        return camera_space_pivot_;
    }
    
    void object::set_sprite(const boost::shared_ptr<sprite>& spr)
    {
        cur_sprite_ = spr;
    }
    
    void object::set_animation(const boost::shared_ptr<animation>& anim)
    {
        cur_animation_ = anim;
    }
    
    void object::set_bgcolor(const color_info& bgcolor)
    {
        bgcolor_ = bgcolor;
    }
    
    void object::enable(bool e)
    {
        is_enabled_ = e;
    }

    bool object::enabled() const
    {
        return is_enabled_;
    }

    void object::visible(bool v)
    {
        is_visible_ = v;
    }

    boost::shared_ptr<spawner> object::get_spawner()
    {
        return spawner_;
    }
    
    boost::shared_ptr<emitter> object::get_emitter()
    {
        return emitter_;
    }
    
    boost::shared_ptr<animation> object::get_animation()
    {
        return cur_animation_;
    }
    
    boost::shared_ptr<sprite> object::get_sprite()
    {
        return cur_sprite_;
    }
    
    boost::shared_ptr<body> object::get_body()
    {
        return body_;
    }
    
    void object::on_collision_begin(object* with)
    {
    }

    void object::on_collision_begin(const std::string& me, const std::string& with)
    {
    }
    
    void object::on_collision_end(object* with)
    {
    }

    void object::on_collision_end(const std::string& me, const std::string& with)
    {
    }
    
    bool object::on_collision_presolve(const std::string& me, const std::string& with, object* other_obj)
    {
        return true;
    }

    void object::set_on_input_began(const boost::function<void(object*)>& fn)
    {
        if(! on_input_began_ && ! on_input_moved_ && ! on_input_ended_)
        {
            config_.get_input_manager().register_object(this);
        }
        
        on_input_began_.set_callback(fn);
    }

    void object::set_on_input_moved(const boost::function<void(object*)>& fn)
    {
        if(! on_input_began_ && ! on_input_moved_ && ! on_input_ended_)
        {
            config_.get_input_manager().register_object(this);
        }
        
        on_input_moved_.set_callback(fn);
    }
    
    void object::set_on_input_ended(const boost::function<void(object*)>& fn)
    {
        if(! on_input_began_ && ! on_input_moved_ && ! on_input_ended_)
        {
            config_.get_input_manager().register_object(this);
        }
        
        on_input_ended_.set_callback(fn);
    }
} // namespace x2d