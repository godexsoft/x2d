//
//  parsers.cpp
//  x2d
//
//  Created by Alex Kremer on 1/31/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#include "configuration.h"
#include "texture.h"
#include "sprite.h"
#include "exceptions.h"
#include "kernel.h"
#include "object.h"
#include "value.h"

#include <sstream>

namespace x2d
{
    namespace config
    {

        void configuration::parse_float(xml_node *node, const config_key& key)
        {
            // must have:
            // n:      name
            // can have:
            // value:  the value
            // or value can be provided as child

            xml_attr *path = node->first_attribute("n");
            if ( !path )
            {
                throw parse_exception("Float must have 'n' defined.");
            }

            xml_attr *value = node->first_attribute("value");
            if ( !value && !node->first_node() )
            {
                throw parse_exception("Float must have either 'value' or inner data defined.");
            }

            if ( !value )
            {
                xml_node *data = node->first_node();

                if ( data->type() == rx::node_data )
                {
                    config_[key] = boost::shared_ptr< value_cfg<float> >(
                            new value_cfg<float>(value_parser<float>::parse(data->value())));
                }
                else
                {
                    // check if it's random
                    if ( std::string("random") == data->name() )
                    {
                        LOG("Random detected.");
                        parse_random<float>(data, key);
                    }
                    else
                    {
                        throw parse_exception("Data element can be either <random> or plain value.");
                    }
                }
            }
            else
            {
                config_[key] = boost::shared_ptr< value_cfg<float> >(
                        new value_cfg<float>(value_parser<float>::parse(value->value())));
            }
        }

        void configuration::parse_int(xml_node *node, const config_key& key)
        {
            // must have:
            // n:      name
            // can have:
            // value:  the value
            // or value can be provided as child

            xml_attr *path = node->first_attribute("n");
            if ( !path )
            {
                throw parse_exception("Int must have 'n' defined.");
            }

            xml_attr *value = node->first_attribute("value");
            if ( !value && !node->first_node() )
            {
                throw parse_exception("Int must have either 'value' or inner data defined.");
            }

            if ( !value )
            {
                xml_node *data = node->first_node();

                if ( data->type() == rx::node_data )
                {
                    config_[key] = boost::shared_ptr< value_cfg<int> >(
                            new value_cfg<int>(value_parser<int>::parse(data->value())));
                }
                else
                {
                    // check if it's random
                    if ( std::string("random") == data->name() )
                    {
                        LOG("Random detected.");
                        parse_random<int>(data, key);
                    }
                    else
                    {
                        throw parse_exception("Data element can be either <random> or plain value.");
                    }
                }
            }
            else
            {
                config_[key] = boost::shared_ptr< value_cfg<int> >(
                        new value_cfg<int>(value_parser<int>::parse(value->value())));
            }
        }

        void configuration::parse_vector(xml_node *node, const config_key& key)
        {
            // must have:
            // n:      name
            // can have:
            // value:  the value
            // or value can be provided as child

            xml_attr *path = node->first_attribute("n");
            if ( !path )
            {
                throw parse_exception("Vector must have 'n' defined.");
            }

            xml_attr *value = node->first_attribute("value");
            if ( !value && !node->first_node() )
            {
                throw parse_exception("Vector must have either 'value' or inner data defined.");
            }

            if ( !value )
            {
                xml_node *data = node->first_node();

                if ( data->type() == rx::node_data )
                {
                    config_[key] = boost::shared_ptr< value_cfg<glm::vec4> >(
                            new value_cfg<glm::vec4>(value_parser<glm::vec4>::parse(data->value())));
                }
                else
                {
                    // check if it's random
                    if ( std::string("random") == data->name() )
                    {
                        LOG("Random detected.");
                        parse_random<glm::vec4>(data, key);
                    }
                    else
                    {
                        throw parse_exception("Data element can be either <random> or plain value.");
                    }
                }
            }
            else
            {
                config_[key] = boost::shared_ptr< value_cfg<glm::vec3> >(
                        new value_cfg<glm::vec3>(value_parser<glm::vec3>::parse(value->value())));
            }
        }

        void configuration::parse_string(xml_node *node, const config_key& key)
        {
            // must have:
            // n:      name
            // can have:
            // value:  the value
            // or value can be provided as child

            LOG("Parsing n in string");
            std::string n = *get_mandatory_attr<std::string>(*this, node, key, "n",
                    parse_exception("String type must have 'n' defined."));

            xml_attr *value = node->first_attribute("value");
            if ( !value && !node->first_node() )
            {
                throw parse_exception("String must have either 'value' or inner data defined.");
            }

            if ( !value )
            {
                xml_node *data = node->first_node();

                if ( data->type() == rx::node_data || data->type() == rx::node_cdata )
                {
                    config_[key] = boost::shared_ptr< value_cfg<std::string> >(
                            new value_cfg<std::string>(value_parser<std::string>::parse(data->value())));
                }
                else
                {
                    throw parse_exception("Data element of String must be plain value or CDATA.");
                }
            }
            else
            {
                config_[key] = boost::shared_ptr< value_cfg<std::string> >(
                        new value_cfg<std::string>(value_parser<std::string>::parse(value->value())));
            }
        }

        void configuration::parse_list(xml_node *node, const config_key& key)
        {
            // must have:
            // n:      name
            // can have:
            // value:  the value
            // or value can be provided as child

            xml_attr *path = node->first_attribute("n");
            if ( !path )
            {
                throw parse_exception("List must have 'n' defined.");
            }

            xml_attr *value = node->first_attribute("value");
            if ( !value && !node->first_node() )
            {
                throw parse_exception("List must have either 'value' or inner data defined.");
            }

            if ( !value )
            {
                xml_node *data = node->first_node();

                if ( data->type() == rx::node_data || data->type() == rx::node_cdata )
                {
                    config_[key] = boost::shared_ptr< value_cfg<std::vector<std::string> > >(
                            new value_cfg<std::vector<std::string> >(value_parser<std::vector<std::string> >::parse(data->value())));
                }
                else
                {
                    throw parse_exception("List data element can be only plain value.");
                }
            }
            else
            {
                config_[key] = boost::shared_ptr< value_cfg<std::vector<std::string> > >(
                        new value_cfg<std::vector<std::string> >(value_parser<std::vector<std::string> >::parse(value->value())));
            }
        }

