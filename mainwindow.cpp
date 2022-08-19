#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
//static void Draw(QRcode *qrc, QImage &image);
static void Draw(QrCode qr, QImage &image);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->from->setValidator( new QIntValidator(0, 100, this) );
    ui->to->setValidator( new QIntValidator(0, 10000, this) );
//    this->setStyleSheet("{background-image: url(:/home/hoanpx/Pictures/White_full.png);}");
}

MainWindow::~MainWindow()
{
    delete ui;
}
#if 0
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
    qDebug()<<image.width();
    //The code value is black.
    QColor foreground(Qt::red);
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
#endif
void MainWindow::on_pushButton_clicked()
{
    QString header = ui->headerQR->text();
    QString from = ui->from->text();
    QString to   = ui->to->text();
    beginNumber  = from.split(" ")[0].toInt();
    endNumber    = to.split(" ")[0].toInt();
    for(int a=beginNumber;a<=endNumber; a++ )
    {
        QString s ;
        s = QString::number(a).rightJustified(10, '0');
        input = QString(header) + QString(s);
//        QRcode *qr = QRcode_encodeString(input.toStdString().c_str(), 1, QR_ECLEVEL_L, QR_MODE_8, 1);
        const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;

        QrCode qr = QrCode::encodeText(input.toStdString().c_str(), errCorLvl);
        QImage image(1080, 1080, QImage::Format_RGB32);
        Draw(qr, image);
        //Use QImage to save the QR code as a picture, the string extension determines the image encoding format.
        image.save("/home/hoanpx/Music/"+ QString(input) +".png");

        QGraphicsScene *graphic = new QGraphicsScene(this);
        graphic->addPixmap(QPixmap::fromImage(image));
        ui->graphicsView->setScene(graphic);
    }
}
void Draw(QrCode qr, QImage &image)
{
    const int width =  qr.getSize()> 0 ?  qr.getSize() : 1;
    double scale = image.width()/width;
    QPainter painter;

    painter.begin(&image);
    //White background image.
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, image.width(), image.width());
    qDebug()<<image.width();
    //The code value is black.
    QColor foreground(Qt::red);
    painter.setBrush(foreground);

    //The conversion of the two-dimensional code data into an image needs to be enlarged, otherwise a single pixel will represent the code value, which looks weird.
    for(int y=0; y<width; y++)
    {
        for(int x=0; x<width; x++)
        {
            unsigned char b=qr.getModule(x, y);
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
