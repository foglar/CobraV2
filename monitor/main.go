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
	LOG_POSITION      = [2]float64{900, 10}
	TEMP_POSITION     = [2]float64{600, 100}
	PRESSURE_POSITION = [2]float64{200, 100}
	ATTITUDE_POSITION = [2]float64{50, 400}
	ROLL_POSITION     = [2]float64{50, 650}
	PITCH_POSITION    = [2]float64{350, 650}
	YAW_POSITION      = [2]float64{650, 650}
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
	temperature := text.New(pixel.V(TEMP_POSITION[0], TEMP_POSITION[1]), info_atlas)
	pressure := text.New(pixel.V(PRESSURE_POSITION[0], PRESSURE_POSITION[1]), info_atlas)
	attitude := text.New(pixel.V(ATTITUDE_POSITION[0], ATTITUDE_POSITION[1]), info_atlas)
	roll := text.New(pixel.V(ROLL_POSITION[0], ROLL_POSITION[1]), info_atlas)
	pitch := text.New(pixel.V(PITCH_POSITION[0], PITCH_POSITION[1]), info_atlas)
	yaw := text.New(pixel.V(YAW_POSITION[0], YAW_POSITION[1]), info_atlas)

	var (
		temperature_gui string
		pressure_gui    string
		attitude_gui    string
		roll_gui        string
		pitch_gui       string
		yaw_gui         string
	)

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
		pressure.Clear()
		attitude.Clear()
		roll.Clear()
		pitch.Clear()
		yaw.Clear()

		// Update information if it is in the parsed block
		if _, ok := info[1]; ok {
			temperature_gui = info[4]
			pressure_gui = info[5]
			attitude_gui = info[6]
			roll_gui = info[1]
			pitch_gui = info[2]
			yaw_gui = info[3]
		}

		win.Clear(colornames.Black)

		// Print information to text blocks
		logging_serial.WriteString(data)
		temperature.WriteString("Temperature: " + temperature_gui)
		pressure.WriteString("Pressure: [hPa] " + pressure_gui)
		attitude.WriteString("Attitude: [m] " + attitude_gui)
		roll.WriteString("Roll: " + roll_gui)
		pitch.WriteString("Pitch: " + pitch_gui)
		yaw.WriteString("Yaw: " + yaw_gui)

		// Draw information to screen
		logging_serial.Draw(win, pixel.IM)
		temperature.Draw(win, pixel.IM)
		pressure.Draw(win, pixel.IM)
		attitude.Draw(win, pixel.IM)
		roll.Draw(win, pixel.IM)
		pitch.Draw(win, pixel.IM)
		yaw.Draw(win, pixel.IM)

		win.Update()
	}
}

func main() {
	pixelgl.Run(run)
}
