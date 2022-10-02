#include <memory>
#include <fstream>

#include "XDAQMessenger.h"

XDAQMessenger::XDAQMessenger()
{
    theActors.clear();
    theLinks.clear();
    ruUnits.clear();
    buUnits.clear();
    muUnits.clear();
    lfUnits.clear();
    gfUnits.clear();
}

XDAQMessenger::~XDAQMessenger()
{

}

std::string XDAQMessenger::createActionMessage(std::string action)
{
    std::string message = "<SOAP-ENV:Envelope SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Header></SOAP-ENV:Header><SOAP-ENV:Body><xdaq:"+action+" xmlns:xdaq=\"urn:xdaq-soap:3.0\"/></SOAP-ENV:Body></SOAP-ENV:Envelope>" ;
    return message;
}

std::string XDAQMessenger::createInfoMessage(std::string appClass, std::string parName, std::string parType)
{
    std::string message = "<SOAP-ENV:Envelope SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Header></SOAP-ENV:Header><SOAP-ENV:Body><xdaq:ParameterGet xmlns:xdaq=\"urn:xdaq-soap:3.0\"><p:properties xmlns:p=\"urn:xdaq-application:"+appClass+"\" xsi:type=\"soapenc:Struct\"><p:"+parName+" xsi:type=\""+parType+"\"/></p:properties></xdaq:ParameterGet></SOAP-ENV:Body></SOAP-ENV:Envelope>";
    return message;

}

std::string XDAQMessenger::createParameterMessage(std::string appClass, std::string parName, std::string parType,std::string parValue)
{

    std::string message = "<SOAP-ENV:Envelope SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:SOAP-ENC=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Header></SOAP-ENV:Header><SOAP-ENV:Body><xdaq:ParameterSet xmlns:xdaq=\"urn:xdaq-soap:3.0\"><p:properties xmlns:p=\"urn:xdaq-application:"+appClass+"\" xsi:type=\"soapenc:Struct\"><p:"+parName+" xsi:type=\""+parType+"\">"+parValue+"</p:"+parName+"></p:properties></xdaq:ParameterSet></SOAP-ENV:Body></SOAP-ENV:Envelope>";
    return message;
}

size_t XDAQMessenger::answerToStdString(void* ptr, size_t size, size_t nmemb, void* str)
{
  std::string* s = static_cast<std::string*>(str);
  std::copy((char*)ptr, (char*)ptr + (size + nmemb), std::back_inserter(*s));
  return size * nmemb;
}

