#include <iostream>

#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "XDAQSpy.h"

XDAQSpy::XDAQSpy( ){

  port = 0;
  sockfd = -1;
  address = "";

}

void XDAQSpy::Disconnect( ){

  if( sockfd != -1 ){
    close(sockfd);
    sockfd = -1;
  }

  port = 0;
  address = "";

}

bool XDAQSpy::Connect( std::string address, int port ){

  std::cout << "Trying to connect to XDAQ Spy" << std::endl;

  // Create socket if it is not already created
  if( sockfd == -1 ){
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
      perror("ERROR opening socket");
      return false;
    }
  }
    
  // Setup address structure
  if( inet_addr(address.c_str( )) == - 1 ){
    
    struct hostent*  he;
    struct in_addr** addr_list;

    if(( he = gethostbyname( address.c_str() ) ) == NULL ){     
      perror("ERROR resolving hostname\n");
      return false;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(int i = 0; addr_list[i] != NULL; i++){
      server.sin_addr = *addr_list[i];
      std::cout<<address<<" resolved to "<<inet_ntoa(*addr_list[i])<<std::endl;
      break;
    }

  }

  else server.sin_addr.s_addr = inet_addr( address.c_str() );

  server.sin_family = AF_INET;
  server.sin_port   = htons( port );
  
  if(connect(sockfd, (struct sockaddr*) &server, sizeof(server)) < 0 ){ 
    perror("ERROR connecting");
    return false;
  }

  std::cout << "Connected to XDAQ Spy" << std::endl;

  return true;

}

XDAQSpy::~XDAQSpy( ){

  this->Disconnect( );

}

int XDAQSpy::ReadAll(int sockfd, char* buff, int sizeToRead){

  int nread = 0;
  int nleft = sizeToRead;

  while(nleft > 0){
    
    nread = recv(sockfd,buff,nleft,0);
    
    if(nread < 0)       return 0;//perror("Cannot read data on the socket\n");
    else if(nread == 0) return 0;//perror("Connection closed by client\n");

    nleft -= nread;
    buff += nread;

  }
  
  return (sizeToRead - nleft);
}

int XDAQSpy::GetNextBuffer( char* buff ){

  int size = sizeof( I2O_SEND_TO_BU_MESSAGE_FRAME );
  
  char *buffer_header = new char[size];
  int nread = ReadAll(sockfd,buffer_header,size);

  if(fVerbose > 10)
    printf("nread header = %i",nread);

  I2O_SEND_TO_BU_MESSAGE_FRAME *i2omsg = ( I2O_SEND_TO_BU_MESSAGE_FRAME* ) buffer_header;
  
  if(fVerbose > 20)
    printf( "Version offset = %u \n MsgFlags = %u \n MessageSize = %u \n TargetAddress = %u \n InitiatorAddress = %u \n Function = %u \n InitiatorContext = %lu\n",
	   i2omsg->PvtMessageFrame.StdMessageFrame.VersionOffset ,
	   i2omsg->PvtMessageFrame.StdMessageFrame.MsgFlags ,
	   i2omsg->PvtMessageFrame.StdMessageFrame.MessageSize ,
	   i2omsg->PvtMessageFrame.StdMessageFrame.TargetAddress,
	   i2omsg->PvtMessageFrame.StdMessageFrame.InitiatorAddress,
	   i2omsg->PvtMessageFrame.StdMessageFrame.Function,
	   i2omsg->PvtMessageFrame.StdMessageFrame.InitiatorContext );
  
  uint32_t buf_size = i2omsg->PvtMessageFrame.StdMessageFrame.MessageSize.LowPart << 2;

  if(buf_size == 0) return 0;
  if(fVerbose > 10)
    printf("Buffer size = %i ",buf_size);

  int data_size = buf_size - size;

  if(fVerbose > 10)
    printf("Data size = %i, Size of header %i\n", data_size, size);
  
  nread = ReadAll(sockfd,buff,data_size);

  if(fVerbose > 10){    
    printf("First 20: ");
    for(int j = 0 ; j < 20 ; ++j)
      printf("%02X ",(unsigned char)buff[j]);    
    printf("\nLast 28: ");
    for(int j = nread-28 ; j < nread ; ++j)
      printf("%02X ",(unsigned char)buff[j]);    
    printf("\n");
    printf("Read %i\n",nread);
  }
  
  delete []buffer_header;

  return nread;
  
}
