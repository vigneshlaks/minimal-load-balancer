// test.go
package main

import (
    "bufio"
    "fmt"
    "net"
    "strings"
    "sync"
)

func main() {
    fmt.Println("Load Balancer Test Suite")
    fmt.Println("------------------------")
    
    testConnectivity()
    testLoadDistribution()
    testConcurrent()
}

// test basic connection to load balancer
func testConnectivity() {
    fmt.Print("Testing basic connectivity... ")
    
    conn, err := net.Dial("tcp", "localhost:8080")
    if err != nil {
        fmt.Printf("Failed: %v\n", err)
        return
    }
    defer conn.Close()
    
    reader := bufio.NewReader(conn)
    greeting, _ := reader.ReadString('\n')
    
    if strings.Contains(greeting, "Connected to backend") {
        fmt.Println("Passed")
    } else {
        fmt.Println("Failed")
    }
}

// test load distribution across backends
func testLoadDistribution() {
    fmt.Print("Testing load distribution... ")
    
    backendCounts := make(map[string]int)
    
    // make multiple connections
    for i := 0; i < 9; i++ {
        conn, err := net.Dial("tcp", "localhost:8080")
        if err != nil {
            continue
        }
        
        reader := bufio.NewReader(conn)
        greeting, _ := reader.ReadString('\n')
        
        // extract port from greeting
        if strings.Contains(greeting, "port") {
            parts := strings.Split(greeting, "port ")
            if len(parts) > 1 {
                port := strings.TrimSpace(parts[1])
                backendCounts[port]++
            }
        }
        conn.Close()
    }
    
    // check if all 3 backends were used
    if len(backendCounts) == 3 {
        fmt.Println("Passed")
        for port, count := range backendCounts {
            fmt.Printf("  Port %s: %d connections\n", port, count)
        }
    } else {
        fmt.Printf("Failed: Only %d backends used\n", len(backendCounts))
    }
}

// test concurrent connections
func testConcurrent() {
    fmt.Print("Testing concurrent connections... ")
    
    var wg sync.WaitGroup
    successCount := 0
    var mutex sync.Mutex
    
    // create 10 concurrent connections
    for i := 0; i < 10; i++ {
        wg.Add(1)
        go func(id int) {
            defer wg.Done()
            
            conn, err := net.Dial("tcp", "localhost:8080")
            if err != nil {
                return
            }
            defer conn.Close()
            
            fmt.Fprintf(conn, "Test message %d\n", id)
            
            buffer := make([]byte, 1024)
            n, err := conn.Read(buffer)
            if err == nil && n > 0 {
                mutex.Lock()
                successCount++
                mutex.Unlock()
            }
        }(i)
    }
    
    wg.Wait()
    
    if successCount == 10 {
        fmt.Println("Passed")
    } else {
        fmt.Printf("Failed: %d/10 successful\n", successCount)
    }
}