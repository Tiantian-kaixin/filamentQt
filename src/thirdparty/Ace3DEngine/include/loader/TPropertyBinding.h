#pragma once
#include <string>
#include "loader/TDefine.h"

namespace T3D {
    class TObject3D;

    class TPropertyBinding {
    public:
        TPropertyBinding(TObject3D* root, const std::string& trackName);
        ~TPropertyBinding();

    public:
        void setValue(float* result, int offset);
        void getValue(float* result, int offset, int valueSize);

    protected:
        TObject3D* findNode(const std::string& objectName);

    protected:
        TObject3D*      _root = nullptr;
        TObject3D*      _target = nullptr;

        int             _valueSize = 1;
        float*          _property = nullptr;

        std::string     _objectName;
        std::string     _propertyName;
    };
}
