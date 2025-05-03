package main

import (
    "fmt"
    "io"
    "log"
    "net"
    "os"
)

func main() {
    if len(os.Args) != 2 {
        log.Fatal("Usage: go run backend.go <port>")
    }
    
    port := os.Args[1]
    listener, err := net.Listen("tcp", ":"+port)
    if err != nil {
        log.Fatal(err)
    }
    
    fmt.Printf("Backend server listening on port %s\n", port)
    
    for {
        conn, err := listener.Accept()
        if err != nil {
            continue
        }
        
        go handleConnection(conn, port)
    }
}

func handleConnection(conn net.Conn, port string) {
    defer conn.Close()
    
    // initial greeting
    greeting := fmt.Sprintf("Connected to backend on port %s\n", port)
    conn.Write([]byte(greeting))
    
    // send back what we receive
    buffer := make([]byte, 1024)
    for {
        n, err := conn.Read(buffer)
        if err != nil {
            if err != io.EOF {
                log.Printf("Read error: %v", err)
            }
            return
        }
        
        // identify server by port
        response := fmt.Sprintf("[Backend %s]: %s", port, string(buffer[:n]))
        conn.Write([]byte(response))
    }
}