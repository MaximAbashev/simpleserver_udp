# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <time.h>

# define SRV_PORT 5555
# define CLNT_PORT 6666

int main ()
{
	short port;
	int sock, new_sock;
	int from_len, len;
	char buf[30];
	struct sockaddr_in clnt_addr, s_addr, new_s_addr;
/*
 * Fabricate socket and set socket options.
 */
	sock = socket (AF_INET, SOCK_DGRAM, 0);
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
	s_addr.sin_port = htons (SRV_PORT);
	len = sizeof (struct sockaddr_in); /* for recvfrom */
/*
 * Sending message.
 */
	printf ("Sending message...\n");
	sendto (sock, "Can you give me new port?\n", 27, 0,
            (struct sockaddr *)&s_addr, sizeof (s_addr));
	printf("Waiting answer from server...\n");
	while (1) /* waiting anser from server */
    {
		from_len = recvfrom (sock, &port, sizeof (port), 0, 
                (struct sockaddr *)&s_addr, &len);
		printf ("I take port: %d\n", port);
		if (from_len > 0)
        {
			close (sock);
			break;
		}
	}
/*
 * Reconfigure and connect to new socket.
 */
	sleep (3);
	printf ("Connecting to new socket...\n");
	new_sock = socket (AF_INET, SOCK_DGRAM, 0);
	new_s_addr.sin_family = AF_INET;
	new_s_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
	new_s_addr.sin_port = htons (port);
	printf ("Sending message...\n");
	sendto (new_sock, "How much o'clock?\n", 19, 0, 
            (struct sockaddr *)&new_s_addr, sizeof (new_s_addr));
	printf("Waiting answer from new server...\n");
	while (2)
    {
		from_len = 
            recvfrom (sock, buf, 30, 0, (struct sockaddr *)&new_s_addr, &len);
		if (from_len > 0)
        {
			break;
		}
	}
	write (1, buf, from_len);
	close (new_sock);	
	return 1;
}
