#pragma once
#include <math.h>
#include <vector>
#include <algorithm>

namespace T3D {
    /*
    // 暂时没有这个动画曲线啥事
    enum WrapMode {
        // Must be kept in sync with WrapMode in InternalUtility.bindings                                                                                                            
        kWrapModeDefault = 0,
        kWrapModeClamp = 1 << 0,
        kWrapModeRepeat = 1 << 1,
        kWrapModePingPong = 1 << 2,
        kWrapModeClampForever = 1 << 3                                                                                                                                               
    };

    enum InternalWrapMode {
        // Values are serialized in CompressedMesh and must be preserved                                                                                                             
        kInternalWrapModePingPong = 0,                                                                                                                                               
        kInternalWrapModeRepeat = 1,                                                                                                                                                 
        kInternalWrapModeClamp = 2,                                                                                                                                                  
        kInternalWrapModeDefault = 3                                                                                                                                                 
    };

    enum WeightedMode {
        kNotWeighted = 0,
        kInWeighted = 1 << 0,
        kOutWeighted = 1 << 1,
        kBothWeighted = kInWeighted | kOutWeighted
    };

    // This Enum needs to stay synchronized with the one in the bindings Runtime\Export\Transform.bindings                                                                       
    enum RotationOrder {
        kOrderXYZ,
        kOrderXZY,
        kOrderYZX,
        kOrderYXZ,
        kOrderZXY,
        kOrderZYX,
        kRotationOrderLast = kOrderZYX,
        kOrderUnityDefault = kOrderZXY
    };

    const int kRotationOrderCount = kRotationOrderLast + 1;

    ///////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    static T Zero() { return T(); }
    
    #define kDefaultWeight 1.0F / 3.0F

    template<class T>
    static T DefaultWeight() { return kDefaultWeight; }
                
    //template<>  
    //inline Quaternionf Zero<Quaternionf>() { return Quaternionf(0.0F, 0.0F, 0.0F, 0.0F); }
                
    //template<>
    //inline Vector3f Zero<Vector3f>() { return Vector3f(0.0F, 0.0F, 0.0F); }

    // Returns float remainder for t / length
    inline float Repeat(float t, float length) {
        return t - floor(t / length) * length;
    }
    
    // Returns double remainder for t / length
    inline double RepeatD(double t, double length) {
        return t - floor(t / length) * length;
    }

    inline float PingPong(float t, float length) {
        t = Repeat(t, length * 2.0F);
        t = length - abs(t - length);
        return t;
    }

    inline float Repeat(float t, float begin, float end) {
        return Repeat(t - begin, end - begin) + begin;
    }

    inline double RepeatD(double t, double begin, double end) {
        return RepeatD(t - begin, end - begin) + begin;
    }

    inline float PingPong(float t, float begin, float end) {
        return PingPong(t - begin, end - begin) + begin;
    }

    inline float clamp(float x, float a, float b) {
        return std::min(std::max(x, a), b);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    struct KeyframeTpl {
        // DECLARE_SERIALIZE_OPTIMIZE_TRANSFER (Keyframe)
        inline static const char* GetTypeString()  { return "Keyframe"; }
        inline static bool MightContainPPtr()  { return false; }
        // Disable transfer optimization in Editor because tangentMode optimized serialization when reading AssetBundles will corrupt data
        inline static bool AllowTransferOptimization() { return true; }

        template<class TransferFunction>
        void Transfer(TransferFunction& transfer);

        float time;
        T value;
        T inSlope;
        T outSlope;

        int weightedMode;

        T inWeight;
        T outWeight;

        KeyframeTpl();
        KeyframeTpl(float t, const T& v) {
            time = t;
            value = v;
            inSlope = Zero<T>();
            outSlope = Zero<T>();
            inWeight = DefaultWeight<T>();
            outWeight = DefaultWeight<T>();
            weightedMode = 0;
        }

        friend bool operator<(const KeyframeTpl& lhs, const KeyframeTpl& rhs) { return lhs.time < rhs.time; }

        bool operator==(KeyframeTpl<T> const& other) const {
            return time == other.time &&
                value == other.value &&
                inSlope == other.inSlope &&
                outSlope == other.outSlope &&
                inWeight == other.inWeight &&
                outWeight == other.outWeight &&
                weightedMode == other.weightedMode;
        }
    };

    enum AnimationCurveType {
        kFloatCurve = 0,
        kVector3Curve = 1,
        kQuaternionCurve = 2
    };

    void HandleSteppedCurve(const KeyframeTpl<float>& lhs, const KeyframeTpl<float>& rhs, float& value);

    void HandleSteppedTangent(const KeyframeTpl<float>& lhs, const KeyframeTpl<float>& rhs, float& tangent);

    //void HandleSteppedCurve(const KeyframeTpl<Vector3f>& lhs, const KeyframeTpl<Vector3f>& rhs, Vector3f& value) {
    //    for (int i = 0; i < 3; i++) {
    //        if (lhs.outSlope[i] == std::numeric_limits<float>::infinity() || rhs.inSlope[i] == std::numeric_limits<float>::infinity())
    //            value[i] = lhs.value[i];
    //    }
    //}

    //void HandleSteppedTangent(const KeyframeTpl<Vector3f>& lhs, const KeyframeTpl<Vector3f>& rhs, Vector3f& value) {
    //    for (int i = 0; i < 3; i++) {
    //        if (lhs.outSlope[i] == std::numeric_limits<float>::infinity() || rhs.inSlope[i] == std::numeric_limits<float>::infinity())
    //            value[i] = std::numeric_limits<float>::infinity();
    //    }
    //}

    //void HandleSteppedCurve(const KeyframeTpl<Quaternionf>& lhs, const KeyframeTpl<Quaternionf>& rhs, Quaternionf& value) {
    //    if (lhs.outSlope[0] == std::numeric_limits<float>::infinity() || rhs.inSlope[0] == std::numeric_limits<float>::infinity() ||
    //        lhs.outSlope[1] == std::numeric_limits<float>::infinity() || rhs.inSlope[1] == std::numeric_limits<float>::infinity() ||
    //        lhs.outSlope[2] == std::numeric_limits<float>::infinity() || rhs.inSlope[2] == std::numeric_limits<float>::infinity() ||
    //        lhs.outSlope[3] == std::numeric_limits<float>::infinity() || rhs.inSlope[3] == std::numeric_limits<float>::infinity())
    //    {
    //        value = lhs.value;
    //    }
    //}

    //void HandleSteppedTangent(const KeyframeTpl<Quaternionf>& lhs, const KeyframeTpl<Quaternionf>& rhs, Quaternionf& tangent) {
    //    for (int i = 0; i < 4; i++) {
    //        if (lhs.outSlope[i] == std::numeric_limits<float>::infinity() || rhs.inSlope[i] == std::numeric_limits<float>::infinity())
    //            tangent[i] = std::numeric_limits<float>::infinity();
    //    }
    //}

    const float unity_pi = 3.14159265358979323846f;                              // pi
    const float unity_pi_over_two = 1.57079632679489661923f;                     // pi / 2
    const float unity_pi_over_three = 1.0471975511965977461542144610932f;        // pi / 3

    #define FAST_CBRT_POSITIVE(x) (float)exp(log(x) / 3.0);
    #define FAST_CBRT(x) (float)(((x) < 0) ? -exp(log(-(x)) / 3.0) : exp(log(x) / 3.0))

    float BezierExtractU(float t, float w1, float w2);

    template<class T>
    inline T HermiteInterpolate(float t, T p0, T m0, T m1, T p1) {
        float t2 = t * t;
        float t3 = t2 * t;
    
        float a = 2.0F * t3 - 3.0F * t2 + 1.0F;
        float b = t3 - 2.0F * t2 + t;
        float c = t3 - t2;
        float d = -2.0F * t3 +  3.0F * t2;
    
        return a * p0 + b * m0 + c * m1 + d * p1;
    }
    
    template<class T>
    T HermiteInterpolate(float curveT, const KeyframeTpl<T>& lhs, const KeyframeTpl<T>& rhs) {
        float dx = rhs.time - lhs.time;
        T m1;
        T m2;
        float t;
        if (dx != 0.0F) {
            t = (curveT - lhs.time) / dx;
            m1 = lhs.outSlope * dx;
            m2 = rhs.inSlope * dx;
        }
        else {
            t = 0.0F;
            m1 = Zero<T>();
            m2 = Zero<T>();
        }

        return HermiteInterpolate(t, lhs.value, m1, m2, rhs.value);
    }

    template<class T>
    inline T BezierInterpolate(float t, T p0, T p1, T p2, T p3) {
        float t2 = t * t;
        float t3 = t2 * t;
        float omt = 1.0F - t;
        float omt2 = omt * omt;
        float omt3 = omt2 * omt;

        return omt3 * p0 + 3.0F * t * omt2 * p1 + 3.0F * t2 * omt * p2 + t3 * p3;
    }

    template<class T>
    T BezierInterpolate(float t, T v1, T m1, T w1, T v2, T m2, T w2) {
        float u = BezierExtractU(t, w1, 1.0F - w2);
        return BezierInterpolate(u, v1, w1 * m1 + v1, v2 - w2 * m2, v2);
    }

    template<class T>
    T BezierInterpolate(float curveT, const KeyframeTpl<T>& lhs, const KeyframeTpl<T>& rhs) {
        T lhsOutWeight = lhs.weightedMode & kOutWeighted ? lhs.outWeight : DefaultWeight<T>();
        T rhsInWeight = rhs.weightedMode & kInWeighted ? rhs.inWeight : DefaultWeight<T>();
    
        float dx = rhs.time - lhs.time;
        if (dx == 0.0F)
            return lhs.value;
    
        return BezierInterpolate((curveT - lhs.time) / dx, lhs.value, lhs.outSlope * dx, lhsOutWeight, rhs.value, rhs.inSlope * dx, rhsInWeight);
    }

    template<class T>
    class AnimationCurveTpl {
        public:
            /// Stores the curve as a pure cubic function with 4 coefficients
            struct Cache {
                int index;
                float time;
                float timeEnd;
                T coeff[4];

                Cache() { time = std::numeric_limits<float>::infinity(); index = 0; timeEnd = 0.0f; memset(&coeff, 0, sizeof(coeff)); }
                void Invalidate() { time = std::numeric_limits<float>::infinity(); index = 0; }
            };

            typedef KeyframeTpl<T> Keyframe;

        public:
            AnimationCurveTpl() {
                m_PreInfinity = m_PostInfinity = kInternalWrapModeClamp;
                m_RotationOrder = kOrderUnityDefault;
            }

            AnimationCurveTpl(const std::vector<Keyframe>& curve)
                : m_Curve(curve)
            {
                m_PreInfinity = m_PostInfinity = kInternalWrapModeClamp;
                m_RotationOrder = kOrderUnityDefault;
            }

            /// Evaluates the AnimationCurve caching the segment.
            T Evaluate(float curveT, Cache* cache = NULL) const;
            T EvaluateClamp(float curveT, Cache* cache = NULL) const;

            int AddKey(const Keyframe& key);

            /// Performs no error checking. And doesn't invalidate the cache!
            void AddKeyBackFast(const Keyframe& key) { m_Curve.push_back(key); }

            const Keyframe& GetKey(int index) const { return m_Curve[index]; }

            /// When changing the keyframe using GetKey you are not allowed to change the time!
            /// After modifying a key you have to call InvalidateCache
            Keyframe& GetKey(int index) { return m_Curve[index]; }

            void InvalidateCache();

            int GetKeyCount() const { return (int)m_Curve.size(); }

            /// Returns the first and last keyframe time
            std::pair<float, float> GetRange() const;

            // How does the curve before the first keyframe
            void SetPreInfinity(WrapMode pre);
            WrapMode GetPreInfinity() const;
            // How does the curve behave after the last keyframe
            void SetPostInfinity(WrapMode post);
            WrapMode GetPostInfinity() const;

            // How does the curve before the first keyframe
            void SetPreInfinityInternal(InternalWrapMode pre) { m_PreInfinity = pre; InvalidateCache(); }
            InternalWrapMode GetPreInfinityInternal() const { return m_PreInfinity; }
            // How does the curve behave after the last keyframe
            void SetPostInfinityInternal(InternalWrapMode post) { m_PostInfinity = post; InvalidateCache(); }
            InternalWrapMode GetPostInfinityInternal() const { return m_PostInfinity; }

            void Assign(const Keyframe& key) {
                m_Curve.assign(&key, &key + 1);
                StripInvalidKeys();
                InvalidateCache();
            }
            void Assign(const Keyframe* begin, const Keyframe* end) {
                m_Curve.assign(begin, end);
                StripInvalidKeys();
                InvalidateCache();
            }

            void Sort() { std::sort(m_Curve.begin(), m_Curve.end()); InvalidateCache(); }

            void StripInvalidKeys();

            void Reserve(int size) { m_Curve.reserve(size); }
            void ResizeUninitialized(int size) { m_Curve.resize_uninitialized(size); }

            void SetRotationOrder(RotationOrder order)    { m_RotationOrder = order; }
            RotationOrder GetRotationOrder() const {return (RotationOrder)m_RotationOrder; }

            ///@TODO: Cleanup old code to completely get rid of this
            int FindIndex(const Cache& cache, float curveT) const;

            ///@TODO: Cleanup old code to completely get rid of this
            /// Returns the closest keyframe index that is less than time.
            /// Returns -1 if time is outside the range of the curve
            int FindIndex(float time) const;

            void CalculateCacheData(Cache& cache, int lhs, int rhs, float timeOffset) const;

            bool operator==(AnimationCurveTpl<T> const& other) const {
                return m_Curve == other.m_Curve &&
                    m_PreInfinity == other.m_PreInfinity &&
                    m_PostInfinity == other.m_PostInfinity &&
                    m_RotationOrder == other.m_RotationOrder;
            }

            inline bool operator!=(AnimationCurveTpl<T> const& other) const { return !(*this == other); }

            float WrapTime(float curveT) const;

        private:
            void FindIndexForSampling(const Cache& cache, float curveT, int& lhs, int& rhs) const;

            /// Evaluates the AnimationCurve directly.
            void EvaluateWithoutCache(float curveT, T& output) const;

            mutable Cache m_Cache;
            mutable Cache m_ClampCache;

            std::vector<Keyframe> m_Curve;
            InternalWrapMode m_PreInfinity;
            InternalWrapMode m_PostInfinity;
            RotationOrder m_RotationOrder;

#define NUMBER_OF_MEMBERS_ON_ANIMATION_CURVE_FOR_SANITY_CHECKING 4
    };

    ///////////////////////////////////////////////////////////////////////////////////////
    template<class T>
    template<class TransferFunction>
    inline void KeyframeTpl<T>::Transfer(TransferFunction& transfer) {
        transfer.SetVersion(3);

        TRANSFER(time);
        TRANSFER(value);
        TRANSFER(inSlope);
        TRANSFER(outSlope);

        TRANSFER(weightedMode);
        TRANSFER(inWeight);
        TRANSFER(outWeight);
    }

    template<class T>
    inline void EvaluateCache(const typename AnimationCurveTpl<T>::Cache& cache, float curveT, T& output) {
        float t = curveT - cache.time;
        output = (t * (t * (t * cache.coeff[0] + cache.coeff[1]) + cache.coeff[2])) + cache.coeff[3];
    }

    template<class T>
    T AnimationCurveTpl<T>::Evaluate(float curveT, Cache* cache) const {
        int lhsIndex, rhsIndex;
        T output;
    
        if (GetKeyCount() == 1)
            return m_Curve.begin()->value;
    
        // use custom cache for multi-threading support?
        if (!cache)
            cache = &m_Cache;
    
        if (curveT >= cache->time && curveT < cache->timeEnd) {
            EvaluateCache<T>(*cache, curveT, output);
    
            return output;
        }
        // @TODO: Optimize IsValid () away if by making the non-valid case always use the *cache codepath
        else {
            float begTime = m_Curve[0].time;
            float endTime = m_Curve.back().time;
            float wrappedTime;
    
            if (curveT >= endTime) {
                if (m_PostInfinity == kInternalWrapModeClamp) {
                    cache->time = endTime;
                    cache->timeEnd = std::numeric_limits<float>::infinity();
                    cache->coeff[0] = cache->coeff[1] = cache->coeff[2] = Zero<T>();
                    cache->coeff[3] = m_Curve.back().value;
                }
                else if (m_PostInfinity == kInternalWrapModeRepeat) {
                    wrappedTime = clamp(Repeat(curveT, begTime, endTime), begTime, endTime);
    
                    FindIndexForSampling(*cache, wrappedTime, lhsIndex, rhsIndex);
    
                    const Keyframe& lhs = m_Curve[lhsIndex];
                    const Keyframe& rhs = m_Curve[rhsIndex];
    
                    if (lhs.weightedMode & kOutWeighted || rhs.weightedMode & kInWeighted) {
                        EvaluateWithoutCache(curveT, output);

                        return output;
                    }
                    else {
                        CalculateCacheData(*cache, lhsIndex, rhsIndex, curveT - wrappedTime);
                    }
                }
                ///@todo optimize pingpong by making it generate a cache too
                else {
                    EvaluateWithoutCache(curveT, output);

                    return output;
                }
            }
            else if (curveT < begTime) {
                if (m_PreInfinity == kInternalWrapModeClamp) {
                    cache->time = curveT - 1000.0F;
                    cache->timeEnd = begTime;
                    cache->coeff[0] = cache->coeff[1] = cache->coeff[2] = Zero<T>();
                    cache->coeff[3] = m_Curve[0].value;
                }
                else if (m_PreInfinity == kInternalWrapModeRepeat) {
                    wrappedTime = Repeat(curveT, begTime, endTime);
                    FindIndexForSampling(*cache, wrappedTime, lhsIndex, rhsIndex);
    
                    const Keyframe& lhs = m_Curve[lhsIndex];
                    const Keyframe& rhs = m_Curve[rhsIndex];
    
                    if (lhs.weightedMode & kOutWeighted || rhs.weightedMode & kInWeighted) {
                        EvaluateWithoutCache(curveT, output);

                        return output;
                    }
                    else {
                        CalculateCacheData(*cache, lhsIndex, rhsIndex, curveT - wrappedTime);
                    }
                }
                ///@todo optimize pingpong by making it generate a cache too
                else {
                    EvaluateWithoutCache(curveT, output);

                    return output;
                }
            }
            else {
                FindIndexForSampling(*cache, curveT, lhsIndex, rhsIndex);
    
                const Keyframe& lhs = m_Curve[lhsIndex];
                const Keyframe& rhs = m_Curve[rhsIndex];
    
                if (lhs.weightedMode & kOutWeighted || rhs.weightedMode & kInWeighted) {
                    EvaluateWithoutCache(curveT, output);

                    return output;
                }
                else {
                    CalculateCacheData(*cache, lhsIndex, rhsIndex, 0.0F);
                }
            }
    
            //      Assert (CompareApproximately (EvaluateCache (*cache, curveT), EvaluateWithoutCache (curveT), 0.001F));
            EvaluateCache<T>(*cache, curveT, output);

            return output;
        }
    }

    // When we look for the next index, how many keyframes do we just loop ahead instead of binary searching?
    #define SEARCH_AHEAD 3

    template<class T>
    void AnimationCurveTpl<T>::FindIndexForSampling(const Cache& cache, float curveT, int& lhs, int& rhs) const {
        int actualSize = (int)m_Curve.size();
        const Keyframe* frames = &m_Curve[0];
    
        // Reference implementation:
        // (index is the last value that is equal to or smaller than curveT)
    
    #if SEARCH_AHEAD > 0
        int cacheIndex = cache.index;
        if (cacheIndex != -1) {
            // We can not use the cache time or time end since that is in unwrapped time space!
            float time = m_Curve[cacheIndex].time;
    
            if (curveT > time) {
                for (int i = 0; i < SEARCH_AHEAD; i++) {
                    int index = cacheIndex + i;
                    if (index + 1 < actualSize && frames[index + 1].time > curveT) {
                        lhs = index;
    
                        rhs = std::min<int>(lhs + 1, actualSize - 1);

                        return;
                    }
                }
            }
            else {
                for (int i = 0; i < SEARCH_AHEAD; i++) {
                    int index = cacheIndex - i;
                    if (index >= 0 && curveT >= frames[index].time) {
                        lhs = index;
                        rhs = std::min<int>(lhs + 1, actualSize - 1);

                        return;
                    }
                }
            }
        }
    #endif
    
        // Fall back to using binary search
        // upper bound (first value larger than curveT)
        int __len = actualSize;
        int __half;
        int __middle;
        int __first = 0;
        while (__len > 0) {
            __half = __len >> 1;
            __middle = __first + __half;
    
            if (curveT < frames[__middle].time) {
                __len = __half;
            }
            else {
                __first = __middle;
                ++__first;
                __len = __len - __half - 1;
            }
        }
    
        // If not within range, we pick the last element twice
        lhs = __first - 1;
        rhs = std::min(actualSize - 1, __first);
    }

    ///@TODO: Handle step curves correctly
    template<class T>
    void AnimationCurveTpl<T>::EvaluateWithoutCache(float curveT, T& output) const {
        if (GetKeyCount() == 1) {
            output = m_Curve[0].value;

            return;
        }
    
        curveT = WrapTime(curveT);
    
        int lhsIndex, rhsIndex;
        FindIndexForSampling(m_Cache, curveT, lhsIndex, rhsIndex);
        const Keyframe& lhs = m_Curve[lhsIndex];
        const Keyframe& rhs = m_Curve[rhsIndex];
    
        if (lhs.weightedMode & kOutWeighted || rhs.weightedMode & kInWeighted) {
            output = BezierInterpolate<T>(curveT, lhs, rhs);
            HandleSteppedCurve(lhs, rhs, output);
        }
        else {
            output = HermiteInterpolate(curveT, lhs, rhs);
            HandleSteppedCurve(lhs, rhs, output);
        }
    }

    template<class T>
    float AnimationCurveTpl<T>::WrapTime(float curveT) const {
        float begTime = m_Curve[0].time;
        float endTime = m_Curve.back().time;
    
        if (curveT < begTime) {
            if (m_PreInfinity == kInternalWrapModeClamp)
                curveT = begTime;
            else if (m_PreInfinity == kInternalWrapModePingPong)
                curveT = PingPong(curveT, begTime, endTime);
            else
                curveT = Repeat(curveT, begTime, endTime);
        }
        else if (curveT > endTime) {
            if (m_PostInfinity == kInternalWrapModeClamp)
                curveT = endTime;
            else if (m_PostInfinity == kInternalWrapModePingPong)
                curveT = PingPong(curveT, begTime, endTime);
            else
                curveT = Repeat(curveT, begTime, endTime);
        }

        return curveT;
    }

    template<class T>
    void AnimationCurveTpl<T>::CalculateCacheData(Cache& cache, int lhsIndex, int rhsIndex, float timeOffset) const {
        const Keyframe& lhs = m_Curve[lhsIndex];
        const Keyframe& rhs = m_Curve[rhsIndex];

        cache.index = lhsIndex;
        cache.time = lhs.time + timeOffset;
        cache.timeEnd = rhs.time + timeOffset;
        cache.index = lhsIndex;
    
        float dx, length;
        T dy;
        T m1, m2, d1, d2;
    
        dx = rhs.time - lhs.time;
        dx = std::max(dx, 0.0001F);
        dy = rhs.value - lhs.value;
        length = 1.0F / (dx * dx);
    
        m1 = lhs.outSlope;
        m2 = rhs.inSlope;
        d1 = m1 * dx;
        d2 = m2 * dx;
    
        cache.coeff[0] = (d1 + d2 - dy - dy) * length / dx;
        cache.coeff[1] = (dy + dy + dy - d1 - d1 - d2) * length;
        cache.coeff[2] = m1;
        cache.coeff[3] = lhs.value;
        SetupStepped(cache.coeff, lhs, rhs);
    }

    void SetupStepped(float* coeff, const KeyframeTpl<float>& lhs, const KeyframeTpl<float>& rhs);

    //void SetupStepped(Vector3f* coeff, const KeyframeTpl<Vector3f>& lhs, const KeyframeTpl<Vector3f>& rhs) {
    //    for (int i = 0; i < 3; i++) {
    //        // If either of the tangents in the segment are set to stepped, make the constant value equal the value of the left key
    //        if (lhs.outSlope[i] == std::numeric_limits<float>::infinity() || rhs.inSlope[i] == std::numeric_limits<float>::infinity()) {
    //            coeff[0][i] = 0.0F;
    //            coeff[1][i] = 0.0F;
    //            coeff[2][i] = 0.0F;
    //            coeff[3][i] = lhs.value[i];
    //        }
    //    }
    //}

    //void SetupStepped(Quaternionf* coeff, const KeyframeTpl<Quaternionf>& lhs, const KeyframeTpl<Quaternionf>& rhs) {
    //    // If either of the tangents in the segment are set to stepped, make the constant value equal the value of the left key
    //    if (lhs.outSlope[0] == std::numeric_limits<float>::infinity() || rhs.inSlope[0] == std::numeric_limits<float>::infinity() ||
    //        lhs.outSlope[1] == std::numeric_limits<float>::infinity() || rhs.inSlope[1] == std::numeric_limits<float>::infinity() ||
    //        lhs.outSlope[2] == std::numeric_limits<float>::infinity() || rhs.inSlope[2] == std::numeric_limits<float>::infinity() ||
    //        lhs.outSlope[3] == std::numeric_limits<float>::infinity() || rhs.inSlope[3] == std::numeric_limits<float>::infinity())
    //    {
    //        for (int i = 0; i < 4; i++) {
    //            coeff[0][i] = 0.0F;
    //            coeff[1][i] = 0.0F;
    //            coeff[2][i] = 0.0F;
    //            coeff[3][i] = lhs.value[i];
    //        }
    //    }
    //}
 
    //typedef AnimationCurveTpl<float>        AnimationCurveBase;
    typedef AnimationCurveTpl<float>        AnimationCurve;
    //typedef AnimationCurveTpl<Quaternionf>  AnimationCurveQuat;
    //typedef AnimationCurveTpl<Vector3f>     AnimationCurveVec3;
    // */
   
}
