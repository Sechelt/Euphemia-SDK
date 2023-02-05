#ifndef H_PShapeBase
#define H_PShapeBase

#include "PHandle.h"
#include "PContext.h"

class PCanvas;

class PShapeBase : public QWidget
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

    virtual States getState() { return nState; }

    virtual QRect   doDoubleClick( QMouseEvent *pEvent ) = 0;                   /*!< mouse press from canvas - return rect to update    */
    virtual QRect   doPress( QMouseEvent *pEvent ) = 0;                         /*!< mouse press from canvas - return rect to update    */
    virtual QRect   doMove( QMouseEvent *pEvent ) = 0;                          /*!< mouse move from canvas - return  rect to update    */
    virtual QRect   doRelease( QMouseEvent *pEvent ) = 0;                       /*!< mouse release from canvas - return rect to update  */
    virtual QRect   doCommit() = 0;                                             /*!< paint shape on QImage                              */
    virtual void    doCancel();                                                 /*!< reset state to StateIdle                           */
    virtual void    doCut() {}
    virtual void    doCopy() {}

    virtual bool    canCommit();
    virtual bool    canCancel();
    virtual bool    canCut() { return false; }
    virtual bool    canCopy() { return false; }

signals:
   void signalChangedState();                                                   /*!<                                                    */ 
   void signalCommitted();

protected:
    States              nState = StateIdle;
    PCanvas *           pCanvas;
    QVector<PHandle*>   vectorHandles;              /*!< Handles are children of canvas widget and are shown on top of shape due to z-order. */
    PHandle *           pHandle = nullptr;          /*!< Handle being moved.                                */

    virtual PHandle *   getHandle( const QPoint &pointPos );

    // state changes
    virtual void doDraw( const QPoint & ) = 0;
    virtual void doManipulate() = 0;
    virtual void doIdle() = 0;

    virtual void doCreateHandles() = 0;
    virtual void doDeleteHandles();
    virtual void doShowHandles( bool b = true );
};

#endif
