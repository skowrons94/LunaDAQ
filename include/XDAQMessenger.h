#ifndef XDAQMESSENGER_H
#define XDAQMESSENGER_H

#include <curl/curl.h>
#include <string>
#include <vector>
#include <QString>
#include <iostream>
#include <stdio.h>
#include <QtXml/QDomText>
#include <QFile>
#include <QDebug>
#include <sstream>
#include <QProgressDialog>
#include <tinyxml2.h>
#include <vector>

enum daqStatus {XDAQ_ERROR=0,XDAQ_CONFIGURED,XDAQ_RUNNING,XDAQ_HALTED};

typedef struct
{
    QString      hostname;
    QString      port;
    QString      appClass;
    QString      instance;
    std::string  status;
} XdaqApplication;


typedef struct
{
    QString      hostname;
    QString      port;
    QString      appClass;
    QString      instance;
    int          linkInterface;
    std::string  status;
} XdaqLink;

class XDAQMessenger : public QObject
{
    Q_OBJECT

public:
    XDAQMessenger();
    ~XDAQMessenger();

public:
    std::string createActionMessage(std::string);
    std::string createInfoMessage(std::string, std::string, std::string);
    std::string createParameterMessage(std::string, std::string, std::string,std::string);

    std::string sendMessage(XdaqApplication, std::string);
    std::string sendMessageLink(XdaqLink, std::string);

    bool setTopologyName(std::string);
    void readTopologyFile();
  
    void listAllActors();
    std::vector<XdaqApplication> getReadoutUnits();
    std::vector<XdaqApplication> getLocalFilters();
    std::vector<XdaqApplication> getBuilderUnits();
    std::vector<XdaqApplication> getMergerUnits();
    std::vector<XdaqApplication> getGlobalFilters();

    bool checkTmux(XdaqApplication myApp);
  
    void checkActors( );
    void updateActorStatus(XdaqApplication& myApp);

    void checkLinks( );
    void updateLinkStatus(XdaqLink& myLink);
  
    std::vector<XdaqLink>         getPeerTransports() {return theLinks; }
    std::vector<XdaqApplication>  getListActors()     {return theActors;}

    void initializeDaqActors();
    bool initializeAnActor(XdaqApplication);
    void configureDaqActors();
    bool configureAnActor(XdaqApplication);
    void startDaqActors();
    bool startAnActor(XdaqApplication);
    void stopDaqActors();
    bool stopAnActor(XdaqApplication);
    void killDaqActors();
    bool killAnActor(XdaqApplication);
    void disableDaqActors();
    bool disableAnActor(XdaqApplication);

    void enableDaqLinks();
    bool enableALink(XdaqLink);
    void configureDaqLinks();
    bool configureALink(XdaqLink);
    void disableDaqLinks();
    bool disableALink(XdaqLink);

    void setRunNumber(std::string);
    void setRunNumberIndividual(XdaqApplication,std::string);

    void setRUConfigFilePath(std::string);
    void setLFConfigFilePath(std::string);
    void setConfigFilePath(XdaqApplication,std::string);

    void setRUInputFilePath(std::string);
    void setLFInputFilePath(std::string);
    void setInputFilePath(XdaqApplication,std::string);

    void setRUOutputFilePath(std::string);
    void setLFOutputFilePath(std::string);
    void setOutputFilePath(XdaqApplication,std::string);

    void setRUFileEnable(bool);
    void setLFFileEnable(bool);
    void setBUFileEnable(bool);
    void setMUFileEnable(bool);
    void setGFFileEnable(bool);
    void setFileEnable(XdaqApplication,bool);
  
    int getInputBW(XdaqApplication*);
    int getOutputBW(XdaqApplication*);
    QString getParameterResponse(std::string);
  
    static size_t answerToStdString(void*,size_t,size_t,void*);

signals:
    void updateStatus(QString);
    void updateProgress(int);
    void stop( );

private:

    std::string topologyName;
    std::vector<XdaqApplication> theActors;
    std::vector<XdaqLink>        theLinks;

    std::vector<XdaqApplication> ruUnits;
    std::vector<XdaqApplication> buUnits;
    std::vector<XdaqApplication> muUnits;
    std::vector<XdaqApplication> lfUnits;
    std::vector<XdaqApplication> gfUnits;

    std::vector<XdaqApplication>::iterator it_act;
    std::vector<XdaqLink>::iterator        it_lin;
    QDomDocument xml_parser;

    std::string tmux_start = "tmux new-session -d -s";
    std::string tmux_kill = "tmux kill-session -t";
    std::string xdaq = "/opt/xdaq/bin/xdaq.exe";
};

#endif // XDAQMESSENGER_H
