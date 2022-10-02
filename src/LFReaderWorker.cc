#include <map>

#include <QFileInfo>

#include "LFReaderWorker.h"

LFReaderWorker::LFReaderWorker( QStringList files, CAENBoards* caenBoards, std::string dir )
{
    fileList = files;
    dataDir  = dir;

    for( auto board : caenBoards->info )
        boardChannels[board.first] = board.second.boardInfo.Channels;
        
}

void LFReaderWorker::start( )
{
    
    int idx = 0;
    std::string fileIn, fileOut;
    for( auto file : fileList ){
        fileIn  = dataDir + "/" + file.toStdString( );
        fileOut = dataDir + "/" + QFileInfo(file).baseName( ).toStdString() + "_lf.root";
        
        reader = new LFReader( boardChannels, fileIn, fileOut );
        
        connect( reader, SIGNAL(updateProgress(int)), this, SLOT(getProgress(int)) );
        emit updateLabel( QString("Processing file ") + file + QString("...") );
        
        reader->start( );
    }

    emit updateLabel( QString("Done.") );
    delete reader;
    emit quit( );
}

void LFReaderWorker::stop( ){
    reader->stop( );
    emit quit( );
}