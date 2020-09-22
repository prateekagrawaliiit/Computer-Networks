//Dependencies
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
	int c_sock;
	
	//SOCKET CREATION
	c_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (c_sock < 0)
		print_error(" ERROR IN SOCKET CREATION");
	
	struct sockaddr_in client;
	
	memset(&client, 0, sizeof(client));
	
	client.sin_family = AF_INET;
	client.sin_port = htons(9090);
	client.sin_addr.s_addr = INADDR_ANY;
	
	//ESTABLISH THE CONNECTION BETWEEN THE SERVER anc CLIENT
	if(connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1)
	{
		print_error(" CONNECTION FAILURE\n");
		return 1;
	}

	printf("This is Client speaking.\n\n");

	char filename[100], extension[100], ext[10], buffer[2000], buf[1];

	printf("Please enter file name without extension: ");
	scanf("%s", filename);

	printf("Please enter the extension of the file : ");
	scanf("%s", ext);

	//Generate complete file name
	strcpy(extension, filename);
	strcat(extension, ".");
	strcat(extension, ext);

	printf("\nThe filename (input recieved) is : %s.\n\n", extension);

	//open the file in read mode
	FILE *fp;
	fp = fopen(extension, "rb");

	printf("[+] %s FILE OPENED SUCCESSFULLY.\n", extension);

	char eof[1] = "0";

	//Send the name of the file and the extension to the server for its use
	send(c_sock, filename, sizeof(filename), 0);
	send(c_sock, ext, sizeof(ext), 0);

	while(!feof(fp))
	{ 
		//Read the file and send its content to the buffer character by character
		fscanf(fp, "%c", &buf[0]);
		send(c_sock, eof, sizeof(eof), 0);
		send(c_sock, buf, sizeof(buf), 0);
	}
	
	printf("[+] FILE READING COMPLETED\n\n");
	
	//send a signal to the server indicating that the file has been sent
	eof[0] = '1';
	send(c_sock, eof, sizeof(eof), 0);
	
	printf("[+] THE FILE HAS BEEN SENT SUCCESSFULLY\n");

	//Close the socket 
	close(c_sock);

	printf("[+] SOCKET CLOSE SUCCESSFULLY\n");


	return 0;
}