void XDAQMessenger::listAllActors()
{
    std::cout << "The topology contains " << theActors.size() << " actors: " << std::endl;
    for(it_act = theActors.begin() ; it_act!=theActors.end();it_act++){
        std::cout << "-- " << it_act->hostname.toStdString()
                  << ":"   << it_act->port.toStdString()
                  << "   " << it_act->appClass.toStdString()
                  << "   " << it_act->instance.toStdString() << std::endl;

    }

}
std::string XDAQMessenger::sendMessageLink(XdaqLink theLink, std::string message)
{
    bool status = true;
    CURL *curl;
    curl = curl_easy_init();
    struct curl_slist *header = NULL;
    std::string answer;
    if(curl) {
        CURLcode res;
        std::string theHost = "http://"+theLink.hostname.toStdString()+":"+theLink.port.toStdString();
        std::string theActionClient = "SOAPAction: urn:xdaq-application:class="+theLink.appClass.toStdString()
                +",instance="+theLink.instance.toStdString();
        header = curl_slist_append(header, theActionClient.c_str());
        header = curl_slist_append(header, "Content-Type: text/xml");
        header = curl_slist_append(header, "Content-Description: SOAP Message");
        curl_easy_setopt(curl, CURLOPT_URL, theHost.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 0);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, answerToStdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE,    2L);


        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
	  //            printf("Error: %s\n", curl_easy_strerror(res));
            return "false";
        }
        qDebug()<< answer.c_str() ;
        curl_slist_free_all(header); /* free the list again */
        curl_easy_cleanup(curl);

    }
    return answer;

}
std::string XDAQMessenger::sendMessage(XdaqApplication theApp, std::string message)
{
    bool status = true;
    CURL *curl;
    curl = curl_easy_init();
    struct curl_slist *header = NULL;
    std::string answer;
    if(curl) {
        CURLcode res;
        std::string theHost = "http://"+theApp.hostname.toStdString()+":"+theApp.port.toStdString();
        std::string theActionClient = "SOAPAction: urn:xdaq-application:class="+theApp.appClass.toStdString()
                +",instance="+theApp.instance.toStdString();
        header = curl_slist_append(header, theActionClient.c_str());
        header = curl_slist_append(header, "Content-Type: text/xml");
        header = curl_slist_append(header, "Content-Description: SOAP Message");
        curl_easy_setopt(curl, CURLOPT_URL, theHost.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 0);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, answerToStdString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &answer);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE,    2L);


        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
	  //            printf("Error: %s\n", curl_easy_strerror(res));
            return "false";
        }
        qDebug()<< answer.c_str() ;
        curl_slist_free_all(header); /* free the list again */
        curl_easy_cleanup(curl);

    }
    return answer;

}
void XDAQMessenger::initializeDaqActors()
{
    for(it_act = theActors.begin() ; it_act != theActors.end() ; it_act++)
        initializeAnActor(*it_act);
}
void XDAQMessenger::killDaqActors()
{    
    checkActors( );
    for(it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
        if(it_act->status == "Running"){
            stopAnActor(*it_act);
        }
        killAnActor(*it_act);
    }
}
void XDAQMessenger::configureDaqActors()
{
    // for the configuration the order is not important so we can
    // configure them in a common loop
    QString infor ( "Your action in progress..." );

    QProgressDialog progress( infor,
                              "Abort ", 0, 1000);
    
    progress.setValue(0);
    int actor = 0;
    for(it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
      configureAnActor(*it_act);
      progress.setValue( ((float)(actor)/theActors.size())*100 );
      actor++;
    }
    progress.setLabelText("Done");
    progress.setValue(100);
    progress.close();
}
void XDAQMessenger::disableDaqActors()
{
    // for the configuration the order is not important so we can
    // configure them in a common loop
    QString infor ( "Your action in progress..." );

    QProgressDialog progress( infor,
                              "Abort ", 0, 1000);
    
    progress.setValue(0);
    int actor = 0;
    for(it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
      disableAnActor(*it_act);
      progress.setValue( ((float)(actor)/theActors.size())*100 );
      actor++;
    }
    progress.setLabelText("Done");
    progress.setValue(100);
    progress.close();
}
void XDAQMessenger::startDaqActors()
{
    // for the start the order is important so we start from the highest level
    // and go down in the chain
    QString infor ( "Your action in progress..." );

    QProgressDialog progress( infor,
                              "Abort ", 0, 100);
    progress.setValue(0);
    int actor = 0;
    progress.setLabelText("Global Filter");
    for(it_act = gfUnits.begin() ; it_act!= gfUnits.end() ; it_act++){
        startAnActor(*it_act);
        progress.setValue( ((float)(actor)/theActors.size())*100 );
        actor++;
    }
    progress.setLabelText("Merger Units");
    for(it_act = muUnits.begin() ; it_act!= muUnits.end() ; it_act++){
        startAnActor(*it_act);
        progress.setValue( ((float)(actor)/theActors.size())*100 );
        actor++;
    }
    progress.setLabelText("Builder Units");
    for(it_act = buUnits.begin() ; it_act!= buUnits.end() ; it_act++){
        startAnActor(*it_act);
        progress.setValue( ((float)(actor)/theActors.size())*100 );
        actor++;
    }
    progress.setLabelText("Local Filter");
    for(it_act = lfUnits.begin() ; it_act!= lfUnits.end() ; it_act++){
        startAnActor(*it_act);
        progress.setValue( ((float)(actor)/theActors.size())*100 );
        actor++;
    }
    progress.setLabelText("Readout Units");
    for(it_act = ruUnits.begin() ; it_act!= ruUnits.end() ; it_act++){
        startAnActor(*it_act);
        progress.setValue( ((float)(actor)/theActors.size())*100 );
        actor++;
    }
    progress.setLabelText("Done");
    progress.setValue(100);
    progress.close();
}

