package main

import (
	//"fmt"
	gui "foglar/monitor/gui"
	//p "foglar/monitor/parse"
	//"foglar/monitor/serial_read"
	//"log"
)

func main() {
	//	 // Initialize serial connection
	//		serialHandler, err := serial_read.NewSerialHandler()
	//		if err != nil {
	//			log.Fatal(err)
	//		}
	//
	//		defer serialHandler.Close()
	//
	//		for {
	//			// Read serial data
	//			data, err := serialHandler.ReadSerial()
	//			if err != nil {
	//				log.Fatal(err)
	//			}
	//
	//			fmt.Println("Received data:", data)
	//			fmt.Println(p.Parser(data))
	//	}

	// Testing GUI interface
	gui.GUI()
}
