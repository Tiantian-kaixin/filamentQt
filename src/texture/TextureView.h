#pragma once

#include <QQuickItem>
#include "TextureNode.h"
#include <qopenglfunctions.h>
#include "RenderThread.h"

class TextureView : public QQuickItem {
Q_OBJECT
public:
    explicit TextureView(QQuickItem* parent = nullptr);
    virtual ~TextureView();
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;

    Q_PROPERTY(int texturesWidth READ texturesWidth WRITE setTexturesWidth)
    void setTexturesWidth(const int& scaleMode);
    int texturesWidth() { return m_texturesWidth; }

    Q_PROPERTY(int texturesHeight READ texturesHeight WRITE setTexturesHeight)
    void setTexturesHeight(const int& scaleMode);
    int texturesHeight() { return m_texturesHeight; }
    Q_SLOT void ready();
    Q_INVOKABLE void mouseEvent(float x, float y, int state);

private:
    // texture的宽高
    int m_texturesWidth = 600;
    int m_texturesHeight = 600;
    GLuint textureID;
    RenderThread* renderThread;
    GLuint genTextureID();
};
