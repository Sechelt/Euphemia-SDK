#ifndef H_PBackground
#define H_PBackground

#include "P.h"

class PBackground : public QWidget
{
public:
    PBackground( QWidget *pParent );

protected:
    void paintEvent( QPaintEvent *pEvent );

private:
    QBrush brush;
};

#endif
