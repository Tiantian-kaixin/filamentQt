#pragma once
#include <vector>
#include "loader/TDefine.h"

namespace T3D {
    class TObject3D;
    class TAnimationMixer;
    class TAnimationClip;
    class TPropertyMixer;

    class TAnimationAction {
    public:
        TAnimationAction(TAnimationMixer* mixer, TAnimationClip* clip, TObject3D* localRoot, int fromIndex = -1, int toIndex = -1);
        ~TAnimationAction();

    public:
        void play();
        void stop();
        void reset();

        bool isRunning();

        bool isScheduled();

        void startAt(float time) { _startTime = time; }

        void setLoop(T_LOOP_MODE mode, int loopCount = -1 /* Infinity */);

        void setEffectiveWeight(float weight);
        float getEffectiveWeight() const { return _effectiveWeight; }
        void setEffectiveTimeScale(float timeScale);
        float getEffectiveTimeScale() const { return _effectiveTimeScale; }

        void fadeIn(float duration);
        void fadeOut(float duration);

        void crossFadeFrom(TAnimationAction* fadeOutAction, float duration, bool warp);
        void crossFadeTo(TAnimationAction* fadeInAction, float duration, bool warp);

        void stopFading();

        void setDuration(float duration);

        void syncWith(TAnimationAction* action);
        void halt(float duration);
        void warp(float startTimeScale, float endTimeScale, float duration);
        void stopWarping();

    public:
        void update(float time, float deltaTime, int timeDirection, int accuIndex);
        void apply(int accuIndex);

    protected:
        float updateWeight(float time);
        float updateTimeScale(float time);
        float updateTime(float deltaTime);

    public:
        void resetAnimationMixer() { _mixer = nullptr; }
        void removeFromAnimationMixer();

    private:

    protected:

        TAnimationMixer*    _mixer = nullptr;
        TAnimationClip*     _clip = nullptr;
        TObject3D*          _root = nullptr;

        float           _time = 0.0f;
        float           _startTime = 0.0f;

        float           _duration = 0.0f;
        float           _clampTime = 0.0f;

        T_LOOP_MODE     _loopMode = T_LOOP_MODE::LOOP_REPEAT;
        int             _repetitions = -1 /* Infinity */;
        int             _loopCount = -1;

        float           _timeScale = 1.0f;
        float           _effectiveTimeScale = 1.0f;

        float           _weight = 1.0f;
        float           _effectiveWeight = 1.0f;

        bool            _active = false;
        bool            _paused = false;
        bool            _enabled = true;
        bool            _resetKeyframeIndex = false;

        std::vector<TPropertyMixer*>    _propertyMixers;
    };
}
