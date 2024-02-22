package main

import (
	"fmt"
	"github.com/tarm/serial"
	"log"
	"os"
	"strconv"
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
		}
		fmt.Println("Invalid port")
	}

	return port
}

func baudrateInput() int {
	var baudrate int
	var s_baudrate string

	fmt.Print("Enter baudrate (for example - '9600'): ")
	fmt.Scanln(&s_baudrate)
	baudrate = baudrateCheck(s_baudrate)
	return baudrate
}

func baudrateCheck(s_baudrate string) int {
	var baudrate int
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
	args := os.Args[1:]
	var port string
	var baudrate int

	// parsing command line arguments
	if len(args) == 0 {
		port, baudrate = UserInput()
	} else {
		i := 0
		for i < len(args) {
			if args[i] == "-p" && i+1 < len(args) {
				port = args[i+1]
			}
			if args[i] == "-h" || args[i] == "--help" {
				fmt.Println("-h, --help - prints this message")
				fmt.Println("-p [port] - listens on port")
				fmt.Println("-b [baudrate] - sets the baudrate")
				fmt.Println("-d - runs the script with default values")
				os.Exit(0)

			}
			if args[i] == "-b" && i+1 < len(args) {
				var err error
				baudrate, err = strconv.Atoi(args[i+1])
				if err != nil {
					fmt.Println("Invalid baudrate")
					os.Exit(1)
				}
			}
			if args[i] == "-d" {
				port = "/dev/ACM0"
				baudrate = 9600
			}
			i++
		}
		_, err := os.Stat(port)
		if port == "" || os.IsNotExist(err) {
			port = portInput()
		}
		if baudrate == 0 {
			baudrate = baudrateInput()
		}
	}

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
