package main

import (
    "fmt"
    "io"
    "log"
    "net"
    "sync"
)

// struct to represent a single server
type Backend struct {
    Address string
    ActiveConnections int 
    mutex sync.Mutex
}

// struct to represent the load balancer
type LoadBalancer struct {
    backends []Backend
    mutex    sync.Mutex
}

// least connections load balancing
func (lb *LoadBalancer) NextBackend() *Backend {
    lb.mutex.Lock()
    defer lb.mutex.Unlock()
    
    if len(lb.backends) == 0 {
        return nil
    }
    
    // find backend with least connections
    minConnections := lb.backends[0].ActiveConnections
    selectedBackend := &lb.backends[0]
    
    for i := 1; i < len(lb.backends); i++ {
        if lb.backends[i].ActiveConnections < minConnections {
            minConnections = lb.backends[i].ActiveConnections
            selectedBackend = &lb.backends[i]
        }
    }
    
    return selectedBackend
}

func (b *Backend) IncrementConnections() {
    b.mutex.Lock()
    defer b.mutex.Unlock()
    b.ActiveConnections++
}


func (b *Backend) DecrementConnections() {
    b.mutex.Lock()
    defer b.mutex.Unlock()
    if b.ActiveConnections > 0 {
        b.ActiveConnections--
    }
}

func main() {
    // init loadbalancer
    lb := &LoadBalancer{
        backends: []Backend{
            {Address: "localhost:8081", ActiveConnections: 0},
            {Address: "localhost:8082", ActiveConnections: 0},
            {Address: "localhost:8083", ActiveConnections: 0},
        },
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
    if backend == nil {
        log.Println("No available backends")
        return
    }
    
    // Increment active connections for this backend
    backend.IncrementConnections()
    defer backend.DecrementConnections() // Decrement when connection closes
    
    fmt.Printf("Forwarding connection to: %s (Active connections: %d)\n", 
        backend.Address, backend.ActiveConnections)

    // connect to specified backend
    backendConn, err := net.Dial("tcp", backend.Address)
    if err != nil {
        log.Printf("Failed to connect to backend %s: %v\n", backend.Address, err)
        return
    }
    defer backendConn.Close()

    // bidirectional data transfer
    done := make(chan struct{})
    go func() {
        io.Copy(backendConn, clientConn)  // client -> backend
        done <- struct{}{}
    }()
    
    io.Copy(clientConn, backendConn)     // backend -> client (receive response)
    <-done
}