void XDAQMessenger::stopDaqActors()
{
    // For the stop we pay attention to the order to start with the lowest
    // level

    emit updateProgress(0);
    int actor = 0;
    emit updateStatus("Stopping Readout Units...");
    for(it_act = ruUnits.begin() ; it_act!= ruUnits.end() ; it_act++){
        stopAnActor(*it_act);
        emit updateProgress( (actor*100)/theActors.size() );
        actor++;
    }
    emit updateStatus("Stopping Local Filter...");
    for(it_act = lfUnits.begin() ; it_act!= lfUnits.end() ; it_act++){
        stopAnActor(*it_act);
        emit updateProgress( (actor*100)/theActors.size() );
        actor++;
    }
    emit updateStatus("Stopping Builder Units...");
    for(it_act = buUnits.begin() ; it_act!= buUnits.end() ; it_act++){
        stopAnActor(*it_act);
        emit updateProgress( (actor*100)/theActors.size() );
        actor++;
    }
    emit updateStatus("Stopping Merger Units...");
    for(it_act = muUnits.begin() ; it_act!= muUnits.end() ; it_act++){
        stopAnActor(*it_act);
        emit updateProgress( (actor*100)/theActors.size() );
        actor++;
    }
    emit updateStatus("Stopping Global Filter...");
    for(it_act = gfUnits.begin() ; it_act!= gfUnits.end() ; it_act++){
        stopAnActor(*it_act);
        emit updateProgress( (actor*100)/theActors.size() );
        actor++;
    }
    emit updateStatus("Done");
    emit updateProgress(100);
    emit stop( );
}

void XDAQMessenger::enableDaqLinks()
{ 
    // For the TCP configuration the order is not important so we can
    // initialize them in a common loop
    QString infor ( "Your action in progress..." );

    QProgressDialog progress( infor,
			      "Abort ", 0, 1000);
    
    progress.setValue(0);
    int link = 0;
    for(it_lin = theLinks.begin() ; it_lin != theLinks.end() ; it_lin++){
      enableALink(*it_lin);
      progress.setValue( ((float)(link)/theLinks.size())*100 );
      link++;
    }
    progress.setLabelText("Done");
    progress.setValue(100);
    progress.close();

}
void XDAQMessenger::configureDaqLinks()
{

    // For the TCP configuration the order is not important so we can
    // initialize them in a common loop
    QString infor ( "Your action in progress..." );

    QProgressDialog progress( infor,
    			      "Abort ", 0, 1000);

    progress.setValue(0);
    int link = 0;
    for(it_lin = theLinks.begin() ; it_lin != theLinks.end() ; it_lin++){
      configureALink(*it_lin);
      progress.setValue( ((float)(link)/theLinks.size())*100 );
      link++;
    }
    progress.setLabelText("Done");
    progress.setValue(100);
    progress.close();

}
void XDAQMessenger::disableDaqLinks()
{
    // for the TCP configuration the order is not important so we can
    // initialize them in a common loop
    QString infor ( "Your action in progress..." );

    QProgressDialog progress( infor,
			      "Abort ", 0, 1000);
    
    progress.setValue(0);
    int link = 0;
    for(it_lin = theLinks.begin() ; it_lin != theLinks.end() ; it_lin++){
      disableALink(*it_lin);
      progress.setValue( ((float)(link)/theLinks.size())*100 );
      link++;
    }
    progress.setLabelText("Done");
    progress.setValue(100);
    progress.close();
}

void XDAQMessenger::setRunNumber(std::string runNb)
{
    //We set the run number to all the actors even for the ones for which
    //we don't write the file
    for(it_act = theActors.begin(); it_act!=theActors.end();it_act++){
        setRunNumberIndividual(*it_act,runNb);
    }
}

