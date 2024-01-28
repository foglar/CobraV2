package main

import (
	"fmt"
	"github.com/tarm/serial"
	"log"
	"os"
	"time"
)

func portInput() string {
	var port string
	for true {
		port = ""
		fmt.Print("Enter port to listen on (for example - '/dev/ttyACM0'): ")
		fmt.Scanln(&port)

		if port == "" {
			port = "/dev/ttyACM0"
		}

		_, err := os.Stat(port)
		if !os.IsNotExist(err) {
			break
		} else {
			fmt.Println("Invalid port")
		}
	}

	return port
}

func baudrateInput() int {
	var baudrate int
	var s_baudrate string

	fmt.Print("Enter baudrate (for example - '9600'): ")
	fmt.Scanln(&s_baudrate)

	switch s_baudrate {
	case "4800":
		baudrate = 4800
	case "9600":
		baudrate = 9600
	case "115200":
		baudrate = 115200
	default:
		baudrate = 9600
	}
	return baudrate
}

func UserInput() (string, int) {
	port := portInput()
	baudrate := baudrateInput()

	return port, baudrate
}

func getTime() string {
	return time.Now().Format("2006-1-2_15:4:5")
}

func main() {
	// User input
	port, baudrate := UserInput()

	// Listen on port
	fmt.Printf("Listening on port %s with baudrate %d:\n", port, baudrate)
	c := &serial.Config{Name: port, Baud: baudrate}

	// Create file
	filename := getTime() + ".txt"
	f, err := os.Create(filename)
	if err != nil {
		log.Fatal(err)
	}
	defer f.Close()

	// Open port
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	var n int
	for {
		// create a buffer
		buf := make([]byte, 254)

		// Read length of serial input
		n, err = s.Read(buf)
		if err != nil {
			log.Fatal(err)
		}

		// Print as a string
		fmt.Print(string(buf[:n]))
		f.WriteString(string(buf[:n]))
	}
}
