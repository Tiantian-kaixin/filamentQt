#pragma once
#include "UnityFloat3.h"

namespace UM {
    //*
    struct plane {
        float3  normal = { 0.0f, 1.0f, 0.0f };
        float   distance = 0.0f;

        plane();
        plane(float3 inNormal, float3 inPoint);
        plane(float3 inNormal, float d);
        plane(float3 a, float3 b, float3 c);

        // Sets a plane using a point that lies within it plus a normal to orient it (note that the normal must be a normalized vector).
        void setNormalAndPosition(float3 inNormal, float3 inPoint);

        // Sets a plane using three points that lie within it.  The points go around clockwise as you look down on the top surface of the plane.
        void set3Points(float3 a, float3 b, float3 c);

        // Make the plane face the opposite direction
        void flip();

        // Return a version of the plane that faces the opposite direction
        plane flipped();

        // Translates the plane into a given direction
        void translate(float3 translation);

        // Creates a plane that's translated into a given direction
        static plane translate(plane plane, float3 translation);

        // Calculates the closest point on the plane.
        float3 closestPointOnPlane(float3 point);

        // Returns a signed distance from plane to point.
        float getDistanceToPoint(float3 point);

        // Is a point on the positive side of the plane?
        bool getSide(float3 point);

        // Are two points on the same side of the plane?
        bool sameSide(float3 inPt0, float3 inPt1);
    };
    // */
}