void XDAQMessenger::setRunNumberIndividual(XdaqApplication myApp, std::string runNb)
{
    std::string myMessage = createParameterMessage(myApp.appClass.toStdString(),
                                                   "runNumber","xsd:unsignedInt",
                                                   runNb);
    std::string myAnswer = sendMessage(myApp,myMessage);
}

bool XDAQMessenger::stopAnActor(XdaqApplication myApp)
{
    std::string myMessage = createActionMessage("Halt");
    std::string myAnswer = sendMessage(myApp,myMessage);
    if(myAnswer.find("Halted")!=std::string::npos)return true;
    else return false;
}

void XDAQMessenger::setRUConfigFilePath(std::string configFilepath)
{
    //We set the config file path for the RU (only path)
    for(it_act=ruUnits.begin(); it_act!=ruUnits.end();it_act++){
        setConfigFilePath(*it_act,configFilepath);
    }
}

void XDAQMessenger::setLFConfigFilePath(std::string configFilepath)
{
    //We set the config file path for the LF (path + file name)
    configFilepath += "/LocalFilter.conf";
    for(it_act=lfUnits.begin(); it_act!=lfUnits.end();it_act++){
        setConfigFilePath(*it_act,configFilepath);
    }
}

void XDAQMessenger::setConfigFilePath(XdaqApplication myApp, std::string configFilepath)
{
    std::string myMessage = createParameterMessage(myApp.appClass.toStdString(),
                                                   "configFilepath","xsd:string",
                                                   configFilepath);
    std::string myAnswer = sendMessage(myApp,myMessage);
}

void XDAQMessenger::setRUInputFilePath(std::string inputFilepath)
{
    //We set the input file path to RU units
    inputFilepath += "/rudata";
    for(it_act = ruUnits.begin(); it_act!=ruUnits.end();it_act++){
        setInputFilePath(*it_act,inputFilepath);
    }
}

void XDAQMessenger::setLFInputFilePath(std::string inputFilepath)
{
    //We set the input file path to RU units
    inputFilepath += "/lfdata";
    for(it_act = lfUnits.begin(); it_act!=lfUnits.end();it_act++){
        setInputFilePath(*it_act,inputFilepath);
    }
}

void XDAQMessenger::setInputFilePath(XdaqApplication myApp, std::string inputFilepath)
{
    std::string myMessage = createParameterMessage(myApp.appClass.toStdString(),
                                                   "inputFilepath","xsd:string",
                                                   inputFilepath);
    std::string myAnswer = sendMessage(myApp,myMessage);
}

void XDAQMessenger::setLFOutputFilePath(std::string outputFilepath)
{
    //We set the output file path to all the actors even for the ones for which
    //we don't write the file
    outputFilepath += "/lfdata";
    for(it_act = lfUnits.begin(); it_act!=lfUnits.end();it_act++){
        setOutputFilePath(*it_act,outputFilepath);
    }
}

void XDAQMessenger::setRUOutputFilePath(std::string outputFilepath)
{
    //We set the output file path to all the actors even for the ones for which
    //we don't write the file
    outputFilepath += "/rudata";
    for(it_act = ruUnits.begin(); it_act!=ruUnits.end();it_act++){
        setOutputFilePath(*it_act,outputFilepath);
    }
}

void XDAQMessenger::setOutputFilePath(XdaqApplication myApp, std::string outputFilepath)
{
    std::string myMessage = createParameterMessage(myApp.appClass.toStdString(),
                                                   "outputFilepath","xsd:string",
                                                   outputFilepath);
    std::string myAnswer = sendMessage(myApp,myMessage);
}

bool XDAQMessenger::checkTmux(XdaqApplication myApp)
{

  std::string cmd;
  int result;
  cmd = "tmux has-session -t " + myApp.appClass.toStdString( ) + " >>/dev/null 2>>/dev/null";
  result = system( cmd.c_str( ) );
  if( result != 0 )
    return false;
  else
    return true;     
  
}

