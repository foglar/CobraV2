package gui

// TODO: finish reading serial input and parsing it and piping it to the gui component

import (
	"io"
	"os"

	"github.com/golang/freetype/truetype"
	"github.com/gopxl/pixel/text"
	"golang.org/x/image/font"
)

// Declare default font and font size
const FONT string = "gui/JetBrainsMonoNerdFont-Medium.ttf"
const SIZE float64 = 36

func LoadFont() (*text.Atlas, error) {
	face, err := LoadTTF(FONT, SIZE)
	if err != nil {
		return nil, err
	}
	return text.NewAtlas(face, text.ASCII), nil
}

func LoadTTF(path string, size float64) (font.Face, error) {
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
