//
//  CharacterGhostShape.h
//  libraries/physcis/src
//
//  Created by Andrew Meadows 2016.09.14
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_CharacterGhostShape_h
#define hifi_CharacterGhostShape_h

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

class CharacterGhostShape : public btCapsuleShape {
    // Same as btCapsuleShape but reports an expanded Aabb for larger ghost overlap cache
public:
    CharacterGhostShape(btScalar radius, btScalar height) : btCapsuleShape(radius, height) {
    }

    virtual void getAabb (const btTransform& t, btVector3& aabbMin, btVector3& aabbMax) const override;
};

#endif // hifi_CharacterGhostShape_h