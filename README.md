# Load Balancing

## What is load balancing?

A load balancer distributes application traffic to different server endpoints.
### Benefits

High spikes in compute load can cause internal servers to lag or cause inconsistent responses. Load balancers improves the availability, scalability, security (can send information to different backends and localize the effect of an attack of an entire system), and performance.
### Algorithms

#### Static

Uses set rules to assign packet to backend server

Examples:
	Round Robin: Distributes requests sequentially across servers
	Random: Randomly selection
	IP Hash: Assigns based on ip
#### Dynamic

Makes decisions based on real time information about system based on server load, response times, health.

Examples:
	Least connection: Connect to server with least connections
	Weighted Round Robin: Assigns servers based on weightings
	Predictive: Uses historical data and trends to anticipate load

See the `least connections` folder for a concrete implementation of a dynamic algorithm.

### Different Types of Load Balancers

#### OSI model

7 layers to the model. Load balancers generally occur in layers 4 (transport) and layer 7 (application).

Layer 4 load balancers. This is a deployment where the accessible ip address refers to the load balancers ip address. When receiving the request the load balancer changes the destination ip to its selected end server.

Layer 7 load balancer. Distribute information based on data found in application layer http headers, cookies, etc. Allow routing decisions based on given parameters.

# Maglev - Google's Distributed System Specification

## Implementation

Explanation of simplified rendition found within codebase focusing on core topics.
## System Design


![High Level Diagram](images/high-level-diagram.png)

*High-level architecture of the Maglev load balancing system*

# Maglev Machine

Composed of a Controller and Forwarder and in charge of a set of ip servers.

## Controller

Does periodic health checks for the forwarder. Depending on the health the forwarder will cut off connection of maglev machine to vip.

## Forwarder

In charge of forwarding packet to particular backend. Creates hash from packet specifications. First, checks our local connection tracking table. If found within table, returns associated backend. Otherwise uses consistency hash table to associate backend with packet and saves within connection tracking table.

### Connection Tracking

A hash table mapping a tuple to a particular backend. When we receive a tuple with the same configuration as we've already seen, it will be stored in our connection tracking table. This allows for session persistence even if our consistency hashing changes.

## Consistency Hash

Creates a large hash table with 100x the amount of backends we have. Creates a preference ordering for each backend for the indexes found within the hash table. In Round Robin fashion, iteratively assigns hash table indexes to a specific backend according to preference lists.

Importantly, our consistency hash table is shared across all maglev machines such that we have a high likelihood of hitting the same backend resource if given a packet with the same configuration (as it will hash to the same backend resource) across our tables. Hence the "consistency" in consistency hash.

# Further Reading (Theory)

The implementation is a simplified rendition aimed at showing concrete code for the important concepts discussed. The theoretical underpinnings of the maglav architecture are discussed in greater detail here.

For complete understanding, reference the original paper found within the references.

## 

  
  

# Todo

  

Break down into disjoint simple pieces to implement.

Understand Hashing Algorithm

Get clear idea of different concepts (maglev machines, forwarder, controller, vip to ip, backend pools) and draw out simplified diagrams