bool XDAQMessenger::initializeAnActor(XdaqApplication myApp)
{
    std::string cmd;
    int result;
    cmd = "tmux has-session -t " + myApp.appClass.toStdString( ) + " >>/dev/null 2>>/dev/null";
    result = system( cmd.c_str( ) );
    if( result != 0 ){ // tmux returns 0 for success
      cmd=tmux_start+" "+ myApp.appClass.toStdString()+" '"+xdaq+" -p "+myApp.port.toStdString()+" -c "+topologyName+"'";
      system( cmd.c_str( ) );
      return true;
    }
    return false;
}

bool XDAQMessenger::killAnActor(XdaqApplication myApp)
{
    std::string cmd;
    int result;
    cmd = "tmux has-session -t " + myApp.appClass.toStdString( ) + " >>/dev/null 2>>/dev/null";
    result = system( cmd.c_str( ) );
    if( result == 0 ){ // tmux returns 0 for success
      cmd=tmux_kill+" "+myApp.appClass.toStdString();
      system( cmd.c_str( ) );
    }
    return true;
}

bool XDAQMessenger::configureAnActor(XdaqApplication myApp)
{
    std::string myMessage = createActionMessage("Configure");
    std::string myAnswer = sendMessage(myApp,myMessage);
    if(myAnswer.find("Configured")!=std::string::npos)return true;
    else return false;
}

bool XDAQMessenger::startAnActor(XdaqApplication myApp)
{
    std::string myMessage = createActionMessage("Enable");
    std::string myAnswer = sendMessage(myApp,myMessage);
    if(myAnswer.find("Enabled")!=std::string::npos)return true;
    else return false;
}

bool XDAQMessenger::disableAnActor(XdaqApplication myApp)
{
    std::string myMessage = createActionMessage("Disable");
    std::string myAnswer = sendMessage(myApp,myMessage);
    if(myAnswer.find("Disabled")!=std::string::npos)return true;
    else return false;
}

void XDAQMessenger::checkActors( )
{

  for( int i = 0; i < theActors.size( ); ++i )
    updateActorStatus(theActors[i]);
  
}

void XDAQMessenger::updateActorStatus(XdaqApplication& myApp)
{
    std::string myMessage = createInfoMessage(myApp.appClass.toStdString(),"stateName","xsd:string");
    std::string myAnswer = sendMessage(myApp,myMessage);
    if(myAnswer.empty()) myApp.status = "Error";
    if(myAnswer=="failed") myApp.status = "Error";

    if(myAnswer.find("Halted")!=std::string::npos){
        myApp.status = "Halted";
    }else if(myAnswer.find("Configured")!=std::string::npos){
        myApp.status = "Configured";
    }else if(myAnswer.find("Running")!=std::string::npos){
        myApp.status = "Running";
    }else
        myApp.status = "Error";
}

void XDAQMessenger::checkLinks( )
{

  for( int i = 0; i < theLinks.size( ); ++i )
    updateLinkStatus( theLinks[i] );
  
}

void XDAQMessenger::updateLinkStatus(XdaqLink& myLink)
{
    std::string myMessage = createInfoMessage(myLink.appClass.toStdString(),"stateName","xsd:string");
    std::string myAnswer = sendMessageLink(myLink,myMessage);
    if(myAnswer.empty()) myLink.status = "Error";
    if(myAnswer=="failed") myLink.status = "Error";

    if(myAnswer.find("Halted")!=std::string::npos){
        myLink.status = "Halted";
    }else if(myAnswer.find("Ready")!=std::string::npos){
        myLink.status = "Ready";
    }else if(myAnswer.find("Enabled")!=std::string::npos){
        myLink.status = "Enabled";
    }else
        myLink.status = "Error";

}

bool XDAQMessenger::enableALink(XdaqLink myLink)
{
    std::string myMessage = createActionMessage("Enable");
    std::string myAnswer = sendMessageLink(myLink,myMessage);
    if(myAnswer.find("Enabled")!=std::string::npos)return true;
    else return false;
}

bool XDAQMessenger::configureALink(XdaqLink myLink)
{
    std::string myMessage = createActionMessage("Configure");
    std::string myAnswer = sendMessageLink(myLink,myMessage);
    if(myAnswer.find("Configured")!=std::string::npos)return true;
    else return false;
}

