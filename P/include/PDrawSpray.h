#ifndef H_PDrawSpray
#define H_PDrawSpray

#include "PFreeBase.h"

class WColorButton;

class PDrawSpray : public PFreeBase
{
public:
    PDrawSpray( PCanvas *pCanvas );

    virtual QRect doPress( QMouseEvent *pEvent ) override;
    virtual QRect doMove( QMouseEvent *pEvent ) override;
    virtual QRect doRelease( QMouseEvent *pEvent ) override;

protected:
    PCanvas *pCanvas;

    QRect   doSpray( const QPoint & );

    double  getRandom();
};

class PSprayToolBar : public QWidget
{
    Q_OBJECT
public:
    PSprayToolBar( QWidget *p );

public slots:
    void slotRefresh( const QPen & );
    void slotRefresh( const PContextSpray & );

protected slots:
    void slotColor( const QColor & );
    void slotRadius( int );
    void slotPoints( int );

protected:
    WColorButton *  pColor;
    QSpinBox *      pRadius;
    QSpinBox *      pPoints;
};


#endif
