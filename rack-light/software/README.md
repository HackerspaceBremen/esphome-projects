# How to compile

This software uses the [esphome](https://esphome.io/) framework. Install esphome and create a `secrets.yaml` with this content (set the required secrets):

```yaml
wifi_networks:
  - ssid: "SSID_1"
    password: "PSK_1"
    priority: 3
  - ssid: "SSID_2"
    password: "PSK_2"
    priority: 2

ap_password: "Fallback AP PSK"
```

Compile and upload the firmware:

```
$ esphome run rack-light.yaml
```