bool XDAQMessenger::disableALink(XdaqLink myLink)
{
    std::string myMessage = createActionMessage("Halt");
    std::string myAnswer = sendMessageLink(myLink,myMessage);
    if(myAnswer.find("Halted")!=std::string::npos)return true;
    else return false;
}


void XDAQMessenger::setRUFileEnable(bool fileEnabled)
{
    for(it_act = ruUnits.begin() ; it_act!= ruUnits.end() ; it_act++){
        setFileEnable(*it_act,fileEnabled);
    }
}
void XDAQMessenger::setLFFileEnable(bool fileEnabled)
{
    for(it_act = lfUnits.begin() ; it_act!= lfUnits.end() ; it_act++){
        setFileEnable(*it_act,fileEnabled);
    }
}
void XDAQMessenger::setBUFileEnable(bool fileEnabled)
{
    for(it_act = buUnits.begin() ; it_act!= buUnits.end() ; it_act++){
        setFileEnable(*it_act,fileEnabled);
    }
}
void XDAQMessenger::setMUFileEnable(bool fileEnabled)
{
    for(it_act = muUnits.begin() ; it_act!= muUnits.end() ; it_act++){
        setFileEnable(*it_act,fileEnabled);
    }
}
void XDAQMessenger::setGFFileEnable(bool fileEnabled)
{
    for(it_act = gfUnits.begin() ; it_act!= gfUnits.end() ; it_act++){
        setFileEnable(*it_act,fileEnabled);
    }
}

void XDAQMessenger::setFileEnable(XdaqApplication myApp ,bool fileEnabled)
{
    std::string myMessage = createParameterMessage(myApp.appClass.toStdString(),
                                                   "writeDataFile","xsd:boolean",
                                                   ((fileEnabled) ? "true" : "false"));
    std::string myAnswer = sendMessage(myApp,myMessage);
}

bool XDAQMessenger::setTopologyName(std::string fileName){

    std::ifstream file( fileName.c_str() );

    if( file.good( ) ){
        topologyName = fileName;
        file.close( );
        return true;
    }
    else{
        file.close( );
        return false;
    }

}

void XDAQMessenger::readTopologyFile()
{
    qDebug() <<"Reading topology file for " << topologyName.c_str();
    QDomDocument topoDoc;
    // Open a file for reading
    QFile topoFile(topologyName.c_str());
    if(!topoFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open the file for reading.";
        return ;
    }
    else {
        // loading
        if(!topoDoc.setContent(&topoFile)){
            qDebug() << "Failed to load the file for reading.";
            return ;
        }
        topoFile.close();
    }
    XdaqApplication tmpApp ;
    XdaqLink        tmplink;
    theActors.clear();
    theLinks.clear();
    // Getting root element
    QDomElement partition = topoDoc.firstChildElement();
    QDomNodeList subNodes;
    QDomNodeList nodes = partition.elementsByTagName("xc:Context");
    qDebug() << "# nodes = " << nodes.count();
    QString actorAddr;
    QString actorHost;
    QString actorPort;
    QString actorClass;
    QString actorInstance;
    std::ostringstream enaServers;
    enaServers.str("");
    QStringList myStringList ;
    for(int i = 0; i < nodes.count(); i++) {
        QDomNode elm = nodes.at(i);
        if(elm.isElement()) {
            QDomElement e = elm.toElement();
            actorAddr = e.attribute("url") ;
            myStringList = actorAddr.split("//").last().split(':');
            actorPort = myStringList.last();
            actorHost = myStringList.first();
            enaServers << actorHost.toStdString() <<std::endl;
            subNodes= e.elementsByTagName("xc:Application");

            qDebug() << "# subNodes = " << subNodes.count();
            for(int j = 0; j < subNodes.count(); j++) {
                QDomNode subElm = subNodes.at(j);
                if(subElm.isElement()) {
                    QDomElement ee = subElm.toElement();
                    if(ee.attribute("class").startsWith("pt::")){
                        tmplink.hostname = actorHost;
                        tmplink.port     = actorPort;
                        tmplink.appClass = ee.attribute("class") ;
                        tmplink.instance = ee.attribute("instance");
                        tmplink.linkInterface =  (tmplink.appClass.mid(15,2)).toInt() ;
                        theLinks.push_back(tmplink);

                    }else{
                        tmpApp.hostname = actorHost;
                        tmpApp.port     = actorPort;
                        tmpApp.appClass = ee.attribute("class") ;
                        tmpApp.instance = ee.attribute("instance") ;
                        theActors.push_back(tmpApp);
                    }
                }
            }
        }
    }
    qDebug() << "Reading finished";
}

