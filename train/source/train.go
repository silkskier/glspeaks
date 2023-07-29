package main

import(
	"fmt"
	//"os"

	"github.com/sjwhitworth/golearn/base"
	"github.com/sjwhitworth/golearn/ensemble"
	"github.com/sjwhitworth/golearn/evaluation")

func train_forest(dataset string) {

        // Load in a dataset, with headers. Header attributes will be stored.
    // Think of instances as a Data Frame structure in R or Pandas.
    // You can also create instances from scratch.
    rawData, err := base.ParseCSVToInstances(dataset, false)
    if err != nil {
        panic(err)
    }

    // Print a pleasant summary of your data.
    fmt.Println(rawData)

    // Initialize a new Random Forest classifier
    forest := ensemble.NewRandomForest(100, 4) // Using 100 trees and 4 features per split

    // Do a training-test split
    trainData, testData := base.InstancesTrainTestSplit(rawData, 0.50)

    // Fit the Random Forest classifier
    forest.Fit(trainData)

    // Make predictions using the Random Forest classifier
    predictions, err := forest.Predict(testData)
    if err != nil {
        panic(err)
    }

    // Print precision/recall metrics
    confusionMat, err := evaluation.GetConfusionMatrix(testData, predictions)
    if err != nil {
        panic(fmt.Sprintf("Unable to get confusion matrix: %s", err.Error()))
    }
    fmt.Println(evaluation.GetSummary(confusionMat))
}
