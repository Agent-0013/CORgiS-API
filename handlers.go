package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"strconv"
	"strings"
	"time"
)

func SetHandler(w http.ResponseWriter, r *http.Request) {
	start := time.Now()

	param := r.URL.Query().Get("param")
	value := r.URL.Query().Get("value")

	// make sure, that param & value combination is valid
	if !URLParamValid(param) {
		w.Write([]byte("error: incorrect param!"))
		log.Output(1, "Invalid request!")
		return
	}
	if !URLValueValid(param, value) {
		w.Write([]byte("error: incorrect value!"))
		log.Output(1, "Invalid request!")
		return
	}

	// format and send a command to the device
	command := ""
	if strings.HasPrefix(param, "PUMP") {
		command = "<" + param + ";>"
		_, err := arduino.Write([]byte(command))
		check(err)
		log.Output(1, fmt.Sprintf("Command sent: %v", command))
	} else {
		command = "<SET_" + param + "=" + value + ";>"
		_, err := arduino.Write([]byte(command))
		if err != nil {
			w.Write([]byte("error: could not send a command to device, check if connected!"))
		}
		log.Output(1, fmt.Sprintf("Command sent: %v", command))
	}

	time.Sleep(30 * time.Millisecond)

	// format and send a response depending on parameter
	if stringInSlice(param, VxxParams) {
		valueToInt, err := strconv.ParseInt(value, 10, 64)
		check(err)
		for {
			answer := outputToMap(singleOutputRead())
			if answer[param] == valueToInt {
				jsonString, err := json.Marshal(answer)
				check(err)
				w.Write([]byte(jsonString))
				log.Output(1, "Valid response received.")
				break
			} else {
				logout := fmt.Sprintf("Response FAILED, %v != %v! Reading again..", answer[param], value)
				log.Output(1, logout)
				time.Sleep(20 * time.Millisecond)
			}
		}
	} else if stringInSlice(param, pumpParams) {
		for {
			answer := outputToMap(singleOutputRead())
			if param == "PUMP_ON" && answer["PUMP"] == int64(1) {
				jsonString, err := json.Marshal(answer)
				check(err)
				w.Write([]byte(jsonString))
				log.Output(1, "Valid response received.")
				break
			} else if param == "PUMP_OFF" && answer["PUMP"] == int64(0) {
				jsonString, err := json.Marshal(answer)
				check(err)
				w.Write([]byte(jsonString))
				log.Output(1, "Valid response received.")
				break
			} else {
				logout := fmt.Sprintf("Response FAILED! Param = '%v', pump value = '%v'", param, answer["PUMP"])
				log.Output(1, logout)
				time.Sleep(80 * time.Millisecond)
			}
		}
		// temperature is inertical, so it doesn't really need imediate response
	} else if stringInSlice(param, TxxParams) {
		answer := outputToMap(singleOutputRead())
		jsonString, err := json.Marshal(answer)
		check(err)
		w.Write([]byte(jsonString))
		log.Output(1, "Valid response received.")
	} else {
		w.Write([]byte("error: something unexpected happened"))
	}
	finish := time.Since(start)
	log.Output(1, fmt.Sprintf("Response took %v", finish))
}

func GetHandler(w http.ResponseWriter, r *http.Request) {
	answer := outputToMap(singleOutputRead())
	jsonString, err := json.Marshal(answer)
	check(err)
	w.Write([]byte(jsonString))
	log.Output(1, "Valid response received.")
}

// Accepts JSON string from request, starts a process routine
func StartHandler(w http.ResponseWriter, r *http.Request) {
	c := make(chan int)
	decoder := json.NewDecoder(r.Body)
	body := make([]map[string]interface{}, 1)
	err := decoder.Decode(&body)
	if err != nil {
		panic(err)
	}

	if validateJSONMap(body) {
		println("JSON OK.")
	} else {
		println("Faulty JSON!")
	}

	go process(c, body)
	cToString := strconv.Itoa(<-c)
	w.Write([]byte(cToString))

}

func StopHandler(w http.ResponseWriter, r *http.Request) {
	id := r.URL.Query().Get("id")
	idInt, err := strconv.Atoi(id)
	if err != nil {
		panic(err)
	}
	ids = append(ids, idInt)
	fmt.Printf("%v\n", ids)
}

func RootHandler(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte(APIRules()))
}
