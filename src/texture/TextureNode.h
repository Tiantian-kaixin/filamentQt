#pragma once

#include <qobject.h>

#include <QMutex>
#include <QQuickWindow>
#include <QSGSimpleTextureNode>

class TextureNode : public QObject, public QSGSimpleTextureNode {
Q_OBJECT
public:
    explicit TextureNode(QQuickWindow* window, GLuint id, const QSize& size);
    ~TextureNode() override;

    Q_SIGNAL void textureInUse();
    Q_SIGNAL void pendingNewTexture();
    Q_SLOT
    void newTexture(GLuint id, const QSize& size);
    int getTextureID();
    Q_SLOT
    void prepareNode();
private:
    GLuint m_id;
    QSize m_size;
    QMutex m_mutex;
    QSGTexture* m_texture;
    QQuickWindow* m_window;
};
