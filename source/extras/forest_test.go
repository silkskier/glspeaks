package main

import (
	"os"
	"fmt"
	"math/rand"
	"encoding/gob"

	"github.com/malaschitz/randomForest"
)


// Function to save the deep forest to a binary file

func saveDeepForestToFile(deepForest randomforest.DeepForest, filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	encoder := gob.NewEncoder(file)
	err = encoder.Encode(deepForest)
	if err != nil {
		return err
	}

	return nil
}

// Function to load the deep forest from a binary file
func loadDeepForestFromFile(filename string) (randomforest.DeepForest, error) {
	var deepForest randomforest.DeepForest

	file, err := os.Open(filename)
	if err != nil {
		return deepForest, err
	}
	defer file.Close()

	decoder := gob.NewDecoder(file)
	err = decoder.Decode(&deepForest)
	if err != nil {
		return deepForest, err
	}

	return deepForest, nil
}
/* 
//ordinary random forest
func main() {
	xData := [][]float64{}
	yData := []int{}
	for i := 0; i < 1000; i++ {
		x := []float64{rand.Float64(), rand.Float64(), rand.Float64(), rand.Float64()}
		y := int(x[0] + x[1] + x[2] + x[3])
		xData = append(xData, x)
		yData = append(yData, y)
	}
	forest := randomforest.Forest{}
	forest.Data = randomforest.ForestData{X: xData, Class: yData}
	forest.Train(1000)
	//test
	fmt.Println("Vote", forest.Vote([]float64{0.1, 0.1, 0.1, 0.1}))
	fmt.Println("Vote", forest.Vote([]float64{0.9, 0.9, 0.9, 0.9}))
}
*/
//DFTRF with save/load functions test
func main() {
	xData := [][]float64{}
	yData := []int{}
	for i := 0; i < 1000; i++ {
		x := []float64{rand.Float64(), rand.Float64(), rand.Float64(), rand.Float64()}
		y := int(x[0] + x[1] + x[2] + x[3])
		xData = append(xData, x)
		yData = append(yData, y)
	}

	forest := randomforest.Forest{} // Create a base forest
	forest.Data = randomforest.ForestData{X: xData, Class: yData}
	forest.Train(1000) // Train the base forest

	deepForest := forest.BuildDeepForest() // Build a deep forest based on the base forest
	deepForest.Train(20, 100, 1000) // Train the deep forest

	// Test
	testData := []float64{0.1, 0.1, 0.1, 0.1}
	fmt.Println("Vote", deepForest.Vote(testData))

	testData = []float64{0.9, 0.9, 0.9, 0.9}
	fmt.Println("Vote", deepForest.Vote(testData))



	//save the generated forest

	deepForestFile := "deep_forest.gob"
	err := saveDeepForestToFile(deepForest, deepForestFile)
	if err != nil {
		fmt.Println("Error saving deep forest:", err)
		return
	}

	fmt.Println("Deep forest saved to", deepForestFile)

	// Loading the deep forest from the binary file
	loadedDeepForest, err := loadDeepForestFromFile(deepForestFile)
	if err != nil {
		fmt.Println("Error loading deep forest:", err)
		return
	}

	// You can now work with the loadedDeepForest object as needed
	fmt.Println("Loaded deep forest:", loadedDeepForest)
}
