// Dependencies
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
	exit(1);
}


int main()
{		

	// Declare client socket
	int c_sock;

	// Create client socket 
	c_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (c_sock < 0)
		print_error("[-] SOCKET CREATION FAILURE\n");	


	// Declarations

	struct sockaddr_in client;
	socklen_t client_len;
		

	// Ensure that adequate memory is provided to client

	memset(&client, 0, sizeof(client));
	
	client_len = sizeof(client);
	client.sin_family = AF_INET;
	client.sin_port = htons(9090);
	client.sin_addr.s_addr = INADDR_ANY;

	printf("[+] This is the client speaking\n\n");

	char filename[100], recieved_file[100], ext[10], buffer[20000], buf[1];

	printf("Please enter the file name without extension: ");
	scanf("%s", filename);

	printf("Please enter the extension of the file: ");
	scanf("%s", ext);

	//Generate new filename using filename and extension
	strcpy(recieved_file, filename);
	strcat(recieved_file, ".");
	strcat(recieved_file, ext);

	printf("\nInput File Name: %s.\n\n", recieved_file);

	//Open the file in read bytes mode
	FILE *fp;
	fp = fopen(recieved_file, "rb");

	printf("[+] FILE OPENED SUCCESSFULLY %s.\n", recieved_file);

	char eof[1] = "0";

	//Send the name of the file and the extension to the server
	sendto(c_sock, filename, sizeof(filename), 0, (struct sockaddr*)&client, sizeof(client));
	sendto(c_sock, ext, sizeof(ext), 0, (struct sockaddr*)&client, sizeof(client));

	while(!feof(fp))
	{
		//Scan the file bit by bit and send the data to the server
		fscanf(fp, "%c", &buf[0]);
		//Send the end of file status to the server
		sendto(c_sock, eof, sizeof(eof), 0, (struct sockaddr*)&client, sizeof(client));
		//Send the content of the file to the server
		sendto(c_sock, buf, sizeof(buf), 0, (struct sockaddr*)&client, sizeof(client));
	}
	
	printf("\n\n[+] END OF FILE RECIEVED.\n\n");
	
	//Send a signal to the server indicating the end of file
	eof[0] = '3';
	sendto(c_sock, eof, sizeof(eof), 0, (struct sockaddr*)&client, sizeof(client));

	printf("[+] The file has been sent successfully.\n");
	printf("[+] Shutting down the client.\n");

	//Close the client
	close(c_sock);

	return 0;
}


