include $(CLEAR_VARS)
LOCAL_MODULE    := box2d

LOCAL_C_INCLUDES := $(LOCAL_PATH)/x2d/libs/box2d
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/x2d/libs/box2d

LOCAL_SRC_FILES := \
   	x2d/libs/box2d/Box2D/Collision/b2BroadPhase.cpp \
	x2d/libs/box2d/Box2D/Collision/b2CollideCircle.cpp \
	x2d/libs/box2d/Box2D/Collision/b2CollideEdge.cpp \
	x2d/libs/box2d/Box2D/Collision/b2CollidePolygon.cpp \
	x2d/libs/box2d/Box2D/Collision/b2Collision.cpp \
	x2d/libs/box2d/Box2D/Collision/b2Distance.cpp \
	x2d/libs/box2d/Box2D/Collision/b2DynamicTree.cpp \
	x2d/libs/box2d/Box2D/Collision/b2TimeOfImpact.cpp \
	x2d/libs/box2d/Box2D/Collision/Shapes/b2ChainShape.cpp \
	x2d/libs/box2d/Box2D/Collision/Shapes/b2CircleShape.cpp \
	x2d/libs/box2d/Box2D/Collision/Shapes/b2EdgeShape.cpp \
	x2d/libs/box2d/Box2D/Collision/Shapes/b2PolygonShape.cpp \
	x2d/libs/box2d/Box2D/Common/b2BlockAllocator.cpp \
	x2d/libs/box2d/Box2D/Common/b2Draw.cpp \
	x2d/libs/box2d/Box2D/Common/b2Math.cpp \
	x2d/libs/box2d/Box2D/Common/b2Settings.cpp \
	x2d/libs/box2d/Box2D/Common/b2StackAllocator.cpp \
	x2d/libs/box2d/Box2D/Common/b2Timer.cpp \
	x2d/libs/box2d/Box2D/Dynamics/b2Body.cpp \
	x2d/libs/box2d/Box2D/Dynamics/b2ContactManager.cpp \
	x2d/libs/box2d/Box2D/Dynamics/b2Fixture.cpp \
	x2d/libs/box2d/Box2D/Dynamics/b2Island.cpp \
	x2d/libs/box2d/Box2D/Dynamics/b2World.cpp \
	x2d/libs/box2d/Box2D/Dynamics/b2WorldCallbacks.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2Contact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2GearJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2Joint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
	x2d/libs/box2d/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
	x2d/libs/box2d/Box2D/Rope/b2Rope.cpp	

include $(BUILD_STATIC_LIBRARY)
