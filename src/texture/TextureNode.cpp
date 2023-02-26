#include "TextureNode.h"

TextureNode::TextureNode(QQuickWindow* window)
    : m_id(0), m_size(0, 0), m_texture(nullptr), m_window(window) {
  // Our texture node must have a texture, so use the default 0 texture.
  m_texture = m_window->createTextureFromId(m_id, QSize(300, 300));
  setTexture(m_texture);
  setFiltering(QSGTexture::Linear);
}

TextureNode::~TextureNode() { delete m_texture; }

void TextureNode::newTexture(GLuint id, const QSize& size) {
  m_mutex.lock();
  m_id = id;
  m_size = size;
  m_mutex.unlock();
  Q_EMIT pendingNewTexture();
}

int TextureNode::getTextureID(){
  return m_id;
}

void TextureNode::prepareNode() {
  m_mutex.lock();
  int newId = m_id;
  QSize size = m_size;
  m_id = 0;
  m_mutex.unlock();

  if (newId) {
    delete m_texture;
    m_texture = m_window->createTextureFromId(newId, size, QQuickWindow::TextureHasAlphaChannel);
    setTexture(m_texture);
    markDirty(DirtyMaterial);
    Q_EMIT textureInUse();
  }
}
