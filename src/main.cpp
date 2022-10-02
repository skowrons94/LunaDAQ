#include <QApplication>
#include <TApplication.h>
#include "interface/SlowControlInterface.h"

int main(int argc, char *argv[])
{
  TApplication rootapp("Qt ROOT Application", &argc, argv);
  QApplication a(argc, argv);
  SlowControlInterface w;
  w.show();

  return a.exec();
}