        void configuration::parse_float_list(xml_node *node, const config_key& key)
        {
            // must have:
            // n:      name
            // can have:
            // value:  the value
            // or value can be provided as child
            
            xml_attr *path = node->first_attribute("n");
            if ( !path )
            {
                throw parse_exception("List must have 'n' defined.");
            }
            
            xml_attr *value = node->first_attribute("value");
            if ( !value && !node->first_node() )
            {
                throw parse_exception("List must have either 'value' or inner data defined.");
            }
            
            if ( !value )
            {
                xml_node *data = node->first_node();
                
                if ( data->type() == rx::node_data || data->type() == rx::node_cdata )
                {
                    config_[key] = boost::shared_ptr< value_cfg<std::vector<float> > >(
                        new value_cfg<std::vector<float> >(value_parser<std::vector<float> >::parse(data->value())));
                }
                else
                {
                    throw parse_exception("List data element can be only plain value.");
                }
            }
            else
            {
                config_[key] = boost::shared_ptr< value_cfg<std::vector<float> > >(
                    new value_cfg<std::vector<float> >(value_parser<std::vector<float> >::parse(value->value())));
            }
        }
        
        void configuration::parse_namespace(xml_node *node, const config_key& key)
        {
            // must have:
            // n: name of the namespace

            // No need to do anything :)
        }

        void configuration::parse_include(xml_node *node, const config_key& key)
        {
            // must have:
            // path:    path to other configuration file

            std::string path = *get_mandatory_attr<std::string>(*this, node, key, "path",
                    parse_exception("Include type must have 'path' defined."));

            // parse the whole config into this configuration
            parse_file(path, key);
        }

        void configuration::parse_texture(xml_node *node, const config_key& key)
        {
            // must have:
            // n:       name of the element
            // path:    path to texture file

            std::string path = *get_mandatory_attr<std::string>(*this, node, key, "path",
                    parse_exception("Texture type must have 'path' defined."));

            config_[key] = boost::shared_ptr<texture_cfg>(new texture_cfg(res_man_, path));
        }

        void configuration::parse_sprite(xml_node *node, const config_key& key)
        {
            // must have:
            // n: name of the element
            //
            // can have:
            // x: x offset inside texture (default to 0)
            // y: y offset inside texture (default to 0)
            // w: width of sprite (default to full texture)
            // h: height of sprite (default to full texture)
            // pivot:    the pivot as 2d vector. defaults to 0,0
            // flip_x:   flip on x axis if true. don't flip if false (default)
            // flip_y:   flip on y axis if true. don't flip if false (default)

            int x = *get_attr<int>(*this, node, key, "x", 0.0f);
            int y = *get_attr<int>(*this, node, key, "y", 0.0f);
            int w = *get_attr<int>(*this, node, key, "w", 0.0f);
            int h = *get_attr<int>(*this, node, key, "h", 0.0f);

            glm::vec2 pivot = *get_attr<glm::vec2>(*this, node, key, "pivot", glm::vec2(0, 0));
            bool flip_x = *get_attr<bool>(*this, node, key, "flip_x", false);
            bool flip_y = *get_attr<bool>(*this, node, key, "flip_y", false);

            // parent must be a texture
            xml_node *parent = node->parent();
            if ( !parent || std::string("texture") != parent->name() )
            {
                throw structure_exception("Sprite must be a child element of a Texture object.");
            }

            config_key parent_key = key.remove_last_path_component();

            config_[key] = boost::shared_ptr<sprite_cfg>(
                    new sprite_cfg(*this, parent_key,
                            point(x, y), size(w, h), pivot, flip_x, flip_y));
        }

        void configuration::parse_animation(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            // duration: default duration for every frame
            //
            // can have:
            // pivot:    the pivot as 2d vector. defaults to 0,0
            // flip_x:   flip on x axis if true. don't flip if false (default)
            // flip_y:   flip on y axis if true. don't flip if false (default)

            float dur = *get_mandatory_attr<float>(*this, node, key, "duration",
                    parse_exception("Animation type must have 'duration' defined."));

            if ( config_.find(key) == config_.end() )
            {
                // No animation object exists! No frames defined?
                throw structure_exception("Animation has no frames. Invalid animation.");
            }
            else
            {
                glm::vec2 pivot = *get_attr<glm::vec2>(*this, node, key, "pivot", glm::vec2(0, 0));
                bool flip_x = *get_attr<bool>(*this, node, key, "flip_x", false);
                bool flip_y = *get_attr<bool>(*this, node, key, "flip_y", false);

                // Set duration
                static_cast<animation_cfg *>(&(*config_[key]))
                        ->set_duration(dur);

                // Set pivot
                static_cast<animation_cfg *>(&(*config_[key]))
                        ->set_pivot(pivot);

                // Set flip stuff
                static_cast<animation_cfg *>(&(*config_[key]))
                        ->set_flip_x(flip_x);
                static_cast<animation_cfg *>(&(*config_[key]))
                        ->set_flip_y(flip_y);
            }
        }

        void configuration::parse_frame(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            // sprite:   key for the sprite item to use
            // can have:
            // duration: duration for this frame

            std::string spr = *get_mandatory_key_attr(*this, node, key, "sprite",
                    parse_exception("Frame type must have 'sprite' defined."));

            // parent must be an animation
            xml_node *parent = node->parent();
            if ( !parent || std::string("animation") != parent->name() )
            {
                throw structure_exception("Frame must be a child element of an Animation object.");
            }

            config_key parent_key = key.remove_last_path_component();
            if ( config_.find(parent_key) == config_.end() )
            {
                // No animation exists yet. Must create one as this is the first frame.
                config_[parent_key] = boost::shared_ptr<animation_cfg>(
                        new animation_cfg(*this));
            }

            // custom duration?
            xml_attr *dur = node->first_attribute("duration");
            if ( dur )
            {
                // add this frame with custom duration
                static_cast<animation_cfg *>(&(*config_[parent_key]))->add(
                        frame_cfg(spr, value_parser<float>::parse(dur->value())));
            }
            else
            {
                // add this frame
                static_cast<animation_cfg *>(&(*config_[parent_key]))->add(frame_cfg(spr));
            }
        }

