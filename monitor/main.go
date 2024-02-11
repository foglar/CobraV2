package main

import (
	"log"

	gui "foglar/monitor/gui"
	p "foglar/monitor/parse"
	"foglar/monitor/serial_read"

	"github.com/gopxl/pixel"
	"github.com/gopxl/pixel/pixelgl"
	"github.com/gopxl/pixel/text"
	"golang.org/x/image/colornames"
)

func run() {
	const LOG_FONT string = "gui/JetBrainsMonoNerdFont-Medium.ttf"
	const LOG_SIZE float64 = 12

	const INFO_FONT string = "gui/JetBrainsMonoNerdFont-Medium.ttf"
	const INFO_SIZE float64 = 24

	// Initialize serial connection
	serialHandler, err := serial_read.NewSerialHandler()
	if err != nil {
		log.Fatal(err)
	}

	defer serialHandler.Close()

	// Create window
	cfg := pixelgl.WindowConfig{
		Title:  "Cobra Monitor",
		Bounds: pixel.R(0, 0, 1024, 768),
	}
	win, err := pixelgl.NewWindow(cfg)
	if err != nil {
		panic(err)
	}

	// Load fonts
	log_atlas, err := gui.LoadFont(LOG_FONT, LOG_SIZE)
	if err != nil {
		panic(err)
	}

	info_atlas, err := gui.LoadFont(INFO_FONT, INFO_SIZE)
	if err != nil {
		panic(err)
	}

	logging_serial := text.New(pixel.V(800, 10), log_atlas)
	temperature := text.New(pixel.V(100, 740), info_atlas)

	var temperature_gui string

	// Window update
	for !win.Closed() {

		// Read Serial Port
		data, err := serialHandler.ReadSerial()
		defer serialHandler.Close()
		if err != nil {
			log.Fatal(err)
		}

		// Parsing data
		info := p.Parser(data)

		// Clear screen values
		temperature.Clear()

		// Update information if it is in the parsed block
		if _, ok := info[1]; ok {
			temperature_gui = info[1]
		}

		win.Clear(colornames.Black)

		// Print information to text blocks
		temperature.WriteString("Temperature: " + temperature_gui)
		logging_serial.WriteString(data)

		// Draw information to screen
		logging_serial.Draw(win, pixel.IM)
		temperature.Draw(win, pixel.IM)

		win.Update()
	}
}

func main() {
	pixelgl.Run(run)
}
