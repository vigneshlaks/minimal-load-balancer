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


## What is load balancing?

A load balancer essentially distributes application traffic to different server endpoints. Sometimes it's important for send information to the same server. This is known as session persistence.

### Benefits

High spikes in compute load can cause internal servers to lag or cause inconsistent responses. Improves the availability, scalability, security (can send information to different backends and localize the effect of an attack of an entire system), and performance.

### Algorithms

Round robin, least connection, random with 2 choices... etc.

### Different Types of Load Balancers

#### OSI model

7 layers to the model. Load balancers generally occur in layers 4 (transport) and layer 7 (application). 

Layer 4 load balancers
	This is a deployment where the accessible ip address refers to the load balancers ip address. When receiving the request the load balancer changes the destination ip to its selected end server.

Layer 7 load balancer
	Distribute information based on data found in application layer http headers, cookies, etc.
	Allow routing decisions based on given parameters

# Todo

Understand and incorporate more advanced algorithms

