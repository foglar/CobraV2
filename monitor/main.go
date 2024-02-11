package main

import (
	"log"

	gui "foglar/monitor/gui"
	"foglar/monitor/serial_read"

	"github.com/gopxl/pixel"
	"github.com/gopxl/pixel/pixelgl"
	"github.com/gopxl/pixel/text"
	"golang.org/x/image/colornames"
)

func run() {
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

	// Load font
	atlas, err := gui.LoadFont()
	if err != nil {
		panic(err)
	}

	txt := text.New(pixel.V(100, 500), atlas)

	// Window update
	for !win.Closed() {

		data, err := serialHandler.ReadSerial()
		if err != nil {
			log.Fatal(err)
		}

		txt.WriteString(data)

		win.Clear(colornames.Black)
		txt.Draw(win, pixel.IM)
		win.Update()
	}
}

func main() {
	pixelgl.Run(run)
}
