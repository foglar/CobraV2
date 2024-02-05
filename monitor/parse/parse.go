package parse

import (
	"log"
	"strconv"
	"strings"
)

func Parser(s string) map[int]string {

	// TODO: check if line isn't comment
	// improve reading data
	lines := strings.Split(s, "\n")
	data_structure := make(map[int]string)
	for _, line := range lines {
		// find $ and * in text and get value between them
		startIndex := strings.Index(line, "$")
		endIndex := strings.Index(line, "*")
		if startIndex != -1 && endIndex != -1 {
			value := line[startIndex+1 : endIndex]
			data := strings.Split(strings.TrimSpace(value), ";")
			ident, err := strconv.Atoi(strings.TrimSpace(data[0]))
			if err != nil {
				log.Print(err)
			}
			info := data[1]
			data_structure[ident] = info
		}
	}

	return data_structure
}
