package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
//	"strconv"

//	"gonum.org/v1/plot"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: glspeaksplot <filename.tsv>")
		return}

	fileName := os.Args[1]
	var data [][]string

	// Open the file for reading.
	file, err := os.Open(fileName)
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file.Close()


	scanner := bufio.NewScanner(file)

	// Read each line and split it into fields based on tabs.
	for scanner.Scan() {
		line := scanner.Text()
		fields := strings.Split(line, "\t")
		data = append(data, fields)}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		return}

	// Print the array.
		//fmt.Println(data)
		//fmt.Println(data[3][3])




	for _, row := range data {fmt.Println(row)}
}
