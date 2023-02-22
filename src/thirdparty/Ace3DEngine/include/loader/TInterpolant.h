#pragma once
#include "loader/TDefine.h"

namespace T3D {
    class TKeyframeTrack;

    class TInterpolant {
    public:
        TInterpolant(TKeyframeTrack* track, float* result, int fromIndex = -1, int toIndex = -1);
        virtual ~TInterpolant();

        virtual void evaluate(float time, bool resetKeyframeIndex, int accuIndex, float weight);

    protected:
        virtual void beforeStart(float time);
        virtual void afterEnd(float time);
        virtual void interpolate(int keyframeIndex0, int keyframeIndex1, float t0, float t, float t1);

        void copySampleValue(int index);

    protected:
        virtual void interpolateLinear(int keyframeIndex0, int keyframeIndex1, float t0, float t, float t1);
        virtual void interpolateQuaternionLinear(int keyframeIndex0, int keyframeIndex1, float t0, float t, float t1);

    public:
        inline T_INTERPOLANT_TYPE getInterpolantType() const { return _interpolantType; }

    protected:
        TKeyframeTrack*     _track = nullptr;
        float*              _result = nullptr;
        int                 _fromIndex = 0;
        int                 _toIndex = 0;

        int                 _valueSize = 1;
        T_INTERPOLANT_TYPE  _interpolantType;

        int                 _nextIndex;
    };
}