        void configuration::parse_font(xml_node *node, const config_key& key)
        {
            // must have:
            // n:          name of the element
            // size:       size of the font
            // base:       baseline
            // lineHeight: height of one line of text
            // scaleW:     width scale
            // scaleH:     height scale

            // can have:
            // stretchH:   height stretch. defaults to 0
            // padding:    padding (list of four ints). defaults to (0 0 0 0)
            // spacing:    spacing (list of two ints). defaults to (0 0)

            int sz = *get_mandatory_attr<int>(*this, node, key, "size",
                    parse_exception("Font type must have 'size' defined."));

            int baseline = *get_mandatory_attr<int>(*this, node, key, "base",
                    parse_exception("Font type must have 'base' defined."));

            int line_height = *get_mandatory_attr<int>(*this, node, key, "lineHeight",
                    parse_exception("Font type must have 'lineHeight' defined."));

            int scale_width = *get_mandatory_attr<int>(*this, node, key, "scaleW",
                    parse_exception("Font type must have 'scaleW' defined."));

            int scale_height = *get_mandatory_attr<int>(*this, node, key, "scaleH",
                    parse_exception("Font type must have 'scaleH' defined."));

            int stretch_height = *get_attr<int>(*this, node, key, "stretchH", 0);

            glm::vec4 padding = *get_attr<glm::vec4>(*this, node, key, "padding", glm::vec4(0, 0, 0, 0));

            size spacing = *get_attr<size>(*this, node, key, "spacing", size(0.0f, 0.0f));

            boost::shared_ptr<font_cfg> cfg =
                    boost::shared_ptr<font_cfg>(
                            new font_cfg(*this, sz, baseline, line_height,
                                    scale_width, scale_height, stretch_height,
                                    padding, spacing));

            // now parse the pages
            //
            xml_node *pages = node->first_node("pages");
            if ( !pages )
            {
                throw structure_exception("Font type must have 'pages' defined as child node.");
            }

            int pages_cnt = *get_mandatory_attr<int>(*this, pages, key, "count",
                    parse_exception("Pages of Font type must define 'count'."));

            std::vector<std::string> pages_vec;
            pages_vec.reserve(pages_cnt);

            for ( xml_node *page = pages->first_node(); page; page = page->next_sibling() )
            {
                if ( page->type() == rx::node_element && page->name() == std::string("page") )
                {
                    LOG("-- Found page");
                    int page_id = *get_mandatory_attr<int>(*this, page, key, "id",
                            parse_exception("Page type must define 'id'."));

                    std::string page_texture_key = *get_mandatory_key_attr(*this, page, key, "texture",
                            parse_exception("Page type must define 'texture'."));

                    LOG("Page id: %d, texture key: %s", page_id, page_texture_key.c_str());
                    pages_vec.push_back(page_texture_key); // page id is the index
                }
            }

            cfg->set_pages(pages_vec);

            // parse all glyphs now
            //
            xml_node *glyphs = node->first_node("glyphs");
            if ( !glyphs )
            {
                throw structure_exception("Font type must have 'glyphs' defined as child node.");
            }

            int glyphs_cnt = *get_mandatory_attr<int>(*this, glyphs, key, "count",
                    parse_exception("Glyphs of Font type must define 'count'."));

            std::vector<glyph> glyphs_vec;
            glyphs_vec.reserve(glyphs_cnt);

            for ( xml_node *gl = glyphs->first_node(); gl; gl = gl->next_sibling() )
            {
                if ( gl->type() == rx::node_element && gl->name() == std::string("glyph") )
                {
                    LOG("-- Found glyph");
                    int glyph_id = *get_mandatory_attr<int>(*this, gl, key, "id",
                            parse_exception("Glyph type must define 'id'."));

                    short x = *get_mandatory_attr<short>(*this, gl, key, "x",
                            parse_exception("Glyph type must define 'x'."));

                    short y = *get_mandatory_attr<short>(*this, gl, key, "y",
                            parse_exception("Glyph type must define 'y'."));

                    short width = *get_mandatory_attr<short>(*this, gl, key, "width",
                            parse_exception("Glyph type must define 'width'."));

                    short height = *get_mandatory_attr<short>(*this, gl, key, "height",
                            parse_exception("Glyph type must define 'height'."));

                    short x_offset = *get_mandatory_attr<short>(*this, gl, key, "xoffset",
                            parse_exception("Glyph type must define 'xoffset'."));

                    short y_offset = *get_mandatory_attr<short>(*this, gl, key, "yoffset",
                            parse_exception("Glyph type must define 'yoffset'."));

                    short x_advance = *get_mandatory_attr<short>(*this, gl, key, "xadvance",
                            parse_exception("Glyph type must define 'xadvance'."));

                    short page_id = *get_mandatory_attr<short>(*this, gl, key, "page",
                            parse_exception("Glyph type must define 'page'."));

                    std::string letter = *get_mandatory_attr<std::string>(*this, gl, key, "letter",
                            parse_exception("Glyph type must define 'letter'."));

                    glyphs_vec.push_back( glyph(glyph_id, x, y, width, height, x_offset, y_offset, x_advance, page_id, letter) );

                    LOG("Glyph id: %d", glyph_id);
                }
            }

            cfg->set_glyphs(glyphs_vec);
            
            // parse all kernings if available
            //
            xml_node *kerns = node->first_node("kernings");
            if ( kerns )
            {
                kern_map kernings;
            
                for ( xml_node *kern = kerns->first_node(); kern; kern = kern->next_sibling() )
                {
                    if ( kern->type() == rx::node_element && kern->name() == std::string("kerning") )
                    {
                        // <kerning first="32" second="84" amount="-4" />
                        int first = *get_mandatory_attr<int>(*this, kern, key, "first",
                            parse_exception("Kerning type must define 'first'."));
                        
                        int second = *get_mandatory_attr<int>(*this, kern, key, "second",
                            parse_exception("Kerning type must define 'second'."));
                        
                        int amount = *get_mandatory_attr<int>(*this, kern, key, "amount",
                            parse_exception("Kerning type must define 'amount'."));
                        
                        kern_map::iterator it = kernings.find(first);
                        if (it != kernings.end())
                        {
                            it->second.insert(std::make_pair(second, amount));
                        }
                        else
                        {
                            std::map<int, int> mp;
                            mp.insert(std::make_pair(second, amount));
                            kernings.insert(std::make_pair(first, mp));
                        }
                    }
                }
                
                cfg->set_kernings(kernings);
            }
            
            config_[key] = cfg;
        }

