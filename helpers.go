package main

import (
	"math/rand"
	"regexp"
	"strconv"
	"strings"
	"time"
)

// Validates raw arduino output against regex pattern and few other conditions.
func outputIsValid(s string, re *regexp.Regexp) bool {
	if len(s) > 168 && len(s) < 215 {
		if s[:4] == "V00=" &&
			strings.HasSuffix(s, ";") &&
			len(re.FindAll([]byte(s), -1)) >= 28 {
			// log.Output(1, s)
			return true
		}
	}
	return false
}

// Transforms output like "V00=0;V01=0;V02=0; ... S01=00;PUMP=0;" to map, for convenient writing to influxdb.
func outputToMap(s string) map[string]interface{} {
	println("STRINGAS: ", s)
	res := make(map[string]interface{})
	splitted_s := strings.Split(s, ";")
	for _, i := range splitted_s[:len(splitted_s)-1] {
		splitted_i := strings.Split(i, "=")
		// hex -> dec
		if strings.HasPrefix(i, "V") {
			num, err := strconv.ParseInt(splitted_i[1], 16, 64)
			check(err)
			res[splitted_i[0]] = num
		} else {
			number, err := strconv.ParseInt(splitted_i[1], 10, 64)
			check(err)
			res[splitted_i[0]] = number
		}
	}
	return res
}

// checks if string is in slice
func stringInSlice(a string, list []string) bool {
	for _, b := range list {
		if b == a {
			return true
		}
	}
	return false
}

// validates provided url value
func URLValueValid(p string, v string) bool {
	if len(v) > 0 {
		valueToInt, err := strconv.ParseInt(v, 10, 64)
		if err != nil {
			return false
		}
		if stringInSlice(p, VxxParams) && valueToInt >= 0 && valueToInt < 256 {
			return true
		} else if stringInSlice(p, TxxParams) && valueToInt >= 0 && valueToInt < 1000 {
			return true
		}
	} else if stringInSlice(p, pumpParams) {
		return true
	}
	return false
}

// validates provided URL param
func URLParamValid(s string) bool {
	if stringInSlice(s, VxxParams) ||
		stringInSlice(s, TxxParams) ||
		stringInSlice(s, pumpParams) {
		return true
	}
	return false
}

func check(err error) {
	if err != nil {
		panic(err.Error())
	}
}

func randInt(min int, max int) int {
	rand.Seed(time.Now().UTC().UnixNano())
	return min + rand.Intn(max-min)
}

func contains(s []int, e int) bool {
	for _, a := range s {
		if a == e {
			return true
		}
	}
	return false
}

func APIRules() string {
	text := `This is an API part of middleware between graphitizer microcontroller and user interface.
API sends commands to microcontroller through HTTP GET requests.

SET request examples:
http://127.0.0.1:9999/set?param=V00&value=255
http://127.0.0.1:9999/set?param=T01&value=80
http://127.0.0.1:9999/set?param=PUMP_OFF

GET_ALL:
http://127.0.0.1:9999/getall
`
	return text
}
