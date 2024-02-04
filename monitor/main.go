package main

import (
	"fmt"
	"foglar/monitor/serial_read"
	"log"
	// "foglar/monitor/parse"
)

func main() {
	// Initialize serial connection
	serialHandler, err := serial_read.NewSerialHandler()
	if err != nil {
		log.Fatal(err)
	}

	defer serialHandler.Close()

	// Read serial data
	data, err := serialHandler.ReadSerial()
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("Received data:", data)
}