        void configuration::parse_music(xml_node *node, const config_key& key)
        {
            // must have:
            // n:          name of the element
            // path:       path to resource
            //
            // can have:
            // loop:       loop the music? true or false. false is default.
            // volume:     defaults to 1.0 - full gain
            // fadein:     fade in time given in seconds. defaults to 0.0 - no fade in
            // fadeout:    fade out time given in seconds. defaults to 0.0 - no fade out
            // start:      start of the track given in seconds (seek)
            // end:        end of the track given in seconds

            std::string path = *get_mandatory_attr<std::string>(*this, node, key, "path",
                    parse_exception("Music type must have 'path' defined."));

            bool loop = *get_attr<bool>(*this, node, key, "loop", false);
            float gain = *get_attr<float>(*this, node, key, "volume", 1.0f);

            float fade_in = *get_attr<float>(*this, node, key, "fadein", 0.0f);
            float fade_out = *get_attr<float>(*this, node, key, "fadeout", 0.0f);

            float start = *get_attr<float>(*this, node, key, "start", 0.0f);
            float end = *get_attr<float>(*this, node, key, "end", 0.0f);

            config_[key] =
                    boost::shared_ptr<music_cfg>(
                            new music_cfg(res_man_, kernel_, path, loop, gain, fade_in, fade_out, start, end));

        }

        void configuration::parse_sfx(xml_node *node, const config_key& key)
        {
            // must have:
            // n:          name of the element
            // path:       path to resource
            //
            // can have:
            // pitch:      pitch for the effect. defaults to 1.0f
            // loop:       loop the sfx? false is default.

            std::string path = *get_mandatory_attr<std::string>(*this, node, key, "path",
                    parse_exception("Sfx type must have 'path' defined."));

            bool loop = *get_attr<bool>(*this, node, key, "loop", false);
            value<float> pitch = get_attr<float>(*this, node, key, "pitch", 1.0f);

            config_[key] =
                    boost::shared_ptr<sfx_cfg>(
                            new sfx_cfg(*this, path, loop, pitch));

        }

        void configuration::parse_camera(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // can have:
            // parent:   the parent camera
            // rotation: initial rotation (defaults to 0.0)
            // zoom:     initial zoom (defaults to 1.0)
            // position: initial position (defaults to 0,0)
            // frustum:  frustum of this camera (defaults to size of screen)

            device_capabilities caps;
            size def_frustum(
                    caps.has_retina ? caps.display_size.width * 2 : caps.display_size.width,
                    caps.has_retina ? caps.display_size.height * 2 : caps.display_size.height);
            size frustum = *get_attr<size>(*this, node, key, "frustum", def_frustum);

            std::string parent = *get_key_attr(*this, node, key, "parent", std::string());

            config_[key] =
                    boost::shared_ptr<camera_cfg>(
                            new camera_cfg(*this, parent, frustum,
                                    get_attr<float>(*this, node, key, "rotation", 0.0f),
                                    get_attr<float>(*this, node, key, "zoom", 1.0f),
                                    get_attr<glm::vec2>(*this, node, key, "position", glm::vec2(0.0f, 0.0f))));
        }

        void configuration::parse_viewport(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            // camera:   camera to use with this viewport
            //
            // can have:
            // box:      position and size of this viewport. defaults to fullscreen
            // bgcolor:  background color. defaults to black.

            device_capabilities caps;
            rect def_box(0, 0,
                    caps.has_retina ? caps.display_size.width * 2 : caps.display_size.width,
                    caps.has_retina ? caps.display_size.height * 2 : caps.display_size.height);
            rect box = *get_attr<rect>(*this, node, key, "box", def_box);

            std::string cam = *get_mandatory_key_attr(*this, node, key, "camera",
                    parse_exception("Viewport type must have 'camera' defined."));

            color_info ci = *get_attr<color_info>(*this, node, key,
                    "bgcolor", color_info(0.0f, 0.0f, 0.0f));

            boost::shared_ptr<viewport_cfg> vp =
                    boost::shared_ptr<viewport_cfg>(new viewport_cfg(
                            *this, box, cam, ci));

            config_[key] = vp;

            // and add this viewport to the kernel
            kernel_.add_viewport(vp->get());
        }

        void configuration::parse_input(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // can have:
            // touch:           boolean; enable/disable touch input. defaults to true
            // accelerometer:   boolean; enable/disable accelerometer input. defaults to false
            // keyboard:        boolean; enable/disable keyboard input. defaults to true; available only on MacOSX

            bool want_touch = *get_attr<bool>(*this, node, key, "touch", true);
            bool want_accel = *get_attr<bool>(*this, node, key, "accelerometer", false);
            bool want_keyboard = *get_attr<bool>(*this, node, key, "keyboard", true);

            boost::shared_ptr<input_cfg> inp =
                    boost::shared_ptr<input_cfg>(new input_cfg(
                            kernel_, want_touch, want_accel, want_keyboard));

            config_[key] = inp;

            // and bind this input manager with the kernel
            kernel_.set_input_manager(inp->get());
        }

        void configuration::parse_keyboard(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // can have:
            // default:   boolean; specifies whether this is the default mapping. defaults to false
            
            bool def = *get_attr<bool>(*this, node, key, "default", false);
            
            if ( config_.find(key) == config_.end() )
            {
                // No keyboard object exists! No keys defined?
                throw structure_exception("Keyboard mapping has no keys defined. Invalid keyboard definition.");
            }
            
            // and bind this keyboard mapping to the kernel if it's default
            if(def)
            {
                kernel_.set_keyboard_mapping(static_cast<keyboard_cfg *>(&(*config_[key]))->get());
            }
        }
        
