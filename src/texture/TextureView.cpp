#include "TextureView.h"

TextureView::TextureView(QQuickItem *parent) : QQuickItem(parent), textureID(0){
    setFlag(QQuickItem::ItemHasContents, true);
    renderThread = new RenderThread();
}

TextureView::~TextureView() {
    delete renderThread;
}

QSGNode* TextureView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) {
    auto* node = dynamic_cast<TextureNode*>(oldNode);
    if (!renderThread->isInitialized) {
        QOpenGLContext* current = window()->openglContext();
        current->doneCurrent();
        auto openGlContext = new QOpenGLContext();
        openGlContext->setFormat(current->format());
        openGlContext->setShareContext(current);
        openGlContext->create();
        renderThread->setOpenGlContext(openGlContext);
        openGlContext->moveToThread(renderThread);
        current->makeCurrent(window());
        QMetaObject::invokeMethod(this, "ready");
        return nullptr;
    }
    if (!node) {
        node = new TextureNode(window(), 0, QSize(m_texturesWidth, m_texturesHeight));
        connect(renderThread, &RenderThread::textureReady, node, &TextureNode::newTexture, Qt::DirectConnection);
        connect(node, &TextureNode::pendingNewTexture, window(), &QQuickWindow::update, Qt::QueuedConnection);
        connect(window(), &QQuickWindow::beforeRendering, node, &TextureNode::prepareNode, Qt::DirectConnection);
        connect(node, &TextureNode::textureInUse, renderThread, &RenderThread::renderNext, Qt::QueuedConnection);
        QMetaObject::invokeMethod(renderThread, "renderNext", Qt::QueuedConnection);
    }
    node->setRect(boundingRect());
    printf("updatePaintNode \n");
    return node;
}

void TextureView::ready() {
    textureID = genTextureID();
    renderThread->renderTargetID = textureID;//window()->winId();
//    renderThread->moveToThread(renderThread);
    renderThread->initialize();
    renderThread->start();
    update();
}

GLuint TextureView::genTextureID() {
    GLuint textures;
    window()->openglContext()->functions()->glGenTextures(1, &textures);
    window()->openglContext()->functions()->glBindTexture(GL_TEXTURE_2D, textures);
    QString filePath = "/Users/tiantian/Desktop/resize_compress.png";
    auto* qImage = new QImage(filePath);
    window()->openglContext()->functions()->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, qImage->width(), qImage->height(),
                                                         0, GL_RGBA,GL_UNSIGNED_BYTE, qImage->mirrored().bits());
    // fixme set color not work
//    std::vector<int8_t> dummy_color(m_texturesWidth * m_texturesHeight * 4, 100);
//    window()->openglContext()->functions()->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texturesWidth, m_texturesHeight,
//                                                         0, GL_RGBA,GL_UNSIGNED_BYTE, dummy_color.data());
    window()->openglContext()->functions()->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    window()->openglContext()->functions()->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return textures;
}

void TextureView::setTexturesWidth(const int& texturesWidth){
    m_texturesWidth = texturesWidth;
}

void TextureView::setTexturesHeight(const int& texturesHeight){
    m_texturesHeight = texturesHeight;
}

void TextureView::mouseEvent(float x, float y, int state) {
    QMetaObject::invokeMethod(renderThread, "mouseEvent", Qt::QueuedConnection,
                              Q_ARG(float, x), Q_ARG(float, y), Q_ARG(int, state));
}
