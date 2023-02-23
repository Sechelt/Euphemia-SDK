#ifndef H_PMagnifierWidget
#define H_PMagnifierWidget

#include "PGraphicsView.h"

class PMagnifierWidget : public QWidget
{
    Q_OBJECT
public:
    PMagnifierWidget( QWidget *pParent );

    void setView( PGraphicsView *p );

protected:
    PGraphicsView *pView = nullptr;

    void paintEvent( QPaintEvent *pEvent );
};

#endif 

