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

    Shapes  nShape;
    QSize   size;
    QImage  image;          /*!< this will be 'stamped' and/or 'dragged' - so not like using QBrush::texture    */

    inline bool operator==( const PContextFreeHand &t ) 
    {
        if ( t.nShape != nShape ) return false;
        if ( t.size != size ) return false;
        if ( t.image != image ) return false;
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
    void setSpray( const PContextSpray & );
    void setText( const PContextText & );

    QImage *            getImage()      { return pImage;        }
    QPen                getPen()        { return pen;           }
    QBrush              getBrush()      { return brush;         }
    QFont               getFont()       { return font;          }
    PContextFreeHand    getFreeHand()   { return freehand;      }
    PContextSpray       getSpray()      { return spray;         }
    PContextText        getText()       { return text;          }

signals:
    void signalModified( const QPen & );
    void signalModified( const QBrush & );
    void signalModified( const QFont & );
    void signalModified( const PContextFreeHand & );
    void signalModified( const PContextSpray & );
    void signalModified( const PContextText & );

public slots:
    void slotImage( QImage *p );
    void slotPen( const QPen &t );
    void slotBrush( const QBrush &t );
    void slotFont( const QFont &t );
    void slotFreeHand( const PContextFreeHand &t );
    void slotSpray( const PContextSpray &t );
    void slotText( const PContextText &t );

protected:
    QImage *            pImage = nullptr;           /*!< tool draws on this image               */
    QPen                pen;                        /*!< defines lines and outlines             */
    QBrush              brush;                      /*!< defines fill details                   */
    QFont               font;                       /*!< defines font details for drawing text  */
    PContextFreeHand    freehand;                   /*!< used by free hand tool                 */
    PContextSpray       spray;                      /*!< used by spray tool                     */
    PContextText        text;                       /*!< used by text tool                      */
};

#define g_Context PContext::instance()

#endif

