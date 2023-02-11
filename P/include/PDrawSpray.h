#ifndef H_PDrawSpray
#define H_PDrawSpray

#include "PFreeBase.h"

class WColorButton;

class PDrawSpray : public PFreeBase
{
public:
    PDrawSpray( PCanvas *pCanvas );

    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override;
    virtual void doRelease( PMouseEvent *pEvent ) override;

protected:
    void doSpray( const QPoint & );

    double getRandom();
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
