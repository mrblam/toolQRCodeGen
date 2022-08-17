#include "mainwindow.h"
#include "ui_mainwindow.h"
static void Draw(QRcode *qrc, QImage &image);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , input ("Hello Hoanpx")
{
    ui->setupUi(this);
    QRcode *qr = QRcode_encodeString(input.toStdString().c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 1);
    QImage image(256, 256, QImage::Format_RGB32);
        Draw(qr, image);
        //Use QImage to save the QR code as a picture, the string extension determines the image encoding format.
        image.save("/home/hoanpx/Music/"+ QString(input) +".png");

    QGraphicsScene *graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(graphic);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void Draw(QRcode *qrc, QImage &image)
{
    const int width = qrc->width > 0 ? qrc->width : 1;
    double scale = image.width()/width;
    QPainter painter;

    painter.begin(&image);
    //White background image.
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, image.width(), image.width());

    //The code value is black.
    QColor foreground(Qt::black);
    painter.setBrush(foreground);

    //The conversion of the two-dimensional code data into an image needs to be enlarged, otherwise a single pixel will represent the code value, which looks weird.
    for(int y=0; y<width; y++)
    {
        for(int x=0; x<width; x++)
        {
            unsigned char b=qrc->data[y*width+x];
            /**
             * Each byte of qrc->data corresponds to a small square in the QR code. If the lowest bit of the byte is high, a black square is drawn.
             * The meaning of each bit in the byte is as follows
             *
             * bit0: 1=black/0=white
             * bit1: data and ecc code area
             * bit2: format information
             * bit3: version information
             * bit4: timing pattern
             * bit5: alignment pattern
             * bit6: finder pattern and separator
             * bit7: non-data modules (format, timing, etc.)
             */
            if(b&0x01)
            {
                QRectF r(x*scale, y*scale, scale, scale);
                painter.drawRects(&r, 1);
            }
        }
    }
    painter.end();
}


void MainWindow::paintEvent(QPaintEvent *)
{
    /*
    QPainter painter(this);
        //NOTE: I have hardcoded some parameters here that would make more sense as variables.
        QRcode *qr = QRcode_encodeString(input.toStdString().c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 1);
        if(0!=qr){
            QColor fg("black");
            QColor bg("white");
            painter.setBrush(bg);
            painter.setPen(Qt::NoPen);
            painter.drawRect(0,0,width(),height());
            painter.setBrush(fg);
            const int s=qr->width>0?qr->width:1;
            const double w=width();
            qDebug() << w << "w";
            const double h=height();
            const double aspect=w/h;
            const double scale=((aspect>1.0)?h:w)/s;
            for(int y=0;y<1;y++){
                const int yy=y*s;
                for(int x=0;x<1;x++){
                    const int xx=yy+x;
                    const unsigned char b=qr->data[xx];
                    if(b &0x01){
                        const double rx1=x*scale, ry1=y*scale;
                        QRectF r(rx1, ry1, scale, scale);
                        painter.drawRects(&r,1);
                        qDebug() << rx1 <<ry1<<scale;
                    }
                }
            }
            QRcode_free(qr);
        }
        else{
            QColor error("red");
            painter.setBrush(error);
            painter.drawRect(0,0,width(),height());
            qDebug()<<"QR FAIL: "<< strerror(errno);
        }
     qr=0;
     */
}

