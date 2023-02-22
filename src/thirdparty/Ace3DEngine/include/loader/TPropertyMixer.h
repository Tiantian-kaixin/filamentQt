#pragma once
#include "loader/TDefine.h"

namespace T3D {
    class TPropertyBinding;
    class TKeyframeTrack;
    class TObject3D;
    class TInterpolant;

    class TPropertyMixer {
    public:
        TPropertyMixer(TObject3D* root, TKeyframeTrack* track, int fromIndex = -1, int toIndex = -1);
        ~TPropertyMixer();

    public:
        void accumulate(float clampTime, bool resetKeyframeIndex, int accuIndex, float weight);
        void apply(int accuIndex);

    public:
        void saveOriginalState();
        void restoreOriginState();

    protected:
        TObject3D*              _root = nullptr;
        TKeyframeTrack*         _track = nullptr;

        int                     _valueSize = 1;
        float                   _cumulativeWeight = 0.0f;

        float*                  _result = nullptr;
        TPropertyBinding*       _propertyBinding = nullptr;

        TInterpolant*           _interpolant = nullptr;
    };
}
