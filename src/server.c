#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

#define SRV_PORT 5555
#define CLNT_PORT 6666

int main ()
{
	short port;
	long int ttime;
	int sock, new_sock, fd_fork;
	int from_len, len;
	char buf[27];
	struct sockaddr_in s_addr, clnt_addr, new_s_addr;
/*
 * Fabricate socket  and initiate socket options
 */
	sock = socket (AF_INET, SOCK_DGRAM, 0);
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
	s_addr.sin_port = htons (SRV_PORT);
	if (bind (sock, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
    {
		perror ("First bind error!\n");
		exit (1);
	}
	len = sizeof(struct sockaddr_in);
	port = 7777;
	while (1) /* main service loop */
    {
		while (2) /* waiting request loop */
        {
			from_len = 
                recvfrom (sock, buf, 27, 0, (struct sockaddr *)&s_addr, &len);
			if (from_len > 0)
            {
				write (1, buf, from_len);
				port++;
				break;
			}
		}
		printf ("Sending message to client...\n");
		sendto (sock, &port, sizeof(port), 0, (struct sockaddr *)&s_addr,
                sizeof (s_addr));	
		fd_fork = fork();
		if (fd_fork == (-1))
        {
			perror ("Fork create error!\n");
			exit (1);
		} 
        else if (fd_fork == 0) /* doughter part */
        {
			close (sock);
			new_sock = socket (AF_INET, SOCK_DGRAM, 0);
			new_s_addr.sin_family = AF_INET;
			new_s_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
			new_s_addr.sin_port = htons (port);
			if (bind (new_sock, (struct sockaddr *)&new_s_addr,
               sizeof (new_s_addr)) < 0)
            {
				perror("Second bind error!\n");
				exit(1);
			}
			len = sizeof (struct sockaddr_in);
			while (3) /* request time loop */
            {
				from_len = recvfrom (new_sock, buf, 19, 0,
                    (struct sockaddr *)&new_s_addr, &len);
				if (from_len > 0)
                {
					write (1, buf, from_len);
					ttime = time (NULL);
					break;
				}
			}
/*
 * Sending time to client.
 */
		printf ("Sending message to client... again...\n");
		sendto (new_sock, ctime(&ttime), 30, 0, (struct sockaddr *)&new_s_addr,
                sizeof (new_s_addr));
		}
	}
	close (new_sock);
	close (sock);
	return 1;
}
