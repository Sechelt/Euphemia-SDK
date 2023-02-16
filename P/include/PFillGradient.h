#ifndef H_PFillGradient
#define H_PFillGradient

#include "PShapeBase.h"

class PFillGradientLinear : public PShapeBase
{
    Q_OBJECT
public:
    PFillGradientLinear( PCanvas *pCanvas );
    virtual ~PFillGradientLinear();

    virtual QImage getCopy() override;

    virtual void doDoubleClick( PMouseEvent * ) override;
    virtual void doPress( PMouseEvent * ) override;
    virtual void doMove( PMouseEvent * ) override; 
    virtual void doRelease( PMouseEvent * ) override;
    virtual void doCommit() override;

protected:
    QPolygon polygon;

    virtual void doPaint( QPainter * );
    virtual void doDrawState( const QPoint & ) override;
    virtual void doManipulateState() override;
    virtual void doIdleState() override;
    virtual void doCreateHandles() override;
    virtual void doSyncHandles() override;
    virtual void doMoveHandle( const QPoint &pointPos );
};

class PFillGradientToolBar : public QWidget
{
    Q_OBJECT
public:
    PFillGradientToolBar( QWidget *p );

public slots:
    void slotRefresh( const QGradient::Type n );
    void slotRefresh( const QLinearGradient & );
    void slotRefresh( const QRadialGradient & );
    void slotRefresh( const QConicalGradient & );

protected slots:
    void slotType( int );

protected:
    QComboBox * pPreset;
    QComboBox * pType;
};

#endif
