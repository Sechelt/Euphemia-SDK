#ifndef H_PContext
#define H_PContext

#include "P.h"

class PContextFreeHand
{
public:
    enum Shapes
    {
        ShapeRound,         /*!< ellipse filled with current pen color and of specified size                    */
        ShapeRectangle,     /*!< rectangle filled with current pen color and of specified size                  */
        ShapeCross,         /*!< a horizontal and vertical line with current pen color and of specified size    */
        ShapeImageScaled,   /*!< specified image scaled to size                                                 */
        ShapeImage          /*!< specified image                                                                */
    };

    Shapes  nShape  = ShapeRound;
    QSize   size    = QSize( 5, 5 );
    QImage  image;          /*!< this will be 'stamped' and/or 'dragged' - so not like using QBrush::texture    */

    inline bool operator==( const PContextFreeHand &t ) 
    {
        if ( t.nShape != nShape ) return false;
        if ( t.size != size ) return false;
        if ( t.image != image ) return false;
        return true;
    }
};

class PContextErase
{
public:
    enum Shapes
    {
        ShapeEllipse,
        ShapeRectangle
    };

    Shapes  nShape  = ShapeEllipse;
    QSize   size    = QSize( 5, 5 );

    inline bool operator==( const PContextErase &t ) 
    {
        if ( t.nShape != nShape ) return false;
        if ( t.size != size ) return false;
        return true;
    }
};

class PContextPaste
{
public:

    QPainter::CompositionMode nCompositionMode = QPainter::CompositionMode_SourceOver;
    bool bStamp = false;

    inline bool operator==( const PContextPaste &t ) 
    {
        if ( t.nCompositionMode != nCompositionMode ) return false;
        if ( t.bStamp != bStamp ) return false;
        return true;
    }
};

class PContextSpray
{
public:
    int nRadius = 10;
    int nPoints = 5;

    inline bool operator==( const PContextSpray &t ) 
    {
        if ( t.nRadius != nRadius ) return false;
        if ( t.nPoints != nPoints ) return false;
        return true;
    }
};

class PContextPolygonFilled
{
public:
    Qt::FillRule nFillRule = Qt::OddEvenFill;

    inline bool operator==( const PContextPolygonFilled &t ) 
    {
        if ( t.nFillRule != nFillRule ) return false;
        return true;
    }
};

class PContextText
{
public:
    QString stringText;
    Qt::AlignmentFlag nHAlign = Qt::AlignHCenter;
    Qt::AlignmentFlag nVAlign = Qt::AlignVCenter;

    inline bool operator==( const PContextText &t ) 
    {
        if ( t.stringText != stringText ) return false;
        if ( t.nHAlign != nHAlign ) return false;
        if ( t.nVAlign != nVAlign ) return false;
        return true;
    }
};

class PContext : public QObject 
{
    Q_OBJECT
public:
    static PContext *instance();

    void setImage( QImage *p );
    void setPen( const QPen & );
    void setBrush( const QBrush & );
    void setFont( const QFont & );
    void setFreeHand( const PContextFreeHand & );
    void setErase( const PContextErase & );
    void setSpray( const PContextSpray & );
    void setText( const PContextText & );
    void setPolygonFilled( const PContextPolygonFilled & );
    void setPaste( const PContextPaste & );

    QImage *                getImage()          { return pImage;        }
    QPen                    getPen()            { return pen;           }
    QBrush                  getBrush()          { return brush;         }
    QFont                   getFont()           { return font;          }
    PContextFreeHand        getFreeHand()       { return freehand;      }
    PContextErase           getErase()          { return erase;         }
    PContextSpray           getSpray()          { return spray;         }
    PContextText            getText()           { return text;          }
    PContextPolygonFilled   getPolygonFilled()  { return polygonfilled; }
    PContextPaste           getPaste()          { return paste;         }

signals:
    void signalModified( const QPen & );
    void signalModified( const QBrush & );
    void signalModified( const QFont & );
    void signalModified( const PContextFreeHand & );
    void signalModified( const PContextSpray & );
    void signalModified( const PContextErase & );
    void signalModified( const PContextText & );
    void signalModified( const PContextPolygonFilled & );
    void signalModified( const PContextPaste & );

public slots:
    void slotImage( QImage *p );
    void slotPen( const QPen &t );
    void slotBrush( const QBrush &t );
    void slotFont( const QFont &t );
    void slotFreeHand( const PContextFreeHand &t );
    void slotErase( const PContextErase &t );
    void slotSpray( const PContextSpray &t );
    void slotText( const PContextText &t );
    void slotPolygonFilled( const PContextPolygonFilled &t );
    void slotPaste( const PContextPaste &t );

protected:
    QImage *                pImage = nullptr;           /*!< tool draws on this image               */
    QPen                    pen;                        /*!< defines lines and outlines             */
    QBrush                  brush;                      /*!< defines fill details                   */
    QFont                   font;                       /*!< defines font details for drawing text  */
    PContextFreeHand        freehand;                   /*!< used by free hand tool                 */
    PContextSpray           spray;                      /*!< used by spray tool                     */
    PContextErase           erase;                      /*!< used by erase tool                     */
    PContextText            text;                       /*!< used by text tool                      */
    PContextPolygonFilled   polygonfilled;              /*!< used by polygon tool                   */
    PContextPaste           paste;                      /*!< used by paste                          */
};

#define g_Context PContext::instance()

#endif

