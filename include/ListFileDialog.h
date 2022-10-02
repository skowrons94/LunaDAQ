#ifndef LISTFILEDIALOG_H
#define LISTFILEDIALOG_H

#include <string>

#include <QWidget>
#include <QDialog>
#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>
#include <QDialogButtonBox>

#include <QGridLayout>
#include <QHBoxLayout>

class ListFileDialog : public QDialog
{
    Q_OBJECT
    
    public:
        ListFileDialog( QString dir, QString ext, QWidget *parent = 0 );
        ~ListFileDialog( );

    signals:
        void sendFiles( QStringList );
        void closeDialog( );

    public slots:

        void highlightChecked( QListWidgetItem* item );
        
        void prepareFiles( );
        
    private:
        QListWidget*       widget;
        QPushButton*       convertButton;
        QPushButton*       closeButton;

        QGridLayout*  layout;
        QHBoxLayout*  btnLayout;

        void createListWidget( QString dir, QString ext );
        void createOtherWidgets();
        void createLayout();
        void createConnections();

};

#endif // LISTFILEDIALOG_H