        void configuration::parse_key(xml_node *node, const config_key& key)
        {
            // parent must be a keyboard
            xml_node *parent = node->parent();
            if ( !parent || std::string("keyboard") != parent->name() )
            {
                throw structure_exception("Key must be a child element of a Keyboard object.");
            }
            
            config_key parent_key = key.remove_last_path_component();
            if ( config_.find(parent_key) == config_.end() )
            {
                // No keyboard mapping exists yet
                config_[parent_key] =
                    boost::shared_ptr<keyboard_cfg>(
                        new keyboard_cfg(kernel_));
            }
            
            // TODO: create special parser for keys
            std::string k =
                *get_mandatory_attr<std::string>(*this, node, key, "key",
                    parse_exception("Key type must have 'key' defined."));
            
            std::string name =
                *get_mandatory_attr<std::string>(*this, node, key, "n",
                    parse_exception("Key type must have 'n' defined."));
            
            static_cast<keyboard_cfg *>(&(*config_[parent_key]))->add(k, name);
        }
        
        void configuration::parse_spawner(xml_node *node, const config_key& key)
        {
            // must have:
            // n:          name of the element
            // objects:    list of objects to spawn
            //
            // can have:
            // wave_size:  size of one wave. count of objects to spawn at once
            // wave_delay: delay between automatic spawns

            list_value<std::string> obj_vec =
                    get_mandatory_list_attr<std::string>(*this, node, key, "objects",
                            parse_exception("Spawner type must have 'objects' defined."));

            glm::vec3 position = *get_attr<glm::vec3>(*this, node, key, "position", glm::vec3(0, 0, 0));
            value<int> wave_size = get_attr<int>(*this, node, key, "wave_size", 1);
            value<float> wave_delay = get_attr<float>(*this, node, key, "wave_delay", 0.0f);

            config_[key] = boost::shared_ptr<spawner_cfg>(new spawner_cfg(*this, obj_vec, position, wave_size, wave_delay));
        }

        void configuration::parse_emitter(xml_node *node, const config_key& key)
        {
            // must have:
            // n:          name of the element
            // type:       integer, type of the emitter. 0 = gravity, 1 = radial
            // texture:    key for the texture to use when drawing particles
            //
            // can have:
            // position, vec2
            // speed, float
            // life_span, float
            // angle, float
            // gravity, vec2
            // radial_acceleration, float
            // tangential_acceleration, float
            // start_color, color_info
            // finish_color, color_info
            // max_particles, int
            // duration, float
            // blend_src, int
            // blend_dst, int
            // start_size, float
            // finish_size, float
            // max_radius, float
            // min_radius, float
            // radius_speed, float
            // rotate_per_second, float
            // rotation_start, float
            // rotation_end, float

            boost::shared_ptr<emitter_cfg> c(new emitter_cfg(*this));

            c->type_ = *get_mandatory_attr<int>(*this, node, key, "type",
                    parse_exception("Emitter must have 'type' defined."));
            if ( c->type_ != 0 && c->type_ != 1 )
            {
                throw structure_exception("Emitter type must be either 0 or 1. See documentation for more info.");
            }

            c->texture_ = *get_mandatory_key_attr(*this, node, key, "texture",
                    parse_exception("Emitter must have 'texture' defined."));

            c->position_ = get_attr<glm::vec2>(*this, node, key, "position", glm::vec2(0.0f, 0.0f));
            c->speed_ = get_attr<float>(*this, node, key, "speed", 0.0f);
            c->life_span_ = get_attr<float>(*this, node, key, "life_span", 0.0f);
            c->angle_ = get_attr<float>(*this, node, key, "angle", 0.0f);
            c->gravity_ = get_attr<glm::vec2>(*this, node, key, "gravity", glm::vec2(0.0f, 0.0f));
            c->radial_acceleration_ = get_attr<float>(*this, node, key, "radial_acceleration", 0.0f);
            c->tangential_acceleration_ = get_attr<float>(*this, node, key, "tangential_acceleration", 0.0f);
            c->start_color_ = get_attr<color_info>(*this, node, key, "start_color", color_info(0.0f, 0.0f, 0.0f, 0.0f));
            c->finish_color_ = get_attr<color_info>(*this, node, key, "finish_color", color_info(0.0f, 0.0f, 0.0f, 0.0f));
            c->max_particles_ = *get_attr<int>(*this, node, key, "max_particles", 0);
            c->duration_ = *get_attr<float>(*this, node, key, "duration", 0.0f);
            c->blend_src_ = *get_attr<int>(*this, node, key, "blend_src", 0);
            c->blend_dst_ = *get_attr<int>(*this, node, key, "blend_dst", 0);
            c->start_size_ = get_attr<float>(*this, node, key, "start_size", 0.0f);
            c->finish_size_ = get_attr<float>(*this, node, key, "finish_size", 0.0f);
            c->min_radius_ = get_attr<float>(*this, node, key, "min_radius", 0.0f);
            c->max_radius_ = get_attr<float>(*this, node, key, "max_radius", 0.0f);
            c->radius_speed_ = get_attr<float>(*this, node, key, "radius_speed", 0.0f);
            c->rotate_per_second_ = get_attr<float>(*this, node, key, "rotate_per_second", 0.0f);
            c->rotation_start_ = get_attr<float>(*this, node, key, "rotation_start", 0.0f);
            c->rotation_end_ = get_attr<float>(*this, node, key, "rotation_end", 0.0f);

            config_[key] = c;
        }

        void configuration::parse_context(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element

            config_[key] = boost::shared_ptr<context_cfg>(new context_cfg());
        }

        void configuration::parse_zone(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // must have one of:
            // rect:     specify a rectangular zone. rect given as 4 ints separated by a space
            //
            // can have:
            // contexts: list of contexts separated by space. if empty or '*' then all available contexts are matched

            // currently it's mandatory
            rect box = *get_mandatory_attr<rect>(*this, node, key, "rect",
                    parse_exception("Zone type must have 'rect' defined (format: 'x y width height')."));

            // get vector of strings out of string ctxs
            std::vector<std::string> ctx_vec = get_list_attr<std::string>(*this, node, key, "contexts", "*").get_list();

            LOG("Parsed %d contexts from zone.contexts string.", ctx_vec.size());
            config_[key] = boost::shared_ptr<zone_cfg>(new zone_cfg(*this, box, ctx_vec));
        }

