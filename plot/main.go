package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"sort"
	"path"
//	"strconv"

//	"gonum.org/v1/plot"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: glspeaksplot <filename.tsv>")
		return}

	fileName := os.Args[1]
	parentDir := path.Dir(fileName)

	plotsDir := path.Join(parentDir, "plots")
	err := os.Mkdir(plotsDir, 0755) // 0755 sets permissions for the new directory
	if err != nil {
		fmt.Println("Error creating directory:", err)
		return
	}



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

	for _, row := range data {row[4] = strings.ReplaceAll(row[4], ".", "")
	if len(row[4]) > 4 {row[4] = "9999"}}

	sort.Slice(data, func(i, j int) bool {
		return data[i][4] > data[j][4]})

	// Print the array.
	//fmt.Println(data)
	for _, row := range data {fmt.Println(row)}
}