std::vector<XdaqApplication> XDAQMessenger::getReadoutUnits()
{
    if(ruUnits.size()>0)
        return ruUnits;
    ruUnits.clear();
    for(auto it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
        if(it_act->appClass.startsWith("ReadoutUnit"))
            ruUnits.push_back(*it_act);
    }
    return ruUnits;
}
std::vector<XdaqApplication> XDAQMessenger::getLocalFilters()
{
    if(lfUnits.size()>0)
        return lfUnits;
    lfUnits.clear();
    for(auto it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
        if(it_act->appClass.startsWith("LocalFilter"))
            lfUnits.push_back(*it_act);
    }
    return lfUnits;
}
std::vector<XdaqApplication> XDAQMessenger::getBuilderUnits()
{
    if(buUnits.size()>0)
        return buUnits;
    buUnits.clear();
    for(auto it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
        if(it_act->appClass.startsWith("BuilderUnit"))
            buUnits.push_back(*it_act);
    }
    return buUnits;
}
std::vector<XdaqApplication> XDAQMessenger::getMergerUnits()
{
    if(muUnits.size()>0)
        return muUnits;
    muUnits.clear();
    for(auto it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
        if(it_act->appClass.startsWith("Merger"))
            muUnits.push_back(*it_act);
    }
    return muUnits;

}
std::vector<XdaqApplication> XDAQMessenger::getGlobalFilters()
{
    if(gfUnits.size()>0)
        return gfUnits;
    gfUnits.clear();
    for(auto it_act = theActors.begin() ; it_act != theActors.end() ; it_act++){
        if(it_act->appClass.startsWith("GlobalFilter"))
            gfUnits.push_back(*it_act);
    }
    return gfUnits;
}

int XDAQMessenger::getInputBW(XdaqApplication *myApp)
{
    std::string myMessage = createInfoMessage(myApp->appClass.toStdString(),"inputBufferRate","xsd:unsignedInt");
    std::string myAnswer = sendMessage(*myApp,myMessage);
//    std::cout << "Rate = " << getParameterResponse(myAnswer).toInt() << std::endl;
    return  getParameterResponse(myAnswer).toInt();
}

int XDAQMessenger::getOutputBW(XdaqApplication *myApp)
{
    std::string myMessage = createInfoMessage(myApp->appClass.toStdString(),"outputBufferRate","xsd:unsignedInt");
    std::string myAnswer = sendMessage(*myApp,myMessage);
//    std::cout << "Rate = " << getParameterResponse(myAnswer).toInt() << std::endl;
    return  getParameterResponse(myAnswer).toInt();
}


QString XDAQMessenger::getParameterResponse(std::string answer)
{
    QDomDocument xml_parser;
    if(!xml_parser.setContent(QString(answer.c_str()))){
        std::cout << "Cannot set the content" << std::endl;
    }
    QDomElement partition = xml_parser.documentElement();
    if(partition.isNull())
    {
        std::cout << "Not found" << std::endl;
        return 0;
    }

    QDomNode n = partition.firstChild();
    QString value ;
    QDomNodeList subNodes ;

    while(n.isElement()){
        QDomElement e = n.toElement();
        subNodes = e.elementsByTagName("xdaq:ParameterGetResponse");
        for(int j = 0; j < subNodes.count(); j++) {
            QDomNode subElm = subNodes.at(j);
            if(subElm.isElement()) {
                QDomElement ee = subElm.toElement();
                value = ee.text() ;
            }
        }
        n = n.nextSibling();
    }
    return value;
}