        void configuration::parse_body(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // can have:
            // type:     body type (static, dynamic, kinematic). defaults to dynamic
            // bullet:   boolean. defaults to false
            // fixed_rotation:   boolean. defaults to false
            // linear_damping:   float. linear damping for the body
            // pivot:            vector2d (x, y). anchor point for relative offsets
            //

            body_type btype = DYNAMIC_BODY;
            xml_attr *tp = node->first_attribute("type");
            if ( tp )
            {
                btype = value_parser<body_type>::parse(tp->value());
            }

            bool bullet = *get_attr<bool>(*this, node, key, "bullet", false);
            bool fix_rot = *get_attr<bool>(*this, node, key, "fixed_rotation", false);
            float lin_d = *get_attr<float>(*this, node, key, "linear_damping", 0.0f);
            glm::vec2 pivot = *get_attr<glm::vec2>(*this, node, key, "pivot", glm::vec2(0.0f, 0.0f));
            
            if ( config_.find(key) == config_.end() )
            {
                // No body object exists! No parts defined?
                throw structure_exception("Body has no parts. Invalid body definition.");
            }
            else
            {
                // Set dynamic, bullet and fixed_rotation
                static_cast<body_cfg *>(&(*config_[key]))
                        ->set_type(btype);
                static_cast<body_cfg *>(&(*config_[key]))
                        ->set_bullet(bullet);
                static_cast<body_cfg *>(&(*config_[key]))
                        ->set_fixed_rotation(fix_rot);
                static_cast<body_cfg *>(&(*config_[key]))
                        ->set_linear_damping(lin_d);
                static_cast<body_cfg *>(&(*config_[key]))
                        ->set_pivot(pivot);
            }
        }

        boost::shared_ptr<body_part_cfg> configuration::parse_body_part_common(xml_node *node, const config_key& key)
        {
            // parent must be a body
            xml_node *parent = node->parent();
            if ( !parent || std::string("body") != parent->name() )
            {
                throw structure_exception("BodyPart must be a child element of a Body object.");
            }

            config_key parent_key = key.remove_last_path_component();
            if ( config_.find(parent_key) == config_.end() )
            {
                // No body exists yet
                config_[parent_key] =
                    boost::shared_ptr<body_cfg>(
                        new body_cfg(*this, kernel_, DYNAMIC_BODY,
                            false, false, 0.0f, glm::vec2(0.0f, 0.0f)));
            }

            float density =
                    *get_attr<float>(*this, node, key, "density", 1.0f);

            float restitution =
                    *get_attr<float>(*this, node, key, "restitution", 0.0f);

            float friction =
                    *get_attr<float>(*this, node, key, "friction", 0.1f);

            uint16_t mask =
                    *get_attr<uint16_t >(*this, node, key, "mask", 0x0000);

            uint16_t category =
                    *get_attr<uint16_t >(*this, node, key, "category", 0x0000);

            bool sensor =
                    *get_attr<bool>(*this, node, key, "sensor", false);

            boost::shared_ptr<body_part_cfg> part =
                boost::shared_ptr<body_part_cfg>(new body_part_cfg(*this, kernel_,
                    key.last_path_component(), density, restitution, friction, mask, category, sensor));

            config_[key] = part;
            static_cast<body_cfg *>(&(*config_[parent_key]))->add(key);

            return part;
        }

        void configuration::parse_body_part_box(xml_node *node, const config_key& key)
        {
            // NOTE: this element must appear nested in a <body> element
            //
            // must have:
            // n:        name of the element

            boost::shared_ptr<body_part_cfg> part =
                    parse_body_part_common(node, key);

            size bottom_left =
                    *get_attr<size>(*this, node, key, "bottom_left",
                            size(-1.0f, -1.0f));

            size top_right =
                    *get_attr<size>(*this, node, key, "top_right",
                            size(1.0f, 1.0f));

            part->bottom_left(bottom_left);
            part->top_right(top_right);
            part->type(BOX_TYPE);
        }

        void configuration::parse_body_part_circle(xml_node *node, const config_key& key)
        {
            // NOTE: this element must appear nested in a <body> element
            //
            // must have:
            // n:        name of the element
            //
            // can have:
            // radius:   radius of the circle

            boost::shared_ptr<body_part_cfg> part =
                parse_body_part_common(node, key);

            float radius =
                *get_attr<float>(*this, node, key, "radius", 0.0f);

            float x = *get_attr<float>(*this, node, key, "x", 0.0f);
            float y = *get_attr<float>(*this, node, key, "y", 0.0f);

            part->radius(radius);
            part->set_x(x);
            part->set_y(y);
            
            part->type(CIRCLE_TYPE);
        }

        void configuration::parse_body_part_polygon(xml_node *node, const config_key& key)
        {
            // NOTE: this element must appear nested in a <body> element
            //
            // must have:
            // n:        name of the element
            
            boost::shared_ptr<body_part_cfg> part =
                parse_body_part_common(node, key);
            
            std::vector<float> polygon =
                get_mandatory_list_attr<float>(*this, node, key, "polygon",
                    structure_exception("Polygon BodyPart must specify 'polygon'.")).get_list();
            
            part->polygon(polygon);
            part->type(POLYGON_TYPE);
        }
        
        void configuration::parse_script(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // can have:
            // ref:   reference another script object (config key)
            // path:  resource path to load from
            // value: inline lua script
            //
            // or instead of ref, value and path it can contain inline lua code
            // as inner body of the script element.
            //
            // NOTE: only ref, value, path or inline code can be given at once
            // in the same script config.

            std::string path = *get_attr<std::string>(*this, node, key, "path", std::string());
            std::string ref = *get_key_attr(*this, node, key, "ref", std::string());

            xml_attr *value = node->first_attribute("value");
            if ( !value && !node->first_node() && path.empty() && ref.empty() )
            {
                throw structure_exception("Script must have either 'value', 'ref', 'path' or inner script defined.");
            }

            if ( !path.empty() )
            {
                if ( value || node->first_node() || !ref.empty() )
                {
                    throw structure_exception("Script can only have one of 'value', 'ref', 'path' or inner script defined.");
                }

                config_[key] = boost::shared_ptr<script_cfg>(new script_cfg(*this,
                        kernel_, "", "", path));
            }
            else if ( !ref.empty() )
            {
                if ( value || node->first_node() || !path.empty() )
                {
                    throw structure_exception("Script can only have one of 'value', 'ref', 'path' or inner script defined.");
                }

                config_[key] = boost::shared_ptr<script_cfg>(new script_cfg(*this,
                        kernel_, "", ref));
            }
            else if ( value )
            {
                if ( node->first_node() || !path.empty() || !ref.empty() )
                {
                    throw structure_exception("Script can only have one of 'value', 'ref', 'path' or inner script defined.");
                }

                config_[key] = boost::shared_ptr<script_cfg>(new script_cfg(*this,
                        kernel_, value_parser<std::string>::parse(value->value())));
            }
            else if ( node->first_node() )
            {
                xml_node *data = node->first_node();

                if ( data->type() == rx::node_data || data->type() == rx::node_cdata )
                {
                    config_[key] = boost::shared_ptr<script_cfg>(new script_cfg(*this,
                            kernel_, value_parser<std::string>::parse(data->value())));
                }
                else
                {
                    throw parse_exception("Data element of Script must be plain value or CDATA.");
                }
            }
        }

