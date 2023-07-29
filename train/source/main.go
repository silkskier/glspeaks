
package main

import (
    "fmt"
    "os"


)


func main() {
	train := false
	generate := false

	if os.Args[1] == "-t" || os.Args[1] == "--train" {train = true}
	if os.Args[1] == "-g" || os.Args[1] == "--generate" {generate = true}

	input := os.Args[len(os.Args)-1]
	fmt.Println(input)

	if len(os.Args) < 2 {
	fmt.Println("Usage: glspeaksplot-dev [arguments] <data>")
	return
	}

	if train == false && generate == false {panic("No action selected.")}


	if train{train_forest("datasets/iris.csv")}

}
