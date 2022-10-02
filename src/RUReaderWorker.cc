#include <map>

#include <QFileInfo>

#include "RUReaderWorker.h"

RUReaderWorker::RUReaderWorker( QStringList files, CAENBoards* caenBoards, std::string dir )
{
    fileList = files;
    dataDir  = dir;

    for( auto board : caenBoards->info )
        boardNames[board.first] = board.second.boardInfo.ModelName;
        
}

void RUReaderWorker::start( )
{
    
    int idx = 0;
    std::string fileIn, fileOut;
    for( auto file : fileList ){
        fileIn  = dataDir + "/" + file.toStdString( );
        fileOut = dataDir + "/" + QFileInfo(file).baseName( ).toStdString() + "_ru.root";
        
        reader = new RUReader( boardNames, fileIn, fileOut );
        
        connect( reader, SIGNAL(updateProgress(int)), this, SLOT(getProgress(int)) );
        emit updateLabel( QString("Processing file ") + file + QString("...") );
        
        reader->start( );
    }

    emit updateLabel( QString("Done.") );
    delete reader;
    emit quit( );

}

void RUReaderWorker::stop( ){
    reader->stop( );
    emit quit( );
}