#include "ListFileDialog.h"

#include <QDir>

ListFileDialog::ListFileDialog( QString dir, QString ext, QWidget *parent): QDialog(parent)
{
    this->setAttribute( Qt::WA_DeleteOnClose );
    setWindowTitle("Select Files");
    createListWidget( dir, ext );
    createConnections( );
}

ListFileDialog::~ListFileDialog( )
{
    delete widget;
    delete convertButton;
    delete closeButton;
    delete btnLayout;
    delete layout;
}

void ListFileDialog::createListWidget( QString dir, QString ext ){
    
    widget = new QListWidget;
    QStringList strList;

    QStringList nameFilter( ext );
    QDir directory( dir );
    strList = directory.entryList( nameFilter );

    widget->addItems(strList);

    QListWidgetItem* item = 0;
    for(int i = 0; i < widget->count(); ++i){
        item = widget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    layout = new QGridLayout( this );
    layout->setMargin( 2 );
    layout->setSpacing( 4 );

    layout->addWidget( widget, 0, 0 );

    btnLayout = new QHBoxLayout();
    btnLayout->setMargin( 2 );
    btnLayout->setSpacing( 4 );
    layout->addLayout( btnLayout, 1, 0);

    convertButton = new QPushButton("Convert", this);
    convertButton->setDefault(true);
    btnLayout->addWidget( convertButton );

    closeButton = new QPushButton("Close", this);
    btnLayout->addWidget( closeButton );

}

void ListFileDialog::prepareFiles( )
{
    QStringList checkedItems;
    for(int i = 0; i < widget->count(); ++i){
        QListWidgetItem* item = widget->item(i);
        if(item->checkState() == Qt::Checked){
            checkedItems.append(item->text());
        }
    }
    emit sendFiles(checkedItems);
    emit closeDialog( );
}

void ListFileDialog::createConnections( ){
    connect(widget, SIGNAL(itemChanged(QListWidgetItem*)),
            this, SLOT(highlightChecked(QListWidgetItem*)));
    connect(closeButton, SIGNAL(clicked( )), 
            this, SLOT(close( )) );
    connect(convertButton, SIGNAL(clicked( )), 
            this, SLOT(prepareFiles( )));
    connect(this, SIGNAL(closeDialog( )), 
            this, SLOT(close( )));
}

void ListFileDialog::highlightChecked( QListWidgetItem *item ){
    if( item->checkState( ) == Qt::Checked )
        item->setBackground( QColor("#ffffb2") );
    else
        item->setBackground( QColor("#ffffff") );
}