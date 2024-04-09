# MLR003 simulator
This program simulates a MLR003 micropelt thermostatic valve. 

## Payload decoder
- 1st byte : Setpoint. Default value 20°C. Set by downlink.
- 2de byte : Ambient temperature. From 15°C to 25°C up and down.

You can use this uplink payload decoder in your favorite Network Server:

```plaintext
function decodeUplink(input) {
  return {
    data: {
      setpoint: input.bytes[0],
      temperature: input.bytes[1]
    },
    warnings: [],
    errors: []
  };
}
```

## Changing the setpoint
The Setpoint can be overwritten using a 1 byte downlink.
