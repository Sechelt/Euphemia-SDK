#ifndef H_PShapeBase
#define H_PShapeBase

#include "PHandle.h"
#include "PContext.h"

class PCanvas;

class PShapeBase : public QGraphicsObject
{
    Q_OBJECT
public:
    enum States
    {
        StateIdle,
        StateDraw,
        StateManipulate
    };

    PShapeBase( PCanvas *pCanvas );
    virtual ~PShapeBase();

    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 ) override; /*!< redirects to doPaint unless idle*/
    QRectF boundingRect() const override;                       /*!< all PShapeBase based objects are same size as scene - keeping it simple    */

    virtual States getState()   { return nState;    }
    virtual QImage getCopy()    { return QImage();  }

    virtual void doDoubleClick( PMouseEvent *pEvent ) = 0;      /*!< mouse press from canvas                                                    */
    virtual void doPress( PMouseEvent *pEvent ) = 0;            /*!< mouse press from pCanvas                                                   */
    virtual void doMove( PMouseEvent *pEvent ) = 0;             /*!< mouse move from canvas                                                     */
    virtual void doRelease( PMouseEvent *pEvent ) = 0;          /*!< mouse release from canvas                                                  */
    virtual void doCommit() = 0;                                /*!< paint shape on QImage                                                      */
    virtual void doCancel();                                    /*!< reset state to StateIdle                                                   */
    virtual void doCut() {}
    virtual void doCopy();
    virtual void doZoom();

    virtual bool    canCommit() { return nState == StateManipulate; }
    virtual bool    canCancel();
    virtual bool    canCut()    { return false; }
    virtual bool    canCopy()   { return nState == StateManipulate; }           /*!< draw/paste shape (copy from doPaint) select shape (copy from canvas) */

    virtual bool    isSelector() { return false; }

signals:
   void signalChangedState();                                                   /*!<                                                    */ 
   void signalCommitted();

protected:
    States              nState = StateIdle;
    PGraphicsView *     pView;                      /*!< view with focus - handles belong here - in pCanvas but we use it a lot so its here     */
    PCanvas *           pCanvas;                    /*!< canvas we are drawing on                                                               */
    QVector<PHandle*>   vectorHandles;              /*!< Handles are children of canvas widget and are shown on top of shape due to z-order.    */
    PHandle *           pHandle = nullptr;          /*!< Handle being moved.                                                                    */

    virtual void doPaint( QPainter *pPainter ) = 0;

    virtual PHandle *   getHandle( const QPoint &pointPos );
    virtual PHandle *   getHandleUnder( PHandle *pHandle, PHandle::Type n );
    virtual PHandle *   getHandleNext( PHandle *pHandle, PHandle::Type n );
    virtual PHandle *   getHandlePrev( PHandle *pHandle, PHandle::Type n );
    virtual QImage      getMask() { return QImage();  }
    virtual QImage      getTrimmed( const QRect &r, QImage &image, const QImage &imageMask, bool bErase = false );

    // state changes
    virtual void doDrawState( const QPoint & ) = 0;
    virtual void doManipulateState() = 0;
    virtual void doIdleState() = 0;

    virtual void doCreateHandles() = 0;
    virtual void doDeleteHandles();
    virtual void doSyncHandles() = 0;
    virtual void doShowHandles( bool b = true );
};

#endif
