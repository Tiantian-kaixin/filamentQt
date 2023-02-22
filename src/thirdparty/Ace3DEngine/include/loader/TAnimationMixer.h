#pragma once
#include <vector>
#include "loader/TDefine.h"

namespace T3D {
    class TObject3D;
    class TAnimationAction;
    class TAnimationClip;

    class TAnimationMixer {
    public:
        explicit TAnimationMixer(TObject3D* root);
        ~TAnimationMixer();

        void update(float deltaTime);

    public:
        TAnimationAction* clipAction(TAnimationClip* clip, TObject3D* optionalRoot, int from = -1, int to = -1);

    public:
        inline TObject3D* getRootObject() const { return _root; }

        void removeAction(TAnimationAction* action);

    protected:
        TObject3D*      _root = nullptr;
        int             _accuIndex = 0;
        float           _time = 0.0f;
        float           _timeScale = 1.0f;

        int                             _activeActions = 0;
        std::vector<TAnimationAction*>  _actions;
    };
}
