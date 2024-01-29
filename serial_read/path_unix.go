//go:build !windows
// +build !windows

package main

import (
	"fmt"
	"github.com/mitchellh/go-homedir"
	"log"
	"os"
)

var (
	HOME string
	PATH string
)

// Check if the directory exists
func init() {

	var err error
	HOME, err = homedir.Dir()
	if err != nil {
		log.Fatal(err)
	}

	PATH = HOME + "/.config/serial-monitor/"

	if _, err := os.Stat(PATH); os.IsNotExist(err) {
		// Directory does not exist, create it
		err := os.MkdirAll(PATH, os.ModePerm)
		if err != nil {
			fmt.Println("Error creating directory:", err)
		} else {
			fmt.Println("Directory created:", PATH)
		}
	}
}
