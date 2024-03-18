package serial_read

import (
	"fmt"
	"log"
	"os"
	"strconv"

	"github.com/tarm/serial"
)

// TODO:
// - Validation of port and baudrate
// - And input of port and baudrate

// SerialHandler struct
type SerialHandler struct {
	port *serial.Port
}

// Initialize new SerialHandler
func NewSerialHandler() (*SerialHandler, error) {
	port := inputPort()
	baudrate, err := inputBaudrate()
	if err != nil {
		fmt.Println("Error - Baudrate is not valid number")
	}

	s, err := serial.OpenPort(&serial.Config{Name: port, Baud: baudrate})
	if err != nil {
		log.Fatal(err)
		return nil, err
	}

	return &SerialHandler{port: s}, nil
}

// Reads from the serial port and returns the received data as a string
func (sh *SerialHandler) ReadSerial() (string, error) {
	buf := make([]byte, 128)
	n, err := sh.port.Read(buf)
	if err != nil {
		log.Fatal(err)
		return "", err
	}

	return string(buf[:n]), nil
}

// Close closes the serial port
func (sh *SerialHandler) Close() error {
	return sh.port.Close()
}

func inputPort() string {
	var s_port string

	for {
		fmt.Print("Enter port (/dev/ttyACM0): ")
		fmt.Scanln(&s_port)

		if isPort(s_port) == true {
			break
		}

		fmt.Println("Error - Invalid Port")

	}

	return s_port
}

func inputBaudrate() (int, error) {
	var s_baud string

	for {
		fmt.Print("Enter baudrate (for example 9600): ")
		fmt.Scanln(&s_baud)

		if isBaud(s_baud) == true {
			break
		}
		fmt.Println("Error - Invalid Baudrate")
	}

	return strconv.Atoi(s_baud)
}

func isPort(port string) bool {
	_, err := os.Stat(port)
	if !os.IsNotExist(err) {
		return true
	}
	return false

}

func isBaud(baud string) bool {
	switch baud {
	case "4800":
		return true
	case "9600":
		return true
	case "115200":
		return true
	default:
		return false
	}
}
