package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"

	"github.com/davecheney/mdns"
)

var defaultHostname string
var hostname string
var ipv4Addresses []string
var ipv6Addresses []string

func mustPublish(record string) {
	err := mdns.Publish(record)
	if err != nil {
		log.Fatalf(`Unable to publish record "%s": %s`, record, err)
	}
}

func publishARecords(hostname string, ipv4Addresses []string) {
	for _, address := range ipv4Addresses {
		mustPublish(fmt.Sprintf("%s.local. 60 IN A %s", hostname, address))
	}
}

func publishAAAARecords(hostname string, ipv6Addresses []string) {
	for _, address := range ipv6Addresses {
		mustPublish(fmt.Sprintf("%s.local. 60 IN AAAA %s", hostname, address))
	}
}

func obtainIpAddresses() (ipv4Addresses []string, ipv6Addresses []string) {
	interfaces, err := net.Interfaces()
	if err != nil {
		log.Fatalf("Unable to obtain network interfaces: %s", err)
	}

	for _, iface := range interfaces {
		if (iface.Flags & net.FlagUp == 0) || (iface.Flags & net.FlagLoopback != 0) {
			continue
		}

		addresses, err := iface.Addrs()
		if err != nil {
			log.Printf(`WARNING: Unable to obtain IP addresses for interface "%s"`, iface.Name)
			continue
		}

		if len(addresses) == 0 {
			log.Printf(`WARNING: interface "%s" has no IP addresses`, iface.Name)
			continue
		}

		for _, address := range addresses {
			ip, _, err := net.ParseCIDR(address.String())
			if err != nil {
				log.Printf(`WARNING: Unable to parse IP address "%s"`, address.String())
				continue
			}

			if ip.To4() != nil {
				ipv4Addresses = append(ipv4Addresses, ip.String())
			} else {
				ipv6Addresses = append(ipv6Addresses, ip.String())
			}
		}

		break
	}

	return
}

func obtainHostname() string {
	hostname, err := os.Hostname()
	if err != nil {
		log.Fatalf("Unable to determine hostname: %s", err)
	}

	if strings.ContainsRune(hostname, '.') {
		hostname = strings.Split(hostname, ".")[0]
	}

	if hostname == "localhost" {
		hostname = defaultHostname
	}

	return hostname
}

func publishRecords() {
	ipv4Addresses, ipv6Addresses = obtainIpAddresses()

	hostname = obtainHostname()

	publishARecords(hostname, ipv4Addresses)
	publishAAAARecords(hostname, ipv6Addresses)
}

func main() {
	if len(os.Args) != 2 {
		log.Fatalf("Usage: %s <default hostname>", os.Args[0])
	}

	defaultHostname = os.Args[1]

	publishRecords()

	log.Printf("Publishing %s.local -> %v with 60-second TTL", hostname, append(ipv4Addresses, ipv6Addresses...))

	ticker := time.NewTicker(1 * time.Minute)
	for _ = range ticker.C {
		publishRecords()
	}
}
