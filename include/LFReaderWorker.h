#ifndef LFNREADERWORKER_H
#define LFNREADERWORKER_H

#include <string>

#include <QProgressDialog>

#include "CAENBoards.h"
#include "LFReader.h"

class LFReaderWorker : public QObject
{
    Q_OBJECT

    public:
        LFReaderWorker( QStringList files, CAENBoards* caenBoards, std::string dir );
       ~LFReaderWorker( ){ };

    private:

        LFReader* reader;

        std::string dataDir;
        QStringList fileList;
        std::map<int,int> boardChannels;

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