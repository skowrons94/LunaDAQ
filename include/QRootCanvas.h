#ifndef QROOTCANVAS_H
#define QROOTCANVAS_H

#include <QWidget>

// Wrap around the TCanvas in order to make it compatible with Qt

class QResizeEvent;
class QMouseEvent;
class QPushButton;
class QTimer;
class TCanvas;

class QRootCanvas : public QWidget
{
   Q_OBJECT

public:
   QRootCanvas( QWidget *parent = 0);
   virtual ~QRootCanvas() {}

   TCanvas* getCanvas() {return fCanvas;}

   bool fReady = true; // Used as a check if TCanvas can be update
                       // Used both in HistoWorker and WaveWorker
  
protected:
   TCanvas        *fCanvas;

   virtual void    mouseMoveEvent( QMouseEvent *e );
   virtual void    mousePressEvent( QMouseEvent *e );
   virtual void    mouseReleaseEvent( QMouseEvent *e );
   virtual void    resizeEvent( QResizeEvent *e );
};

#endif


