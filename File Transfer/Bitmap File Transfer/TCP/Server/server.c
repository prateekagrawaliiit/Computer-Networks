#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>

void print_error(const char* msg)
{
	perror(msg);
	_exit(1);
}

int main()
{
	//declarations
	int s_sock, c_sock;
	//Declare the socket 
	s_sock = socket(AF_INET, SOCK_STREAM, 0);

	if(s_sock<0)
		print_error("[-] ERROR IN SOCKET CREATION \n");
	

	struct sockaddr_in server, other;
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));

	server.sin_family = AF_INET;
	server.sin_port = htons(9090);
	server.sin_addr.s_addr = INADDR_ANY;

	//BIND THE SERVER WITH THE CLIENT
	if(bind(s_sock, (struct sockaddr*)&server, sizeof(server)) != -1)
	{
		printf("This is the server speaking \n");
		socklen_t add;
		add = sizeof(other);
		
		//Declare the number of clients the server can listen to at a time (here say 5)
		listen(s_sock, 5);
		
		//Accept the connection request from the clients
		c_sock = accept(s_sock, (struct sockaddr*)&other, &add);
	}
	else
	{
		print_error("[-] BINDING ERROR \n");
		return 1;
	}


	char filename[100], recieved_file[100] = "", ext[10], buffer[2000], buf[1];

	//Recieve the file name along with the extension
	recv(c_sock, filename, sizeof(filename), 0);
	recv(c_sock, ext, sizeof(ext), 0);

	// Create new file to store the contents of the file
	strcat(recieved_file, filename);
	strcat(recieved_file, "_received.");
	strcat(recieved_file, ext);


	//Create file pointer
	FILE *fp;
	// Open file to write in byte modes
	fp = fopen(recieved_file, "wb");

	// declare end of file
	char eof[1] = "0";

	int flag = 0;
	while(flag == 0)
	{
		// Keep checking for the end of file from the server. As soon as it is recieved set flag to 1 and exit
		recv(c_sock, eof, sizeof(eof), 0);
		
		if(eof[0] == '1') 
			flag = 1;
			
		else 
		{
			//continue receiving data until end of file signal is received from the client
			recv(c_sock, buf, sizeof(buf), 0);
			fprintf(fp, "%c", buf[0]);
		}
	}
	
	printf("[+] FILE RECIEVED SUCCESSFULLY.\n\n");
	printf("[+] CLOSING THE FILE\n\n");

	//Close the file
	fclose(fp);
		
	printf("The following file has been recieved : %s \n", recieved_file);

	//Close server and client connections
	close(c_sock);
	close(s_sock);
	
	return 0;

}