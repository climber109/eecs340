#include "minet_socket.h"
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

#define BUFSIZE 1024
#define FILENAMESIZE 100

int handle_connection(int);
int writenbytes(int,char *,int);
int readnbytes(int,char *,int);

int main(int argc,char *argv[])
{
  int server_port;
  int listeningSocket, acceptedSocket;
  struct sockaddr_in listiningSocketAddress,acceptedSocketAddress;
  int rc,i;
  fd_set readlist;
  fd_set connections;
  int maxfd;


  /* parse command line args */
  if (argc != 3)
  {
    fprintf(stderr, "usage: http_server1 k|u port\n");
    exit(-1);
  }
  server_port = atoi(argv[2]);
  if (server_port < 1500)
  {
    fprintf(stderr,"INVALID PORT NUMBER: %d; can't be < 1500\n",server_port);
    exit(-1);
  }

  /
  /* initialize minet */
  if (toupper(*(argv[1])) == 'K') {
    minet_init(MINET_KERNEL);  
  } else if (toupper(*(argv[1])) == 'U') {
    minet_init(MINET_USER);
  } else {
    fprintf(stderr, "First argument must be k or u\n");
    exit(-1);
  }

  /* initialize and make socket */
  if ((listeningSocket = minet_socket (SOCK_STREAM)) < 0) {
    minet_perror("Error creating socket");
    minet_deinit();
    return -1;
  }

  /* set server address*/
  memset(&listeningSocketAddress, 0, sizeof(listeningSocketAddress));
  listeningSocketAddress.sin_port = htons(server_port);
  listeningSocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  listeningSocketAddress.sin_family = AF_INET;
 
  /* bind listening socket */
  if((minet_bind(listeningSocket, &listeningSocketAddress)) < 0) {
    minet_perror("Error binding socket");
    minet_deinit();
    return -1;
  }
  //Start listening
 
  if((minet_listen(listeningSocket, 1)) < 0) {
    minet_perror("Error listening on socket");
    minet_deinit();
    return -1;
  }


  /* initalize the list of open connections to empty */
  FD_ZERO(&connections);
  FD_ZERO(&readlist);
  maxfd = 0;

  /* connection handling loop */
  while(1)
  {
    /* create read list */
    

    /* do a select */
    rc = minet_select(maxfd+1, &readlist, NULL, NULL, NULL);   

    /* process sockets that are ready */
    for (i = 0; i < maxfd; i++) {
      if (FD_ISSET(i,&readlist) {

	  /* for the accept socket, add accepted connection to connections */
	  if (i == listeningSocket)
	    {
	      if((acceptedSocket = minet_accept(listeningSocket, &acceptedSocketAddress)) < 0) {
		minet_perror("Error accepting socket");
		//minet_deinit();
		//return -1;
	      }
	      FD_SET(acceptedSocket,&connections);
	      //TODO: update new state for this socket 
	      if (acceptedSocket > maxfd) maxfd = acceptedSocket;
	    }
	  else /* for a connection socket, handle the connection */
	    {
	      rc = handle_connection(i);
	    }
	}      
    }
  }
}

int handle_connection(int sock2)
{
  char filename[FILENAMESIZE+1];
  int rc;
  int fd;
  struct stat filestat;
  char buf[BUFSIZE+1];
  char *headers;
  char *endheaders;
  char *bptr;
  int datalen=0;
  char *ok_response_f = "HTTP/1.0 200 OK\r\n"\
                      "Content-type: text/plain\r\n"\
                      "Content-length: %d \r\n\r\n";
  char ok_response[100];
  char *notok_response = "HTTP/1.0 404 FILE NOT FOUND\r\n"\
                         "Content-type: text/html\r\n\r\n"\
                         "<html><body bgColor=black text=white>\n"\
                         "<h2>404 FILE NOT FOUND</h2>\n"\
                         "</body></html>\n";
  bool ok=true;

  /* first read loop -- get request and headers*/

  /* parse request to get file name */
  /* Assumption: this is a GET request and filename contains no spaces*/

    /* try opening the file */

  /* send response */
  if (ok)
  {
    /* send headers */

    /* send file */
  }
  else	// send error response
  {
  }

  /* close socket and free space */

  if (ok)
    return 0;
  else
    return -1;
}

int readnbytes(int fd,char *buf,int size)
{
  int rc = 0;
  int totalread = 0;
  while ((rc = minet_read(fd,buf+totalread,size-totalread)) > 0)
    totalread += rc;

  if (rc < 0)
  {
    return -1;
  }
  else
    return totalread;
}

int writenbytes(int fd,char *str,int size)
{
  int rc = 0;
  int totalwritten =0;
  while ((rc = minet_write(fd,str+totalwritten,size-totalwritten)) > 0)
    totalwritten += rc;

  if (rc < 0)
    return -1;
  else
    return totalwritten;
}

