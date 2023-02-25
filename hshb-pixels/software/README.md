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
$ esphome run hshb-pixels.yaml
```

# References

- [Wave GIF (CC0), modified](https://opengameart.org/content/cartoon-waves-animated)
- [CG Pixel 3x5 font (CC0)](https://fontstruct.com/fontstructions/show/1404325/cg-pixel-4x5-2)
