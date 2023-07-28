package main

import (
	"bufio"
	"math"
	"fmt"
	"os"
	"strings"
	"strconv"
	//"sort"

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

type LocalAvg struct {
  sum float64
  number int
}

func calculatePhaseShift(arr []float64) (pulsePhaseShift float64, eclipsePhaseShift float64, diff float64, eclipsing bool) {
	var (
		locMinIndex, locMaxIndex, NoTop25 int
		minValue, maxValue = arr[0], arr[0]
	)

	for i, val := range arr {
		if val < minValue {
			locMinIndex = i
			minValue = val
		}
		if val > maxValue {
			locMaxIndex = i
			maxValue = val
		}
	}

	eclipsing = false
	NoTop25 = 0

	for _, val := range arr {if val > ((maxValue * 0.75) + (minValue * 0.25)) {NoTop25++}}
	if (NoTop25 * 2) >= len(arr) {eclipsing = true}

	diff = maxValue - minValue
	return float64(locMaxIndex) / float64(len(arr)), math.Mod(float64(locMinIndex + (len(arr) / 4)) / float64(len(arr)), 1.), diff, eclipsing
}


type customYTicks struct{}
func (customYTicks) Ticks(min, max float64) [] plot.Tick {
	ticks := make([]plot.Tick, 0)

	// Calculate the desired tick spacing based on the range of Y-axis values
	rangeY := max - min
	tickSpacing := rangeY * 0.124969482421875 // Eight ticks between min and max

	for v := min; v <= max; v += tickSpacing {
		label := fmt.Sprintf("%.2f", - v)
		if rangeY < 0.1 {label = fmt.Sprintf("%.2f", - v)}
		tick := plot.Tick{Value: v, Label: label}
		ticks = append(ticks, tick)
	}
	return ticks}


type customXTicks struct{}
func (customXTicks) Ticks(min, max float64) [] plot.Tick {
	ticks := make([]plot.Tick, 0)

	// Calculate the desired tick spacing based on the range of Y-axis values
	rangeX := max - min
	tickSpacing := rangeX * 0.124969482421875 // Eight ticks between min and max

	for v := min; v <= max; v += tickSpacing {
		var label string
		if rangeX < 1.1 {label = fmt.Sprintf("%.3f", v)
		} else {label = fmt.Sprintf("%.2f", v)}
		tick := plot.Tick{Value: v, Label: label}
		ticks = append(ticks, tick)
	}
	return ticks}


func calculateClosestSum(avgArray []LocalAvg) []LocalAvg {
	avgArray2 := make([]LocalAvg, len(avgArray))

	for i := range avgArray {
		prevIndex := (i - 1 + len(avgArray)) % len(avgArray)
		nextIndex := (i + 1) % len(avgArray)

		avgArray2[i].sum = avgArray[prevIndex].sum + avgArray[i].sum + avgArray[nextIndex].sum
		avgArray2[i].number = avgArray[prevIndex].number + avgArray[i].number + avgArray[nextIndex].number
	}

	return avgArray2
}



func generatePlot(file string, outputDir string, frequency float64, match_strength string, photometry_location string){

	plotname := outputDir + "/" + match_strength + "_" + file + ".png"

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

		for i := range measurements {measurements[i].Z = math.Mod(measurements[i].X*frequency, 1.0)}

		//sort.Sort(ByZ(measurements))

		// Create the array of 128 LocalAvg structs
		partialAvgs := make([]LocalAvg, 128)

		for _, m := range measurements {
		index := int(m.Z * 128) // Calculate the index corresponding to Z range

		// Update the sum and number of measurements for the corresponding Z range
		partialAvgs[index].sum += m.Y
		partialAvgs[index].number++}

		valueArray := calculateClosestSum(partialAvgs)

	// Print the array of LocalAvg structs
	for i := range valueArray {if valueArray[i].number == 0 {valueArray[i].sum = math.NaN(); valueArray[i].number = 1}}
	//for i, avg := range valueArray {fmt.Printf("Z range %.3f-%.3f: Sum=%.2f, Number of Measurements=%d\n", float64(i)/128.0, float64(i+1)/128.0, avg.sum, avg.number)}

	averages := make([] float64, 128)
	for i := range averages {averages[i] = valueArray[i].sum / float64(valueArray[i].number)}
	//for i := range averages {fmt.Println(averages[i])}

	pulsePhaseShift, eclipsePhaseShift, amplitude, eclipsing := calculatePhaseShift(averages)

	//align the phase
	if eclipsing{fmt.Println("Eclipse or sth", pulsePhaseShift, eclipsePhaseShift, amplitude)}


		//Plot the measurements
		plot_data := make(plotter.XYs, len(local_data))
	for i := range local_data {
		plot_data[i].X = (math.Mod((measurements[i].X*frequency - pulsePhaseShift), 2.0)) // - phaseshift
		plot_data[i].Y = measurements[i].Y}

	if eclipsing {for i := range plot_data {plot_data[i].X *= 0.5}}


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

	if eclipsing {plt.X.Max = 1} //fix the phase for eclipsing binaries ;

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
