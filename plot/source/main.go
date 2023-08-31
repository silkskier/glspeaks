package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"sort"
	"path"
	"strconv"
	"sync"
	"runtime" //runtime.GC()
)


func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: glspeaksplot <filename.tsv>")
		return
	}

	fileName := os.Args[1]
	parentDir := path.Dir(fileName)

	plotsDir := path.Join(parentDir, "plots")
	err := os.Mkdir(plotsDir, 0755) // 0755 sets permissions for the new directory
	if err != nil {
		fmt.Println("Error creating directory:", err)
		return
	}
	err = os.Mkdir(plotsDir + "/ecl", 0755) // 0755 sets permissions for the new directory
	if err != nil {
		fmt.Println("Error creating directory:", err)
		return
	}



	var data [][]string

	// Open the file for reading.
	file_tsv, err := os.Open(fileName)
	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}
	defer file_tsv.Close()

	scanner := bufio.NewScanner(file_tsv)

	// Read each line and split it into fields based on tabs.
	for scanner.Scan() {
		file := scanner.Text()
		fields := strings.Split(file, "\t")
		data = append(data, fields)
	}

	if err := scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		return
	}

	data = data[1:]


	for _, file := range data {
		file[4] = strings.ReplaceAll(file[4], ".", "")
		if len(file[4]) > 6 {
			file[4] = "999999"}}

	for _, file := range data {file[4] = file[4][:len(file[4])-2]}

	sort.Slice(data, func(i, j int) bool {
		return data[i][4] > data[j][4]
	})

	// Print the array.
	// fmt.Println(data)
	//for _, file := range data {fmt.Println(file)}



	 numCores := runtime.NumCPU()
    runtime.GOMAXPROCS(numCores)

    // Create a wait group to synchronize goroutines.
    var wg sync.WaitGroup

    // Create a channel to send data to goroutines.
    dataChannel := make(chan []string, numCores)
    defer close(dataChannel) // Close the channel when done.

    // Start numCores goroutines.
    for grID := 0; grID < numCores; grID++ {
        wg.Add(1)
        go func(grID int) {
            defer wg.Done()
            for file := range dataChannel {
                freq, err := strconv.ParseFloat(file[1], 64)
                if err != nil {
                    fmt.Println("Error parsing float:", err)
                    continue
                }
                generatePlot(strings.TrimSuffix(path.Base(file[0]), path.Ext(file[0])), plotsDir, freq, file[4], strings.Trim(file[0], `"`))
            }
        }(grID)
    }

    // Feed data to goroutines in a loop.
    for _, file := range data {
        dataChannel <- file
    }

    // Close the data channel to signal goroutines to exit when done.
    close(dataChannel)

    // Wait for all goroutines to finish.
    wg.Wait()
}
