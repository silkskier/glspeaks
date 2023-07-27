package main

import (
	"bufio"
	"math"
	"fmt"
	"os"
	"strings"
	"strconv"
	"sort"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/plotutil"
	"gonum.org/v1/plot/vg"
	"gonum.org/v1/plot/vg/draw"
)

type Measurement struct {
  X float64
  Y float64
  Z float64
}

type ByZ []Measurement

func (a ByZ) Len() int           { return len(a) }
func (a ByZ) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a ByZ) Less(i, j int) bool { return a[i].Z < a[j].Z }

/*
func min(arr []float64) float64 {
	min := arr[0]
	for _, value := range arr {if value < min {min = value}}
return min
}

func max(arr []float64) float64 {
	max := arr[0]
	for _, value := range arr {if value > max {max = value}}
return max
}
*/


type customYTicks struct{}
func (customYTicks) Ticks(min, max float64) [] plot.Tick {
	ticks := make([]plot.Tick, 0)

	// Calculate the desired tick spacing based on the range of Y-axis values
	rangeY := max - min
	tickSpacing := rangeY * 0.124755859375 // Eight ticks between min and max

	for v := min; v <= max; v += tickSpacing {
		label := fmt.Sprintf("%.2f", math.Abs(v))
		if rangeY < 0.1 {label = fmt.Sprintf("%.2f", math.Abs(v))}
		tick := plot.Tick{Value: v, Label: label}
		ticks = append(ticks, tick)
	}
	return ticks}


type customXTicks struct{}
func (customXTicks) Ticks(min, max float64) [] plot.Tick {
	ticks := make([]plot.Tick, 0)

	// Calculate the desired tick spacing based on the range of Y-axis values
	rangeX := max - min
	tickSpacing := rangeX * 0.124755859375 // Eight ticks between min and max

	for v := min; v <= max; v += tickSpacing {
		label := fmt.Sprintf("%.2f", math.Abs(v))
		tick := plot.Tick{Value: v, Label: label}
		ticks = append(ticks, tick)
	}
	return ticks}





func generatePlot(file string, outputDir string, frequency float64, match_strength string, photometry_location string){

	plotname := outputDir + "/" + match_strength + "_" + file + ".png"

	var isEclipsing bool = false

	var err error

	var local_data [][]float64
	loc_file, loc_err := os.Open(photometry_location)
	if loc_err != nil {
		fmt.Println("Error opening file:", loc_err)
		return}
	defer loc_file.Close()

	local_scanner := bufio.NewScanner(loc_file)
	// Read each line and split it into fields based on spaces.
		for local_scanner.Scan() {
		line := local_scanner.Text()
		local_fields := strings.Split(line, " ")

		// Convert the string fields to float64 values.
		var floatFields [] float64
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
	measurements := make([]Measurement, len(local_data))
		for i, line := range local_data {
			measurements[i].X = line[0]
			measurements[i].Y = - line[1]
	}

		for i := range measurements {measurements[i].Z = math.Mod(measurements[i].X*frequency, 1.0); measurements[i].X = math.Mod(measurements[i].X*frequency, 2.0)}

		sort.Sort(ByZ(measurements))


		//Plot the measurements
		plot_data := make(plotter.XYs, len(local_data))
	for i := range local_data {
		plot_data[i].X = measurements[i].X
		plot_data[i].Y = measurements[i].Y
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

	plt.X.Min = 0
	plt.X.Max = 2

	//align the phase


	if isEclipsing {plt.X.Max = 1; for i := range plot_data {plot_data[i].X *= 0.5}} //fix the phase for eclipsing binaries

	plt.X.Label.Text = "Phase"
	plt.Y.Label.Text = "Magnitude"

	plt.Add(s)

	// Customize the tick labels with a custom formatter
	plt.Y.Tick.Marker = customYTicks{}
	plt.X.Tick.Marker = customXTicks{}


	// Set the plot size in pixels (width x height).
	widthPixels, heightPixels := 600, 300

	// Save the plot to a file with the specified size in pixels.
	err = plt.Save(vg.Length(widthPixels), vg.Length(heightPixels), plotname)
	if err != nil {
		panic(err)
	}
}
