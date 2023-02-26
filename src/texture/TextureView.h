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
    Q_INVOKABLE int getTextureID();
    QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*) override;

    Q_PROPERTY(int texturesWidth READ texturesWidth WRITE setTexturesWidth)
    void setTexturesWidth(const int& scaleMode);
    int texturesWidth() { return m_texturesWidth; }

    Q_PROPERTY(int texturesHeight READ texturesHeight WRITE setTexturesHeight)
    void setTexturesHeight(const int& scaleMode);
    int texturesHeight() { return m_texturesHeight; }
    Q_SLOT void ready();

private:
    // texture的宽高
    int m_texturesWidth = 300;
    int m_texturesHeight = 300;
    RenderThread* renderThread;
    GLuint genTextureID();
};
