#ifndef RUNREADERWORKER_H
#define RUNREADERWORKER_H

#include <string>

#include <QProgressDialog>

#include "CAENBoards.h"
#include "RUReader.h"

class RUReaderWorker : public QObject
{
    Q_OBJECT

    public:
        RUReaderWorker( QStringList files, CAENBoards* caenBoards, std::string dir );
       ~RUReaderWorker( ){ };

    private:

        RUReader* reader;

        std::string dataDir;
        QStringList fileList;
        std::map<int,std::string> boardNames;

    signals:
        void updateProgress( int progress );
        void updateLabel( QString label ); 
        void quit( );

    private slots:
        void start( );
        void stop( );

        void getProgress( int progress ){ emit updateProgress( progress ); };

};  

#endif