        void configuration::parse_listener(xml_node *node, const config_key& key)
        {
            // must have:
            // n:       name of the element
            // event:   event name to listen for
            //
            // can have:
            // script:  reference to a script object (config key)

            std::string ev = *get_mandatory_attr<std::string>(*this, node, key, "event",
                    structure_exception("Listener must have event defined."));
            std::string scr = *get_key_attr(*this, node, key, "script",
                    std::string());

            config_[key] = boost::shared_ptr<listener_cfg>(new listener_cfg(*this,
                    kernel_, ev, scr));
        }

        void configuration::parse_object(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // can have:
            // proto:     prototype for the object (config key pointing at another object)
            // parent:    parent object (inherit space - position, rotation, scale, etc.)
            // contexts:  list of contexts separated by space
            // position:  position vector2d
            // scale:     scale as float
            // rotation:  rotation as float angle in degrees
            // sprite:    sprite to draw
            // animation: animation to draw
            // bgcolor:   color to fill with (given as vector of four floats: r g b a)
            // camera:    the camera to use
            // space:     'world' (default), 'screen' or 'camera' space
            // parent_space: 'both' (default), 'position' or 'box'
            // spawner:   configuration key to attach a spawner
            // emitter:   configuration key to attach a particle system emitter
            // zone:      configuration key to attach a zone
            // box:       rectangle (can be filled with color etc.)
            // pivot:     the pivot as 2d vector. defaults to 0,0
            // font:      font to use when drawing text
            // text:      text to render. must have font specified to use this
            // align:     text alignment ('left', 'center' or 'right'. defaults to 'left')
            // body:      physics body
            // lifetime:  lifetime of the object
            //
            // scripts:
            // on_create:  executed right after object is instantiated
            // on_destroy: executed before object is destroyed

            object_traits tr, proto_tr;

            // first thing we do is create a new empty configuration for this object
            // unless it already exists there.
            // we will swap the traits later at the end of this function.
            if ( !exists(key) )
            {
                LOG("Object at '%s' does not exist. create it..", key.string().c_str());
                config_[key] =
                        boost::shared_ptr<object_cfg>(
                                new object_cfg(*this, std::string("scene") == node->name()));
            }
            else
            {
                // copy traits already created before (children)
                tr = static_cast<object_cfg *>(&(*config_[key]))->get_traits();
            }

            // check for prototype
            xml_attr *proto = node->first_attribute("proto");
            if ( proto )
            {
                std::string k = lookup_key(proto->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    LOG("Prototype detected.");
                    proto_tr = static_cast<object_cfg *>(&(*config_[k]))->get_traits();
                    tr.proto = k;
                }
                else
                {
                    throw structure_exception(
                            "Can't locate object prototype at key "
                                    + std::string(proto->value()));
                }
            }

            xml_attr *parent = node->first_attribute("parent");
            xml_node *parent_node = node->parent();

            // scene objects can't be nested within other scenes or objects
            if ( std::string("scene") == node->name() )
            {
                if ( parent || (parent_node &&
                        (std::string("object") == parent_node->name()
                                || std::string("scene") == parent_node->name())) )
                {
                    throw structure_exception(
                            "Scene with key '" + key.string() +
                                    "' attempts to define a parent. That's illigal.");
                }
            }

            if ( parent ) // explicit parent takes over if specified
            {
                std::string k = lookup_key(parent->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    static_cast<object_cfg *>(&(*config_[k]))->add(key);
                    tr.has_parent = true;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's parent is not found under key '")
                                    + parent->value() + "'.");
                }
            }
            else if ( parent_node &&
                    (std::string("object") == parent_node->name()
                            || std::string("scene") == parent_node->name()) )
            {
                config_key parent_key = key.remove_last_path_component();
                LOG("Object is nested in other object. Set implicit parent to '%s'", parent_key.string().c_str());

                if ( !exists(parent_key) )
                {
                    LOG("Parent does not exist yet. Create it now.");

                    // create an empty default object as the parent
                    config_[parent_key] =
                            boost::shared_ptr<object_cfg>(
                                    new object_cfg(*this, std::string("scene") == parent_node->name()));
                }

                static_cast<object_cfg *>(&(*config_[parent_key]))->add(key);
                tr.has_parent = true;
            }

            tr.name = *get_mandatory_attr<std::string>(*this, node, key, "n",
                    parse_exception("Object must have 'n' defined."));
            tr.path = key;

            // no contexts by default
            tr.contexts = get_list_attr<std::string>(*this, node, key, "contexts", "").get_list();

            tr.scale = get_attr<float>(*this, node, key, "scale", 1.0f);
            tr.rotation = get_attr<float>(*this, node, key, "rotation", 0.0f);
            tr.box = get_attr<size>(*this, node, key, "box", size());
            tr.bgcolor = *get_attr<color_info>(*this, node, key, "bgcolor", color_info(0.0f, 0.0f, 0.0f, 0.0f));
            tr.visible = get_attr<bool>(*this, node, key, "visible", true);
            tr.enabled = get_attr<bool>(*this, node, key, "enabled", true);
            tr.lifetime = get_attr<float>(*this, node, key, "lifetime", 0.0f);

            xml_attr *bgcolor = node->first_attribute("bgcolor");
            if ( bgcolor )
            {
                tr.has_bgcolor = true;
            }

