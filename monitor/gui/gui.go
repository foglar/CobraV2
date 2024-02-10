package gui

// TODO: finish reading serial input and parsing it and piping it to the gui component

import (
	"io"
	"os"
	"time"

	"github.com/golang/freetype/truetype"
	"github.com/gopxl/pixel"
	"github.com/gopxl/pixel/pixelgl"
	"github.com/gopxl/pixel/text"
	"golang.org/x/image/colornames"
	"golang.org/x/image/font"
)

func loadTTF(path string, size float64) (font.Face, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	bytes, err := io.ReadAll(file)
	if err != nil {
		return nil, err
	}

	font, err := truetype.Parse(bytes)
	if err != nil {
		return nil, err
	}

	return truetype.NewFace(font, &truetype.Options{
		Size:              size,
		GlyphCacheEntries: 1,
	}), nil
}

func run() {
	cfg := pixelgl.WindowConfig{
		Title:  "Cobra Monitor",
		Bounds: pixel.R(0, 0, 1024, 768),
	}
	win, err := pixelgl.NewWindow(cfg)
	if err != nil {
		panic(err)
	}

	face, err := loadTTF("gui/JetBrainsMonoNerdFont-Medium.ttf", 36)
	if err != nil {
		panic(err)
	}

	atlas := text.NewAtlas(face, text.ASCII)
	txt := text.New(pixel.V(100, 500), atlas)

	fps := time.Tick(time.Second / 120)

	for !win.Closed() {

		//txt.WriteString(data)

		win.Clear(colornames.Black)
		txt.Draw(win, pixel.IM)
		win.Update()

		<-fps
	}
}

func GUI() {
	pixelgl.Run(run)
}
