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
    QRect   doSpray( const QPoint & );

    double  getRandom();
};

class PSprayToolBar : public QWidget
{
    Q_OBJECT
public:
    PSprayToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextSpray & );

protected slots:
    void slotRadius( int );
    void slotPoints( int );

protected:
    QSpinBox *      pRadius;
    QSpinBox *      pPoints;
};


#endif
