
package main

import (
    "fmt"
    "math/rand"
    randomforest "github.com/malaschitz/randomForest"

    "encoding/gob"
    "os"
)

func main() {

	xData := [][]float64{}
	yData := []int{}
	for i := 0; i < 10000; i++ {
		x := []float64{rand.Float64(), rand.Float64(), rand.Float64(), rand.Float64()}
		y := int(x[0] + x[1] + x[2] + x[3])
		xData = append(xData, x)
		yData = append(yData, y)
	}

	/*
	// random forest
	forest := randomforest.Forest{}
	forest.Data = randomforest.ForestData{X: xData, Class: yData}
	forest.Train(1000)
	*/

	//deep forward thinking random forest
	f := randomforest.Forest{}
	f.Data = randomforest.ForestData{X: xData, Class: yData}
	dForest := f.BuildDeepForest()
	dForest.Train(50, 10, 1000)

	dataFile, _ := os.Create("forest.drf")
	dataEncoder := gob.NewEncoder(dataFile)
	dataEncoder.Encode(dForest)
	dataFile.Close()


	//test
	fmt.Println("Vote", dForest.Vote([]float64{0.1, 0.1, 0.1, 0.1}))
	fmt.Println("Vote", dForest.Vote([]float64{0.9, 0.9, 0.9, 0.9}))
}
