package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"sort"
	"path"
	"strconv"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/plotutil"
	"gonum.org/v1/plot/vg"
	"gonum.org/v1/plot/vg/draw"
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

	for _, file := range data {
		file[4] = strings.ReplaceAll(file[4], ".", "")
		if len(file[4]) > 4 {
			file[4] = "9999"
		}
	}

	sort.Slice(data, func(i, j int) bool {
		return data[i][4] > data[j][4]
	})

	// Print the array.
	// fmt.Println(data)
	for _, file := range data {
		fmt.Println(file)
	}






	for _, file := range data {	//main loop
	filename := strings.TrimSuffix(path.Base(file[0]), path.Ext(file[0]))
	//frequency := file[1]

	plotname := plotsDir + "/" + file[4] + "_" + filename + ".png"
	//fmt.Println(plotname)

	photometry_location := strings.Trim(file[0], `"`)

	var local_data [][]float64
	// Open the file for reading.
	loc_file, loc_err := os.Open(photometry_location)
	if loc_err != nil {
		fmt.Println("Error opening file:", loc_err)
		return
	}
	defer loc_file.Close()

	local_scanner := bufio.NewScanner(loc_file)
	// Read each line and split it into fields based on spaces.
	for local_scanner.Scan() {
		line := local_scanner.Text()
		local_fields := strings.Split(line, " ")

		// Convert the string fields to float64 values.
		var floatFields []float64
		for _, field := range local_fields {
			val, err := strconv.ParseFloat(field, 64)
			if err != nil {
				fmt.Println("Error parsing float:", err)
				return
			}
			floatFields = append(floatFields, val)
		}

		local_data = append(local_data, floatFields)
	}

	if err := local_scanner.Err(); err != nil {
		fmt.Println("Error reading file:", err)
		return
	}

	// Print the array	for _, line := range local_data {fmt.Println(line)}



		//Plot the measurements
		plot_data := make(plotter.XYs, len(local_data))
	for i, line := range local_data {
		plot_data[i].X = line[0]
		plot_data[i].Y = line[1]
	}

	plt := plot.New()
	if err != nil {
		panic(err)
	}

	// Set the scatter plot style to use filled circles.
	s, err := plotter.NewScatter(plot_data)
	if err != nil {
		panic(err)
	}
	s.GlyphStyle.Shape = draw.CircleGlyph{}
	s.GlyphStyle.Color = plotutil.DefaultColors[0]

	plt.Add(s)

	// Set the plot size in pixels (width x height).
	widthPixels, heightPixels := 600, 300

	// Save the plot to a file with the specified size in pixels.
	err = plt.Save(vg.Length(widthPixels), vg.Length(heightPixels), plotname)
	if err != nil {
		panic(err)
	}


}}
