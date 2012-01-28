//
//  resource.h
//  x2d
//
//  Created by Alex Kremer on 1/27/12.
//  Copyright (c) 2012 godexsoft. All rights reserved.
//

#pragma once
#ifndef __X2D_RESOURCE_H__
#define __X2D_RESOURCE_H__

#include "platform.h"

namespace x2d {
namespace resource {
    
    using namespace platform::thread;
    
    template <typename T>
    class resource_ptr;
    
    template <typename T>
    class resource_weak_ptr
    {
    private:
        friend class resource_ptr<T>;
        
        // Shared data
        struct counters 
        {
            counter    sc;
            counter    wc;
        };
        
        static counters for_nulls;
        
        T          * obj_;  // Managed by resource_ptr
        counters   * cnt_;  // Managed by resource_weak_ptr
        
        void attach()
        {
            interlocked_increment(&cnt_->wc);
        }
        
        void detach()
        {
            if( interlocked_decrement(&cnt_->wc) == 0 ) 
            {
                delete cnt_;
            }
        }
        
        // Used by resource_ptr only
        resource_weak_ptr(T* p)
        : obj_(p)
        , cnt_( new counters )
        {
            cnt_->sc = 0;
            cnt_->wc = 0;
            attach();
        }
    public:
        resource_weak_ptr()
        : obj_(NULL)
        , cnt_(&for_nulls)
        {
            attach();
        }
        
        resource_weak_ptr(const resource_weak_ptr<T>& r)
        : obj_(r.obj_)
        , cnt_(r.cnt_)
        {
            attach();
        }
        
        void swap(resource_weak_ptr<T>& other)
        {
            std::swap(obj_, other.obj_);
            std::swap(cnt_, other.cnt_);
        }
        
        resource_weak_ptr<T>& operator= (const resource_weak_ptr<T>& r)
        {
            resource_weak_ptr<T> temp(r);
            swap(temp);
            return *this;
        }
        
        resource_weak_ptr(resource_ptr<T>& p)
        : obj_(p.ptr.obj_)
        , cnt_(p.ptr.cnt_)
        {
            attach();
        }
        
        ~resource_weak_ptr()
        {
            detach();
        }
        
        resource_ptr<T> lock()
        {
            counter c = interlocked_increment(&cnt_->sc);
            resource_ptr<T> result;
            if( c != 1 )
                result = resource_ptr<T>(obj_, cnt_);
            interlocked_decrement(&cnt_->sc);
            
            return result;
        }
    };
    
    template<typename T>
    typename resource_weak_ptr<T>::counters resource_weak_ptr<T>::for_nulls = {1,1};

    
    template<typename T>
    class resource_ptr
    {
        friend class resource_weak_ptr<T>;
        
    private:
        resource_weak_ptr<T>  ptr;
        
        void attach()
        {
            interlocked_increment(&ptr.cnt_->sc);
        }
        
        void detach()
        {
            if( interlocked_decrement(&ptr.cnt_->sc) == 0 ) {
                delete ptr.obj_;
                ptr.obj_ = 0;
            }
        }
        
        resource_ptr(T* p, typename resource_weak_ptr<T>::counters * c)
        {
            ptr.obj_ = p;
            ptr.cnt_ = c;
            attach();
        }
        
    public:
        resource_ptr()
        {
            attach();
        }
        explicit resource_ptr(T* p)
        : ptr(p)
        {
            attach();
        }
        
        resource_ptr(const resource_ptr<T>& r)
        : ptr(r.ptr)
        {
            attach();
        }
        
        ~resource_ptr()
        {
            detach();
        }
        
        bool isTheOne()const { return ptr.cnt_->sc; }
        void swap(resource_ptr<T>& other)
        {
            ptr.swap(other.ptr);
        }
        
        resource_ptr<T>& operator= (const resource_ptr<T>& r)
        {
            resource_ptr<T> temp(r);
            swap(temp);
            return *this;
        }
        
        inline T * get() const
        {
            return ptr.obj_;
        }
        
        inline T * operator-> () const
        {
            return ptr.obj_;
        }
        
        inline operator bool() const
        {
            return ptr.obj_ != 0;
        }

        inline bool operator < ( const resource_ptr<T>& other ) const
        {
            return ptr.obj_ < other.ptr.obj_;
        }
    };
    
    template<typename T>
    inline bool operator==(const resource_ptr<T>& a, const resource_ptr<T>& b )
    {
        return a.get() == b.get();
    }
    template<typename T>
    inline bool operator==(const resource_ptr<T>& a, T * b)
    {
        return a.get() == b;
    }
    template<typename T>
    inline bool operator==(T * a, const resource_ptr<T>& b)
    {
        return a == b.get();
    }
    
} // namespace resource    
} // namespace x2d
using namespace x2d::resource;

#endif // __X2D_RESOURCE_H__
