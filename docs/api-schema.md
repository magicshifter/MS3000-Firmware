api urls:

/schema(.json)
  returns json object:
  {
    hw-id: "MMA8452Q", // string of hardware id
    versions: [1], // array of supported versions
    init: ["settings/server"], // array of settings urls to request in clients
    modes: { // this object describes the modi
      clock: {
        routes: [
          {
            path: "time",
            args: [
              t: "number",
            ]
          }
        ]
      }
    }
  }
  
root path of api:
/${path}${version}/

example paths:
/{hw-id}/v{version}/{modename}/function
eg
/MMA8452Q/v1/clock/time
/MMA8452Q/v1/pov/upload