            xml_attr *anim = node->first_attribute("animation");
            if ( anim )
            {
                std::string k = lookup_key(anim->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.animation = k;
                    tr.has_animation = true;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's animation is not found under key '")
                                    + anim->value() + "'.");
                }
            }

            xml_attr *spr = node->first_attribute("sprite");
            if ( spr )
            {
                std::string k = lookup_key(spr->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.sprite = k;
                    tr.has_sprite = true;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's sprite is not found under key '")
                                    + spr->value() + "'.");
                }
            }

            xml_attr *spw = node->first_attribute("spawner");
            if ( spw )
            {
                std::string k = lookup_key(spw->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.spawner = k;
                    tr.has_spawner = true;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's spawner is not found under key '")
                                    + spw->value() + "'.");
                }
            }

            xml_attr *em = node->first_attribute("emitter");
            if ( em )
            {
                std::string k = lookup_key(em->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.emitter = k;
                    tr.has_emitter = true;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's emitter is not found under key '")
                                    + em->value() + "'.");
                }
            }

            xml_attr *zn = node->first_attribute("zone");
            if ( zn )
            {
                std::string k = lookup_key(zn->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.zone = k;
                    tr.has_zone = true;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's zone is not found under key '")
                                    + zn->value() + "'.");
                }
            }

            xml_attr *cam = node->first_attribute("camera");
            if ( cam )
            {
                std::string k = lookup_key(cam->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.camera = k;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's camera is not found under key '")
                                    + cam->value() + "'.");
                }
            }

            xml_attr *sp = node->first_attribute("space");
            if ( sp )
            {
                tr.obj_space = value_parser<space>::parse(sp->value());
            }
            else
            {
                // scene is always in camera space
                if ( std::string("scene") == node->name() )
                {
                    tr.obj_space = CAMERA_SPACE;
                }
                else
                {
                    tr.obj_space = WORLD_SPACE;
                }
            }

            // camera or parent space
            if ( tr.obj_space == CAMERA_SPACE || tr.has_parent )
            {
                tr.par_space = PARENT_SPACE_BOTH;
                tr.pivot = get_attr<glm::vec2>(*this, node, key, "pivot", glm::vec2(0.5, 0.5));
                tr.position = get_attr<glm::vec3>(*this, node, key, "position", glm::vec3(0.5f, 0.5f, 0.0f));
            }
            else
            {
                tr.par_space = PARENT_SPACE_NONE;
                tr.pivot = get_attr<glm::vec2>(*this, node, key, "pivot", glm::vec2(0, 0));
                tr.position = get_attr<glm::vec3>(*this, node, key, "position", glm::vec3(0.0f, 0.0f, 0.0f));
            }

            xml_attr *psp = node->first_attribute("parent_space");
            if ( psp )
            {
                tr.par_space = value_parser<parent_space>::parse(psp->value());
            }

            if ( tr.par_space == PARENT_SPACE_POSITION )
            {
                // don't use the default pivot at 0.5,0.5 - use 0.0,0.0
                tr.pivot = get_attr<glm::vec2>(*this, node, key, "pivot", glm::vec2(0, 0));
            }

            xml_attr *align = node->first_attribute("align");
            if ( align )
            {
                tr.align = value_parser<alignment>::parse(align->value());
            }
            else
            {
                tr.align = LEFT_ALIGN;
            }

            xml_attr *fnt = node->first_attribute("font");
            if ( fnt )
            {
                std::string k = lookup_key(fnt->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.font = k;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's font is not found under key '")
                                    + fnt->value() + "'.");
                }
            }

            xml_attr *text = node->first_attribute("text");
            if ( text )
            {
                if ( !fnt )
                {
                    throw structure_exception("Text is specified but Font is not. Can't construct object.");
                }

                tr.text = text->value();
                tr.has_text = true;
            }

            xml_attr *body = node->first_attribute("body");
            if ( body )
            {
                std::string k = lookup_key(body->value(), key.remove_last_path_component());

                if ( k.empty() == false )
                {
                    tr.body = k;
                    tr.has_body = true;
                }
                else
                {
                    throw structure_exception(
                            std::string("Object's body is not found under key '")
                                    + body->value() + "'.");
                }
            }

            // scripts
            if ( exists(key / "on_create") )
            {
                tr.on_create = key / "on_create";
            }
            if ( exists(key / "on_destroy") )
            {
                tr.on_destroy = key / "on_destroy";
            }
            if ( exists(key / "on_input_began") )
            {
                tr.on_input_began = key / "on_input_began";
            }
            if ( exists(key / "on_input_moved") )
            {
                tr.on_input_moved = key / "on_input_moved";
            }
            if ( exists(key / "on_input_ended") )
            {
                tr.on_input_ended = key / "on_input_ended";
            }

            // merge overrides into prototype
            if ( proto )
            {
                proto_tr.merge(tr);
            }
            else
            {
                proto_tr = tr;
            }

            static_cast<object_cfg *>(&(*config_[key]))->set_traits(proto_tr);
        }

        void configuration::parse_scene(xml_node *node, const config_key& key)
        {
            // must have:
            // n:        name of the element
            //
            // can have:
            // because a scene is really an object, everything what can be defined for object can also be defined for scene.
            //
            // scripts:
            // on_transition_to:  executed when transition to this scene is requested
            // on_transition_from: executed when transition to another scene is requested

            // fill in with default properties of scene objects
            object_traits tr;
            tr.box.set(size(1.0f, 1.0f)); // full-viewport
            tr.position.set(glm::vec3(0.5f, 0.5f, 0.0f));
            tr.pivot.set(glm::vec2(0.5f, 0.5f));

            if ( exists(key / "on_transition_to") )
            {
                tr.on_transition_to = key / "on_transition_to";
            }
            if ( exists(key / "on_transition_from") )
            {
                tr.on_transition_from = key / "on_transition_from";
            }

            // scene is really just an alias for object plus a little extra
            parse_object(node, key);

            // updated the traits with scene defaults if they were not set thru xml
            object_traits obj_tr = static_cast<object_cfg *>(&(*config_[key]))->get_traits();

            tr.merge(obj_tr);
            tr.obj_space = CAMERA_SPACE; // scene is always in camera space
            tr.par_space = PARENT_SPACE_BOTH;

            static_cast<object_cfg *>(&(*config_[key]))->set_traits(tr);
        }

    } // namespace config
} // namespace x2d