package serial_read

import (
	"github.com/tarm/serial"
	"log"
)

// TODO:
// - Validation of port and baudrate
// - And input of port and baudrate

// SerialHandler is a struct to handle serial communication
type SerialHandler struct {
	port *serial.Port
}

// NewSerialHandler initializes a new SerialHandler
func NewSerialHandler() (*SerialHandler, error) {
	// CALL port and baudrate input from user !!!
	s, err := serial.OpenPort(&serial.Config{Name: "/dev/ttyACM0", Baud: 9600})
	if err != nil {
		log.Fatal(err)
		return nil, err
	}

	return &SerialHandler{port: s}, nil
}

// ReadSerial reads from the serial port and returns the received data as a string
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
