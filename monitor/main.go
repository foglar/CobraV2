package main

import (
	"foglar/monitor/gui"
	"foglar/monitor/parse"
	"foglar/monitor/serial_read"

	"github.com/gopxl/pixel"
	"github.com/gopxl/pixel/pixelgl"
	"github.com/gopxl/pixel/text"
	"golang.org/x/image/colornames"
)

const (
	LOG_FONT  = "gui/JetBrainsMonoNerdFont-Medium.ttf"
	INFO_FONT = "gui/JetBrainsMonoNerdFont-Medium.ttf"

	LOG_SIZE  = 12
	INFO_SIZE = 24
)

var (
	LOG_POSITION  = [2]float64{800, 10}
	INFO_POSITION = [2]float64{100, 740}
)

func run() {
	// Initialize serial connection
	serialHandler, err := serial_read.NewSerialHandler()
	if err != nil {
		panic(err)
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

	// Text
	logging_serial := text.New(pixel.V(LOG_POSITION[0], LOG_POSITION[1]), log_atlas)
	temperature := text.New(pixel.V(INFO_POSITION[0], INFO_POSITION[1]), info_atlas)

	var temperature_gui string

	// Window update
	for !win.Closed() {

		// Read Serial Port
		data, err := serialHandler.ReadSerial()
		defer serialHandler.Close()
		if err != nil {
			panic(err)
		}

		// Parsing data
		info := parse.Parser(data)

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
