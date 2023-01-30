#ifndef H_PShapeBase
#define H_PShapeBase

#include "PHandle.h"
#include "PContext.h"

class PCanvas;

class PShapeBase : public QWidget
{
    Q_OBJECT
public:
    PShapeBase( PCanvas *pCanvas );
    virtual ~PShapeBase();

    virtual bool doPress( QMouseEvent *pEvent ) = 0;                            /*!< mouse press from canvas - return false when done   */
    virtual bool doMove( QMouseEvent *pEvent ) = 0;                             /*!< mouse move from canvas - return false when done    */
    virtual bool doRelease( QMouseEvent *pEvent ) = 0;                          /*!< mouse release from canvas - return false when done */
    virtual void doCommit() = 0;                                                /*!< paint shape on QImage                              */
    virtual void doCancel();

signals:
   void signalModified();                           /*!< setters should probably emit this                  */ 

protected:
    PCanvas *          pCanvas;
    QVector<PHandle*>  vectorHandles;              /*!< Handles are children of canvas widget and are shown on top of shape due to z-order. */
    PHandle *          pHandle = nullptr;          /*!< Handle being moved.                                */

    virtual PHandle *   getHandle( const QPoint &pointPos );
    virtual QRect       getGeometry( const QRect &r, int nPenWidth );

    virtual void doCreateHandles() = 0;
    virtual void doDeleteHandles();
    virtual void doShowHandles( bool b = true );
};

#endif
