# Basic Load Balancing

2 types
	static - Work the same way regardless of the state of the servers
		Examples
			Round Robin
	Dynamic Load Balancing - Considers state and communicates w the server

We can use a weight parameters to send information to specific servers to a greater degree. If we have homogenous server configuration then we would no use weighting.

# Static Algorithms

	Don't collect metrics but provide standard health checks i.e. if a server is removed then it will no longer be considered

Round Robin Algo
	First Request sent to first server, next request sent to next server and so on

Weighted Round Robin
	Can set weight properties for servers with better specs 

DNS Round Robin
	Instead of having to manually set which server to send the information to the DNS will resolve to a different server

IP Hash
	Uses client ip and server ip to create a unique hash to connect to a specific server

### Dynamic Algorithms

Least Connection
	Send to server with least connection times

# Todo

Understand and incorporate more advanced algorithms

