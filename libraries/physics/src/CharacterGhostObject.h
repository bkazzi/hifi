//
//  CharacterGhostObject.h
//  libraries/physcis/src
//
//  Created by Andrew Meadows 2016.08.26
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_CharacterGhostObject_h
#define hifi_CharacterGhostObject_h

#include <stdint.h>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <stdint.h>

#include "CharacterSweepResult.h"
#include "CharacterRayResult.h"

class CharacterGhostShape;

class CharacterGhostObject : public btPairCachingGhostObject {
public:
    CharacterGhostObject() { }
    ~CharacterGhostObject();

    void setCollisionGroupAndMask(int16_t group, int16_t mask);
    void getCollisionGroupAndMask(int16_t& group, int16_t& mask) const;

    void setRadiusAndHalfHeight(btScalar radius, btScalar halfHeight);
    void setUpDirection(const btVector3& up);
    void setMotorVelocity(const btVector3& velocity);
    void setGravity(btScalar gravity) { _gravity = gravity; } // NOTE: we expect _gravity to be negative (in _upDirection)
    void setMinWallAngle(btScalar angle) { _maxWallNormalUpComponent = cosf(angle); }
    void setMaxStepHeight(btScalar height) { _maxStepHeight = height; }

    const btVector3& getLinearVelocity() const { return _linearVelocity; }

    void setCharacterCapsule(btCapsuleShape* capsule);

    void setCollisionWorld(btCollisionWorld* world);

    void move(btScalar dt, btScalar overshoot);

    bool sweepTest(const btConvexShape* shape,
            const btTransform& start,
            const btTransform& end,
            CharacterSweepResult& result) const;

    bool isHovering() const { return _hovering; }

protected:
    void removeFromWorld();
    void addToWorld();

    bool rayTest(const btVector3& start,
            const btVector3& end,
            CharacterRayResult& result) const;

    bool resolvePenetration(int numTries);
    void refreshOverlappingPairCache();
    void updateVelocity(btScalar dt);
    void updateTraction(const btVector3& position);
    btScalar measureAvailableStepHeight() const;
    void updateHoverState(const btVector3& position);

protected:
    btVector3 _upDirection { 0.0f, 1.0f, 0.0f }; // input, up in world-frame
    btVector3 _motorVelocity { 0.0f, 0.0f, 0.0f }; // input, velocity character is trying to achieve
    btVector3 _linearVelocity { 0.0f, 0.0f, 0.0f }; // internal, actual character velocity
    btVector3 _floorNormal { 0.0f, 0.0f, 0.0f }; // internal, probable floor normal
    btVector3 _floorContact { 0.0f, 0.0f, 0.0f }; // internal, last floor contact point
    btCollisionWorld* _world { nullptr }; // input, pointer to world
    //btScalar _distanceToFeet { 0.0f }; // input, distance from object center to lowest point on shape
    btScalar _halfHeight { 0.0f };
    btScalar _radius { 0.0f };
    btScalar _motorSpeed { 0.0f }; // internal, cached for speed
    btScalar _gravity { 0.0f }; // input, amplitude of gravity along _upDirection (should be negative)
    btScalar _maxWallNormalUpComponent { 0.0f }; // input: max vertical component of wall normal
    btScalar _maxStepHeight { 0.0f }; // input, max step height the character can climb
    btCapsuleShape* _characterShape { nullptr }; // input, shape of character
    CharacterGhostShape* _ghostShape{ nullptr }; // internal, shape whose Aabb is used for overlap cache
    int16_t _collisionFilterGroup { 0 };
    int16_t _collisionFilterMask { 0 };
    bool _inWorld { false }; // internal, was added to world
    bool _hovering { false }; // internal,
    bool _onFloor { false }; // output, is actually standing on floor
    bool _hasFloor { false }; // output, has floor underneath to fall on
};

#endif // hifi_CharacterGhostObject_h