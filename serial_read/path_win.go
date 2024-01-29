//go:build windows
// +build windows

package main

import (
	"github.com/mitchellh/go-homedir"
	"log"
)

var (
	HOME string
	PATH string
)

func init() {
	var err error
	HOME, err = homedir.Dir()
	if err != nil {
		log.Fatal(err)
	}

	PATH = HOME + "\\Downloads"
}
