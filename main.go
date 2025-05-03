package main

import (
    "fmt"
    "io"
    "log"
    "net"
)

// struct to represent a single server
type Backend struct {
    Address string
}

// struct to represent the load balancer
type LoadBalancer struct {
    backends []Backend
    current  int
}

// round robin load balancing
func (lb *LoadBalancer) NextBackend() Backend {
    backend := lb.backends[lb.current]
    lb.current = (lb.current + 1) % len(lb.backends)
    return backend
}

func main() {
    // init loadbalancer
    lb := &LoadBalancer{
        backends: []Backend{
            {Address: "localhost:8081"},
            {Address: "localhost:8082"},
            {Address: "localhost:8083"},
        },
        current: 0,
    }

    // listen to ports
    listener, err := net.Listen("tcp", ":8080")
    if err != nil {
        log.Fatal(err)
    }
    fmt.Println("Load balancer listening on port 8080...")
    
    for {
        // accept client connection
        clientConn, err := listener.Accept()
        if err != nil {
            log.Println("Error accepting connection:", err)
            continue
        }
        
        // handle connection with goroutine
        go handleClientConnection(lb, clientConn)
    }
}

func handleClientConnection(lb *LoadBalancer, clientConn net.Conn) {
    defer clientConn.Close()

    // get next backend
    backend := lb.NextBackend()
    fmt.Printf("Forwarding connection to: %s\n", backend.Address)

    // connect to specified backend
    backendConn, err := net.Dial("tcp", backend.Address)
    if err != nil {
        log.Printf("Failed to connect to backend %s: %v\n", backend.Address, err)
        return
    }
    defer backendConn.Close()

    // birdirectional data transfer
    go io.Copy(backendConn, clientConn)  // client -> backend
    io.Copy(clientConn, backendConn)     // backend -> client (receive response)
}