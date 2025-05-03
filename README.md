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

# Maglev Summary

System consists of multiple backends located between routers and endpoints. Maglev machines are responsible for matching packets to services and spreading evenly.

Maglev uses VIPs which connect to multiple endpoints. Routers have access to these VIPs (essentially acting as pseudo ip addresses). DNS sends to frontend location (specific VIP). Forwards packets to a Maglev machine via ECMP.

Each machine contains a controller and forwarder. Controller states the health of the forwarder and cuts off connection to specific Maglev Machine if it’s not healthy. Forwarders contain backend pools connecting to real ip endpoints. Forwarder handles forwarding of connections.

Forwarder’s design is optimized for speed by bypassing the linux kernel network stack. Performs a 5 tuple hashing to distribute packets to threads which then select backends.

Connection oriented protocols like TCP need all packets sent to the same backend server. To do this they use a combination of consistent hashing and connection tracking. The 5 tuple hash is stored for quick look up. If this does not work, no healthy backend or no connection found, a new one is set using consistent hashing.

Consistent hashing helps mitigate the effects of changing the maglev machine.

Algorithm prioritizes load balancing by allowing backends to take turns filling consistency table. 

# Maglev Summary

![High Level Diagram](images/high-level-diagram.png)
*High-level architecture of the Maglev load balancing system*


# Todo

Break down into disjoint simple pieces to implement.
    Understand Hashing Algorithm
    Get clear idea of different concepts (maglev machines, forwarder, controller, vip to ip, backend pools) and draw out simplified diagrams
