#ifndef H_PDrawPolygonFilled
#define H_PDrawPolygonFilled

#include "PDrawPolygon.h"

class PDrawPolygonFilled : public PDrawPolygon
{
    Q_OBJECT
public:
    PDrawPolygonFilled( PCanvas *pCanvas );

protected:
    virtual void doPaint( QPainter *, bool bCommit = true );
};

class PPolygonFilledToolBar : public QWidget
{
    Q_OBJECT
public:
    PPolygonFilledToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextPolygonFilled & );

protected slots:
    void slotFillRule( int );

protected:
    QComboBox *pFillRule;
};

#endif
