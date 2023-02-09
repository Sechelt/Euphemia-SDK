#ifndef H_PGraphicsView
#define H_PGraphicsView

#include "PGraphicsScene.h"

/*!
 * \brief A graphics view for PGraphicsScene. 
 *  
 *  
 * 
 * \author pharvey (2/8/23)
 */
class PGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    PGraphicsView( PGraphicsScene *pScene, QWidget *pParent = nullptr );

    void setScale( qreal n );

    PGraphicsScene *    getScene();
    PLayers *           getLayers();
    PCanvas *           getCanvas();

    qreal getOffsetH();
    qreal getScaleH();
    qreal getLengthH();

    qreal getOffsetV();
    qreal getScaleV();
    qreal getLengthV();

signals:
    void signalChangedH( qreal nOffset, qreal nScale, qreal nLength );
    void signalChangedV( qreal nOffset, qreal nScale, qreal nLength );

public slots:
    void slotSceneRectChanged( const QRectF & );

protected:
    virtual void resizeEvent( QResizeEvent *pEvent );
    virtual void scrollContentsBy( int dx, int dy );
};

#endif


